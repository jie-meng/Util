#ifndef TESTS_UNITTESTHOSTER
#define TESTS_UNITTESTHOSTER

#include "util/base.hpp"

class UnitTestHoster
{
public:
    SINGLETON(UnitTestHoster)
    UnitTestHoster();
    void run();
private:
    DISALLOW_COPY_AND_ASSIGN(UnitTestHoster)
};

#endif //TESTS_UNITTESTHOSTER
