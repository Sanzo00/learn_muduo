#include "Poller.h"
#include "muduo/net/Channel.h"

using namespace muduo;
using namespace muduo::net;

Pller::Poller(EventLoop* loop)
	: ownerLoop_(loop)
{
}

Poller::~Poller() = default;

bool Poller::hasChannel(Channel* channel) const
{
	assertInLoopThread();
	ChannelMap::const_iterator it = channels.find(channel->fd());
	return it != channels.end() && it->second == channel;
}
