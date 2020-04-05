#ifndef MUDUO_NET_SOCKET_H
#define MUDUO_NET_SOCKET_H

#include "muduo/base/noncopyable.h"

// struct tcp_info is in <netinet/tcp.h>
struct tcp_info;

namespace muduo
{
// tcp networking
namespace net
{
// Wrapper of socket file descriptor
class Socket : noncopyable
{
public:
	explicit Socket(int sockfd)
		: sockfd_(sockfd)
	{
	}
	// closes the socket file descriptor
	~Socket();

	int fd() const { return sockfd_; }
	bool getTcpInfo(struct tcp_info*) const;
	bool getTcpInfoString(char* buf, int len) const;

	// abort if address in use
	void bindAddress(const InetAddress& localaddr);
	void listen();

	int accept(InetAddress* peeraddr);
	void shutdownWrite();

	// TCP_NODELAY
	void setTcpNoDelay(bool on);
	// SO_REUSEADDR
	void setReuseAddr(bool on);
	// SO_REUSEPORT
	void setReusePort(bool on);
	// SO_KEEPALIVE
	void setKeepAlive(bool on);
	
private:
	const int sockfd_;
}; // class Socket

} // namespace net

} // namespace muduo

#endif // MUDUO_NET_SOCKET_H
