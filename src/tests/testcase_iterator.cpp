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
    
    typedef CollectionIterator< int, vector<int> > IntVecIter;
    IntVecIter iv(vec);
    Iterator<int>* piter = &iv;
    
    int i = 0;
    while (piter->hasNext())
        assertEquals<int>(i++, piter->next(), ASSERT_POSITION);
    
    assertEquals<int>(i, vec.size(), ASSERT_POSITION);
}

void TestCaseIterator::testMapIterator()
{
    map<string, int> mp;
    mp["a"] = 0;
    mp["b"] = 1;
    mp["c"] = 2;

    typedef MapCollectionIterator< string, int, map<string, int> > MpIter;
    MpIter mi(mp);
    MapIterator<string, int>* piter = &mi;
    
    int i = 0;
    while (piter->hasNext())
    {
        auto kv = piter->nextKeyValue();
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
