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

		cout << "�����߳�,����ÿ��1S��ӡ����" << endl;
		SleepMill(1000);
	}
}
