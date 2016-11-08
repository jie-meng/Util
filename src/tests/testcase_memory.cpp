#include "testcase_memory.hpp"
#include "util/base.hpp"

using namespace std;
using namespace util;

void TestCaseMemory::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseMemory, testDelete)
    REGISTER_TEST_FUNCTION(TestCaseMemory, testMemory)
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
}