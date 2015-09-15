#include "zmq/zmq.h"
#include "zmq_server.h"

#include <iostream>
#include <sstream>

using namespace std;

ZmqServer::ZmqServer()
: m_context(NULL)
, m_frontend(NULL)
, m_backend(NULL)
{
	
}

ZmqServer::~ZmqServer(void)
{
	StopServer();
}

int ZmqServer::StartServer( int maxThread )
{
	m_context = zmq_ctx_new();
	BASE_ASSERT(NULL != m_context);

	m_frontend = zmq_socket(m_context, ZMQ_ROUTER);
	BASE_ASSERT(NULL != m_frontend);

	m_backend = zmq_socket(m_context, ZMQ_DEALER);
	BASE_ASSERT(NULL != m_backend);

	if (0 != zmq_bind(m_backend, "inproc://workers"))
	{
		cout << "绑定" << "inproc://workers" << "失败" << endl;
		return ERR_REMOTE_BIND;
	}

	if (maxThread < 1)
	{
		m_nMaxThread = 1;
	}
	else
	{
		m_nMaxThread = maxThread;
	}

	// 启动工作线程
	StartWorkThreads();

	// 启动服务线程
	this->Start();

	return REMOTE_NOERROR;
}

void ZmqServer::StopServer()
{
    // 退出工作线程
    StopWorkThreads();

	if (NULL != m_backend)
	{
		zmq_close(m_backend);
		m_backend = NULL;
	}

	if (NULL != m_frontend)
	{
		zmq_close(m_frontend);
		m_frontend = NULL;
	}

	if (NULL != m_context)
	{
		zmq_ctx_term(m_context);
		m_context = NULL;
	}
}

void ZmqServer::StartWorkThreads()
{
    for (int i = 0; i < m_nMaxThread; i++)
    {
        ZmqWorker *worker = new ZmqWorker(m_context);
        worker->Start();

        m_workers.push_back(worker);
    }
}

void ZmqServer::StopWorkThreads()
{
    int thrdCnt = m_workers.size();
     for (int i = 0; i < thrdCnt; i++)
     {
         ZmqWorker *worker = m_workers[i];
         if (worker != NULL)
         {
			 worker->Terminate();
			 worker->Wait();
 
             delete worker;
             worker = NULL;
         }
     }

    m_workers.clear();
}

void ZmqServer::Run()
{
    // 启动代理
	zmq_proxy(m_frontend, m_backend, NULL);
}

int ZmqServer::BindTcpServer(int nPort, const char *ipAddr /*= "*"*/)
{
	stringstream ss;
	ss << "tcp://" << ipAddr << ":" << nPort;
	if (0 != zmq_bind(m_frontend, ss.str().c_str()))
	{
		cout << "绑定地址" << ss.str() << "失败!" << endl;
		return ERR_REMOTE_BIND;
	}

	return REMOTE_NOERROR;
}

#ifndef WIN32
int ZmqServer::BindIpcServer(const char *ipcName)
{
	stringstream ss;
	ss << "ipc://" << ipcName;
	if (0 != zmq_bind(m_frontend, ss.str().c_str()))
	{
		cout << "绑定地址" << ss.str() << "失败!" << endl;
		return ERR_REMOTE_BIND;
	}

	return REMOTE_NOERROR;
}
#endif
