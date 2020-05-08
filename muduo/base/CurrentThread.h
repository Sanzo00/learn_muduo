#ifndef MUDUO_BASE_CURRENTTHREAD_H
#define MUDUO_BASE_CURRENTTHREAD_H

#include "muduo/base/Types.h"

namespace muduo
{

namespace CurrentThread
{
// 内部
	extern __thread int t_cachedTid;
	extern __thread char t_tidString[32];
	extern __thread int t_tidStringLength;
	extern __thread const char* t_threadName;
	void cacheTid(); // Thread.cpp定义

	inline int tid()
	{
		// __builtin_expect是GCC提供给程序员使用的, 目的是将"分支转移"的信息提供给编译器
		// 编译器对代码进行优化, 减少指令跳转带来的性能下降
		// __builtin_expect(EXP, N) EXP==N的可能性更大
		if (__builtin_expect(t_cachedTid == 0, 0)) {
			cacheTid();
		}
		return t_cachedTid;
	}

	inline const char* tidString() // for logging
	{
		return t_tidString;
	}

	inline int tidStringLength() // for logging
	{
		return t_tidStringLength;
	}

	inline const char* name()
	{
		return t_threadName;
	}

	bool isMainThread(); // Thread.cpp定义

	void sleepUsec(int64_t usec); // for testing, Thread.cpp定义

	// C/C++语言在编译以后, 函数的名字会被编译器修改, 改成编译器内部的名字, 这个名字会在链接的时候用到
	// 将C++源程序标识符(original C++ source identifier)转换成C++ ABI标识符(C++ ABI identifier)的过程称为mangle, 相反的过程称为demangle
	string stackTrace(bool demangle);
}// namespace CurrentThread
}// namespace muduo

#endif
