#include <iostream>
#include "muduo/net/TcpClient.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"

using namespace muduo;
using namespace muduo::net;
using namespace std;

void connCb(const TcpConnectionPtr& ptr)
{
	if (ptr->connected()) {
		ptr->send("hello server!\n");
	}
}

void newMessage(const TcpConnectionPtr& ptr, Buffer* buf, Timestamp time)
{
	cout << "receive: " <<  buf->retrieveAllAsString() << endl;
}

int main()
{
	EventLoop loop;
	InetAddress serverAddr("127.0.0.1", 9999);
	TcpClient client(&loop, serverAddr, "test_TcpClient");
	client.setMessageCallback(newMessage);
	client.setConnectionCallback(connCb);
	client.connect();
	loop.loop();

	return 0;
}
