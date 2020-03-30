#include "muduo/base/Timestamp.h"
#include "muduo/base/Date.h"
#include "muduo/base/Exception.h"
#include "muduo/base/CurrentThread.h"

#include <iostream>
#include <sys/time.h>


void test_Timestamp()
{
	std::cout << "---- timestamp ----" << std::endl;

	muduo::Timestamp t1 = muduo::Timestamp::now();
	std::cout << t1.toString() << std::endl;
	std::cout << t1.toFormattedString(true) << std::endl;

	muduo::Timestamp t2 = muduo::Timestamp::now();
	if (t1 != t2) {
		std::cout << "not equal" << std::endl;
	}

	if (t2 > t1) {
		std::cout << "t2 > t1" << std::endl;
	}
}

void test_Date()
{
	std::cout << "---- Date ----" << std::endl;

	struct tm t;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	time_t seconds = tv.tv_sec;
	gmtime_r(&seconds, &t);
//	muduo::Date date = muduo::Date(t);

	muduo::Date date = muduo::Date::getLocalDate();
	std::cout << "julianDayNumber: " << date.julianDayNumber() << std::endl;
	std::cout << "toIsoString: " << date.toIsoString() << std::endl;
}

void throw_Exception()
{
	throw muduo::Exception("err");	
}

void test_Exception()
{
	std::cout << "---- Exception ----\n";
	try {
		throw_Exception();
	} catch(muduo::Exception e) {
		std::cout << e.what() << std::endl;
		std::cout << e.stackTrace() << std::endl;
	}

}

int main() {

	test_Timestamp();
	test_Date();
	test_Exception();


	return 0;
}
