#include "muduo/net/Socket.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/SocketsOps.h"
#include "muduo/base/Logging.h"

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

Socket::~Socket()
{
	sockets::close(sockfd_);
}

bool Socket::getTcpInfo(struct tcp_info* tcpi) const
{
	socklen_t len = sizeof(*tcpi);
	memZero(tcpi, len);
	return ::getsockopt(sockfd_, SOL_TCP, TCP_INFO, tcpi, &len) == 0;
}

bool Socket::getTcpInfoString(char* buf, int len) const
{
	struct tcp_info tcpi;
	bool ok = getTcpInfo(&tcpi);
	if (ok) {
		snprintf(buf, len, "unrecovered=%u "
				"rto=%u ato=%u snd_mss=%u rcv_mss=%u "
				"lost=%u retrans=%u rtt=%u rttvar=%u "
				"sshthresh=%u cwnd=%u total_retrans=%u ",
				tcpi.tcpi_retransmits,	// 重传数, 当前待重传的包数, 重传完毕后清零
				tcpi.tcpi_rto,			// 重传超时时间, 和RTT有关,RTT越大,rto越大
				tcpi.tcpi_ato,			// 延时确认的估值
				tcpi.tcpi_snd_mss,		// 本端的mss
				tcpi.tcpi_rcv_mss,		// 对端的mss
				tcpi.tcpi_lost,			// 本段发送出去丢失的报文数, 重传后清零
				tcpi.tcpi_retrans,		// 重传且为确认的数据段数
				tcpi.tcpi_rtt,			// 往返时间 round trip time
				tcpi.tcpi_rttvar,		// 描述RTT的平均偏差, 值越大, 说明抖动越大
				tcpi.tcpi_snd_ssthresh,	// 拥塞控制慢开始阈值
				tcpi.tcpi_snd_cwnd,		// 拥塞控制窗口大小
				tcpi.tcpi_total_retrans);	// 统计总重传包数, 持续增长
	}
	return ok;
}

void Socket::bindAddress(const InetAddress& addr)
{
	sockets::bindOrDie(sockfd_, addr.getSockAddr());
}

void Socket::listen()
{
	sockets::listenOrDie(sockfd_);
}

int Socket::accept(InetAddress* peeraddr)
{
	struct sockaddr_in6 addr;
	memZero(&addr, sizeof addr);
	int connfd = sockets::accept(sockfd_, &addr);
	if (connfd >= 0) {
		peeraddr->setSockAddrInet6(addr);
	}
	return connfd;
}

void Socket::shutdownWrite()
{
	sockets::shutdownWrite(sockfd_);
}

void Socket::setTcpNoDelay(bool on)
{
	int optval = on ? 1 : 0;
	::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReuseAddr(bool on)
{
	int optval = on ? 1 : 0;
	::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReusePort(bool on)
{
#ifdef SO_REUSEPORT
	int optval = on ? 1 : 0;
	int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof optval));
	if (ret < 0 && on) {
		LOG_SYSERR << "SO_REUSEPORT failed.";
	}
#else
	if (on) {
		LOG_ERROR << "SO_REUSEPORT is not supported.";
	}	
#endif
}

void Socket::setKeepAlive(bool on)
{
	int optval = on ? 1 : 0;
	::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &optval, static_cast<socklen_t>(sizeof optval));
}
