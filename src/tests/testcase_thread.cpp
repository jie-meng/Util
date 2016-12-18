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
