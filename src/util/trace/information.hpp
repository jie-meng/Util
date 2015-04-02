#ifndef UTIL_TRACE_INFORMATION_HPP
#define UTIL_TRACE_INFORMATION_HPP

#include "util/base.hpp"

namespace util
{

class Information
{
public:
    enum InformationLevel
    {
        verbose = 0,
        debug,
        info,
        wanning,
        error,
        fatal,
        assert,
        silent,
        maxlevel
    };

    Information(const std::string& module, InformationLevel info_level, const std::string& info);
    ~Information();
    inline InformationLevel getLevel() const { return level_; }
    inline std::string getModule() const { return module_; }
    inline std::string getInfo() const { return info_; }
    inline tm getTime() const { return time_; }
    inline size_t getYear() const { return time_.tm_year + 1900; }
    inline size_t getMonth() const { return time_.tm_mon + 1; }
    inline size_t getDay() const { return time_.tm_mday; }
    inline size_t getWday() const { return time_.tm_wday; }
    inline size_t getHour() const { return time_.tm_hour; }
    inline size_t getMinute() const { return time_.tm_min; }
    inline size_t getSecond() const { return time_.tm_sec; }
    std::string getLevelStr() const;
    std::string getWdayStr() const;
    std::string getTimeStr() const;
    std::string getFormatedInformation() const;
private:
    InformationLevel level_;
    std::string module_;
    std::string info_;
    tm time_;
};

class InformationListener
{
public:
    virtual ~InformationListener() {};
    virtual void process(Information* pinfo) = 0;
};

} //namespace util

#endif // UTIL_TRACE_INFORMATION_HPP
