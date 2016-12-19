#include "testcase_thread.hpp"
#include <vector>

using namespace std;
using namespace util;

void TestCaseThread::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseThread, testSleep)
    REGISTER_TEST_FUNCTION(TestCaseThread, testGetCurrentThreadId)
    REGISTER_TEST_FUNCTION(TestCaseThread, testThread)
    REGISTER_TEST_FUNCTION(TestCaseThread, testWorkerThread)
    REGISTER_TEST_FUNCTION(TestCaseThread, testMsgThread)
    REGISTER_TEST_FUNCTION(TestCaseThread, testMutex)
    REGISTER_TEST_FUNCTION(TestCaseThread, testLock)
    REGISTER_TEST_FUNCTION(TestCaseThread, testLockTimedWait)
    REGISTER_TEST_FUNCTION(TestCaseThread, testMultiLockTimedWait)
}

void TestCaseThread::setUp()
{
}

void TestCaseThread::tearDown()
{    
}

void TestCaseThread::testSleep()
{
    DateTime dt1 = DateTime::now();
    sleep(1);
    DateTime dt2 = DateTime::now();    
    assertEquals<double>(1, dt2.drift(dt1), ASSERT_POSITION);
    msleep(1000);
    DateTime dt3 = DateTime::now();
    assertEquals<double>(2, dt3.drift(dt1), ASSERT_POSITION);
}

void TestCaseThread::testGetCurrentThreadId()
{
    assertTrue(util::getCurrentThreadId() > 0, ASSERT_POSITION);
}

void threadFunc(int* pthreadId)
{
    *pthreadId = getCurrentThreadId();
}

void TestCaseThread::testThread()
{
    int threadId = 0;
    Thread td(UtilBind(threadFunc, &threadId));
    assertTrue(td.start(), ASSERT_POSITION);
    assertTrue(td.join(), ASSERT_POSITION);
    assertTrue(threadId > 0, ASSERT_POSITION);
    assertNotEquals<int>(getCurrentThreadId(), threadId, ASSERT_POSITION);
}

class DerivedWorkerThread : public WorkerThread
{
public:
    DerivedWorkerThread() : threadId_(0) {}
    inline int getThreadId() const { return threadId_; }
protected:
    virtual void run()
    {
        threadId_ = getCurrentThreadId();
    }
private:
    int threadId_ = 0;
private:
    DISALLOW_COPY_AND_ASSIGN(DerivedWorkerThread)
};

void TestCaseThread::testWorkerThread()
{
    DerivedWorkerThread td;
    assertEquals<int>(0, td.getThreadId(), ASSERT_POSITION);
    assertTrue(td.start(), ASSERT_POSITION);
    assertTrue(td.join(), ASSERT_POSITION);
    assertTrue(td.getThreadId() > 0, ASSERT_POSITION);
    assertNotEquals<int>(getCurrentThreadId(), td.getThreadId(), ASSERT_POSITION);
}

class MsgProcessThread : public MsgThread<int>
{
public:
    MsgProcessThread() {}
    vector<int> vec;
protected:
    virtual void msgProc(MsgPtr pmsg)
    {
        vec.push_back(*pmsg);
    }
private:
    DISALLOW_COPY_AND_ASSIGN(MsgProcessThread)
};

void TestCaseThread::testMsgThread()
{
    MsgProcessThread td;
    assertFalse(td.isRunning(), ASSERT_POSITION);
    assertTrue(td.vec.empty(), ASSERT_POSITION);
    td.queueMsg(MsgProcessThread::MsgPtr(new int(1)));
    td.queueMsg(MsgProcessThread::MsgPtr(new int(2)));
    td.queueMsg(MsgProcessThread::MsgPtr(new int(3)));
    msleep(200);
    assertTrue(td.vec.empty(), ASSERT_POSITION);
    assertTrue(td.start(), ASSERT_POSITION);
    assertTrue(td.isRunning(), ASSERT_POSITION);
    td.queueMsg(MsgProcessThread::MsgPtr(new int(1)));
    td.queueMsg(MsgProcessThread::MsgPtr(new int(2)));
    td.queueMsg(MsgProcessThread::MsgPtr(new int(3)));
    td.stop();
    assertFalse(td.isRunning(), ASSERT_POSITION);
    assertEquals<int>(3, td.vec.size(), ASSERT_POSITION);
    assertEquals<int>(1, td.vec[0], ASSERT_POSITION);
    assertEquals<int>(2, td.vec[1], ASSERT_POSITION);
    assertEquals<int>(3, td.vec[2], ASSERT_POSITION);
}
    
void TestCaseThread::synchronizeMethod()
{
    Synchronize sc(mutex_);
    int step = number_ >= 5000 ? 2 : 1;
    for (int i=0; i<5000; ++i)
    {
        number_ += step;
    }
}

void TestCaseThread::testMutex()
{
    number_ = 0;
    Thread td(UtilBind(&TestCaseThread::synchronizeMethod, this));
    assertTrue(td.start(), ASSERT_POSITION);
    {
        Synchronize sc(mutex_);
        int step = number_ >= 5000 ? 2 : 1;
        for (int i=0; i<5000; ++i)
        {
            number_ += step;
        }
    }
    assertTrue(td.join(), ASSERT_POSITION);
    assertEquals<int>(15000, number_, ASSERT_POSITION);
}

void TestCaseThread::lockThread0()
{
    for (int i=0; i<10; ++i)
    {
        lock0_.wait();
        lockStr_ += "0";
        msleep(10);
        lock1_.notify();
    }
}

void TestCaseThread::lockThread1()
{
    for (int i=0; i<10; ++i)
    {
        lock1_.wait();
        lockStr_ += "1";
        msleep(10);
        lock0_.notify();
    }
}

void TestCaseThread::testLock()
{
    lockStr_.clear();
    Thread td0(UtilBind(&TestCaseThread::lockThread0, this));
    assertTrue(td0.start(), ASSERT_POSITION);
    Thread td1(UtilBind(&TestCaseThread::lockThread1, this));
    assertTrue(td1.start(), ASSERT_POSITION);
    msleep(30);
    lock0_.notify();
    td0.join();
    td1.join();
    for (int i=0; i<20; ++i)
    {
        if (i%2 == 0)
            assertEquals<char>('0', lockStr_[i], ASSERT_POSITION);
        else
            assertEquals<char>('1', lockStr_[i], ASSERT_POSITION);
    }
}

void TestCaseThread::lockTimedWaitThread(bool timeout)
{
    assertNotEquals<bool>(timeout, lock2_.timedWait(500), ASSERT_POSITION);
    number_ = 1;
}

void TestCaseThread::testLockTimedWait()
{
    number_ = 0;
    Thread td(UtilBind(&TestCaseThread::lockTimedWaitThread, this, true));
    assertTrue(td.start(), ASSERT_POSITION);
    msleep(200);
    assertEquals<int>(0, number_, ASSERT_POSITION);
    msleep(400);
    assertEquals<int>(1, number_, ASSERT_POSITION);
    
    number_ = 0;
    Thread td1(UtilBind(&TestCaseThread::lockTimedWaitThread, this, false));
    assertTrue(td1.start(), ASSERT_POSITION);
    msleep(200);
    assertEquals<int>(0, number_, ASSERT_POSITION);
    lock2_.notify();
    msleep(50);
    assertEquals<int>(1, number_, ASSERT_POSITION);
}

void TestCaseThread::multiLockTimedWaitThread(MultiLock* pmlock, bool timeout)
{
    assertNotEquals<bool>(timeout, pmlock->timedWait(500), ASSERT_POSITION);
    number_ = 1;   
}

void TestCaseThread::testMultiLockTimedWait()
{
    //wait one
    MultiLock mlock(2, false);
    
    number_ = 0;
    Thread td(UtilBind(&TestCaseThread::multiLockTimedWaitThread, this, &mlock, true));
    assertTrue(td.start(), ASSERT_POSITION);
    msleep(200);
    assertEquals<int>(0, number_, ASSERT_POSITION);
    msleep(400);
    assertEquals<int>(1, number_, ASSERT_POSITION);
    
    number_ = 0;
    Thread td1(UtilBind(&TestCaseThread::multiLockTimedWaitThread, this, &mlock, false));
    assertTrue(td1.start(), ASSERT_POSITION);
    msleep(200);
    assertEquals<int>(0, number_, ASSERT_POSITION);
    mlock.notify(1);
    msleep(50);
    assertEquals<int>(1, number_, ASSERT_POSITION);
    
    //wait all
    MultiLock mlock1(2, true);
    
    number_ = 0;
    Thread td2(UtilBind(&TestCaseThread::multiLockTimedWaitThread, this, &mlock1, true));
    assertTrue(td2.start(), ASSERT_POSITION);
    msleep(200);
    assertEquals<int>(0, number_, ASSERT_POSITION);
    msleep(400);
    assertEquals<int>(1, number_, ASSERT_POSITION);
    
    number_ = 0;
    Thread td3(UtilBind(&TestCaseThread::multiLockTimedWaitThread, this, &mlock1, false));
    assertTrue(td3.start(), ASSERT_POSITION);
    msleep(200);
    assertEquals<int>(0, number_, ASSERT_POSITION);
    mlock1.notify(1);
    msleep(50);
    assertEquals<int>(0, number_, ASSERT_POSITION);
    mlock1.notify(0);
    msleep(50);
    assertEquals<int>(1, number_, ASSERT_POSITION);
}