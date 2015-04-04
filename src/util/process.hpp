#ifndef UTIL_PROCESS_HPP
#define UTIL_PROCESS_HPP

#include "base.hpp"

namespace util
{

//execute process and wait it end, returns process exit_code (fail : -1)
int executeProcess(const std::string& cmdline, const std::string& cur_path = "");

//execute process and do not wait, returns immidiately.
bool executeProcessAsyn(const std::string& cmdline, const std::string& cur_path = "");

class Process
{
    typedef UtilFunction<void (const std::string&)> OutputFunc;
public:
    Process();
    ~Process();
    bool create(const std::string& cmdline,
        const std::string& cur_path = "",
        bool input = false,
        bool output = false,
        OutputFunc output_func = 0);
    void kill();
    bool input(const std::string& str);
    bool isRunning();
private:
    struct ProcessImpl;
    UtilAutoPtr<ProcessImpl> impl_;
private:
    DISALLOW_COPY_AND_ASSIGN(Process)
};

} //namespace util

#endif // UTIL_PROCESS_HPP
