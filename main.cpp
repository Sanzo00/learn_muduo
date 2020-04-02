#include "muduo/base/BoundedBlockingQueue.h"
#include "muduo/base/BlockingQueue.h"
#include "muduo/base/Thread.h"

#include <iostream>
#include <stdio.h>

using namespace muduo;

BoundedBlockingQueue<int> que(3);
BlockingQueue<int> que2;
void produce() 
{
	int num = 1;
	while (true) {
		// que.put(num); // for BoundedBlockingQueue
		que2.put(num);
		int ts = rand() % 3;
		std::cout << "produce: put " << num << " and sleep " << ts << "s\n";
		sleep(ts);
		++num;
	}
}

void consume() 
{
	while (true) {
		// int num = que.take(); // for BoundedBlockingQueue
		int num = que2.take(); // for BlockingQueue
		int ts = rand() % 3;
		std::cout << "consume: take " << num << " and sleep " << ts << "s\n";
		sleep(ts);
	}
}

void test_BlockingQueue()
{
	// std::cout << "----- BoundedBlockingQueue -----\n";
	std::cout << "----- BlockingQueue -----\n";
	
	Thread t1(produce);
	Thread t2(consume);

	t1.start();
	t2.start();

	t1.join();
	t2.join();
}

int main() {

	test_BlockingQueue();

	return 0;
}
