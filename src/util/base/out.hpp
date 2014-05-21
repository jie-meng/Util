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
const std::string kTraceVerbose = "verbose";
const std::string kTraceDebug = "debug";
const std::string kTraceInfo = "info";
const std::string kTraceWarn = "warn";
const std::string kTraceError = "error";
const std::string kTraceFatal = "fatal";
const std::string kTraceAssert = "assert";
const std::string kTraceSilent = "silent";

class Trace
{
public:
    SINGLETON(Trace)
    enum Level {
        Verbose = 0,
        Debug,
        Info,
        Warn,
        Error,
        Fatal,
        Assert,
        Silent
    };
    inline void setTraceLevel(Level lv) { trace_level_ = lv; }
    inline Trace::Level getTraceLevel() const { return trace_level_; }
private:
    Trace() : trace_level_(Info) {}
    ~Trace() {}
private:
    Level trace_level_;
private:
    DISALLOW_COPY_AND_ASSIGN(Trace)
};

std::string traceLvStr(Trace::Level lv);
Trace::Level getTraceLv();
void setTraceLevel(Trace::Level lv);
std::string getTraceLevelStr();
void setTraceLevelStr(const std::string& str);
void trace(Trace::Level lv, const char* fmt, ...);

} // namespace util

#endif // UTIL_BASE_OUT_HPP
