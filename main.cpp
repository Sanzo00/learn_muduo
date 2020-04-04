#include "muduo/base/Thread.h"
#include "muduo/base/Logging.h"

#include <iostream>
#include <stdio.h>

using namespace muduo;

void threadFunc() 
{
	LOG_TRACE << "thread TRACE";
	LOG_DEBUG << "thread DEBUG";
	LOG_INFO << "thread  INFO";
	LOG_WARN << "thread  WARN";
	LOG_ERROR << "thread ERROR";
	LOG_SYSERR << "thread SYSERR";

}

void test_Log()
{
	std::cout << "----- Log -----\n";
	
	Thread t1(threadFunc);

	t1.start();
	t1.join();
}

int main() {

	test_Log();

	return 0;
}
