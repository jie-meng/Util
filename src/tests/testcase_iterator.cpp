#include "testcase_iterator.hpp"
#include "util/iterator.hpp"

using namespace std;
using namespace util;

void TestCaseIterator::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseIterator, testIterator)
    REGISTER_TEST_FUNCTION(TestCaseIterator, testMapIterator)
}

void TestCaseIterator::setUp()
{
}

void TestCaseIterator::tearDown()
{    
}

void TestCaseIterator::testIterator()
{
    vector<int> vec;
    vec.push_back(0);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    
    Iterator<int> it = createIterator<int>(vec);
    Iterator<int> it2(it);
    auto it3 = it;
    
    int i = 0;
    while (it.hasNext())
        assertEquals<int>(i++, it.next(), ASSERT_POSITION);
    assertEquals<int>(i, vec.size(), ASSERT_POSITION);
    
    i = 0;
    while (it2.hasNext())
        assertEquals<int>(i++, it2.next(), ASSERT_POSITION);
    assertEquals<int>(i, vec.size(), ASSERT_POSITION);
    
    i = 0;
    while (it3.hasNext())
        assertEquals<int>(i++, it3.next(), ASSERT_POSITION);
    assertEquals<int>(i, vec.size(), ASSERT_POSITION);
}

void TestCaseIterator::testMapIterator()
{
    map<string, int> mp;
    mp["a"] = 0;
    mp["b"] = 1;
    mp["c"] = 2;

    Iterator< pair<string, int> > it = createIterator< pair<string, int> >(mp);
    auto it2(it);
    auto it3 = it;
    
    int i = 0;
    while (it.hasNext())
    {
        auto kv = it.next();
        if (kv.first == "a")
            assertEquals<int>(0, kv.second, ASSERT_POSITION);
        else if (kv.first == "b")
            assertEquals<int>(1, kv.second, ASSERT_POSITION);
        else if (kv.first == "c")
            assertEquals<int>(2, kv.second, ASSERT_POSITION);
        else
            break;
        
        ++i;
    }
    
    assertEquals<int>(i, mp.size(), ASSERT_POSITION);
    
    i = 0;
    while (it2.hasNext())
    {
        auto kv = it2.next();
        if (kv.first == "a")
            assertEquals<int>(0, kv.second, ASSERT_POSITION);
        else if (kv.first == "b")
            assertEquals<int>(1, kv.second, ASSERT_POSITION);
        else if (kv.first == "c")
            assertEquals<int>(2, kv.second, ASSERT_POSITION);
        else
            break;
        
        ++i;
    }
    
    assertEquals<int>(i, mp.size(), ASSERT_POSITION);
    
    i = 0;
    while (it3.hasNext())
    {
        auto kv = it3.next();
        if (kv.first == "a")
            assertEquals<int>(0, kv.second, ASSERT_POSITION);
        else if (kv.first == "b")
            assertEquals<int>(1, kv.second, ASSERT_POSITION);
        else if (kv.first == "c")
            assertEquals<int>(2, kv.second, ASSERT_POSITION);
        else
            break;
        
        ++i;
    }
    
    assertEquals<int>(i, mp.size(), ASSERT_POSITION);
}
