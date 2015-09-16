#ifndef ZMQ_MSG_H
#define ZMQ_MSG_H

#include "remote_def.h"

struct zmq_msg_t;
class REMOTE_EXPORT ZmqMsg
{
public:
    ZmqMsg(void);
    virtual ~ZmqMsg(void);

	void Reset();
    size_t Size();
    zmq_msg_t* GetMsg();

    void FillData(char* buffer, size_t buf_size);
	size_t GetData(char* buffer, size_t buf_size);

private:
    zmq_msg_t *m_Msg;
	bool m_bInit;
};
#endif