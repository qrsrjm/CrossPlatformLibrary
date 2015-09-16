#include "zmq/zmq.h"
#include "zmq_msg.h"
#include <sstream>
#include <memory.h>

using namespace std;

ZmqMsg::ZmqMsg(void)
: m_bInit(true)
{
    m_Msg = new zmq_msg_t;
	zmq_msg_init(m_Msg);
}

ZmqMsg::~ZmqMsg(void)
{
	Reset();
    delete m_Msg;
}

void ZmqMsg::Reset()
{
	if (m_bInit)
	{
		zmq_msg_close(m_Msg);
		m_bInit = false;
	}
}

size_t ZmqMsg::Size()
{
    return zmq_msg_size(m_Msg);
}

zmq_msg_t* ZmqMsg::GetMsg()
{
    return m_Msg;
}

void ZmqMsg::FillData( char* buffer, size_t buf_size )
{
	if (m_bInit)
	{
		Reset();
	}

    zmq_msg_init_size(m_Msg, buf_size);
	m_bInit = true;
	memcpy(zmq_msg_data(m_Msg), buffer, buf_size);
}

size_t ZmqMsg::GetData(char* buffer, size_t buf_size)
{
	size_t size = zmq_msg_size(m_Msg);
	if (buf_size < size)
	{
		return -1;
	}
	memcpy(buffer, zmq_msg_data(m_Msg), size);

	return size;
}
