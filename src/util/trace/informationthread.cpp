#include "informationthread.hpp"
#include <algorithm>

namespace util
{

using namespace std;

unsigned int threadFunc(void* param)
{
    InformationThread* pinfo_thread = (InformationThread*)param;

    pinfo_thread->is_running_ = true;
    while (pinfo_thread->is_running_)
    {
        pinfo_thread->waitForKey();

        if (!pinfo_thread->isMsgQueueEmpty())
        {
            Information* pmsg(pinfo_thread->popMsg());

            if (0 != pmsg)
            {
                pinfo_thread->informationProcess(pmsg);
                delete pmsg;
            }
            else
            {
                pinfo_thread->clearMsgQueue();
                break;
            }
        }

        pinfo_thread->resetKeyIfMsgQueueEmpty();
    }
    pinfo_thread->is_running_ = false;

    return 0;
}

InformationThread::InformationThread(bool listener_sync) :
    thread_(UtilBind(threadFunc, this)),
    is_running_(false),
    key_(false),
    listener_sync_(listener_sync)
{}

InformationThread::~InformationThread()
{
    stop();
}

void InformationThread::stop()
{
    if (is_running_)
    {
        queueMsg(0);
        thread_.join();
    }
}

void InformationThread::queueMsg(Information* pinfo)
{
    if (is_running_)
    {
        {
            Synchronize sc(msg_queue_mutex_);
            msg_queqe_.push_back(pinfo);
        }
        setKey();
    }
}

Information* InformationThread::popMsg()
{
    Synchronize sc(msg_queue_mutex_);
    Information* pinfo(msg_queqe_.front());
    msg_queqe_.pop_front();
    return pinfo;
}

void InformationThread::clearMsgQueue()
{
    Synchronize sc(msg_queue_mutex_);
    while (!msg_queqe_.empty())
    {
        InformationQueue::iterator it;
        for (it = msg_queqe_.begin(); it != msg_queqe_.end(); ++it)
            delete *it;

        msg_queqe_.clear();
    }
}

bool InformationThread::isMsgQueueEmpty()
{
    Synchronize sc(msg_queue_mutex_);
    return msg_queqe_.empty();
}

void InformationThread::resetKeyIfMsgQueueEmpty()
{
    Synchronize sc(msg_queue_mutex_);
    if (msg_queqe_.empty())
        resetKey();
}

void InformationThread::informationProcess(Information* pinfo)
{
    if (listener_sync_)
    {
        Synchronize sc(listener_mutex_);
        informationProcessAsync(pinfo);
    }
    else
    {
        informationProcessAsync(pinfo);
    }
}

void InformationThread::informationProcessAsync(Information* pinfo)
{
    for (PInfoListenerList::iterator it = pinfo_listener_list.begin(); it != pinfo_listener_list.end(); ++it)
        (*it)->process(pinfo);
}

bool InformationThread::addListener(InformationListener* listener)
{
    if (listener_sync_)
    {
        Synchronize sc(listener_mutex_);
        return addListenerAsync(listener);
    }
    else
    {
        return addListenerAsync(listener);
    }
}

bool InformationThread::removeListener(InformationListener* listener)
{
    if (listener_sync_)
    {
        Synchronize sc(listener_mutex_);
        return removeListenerAsync(listener);
    }
    else
    {
        return removeListenerAsync(listener);
    }
}

void InformationThread::clearAllListeners()
{
    if (listener_sync_)
    {
        Synchronize sc(listener_mutex_);
        clearAllListenersAsync();
    }
    else
    {
        clearAllListenersAsync();
    }
}

bool InformationThread::addListenerAsync(InformationListener* listener)
{
    PInfoListenerList::iterator it = find(pinfo_listener_list.begin(), pinfo_listener_list.end(), listener);
    if (it == pinfo_listener_list.end())
    {
        pinfo_listener_list.push_back(listener);
        return true;
    }

    return false;
}

bool InformationThread::removeListenerAsync(InformationListener* listener)
{
    PInfoListenerList::iterator it = find(pinfo_listener_list.begin(), pinfo_listener_list.end(), listener);
    if (it == pinfo_listener_list.end())
        return false;

    pinfo_listener_list.erase(it);
    return true;
}

void InformationThread::clearAllListenersAsync()
{
    pinfo_listener_list.clear();
}

} //namespace util
