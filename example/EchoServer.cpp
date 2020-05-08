#include "muduo/base/Logging.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"

#include <iostream>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

void onConnection(const TcpConnectionPtr& conn) 
{
	LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
			 << conn->localAddress().toIpPort() << " is "
			 << (conn->connected() ? "UP" : "DOWN");
}

void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
{
	string msg(buf->retrieveAllAsString());
	LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, "
			 << "data received at " << time.toString();
	conn->send(msg);
}


int main()
{
	// Logger::setLogLevel(Logger::ERROR);
	LOG_INFO << "pid = " << getpid();
	
	EventLoop loop;
	InetAddress listenAddr(2020);

	TcpServer server(&loop, listenAddr, "EchoServer");
	server.setConnectionCallback(onConnection);
	server.setMessageCallback(onMessage);

	server.start();
	std::cout << "echo server listen at " << listenAddr.toIpPort() << std::endl; 

	loop.loop();

	return 0;
}
