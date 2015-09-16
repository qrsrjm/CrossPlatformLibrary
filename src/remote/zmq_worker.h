#ifndef ZMQ_WORKER_H
#define ZMQ_WORKER_H

#include "basic.h"
#include "remote_def.h"

class ZmqMsg;

class ZmqWorker :
	public ThreadBase
{
public:
	ZmqWorker(void* context);
	virtual ~ZmqWorker(void);

	virtual void Run();

private:
	void HandleClientRequest(ZmqMsg *request, ZmqMsg *reply);

private:
	void *m_context;
};


#endif