#ifndef THREAD_BASE_H
#define THREAD_BASE_H

#include "mutex.h"

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif

void SleepMill(int ms);

class ThreadBase
{
public:
	ThreadBase();
	virtual ~ThreadBase();

	// 启动线程
	void Start();

	// 等待线程退出
	bool Wait(unsigned long milli = INFINITE);

	// 判断线程是否正在运行
	bool IsRunning();

	// 停止线程，调用后可以调用wait等待线程真正完全退出
	void Terminate();

	// 获取线程ID
	int GetThreadId() { return m_nThreadId; }

protected:
	// 子类必须以该函数作为循环的条件，以保证调用terminate时能安全退出线程
	bool IsBreak();

	// 线程运行体
	virtual void Run() = 0;

private:
#ifdef WIN32
	unsigned int m_nThreadId;
	HANDLE m_hThread;
	static int ThreadProc(void *arg);
#else
	pthread_t m_nThreadId;
	static void *ThreadProc(void *arg);
#endif

	bool m_bBreak;
	Mutex m_mutex;
};

#endif