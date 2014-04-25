#ifndef UTIL_TRACE_INFORMATIONTHREAD_HPP
#define UTIL_TRACE_INFORMATIONTHREAD_HPP

#include <list>
#include <deque>
#include "util/thread.hpp"
#include "information.hpp"

namespace util
{

class InformationThread
{
public:
    explicit InformationThread(bool listener_sync);
    ~InformationThread();
    inline bool start() { return thread_.start(); }
    void stop();
    void queueMsg(Information* pinfo);
    inline bool isRunning() const { return is_running_; }

    bool addListenerAsync(InformationListener* listener);
    bool removeListenerAsync(InformationListener* listener);
    void clearAllListenersAsync();
    bool addListener(InformationListener* listener);
    bool removeListener(InformationListener* listener);
    void clearAllListeners();
private:
    friend unsigned int threadFunc(void* param);
private:
    inline void waitForKey() { if (!key_) lock_.wait(); }
    inline void resetKey() { key_ = false; }
    inline void setKey() { key_ = true; lock_.notify(); }
    Information* popMsg();
    void clearMsgQueue();
    bool isMsgQueueEmpty();
    void resetKeyIfMsgQueueEmpty();
    void informationProcess(Information* pinfo);
    void informationProcessAsync(Information* pinfo);
private:
    typedef std::list<InformationListener*> PInfoListenerList;
    typedef std::deque<Information*> InformationQueue;

    util::Thread thread_;
    bool is_running_;
    bool key_;
    bool listener_sync_;
    PInfoListenerList pinfo_listener_list;
    InformationQueue msg_queqe_;
    util::Lock lock_;
    util::Mutex msg_queue_mutex_;
    util::Mutex listener_mutex_;
private:
    DISALLOW_COPY_AND_ASSIGN(InformationThread)
};

} //namespace util

#endif // UTIL_TRACE_INFORMATIONTHREAD_HPP
