#include <iostream>
#include <stdio.h>
#include "basic.h"
#include "thread_test.h"

using namespace std;

int main()
{
	ThreadTest thread;
	thread.Start();

	bool ret = thread.Wait();
	BASE_ASSERT(ret);

	cout << "�߳̽�����,���س����˳�" << endl;

	getchar();

	return 0;
}