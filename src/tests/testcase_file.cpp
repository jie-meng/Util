#include "testcase_file.hpp"
#include "util/file.hpp"

using namespace std;
using namespace util;

const string kTempFileName = "temp_file";
const string kTempDirName = "temp_dir";

void TestCaseFile::registerTestFunctions()
{
    REGISTER_TEST_FUNCTION(TestCaseFile, testReadWrite)
    REGISTER_TEST_FUNCTION(TestCaseFile, testCreateBinaryFile)
    REGISTER_TEST_FUNCTION(TestCaseFile, testWriteBinaryFile)
    REGISTER_TEST_FUNCTION(TestCaseFile, testFileInfo)
    REGISTER_TEST_FUNCTION(TestCaseFile, testPath)
    REGISTER_TEST_FUNCTION(TestCaseFile, testListFile)
    REGISTER_TEST_FUNCTION(TestCaseFile, testIsTextFile)
    REGISTER_TEST_FUNCTION(TestCaseFile, testMkDir)
}

void TestCaseFile::setUp()
{
}

void TestCaseFile::tearDown()
{
    if (isPathFile(kTempFileName))
        pathRemove(kTempFileName);

    if (isPathDir(kTempDirName))
        pathRemoveAll(kTempDirName);
}

void TestCaseFile::testReadWrite()
{
    assertStringNotWhiteSpace(readTextFile("LICENSE"), ASSERT_POSITION);
    string content = "This is a test string.";
    assertTrue(writeTextFile(kTempFileName, content), ASSERT_POSITION);
    assertEquals(content, readTextFile(kTempFileName), ASSERT_POSITION);
    assertTrue(writeTextFile(kTempFileName, content), ASSERT_POSITION);
    assertEquals(content, readTextFile(kTempFileName), ASSERT_POSITION);
    assertTrue(writeTextFile(kTempFileName, content, true), ASSERT_POSITION);
    assertEquals(content + content, readTextFile(kTempFileName), ASSERT_POSITION);
}

void TestCaseFile::testCreateBinaryFile()
{
    assertTrue(createBinaryFile(kTempFileName, 64, 'A'), ASSERT_POSITION);
    assertEquals<uint64_t>(64, fileSize(kTempFileName), ASSERT_POSITION);
    char buf[64];
    memset(buf, 0, sizeof(buf));
    assertTrue(readBinaryFile(kTempFileName, buf, 64), ASSERT_POSITION);
    assertEquals<char>('A', buf[45], ASSERT_POSITION);
    
    memset(buf, 0, sizeof(buf));
    assertTrue(readBinaryFile(kTempFileName, buf, 32, 32), ASSERT_POSITION);
}

void TestCaseFile::testWriteBinaryFile()
{
    char buf[64];
    char read[128];
    
    //?
    memset(buf, '?', sizeof(buf));
    assertTrue(writeBinaryFile(kTempFileName, buf, sizeof(buf)), ASSERT_POSITION);
    assertEquals<uint64_t>(sizeof(buf), fileSize(kTempFileName), ASSERT_POSITION);
    
    memset(read, 0, sizeof(read));
    assertTrue(readBinaryFile(kTempFileName, read, sizeof(read)), ASSERT_POSITION);
    assertEquals<char>('?', read[22], ASSERT_POSITION);
    
    //#
    memset(buf, '#', sizeof(buf));
    assertTrue(writeBinaryFile(kTempFileName, buf, sizeof(buf)), ASSERT_POSITION);
    assertEquals<uint64_t>(sizeof(buf), fileSize(kTempFileName), ASSERT_POSITION);
    
    memset(read, 0, sizeof(read));
    assertTrue(readBinaryFile(kTempFileName, read, sizeof(read)), ASSERT_POSITION);
    assertEquals<char>('#', read[22], ASSERT_POSITION);
    
    //$
    memset(buf, '$', sizeof(buf));
    assertTrue(writeBinaryFile(kTempFileName, buf, sizeof(buf), true), ASSERT_POSITION);
    assertEquals<uint64_t>(sizeof(buf) * 2, fileSize(kTempFileName), ASSERT_POSITION);
    
    memset(read, 0, sizeof(read));
    assertTrue(readBinaryFile(kTempFileName, read, sizeof(read)), ASSERT_POSITION);
    assertEquals<char>('#', read[22], ASSERT_POSITION);
    assertEquals<char>('$', read[65], ASSERT_POSITION);
    
    //^
    memset(buf, '^', sizeof(buf));
    assertTrue(overwriteBinaryFile(kTempFileName, buf, sizeof(buf), 32), ASSERT_POSITION);
    
    memset(read, 0, sizeof(read));
    assertTrue(readBinaryFile(kTempFileName, read, sizeof(read)), ASSERT_POSITION);
    assertEquals<char>('#', read[22], ASSERT_POSITION);
    assertEquals<char>('^', read[55], ASSERT_POSITION);
    assertEquals<char>('^', read[73], ASSERT_POSITION);
    assertEquals<char>('$', read[110], ASSERT_POSITION);
}

void TestCaseFile::testFileInfo()
{
    assertTrue(fileSize("unittest.lua") > 0, ASSERT_POSITION);
    string pathname = "/home/joshua/Documents/readme.md";
    assertEquals<string>("md", fileExtension(pathname), ASSERT_POSITION);
    assertEquals<string>("readme", fileBaseName(pathname), ASSERT_POSITION);
    pathname = "C:\\Program Files\\Microsoft\\Temp\\Test.exe";
    assertEquals<string>("exe", fileExtension(pathname), ASSERT_POSITION);
    assertEquals<string>("Test", fileBaseName(pathname), ASSERT_POSITION);

    DateTime createTime = fileTime("make_and_test.py", FtCreationTime);
    DateTime writeTime = fileTime("make_and_test.py", FtLastWriteTime);
    DateTime accessTime = fileTime("make_and_test.py", FtLastAccessTime);

    assertTrue(isPathFile("make_and_test.py"), ASSERT_POSITION);
    assertTrue(createTime.isValid(), ASSERT_POSITION);
    assertTrue(writeTime.isValid(), ASSERT_POSITION);
    assertTrue(accessTime.isValid(), ASSERT_POSITION);
}

void TestCaseFile::testPath()
{
    std::pair<std::string, std::string> pair = splitPathname(appPath());
    string path = pair.first;
    if (strEndWith(path, "/."))
        path = strLeft(path, path.size() - 2);
    assertEquals<string>(currentPath(), path, ASSERT_POSITION);
    
    string pathname = "/home/joshua/Documents/readme.md";
    pair = splitPathname(pathname);
    assertEquals<string>("/home/joshua/Documents", pair.first, ASSERT_POSITION);
    assertEquals<string>("readme.md", pair.second, ASSERT_POSITION);
    
    pathname = "C:\\Program Files\\Microsoft\\Temp\\Test.exe";
    pair = splitPathname(pathname);
    assertEquals<string>("C:\\Program Files\\Microsoft\\Temp", pair.first, ASSERT_POSITION);
    assertEquals<string>("Test.exe", pair.second, ASSERT_POSITION);
    
    string lastPath = currentPath();
    setCurrentPath(currentPath() + "/src");
    assertEquals<string>(lastPath + "/src", currentPath(), ASSERT_POSITION);
    setCurrentPath(lastPath);
    assertEquals<string>(lastPath, currentPath(), ASSERT_POSITION);
    
    string ab = relativePathToAbsolutePath("src");
    assertEquals<string>(currentPath() + "/src", ab, ASSERT_POSITION);
    
    assertTrue(isPathExists(currentPath() + "/src"), ASSERT_POSITION);
    assertTrue(isPathExists("src"), ASSERT_POSITION);
    assertTrue(isPathDir(currentPath() + "/src"), ASSERT_POSITION);
    assertTrue(isPathDir("src"), ASSERT_POSITION);
    assertFalse(isPathFile(currentPath() + "/src"), ASSERT_POSITION);
    assertFalse(isPathFile("src"), ASSERT_POSITION);
    
    assertTrue(isPathExists(currentPath() + "/src/main.cpp"), ASSERT_POSITION);
    assertTrue(isPathExists("src"), ASSERT_POSITION);
    assertFalse(isPathDir(currentPath() + "/src/main.cpp"), ASSERT_POSITION);
    assertFalse(isPathDir("src/main.cpp"), ASSERT_POSITION);
    assertTrue(isPathFile(currentPath() + "/src/main.cpp"), ASSERT_POSITION);
    assertTrue(isPathFile("src/main.cpp"), ASSERT_POSITION);
    
    assertFalse(isPathEmpty("src"), ASSERT_POSITION);
    assertFalse(isPathEmpty(currentPath() + "/src"), ASSERT_POSITION);
    assertFalse(isPathEmpty("src/main.cpp"), ASSERT_POSITION);
    assertFalse(isPathEmpty(currentPath() + "/src/main.cpp"), ASSERT_POSITION);
    assertTrue(mkDir(kTempDirName), ASSERT_POSITION);
    assertTrue(isPathEmpty(kTempDirName), ASSERT_POSITION);
    assertTrue(writeTextFile(kTempDirName + "/testfile", "content"), ASSERT_POSITION);
    assertFalse(isPathEmpty(kTempDirName), ASSERT_POSITION);
    assertFalse(pathRemove(kTempDirName), ASSERT_POSITION);
    assertTrue(pathRemove(kTempDirName + "/testfile"), ASSERT_POSITION);
    assertTrue(pathRemove(kTempDirName), ASSERT_POSITION);
    assertTrue(mkDir(kTempDirName), ASSERT_POSITION);
    assertTrue(isPathEmpty(kTempDirName), ASSERT_POSITION);
    assertTrue(writeTextFile(kTempDirName + "/testfile0", "content"), ASSERT_POSITION);
    assertTrue(writeTextFile(kTempDirName + "/testfile1", "content"), ASSERT_POSITION);
    assertTrue(writeTextFile(kTempDirName + "/testfile2", "content"), ASSERT_POSITION);
    pathRemoveAll(kTempDirName);
    assertFalse(isPathExists(kTempDirName), ASSERT_POSITION);

    assertTrue(mkDir(kTempDirName), ASSERT_POSITION);
    assertTrue(writeTextFile(kTempDirName + "/testfile", "content"), ASSERT_POSITION);
    assertFalse(isPathDir("test2_mkdir"), ASSERT_POSITION);
    assertTrue(pathRename(kTempDirName, "test2_mkdir"), ASSERT_POSITION);
    assertTrue(isPathDir("test2_mkdir"), ASSERT_POSITION);
    assertFalse(isPathFile("test2_mkdir/testfile2"), ASSERT_POSITION);
    assertTrue(pathRename(currentPath() + "/test2_mkdir/testfile", "test2_mkdir/testfile2"), ASSERT_POSITION);
    assertTrue(isPathFile("test2_mkdir/testfile2"), ASSERT_POSITION);
    pathRemoveAll("test2_mkdir");
}


void TestCaseFile::testListFile()
{
    vector<string> vec;
    FileFilter ff("cpp");
    assertTrue(listFiles(currentPath() + "/src", vec, &ff) > 0, ASSERT_POSITION);
    for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
        assertTrue(strEndWith(*it, "cpp"), ASSERT_POSITION);
    
    vec.clear();
    DirFilter df;
    size_t cnt = listFiles("src", vec, &df);
    assertTrue(cnt > 0, ASSERT_POSITION);
    assertEquals<size_t>(cnt, vec.size(), ASSERT_POSITION);
    
    vec.clear();
    FileFilterRecursive< vector<string> > ffr(vec, "hpp");
    assertTrue(listFiles("src", vec, &ffr) > 20, ASSERT_POSITION);
    for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
        assertTrue(strEndWith(*it, ".hpp"), ASSERT_POSITION);
    
    vec.clear();
    findFilesInDir(currentPath() + "/src", vec, "cpp");
    assertTrue(vec.size() > 0, ASSERT_POSITION);
    for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
        assertTrue(strEndWith(*it, "cpp"), ASSERT_POSITION);
    
    vec.clear();
    findPathInDir("src", vec);
    assertTrue(cnt > 0, ASSERT_POSITION);
    assertEquals<size_t>(cnt, vec.size(), ASSERT_POSITION);
    
    vec.clear();
    findFilesInDirRecursively("src", vec, "hpp");
    assertTrue(vec.size() > 20, ASSERT_POSITION);
    for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
        assertTrue(strEndWith(*it, ".hpp"), ASSERT_POSITION);
}

void TestCaseFile::testIsTextFile()
{
    assertFalse(isTextFile(""), ASSERT_POSITION);
    assertTrue(isTextFile("Makefile"), ASSERT_POSITION);
    assertTrue(isTextFile("CMakeLists.txt"), ASSERT_POSITION);
    assertTrue(isTextFile("make.py"), ASSERT_POSITION);
}

void TestCaseFile::testMkDir()
{
    assertTrue(mkDir(kTempDirName), ASSERT_POSITION);
    assertFalse(mkDir(kTempDirName), ASSERT_POSITION);
    assertTrue(mkDir(kTempDirName, false), ASSERT_POSITION);
    
    pathRemoveAll(kTempDirName);
    assertTrue(mkFullDir(kTempDirName + "/test/a/b/c"), ASSERT_POSITION);
    printLine("Check: " + kTempDirName + "/test");
    assertTrue(isPathDir(kTempDirName + "/test"), ASSERT_POSITION);
    assertTrue(isPathDir(kTempDirName + "/test/a"), ASSERT_POSITION);
    assertTrue(isPathDir(kTempDirName + "/test/a/b"), ASSERT_POSITION);
    assertTrue(isPathDir(kTempDirName + "/test/a/b/c"), ASSERT_POSITION);
}
