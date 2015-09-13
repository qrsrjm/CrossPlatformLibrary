#include "base_time.h"
#include "except.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

#define _MAX__TIME64_T     0x793406fffLL 

// 参考Linux源代码中的实现
// 详解访问：http://blog.csdn.net/axx1611/article/details/1792827
static inline unsigned long linux_mktime (
              unsigned int year, unsigned int mon,
              unsigned int day, unsigned int hour,
              unsigned int min, unsigned int sec)
{
    /* 1..12 -> 11,12,1..10 */
    if (0 >= (int) (mon -= 2)) 
    {    
        mon += 12;        /* Puts Feb last since it has leap day */
        year -= 1;
    }

    return ((((unsigned long) (year / 4 - year / 100 + year / 400 + 367 * mon / 12 + day)
              + year * 365 - 719499) * 24 + hour) * 60 + min ) * 60 + sec; /* finally seconds */
}


/************************************************************************/
/* Time定义 */
/************************************************************************/
Time::Time()
{
    m_time = time(NULL);
}

Time::Time(time_t tm)
{
    m_time = tm;
}

Time::Time(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec)
{
    if (SetUTCTime (nYear, nMonth, nDay, nHour, nMin, nSec))
    {
#ifdef WIN32
        _tzset ();
        long timezone;
        _get_timezone(&timezone);
        m_time += timezone;
#else
        tzset ();
        m_time += timezone;        
#endif
    }
}

bool Time::SetUTCTime (int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec)
{
    if ((nDay >= 1 && nDay <= 31)
        && (nMonth >= 1 && nMonth <= 12)
        && (nYear >= 1900)
		&& (nHour >= 0 && nHour <= 23)
		&& (nMin >= 0 && nMin <= 59)
		&& (nSec >= 0 && nSec <= 59))
    {
        m_time = linux_mktime (nYear, nMonth, nDay, nHour, nMin, nSec);
        return true;
    }

    m_time = -1;

    return false;
}

void Time::GetLocalTm(struct tm* lc) const
{
#ifdef WIN32
    localtime_s(lc, &m_time);
#else
	localtime_r(&m_time, lc);
#endif
}

void Time::GetGmTm (struct tm* gm) const
{
#ifdef WIN32
    gmtime_s(gm, &m_time);
#else
	gmtime_r(&m_time, gm);
#endif
}

void Time::GetUTCTime (int &nYear, int &nMonth, int &nDay, int &nHour, int &nMin, int &nSec) const
{
    if (m_time > 0 && m_time <= _MAX__TIME64_T)
    {
        struct tm g;
        GetGmTm(&g);

        nYear = g.tm_year + 1900;
        nMonth = g.tm_mon + 1;
        nDay = g.tm_mday;
        nHour = g.tm_hour;
        nMin = g.tm_min;
        nSec = g.tm_sec;
    }
}

Time Time::GetCurrentTime ()
{
    return Time(time(NULL));
}

string Time::ToString() const
{
    char szTime[100] = {0};
    if (m_time > 0 && m_time <= _MAX__TIME64_T )
    {
#ifdef WIN32
        sprintf_s(szTime, 100, "%4d-%02d-%02d %02d:%02d:%02d",
            GetYear(), GetMonth(), GetDay(),
            GetHour(), GetMinute(), GetSecond());
#else
        sprintf(szTime, "%4d-%02d-%02d %02d:%02d:%02d",
            GetYear(), GetMonth(), GetDay(),
            GetHour(), GetMinute(), GetSecond());
#endif
    }
    return string(szTime);
}

string Time::ToUTCString() const
{
    char szTime[100] = {0};
    if (m_time > 0 && m_time <= _MAX__TIME64_T )
    {
#ifdef WIN32
        sprintf_s(szTime, 100, "%4d-%02d-%02d %02d:%02d:%02d",
            GetUTCYear(), GetUTCMonth(), GetUTCDay(),
            GetUTCHour(), GetUTCMinute(), GetUTCSecond());
#else
        sprintf(szTime, "%4d-%02d-%02d %02d:%02d:%02d",
            GetUTCYear(), GetUTCMonth(), GetUTCDay(),
            GetUTCHour(), GetUTCMinute(), GetUTCSecond());
#endif
    }
    return string(szTime);
}

string Time::DateToString() const
{
	char szTime[100] = { 0 };
	if (m_time > 0 && m_time <= _MAX__TIME64_T)
	{
#ifdef WIN32
		sprintf_s(szTime, 100, "%4d%02d%02d", GetYear(), GetMonth(), GetDay());
#else
		sprintf(szTime, "%4d%02d%02d", GetYear(), GetMonth(), GetDay());
#endif
	}

	return string(szTime);
}

string Time::UTCDateToString() const
{
	char szTime[100] = {0};
	if (m_time > 0 && m_time <= _MAX__TIME64_T )
	{
#ifdef WIN32
		sprintf_s(szTime, 100, "%4d%02d%02d", GetUTCYear(), GetUTCMonth(), GetUTCDay());
#else
		sprintf(szTime, "%4d%02d%02d",	GetUTCYear(), GetUTCMonth(), GetUTCDay());
#endif
	}

	return string(szTime);
}

string Time::TimeToString() const
{
 	char szTime[100] = {0};
 	if (m_time > 0 && m_time <= _MAX__TIME64_T )
 	{
#ifdef WIN32
 		sprintf_s(szTime, 100, "%02d%02d%02d", GetHour(), GetMinute(), GetSecond());
#else
 		sprintf(szTime, "%02d%02d%02d",	GetHour(), GetMinute(), GetSecond());
#endif
 	}

	return string(szTime);
}

string Time::UTCTimeToString() const
{
	char szTime[100] = { 0 };
	if (m_time > 0 && m_time <= _MAX__TIME64_T)
	{
#ifdef WIN32
		sprintf_s(szTime, 100, "%02d%02d%02d", GetUTCHour(), GetUTCMinute(), GetUTCSecond());
#else
		sprintf(szTime, "%02d%02d%02d", GetUTCHour(), GetUTCMinute(), GetUTCSecond());
#endif
	}

	return string(szTime);
}

/************************************************************************/
/* MilliTime定义 */
/************************************************************************/
std::string MilliTime::ToStringA ()
{
    char szTime[100] = {0};
    if (m_time > 0 && m_time <= _MAX__TIME64_T)
    {
#ifdef WIN32
        sprintf_s(szTime, 100, "%04d-%02d-%02d %02d:%02d:%02d.%03d", 
            GetYear(), GetMonth(), GetDay(),
            GetHour(), GetMinute(), GetSecond(), GetMillisecond());
#else
        sprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d", 
            GetYear(), GetMonth(), GetDay(),
            GetHour(), GetMinute(), GetSecond(), GetMillisecond());
#endif
    }

    return string(szTime);
}

std::wstring MilliTime::ToStringW ()
{
    wchar_t buf[30];
    if (m_time > 0 && m_time <= _MAX__TIME64_T )
    {
        swprintf(buf,30, L"%04d-%02d-%02d %02d:%02d:%02d.%03d", 
            GetYear(), GetMonth(), GetDay(),
            GetHour(), GetMinute(), GetSecond(), GetMillisecond());
    }
    return buf;
}

std::string MilliTime::ToUTCStringA()
{
    char szTime[100] = {0};
    if (m_time > 0 && m_time <= _MAX__TIME64_T)
    {
#ifdef WIN32
        sprintf_s(szTime, 100, "%04d-%02d-%02d %02d:%02d:%02d.%03d", 
            GetUTCYear(), GetUTCMonth(), GetUTCDay(),
            GetUTCHour(), GetUTCMinute(), GetUTCSecond(), GetMillisecond());
#else
        sprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d", 
            GetUTCYear(), GetUTCMonth(), GetUTCDay(),
            GetUTCHour(), GetUTCMinute(), GetUTCSecond(), GetMillisecond());
#endif
    }

    return string(szTime);
}

std::wstring MilliTime::ToUTCStringW()
{
    wchar_t buf[30];
    if (m_time > 0 && m_time <= _MAX__TIME64_T )
    {
        swprintf(buf,30, L"%04d-%02d-%02d %02d:%02d:%02d.%03d", 
            GetUTCYear(), GetUTCMonth(), GetUTCDay(),
            GetUTCHour(), GetUTCMinute(), GetUTCSecond(), GetMillisecond());
    }
    return buf;
}

#ifdef WIN32
// 自定义字符拆分函数
wchar_t* my_wcstok(wchar_t *wcs, const wchar_t *delim)
{
    static wchar_t *state = NULL;
    return wcstok_s(wcs, delim, &state);
}

char* my_strtok(char* cs, const char* delim)
{
    static char* state = NULL;
    return strtok_s(cs, delim, &state);
}
#else
long _wtoi(const wchar_t *ptr)
{
    wchar_t *end_ptr = NULL;
    return wcstol(ptr, &end_ptr, 10);
}

wchar_t *my_wcstok(wchar_t *wcs, const wchar_t *delim)
{
    static wchar_t *state = NULL;
    return wcstok (wcs, delim, &state);
}

char* my_strtok(char* cs, const char* delim)
{
	static char *state = NULL;
    return strtok_r(cs, delim, &state);
}
#endif // WIN32

bool MilliTime::ParseStrW (const wchar_t* str_in, int &nYear, int &nMonth, int &nDay, int &nHour, int &nMin, int &nSec, int &nMilli)
{
    wstring str(str_in);
    wchar_t seps1[]   = L"-/";
    wchar_t seps2[]   = L" ";
    wchar_t seps3[]   = L":";
    wchar_t seps4[]   = L".";

    int year = 0, mon = 0, day = 0, hour = 0, min = 0, sec = 0, milli = 0;
    bool ok = false;
    wchar_t* tok = my_wcstok ((wchar_t *)str.c_str(), seps1);    // take year
    if (NULL != tok)
    {
        year = _wtoi(tok);
        tok = my_wcstok (NULL, seps1);    // take month
        if (NULL != tok)
        {
            mon = _wtoi (tok);
            tok = my_wcstok (NULL, seps2);    // take day
            if (NULL != tok)
            {
                day = _wtoi(tok);
                ok = true;
                tok = my_wcstok(NULL, seps3);    // take hour
                if (NULL != tok)
                {
                    ok = false;
                    hour = _wtoi(tok);
                    tok = my_wcstok(NULL, seps3);    // take min
                    if (NULL != tok)
                    {
                        min = _wtoi(tok);
                        tok = my_wcstok(NULL, seps4);    // take second
                        if (NULL != tok)
                        {
                            sec = _wtoi(tok);
                            ok = true;
                            tok = my_wcstok(NULL, seps4);    // take milli
                            if (NULL != tok)
                                milli = _wtoi(tok);
                        }
                    }
                }
            }
        }
    }

    if (ok)
    {
        nYear = year;
        nMonth = mon;
        nDay = day;
        nHour = hour;
        nMin = min;
        nSec = sec;
        nMilli = milli;
        return true;
    }

    return false;
}


bool MilliTime::ParseStrA (const char * str_in, int &nYear, int &nMonth, int &nDay, int &nHour, int &nMin, int &nSec, int &nMilli)
{
    string str(str_in);
    char seps1[]   = "-/";
    char seps2[]   = " ";
    char seps3[]   = ":";
    char seps4[]   = ".";

    int year = 0, mon = 0, day = 0, hour = 0, min = 0, sec = 0, milli = 0;
    bool ok = false;

    const char* tok = my_strtok ((char *)str.c_str(), seps1);    // take year
    if (NULL != tok)
    {
        year = atoi(tok);
        tok = my_strtok (NULL, seps1);    // take month
        if (NULL != tok)
        {
            mon = atoi (tok);
            tok = my_strtok (NULL, seps2);    // take day
            if (NULL != tok)
            {
                day = atoi(tok);
                ok = true;
                tok = my_strtok(NULL, seps3);    // take hour
                if (NULL != tok)
                {
                    ok = false;
                    hour = atoi(tok);
                    tok = my_strtok(NULL, seps3);    // take min
                    if (NULL != tok)
                    {
                        min = atoi(tok);
                        tok = my_strtok(NULL, seps4);    // take second
                        if (NULL != tok)
                        {
                            sec = atoi(tok);
                            ok = true;
                            tok = my_strtok(NULL, seps4);    // take milli
                            if (NULL != tok)
                                milli = atoi(tok);
                        }
                    }
                }
            }
        }
    }

    if (ok)
    {
        nYear = year;
        nMonth = mon;
        nDay = day;
        nHour = hour;
        nMin = min;
        nSec = sec;
        nMilli = milli;
        return true;
    }

    return false;
}

void MilliTime::SetUTCByStringW (const wchar_t* str)
{
    int year = 0, mon = 0, day = 0, hour = 0, min = 0, sec = 0, milli = 0;
    if (ParseStrW(str, year, mon, day, hour, min, sec, milli))
    {
        SetUTCTime(year, mon, day, hour, min, sec);
        m_nMilli = milli;
    }
    else
    {
        throw Except (DATETIME_FMT, "Datetime format error. used as yyyy-mm-dd HH:MM:SS.xxxx");
    }
}
                
void MilliTime::SetLocalByStringW (const wchar_t* str)
{
    int year = 0, mon = 0, day = 0, hour = 0, min = 0, sec = 0, milli = 0;
    if (ParseStrW(str, year, mon, day, hour, min, sec, milli))
    {
        m_time = Time(year, mon, day, hour, min, sec).GetTime();
        m_nMilli = milli;
    }
    else
    {
        throw Except (DATETIME_FMT, "Datetime format error. used as yyyy-mm-dd HH:MM:SS.xxxx");
    }
}

void MilliTime::SetUTCByStringA(const char * str)
{
    int year = 0, mon = 0, day = 0, hour = 0, min = 0, sec = 0, milli = 0;
    if (ParseStrA(str, year, mon, day, hour, min, sec, milli))
    {
        SetUTCTime(year, mon, day, hour, min, sec);
        m_nMilli = milli;
    }
	else
	{
		throw Except (DATETIME_FMT, "Datetime format error. used as yyyy-mm-dd HH:MM:SS.xxxx");
	}
}

void MilliTime::SetLocalByStringA(const char * str)
{
    int year = 0, mon = 0, day = 0, hour = 0, min = 0, sec = 0, milli = 0;
    if (ParseStrA(str, year, mon, day, hour, min, sec, milli))
    {
        m_time = Time(year, mon, day, hour, min, sec).GetTime();
        m_nMilli = milli;
    }
	else
	{
		throw Except (DATETIME_FMT, "Datetime format error. used as yyyy-mm-dd HH:MM:SS.xxxx");
	}
}