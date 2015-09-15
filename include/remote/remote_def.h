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

#define REMOTE_SERVER  0
#define LOCALE_SERVER  1

const int REMOTE_NOERROR      = 0;       // �޴���

const int REMOTE_MODEL = 0x7000;

const int ERR_REMOTE_BIND       = REMOTE_MODEL + 0x001;   // �󶨵�ַʧ��
const int REMOTE_SOCKET_ERROR   = REMOTE_MODEL + 0x002;   // SOCKET����
const int REMOTE_DATA_ERROR     = REMOTE_MODEL + 0x003;   // ���ݴ���
const int REMOTE_PORT_USED      = REMOTE_MODEL + 0x004;   // �˿��ѱ�ռ��
const int REMOTE_PROTOCOL_ERROR = REMOTE_MODEL + 0x005;   // �����Э��
const int REMOTE_RPC_ERROR      = REMOTE_MODEL + 0x006;   // Զ�̵���ʧ��
const int REMOTE_NOT_VERIFIED   = REMOTE_MODEL + 0x007;   // δ��֤
const int REMOTE_NOT_INIT       = REMOTE_MODEL + 0x008;   // δ��ʼ��
const int REMOTE_CONN_POOL_BUSY = REMOTE_MODEL + 0x009;   // ���ӳ�æ
const int REMOTE_CONN_FAILED    = REMOTE_MODEL + 0x00A;   // ����ʧ��

#endif