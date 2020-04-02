#ifndef MUDUO_BASE_CONDITION_H
#define MUDUO_BASE_CONDITION_H

#include "muduo/base/Mutex.h"
#include <pthread.h>

namespace muduo
{

class Condition : noncopyable
{
public:
	explicit Condition(MutexLock& mutex)
		: mutex_(mutex)
	{
		int ret = pthread_cond_init(&pcond_, NULL);
		assert(ret == 0);
	}

	~Condition()
	{
		int ret = pthread_cond_destroy(&pcond_);
		assert(ret == 0);
	}

	void wait()
	{
		MutexLock::UnassignGuard ug(mutex_);
		int ret = pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()); // 将线程添加到条件变量
		assert(ret == 0);
	}

	bool waitForSeconds(double seconds);

	void notify()
	{
		int ret = pthread_cond_signal(&pcond_);
		assert(ret == 0);
	}

	void notifyAll()
	{
		int ret = pthread_cond_broadcast(&pcond_);
		assert(ret == 0);
	}

private:
	MutexLock& 		mutex_;
	pthread_cond_t	pcond_;
}; // class Condition

} // namespace muduo

#endif // MUDUO_BASE_CONDITION_H
