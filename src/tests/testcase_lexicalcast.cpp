#include "testcase_lexicalcast.hpp"
#include "util/lexicalcast.hpp"

using namespace std;
using namespace util;

void TestCaseLexicalCast::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseLexicalCast, testLexicalCast)
    REGISTER_TEST_FUNCTION(TestCaseLexicalCast, testPowUInt)
    REGISTER_TEST_FUNCTION(TestCaseLexicalCast, testHexStrToDec)
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
    
    int ai = 3;
    assertEquals<int>(5, lexicalCastDefault<int>("5", 0), ASSERT_POSITION);
    
    size_t as = 5;
    assertEquals<size_t>(0, lexicalCastDefault<int>("6.0", 0), ASSERT_POSITION);
    
    assertTrue(checkLexical<int>("3"), ASSERT_POSITION);
    assertTrue(checkLexical<size_t>("3"), ASSERT_POSITION);
    assertTrue(checkLexical<float>("3.14"), ASSERT_POSITION);
    assertTrue(checkLexical<float>("3.1415926"), ASSERT_POSITION);
}

void TestCaseLexicalCast::testPowUInt()
{
    assertEquals(1, powUInt(2, 0), ASSERT_POSITION);
    assertEquals(-2, powUInt(-2, 1), ASSERT_POSITION);
    assertEquals(4, powUInt(2, 2), ASSERT_POSITION);
    assertEquals(9, powUInt(-3, 2), ASSERT_POSITION);
    assertEquals(-125, powUInt(-5, 3), ASSERT_POSITION);
}

void TestCaseLexicalCast::testHexStrToDec()
{
    int a = 0;
    assertTrue(hexStrToDec<int>("11", a), ASSERT_POSITION);
    assertEquals<int>(17, a, ASSERT_POSITION);
    
    size_t b = 0;
    assertTrue(hexStrToDec<size_t>("Fa6C", b), ASSERT_POSITION);
    assertEquals<size_t>(64108, b, ASSERT_POSITION);
    
    assertEquals<string>("3078764", hexStrToDecStr<size_t>("2eFa6C"), ASSERT_POSITION);
}