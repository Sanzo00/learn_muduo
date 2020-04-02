#include "muduo/base/Timestamp.h"
#include "muduo/base/Mutex.h"
#include "muduo/base/Thread.h"
#include "muduo/base/CountDownLatch.h"

#include <iostream>
#include <stdio.h>
#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <vector>


muduo::MutexLock g_mutex; // 声明锁
std::vector<int> g_vec;
const int kCount = 10000000; // 每次插入1000w个数

void threadFunc()
{
	for (int i = 0; i < kCount; ++i) {
		muduo::MutexLockGuard lock(g_mutex); // 上锁
		g_vec.push_back(i);
	}
}

void test_Mutex()
{
	std::cout << "---- Mutex ----\n";
	const int kMaxThreads = 8; // 最多8个线程
	g_vec.reserve(kMaxThreads * kCount); // 提前分配大小

	muduo::Timestamp start(muduo::Timestamp::now()); // 当前时间戳
	// 单个线程不用锁的时间
	for (int i = 0; i < kCount; ++i) {
		g_vec.push_back(i);
	}
	printf("1 thread(s) without lock %f\n", muduo::timeDifference(muduo::Timestamp::now(), start));

	for (int i = 0; i < kMaxThreads; ++i) {
		// i个线程用锁的时间
		boost::ptr_vector<muduo::Thread> threads;
		g_vec.clear();
		start = muduo::Timestamp::now(); // 更新时间戳
		for (int j = 0; j <= i; ++j) {
			threads.push_back(new muduo::Thread(&threadFunc)); // 创建线程
			threads.back().start(); // 启动线程	
		}

		for (int j = 0; j <= i; ++j) {
			threads[j].join(); // 回收线程
		}
		printf("%d thread(s) with lock %f\n", i+1, muduo::timeDifference(muduo::Timestamp::now(), start));
	}
}

int main() {

	test_Mutex();

	return 0;
}
