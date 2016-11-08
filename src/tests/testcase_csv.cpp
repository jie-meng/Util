#include "testcase_csv.hpp"
#include "util/csv.hpp"
#include "util/file.hpp"

using namespace std;
using namespace util;

void TestCaseCsv::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseCsv, testCsv)
}

void TestCaseCsv::setUp()
{
}

void TestCaseCsv::tearDown()
{    
}

void TestCaseCsv::testCsv()
{
    Csv csv;
    assertTrue(csv.empty(), ASSERT_POSITION);
    assertEquals<size_t>(0, csv.rows(), ASSERT_POSITION);
    assertEquals<size_t>(0, csv.cols(), ASSERT_POSITION);
    
    vector<string> vec;
    vec.push_back("1");
    vec.push_back("2");
    vec.push_back("3");
    vec.push_back("4");
    
    csv.addRow(vec);
    assertFalse(csv.empty(), ASSERT_POSITION);
    assertEquals<size_t>(1, csv.rows(), ASSERT_POSITION);
    assertEquals<size_t>(4, csv.cols(), ASSERT_POSITION);
    assertEquals<string>("1", csv.getCellValue(0, 0), ASSERT_POSITION);
    assertEquals<string>("2", csv.getCellValue(0, 1), ASSERT_POSITION);
    assertEquals<string>("3", csv.getCellValue(0, 2), ASSERT_POSITION);
    assertEquals<string>("4", csv.getCellValue(0, 3), ASSERT_POSITION);
    
    assertTrue(csv.setCellValue(0, 2, "10"), ASSERT_POSITION);
    assertEquals<string>("10", csv.getCellValue(0, 2), ASSERT_POSITION);
    
    assertFalse(csv.setCellValue(1, 2, "10"), ASSERT_POSITION);
    
    assertTrue(csv.setCellValue(0, 3, "12, 10"), ASSERT_POSITION);
    assertEquals<string>("12, 10", csv.getCellValue(0, 3), ASSERT_POSITION);
    
    assertFalse(csv.write(), ASSERT_POSITION);
    assertTrue(csv.write("./test.csv"), ASSERT_POSITION);
    string text = readTextFile("./test.csv");
    assertEquals<string>("1,2,10,\"12, 10\"\n", text, ASSERT_POSITION);
    assertTrue(pathRemove("./test.csv"), ASSERT_POSITION);
    
    vec.pop_back();
    assertTrue(csv.addRow(vec.begin(), vec.end()), ASSERT_POSITION);
    assertTrue(csv.write("./test.csv"), ASSERT_POSITION);
    text = readTextFile("./test.csv");
    assertEquals<string>("1,2,10,\"12, 10\"\n1,2,3,\n", text, ASSERT_POSITION);
    assertTrue(pathRemove("./test.csv"), ASSERT_POSITION);
    
    vec.push_back("13,15,16");
    vec.push_back("27");
    assertTrue(csv.addRow(vec), ASSERT_POSITION);
    assertTrue(csv.write("./test.csv"), ASSERT_POSITION);
    text = readTextFile("./test.csv");
    assertEquals<string>("1,2,10,\"12, 10\"\n1,2,3,\n1,2,3,\"13,15,16\"\n", text, ASSERT_POSITION);
    assertTrue(pathRemove("./test.csv"), ASSERT_POSITION);
    
    csv.clear();
    assertTrue(csv.empty(), ASSERT_POSITION);
    assertEquals<size_t>(0, csv.rows(), ASSERT_POSITION);
    assertEquals<size_t>(0, csv.cols(), ASSERT_POSITION);
}
