#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "base.hpp"
#include "out.hpp"
#include "string.hpp"
#include "time.hpp"

namespace util
{

PrintFunc print_func = UtilBind(&stdPrint, _1);

std::string memContent(char* buf, size_t bytes)
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
Trace::Level getTraceLv()
{
    return Trace::getInstance().getTraceLevel();
}

void setTraceLevel(Trace::Level lv)
{
    Trace::getInstance().setTraceLevel(lv);
}

std::string traceLvStr(Trace::Level lv)
{
    switch (lv)
    {
    case Trace::Verbose:
        return kTraceVerbose;
    case Trace::Debug:
        return kTraceDebug;
    case Trace::Info:
        return kTraceInfo;
    case Trace::Warn:
        return kTraceWarn;
    case Trace::Error:
        return kTraceError;
    case Trace::Fatal:
        return kTraceFatal;
    case Trace::Assert:
        return kTraceAssert;
    case Trace::Silent:
        return kTraceSilent;
    default:
        return "";
    }
}

std::string getTraceLevelStr()
{
    return traceLvStr(getTraceLv());
}

void setTraceLevelStr(const std::string& str)
{
    std::string lower_lstr = strTrim(strToLower(str));
    if (kTraceVerbose == lower_lstr)
        setTraceLevel(Trace::Verbose);
    else if (kTraceDebug == lower_lstr)
        setTraceLevel(Trace::Debug);
    else if (kTraceInfo == lower_lstr)
        setTraceLevel(Trace::Info);
    else if (kTraceWarn == lower_lstr)
        setTraceLevel(Trace::Warn);
    else if (kTraceError == lower_lstr)
        setTraceLevel(Trace::Error);
    else if (kTraceFatal == lower_lstr)
        setTraceLevel(Trace::Fatal);
    else if (kTraceAssert == lower_lstr)
        setTraceLevel(Trace::Assert);
    else if (kTraceSilent == lower_lstr)
        setTraceLevel(Trace::Silent);
    else
        setTraceLevel(Trace::Info);
}

void trace(Trace::Level lv, const char* fmt, ...)
{
    if (lv > Trace::Silent || lv < 0)
        return;

    if (getTraceLv() > lv)
        return;

    va_list ap;
    va_start(ap, fmt);
    char buffer[kBufSize] = {0};
    vsnprintf(buffer, kBufSize, fmt, ap);
    va_end(ap);

    DateTime dt = DateTime::now();
    std::string lv_str = util::strFormat("[TRACE %s %02d:%02d:%02d] ",
                                              traceLvStr(lv).c_str(),
                                              dt.getHour(),
                                              dt.getMinute(),
                                              dt.getSecond());
    printLine(lv_str + std::string(buffer));
}

} // namespace util
