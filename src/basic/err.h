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

// �����Ķ��ԣ���assert��������
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

// ��GetLastError��Ӧ�Ĵ������ת��Ϊ������Ϣ
void WinError(char *buffer, size_t bufferSize);

// Windows�µ�GetLastError��ʽ�Ĵ������
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

// windows��errno��ʽ�Ĵ������
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

// Linux��errno��ʽ�Ĵ������
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

// POSIX��ʽ�Ĵ������
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


// �ڴ����ʧ�ܵĶ���
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