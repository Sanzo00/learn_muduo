#include "muduo/net/InetAddress.h"
#include "muduo/net/Endian.h"
#include "muduo/net/SocketsOps.h"
#include "muduo/base/Logging.h"

#include <netdb.h>
#include <netinet/in.h>

// INADDR_ANY use (type)value casting.
#pragma GCC diagnostic ignored "-Wold-style-cast"
static const in_addr_t kInaddrAny = INADDR_ANY; // 绑定地址0.0.0.0, 能收到任意一块网卡连接
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK; // 绑定地址127.0.0.1, 只能收到127.0.0.1的连接请求
#pragma GCC diagnostic error "-Wold-style-cast"

using namespace muduo;
using namespace muduo::net;

static_assert(sizeof(InetAddress) == sizeof(struct sockaddr_in6), "InetAddress should be same size as sockaddr_in6");
static_assert(offsetof(sockaddr_in, sin_family) == 0, "sin_family offset 0");
static_assert(offsetof(sockaddr_in6, sin6_family) == 0, "sin6_family offset 0");
static_assert(offsetof(sockaddr_in, sin_port) == 2, "sin_port offset 2");
static_assert(offsetof(sockaddr_in6, sin6_port) == 2, "sin6_family offset 2");

InetAddress::InetAddress(uint16_t port, bool loopbackOnly, bool ipv6)
{
	static_assert(offsetof(InetAddress, addr6_) == 0, "addr6_ offset 0");
	static_assert(offsetof(InetAddress, addr_) == 0, "addr_ offset 0");
	if (ipv6) {
		memZero(&addr6_, sizeof addr6_);
		addr6_.sin6_family = AF_INET6;
		in6_addr ip = loopbackOnly ? in6addr_loopback : in6addr_any;
		addr6_.sin6_addr = ip;
		addr6_.sin6_port = sockets::hostToNetwork16(port);
	}else {
		memZero(&addr_, sizeof(addr_));
		addr_.sin_family = AF_INET;
		in_addr_t ip = loopbackOnly ? kInaddrLoopback : kInaddrAny;
		addr_.sin_addr.s_addr = sockets::hostToNetwork32(ip);
		addr_.sin_port = sockets::hostToNetwork16(port);
	}
}

InetAddress::InetAddress(StringArg ip, uint16_t port, bool ipv6)
{
	if (ipv6) {
		memZero(&addr6_, sizeof addr6_);
		sockets::fromIpPort(ip.c_str(), port, &addr6_);
	}else {
		memZero(&addr_, sizeof addr_);
		sockets::fromIpPort(ip.c_str(), port, &addr_);
	}
}

// sockaddr*转为字符串ip+port
string InetAddress::toIpPort() const
{
	char buf[64] = "";
	sockets::toIpPort(buf, sizeof buf, getSockAddr());
	return buf;
}
// sockaddr*转为字符串ip
string InetAddress::toIp() const
{
	char buf[64] = "";
	sockets::toIp(buf, sizeof buf, getSockAddr());
	return buf;
}

uint32_t InetAddress::ipNetEndian() const
{
	assert(family() == AF_INET);
	return addr_.sin_addr.s_addr;
}

uint16_t InetAddress::toPort() const
{
	return sockets::networkToHost16(portNetEndian());
}

static __thread char t_resolveBuffer[64 * 1024];

bool InetAddress::resolve(StringArg hostname, InetAddress* out)
{
/*	
 struct hostent { 
　　 char *h_name; 
　　 char **h_aliases; 
　　 int h_addrtype; 
　　 int h_length; 
　　 char **h_addr_list; 
};
*/
	assert(out != NULL);
	struct hostent hent;
	struct hostent* he = NULL;
	int herrno = 0;
	memZero(&hent, sizeof hent);

	int ret = gethostbyname_r(hostname.c_str(), &hent, t_resolveBuffer, sizeof t_resolveBuffer, &he, &herrno);
	if (ret == 0 && he != NULL) {
		assert(he->h_addrtype == AF_INET && he->h_length == sizeof(uint32_t));
		out->addr_.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
		return true;
	}else {
		if (ret) {
			LOG_SYSERR << "InetAddress::resolve";
		}
		return false;
	}
}

void InetAddress::setScopeId(uint32_t scope_id)
{
	if (family() == AF_INET6) {
		addr6_.sin6_scope_id = scope_id;
	}
}

// 在IPv6地址结构中（对应于IPv4的struct sockaddr_in），有一个我们非常陌生的字段scope_id
// 这个字段在我们使用链路本地地址来编程的时候是必须要使用的，这个字段表示我们需要选择接口ID。
// 为什么需要需要有这么一个字段，那是因为链路本地地址的特殊性，一个网络节点可以有多个网络接口，多个网络接口可以有相同的链路本地地址。
// 例如我们需要bind一个本地链路地址，这个时候就会有冲突，操作系统无法决策需要绑定的是哪个接口的本地链路地址。
// 又例如，如果我们在直连的2个主机之间直接用链路本地地址ping的话，会ping失败。
// 因此IPv6引入了scope_id来解决这个问题，scope_id指定了使用哪个网络接口。
