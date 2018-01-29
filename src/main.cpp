#include "util/luaextend.hpp"
#include "tests/unittest_hoster.hpp"

using namespace std;
using namespace util;

const string kVersion = "1.3.0";

int main(int argc, char* argv[])
{
    if (argc > 1 && strAreEqual("$", argv[1]))
    {
        return UnitTestHoster::getInstance().run() ? 0 : -1;
    }
    else
    {
        if (argc == 1)
            printLine("---------------------------------\nluaexec (version: " + kVersion + ")\n---------------------------------");
        
        luaExecutor(argc, argv);
        return 0;
    }
}
