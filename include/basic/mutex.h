#ifndef BASIC_INCLUDE_H
#define BASIC_INCLUDE_H

#include "err.h"
#include "base_def.h"

#ifdef WIN32
#include <windows.h>

class BASE_EXPORT Mutex
{
public:
	Mutex() { InitializeCriticalSection(&_critSection); }
	~Mutex() { DeleteCriticalSection(&_critSection); }
	void Lock() const { EnterCriticalSection(&_critSection); }
	void Unlock() const { LeaveCriticalSection(&_critSection); }

private:
	Mutex(const Mutex&);
	const Mutex &operator = (const Mutex&);

private:
	mutable CRITICAL_SECTION _critSection;
};

#ifndef ETIMEDOUT
#define ETIMEDOUT WAIT_TIMEOUT
#endif

class BASE_EXPORT CondMutex
{
public:
	CondMutex()
	{
		m_cond = CreateEvent(NULL, FALSE, FALSE, NULL);
		WIN_ASSERT(0 != m_cond);
	}

	~CondMutex()
	{
		BOOL ret = CloseHandle(m_cond);
		WIN_ASSERT(TRUE == ret);
	}

	int WaitWithTimeout(Mutex *mutex, int mill) const 
	{
		mutex->Unlock();
		DWORD ret = WaitForSingleObject(m_cond, mill);
		mutex->Lock();
		if (WAIT_OBJECT_0 == ret)
		{
			return 0;
		}
		else if (WAIT_TIMEOUT == ret)
		{
			return ETIMEDOUT;
		}

		WIN_ASSERT(0);

		return ret;
	}

	int Wait(Mutex *mutex) const
	{
		mutex->Unlock();
		DWORD ret = WaitForSingleObject(m_cond, INFINITE);
		mutex->Lock();
		if (WAIT_OBJECT_0 == ret)
		{
			return 0;
		}

		WIN_ASSERT(0);

		return ret;
	}

	void Signal() const
	{
		BOOL ret = SetEvent(m_cond);
		WIN_ASSERT(TRUE == ret);
	}

private:
	HANDLE m_cond;
};

#else

#include <pthread.h>

class BASE_EXPORT Mutex
{
public:
	Mutex()
	{
		m_count = 0;
		m_owner = 0;
		int ret = pthread_mutex_init(&m_mutex, NULL);
		POSIX_ASSERT(ret);
	}

	~Mutex()
	{
		int ret = pthread_mutex_destroy(&m_mutex);
		POSIX_ASSERT(ret);
	}

	void Lock() const
	{
		pthread_t self = pthread_self();
		if (m_owner != self)
		{
			int ret = pthread_mutex_lock(&m_mutex);
			POSIX_ASSERT(ret);
			m_owner = self;
		}
		m_count++;
	}

	void Unlock() const
	{
		BASE_ASSERT(pthread_self() == m_owner);
		if (--m_count == 0)
		{
			m_owner = 0;
			int ret = pthread_mutex_unlock(&m_mutex);
			POSIX_ASSERT(ret);
		}
	}

	pthread_mutex_t *GetMutex()
	{
		return &m_mutex;
	}

private:
	Mutex(const Mutex&);
	const Mutex &operator = (const Mutex&);

private:
	mutable pthread_mutex_t m_mutex;
	mutable int m_count;
	mutable pthread_t m_owner;
};

class BASE_EXPORT CondMutex
{
public:
	CondMutex()
	{
		int ret = pthread_cond_init(&m_cond, NULL);
		POSIX_ASSERT(ret);
	}

	~CondMutex()
	{
		int ret = pthread_cond_destroy(&m_cond);
		POSIX_ASSERT(ret);
	}

	int WaitWithTimeout(Mutex *mutex, int mill) const
	{
		struct timespec tv;
		tv.tv_sec = mill / 1000;
		tv.tv_nsec = (mill % 1000) * 1000;
		int ret = pthread_cond_timedwait(&m_cond, mutex->GetMutex(), &tv);
		if (0 != ret && ETIMEDOUT != ret)
		{
			POSIX_ASSERT(ret);
		}

		return ret;
	}

	int Wait(Mutex *mutex) const
	{
		int ret = pthread_cond_wait(&m_cond, mutex->GetMutex());
		if (0 != ret && ETIMEDOUT != ret)
		{
			POSIX_ASSERT(ret);
		}

		return ret;
	}

	void Signal() const
	{
		int ret = pthread_cond_signal(&m_cond);
		POSIX_ASSERT(ret);
	}

private:
	mutable pthread_cond_t m_cond;
};

#endif

class BASE_EXPORT CritSectLock
{
public:
	CritSectLock(Mutex& mx) :m_mutex(mx) { m_mutex.Lock(); }
	~CritSectLock(){ m_mutex.Unlock(); }

private:
	CritSectLock(const CritSectLock& critical);
	const CritSectLock& operator=(const CritSectLock& critical);

private:
	Mutex& m_mutex;
};

#define HT_CS(mx) CritSectLock __critical_section(mx)

#endif