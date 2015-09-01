#include "err.h"

void SafeAbort(const char *errMsg)
{
#ifdef WIN32
	ULONG_PTR extra_info[1];
	extra_info[0] = (ULONG_PTR)errMsg;
	RaiseException(0x40000015, EXCEPTION_NONCONTINUABLE, 1, extra_info);
#else
	(void)errmsg_;
	abort();
#endif
}

#ifdef WIN32

void WinError(char *buffer, size_t bufferSize)
{
	DWORD errcode = GetLastError();
	DWORD rc = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errcode, MAKELANGID(LANG_NEUTRAL,
		SUBLANG_DEFAULT), buffer, (DWORD)bufferSize, NULL);
	BASE_ASSERT(rc);
}

#endif
