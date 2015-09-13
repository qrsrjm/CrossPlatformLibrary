#ifndef THREAD_TEST_H
#define THREAD_TEST_H

#include "basic.h"

class ThreadTest : public ThreadBase
{
public:
	ThreadTest();
	~ThreadTest();

protected:
	// 线程运行体
	virtual void Run();
};

#endif