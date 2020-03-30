#include "muduo/base/Exception.h"
#include "muduo/base/CurrentThread.h"

namespace muduo
{

	// std::move() 将左值引用转换为右值引用
	Exception::Exception(string msg)
		: message_(std::move(msg)),
		  stack_(CurrentThread::stackTrace(false))
	{
	}		  
} // namespace muduo
