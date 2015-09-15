#include "zmq_server.h"
#include "zmq/zmq.h"

#include <iostream>
#include <sstream>

using namespace std;

ZmqServer::ZmqServer()
: m_context(NULL)
, m_frontend(NULL)
, m_backend(NULL)
{
	m_context = zmq_ctx_new();
	m_frontend = zmq_socket(m_context, ZMQ_ROUTER);
	m_backend = zmq_socket(m_context, ZMQ_DEALER);
}

ZmqServer::~ZmqServer(void)
{
	if (NULL != m_frontend)
    {
		zmq_close(m_frontend);
		m_frontend = NULL;
    }

	if (NULL != m_backend)
	{
		zmq_close(m_backend);
		m_backend = NULL;
	}

	if (NULL != m_context)
	{
		zmq_ctx_term(m_context);
		m_context = NULL;
	}
}

int ZmqServer::StartServer( int maxThread )
{
    if (NULL == m_context)
    {
        return REMOTE_PORT_USED;
    }

    m_nPort = nPort;

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

int ZmqServer::StartIpcServer( const char* szServerName, int maxThread )
{
    m_context = m_pZmqUtil->IpcListen(szServerName, 1);

    if (NULL == m_context)
    {
        return REMOTE_PORT_USED;
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
    this->StartThread(false);

    return REMOTE_NOERROR;
}

void ZmqServer::StopServer()
{
    // 退出工作线程
    StopWorkThreads();

    if (NULL != m_context)
    {
        m_pZmqUtil->CloseListen();
    }
}

void ZmqServer::StartWorkThreads()
{
    for (int i = 0; i < m_nMaxThread; i++)
    {
        ZmqWorker *worker = new ZmqWorker(m_context);
        worker->StartThread(0);

        m_workers->push_back(worker);
    }
}

void ZmqServer::StopWorkThreads()
{
    int thrdCnt = m_workers->size();
     for (int i = 0; i < thrdCnt; i++)
     {
         ZmqWorker *worker = (*m_workers)[i];
         if (worker != NULL)
         {
			 worker->Terminate(0);
 
             delete worker;
             worker = NULL;
         }
     }

    m_workers->clear();
}

void ZmqServer::Run()
{
    // 启动代理
    
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
