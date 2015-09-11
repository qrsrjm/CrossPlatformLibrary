#ifndef BASE_DEFINE_H
#define BASE_DEFINE_H

#if defined WIN32
#   if defined _LIB
#       define BASE_EXPORT
#   elif defined _DLL
#       define BASE_EXPORT __declspec(dllexport)
#   else
#       define BASE_EXPORT __declspec(dllimport)
#   endif
#else
#   if defined __SUNPRO_C  || defined __SUNPRO_CC
#       define BASE_EXPORT __global
#   elif (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER
#       define BASE_EXPORT __attribute__ ((visibility("default")))
#   else
#       define BASE_EXPORT
#   endif
#endif

#include "type_def.h"

#ifdef linux
extern long _wtoi(const wchar_t *ptr);
#endif

const uint32_t UNKNOW_ERROR        = 0x1001;    // unknow error
const uint32_t DATETIME_FMT        = 0x1002;    // Datetime format error. used as yyyy-mm-dd HH:MM:SS.xxxx

#endif