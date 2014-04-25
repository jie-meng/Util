#ifndef UTIL_BASE_OUT_HPP
#define UTIL_BASE_OUT_HPP

namespace util
{

//print
typedef UtilFunction<void (const std::string& str)> PrintFunc;
extern PrintFunc print_func;

template <typename T>
inline std::string toString(const T& t)
{
    std::ostringstream os;
    os << t << std::flush;
    return os.str();
}
inline std::string toString(const std::string& str) { return str; }
inline std::string toString(const char* c_str) { return std::string(c_str); }

template <typename T>
inline void print(const T& t) { print_func(toString(t)); }
template <typename T>
inline void printLine(const T& t) { print_func(toString(t) + "\n"); }

void print(const char* fmt, ...);
void printLine(const char* fmt, ...);
//memory
std::string memoryStr(char* buf, size_t bytes);
inline void printMemory(char* buf, size_t bytes) { printLine(memoryStr(buf, bytes)); }

//standard version
inline void stdPrint(const std::string& str) { std::cout<<str<<std::flush; }
//user can set 'util::Print' with SetPrintFunc & ResetPrintFunc
inline void setPrintFunc(PrintFunc func) { print_func = func; }
inline void resetPrintFunc() { print_func = util::stdPrint; }

//trace
enum TraceLv {
    Verbose = 0,
    Debug,
    Info,
    Warn,
    Error,
    Fatal,
    Assert,
    MaxLv
};

void setTraceLv(TraceLv lv, bool enable);
inline void disableTraceLv(TraceLv lv) { setTraceLv(lv, 0); }
inline void enableTraceLv(TraceLv lv) { setTraceLv(lv, 1); }
bool isTraceLvEnable(TraceLv lv);
void trace(TraceLv lv, const char* fmt, ...);

} // namespace util

#endif // UTIL_BASE_OUT_HPP
