#include "testcase_memory.hpp"
#include "util/base.hpp"

using namespace std;
using namespace util;

void TestCaseMemory::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseMemory, testDelete)
    REGISTER_TEST_FUNCTION(TestCaseMemory, testMemory)
    REGISTER_TEST_FUNCTION(TestCaseMemory, testMemoryBuf)
}

void TestCaseMemory::setUp()
{
}

void TestCaseMemory::tearDown()
{    
}

void TestCaseMemory::testDelete()
{
    int* pa = new int(3);
    safeDelete(pa);
    assertNull(pa, ASSERT_POSITION);
    
    float* pfa = new float[10];
    safeDeleteArray(pfa);
    assertNull(pfa, ASSERT_POSITION);
    
    void* pv = new double(8.0);
    deleteVoid<double>(pv);
    assertNotNull(pv, ASSERT_POSITION);
}

void TestCaseMemory::testMemory()
{
    Memory<char> mem0;
    assertNull(mem0.buf(), ASSERT_POSITION);
    assertEquals<size_t>(0, mem0.count(), ASSERT_POSITION);
    assertEquals<size_t>(0, mem0.size(), ASSERT_POSITION);
    assertNotNull(mem0.create(20), ASSERT_POSITION);
    assertNotNull(mem0.buf(), ASSERT_POSITION);
    assertEquals<size_t>(20, mem0.count(), ASSERT_POSITION);
    assertEquals<size_t>(20, mem0.size(), ASSERT_POSITION);
    mem0[5] = 'A';
    assertEquals<char>('A', mem0[5], ASSERT_POSITION);
    mem0.clear();
    assertNotEquals<char>('A', mem0[5], ASSERT_POSITION);
    
    Memory<int> mem1;
    assertNull(mem1.buf(), ASSERT_POSITION);
    assertEquals<size_t>(0, mem1.count(), ASSERT_POSITION);
    assertEquals<size_t>(0, mem1.size(), ASSERT_POSITION);
    assertNotNull(mem1.create(20), ASSERT_POSITION);
    assertNotNull(mem1.buf(), ASSERT_POSITION);
    assertEquals<size_t>(20, mem1.count(), ASSERT_POSITION);
    assertEquals<size_t>(20 * sizeof(int), mem1.size(), ASSERT_POSITION);
    mem1[15] = 26;
    assertEquals<int>(26, mem1[15], ASSERT_POSITION);
    mem1.clear();
    assertEquals<int>(0, mem1[15], ASSERT_POSITION);
    
    Memory<double> mem2(10);
    assertNotNull(mem2.buf(), ASSERT_POSITION);
    assertEquals<size_t>(10, mem2.count(), ASSERT_POSITION);
    assertEquals<size_t>(10 * sizeof(double), mem2.size(), ASSERT_POSITION);
    mem2[2] = 3.1415926;
    assertEquals<double>(3.1415926, mem2[2], ASSERT_POSITION);
    mem2.clear();
    assertEquals<double>(0, mem2[2], ASSERT_POSITION);
        
    Memory<char> mem3(128);
    strcpy(mem3.buf(), "This is a test string.");
    assertEquals<string>("This is a test string.", string(mem3.buf()), ASSERT_POSITION);
}

void TestCaseMemory::testMemoryBuf()
{
    Memory<char> mem(5);
    strcpy(mem.buf(), "test");
    assertEquals<size_t>(5, mem.count(), ASSERT_POSITION);
    assertEquals<char>('t', *mem.buf(), ASSERT_POSITION);
    assertEquals<char>('t', *mem.buf(0), ASSERT_POSITION);
    assertEquals<char>('e', *mem.buf(1), ASSERT_POSITION);
    assertEquals<char>('s', *mem.buf(2), ASSERT_POSITION);
    assertEquals<char>('t', *mem.buf(3), ASSERT_POSITION);
    assertEquals<char>('\0', *mem.buf(4), ASSERT_POSITION);
    assertEquals<char>('t', *mem.buf(-5), ASSERT_POSITION);
    assertEquals<char>('e', *mem.buf(-4), ASSERT_POSITION);
    assertEquals<char>('s', *mem.buf(-3), ASSERT_POSITION);
    assertEquals<char>('t', *mem.buf(-2), ASSERT_POSITION);
    assertEquals<char>('\0', *mem.buf(-1), ASSERT_POSITION);
    bool exception = false;
    try
    {
        mem.buf(-6);
    }
    catch (Exception& e)
    {
        exception = true;
    }
    assertTrue(exception, ASSERT_POSITION);
    
    Memory<int> mem2(5);
    int* p = mem2.buf();
    for (int i=0; i<5; ++i)
    {
        *p = i;
        ++p;
    }
    assertEquals<size_t>(5, mem2.count(), ASSERT_POSITION);
    assertEquals<int>(0, *mem2.buf(), ASSERT_POSITION);
    assertEquals<int>(0, *mem2.buf(0), ASSERT_POSITION);
    assertEquals<int>(1, *mem2.buf(1), ASSERT_POSITION);
    assertEquals<int>(2, *mem2.buf(2), ASSERT_POSITION);
    assertEquals<int>(3, *mem2.buf(3), ASSERT_POSITION);
    assertEquals<int>(4, *mem2.buf(4), ASSERT_POSITION);
    assertEquals<int>(0, *mem2.buf(-5), ASSERT_POSITION);
    assertEquals<int>(1, *mem2.buf(-4), ASSERT_POSITION);
    assertEquals<int>(2, *mem2.buf(-3), ASSERT_POSITION);
    assertEquals<int>(3, *mem2.buf(-2), ASSERT_POSITION);
    assertEquals<int>(4, *mem2.buf(-1), ASSERT_POSITION);
    exception = false;
    try
    {
        mem2.buf(-6);
    }
    catch (Exception& e)
    {
        exception = true;
    }
    assertTrue(exception, ASSERT_POSITION);
}