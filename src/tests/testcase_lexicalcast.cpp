#include "testcase_lexicalcast.hpp"
#include "util/lexicalcast.hpp"

using namespace std;
using namespace util;

void TestCaseLexicalCast::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseLexicalCast, testLexicalCast)
}

void TestCaseLexicalCast::setUp()
{
}

void TestCaseLexicalCast::tearDown()
{
    
}

void TestCaseLexicalCast::testLexicalCast()
{  
    int a = 0; 
    assertTrue(lexicalCast<int>("0", a), ASSERT_POSITION);
    assertEquals<int>(0, a, ASSERT_POSITION);
    
    assertTrue(lexicalCast<int>("10", a), ASSERT_POSITION);
    assertEquals<int>(10, a, ASSERT_POSITION);
    
    assertTrue(lexicalCast<int>("-10", a), ASSERT_POSITION);
    assertEquals<int>(-10, a, ASSERT_POSITION);
    
    uint64_t b = 0;
    assertTrue(lexicalCast<uint64_t>("0", b), ASSERT_POSITION);
    assertEquals<uint64_t>(0, b, ASSERT_POSITION);
    
    assertTrue(lexicalCast<uint64_t>("10", b), ASSERT_POSITION);
    assertEquals<uint64_t>(10, b, ASSERT_POSITION);
    
    assertFalse(lexicalCast<uint64_t>("-10", b), ASSERT_POSITION);
}
