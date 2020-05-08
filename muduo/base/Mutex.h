#ifndef MUDUO_BASE_MUTEX_H
#define MUDUO_BASE_MUTEX_H

#include "muduo/base/CurrentThread.h"
#include "muduo/base/noncopyable.h"
#include <assert.h>
#include <pthread.h>

namespace muduo
{

class MutexLock : noncopyable
{
public:
	MutexLock()
		: holder_(0)
	{
		int ret = pthread_mutex_init(&mutex_, NULL);
		assert(ret == 0);
	}

	~MutexLock()
	{
		assert(holder_ == 0);
		int ret = pthread_mutex_destroy(&mutex_);
		assert(ret == 0);
	}

	// 加锁之后调用
	bool isLockedByThisThread() const
	{
		return holder_ == CurrentThread::tid();
	}

	void assertLocked()
	{
		assert(isLockedByThisThread());
	}

	void lock()
	{
		int ret = pthread_mutex_lock(&mutex_);
		assert(ret == 0);
		assignHolder();
	}

	void unlock()
	{
		unassignHolder();
		int ret = pthread_mutex_unlock(&mutex_);
		assert(ret == 0);
	}

	pthread_mutex_t* getPthreadMutex()
	{
		return &mutex_;
	}

private:
	friend class Condition; // 条件变量类用到锁

	class UnassignGuard : noncopyable // 为了实现Condition中的wait()
	{
	public:
		explicit UnassignGuard(MutexLock &owner)
			: owner_(owner)
		{
			owner_.unassignHolder();
		}

		~UnassignGuard()
		{
			owner_.assignHolder();
		}
	
	private:
		MutexLock& owner_;	
	}; // class UnassignGuard

	void unassignHolder()
	{
		holder_ = 0;
	}

	void assignHolder() 
	{
		holder_ = CurrentThread::tid();
	}
	
	pthread_mutex_t mutex_;
	pid_t holder_;
}; // MutexLock

// 对栈上的对象加锁
// RAII技法封装, 构造获取资源, 析构释放资源
class MutexLockGuard : noncopyable
{
public:
	explicit MutexLockGuard(MutexLock& mutex)
		: mutex_(mutex)
	{
		mutex_.lock();
	}
	
	~MutexLockGuard()
	{
		mutex_.unlock();
	}		

private:
	
	MutexLock& mutex_; // 这里用的是引用, 对象结束, mutex不会结束

}; // class MutexLockGuard 

} // namespace muduo

#define MutexLockGuard(x) error "Missing guard object name"

#endif
