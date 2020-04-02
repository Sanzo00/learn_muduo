#ifndef	MUDUO_BASE_ATOMIC_H
#define MUDUO_BASE_ATOMIC_H

#include "muduo/base/noncopyable.h"

#include <stdint.h>

namespace muduo
{

namespace detail
{

template<typename T>
class AtomicIntegerT : noncopyable
{
public:
	AtomicIntegerT() : value_(0) {}

	// 如果需要拷贝和赋值, 取消注释
	// AtomicIntegerT(const AtomicIntegerT& that) 
	// 	: value_(that.get()) 
	// {}

	// AtomicIntegerT& operator = (const AtomicIntegerT& that)
	// {
	// 	getAndSet(that.get());
	// 	return *this;
	// }

	// 原子比较和交换
	// __sync_val_compare_add_swap(type *ptr, type oldval, newval), 先判断*ptr是否和oldval相等, 如果相等将值设置为newval
	T get()
	{
		return __sync_val_compare_and_swap(&value_, 0, 0);
	}

	// 原子自增, value += x
	T getAndAdd(T x)
	{
		return __sync_fetch_and_add(&value_, x);
	}
	
	// 原子赋值, value = newValue
	T getAndSet(T newValue)
	{
		return __sync_lock_test_and_set(&value_, newValue);
	}

	T addAndGet(T x)
	{
		return getAndAdd(x) + x;
	}

	T incrementAndGet()
	{
		return addAndGet(1);
	}

	T decrementAndGet()
	{
		return addAndGet(-1);
	}

	void add(T x)
	{
		getAndAdd(x);
	}

	void increment()
	{
		incrementAndGet();
	}

	void decrement()
	{
		decrementAndGet();
	}

private:
	volatile T value_;

}; 
} // namespace detail	
typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;
} // namespace muduo

#endif // MUDUO_BASE_ATOMIC_H
