#ifndef THREAD_TEST_H
#define THREAD_TEST_H

#include "basic.h"

class ThreadTest : public ThreadBase
{
public:
	ThreadTest();
	~ThreadTest();

protected:
	// �߳�������
	virtual void Run();
};

#endif