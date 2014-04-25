#ifndef UTIL_LEXICALCAST_HPP
#define UTIL_LEXICALCAST_HPP

#include "base.hpp"

namespace util
{

template <typename T>
bool lexicalCast(const std::string& str, T& out)
{
    std::stringstream ss;
    ss << str;

    T tmp(out);
    ss >> tmp;

    if (toString(tmp) != str)
        return false;

    out = tmp;
    return true;
}

template<>
bool lexicalCast<std::string>(const std::string& str, std::string& out);

template<>
bool lexicalCast<float>(const std::string& str, float& out);
template<>
bool lexicalCast<double>(const std::string& str, double& out);
template<>
bool lexicalCast<long double>(const std::string& str, long double& out);

template <typename T>
T lexicalCastDefault(const std::string& str, T default_value)
{
    T ret(default_value);
    return lexicalCast(str, ret) ? ret : default_value;
}

template <typename T>
bool checkLexical(const std::string& str)
{
    T tmp;
    return lexicalCast(str, tmp) ? true : false;
}

template <typename T>
struct PowUInt
{
    T operator() (T data, size_t n)
    {
        if (0 == n)
            return 1;

        if (1 == n)
            return data;

        T out(data);
        for (size_t i = 1; i<n; ++i)
            out *= data;

        return out;
    }
};

int getDecFromHexCh(char ch);

template<typename T>
bool hexStrToDec(const std::string& hex, T& out_data)
{
    std::string::size_type found = hex.find_first_not_of("0123456789aAbBcCdDeEfF");
    if (found != std::string::npos)
        return false;

    T data(0);
    for (size_t i=0; i<hex.size(); ++i)
    {
        int dec = getDecFromHexCh(hex.at(i));
        if (dec < 0)
            return false;
        data += dec * PowUInt<T>()(16, (size_t)(hex.size() - i - 1));
    }
    out_data = data;
    return true;
}

template<typename T>
inline std::string hexStrToDecStr(const std::string& hex)
{
    T data(0);
    return hexStrToDec(hex, data) ? toString(data) : "";
}

} // namespace util

#endif // UTIL_LEXICALCAST_HPP
