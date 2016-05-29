#ifndef UTIL_TIMER_HPP
#define UTIL_TIMER_HPP

#include "thread.hpp"

namespace util
{

class Timer : private WorkerThread
{
public:
    explicit Timer() :
        delay_(0),
        period_(1000),
        run_(false) {}
    virtual ~Timer() { stop(); }
    inline bool start(size_t delay, size_t period)
    {
        if (isRunning())
            return true;
        delay_ = delay;
        period_ = period;
        return WorkerThread::start();
    }
    inline void stop() { run_ = false; WorkerThread::join(); }
    inline size_t getPeriod() { return period_; }
    inline void setPeriod(size_t period) { period_ = period; }
    inline bool isRunning() const { return run_; }
protected:
    virtual void run()
    {
        run_  = true;
        msleep(delay_);
        while (run_)
        {
            msleep(period_);
            runTask();
        }
    }
    virtual void runTask() = 0;
private:
    size_t delay_;
    size_t period_;
    bool run_;
private:
    DISALLOW_COPY_AND_ASSIGN(Timer)
};

template <class TimerTask>
class TaskObjTimer : Timer
{
public:
    explicit TaskObjTimer(UtilAutoPtr<TimerTask> ptimer_task) :
        ptimer_task_(ptimer_task)
        {}
    ~TaskObjTimer() { stop(); }
protected:
    virtual void runTask()
    {
        if (ptimer_task_.get())
            ptimer_task_->run();
    }
private:
    UtilAutoPtr<TimerTask> ptimer_task_;
private:
    DISALLOW_COPY_AND_ASSIGN(TaskObjTimer)
};

class TaskFuncTimer : public Timer
{
    typedef UtilFunction<void ()> TimerTask;
public:
    explicit TaskFuncTimer(TimerTask timer_task) :
        timer_task_(timer_task) {}
    ~TaskFuncTimer() { stop(); }
protected:
    virtual void runTask()
    {
        if (timer_task_)
            timer_task_();
    }
private:
    TimerTask timer_task_;
private:
    DISALLOW_COPY_AND_ASSIGN(TaskFuncTimer)
};

} // namespace util

#endif // UTIL_TIMER_HPP
