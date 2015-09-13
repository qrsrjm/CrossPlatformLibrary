#include "thread_test.h"

#include <iostream>
using namespace std;

ThreadTest::ThreadTest()
{
}


ThreadTest::~ThreadTest()
{
}

void ThreadTest::Run()
{
	int nCount = 0;
	while ( !IsBreak() )
	{
		if (nCount++ > 10)
		{
			break;
		}

		cout << "我是线程,我在每隔1S打印东西" << endl;
		SleepMill(1000);
	}
}
