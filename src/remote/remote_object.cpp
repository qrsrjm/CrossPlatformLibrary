#include "zmq/zmq.h"

#include "remote_object.h"
#include "zmq_msg.h"

#include <sstream>
#include <string.h>

using namespace std;

RemoteObject::RemoteObject()
: m_socket(NULL)
, m_port(0)
, m_bInit(false)
{
    m_context = zmq_ctx_new();
	BASE_ASSERT(NULL != m_context);
}

RemoteObject::~RemoteObject(void)
{
    if (m_socket != NULL)
    {
		int val = 0;
		zmq_setsockopt(m_socket, ZMQ_LINGER, &val, sizeof(int));
        zmq_close(m_socket);
    }
    
	zmq_ctx_term(m_context);
}

void RemoteObject::Init(const std::string& srvAddr, int port)
{
	m_strAddress = srvAddr;
	m_port = port;

	if (0 == Connect())
	{
		m_bInit = true;
	}
	else
	{
		throw Except(ERR_REMOTE_CONN_FAILED, "连接远程服务器失败");
	}
}

int RemoteObject::Connect()
{
	stringstream url;

	if (m_port == 0)
	{
		url << "ipc://" << m_strAddress;
	}
	else
	{
		url << "tcp://" << m_strAddress << ":" << m_port;
	}

	if (NULL == m_socket)
	{
		m_socket = zmq_socket(m_context, ZMQ_REQ);
	}

	return zmq_connect(m_socket, url.str().c_str());
}

void RemoteObject::Reset()
{
	int val = 0;
	zmq_setsockopt(m_socket, ZMQ_LINGER, &val, sizeof(int));
    zmq_close(m_socket);
    m_socket = NULL;

    zmq_ctx_term(m_context);
	m_context = zmq_ctx_new();
	BASE_ASSERT(NULL != m_context);

    Connect();
}

void RemoteObject::Request(ZmqMsg *requestMsg, ZmqMsg *replyMsg)
{
    if (!m_bInit)
    {
		throw Except(ERR_REMOTE_NOT_INIT, "远程对象未初始化");
    }

    HT_CS(m_Mutex);

	zmq_sendmsg(m_socket, requestMsg->GetMsg(), 0);

	zmq_pollitem_t poolItem = { m_socket, 0, ZMQ_POLLIN, 0 };
    int rc = zmq_poll(&poolItem, 1, 7000);
    if (rc == -1)
    {
        Reset();
		throw Except(ERR_REMOTE_SOCKET_ERROR, "socket错误，请重试");
    }
    else if (rc == 0)
    {
        Reset();
		throw Except(ERR_REMOTE_TIMEOUT, "请求超时，服务器可能忙, 请稍后再试");
    }
    else
    {
		zmq_recvmsg(m_socket, replyMsg->GetMsg(), 0);
    }
}

