#include "thread_base.h"

#ifdef WIN32
#include <process.h>
typedef unsigned (__stdcall * ThreadStartProc) (void *);

ThreadBase::ThreadBase()
: m_hThread(NULL)
, m_nThreadId(0)
, m_bBreak(false)
{
}

ThreadBase::~ThreadBase()
{
	if (IsRunning())
	{
		Terminate();
		Wait();

		BOOL ret = CloseHandle(m_hThread);
		WIN_ASSERT(TRUE == ret);
		m_hThread = NULL;
	}
}

void ThreadBase::Start()
{
	if (IsRunning())
	{
		return;
	}

	if (NULL != m_hThread)
	{
		BOOL ret = CloseHandle(m_hThread);
		WIN_ASSERT(TRUE == ret);
		m_hThread = NULL;
	}

	m_bBreak = false;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, (ThreadStartProc)ThreadProc, this, 0, &m_nThreadId);
	ERRNO_ASSERT(NULL != m_hThread);
	BASE_ASSERT(0 != m_nThreadId);
}

bool ThreadBase::Wait(unsigned long milli /*= INFINITE*/)
{
	if (!IsRunning())
	{
		return true;
	}

	DWORD ret = WaitForSingleObject(m_hThread, milli);
	if (WAIT_OBJECT_0 == ret)
	{
		return true;
	}
	else if (WAIT_TIMEOUT == ret)
	{
		return false;
	}

	WIN_ASSERT(0);

	return false;
}

bool ThreadBase::IsRunning()
{
	HT_CS(m_mutex);
	return 0 != m_nThreadId;
}

void ThreadBase::Terminate()
{
	HT_CS(m_mutex);
	m_bBreak = true;
}

bool ThreadBase::IsBreak()
{
	HT_CS(m_mutex);
	return m_bBreak;
}

int ThreadBase::ThreadProc(void *arg)
{
	ThreadBase *pThis = (ThreadBase*)arg;
	pThis->Run();
	HT_CS(pThis->m_mutex);
	pThis->m_nThreadId = 0;
	return 0;
}

#else

ThreadBase::ThreadBase()
: m_nThreadId(0)
, m_bBreak(false)
{
}


ThreadBase::~ThreadBase()
{
	if (IsRunning())
	{
		Terminate();
		Wait();
	}
}

void ThreadBase::Start()
{
	if (IsRunning())
	{
		return;
	}

	m_bBreak = false;
	int ret = pthread_create(&m_nThreadId, NULL, ThreadProc, this);
	POSIX_ASSERT(ret);
}

bool ThreadBase::Wait(unsigned long milli /*= INFINITE*/)
{
	if (!IsRunning())
	{
		return true;
	}

	int ret = pthread_join(m_nThreadId, NULL);
	POSIX_ASSERT(ret);

	return true;
}

bool ThreadBase::IsRunning()
{
	HT_CS(m_mutex);
	return 0 != m_nThreadId;
}

void ThreadBase::Terminate()
{
	HT_CS(m_mutex);
	m_bBreak = true;
}

bool ThreadBase::IsBreak()
{
	HT_CS(m_mutex);
	return m_bBreak;
}

int ThreadBase::ThreadProc(void *arg)
{
	ThreadBase *pThis = (ThreadBase*)arg;
	pThis->Run();
	HT_CS(pThis->m_mutex);
	pThis->m_nThreadId = 0;
	return 0;
}

#endif

void SleepMill(int ms)
{
#ifdef WIN32
	Sleep(ms);
#else
	usleep(ms * 1000);
#endif
}
