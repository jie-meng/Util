#include "unittest.hpp"
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "net.hpp"

namespace util
{
// TestResult
void TestResult::run(TestCase* ptest)
{
    startTest(ptest);
    runProtected(ptest);
    endTest(ptest);
}

void TestResult::runProtected(TestCase* ptest)
{
    try
    {
        ptest->runBare();
    }
    catch (const AssertFailException& e)
    {
        addFailure(ptest, e);
    }
    catch (const Exception& e)
    {
        addError(ptest, e);
    }
}

void TestResult::startTest(Test* ptest)
{
    size_t count = ptest->countTestCases();
    run_tests_ += count;

    TestListenerVector::iterator it;
    for (it = listeners_.begin(); it != listeners_.end(); ++it)
        (*it)->startTest(ptest);
}

void TestResult::endTest(Test* ptest)
{
    TestListenerVector::iterator it;
    for (it = listeners_.begin(); it != listeners_.end(); ++it)
        (*it)->endTest(ptest);
}

void TestResult::addError(TestCase* ptest, const Exception& e)
{
    errors_.push_back(new TestFailure(ptest, e));

    TestListenerVector::iterator it;
    for (it = listeners_.begin(); it != listeners_.end(); ++it)
        (*it)->addError(ptest, e);
}

void TestResult::addFailure(TestCase* ptest, const AssertFailException& e)
{
    failures_.push_back(new TestFailure(ptest, e));

    TestListenerVector::iterator it;
    for (it = listeners_.begin(); it != listeners_.end(); ++it)
        (*it)->addFailure(ptest, e);
}

void TestResult::cleanup()
{
    TestFailureVector::iterator it;

    for (it = failures_.begin(); it != failures_.end(); ++it)
        safeDelete(*it);
    failures_.clear();

    for (it = errors_.begin(); it != errors_.end(); ++it)
        safeDelete(*it);
    errors_.clear();

    listeners_.clear();
    run_tests_ = 0;
}

// TestCase
void TestCase::runBare()
{
    AutoSetupTearDown astd(this);
    runTest();
}

void TestCase::runTest()
{
    try
    {
        TestFunction test_function = findTestFunction(getName());
        if (!test_function)
        {
            std::string message = std::string("cannot find function \"") + getName() + std::string("\"");
            throw Exception(message);
        }
        test_function();
    }
    catch (const AssertFailException&)
    {
        throw;
    }
    catch (const Exception&)
    {
        throw;
    }
    catch (const std::exception& e)
    {
        throw Exception(e.what());
    }
}

TestFunction TestCase::findTestFunction(const std::string& name)
{
    TestFunctionMap::iterator it = test_function_map_.find(name);
    return (it != test_function_map_.end()) ? it->second : 0;
}

// TestSuite
size_t TestSuite::countTestCases() const
{
    size_t count = 0;
    TestVector::const_iterator it;
    for (it = tests_.begin(); it != tests_.end(); ++it)
        count += (*it)->countTestCases();
    return count;
}

void TestSuite::run(TestResult* ptest_result)
{
    TestVector::iterator it;
    for (it = tests_.begin(); it != tests_.end(); ++it)
        runTest(*it, ptest_result);
}

void TestSuite::cleanup()
{
    TestVector::iterator it;
    for (it = tests_.begin(); it != tests_.end(); ++it)
        safeDelete(*it);
    tests_.clear();
}

void TestSuite::addTestSuite(const TestCase& test_case)
{
    TestCase& tc = const_cast<TestCase&>(test_case);
    TestFunctionMapIterator test_function_map_it = tc.testFunctionMapIterator();
    if (!test_function_map_it.hasNext())
        return;

    TestSuite* p = new TestSuite();
    while (test_function_map_it.hasNext())
        p->addTest(tc.create(test_function_map_it.nextKeyValue().first));

    this->addTest(p);
}

// StdOutPrinter
void StdOutPrinter::print(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buffer[kBufSize] = {0};
    vsnprintf(buffer, kBufSize, fmt, ap);
    va_end(ap);

    util::print(std::string(buffer));
}

void StdOutPrinter::printLine(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buffer[kBufSize] = {0};
    vsnprintf(buffer, kBufSize, fmt, ap);
    va_end(ap);

    util::printLine(std::string(buffer));
}

// UdpPrinter
struct UdpPrinter::UdpPrinterData
{
    UdpPrinterData(unsigned short port, const std::string& ip) :
        ds_(Family_IPv4),
        port_(port),
        ip_(ip)
    {}
    DgramSocket ds_;
    unsigned short port_;
    std::string ip_;
};

UdpPrinter::UdpPrinter(unsigned short port, const std::string& ip) :
    pdata_(new UdpPrinterData(port, ip))
{}

void UdpPrinter::print(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buffer[kBufSize] = {0};
    vsnprintf(buffer, kBufSize, fmt, ap);
    va_end(ap);

    std::string str(buffer);
    pdata_->ds_.sendTo(str.c_str(), str.length(), pdata_->ip_, pdata_->port_);
}

void UdpPrinter::printLine(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buffer[kBufSize] = {0};
    vsnprintf(buffer, kBufSize, fmt, ap);
    va_end(ap);

    std::string str(buffer);
    str += "\n";
    pdata_->ds_.sendTo(str.c_str(), str.length(), pdata_->ip_, pdata_->port_);
}

// ResultPrinter
ResultPrinter::ResultPrinter() :
    printer_(new StdOutPrinter())
{}

ResultPrinter::~ResultPrinter()
{}

void ResultPrinter::addError(Test* ptest, const Exception& e)
{
    printer_->print("E");
}

void ResultPrinter::addFailure(Test* ptest, const AssertFailException& e)
{
    printer_->print("F");
}

void ResultPrinter::startTest(Test* ptest)
{
    printer_->print(".");
}

void ResultPrinter::endTest(Test* ptest)
{}

void ResultPrinter::print(TestResult* presult, unsigned long run_time)
{
    printHeader(run_time);
    printer_->printLine("");
    printErrors(presult);
    printer_->printLine("");
    printFailures(presult);
    printFooter(presult);
}

void ResultPrinter::printWaitPrompt()
{
    printer_->printLine("");
    printer_->printLine("<RETURN> to continue");
}

void ResultPrinter::printHeader(unsigned long run_time)
{
    printer_->printLine("");
    printer_->printLine("Time: %u s", run_time);
}

void ResultPrinter::printErrors(TestResult* presult)
{
    FailuresIterator it = presult->errorsIterator();
    printDefects(it, presult->errorCount(), "error");
}

void ResultPrinter::printFailures(TestResult* presult)
{
    FailuresIterator it = presult->failuresIterator();
    printDefects(it, presult->failureCount(), "failure");
}

void ResultPrinter::printDefects(Iterator<TestFailure*>& it, size_t count, const std::string& type)
{
    if (0 == count)
        return;
    if (1 == count)
        printer_->printLine("There is %u %s", count, type.c_str());
    else
        printer_->printLine("There are %u %ss", count, type.c_str());

    size_t i = 1;
    while(it.hasNext())
        printDefect(it.next(), i++);
}

void ResultPrinter::printDefect(TestFailure* ptest_failure, size_t count)
{
    printDefectHeader(ptest_failure, count);
    //printDefectTrace(ptest_failure);
}

void ResultPrinter::printDefectHeader(TestFailure* ptest_failure, size_t count)
{
    printer_->printLine("%u) %s<%s> %s",
            count,
            ptest_failure->failedTest()->getCaseName().c_str(),
            ptest_failure->failedTest()->getName().c_str(),
            ptest_failure->thrownException()->what());
}

void ResultPrinter::printDefectTrace(TestFailure* ptest_failure)
{
    //getWriter().print(BaseTestRunner.getFilteredTrace(booBoo.Trace()));
}

void ResultPrinter::printFooter(TestResult* presult)
{
    if (presult->isSuccess())
    {
        std::string str_tests;
        if (presult->runCount() <= 1)
            str_tests = "";
        else
            str_tests = "s";

        printer_->printLine("OK (%u test%s)", presult->runCount(), str_tests.c_str());
    }
    else
    {
        printer_->printLine("");
        printer_->printLine("FAILURE !!!");
        printer_->printLine("Test run %u, Failures: %u, Errors: %u",
                presult->runCount(),
                presult->failureCount(),
                presult->errorCount());
    }
    printer_->printLine("");
}

// TestRunner
void TestRunner::doRun(Test& test, bool wait)
{
    DateTime start_time = DateTime::now();
    test.run(&result_);
    DateTime end_time = DateTime::now();
    double dt = end_time.drift(start_time);
    printer_.print(&result_, (unsigned long)dt);

    pause(wait);
}

void TestRunner::pause(bool wait)
{
    if (!wait) return;

    printer_.printWaitPrompt();
    getInput();
}

} // namespace util
