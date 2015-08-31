#ifndef BASIC_INCLUDE_H
#define BASIC_INCLUDE_H

#include "err.h"

#ifdef WIN32
#include <windows.h>

class Mutex
{
public:
	Mutex() { InitializeCriticalSection(&_critSection); }
	~Mutex() { DeleteCriticalSection(&_critSection); }
	void lock(){ EnterCriticalSection(&_critSection); }
	void unlock(){ LeaveCriticalSection(&_critSection); }

private:
	Mutex(const Mutex&);
	const Mutex &operator = (const Mutex&);

private:
	CRITICAL_SECTION _critSection;
};

#ifndef ETIMEDOUT
#define ETIMEDOUT WAIT_TIMEOUT
#endif

class CondMutex
{
public:
	CondMutex()
	{
		m_cond = CreateEvent(NULL, FALSE, FALSE, NULL);
		win_assert(0 != m_cond);
	}

	~CondMutex()
	{
		BOOL ret = CloseHandle(m_cond);
		win_assert(TRUE == ret);
	}

	int wait_with_timeout(Mutex *mutex, int mill) const 
	{
		mutex->unlock();
		DWORD ret = WaitForSingleObject(m_cond, mill);
		mutex->lock();
		if (WAIT_OBJECT_0 == ret)
		{
			return 0;
		}
		else if (WAIT_TIMEOUT == ret)
		{
			return ETIMEDOUT;
		}

		win_assert(0);

		return ret;
	}

	int wait(Mutex *mutex) const
	{
		mutex->unlock();
		DWORD ret = WaitForSingleObject(m_cond, INFINITE);
		mutex->lock();
		if (WAIT_OBJECT_0 == ret)
		{
			return 0;
		}

		win_assert(0);

		return ret;
	}

	void signal() const
	{
		BOOL ret = SetEvent(m_cond);
		win_assert(TRUE == ret);
	}

private:
	HANDLE m_cond;
};

#else

#include <pthread.h>

class Mutex
{
public:
	Mutex()
	{
		m_count = 0;
		m_owner = 0;
		int ret = pthread_mutex_init(&m_mutex, NULL);
		posix_assert(ret);
	}

	~Mutex()
	{
		int ret = pthread_mutex_destroy(&m_mutex);
		posix_assert(ret);
	}

	void lock() const
	{
		pthread_t self = pthread_self();
		if (m_owner != self)
		{
			int ret = pthread_mutex_lock(&m_mutex);
			posix_assert(ret);
			m_owner = self;
		}
		m_count++;
	}

	void unlock() const
	{
		base_assert(pthread_self() == m_owner);
		if (--m_count == 0)
		{
			m_owner = 0;
			int ret = pthread_mutex_unlock(&m_mutex);
			posix_assert(ret);
		}
	}

	pthread_mutex_t *get_mutex()
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

class CondMutex
{
public:
	CondMutex()
	{
		int ret = pthread_cond_init(&m_cond, NULL);
		posix_assert(ret);
	}

	~CondMutex()
	{
		int ret = pthread_cond_destroy(&m_cond);
		posix_assert(ret);
	}

	int wait_with_timeout(Mutex *mutex, int mill) const
	{
		struct timespec tv;
		tv.tv_sec = mill / 1000;
		tv.tv_nsec = (mill % 1000) * 1000;
		return pthread_cond_timedwait(&m_cond, mutex->get_mutex(), &tv);
	}

	int wait(Mutex *mutex) const
	{
		return pthread_cond_wait(&m_cond, mutex->get_mutex());
	}

	void signal() const
	{
		int ret = pthread_cond_signal(&m_cond);
		posix_assert(ret);
	}

private:
	mutable pthread_cond_t m_cond;
};

#endif

class CritSectLock
{
public:
	CritSectLock(Mutex& mx) :m_mutex(mx) { m_mutex.lock(); }
	~CritSectLock(){ m_mutex.unlock(); }

private:
	CritSectLock(const CritSectLock& critical);
	const CritSectLock& operator=(const CritSectLock& critical);

private:
	Mutex& m_mutex;
};

#define HT_CS(mx) CritSectLock __critical_section(mx)

#endif