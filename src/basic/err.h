#ifndef ERR_INCLUDED_H
#define ERR_INCLUDED_H

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
#include "windows.h"
#endif

void SafeAbort(const char *errMsg);

// 基本的断言，与assert功能类似
#define BASE_ASSERT(x) \
	do \
	{ \
		if (!(x)) \
		{ \
			fprintf(stderr, "Assertion failed: %s (%s:%d)\n", #x, __FILE__, __LINE__); \
			SafeAbort(#x); \
		} \
	} while (false)

#ifdef WIN32

// 将GetLastError对应的错误代码转换为错误信息
void WinError(char *buffer, size_t bufferSize);

// Windows下的GetLastError样式的错误断言
#define WIN_ASSERT(x) \
	do \
	{ \
		if (!(x)) \
		{ \
			char errStr[256]; \
			WinError(errStr, 256); \
			fprintf(stderr, "Assertion failed: %s (%s:%d)\n", errStr, __FILE__, __LINE__); \
			SafeAbort(errStr); \
		} \
	} while (false)

// windows下errno样式的错误断言
#define ERRNO_ASSERT(x) \
	do \
	{ \
		if (!(x)) \
		{ \
			char errStr[256]; \
			strerror_s(errStr, 256, errno); \
			fprintf(stderr, "%s (%s:%d)\n", errStr, __FILE__, __LINE__); \
			SafeAbort(errStr); \
		} \
	} while (false)

#else

// Linux下errno样式的错误断言
#define ERRNO_ASSERT(x) \
	do \
	{ \
		if (!(x)) \
		{ \
			const char *errStr = strerror(errno); \
			fprintf(stderr, "%s (%s:%d)\n", errStr, __FILE__, __LINE__); \
			SafeAbort(errStr); \
		} \
	} while (false)

#endif

// POSIX样式的错误断言
#define POSIX_ASSERT(x) \
	do \
	{ \
		if (x) \
		{ \
			const char *errStr = strerror(x); \
			fprintf(stderr, "%s (%s:%d)\n", errStr, __FILE__, __LINE__); \
			SafeAbort(errStr); \
		} \
	} while (false)


// 内存分配失败的断言
#define ALLOC_ASSERT(x) \
	do \
	{ \
		if (!(x)) \
		{ \
			fprintf(stderr, "FATAL ERROR: OUT OF MEMORY (%s:%d)\n", \
			__FILE__, __LINE__); \
			SafeAbort("FATAL ERROR: OUT OF MEMORY"); \
		}\
	} while (false)

#endif