#ifndef MUDUO_NET_EVENTLOOPTHREADPOOL_H
#define MUDUO_NET_EVENTLOOPTHREADPOOL_H

#include "muduo/base/noncopyable.h"
#include "muduo/base/Types.h"

#include <functional>
#include <memory>
#include <vector>

namespace muduo
{
namespace net
{
class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : noncopyable
{
public:
	typedef std::function<void(EventLoop*)> ThreadInitCallback;
	EventLoopThreadPool(EventLoop* baseLoop, const string& nameArg);
	~EventLoopThreadPool();
	void setThreadNum(int numThreads) { numThreads_ = numThreads; }
	void start(const ThreadInitCallback& cb = ThreadInitCallback());

	EventLoop* getNextLoop();
	// same hash code return same EventLoop
	EventLoop* getLoopForHash(size_t hashCode);
	std::vector<EventLoop*> getAllLoops();

	bool started() const { return started_; }
	const string& name() const { return name_; }

private:
	EventLoop* baseLoop_;
	string name_;
	bool started_;
	int numThreads_;
	int next_;
	std::vector<std::unique_ptr<EventLoopThread>> threads_;
	std::vector<EventLoop*> loops_;

}; // class EventLoopThreadPool
} // namespace net
} // namespace muduo

#endif // MUDUO_NET_EVENTLOOPTHREADPOOL_H
