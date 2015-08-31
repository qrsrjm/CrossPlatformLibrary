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

void safe_abort(const char *errMsg);

// 基本的断言，与assert功能类似
#define base_assert(x) \
	do \
	{ \
		if (!(x)) \
		{ \
			fprintf(stderr, "Assertion failed: %s (%s:%d)\n", #x, __FILE__, __LINE__); \
			safe_abort(#x); \
		} \
	} while (false)

#ifdef WIN32

// 将GetLastError对应的错误代码转换为错误信息
void win_error(char *buffer, size_t bufferSize);

// Windows下的GetLastError样式的错误断言
#define win_assert(x) \
	do \
	{ \
		if (!(x)) \
		{ \
			char errStr[256]; \
			win_error(errStr, 256); \
			fprintf(stderr, "Assertion failed: %s (%s:%d)\n", errStr, __FILE__, __LINE__); \
			safe_abort(errStr); \
		} \
	} while (false)

#endif

// errno样式的错误断言
#define errno_assert(x) \
	do \
	{ \
		if (!(x)) \
		{ \
			const char *errStr = strerror(errno); \
			fprintf(stderr, "%s (%s:%d)\n", errStr, __FILE__, __LINE__); \
			safe_abort(errStr); \
		} \
	} while (false)

// POSIX样式的错误断言
#define posix_assert(x) \
	do \
	{ \
		if (x) \
		{ \
			const char *errStr = strerror(x); \
			fprintf(stderr, "%s (%s:%d)\n", errStr, __FILE__, __LINE__); \
			safe_abort(errStr); \
		} \
	} while (false)


// 内存分配失败的断言
#define alloc_assert(x) \
	do \
	{ \
		if (!(x)) \
		{ \
			fprintf(stderr, "FATAL ERROR: OUT OF MEMORY (%s:%d)\n", \
			__FILE__, __LINE__); \
			safe_abort("FATAL ERROR: OUT OF MEMORY"); \
		}\
	} while (false)

#endif