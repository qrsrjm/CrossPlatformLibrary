#include "zmq/zmq.h"
#include "zmq_worker.h"
#include "zmq_msg.h"
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
		// 使用poll机制而不是阻塞等待的原因是让线程有机会退出
		if ((ret = zmq_poll(&pollItem, 1, 1000)) <= 0)
		{
			// 读取超时
			if (ret == 0) {	continue; }

			// 上下文被关闭，安全退出
			if (ETERM == errno)
			{
				int val = 0;
				zmq_setsockopt(receiver, ZMQ_LINGER, &val, sizeof(int));
				zmq_close(receiver);
				return;
			}

			// 其它错误（pollItem无效/被信号中断）
			ERRNO_ASSERT(0);
		}

		ZmqMsg requestMsg, replyMsg;

		if (zmq_recvmsg(receiver, requestMsg.GetMsg(), 0) < 0)
		{
			// 上下文被关闭，安全退出
			if (ETERM == errno)
			{
				int val = 0;
				zmq_setsockopt(receiver, ZMQ_LINGER, &val, sizeof(int));
				zmq_close(receiver);
				return;
			}

			// 其它错误
			ERRNO_ASSERT(0);
		}

		HandleClientRequest(&requestMsg, &replyMsg);

		if (zmq_sendmsg(receiver, replyMsg.GetMsg(), 0) < 0)
		{
			// 上下文被关闭，安全退出
			if (ETERM == errno)
			{
				int val = 0;
				zmq_setsockopt(receiver, ZMQ_LINGER, &val, sizeof(int));
				zmq_close(receiver);
				return;
			}

			// 其它错误
			ERRNO_ASSERT(0);
		}
    }

    zmq_close(receiver);
}

void ZmqWorker::HandleClientRequest(ZmqMsg *request, ZmqMsg *reply)
{
    
}

