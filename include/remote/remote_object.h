#ifndef ZMQ_CLIENT_H
#define ZMQ_CLIENT_H

#include "remote_def.h"
#include "basic.h"
#include <string>
#include <list>

class ZmqMsg;

class REMOTE_EXPORT RemoteObject
{
public:
	RemoteObject();

    ~RemoteObject();

	void Init(const std::string& srvAddr, int port = 0);
	void Request(ZmqMsg *requestMsg, ZmqMsg *replyMsg);

private:
	int Connect();
	void Reset();

private:
	void *m_context;

    void *m_socket;
    std::string m_strAddress;
    int m_port;
    bool m_bInit;

    Mutex m_Mutex;
};
#endif