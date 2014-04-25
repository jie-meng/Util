#ifndef UTIL_BASE_IN_HPP
#define UTIL_BASE_IN_HPP

namespace util
{

//input
inline std::string getInput() { std::string str; std::cin>>str; return str; }
inline std::string getLine() { std::string str; std::getline(std::cin, str); return str; }

} // namespace util

#endif // UTIL_BASE_IN_HPP
