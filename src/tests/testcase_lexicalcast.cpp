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
    
    float f = 0.0;
    assertTrue(lexicalCast<float>("-0.2", f), ASSERT_POSITION);
    assertEquals<float>(-0.2, f, ASSERT_POSITION);
    
    double d = 0.0;
    assertTrue(lexicalCast<double>("2.4E-25", d), ASSERT_POSITION);
    assertEquals(2.4E-25, d, ASSERT_POSITION);
    
    string s;
    assertTrue(lexicalCast<string>("test a  b\nc", s), ASSERT_POSITION);
    assertEquals<string>("test a  b\nc", s, ASSERT_POSITION);
}

