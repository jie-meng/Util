#ifndef UTIL_THREAD_HPP
#define UTIL_THREAD_HPP

#include "base.hpp"
#include <vector>
#include <deque>

namespace util
{

void sleep(size_t s);

void msleep(size_t ms);

unsigned long getCurrentThreadId();

class Mutex
{
public:
    Mutex();
    ~Mutex();
    void lock();
    void unlock();
private:
    struct MutexData;
    UtilAutoPtr<MutexData> pdata_;
private:
    DISALLOW_COPY_AND_ASSIGN(Mutex)
};

class Synchronize
{
public:
    explicit Synchronize(Mutex& mutex) : mutex_(mutex) { mutex_.lock(); }
    ~Synchronize() { mutex_.unlock(); }
private:
    Mutex& mutex_;
private:
    DISALLOW_COPY_AND_ASSIGN(Synchronize)
};

class Lock
{
public:
    Lock();
    ~Lock();
    void wait(bool reset = true);
    bool timedWait(size_t ms, bool reset = true);
    void notify();
private:
    struct LockData;
    UtilAutoPtr<LockData> pdata_;
private:
    DISALLOW_COPY_AND_ASSIGN(Lock)
};

class MultiLock
{
public:
    MultiLock(size_t lock_cnt, bool wait_all);
    ~MultiLock();
    void wait(bool reset = true);
    bool timedWait(size_t ms, bool reset = true);
    void notify(size_t index);
    void notifyAll();
private:
    struct MultiLockData;
    UtilAutoPtr<MultiLockData> pdata_;
private:
    DISALLOW_COPY_AND_ASSIGN(MultiLock)
};

class Thread
{
    typedef UtilFunction<void ()> ThreadFunc;
public:
    explicit Thread(ThreadFunc tf);
    ~Thread();
    bool start(size_t stack_size = 0);
    bool join();
    void kill();
private:
    static unsigned int threadFunc(void* param);
private:
    struct ThreadData;
    UtilAutoPtr<ThreadData> pdata_;
private:
    DISALLOW_COPY_AND_ASSIGN(Thread)
};

class WorkerThread
{
public:
    WorkerThread() :
        thread_(UtilBind(&WorkerThread::run, this))
    {}
    virtual ~WorkerThread() {}
    inline bool start() { return thread_.start(); }
    inline bool join() { return thread_.join(); }
    inline void kill() { thread_.kill(); }
protected:
    virtual void run() = 0;
private:
    Thread thread_;
private:
    DISALLOW_COPY_AND_ASSIGN(WorkerThread)
};

template <typename T>
class MsgThread
{
public:
    typedef UtilSharedPtr<T> MsgPtr;
    explicit MsgThread();
    virtual ~MsgThread();
    inline bool isRunning() const { return is_running_; }
    bool start();
    void stop();
    void queueMsg(MsgPtr pmsg);
protected:
    virtual void msgProc(MsgPtr pmsg) = 0;
private:
    inline void waitForKey() { if (!key_) lock_.wait(); }
    inline void resetKey() { key_ = false; }
    inline void setKey() { key_ = true; lock_.notify(); }
    inline MsgPtr popMsg()
    {
        Synchronize sc(mutex_);
        MsgPtr pmsg(msg_queqe_.front());
        msg_queqe_.pop_front();
        return pmsg;
    }
    inline void clearMsgQueue()
    {
        Synchronize sc(mutex_);
        while (!msg_queqe_.empty())
            msg_queqe_.pop_front();
    }
    void run();
    bool isEmpty();
    void resetKeyIfEmpty();
private:
    UtilAutoPtr<Thread> pthread_;
    std::deque<MsgPtr> msg_queqe_;
    bool is_running_;
    bool key_;
    Lock lock_;
    Lock start_lock_;
    Mutex mutex_;
private:
    DISALLOW_COPY_AND_ASSIGN(MsgThread)
};

template <typename T>
MsgThread<T>::MsgThread() :
    pthread_(new Thread(UtilBind(&MsgThread::run, this))),
    is_running_(false),
    key_(false)
{}

template <typename T>
MsgThread<T>::~MsgThread()
{
    stop();
}

template <typename T>
bool MsgThread<T>::start() 
{ 
    if (isRunning())
        return false;
    
    if (!pthread_->start())
        return false;
    
    start_lock_.wait();
    return true;
}

template <typename T>
void MsgThread<T>::stop()
{
    if (is_running_)
    {
        queueMsg(MsgPtr());
        if (pthread_.get())
            pthread_->join();
    }
}

template <typename T>
void MsgThread<T>::queueMsg(MsgPtr pmsg)
{
    if (is_running_)
    {
        {
            Synchronize sc(mutex_);
            msg_queqe_.push_back(pmsg);
        }
        setKey();
    }
}

template <typename T>
bool MsgThread<T>::isEmpty()
{
    Synchronize sc(mutex_);
    return msg_queqe_.empty();
}

template <typename T>
void MsgThread<T>::resetKeyIfEmpty()
{
    Synchronize sc(mutex_);
    if (msg_queqe_.empty())
        resetKey();
}

template <typename T>
void MsgThread<T>::run()
{
    is_running_ = true;
    start_lock_.notify();
    while (is_running_)
    {
        waitForKey();

        if (!isEmpty())
        {
            MsgPtr pmsg(popMsg());

            if (0 != pmsg.get())
            {
                msgProc(pmsg);
            }
            else
            {
                clearMsgQueue();
                break;
            }
        }

        resetKeyIfEmpty();
    }
    is_running_ = false;
}

} // namespace util

#endif // UTIL_THREAD_HPP
