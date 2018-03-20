#include "thread.hpp"
#ifdef _PLATFORM_WINDOWS_
#include <process.h>
#include <windows.h>
#endif
#ifdef _PLATFORM_UNIX_
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#ifdef _ANDROID_
#include <thread_db.h>
#endif
#endif

namespace util
{

#ifdef _PLATFORM_WINDOWS_
void sleep(size_t s)
{
    ::Sleep(s * 1000);
}

void msleep(size_t ms)
{
    ::Sleep(ms);
}

unsigned long getCurrentThreadId()
{
    return ::GetCurrentThreadId();
}

struct Mutex::MutexData
{
   HANDLE hmutex_;
   //CRITICAL_SECTION cs_;
};

Mutex::Mutex() :
    pdata_(new MutexData())
{
    pdata_->hmutex_ = ::CreateMutex(NULL, FALSE, NULL);
    //::InitializeCriticalSection(&pdata_->cs_);
}
Mutex::~Mutex()
{
    ::CloseHandle(pdata_->hmutex_);
    //::DeleteCriticalSection(&pdata_->cs_);
}

void Mutex::lock()
{
    ::WaitForSingleObject(pdata_->hmutex_, INFINITE);
    //::EnterCriticalSection(&pdata_->cs_);
}

void Mutex::unlock()
{
    ::ReleaseMutex(pdata_->hmutex_);
    //::LeaveCriticalSection(&pdata_->cs_);
}

struct Lock::LockData
{
    HANDLE hlock_;
};

Lock::Lock() :
    pdata_(new LockData())
{
    pdata_->hlock_ = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

Lock::~Lock()
{
    ::CloseHandle(pdata_->hlock_);
}

void Lock::wait(bool reset)
{
    if(reset)
        ::ResetEvent(pdata_->hlock_);
    ::WaitForSingleObject(pdata_->hlock_, INFINITE);
}

bool Lock::timedWait(size_t ms, bool reset)
{
    if(reset)
        ::ResetEvent(pdata_->hlock_);
    return WAIT_OBJECT_0 == ::WaitForSingleObject(pdata_->hlock_, ms) ? true : false;
}

void Lock::notify()
{
    ::SetEvent(pdata_->hlock_);
}

struct Thread::ThreadData
{
    typedef UtilFunction<void ()> ThreadFunc;
    ThreadData(ThreadFunc tf) :
        tf_(tf),
        hthread_(0),
        id_(0)
    {}
    ThreadFunc tf_;
    HANDLE hthread_;
    unsigned int id_;
};

Thread::Thread(ThreadFunc tf) :
    pdata_(new ThreadData(tf))
{}

Thread::~Thread()
{
    ::CloseHandle(pdata_->hthread_);
}

bool Thread::start(size_t stack_size)
{
    ThreadFunc* pcf = new ThreadFunc(pdata_->tf_);
    pdata_->hthread_ = (HANDLE)::_beginthreadex(0,
                                        stack_size,
                                        (unsigned int (__stdcall*)(void*))Thread::threadFunc,
                                        reinterpret_cast<void*>(pcf),
                                        0,
                                        &pdata_->id_);

    return (pdata_->hthread_) ? true : false;
}

void Thread::join()
{
    //return WAIT_OBJECT_0 == ::WaitForSingleObject(pdata_->hthread_, INFINITE) ? true : false;
    ::WaitForSingleObject(pdata_->hthread_, INFINITE);
}

void Thread::kill()
{
    ::TerminateThread(pdata_->hthread_, 0);
    ::CloseHandle(pdata_->hthread_);
    pdata_->hthread_ = 0;
}

unsigned int Thread::threadFunc(void* param)
{
    if (!param)
        return -1;
    UtilAutoPtr<ThreadFunc> ap_tf(reinterpret_cast<ThreadFunc*>(param));
    if(*ap_tf)
        (*ap_tf)();

    return 0;
}

#endif // _PLATFORM_WINDOWS_

#ifdef _PLATFORM_UNIX_

#ifdef __APPLE__

void timedwait_notify_tdfunc(sem_t* sem, size_t ms, int* ptimeout)
{
    msleep(ms);
    *ptimeout = -1;
    ::sem_post(sem);
}

int sem_timedwait(sem_t* sem, size_t ms)
{    
    int timeout = 0;
    Thread td(UtilBind(timedwait_notify_tdfunc, sem, ms, &timeout));
    td.start();
    ::sem_wait(sem);
    td.kill();
    return timeout;
}

#endif    

void sleep(size_t s)
{
    ::sleep(s);
}

void msleep(size_t ms)
{
    ::usleep(ms * 1000);
}

unsigned long getCurrentThreadId()
{
    return (unsigned long)pthread_self();
}

struct Mutex::MutexData
{
   pthread_mutex_t mutex_;
};

Mutex::Mutex() :
    pdata_(new MutexData())
{
#ifdef __APPLE__
    ::pthread_mutex_init(&pdata_->mutex_, NULL);
#else
    pthread_mutexattr_t mattr;
    pthread_mutexattr_settype(&mattr,PTHREAD_MUTEX_RECURSIVE_NP);
    ::pthread_mutex_init(&pdata_->mutex_, &mattr);
#endif
}
Mutex::~Mutex()
{
    ::pthread_mutex_destroy(&pdata_->mutex_);
}

void Mutex::lock()
{
     ::pthread_mutex_lock(&pdata_->mutex_);
}

void Mutex::unlock()
{
     ::pthread_mutex_unlock(&pdata_->mutex_);
}

struct Lock::LockData
{
#ifdef __APPLE__
    sem_t* psem_;
#else
    sem_t sem_;
#endif
    bool notify_;
};

Lock::Lock() :
    pdata_(new LockData())
{
#ifdef __APPLE__
    std::string name = strFormat("sem_%d", this);
    ::sem_unlink(name.c_str()); //check?
    pdata_->psem_ = ::sem_open(name.c_str(), O_CREAT, O_RDWR, 0);
#else
    ::sem_init(&pdata_->sem_, 0, 0);
#endif
}

Lock::~Lock()
{
#ifdef __APPLE__
    ::sem_close(pdata_->psem_);
#else
    ::sem_destroy(&pdata_->sem_);
#endif
}

void Lock::wait(bool reset)
{
    pdata_->notify_ = false;
    if (reset)
    {
        while(!pdata_->notify_)
        {
#ifdef __APPLE__
            ::sem_wait(pdata_->psem_);
#else
            ::sem_wait(&pdata_->sem_);
#endif
        }
    }
    else
    {
#ifdef __APPLE__
        ::sem_wait(pdata_->psem_);
#else
        ::sem_wait(&pdata_->sem_);
#endif
    }
}

bool Lock::timedWait(size_t ms, bool reset)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) < 0)
        return false;

    ts.tv_sec += (ms / 1000);
    ts.tv_nsec += ((ms % 1000) * 1000 * 1000);
    ts.tv_sec += ts.tv_nsec / 1e+9; // Nanoseconds [0 .. 999999999]
    ts.tv_nsec = ts.tv_nsec % (int)1e+9;

    pdata_->notify_ = false;
    if (reset)
    {
        int ret(0);
        while(!pdata_->notify_ && 0 == ret)
        {
#ifdef __APPLE__
            ret = sem_timedwait(pdata_->psem_, ms);
#else
            ret = sem_timedwait(&pdata_->sem_, &ts);
#endif
        }

        return 0 == ret;
    }
    else
    {
#ifdef __APPLE__
            return 0 == sem_timedwait(pdata_->psem_, ms);
#else
            return 0 == sem_timedwait(&pdata_->sem_, &ts);
#endif
    }
//#endif
}

void Lock::notify()
{
    pdata_->notify_ = true;
#ifdef __APPLE__
    ::sem_post(pdata_->psem_);
#else
    ::sem_post(&pdata_->sem_);
#endif
}

struct Thread::ThreadData
{
    typedef UtilFunction<void ()> ThreadFunc;
    ThreadData(ThreadFunc tf) :
        tf_(tf)
    {}
    ThreadFunc tf_;
    pthread_t a_thread_;
};

Thread::Thread(ThreadFunc tf) :
    pdata_(new ThreadData(tf))
{}

Thread::~Thread()
{}

bool Thread::start(size_t stack_size)
{
    ThreadFunc* pcf = new ThreadFunc(pdata_->tf_);

    int attr_ret(-1);
    pthread_attr_t attr;
    pthread_attr_t* pattr = 0;

    if (stack_size > 0)
    {
        pthread_attr_t attr;
        attr_ret = pthread_attr_init(&attr);
        if(0 == attr_ret)
        {
            if(0 == pthread_attr_setstacksize(&attr, stack_size))
            {
                pattr = &attr;
            }
        }
    }

    int res = ::pthread_create(&pdata_->a_thread_, pattr, (void* (*)(void*))Thread::threadFunc, pcf);

    if (0 == attr_ret)
        pthread_attr_destroy(&attr);

    return (0 == res) ? true : false;
}

void Thread::join()
{
    void* pthread_result(0);
    ::pthread_join(pdata_->a_thread_, &pthread_result);
    //return (0 == res) ? true : false;
}

void Thread::kill()
{
	//not define _ANDROID_ !!!
#ifndef _ANDROID_
    pthread_cancel(pdata_->a_thread_);
#endif
}

unsigned int Thread::threadFunc(void* param)
{
    if (!param)
        return -1;
    //not define _ANDROID_ !!!
#ifndef _ANDROID_
    int res = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    if (0 != res)
        return -1;
    res = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
    if (0 != res)
        return -1;
#endif
    UtilAutoPtr<ThreadFunc> ap_tf(reinterpret_cast<ThreadFunc*>(param));
    if(*ap_tf)
        (*ap_tf)();
    ::pthread_exit(0);
    return 0;
}
#endif //_PLATFORM_LINUX_

} // namespace util
