#include "muduo/base/Thread.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/EventLoop.h"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace muduo;
using namespace muduo::net;

void newconn(const TcpConnectionPtr& conn)
{
	cout << "newconn: " << conn->getTcpInfoString();
}

void newmessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp t)
{
	cout << "newmessage: " << "readableBytes = " << buf->readableBytes() << ", " << buf->retrieveAllAsString();
	conn->send(Timestamp::now().toString() + '\n');
}

void threadInit(EventLoop*) 
{
	cout << "Just empty threadInit";
}

void writeComplete(const TcpConnectionPtr& conn)
{
	cout << conn->name() << " " << conn->peerAddress().toIpPort() << " writeComplete";
}

void cb()
{
	cout << "cb() is calling!";
}

int main() {

	Logger::setLogLevel(Logger::DEBUG);
	InetAddress listenAddr("0.0.0.0", 9999);
	EventLoop loop;
	TcpServer server(&loop, listenAddr, "TcpServer");
	server.setConnectionCallback(newconn);
	server.setMessageCallback(newmessage);
	server.setThreadNum(3);
	server.setThreadInitCallback(threadInit);
	server.setWriteCompleteCallback(writeComplete);
	server.start();

	loop.loop();

	return 0;
}
