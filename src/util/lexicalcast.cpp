#include "lexicalcast.hpp"
#include <cmath>

namespace util
{

const std::string kIntChars = "0123456789";
const std::string kFloatChars = "0123456789.";

size_t getCharCnt(const std::string& str, char c)
{
    size_t cnt = 0;
    for (size_t i=0; i<str.length(); ++i)
    {
        if (c == str[i])
            ++cnt;
    }
    return cnt;
}

bool isInt(const std::string& str)
{
    if (str.empty())
        return false;

    std::string format_str = str;
    if ('-' == str[0] || '+' == str[0])
        format_str = strRight(str, str.length()-1);

    if (format_str.empty())
        return false;

    std::string::size_type found = format_str.find_first_not_of(kIntChars);
    if (found != std::string::npos)
        return false;

    return true;
}

bool isFloat(const std::string& str)
{
    if (str.empty())
        return false;

    std::string format_str = str;
    if ('-' == str[0] || '+' == str[0])
        format_str = strRight(str, str.length()-1);

    if (format_str.empty())
        return false;

    if (getCharCnt(format_str, '.') > 1)
        return false;

    std::string::size_type found = format_str.find_first_not_of(kFloatChars);
    if (found != std::string::npos)
        return false;

    return true;
}

bool isFloatEx(const std::string& str)
{
    std::string str_format = strToLower(str);
    if (!strContains(str_format, "e"))
    {
        return isFloat(str_format);
    }
    else
    {
        if (getCharCnt(str_format, 'e')> 1)
            return false;

        std::string::size_type pos = 0;
        pos = str_format.find("e", pos);
        if (!isFloat(strLeft(str_format, pos)))
            return false;
        if (!isInt(strRight(str_format, str_format.length() - (pos+1))))
            return false;
    }

   return true;
}

template<>
bool lexicalCast<std::string>(const std::string& str, std::string& out)
{
    out = str;
    return true;
}

template<typename Float>
bool lexicalCastFloat(const std::string& str, Float& out)
{
    if (!isFloatEx(str))
        return false;

//    std::stringstream ss;
//    ss << str;
//    ss >> out;

    //mingw will crash when get float/double/long double from std::stringstream in multi-thread. (msvc is ok)
    //so use atof instead
    out = atof(str.c_str());

    return true;
}

template<>
bool lexicalCast<float>(const std::string& str, float& out)
{
    return lexicalCastFloat(str, out);
}

template<>
bool lexicalCast<double>(const std::string& str, double& out)
{
    return lexicalCastFloat(str, out);
}

template<>
bool lexicalCast<long double>(const std::string& str, long double& out)
{
    return lexicalCastFloat(str, out);
}

int getDecFromHexCh(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';

    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;

    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;

    return -1;
}


} // namespace util
