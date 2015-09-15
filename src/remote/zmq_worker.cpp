#include "zmq/zmq.h"
#include "zmq_worker.h"
#include <string.h>

ZmqWorker::ZmqWorker(void* context)
: m_context(context)
{
}

ZmqWorker::~ZmqWorker(void)
{
}

void ZmqWorker::Run()
{
    void *receiver = zmq_socket(m_context, ZMQ_REP);
    zmq_connect(receiver, "inproc://workers");

    zmq_pollitem_t pollItem = {receiver, 0, ZMQ_POLLIN, 0};
    while (!IsBreak())
    {
        int ret;
		if ((ret = zmq_poll(&pollItem, 1, 1000)) <= 0)
		{
			// ��ȡ��ʱ
			if (ret == 0)
			{
				continue;
			}

			// �����ı��رգ���ȫ�˳�
			if (ETERM == errno)
			{
				int val = 0;
				zmq_setsockopt(receiver, ZMQ_LINGER, &val, sizeof(int));
				zmq_close(receiver);
				return;
			}

			// ��������pollItem��Ч/���ź��жϣ�
			ERRNO_ASSERT(0);
		}

		zmq_msg_t reqMsg, repMsg;

		if (zmq_recvmsg(receiver, &reqMsg, 0) < 0)
		{
			// �����ı��رգ���ȫ�˳�
			if (ETERM == errno)
			{
				int val = 0;
				zmq_setsockopt(receiver, ZMQ_LINGER, &val, sizeof(int));
				zmq_close(receiver);
				return;
			}

			// ��������
			ERRNO_ASSERT(0);
		}

		HandleClientRequest(&reqMsg, &repMsg);

		if (zmq_sendmsg(receiver, &repMsg, 0) < 0)
		{
			// �����ı��رգ���ȫ�˳�
			if (ETERM == errno)
			{
				int val = 0;
				zmq_setsockopt(receiver, ZMQ_LINGER, &val, sizeof(int));
				zmq_close(receiver);
				return;
			}

			// ��������
			ERRNO_ASSERT(0);
		}
    }

    zmq_close(receiver);
}

void ZmqWorker::HandleClientRequest(void *request, void *reply)
{
    
}

