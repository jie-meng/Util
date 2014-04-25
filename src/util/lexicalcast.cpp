#include "lexicalcast.hpp"
#include <cmath>

namespace util
{

bool isFloat(const std::string& str)
{
    size_t point_cnt = 0;
    size_t e_cnt = 0;
    size_t plus_cnt = 0;

    size_t point_pos = 0;
    size_t e_pos = 0;
    size_t plus_pos = 0;

    for (size_t i=0; i<str.length(); ++i)
    {
        char ch = str.at(i);
        if('-' == ch && 0 == i)
        {
            ;
        }
        if ('.' == ch)
        {
            point_pos = i;
            if (++point_cnt > 1)
                return false;
        }
        else if ('e' == ch || 'E' == ch)
        {
            e_pos = i;
            if (0 == i || str.length() == i+1)
                return false;
            if (++e_cnt > 1)
                return false;
        }
        else if ('+' == ch)
        {
            plus_pos = i;
            if (i < 2 || str.length() == i+1)
                return false;
            if ('e' != str.at(i-1) && 'E' != str.at(i-1))
                return false;
            if (++plus_cnt > 1)
                return false;
        }
        else if (ch >= '0' && ch <= '9')
        {
            ;
        }
        else
        {
            return false;
        }
    }

    if (e_cnt > 0 || plus_cnt > 0)
    {
        if (point_pos > e_pos || point_pos > plus_pos)
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
    if (!isFloat(str))
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
