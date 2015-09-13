#ifndef BASE_TIME_H
#define BASE_TIME_H

#include "base_def.h"
#include <time.h>
#include <string>
#include "err.h"

#ifdef WIN32
#include <windows.h>
#endif

#ifndef WIN32
#include <sys/time.h>
#endif

// 秒级别的时间差类
class BASE_EXPORT TimeSpan
{
public:
    TimeSpan();
    TimeSpan(time_t time);
    TimeSpan(long nDays, int nHours, int nMins, int nSecs);
    TimeSpan(const TimeSpan& timeSpan);

    const TimeSpan& operator = (const TimeSpan& timeSpan);

    long GetDays() const;
    long GetTotalHours() const;
    long GetTotalMinutes() const;
    long GetTotalSeconds() const;
    int GetHours() const;
    int GetMinutes() const;
    int GetSeconds() const;

	TimeSpan operator + (const TimeSpan& timeSpan) const;
	TimeSpan operator - (const TimeSpan& timeSpan) const;
    const TimeSpan& operator += (const TimeSpan& timeSpan);
    const TimeSpan& operator -= (const TimeSpan& timeSpan);
    bool operator == (const TimeSpan& timeSpan) const;
    bool operator != (const TimeSpan& timeSpan) const;
    bool operator < (const TimeSpan& timeSpan) const;
    bool operator > (const TimeSpan& timeSpan) const;
    bool operator <= (const TimeSpan& timeSpan) const;
    bool operator >= (const TimeSpan& timeSpan) const;

protected:
    time_t m_timeSpan;
    friend class Time;
};

// 秒级别的时间类
class BASE_EXPORT Time
{
public:
    Time();
    Time(time_t tm);
    Time(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec);

    static Time GetCurrentTime();

    bool SetUTCTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec);
    void GetUTCTime(int &nYear, int &nMonth, int &nDay, int &nHour, int &nMin, int &nSec) const;
    void GetLocalTm(struct tm* localtime) const;
    void GetGmTm(struct tm* gm) const;
	std::string ToString() const;
	std::string ToUTCString() const;
	std::string DateToString() const;
	std::string UTCDateToString() const;
	std::string TimeToString() const;
	std::string UTCTimeToString() const;

    time_t GetTime() const;

    int GetUTCYear() const;
    int GetUTCMonth() const;
    int GetUTCDay() const;
    int GetUTCHour() const;
    int GetUTCMinute() const;
    int GetUTCSecond() const;
    int GetUTCDayOfWeek() const;
    int GetUTCDayOfMonth() const;
    int GetUTCDayOfYear() const;

    int GetYear() const;
    int GetMonth() const;
    int GetDay() const;
    int GetHour() const;
    int GetMinute() const;
    int GetSecond() const;
    int GetDayOfWeek() const;
    int GetDayOfMonth() const;
    int GetDayOfYear() const;

    TimeSpan operator - (const Time& time) const;
    Time operator - (const TimeSpan& time) const;
    Time operator + (const TimeSpan& timeSpan) const;
    const Time& operator += (const TimeSpan& timeSpan);
    const Time& operator -= (const TimeSpan& timeSpan);
    bool operator == (const Time& time) const;
    bool operator != (const Time& time) const;
    bool operator < (const Time& time) const;
    bool operator > (const Time& time) const;
    bool operator <= (const Time& time) const;
    bool operator >= (const Time& time) const;

protected:
    time_t m_time;
};

// 毫秒级别的时间差类
class BASE_EXPORT MilliTimeSpan : public TimeSpan
{
public:
	MilliTimeSpan();
	MilliTimeSpan(time_t time, int nMins);
	MilliTimeSpan(long lDays, int nHours, int nMins, int nSecs, int nMills);

	MilliTimeSpan(const MilliTimeSpan& timeSpanSrc);
	const MilliTimeSpan& operator=(const MilliTimeSpan& timeSpanSrc);

	int GetMillisecond() const { return m_nMilli; }

	MilliTimeSpan operator-(const MilliTimeSpan& timeSpan) const;
	MilliTimeSpan operator+(const MilliTimeSpan& timeSpan) const;
	const MilliTimeSpan& operator+=(const MilliTimeSpan& timeSpan);
	const MilliTimeSpan& operator-=(const MilliTimeSpan& timeSpan);
	bool operator==(const MilliTimeSpan& timeSpan) const;
	bool operator!=(const MilliTimeSpan& timeSpan) const;
	bool operator<(const MilliTimeSpan& timeSpan) const;
	bool operator>(const MilliTimeSpan& timeSpan) const;
	bool operator<=(const MilliTimeSpan& timeSpan) const;
	bool operator>=(const MilliTimeSpan& timeSpan) const;

protected:
	int m_nMilli;    // 毫秒值
	friend class MilliTime;
};

// 毫秒级别的时间类
class BASE_EXPORT MilliTime : public Time
{
public:
	MilliTime();
	MilliTime(time_t tm, int nMills);

	MilliTime(Time& t) : Time(t), m_nMilli(0)    {}
	MilliTime(const char * str)    { SetLocalByStringA(str); }
	MilliTime(const wchar_t* str)    { SetLocalByStringW(str); }

	MilliTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nMilli);
	MilliTime(const MilliTime& right);

public:
	void SetMillisecond(int nMilli) { m_nMilli = nMilli; }
	int GetMillisecond() const { return m_nMilli; }
	void SetUTCByStringA(const char * str);
	void SetUTCByStringW(const wchar_t* str);
	void SetLocalByStringA(const char * str);
	void SetLocalByStringW(const wchar_t* str);
	std::string ToStringA();
	std::wstring ToStringW();
	std::string ToUTCStringA();
	std::wstring ToUTCStringW();
	static bool ParseStrW(const wchar_t* str, int &nYear, int &nMonth, int &nDay, int &nHour, int &nMin, int &nSec, int &nMilli);
	static bool ParseStrA(const char * str, int &nYear, int &nMonth, int &nDay, int &nHour, int &nMin, int &nSec, int &nMilli);

public:
	MilliTimeSpan operator-(const MilliTime& time) const;
	MilliTime operator-(const MilliTimeSpan& timeSpan) const;
	MilliTime operator+(const MilliTimeSpan& timeSpan) const;
	const MilliTime& operator+=(const MilliTimeSpan& timeSpan);
	const MilliTime& operator-=(const MilliTimeSpan& timeSpan);
	bool operator==(const MilliTime& time) const;
	bool operator!=(const MilliTime& time) const;
	bool operator<(const MilliTime& time) const;
	bool operator>(const MilliTime& time) const;
	bool operator<=(const MilliTime& time) const;
	bool operator>=(const MilliTime& time) const;

protected:
	int m_nMilli; // 毫秒值
};

/************************************************************************/
/* TimeSpan内联定义 */
/************************************************************************/
inline TimeSpan::TimeSpan()
{
}

inline TimeSpan::TimeSpan(time_t time)
{
    m_timeSpan = time; 
}

inline TimeSpan::TimeSpan(long lDays, int nHours, int nMins, int nSecs)
{
    m_timeSpan = nSecs + 60* (nMins + 60* (nHours + 24* lDays)); 
}

inline TimeSpan::TimeSpan(const TimeSpan& timeSpanSrc)
{
    m_timeSpan = timeSpanSrc.m_timeSpan; 
}

inline const TimeSpan& TimeSpan::operator=(const TimeSpan& timeSpanSrc)
{
    m_timeSpan = timeSpanSrc.m_timeSpan; 
    return *this; 
}

inline long TimeSpan::GetDays() const
{
    return (long)m_timeSpan / (24 * 3600L); 
}

inline long TimeSpan::GetTotalHours() const
{
    return (long)m_timeSpan/3600; 
}

inline int TimeSpan::GetHours() const
{
    return (int)(GetTotalHours() - GetDays() * 24); 
}

inline long TimeSpan::GetTotalMinutes() const
{
    return (long)m_timeSpan / 60; 
}

inline int TimeSpan::GetMinutes() const
{
    return (int)(GetTotalMinutes() - GetTotalHours() * 60); 
}

inline long TimeSpan::GetTotalSeconds() const
{
    return (long)m_timeSpan; 
}

inline int TimeSpan::GetSeconds() const
{
    return (int)(GetTotalSeconds() - GetTotalMinutes() * 60); 
}

inline TimeSpan TimeSpan::operator + (const TimeSpan& timeSpan) const
{
	return TimeSpan(m_timeSpan + timeSpan.m_timeSpan);
}

inline TimeSpan TimeSpan::operator - (const TimeSpan& timeSpan) const
{
    return TimeSpan(m_timeSpan - timeSpan.m_timeSpan); 
}

inline const TimeSpan& TimeSpan::operator += (const TimeSpan& timeSpan)
{
    m_timeSpan += timeSpan.m_timeSpan; 
    return *this; 
}

inline const TimeSpan& TimeSpan::operator -= (const TimeSpan& timeSpan)
{
    m_timeSpan -= timeSpan.m_timeSpan; 
    return *this; 
}

inline bool TimeSpan::operator == (const TimeSpan& timeSpan) const
{
    return m_timeSpan == timeSpan.m_timeSpan; 
}

inline bool TimeSpan::operator != (const TimeSpan& timeSpan) const
{
    return m_timeSpan != timeSpan.m_timeSpan; 
}

inline bool TimeSpan::operator < (const TimeSpan& timeSpan) const
{
    return m_timeSpan < timeSpan.m_timeSpan;
}

inline bool TimeSpan::operator > (const TimeSpan& timeSpan) const
{
    return m_timeSpan > timeSpan.m_timeSpan; 
}

inline bool TimeSpan::operator <= (const TimeSpan& timeSpan) const
{
    return m_timeSpan <= timeSpan.m_timeSpan; 
}

inline bool TimeSpan::operator >= (const TimeSpan& timeSpan) const
{
    return m_timeSpan >= timeSpan.m_timeSpan; 
}

/************************************************************************/
/* Time内联定义 */
/************************************************************************/
inline time_t Time::GetTime() const
{
    return m_time; 
}

inline int Time::GetYear() const
{
	tm t;
#ifdef WIN32
    localtime_s(&t, &m_time);
#else
	localtime_r(&m_time, &t);
#endif
	return t.tm_year + 1900; 
}

inline int Time::GetMonth() const
{
	tm t;
#ifdef WIN32
    localtime_s(&t, &m_time);
#else
	localtime_r(&m_time, &t);
#endif
	return t.tm_mon + 1;
}

inline int Time::GetDay() const
{
	tm t;
#ifdef WIN32
    localtime_s(&t, &m_time);
#else
	localtime_r(&m_time, &t);
#endif 
	return t.tm_mday; 
}

inline int Time::GetHour() const
{
	tm t;
#ifdef WIN32
    localtime_s(&t, &m_time);
#else
	localtime_r(&m_time, &t);
#endif
	return t.tm_hour; 
}

inline int Time::GetMinute() const
{
	tm t;
#ifdef WIN32
    localtime_s(&t, &m_time);
#else
	localtime_r(&m_time, &t);
#endif 
	return t.tm_min; 
}

inline int Time::GetSecond() const
{
	tm t;
#ifdef WIN32
    localtime_s(&t, &m_time);
#else
	localtime_r(&m_time, &t);
#endif
	return t.tm_sec; 
}

inline int Time::GetDayOfWeek() const
{
	tm t;
#ifdef WIN32
    localtime_s(&t, &m_time);
#else
	localtime_r(&m_time, &t);
#endif
	return t.tm_wday + 1;
}

inline int Time::GetDayOfMonth() const
{
	tm t;
#ifdef WIN32
    localtime_s(&t, &m_time);
#else
	localtime_r(&m_time, &t);
#endif
	return t.tm_mday;
}

inline int Time::GetDayOfYear() const
{
	tm t;
#ifdef WIN32
    localtime_s(&t, &m_time);
#else
	localtime_r(&m_time, &t);
#endif
	return t.tm_yday + 1;
}

inline int Time::GetUTCYear() const
{
	tm t;
#ifdef WIN32
    gmtime_s(&t, &m_time);
#else
	gmtime_r(&m_time, &t);
#endif
	return t.tm_year + 1900;
}

inline int Time::GetUTCMonth() const
{
	tm t;
#ifdef WIN32
    gmtime_s(&t, &m_time);
#else
	gmtime_r(&m_time, &t);
#endif
	return t.tm_mon + 1;
}

inline int Time::GetUTCDay() const
{
	tm t;
#ifdef WIN32
    gmtime_s(&t, &m_time);
#else
	gmtime_r(&m_time, &t);
#endif
	return t.tm_mday; 
}

inline int Time::GetUTCHour() const
{
	tm t;
#ifdef WIN32
    gmtime_s(&t, &m_time);
#else
	gmtime_r(&m_time, &t);
#endif
	return t.tm_hour; 
}

inline int Time::GetUTCMinute() const
{
	tm t;
#ifdef WIN32
    gmtime_s(&t, &m_time);
#else
	gmtime_r(&m_time, &t);
#endif
	return t.tm_min; 
}

inline int Time::GetUTCSecond() const
{
	tm t;
#ifdef WIN32
    gmtime_s(&t, &m_time);
#else
	gmtime_r(&m_time, &t);
#endif
	return t.tm_sec; 
}

inline int Time::GetUTCDayOfWeek() const
{
	tm t;
#ifdef WIN32
    gmtime_s(&t, &m_time);
#else
	gmtime_r(&m_time, &t);
#endif
	return t.tm_wday + 1; 
}

inline int Time::GetUTCDayOfMonth() const
{
	tm t;
#ifdef WIN32
    gmtime_s(&t, &m_time);
#else
	gmtime_r(&m_time, &t);
#endif
	return t.tm_mday; 
}

inline int Time::GetUTCDayOfYear() const
{
	tm t;
#ifdef WIN32
    gmtime_s(&t, &m_time);
#else
	gmtime_r(&m_time, &t);
#endif
	return t.tm_yday + 1; 
}

inline TimeSpan Time::operator - (const Time& time) const
{
    return TimeSpan(m_time - time.m_time); 
}

inline Time Time::operator - (const TimeSpan& timeSpan) const
{
    return Time(m_time - timeSpan.m_timeSpan); 
}

inline Time Time::operator + (const TimeSpan& timeSpan) const
{
    return Time(m_time + timeSpan.m_timeSpan); 
}

inline const Time& Time::operator += (const TimeSpan& timeSpan)
{
    m_time += timeSpan.m_timeSpan; 
    return *this; 
}

inline const Time& Time::operator -= (const TimeSpan& timeSpan)
{
    m_time -= timeSpan.m_timeSpan; 
    return *this; 
}

inline bool Time::operator==(const Time& time) const
{
    return m_time == time.m_time; 
}

inline bool Time::operator!=(const Time& time) const
{
    return m_time != time.m_time; 
}

inline bool Time::operator<(const Time& time) const
{
    return m_time < time.m_time; 
}

inline bool Time::operator>(const Time& time) const
{
    return m_time > time.m_time; 
}

inline bool Time::operator<=(const Time& time) const
{
    return m_time <= time.m_time; 
}

inline bool Time::operator>=(const Time& time) const
{
    return m_time >= time.m_time; 
}

/************************************************************************/
/* MilliTimeSpan内联定义 */
/************************************************************************/
inline MilliTimeSpan::MilliTimeSpan() : m_nMilli(0)
{

}

inline MilliTimeSpan::MilliTimeSpan(time_t time, int nMilli) : TimeSpan(time)
{
	BASE_ASSERT(nMilli >= 0 && nMilli < 1000);
	m_nMilli = nMilli;
}

inline MilliTimeSpan::MilliTimeSpan(long lDays, int nHours, int nMins, int nSecs, int nMilli)
	: TimeSpan(lDays, nHours, nMins, nSecs)
{
	BASE_ASSERT(nMilli >= 0 && nMilli < 1000);
	m_nMilli = nMilli;
}

inline MilliTimeSpan::MilliTimeSpan(const MilliTimeSpan& timeSpanSrc)
	: TimeSpan(timeSpanSrc), m_nMilli(timeSpanSrc.m_nMilli)
{

}

inline const MilliTimeSpan& MilliTimeSpan::operator=(const MilliTimeSpan& timeSpanSrc)
{
	m_timeSpan = timeSpanSrc.m_timeSpan;
	m_nMilli = timeSpanSrc.m_nMilli;
	return *this;
}

inline MilliTimeSpan MilliTimeSpan::operator-(const MilliTimeSpan& timeSpan) const
{
	if (m_nMilli < timeSpan.m_nMilli)
	{
		return MilliTimeSpan(m_timeSpan - timeSpan.m_timeSpan - 1, m_nMilli - timeSpan.m_nMilli + 1000);
	}
	else
	{
		return MilliTimeSpan(m_timeSpan - timeSpan.m_timeSpan, m_nMilli - timeSpan.m_nMilli);
	}
}

inline const MilliTimeSpan& MilliTimeSpan::operator-=(const MilliTimeSpan& timeSpan)
{
	if (m_nMilli < timeSpan.m_nMilli)
	{
		m_timeSpan -= (timeSpan.m_timeSpan + 1), m_nMilli -= (timeSpan.m_nMilli - 1000);
	}
	else
	{
		m_timeSpan -= timeSpan.m_timeSpan, m_nMilli -= timeSpan.m_nMilli;
	}

	return *this;
}

inline MilliTimeSpan MilliTimeSpan::operator+(const MilliTimeSpan& timeSpan) const
{
	int nMilli = m_nMilli + timeSpan.m_nMilli;
	if (nMilli >= 1000)
	{
		return MilliTimeSpan(m_timeSpan + timeSpan.m_timeSpan + 1, nMilli - 1000);
	}
	else
	{
		return MilliTimeSpan(m_timeSpan + timeSpan.m_timeSpan, nMilli);
	}
}

inline const MilliTimeSpan& MilliTimeSpan::operator+=(const MilliTimeSpan& timeSpan)
{
	int nMilli = m_nMilli + timeSpan.m_nMilli;
	if (nMilli >= 1000)
	{
		m_timeSpan += (timeSpan.m_timeSpan + 1), m_nMilli = (nMilli - 1000);
	}
	else
	{
		m_timeSpan += timeSpan.m_timeSpan, m_nMilli = nMilli;
	}

	return *this;
}

inline bool MilliTimeSpan::operator==(const MilliTimeSpan& timeSpan) const
{
	return m_timeSpan == timeSpan.m_timeSpan && m_nMilli == timeSpan.m_nMilli;
}

inline bool MilliTimeSpan::operator!=(const MilliTimeSpan& timeSpan) const
{
	return m_timeSpan != timeSpan.m_timeSpan || m_nMilli != timeSpan.m_nMilli;
}

inline bool MilliTimeSpan::operator<(const MilliTimeSpan& timeSpan) const
{
	return (m_timeSpan == timeSpan.m_timeSpan) ? (m_nMilli < timeSpan.m_nMilli) : (m_timeSpan < timeSpan.m_timeSpan);
}

inline bool MilliTimeSpan::operator>(const MilliTimeSpan& timeSpan) const
{
	return (m_timeSpan == timeSpan.m_timeSpan) ? (m_nMilli > timeSpan.m_nMilli) : (m_timeSpan > timeSpan.m_timeSpan);
}

inline bool MilliTimeSpan::operator<=(const MilliTimeSpan& timeSpan) const
{
	return (m_timeSpan == timeSpan.m_timeSpan) ? (m_nMilli <= timeSpan.m_nMilli) : (m_timeSpan <= timeSpan.m_timeSpan);
}

inline bool MilliTimeSpan::operator>=(const MilliTimeSpan& timeSpan) const
{
	return (m_timeSpan == timeSpan.m_timeSpan) ? (m_nMilli >= timeSpan.m_nMilli) : (m_timeSpan >= timeSpan.m_timeSpan);
}

/************************************************************************/
/* MilliTime内联定义 */
/************************************************************************/
inline MilliTime::MilliTime()
{
#ifdef WIN32
	SYSTEMTIME   st;
	GetLocalTime(&st);
	m_nMilli = st.wMilliseconds;
#else
	struct timeval nowtimeval;
	gettimeofday(&nowtimeval, 0);
	m_nMilli = nowtimeval.tv_usec / 1000;
	//m_time = nowtimeval.tv_sec;
#endif
}

inline MilliTime::MilliTime(time_t tm, int nMills) : Time(tm), m_nMilli(nMills)
{
	BASE_ASSERT(m_nMilli >= 0 && m_nMilli < 1000);
}

inline MilliTime::MilliTime(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nMilli)
	: Time(nYear, nMonth, nDay, nHour, nMin, nSec)
{
	BASE_ASSERT(nMilli >= 0 && nMilli < 1000);
	m_nMilli = nMilli;
}

inline MilliTime::MilliTime(const MilliTime& right) : Time(right), m_nMilli(right.m_nMilli)
{

}

inline MilliTimeSpan MilliTime::operator-(const MilliTime& right) const
{
	if (m_nMilli < right.m_nMilli)
	{
		return MilliTimeSpan(m_time - right.m_time - 1, m_nMilli - right.m_nMilli + 1000);
	}
	else
	{
		return MilliTimeSpan(m_time - right.m_time, m_nMilli - right.m_nMilli);
	}
}

inline MilliTime MilliTime::operator-(const MilliTimeSpan& right) const
{
	if (m_nMilli < right.m_nMilli)
	{
		return MilliTime(m_time - right.m_timeSpan - 1, m_nMilli - right.m_nMilli + 1000);
	}
	else
	{
		return MilliTime(m_time - right.m_timeSpan, m_nMilli - right.m_nMilli);
	}
}

inline const MilliTime& MilliTime::operator-=(const MilliTimeSpan& timeSpan)
{
	if (m_nMilli < timeSpan.m_nMilli)
	{
		m_time -= timeSpan.m_timeSpan - 1, m_nMilli -= (timeSpan.m_nMilli - 1000);
	}
	else
	{
		m_time -= timeSpan.m_timeSpan, m_nMilli -= timeSpan.m_nMilli;
	}

	return *this;
}

inline MilliTime MilliTime::operator+(const MilliTimeSpan& timeSpan) const
{
	if ((m_nMilli + timeSpan.m_nMilli) >= 1000)
	{
		return MilliTime(m_time + timeSpan.m_timeSpan + 1, m_nMilli + timeSpan.m_nMilli - 1000);
	}
	else
	{
		return MilliTime(m_time + timeSpan.m_timeSpan, m_nMilli + timeSpan.m_nMilli);
	}
}

inline const MilliTime& MilliTime::operator+=(const MilliTimeSpan &timeSpan)
{
	if ((m_nMilli + timeSpan.m_nMilli) >= 1000)
	{
		m_time += (timeSpan.m_timeSpan + 1), m_nMilli += (timeSpan.m_nMilli - 1000);
	}
	else
	{
		m_time += timeSpan.m_timeSpan, m_nMilli += timeSpan.m_nMilli;
	}

	return *this;
}

inline bool MilliTime::operator==(const MilliTime& time) const
{
	return (m_time == time.m_time) && (m_nMilli == time.m_nMilli);
}

inline bool MilliTime::operator!=(const MilliTime& time) const
{
	return (m_time != time.m_time) || (m_nMilli != time.m_nMilli);
}

inline bool MilliTime::operator<(const MilliTime& time) const
{
	return (m_time == time.m_time) ? (m_nMilli < time.m_nMilli) : (m_time < time.m_time);
}

inline bool MilliTime::operator>(const MilliTime& time) const
{
	return (m_time == time.m_time) ? (m_nMilli > time.m_nMilli) : (m_time > time.m_time);
}

inline bool MilliTime::operator<=(const MilliTime& time) const
{
	return (m_time == time.m_time) ? (m_nMilli <= time.m_nMilli) : (m_time <= time.m_time);
}

inline bool MilliTime::operator>=(const MilliTime& time) const
{
	return (m_time == time.m_time) ? (m_nMilli >= time.m_nMilli) : (m_time >= time.m_time);
}

extern wchar_t* my_wcstok(wchar_t *wcs, const wchar_t *delim);
extern char* my_strtok(char *wcs, const char *delim);

#endif