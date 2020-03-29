#include "muduo/base/Timestamp.h"

#include <iostream>

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

int main() {

	test_Timestamp();

	return 0;
}
