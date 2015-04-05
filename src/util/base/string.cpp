#include "string.hpp"
#include <algorithm>
#include <iterator>
#include <cstdio>
#include <cstdarg>
#include <ctype.h>
#include "constants.hpp"

namespace util
{

bool isSpace(char c)
{
    return std::string::npos != kWhiteSpace.find(c);
}

std::string strFormat(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buffer[util::kBufSize] = {0};
    vsnprintf(buffer, util::kBufSize, fmt, ap);
    va_end(ap);
    return std::string(buffer);
}

std::string strToLower(const std::string& str)
{
    std::string ret;
    transform(str.begin(), str.end(), std::back_inserter(ret), tolower);
    return ret;
}

std::string strToUpper(const std::string& str)
{
    std::string ret;
    transform(str.begin(), str.end(), std::back_inserter(ret), toupper);
    return ret;
}

std::string strTrimLeftEx(const std::string& str, const std::string& chstr)
{
    std::string ret = str;
    std::string::size_type found = ret.find_first_not_of(chstr);
    if (found != std::string::npos)
        ret.erase(0, found);
    else
        ret.clear();

    return ret;
}

std::string strTrimRightEx(const std::string& str, const std::string& chstr)
{
    std::string ret = str;
    std::string::size_type found = ret.find_last_not_of(chstr);
    if (found != std::string::npos)
        ret.erase(found + 1);
    else
        ret.clear();

    return ret;
}

std::string strReplace(const std::string& str_src, const std::string& str_find, const std::string& str_replace)
{
    std::string ret_str = str_src;
    std::string::size_type pos = 0;

    if ((pos = ret_str.find(str_find, pos)) != std::string::npos)
    {
        ret_str.replace(pos, str_find.length(), str_replace);
        return ret_str;
    }
    return ret_str;
}

std::string strReplaceAll(const std::string& str_src, const std::string& str_find, const std::string& str_replace)
{
    std::string ret_str = str_src;
    std::string::size_type pos = 0;

    while ((pos = ret_str.find(str_find, pos)) != std::string::npos)
    {
        ret_str.replace(pos, str_find.length(), str_replace);
        pos += str_replace.length();
    }
    return ret_str;
}

bool strStartWith(const std::string& str, const std::string& start, bool case_sensitive)
{
    if (str.size() >= start.size())
    {
        if (case_sensitive)
        {
            if (str.substr(0, start.size()) == start)
                return true;
        }
        else
        {
            std::string str_lower = strToLower(str);
            std::string start_lower = strToLower(start);
            if (str_lower.substr(0, start_lower.size()) == start_lower)
                return true;
        }
    }
    return false;
}

bool strEndWith(const std::string& str, const std::string& end, bool case_sensitive)
{
    if (str.size() >= end.size())
    {
        if(case_sensitive)
        {
            if (str.substr(str.size() - end.size()) == end)
                return true;
        }
        else
        {
            std::string str_lower = strToLower(str);
            std::string end_lower = strToLower(end);
            if (str_lower.substr(str_lower.size() - end_lower.size()) == end_lower)
                return true;
        }
    }
    return false;
}

std::string strLeft(const std::string& str, size_t n)
{
    return (n >= str.length()) ? str : str.substr(0, n);
}

std::string strRight(const std::string& str, size_t n)
{
    return (n >= str.length()) ? str : str.substr(str.length() - n, n);
}

std::string strMid(const std::string& str, size_t pos, size_t n)
{
    if (pos >= str.length())
        return "";
    if (pos + n >= str.length())
        return str.substr(pos, str.length() - pos);
    return str.substr(pos, n);
}

bool strContains(const std::string& str, const std::string& contain, bool case_sensitive)
{
    if (str.size() >= contain.size())
    {
        if (case_sensitive)
        {
            if (str.find(contain) != std::string::npos)
                return true;
        }
        else
        {
            std::string str_lower = strToLower(str);
            std::string contain_lower = strToLower(contain);
            if (str_lower.find(contain_lower) != std::string::npos)
                return true;
        }
    }
    return false;
}

int strCompare(const std::string& str_first, const std::string& str_second, bool case_sensitive)
{
    std::string str_first_tmp(str_first);
    std::string str_second_tmp(str_second);
    if (!case_sensitive)
    {
        str_first_tmp = strToLower(str_first);
        str_second_tmp = strToLower(str_second);
    }
    return str_first_tmp.compare(str_second_tmp);
}

bool strAreEqual(const std::string& str_first, const std::string& str_second, bool case_sensitive)
{
    return (0 == strCompare(str_first, str_second, case_sensitive)) ? true : false;
}

char* strToCharArray(const std::string& str, char* pout_buf, size_t buf_len)
{
    memset(pout_buf, 0, buf_len);
    if (str.length() > buf_len -1)
    {
        memcpy(pout_buf, str.c_str(), buf_len - 1);
        *(pout_buf + buf_len - 1) = '\0';
        return 0;
    }
    else
    {
        memcpy(pout_buf, str.c_str(), str.length());
        *(pout_buf + str.length()) = '\0';
        return pout_buf;
    }
}

} // namespace util

