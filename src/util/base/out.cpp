#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "base.hpp"
#include "out.hpp"
#include "util/string.hpp"
#include "util/time.hpp"

namespace util
{

PrintFunc print_func = UtilBind(&stdPrint, _1);

std::string memoryStr(char* buf, size_t bytes)
{
    std::string str("");
    for (size_t i=0; i<bytes; ++i)
    {
        if (0 != i && 0 == i%8)
        {
            if (0 == i%16)
                str += "\n";
            else
                str += " ";
        }
        str += strFormat("%02x ", *((unsigned char*)(buf + i)));
    }
    return str;
}

void print(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buffer[kBufSize] = {0};
    vsnprintf(buffer, kBufSize, fmt, ap);
    va_end(ap);

    print(std::string(buffer));
}

void printLine(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buffer[kBufSize] = {0};
    vsnprintf(buffer, kBufSize, fmt, ap);
    va_end(ap);

    printLine(std::string(buffer));
}

//trace
int TraceDisable[MaxLv] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };

void trace(TraceLv lv, const char* fmt, ...)
{
    if (lv >= MaxLv || lv < 0)
        return;
    if (!TraceDisable[lv])
        return;

    va_list ap;
    va_start(ap, fmt);
    char buffer[kBufSize] = {0};
    vsnprintf(buffer, kBufSize, fmt, ap);
    va_end(ap);

    tm t = localTime();
    std::string lv_str = util::strFormat("[TRACE 0x%04x %02d:%02d:%02d] ",
                                              lv,
                                              t.tm_hour,
                                              t.tm_min,
                                              t.tm_sec);
    printLine(lv_str + std::string(buffer));
}

void setTraceLv(TraceLv lv, bool enable)
{
    TraceDisable[lv] = enable;
}

bool isTraceLvEnable(TraceLv lv)
{
    return TraceDisable[lv];
}

} // namespace util
