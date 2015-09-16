#ifndef ZMQ_SERVER_H
#define ZMQ_SERVER_H

#include "remote_def.h"
#include "basic.h"
#include <vector>

class ZmqWorker;

// 下面两行代码是为解决C4251警告
template class REMOTE_EXPORT std::vector < ZmqWorker * >;
template class REMOTE_EXPORT std::allocator < ZmqWorker * >;

class REMOTE_EXPORT ZmqServer : public ThreadBase
{
public:
	ZmqServer();
	virtual ~ZmqServer(void);

	int BindTcpServer(int nPort, const char *ipAddr = "*");
#ifndef WIN32
	int BindIpcServer(const char *ipcName);
#endif
	int StartServer(int maxThread);
	void StopServer();

protected:
	virtual void Run();

	void StartWorkThreads();
	void StopWorkThreads();

private:
	std::vector<ZmqWorker*> m_workers;

	void *m_context;
	void *m_frontend;
	void *m_backend;

    int m_nPort;
	int m_nMaxThread;
};
#endif