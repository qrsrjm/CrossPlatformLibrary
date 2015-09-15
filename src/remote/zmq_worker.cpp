#include "zmq/zmq.h"
#include "zmq_worker.h"
#include "zmq_msg.h"
#include "remote_def.h"
#include "invoke_object.h"
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
        int rc = zmq_poll(&pollItem, 1, 1000);
        if (rc <= 0)
        {
            continue;
        }

        int code = 0;
        ZmqMsg request;
        ZmqMsg reply;

        if (-1 != zmq_recvmsg(receiver, request.GetMsg(), 0))
        {
            HandleClientRequest(&request, &reply);

            zmq_sendmsg(receiver, reply.GetMsg(), 0);
        }
        else
        {
            zmq_close(receiver);
            void *receiver = zmq_socket(m_context, ZMQ_REP);
            zmq_connect(receiver, "inproc://workers");
        }
    }

    zmq_close(receiver);
}

void ZmqWorker::OnExit()
{
    
}

void ZmqWorker::HandleClientRequest( ZmqMsg* request, ZmqMsg *reply )
{
    AmfAsObjectPtr replyObjPtr = new AmfAsObject;
    AmfAsObjectPtr reqObjPtr = request->ReadObject();
    if (reqObjPtr == NULL)
    {
        replyObjPtr->AddData(L"code", (long)REMOTE_DATA_ERROR);
        replyObjPtr->AddData(L"what", L"data format error");
    }
    else
    {
        try
        {
            AmfStringPtr className = reqObjPtr->Find(L"class");
            AmfStringPtr funcName = reqObjPtr->Find(L"func");
            AmfObjectPtr params = reqObjPtr->Find(L"params");

            if ((NULL == className) || (NULL == funcName))
            {
                stringstream ss;
                ss << WToA(className->ToString()) << "::" << WToA(funcName->ToString()) << " not defined";
                throw Except(REMOTE_DATA_ERROR, ss.str().c_str());
            }

            if (NULL == params)
            {
                params = new AmfAsObject;
            }

            InvokeObject* pServiceObj = InvokeObject::GetObjectByClass(WToA(className->GetValue()).c_str());
            if (pServiceObj != NULL)
            {
                replyObjPtr->AddData(L"code", (long)REMOTE_NOERROR);

                AmfObjectPtr p = pServiceObj->Invoke(WToA(funcName->GetValue()), params);
                if (p == NULL)
                {
                    replyObjPtr->AddData(L"code", (long)REMOTE_RPC_ERROR);
                    replyObjPtr->AddData(L"what", L"func not found");
                }
                else
                {
                    replyObjPtr->AddData(L"ret", p);
                }
            }
            else
            {
                replyObjPtr->AddData(L"code", (long)REMOTE_RPC_ERROR);
                replyObjPtr->AddData(L"what", L"Create RPC object failed");
            }
        }
        catch (Except& e)
        {
            replyObjPtr->AddData(L"code", (long)REMOTE_PROTOCOL_ERROR);
            replyObjPtr->AddData(L"what", e.what());
        }
    }

    reply->WriteObject(replyObjPtr);
}