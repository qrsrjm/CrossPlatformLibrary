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

	// �����߳�
	void Start();

	// �ȴ��߳��˳�
	bool Wait(unsigned long milli = INFINITE);

	// �ж��߳��Ƿ���������
	bool IsRunning();

	// ֹͣ�̣߳����ú���Ե���wait�ȴ��߳�������ȫ�˳�
	void Terminate();

	// ��ȡ�߳�ID
	int GetThreadId() { return m_nThreadId; }

protected:
	// ��������Ըú�����Ϊѭ�����������Ա�֤����terminateʱ�ܰ�ȫ�˳��߳�
	bool IsBreak();

	// �߳�������
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