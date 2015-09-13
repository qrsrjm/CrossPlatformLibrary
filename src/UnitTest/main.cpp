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

	cout << "线程结束了,按回车键退出" << endl;

	getchar();

	return 0;
}