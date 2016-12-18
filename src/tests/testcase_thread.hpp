#ifndef TESTS_TESTCASETHREAD
#define TESTS_TESTCASETHREAD

#include "util/unittest.hpp"
#include "util/thread.hpp"

class TestCaseThread : public util::TestCase
{
public:
    TESTCASE_DECLARE(TestCaseThread)
    virtual void registerTestFunctions();
    
    //Tests
    void testSleep();
    void testGetCurrentThreadId();
    void testThread();
    void testWorkerThread();
    void testMsgThread();
    void testMutex();
protected:
    virtual void setUp();
    virtual void tearDown();
private:
    void synchronizeMethod();
    util::Mutex mutex_;
    int number_;
};

#endif //TESTS_TESTCASETHREAD
