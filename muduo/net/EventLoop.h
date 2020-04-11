#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include <atomic>
#include <functional>
#include <vector>
#include <boost/any.hpp>

#include "muduo/base/Mutex.h"
#include "muduo/base/CurrentThread.h"
#include "muduo/base/Timestamp.h"
#include "muduo/net/Callbacks.h"
#include "muduo/net/TimerId.h"

namespace muduo
{

namespace net{

class Channel;
class Poller;
class TimerQueue;

// Reactor, at most one per thread.
// This is an interface class, so don't expose too much details.
class EventLoop : noncopyable
{
public:
	typedef std::function<void()> Functor;
	EventLoop();
	~EventLoop();
	
	void loop();
	void quit();	
	
	Timestamp pollReturnTime() const { return pollReturnTime_; }
	int64_t iteration() const { return iteration_; }

	// Runs callback immediately in the loop thread.
	void runInLoop(Functor cb);
	// Queues callback in the loop thread.
	void queueInLoop(Functor cb);
	size_t queueSize() const;

	// Runs callback at time
	TimerId runAt(Timestamp time, TimerCallback cb);
	// Runs callback after delay seconds.
	TimerId runAfter(double delay, TimerCallback cb);
	// Runs callback every interval seconds.
	TimerId runEvery(double interval, TimerCallback cb);
	// Cancels the timer.
	void cancel(TimerId timerId);

	void wakeup();
	void updateChannel(Channel* channel);
	void removeChannel(Channel* channel);
	bool hasChannel(Channel* channel);

	void assertInLoopThread()
	{
		if (!isInLoopThread()) {
			abortNotInLoopThread();
		}
	}

	bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
	bool eventHandling() const { return eventHandling_; }
	
	void setContext(const boost::any& context) { context_ = context; }
	boost::any* getMutableContext() { return &context_; }

	static EventLoop* getEventLoopOfCurrentThread();

private:
	void abortNotInLoopThread();
	void handleRead(); // wake up
	void doPendingFunctors();
	void printActiveChannels() const;
	typedef std::vector<Channel*> ChannelList;

	bool looping_;
	std::atomic<bool> quit_;
	bool eventHandling_;
	bool callingPendingFunctors_;
	int64_t iteration_;
	const pid_t threadId_;
	Timestamp pollReturnTime_;
	std::unique_ptr<Poller> poller_;
	std::unique_ptr<TimerQueue> timerQueue_;
	int wakeupFd_;
	
	std::unique_ptr<Channel> wakeupChannel_;
	boost::any context_;

	ChannelList activeChannels_;
	Channel* currentActiveChannel_;

	mutable MutexLock mutex_;
	std::vector<Functor> pendingFunctors_;
};
} // namespace net
} // namespace muduo

#endif // MUDUO_NET_EVENTLOOP_H
