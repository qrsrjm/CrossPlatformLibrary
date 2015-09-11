#ifndef BASE_TIME_H
#define BASE_TIME_H

#include "base_def.h"
#include <time.h>
#include <string>

#ifdef WIN32
#include <windows.h>
#endif

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

extern wchar_t* my_wcstok(wchar_t *wcs, const wchar_t *delim);
extern char* my_strtok(char *wcs, const char *delim);

#endif