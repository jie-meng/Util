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
    inline DateTime getTime() const { return time_; }
    std::string getLevelStr() const;
    std::string getWdayStr() const;
    std::string getTimeStr() const;
    std::string getFormatedInformation() const;
private:
    InformationLevel level_;
    std::string module_;
    std::string info_;
    DateTime time_;
};

class InformationListener
{
public:
    virtual ~InformationListener() {};
    virtual void process(Information* pinfo) = 0;
};

} //namespace util

#endif // UTIL_TRACE_INFORMATION_HPP
