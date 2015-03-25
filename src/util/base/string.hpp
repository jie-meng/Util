#ifndef UTIL_STRING_HPP
#define UTIL_STRING_HPP

#include <cstring>
#include <string>

namespace util
{

const std::string kWhiteSpace = " \t\f\v\n\r";

bool isSpace(char c);

std::string strFormat(const char* fmt, ...);

std::string strToLower(const std::string& str);
std::string strToUpper(const std::string& str);

std::string strTrimLeftEx(const std::string& str, const std::string& chstr);
std::string strTrimRightEx(const std::string& str, const std::string& chstr);
inline std::string strTrimEx(const std::string& str, const std::string& chstr) { return strTrimLeftEx(strTrimRightEx(str, chstr), chstr); }
inline std::string strTrimLeft(const std::string& str) { return strTrimLeftEx(str, kWhiteSpace); }
inline std::string strTrimRight(const std::string& str) { return strTrimRightEx(str, kWhiteSpace); }
inline std::string strTrim(const std::string str) { return strTrimEx(str, kWhiteSpace); }

std::string strReplace(const std::string& str_src, const std::string& str_find, const std::string& str_replace);
std::string strReplaceAll(const std::string& str_src, const std::string& str_find, const std::string& str_replace);

bool strStartWith(const std::string& str, const std::string& start, bool case_sensitive = true);
bool strEndWith(const std::string& str, const std::string& end, bool case_sensitive = true);

std::string strLeft(const std::string& str, size_t n);
std::string strRight(const std::string& str, size_t n);
std::string strMid(const std::string& str, size_t pos, size_t n);

bool strContains(const std::string& str, const std::string& contain, bool case_sensitive = true);
int strCompare(const std::string& str_first, const std::string& str_second, bool case_sensitive = true);
bool strIsEqual(const std::string& str_first, const std::string& str_second, bool case_sensitive = true);

template <typename ConstIterator>
std::string strJoin(ConstIterator c_it_begin, ConstIterator c_it_end, const std::string& del)
{
    std::string str_join = "";
    ConstIterator c_it;
    while(c_it_begin != c_it_end)
    {
        str_join += *c_it_begin + del;
        ++c_it_begin;
    }
    return str_join.substr(0, str_join.length() - 1);
}

template <typename Coll>
std::string strJoin(const Coll& coll, const std::string& del)
{
    std::string str_join = "";
    typename Coll::const_iterator c_it;
    for (c_it = coll.begin(); c_it != coll.end(); ++c_it)
        str_join += *c_it + del;
    return str_join.substr(0, str_join.length() - 1);
}

template <typename Coll>
inline size_t strSplit(const std::string& str, const std::string& del, Coll& out_coll, size_t limit = 0)
{
    std::string::size_type start = 0;
    std::string::size_type idx = 0;
    std::string::size_type del_len = del.length();

    idx = str.find(del, start);
    while (std::string::npos != idx)
    {
        if (limit > 0 && out_coll.size() + 1 >= limit)
            break;
        out_coll.push_back(str.substr(start, idx - start));

        start = idx + del_len;
        idx = str.find(del, start);
    }

    if (start<str.length())
        out_coll.push_back(str.substr(start));

    return out_coll.size();
}

char* strToCharArray(const std::string& str, char* pout_buf, size_t buf_len);

} // namespace util

#endif // UTIL_STRING_HPP
