#ifndef MUDUO_NET_TCPSERVER_H
#define MUDUO_NET_TCPSERVER_H

#include "muduo/base/Atomic.h"
#include "muduo/base/Types.h"
#include "muduo/net/TcpConnection.h"

#include <map>

namespace muduo
{
namespace net
{

class Acceptor;
class EventLoop;
class EventLoopThreadPool;

// Tcp server, supports single threaded and thread-pool models.
class TcpServer : noncopyable
{
public:
	typedef std::function<void (EventLoop*)> ThreadInitCallback;
	enum Option
	{
		kNoReusePort,
		kReusePort,
	};

	TcpServer(EventLoop* loop,
			  const InetAddress& listenAddr,
			  const string& nameArg,
			  Option option = kNoReusePort);
	~TcpServer();
	const string& ipPort() const { return ipPort_; }
	const string& name() const { return name_; }
	EventLoop* getLoop() const { return loop_; }
	/// Set the number of threads for handling input.
	///
	/// Always accepts new connection in loop's thread.
	/// Must be called before @c start
	/// @param numThreads
	/// - 0 means all I/O in loop's thread, no thread will created.
	///   this is the default value.
	/// - 1 means all I/O in another thread.
	/// - N means a thread pool with N threads, new connections
	///   are assigned on a round-robin basis.	
	void setThreadNum(int numThreads);
	void setThreadInitCallback(const ThreadInitCallback& cb) { threadInitCallback_ = cb; }
	std::shared_ptr<EventLoopThreadPool> threadPool() { return threadPool_; }
	
	void start();
	void setConnectionCallback(const ConnectionCallback& cb) { connectionCallback_ = cb; }
	void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }
	void setWriteCompleteCallback(const WriteCompleteCallback& cb) { writeCompleteCallback_ = cb; }

private:

	void newConnection(int sockfd, const InetAddress& peerAddr);
	void removeConnection(const TcpConnectionPtr& conn);
	void removeConnectionInLoop(const TcpConnectionPtr& conn);
	typedef std::map<string, TcpConnectionPtr> ConnectionMap;
	
	EventLoop* loop_; // acceptor loop
	const string ipPort_;
	const string name_;
	std::unique_ptr<Acceptor> acceptor_;
	std::shared_ptr<EventLoopThreadPool> threadPool_;
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
	WriteCompleteCallback writeCompleteCallback_;
	ThreadInitCallback threadInitCallback_;
	AtomicInt32 started_;
	
	int nextConnId_;
	ConnectionMap connections_;
}; // class TcpServer

} // namespace net


} // namespace muduo

#endif // MUDUO_NET_TCPSERVER_H
