#ifndef UTIL_UNITTEST_HPP
#define UTIL_UNITTEST_HPP

#include <exception>
#include <vector>
#include <map>
#include <typeinfo>
#include "iterator.hpp"

namespace util
{

#define REGISTER_TEST_FUNCTION(className, functionName) \
    registerTestFunction(#functionName, UtilBind(&className::functionName, this));

class Test;
class TestCase;

class Exception : public std::exception
{
public:
    explicit Exception(const std::string& message) throw() : name_("Exception"), message_(message)
    {}
    virtual ~Exception() throw() {}
public:
    inline std::string getName() const { return name_; }
    inline std::string getMessage() const { return message_; }
    virtual const char* what() const throw()
    { name_message_ = name_ + " : " + message_; return name_message_.c_str(); }
    virtual Exception* clone() const { return new Exception(getMessage()); }
protected:
    inline void setName(const std::string& name) { name_ = name; }
private:
    std::string name_;
    std::string message_;
    mutable std::string name_message_;
};

class AssertFailException : public Exception
{
public:
    explicit AssertFailException(const std::string& message) throw() : Exception(message)
    { setName("AssertFailException"); }
    virtual ~AssertFailException() throw() {}
public:
    virtual Exception* clone() const { return new AssertFailException(getMessage()); }
};

class Assert
{
public:
    inline static void assertTrue(bool condition, const std::string& message = "") { if (!condition) fail(message); }
    inline static void assertFalse(bool condition, const std::string& message = "") { assertTrue(!condition, message); }
    template <typename T>
    inline static void assertNull(T* p, const std::string& message = "") { assertTrue(0 == p, message); }
    template <typename T>
    inline static void assertNotNull(T* p, const std::string& message = "") { assertTrue(0 != p, message); }
    template <typename T>
    inline static void assertEquals(T t1, T t2, const std::string& message = "") { assertTrue(t1 == t2, message); }
    template <typename T>
    inline static void assertSame(T t1, T t2, const std::string& message = "") { assertTrue(&t1 == &t2, message); }
    template <typename T>
    inline static void assertNotSame(T t1, T t2, const std::string& message = "") { assertTrue(&t1 != &t2, message); }

    inline static void fail(const std::string& message) { throw AssertFailException(message); }
    inline static void fail() { fail(""); }
protected:
    Assert() {}
    ~Assert() {}
};

class TestFailure
{
public:
    TestFailure(TestCase* pfailed_test_case, const Exception& e) :
        pfailed_test_case_(pfailed_test_case)
        { pexception_ = e.clone(); }
    ~TestFailure() { safeDelete(pexception_); }
public:
    inline TestCase* failedTest() const { return pfailed_test_case_; }
    inline Exception* thrownException() const { return pexception_; }
    inline bool isFailure() const { return typeid(*pexception_) == typeid(AssertFailException); }
protected:
    TestCase* pfailed_test_case_;
    Exception* pexception_;
private:
    DISALLOW_COPY_AND_ASSIGN(TestFailure)
};

class TestListener;

typedef std::vector<TestFailure*> TestFailureVector;
typedef std::vector<TestListener*> TestListenerVector;
typedef CollectionIterator<TestFailure*, TestFailureVector> FailuresIterator;

class TestResult
{
public:
    TestResult() {}
    ~TestResult() { cleanup(); }
public:
    void run(TestCase* ptest);
    void startTest(Test* ptest);
    void endTest(Test* ptest);
    void runProtected(TestCase* ptest);
    void addError(TestCase* ptest, const Exception& e);
    void addFailure(TestCase* ptest, const AssertFailException& e);
    void cleanup();

    inline size_t runCount() const { return run_tests_; }
    inline size_t failureCount() const { return failures_.size(); }
    inline size_t errorCount() const { return errors_.size(); }
    inline TestFailure* getFailure(size_t index) const { return failures_.at(index); }
    inline TestFailure* getError(size_t index) const { return errors_.at(index); }
    inline FailuresIterator failuresIterator() const { return FailuresIterator(failures_); }
    inline FailuresIterator errorsIterator() const { return FailuresIterator(errors_); }
    inline bool isSuccess() const { return failures_.empty() && errors_.empty(); }
    inline void addListener(TestListener* plistener) { listeners_.push_back(plistener); }
private:
    size_t run_tests_;
    mutable TestFailureVector failures_;
    mutable TestFailureVector errors_;
    mutable TestListenerVector listeners_;
private:
    DISALLOW_COPY_AND_ASSIGN(TestResult)
};

class TestListener
{
public:
    virtual void addError(Test* ptest, const Exception& e) = 0;
    virtual void addFailure(Test* ptest, const AssertFailException& e) = 0;
    virtual void startTest(Test* ptest) = 0;
    virtual void endTest(Test* ptest) = 0;
};

class Test : public Interface
{
public:
    virtual size_t countTestCases() const = 0;
    virtual void run(TestResult* ptest_result) = 0;
};

class SetUpAndTearDown
{
public:
    virtual void setUp() throw (std::exception) = 0;
    virtual void tearDown() throw (std::exception) = 0;
};

class AutoSetupTearDown
{
public:
    explicit AutoSetupTearDown(SetUpAndTearDown* psetup_and_teardown) : psetup_and_teardown_(psetup_and_teardown)
    { if (psetup_and_teardown_) psetup_and_teardown_->setUp(); }
    ~AutoSetupTearDown() { psetup_and_teardown_->tearDown(); }
private:
    SetUpAndTearDown* psetup_and_teardown_;
private:
    DISALLOW_COPY_AND_ASSIGN(AutoSetupTearDown)
};

typedef UtilFunction<void ()> TestFunction;
typedef std::map<std::string, TestFunction> TestFunctionMap;
typedef MapCollectionIterator<std::string, TestFunction, TestFunctionMap> TestFunctionMapIterator;

#define TESTCASE_COMMON(className) \
    className(const std::string& case_name) : TestCase(case_name) { registerTestFunctions(); } \
    explicit className(const std::string& case_name, const std::string& name) : TestCase(case_name, name) { registerTestFunctions(); } \
    virtual ~className() {} \
    virtual className* create(const std::string& name) const { return new className(getCaseName(), name); }

class TestCase : public Assert, public Test, public SetUpAndTearDown
{
public:
    explicit TestCase(const std::string& case_name) : case_name_(case_name) {}
    TestCase(const std::string& case_name, const std::string& name) : case_name_(case_name), name_(name) {}
    virtual ~TestCase() {}
public:
    inline std::string getCaseName() const { return case_name_; }
    inline std::string getName() const { return name_; }
    inline void setName(const std::string& name) { name_ = name; }
    inline TestFunctionMapIterator testFunctionMapIterator() { return TestFunctionMapIterator(test_function_map_); }

    void runBare() throw (Exception);
    size_t registeredTestFunctionCount() const { return test_function_map_.size(); }

    virtual TestCase* create(const std::string& name) const = 0;
    virtual size_t countTestCases() const { return 1; }
    virtual void run(TestResult* ptest_result) { ptest_result->run(this); }
protected:
    inline void registerTestFunction(const std::string& name, TestFunction test_function)
    { test_function_map_[name] = test_function; }

    virtual void registerTestFunctions() = 0;
    virtual void setUp() throw (std::exception) {}
    virtual void tearDown() throw (std::exception) {}
    void runTest() throw (Exception);
private:
    TestFunction findTestFunction(const std::string& name);
private:
    std::string case_name_;
    std::string name_;
    TestFunctionMap test_function_map_;
private:
    DISALLOW_COPY_AND_ASSIGN(TestCase)
};

class TestSuite : public Test
{
public:
    typedef std::vector<Test*> TestVector;

    TestSuite() {}
    TestSuite(const std::string& name) : name_(name) {}
    virtual ~TestSuite() { cleanup(); }
public:
    inline std::string name() const { return name_; }
    inline void setName(const std::string& name) { name_ = name; }
    inline void addTest(Test* ptest) { tests_.push_back(ptest); }
    inline void runTest(Test* ptest, TestResult* ptest_result) { ptest->run(ptest_result); }
    inline size_t testCount() const { return tests_.size(); }
    inline Test* getTest(size_t index) const { return tests_.at(index); }

    void addTestSuite(const TestCase& test_case);
    void run(TestResult* ptest_result);

    virtual size_t countTestCases() const;
private:
    void cleanup();
private:
    std::string name_;
    TestVector tests_;
private:
    DISALLOW_COPY_AND_ASSIGN(TestSuite)
};

class Printer : public Interface
{
public:
    virtual void print(const char* fmt, ...) = 0;
    virtual void printLine(const char* fmt, ...) = 0;
};

class StdOutPrinter : public Printer
{
public:
    virtual void print(const char* fmt, ...);
    virtual void printLine(const char* fmt, ...);
};

class UdpPrinter : public Printer
{
public:
    explicit UdpPrinter(unsigned short port, const std::string& ip = "127.0.0.1");

    virtual void print(const char* fmt, ...);
    virtual void printLine(const char* fmt, ...);
private:
    struct UdpPrinterData;
    UtilAutoPtr<UdpPrinterData> pdata_;
};

class ResultPrinter : public TestListener
{
public:
    ResultPrinter();
    ~ResultPrinter();
public:
    inline void setPrinter(UtilAutoPtr<Printer> printer) { printer_ = printer; }

    void print(TestResult* presult, unsigned long run_time);
    void printWaitPrompt();

    virtual void addError(Test* ptest, const Exception& e);
    virtual void addFailure(Test* ptest, const AssertFailException& e);
    virtual void startTest(Test* ptest);
    virtual void endTest(Test* ptest);
protected:
    void printHeader(unsigned long run_time);
    void printErrors(TestResult* presult);
    void printFailures(TestResult* presult);
    void printFooter(TestResult* presult);
    void printDefects(Iterator<TestFailure*>& it, size_t count, const std::string& type);
    void printDefect(TestFailure* ptest_failure, size_t count);
    void printDefectHeader(TestFailure* ptest_failure, size_t count);
    void printDefectTrace(TestFailure* ptest_failure);
private:
    UtilAutoPtr<Printer> printer_;
private:
    DISALLOW_COPY_AND_ASSIGN(ResultPrinter)
};

class TestRunner
{
public:
    TestRunner()
    { result_.cleanup(); result_.addListener(&printer_); }
    ~TestRunner() {};
public:
    inline void setPrinter(UtilAutoPtr<Printer> printer) { printer_.setPrinter(printer); }
    inline void run(Test& test) { doRun(test); }
    inline void doRun(Test& test) { doRun(test, false); }
    void doRun(Test& test, bool wait);
private:
    void pause(bool wait);
private:
    TestResult result_;
    ResultPrinter printer_;
private:
    DISALLOW_COPY_AND_ASSIGN(TestRunner)
};

// demo
//class DemoTestCase : public TestCase
//{
//public:
//    TESTCASE_COMMON(DemoTestCase)
//
//    virtual void registerTestFunctions()
//    {
//        REGISTER_TEST_FUNCTION(DemoTestCase, test1)
//        REGISTER_TEST_FUNCTION(DemoTestCase, test2)
//        REGISTER_TEST_FUNCTION(DemoTestCase, test3)
//        REGISTER_TEST_FUNCTION(DemoTestCase, test4);
//    }
//
//    void test1() throw(std::exception) { int* x = 0; assertNull((int*)0, "test1 exception"); }
//    void test2() throw(std::exception) { int p = 0; assertNotNull(&p, "test2 exception"); }
//    void test3() throw(std::exception) { assertEquals(1, 1, "test3 exception"); }
//    void test4() throw(std::exception) { throw std::exception(); }
//protected:
//    virtual void setUp() throw (std::exception) {}
//    virtual void tearDown() throw (std::exception) {}
//};
//
//void unitTest()
//{
//    TestSuite suit;
//    DemoTestCase demo_case("DemoTestCase");
//    suit.addTestSuite(demo_case);
//    suit.addTest(demo_case.create("test6"));
//
//    TestRunner runner;
//    runner.run(suit);
//}

} // namespace util

#endif // UTIL_UNITTEST_H
