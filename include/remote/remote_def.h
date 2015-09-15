#ifndef RMT_DEF_H
#define RMT_DEF_H

#ifdef _WIN32
#   if defined _LIB
#       define REMOTE_EXPORT
#   elif defined _DLL
#       define REMOTE_EXPORT __declspec(dllexport)
#   else
#       define REMOTE_EXPORT __declspec(dllimport)
#   endif
#else
#   if defined __SUNPRO_C  || defined __SUNPRO_CC
#       define REMOTE_EXPORT __global
#   elif (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER
#       define REMOTE_EXPORT __attribute__ ((visibility("default")))
#   else
#       define REMOTE_EXPORT
#   endif
#endif

const int REMOTE_NOERROR      = 0;       // Œﬁ¥ÌŒÛ

const int REMOTE_MODEL = 0x7000;

const int ERR_REMOTE_BIND       = REMOTE_MODEL + 0x001;   // ∞Û∂®µÿ÷∑ ß∞‹

#endif