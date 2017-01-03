#ifndef UTIL_CMDLINE_HPP
#define UTIL_CMDLINE_HPP

#include <list>
#include <map>
#include <iterator>
#include "base.hpp"
#include "lexicalcast.hpp"

namespace util
{

//////////////////////////////////////////////////
// class name : CmdLine
// description :
// author :
// time : 2013-01-08-21.36
//////////////////////////////////////////////////
class CmdLine
{
    typedef UtilFunction<void (const std::string&)> CmdFunc;
public:
    explicit CmdLine(CmdFunc func, const std::string& prompt = ">>>", const std::string quit_cmd = "quit", bool is_case_sensitive = false);
    ~CmdLine();
    void start();
    inline bool isCaseSensitive() const { return is_case_sensitive_; }
    inline int strCompare(const std::string& str_first, const std::string& str_second) const { return util::strCompare(str_first, str_second, is_case_sensitive_); }
private:
    CmdFunc cmdfunc_;
    std::string prompt_;
    std::string quit_cmd_;
    bool is_case_sensitive_;
private:
    DISALLOW_COPY_AND_ASSIGN(CmdLine)
};

//////////////////////////////////////////////////
// class name : CommonCmdline
// description :
// author :
// time : 2013-01-08-21.36
//////////////////////////////////////////////////
class CommonCmdLine
{
public:
    explicit CommonCmdLine(const std::string& prompt = ">>>", const std::string quit_cmd = "quit", bool is_case_sensitive = false) :
        cmdline_(UtilBind(&CommonCmdLine::process, this, _1), prompt, quit_cmd, is_case_sensitive)
    {}
    virtual ~CommonCmdLine() {}

    virtual void process(const std::string& cmd) = 0;
    inline void start() { cmdline_.start(); }
    inline bool isCaseSensitive() const { return cmdline_.isCaseSensitive(); }
    inline int strCompare(const std::string& str_first, const std::string& str_second)
    { return cmdline_.strCompare(str_first, str_second); }
private:
    CmdLine cmdline_;
private:
    DISALLOW_COPY_AND_ASSIGN(CommonCmdLine)
};

//////////////////////////////////////////////////
// class name : CmdLineParser
// description :
// author :
// time : 2013-01-08-21.36
//////////////////////////////////////////////////
class CmdLineParser
{
    typedef std::map<std::string, std::string> KeyValueMap;
    typedef std::list<std::string> StrList;
public:
    explicit CmdLineParser(bool is_case_sensitive = false,
                           const std::string& params_del = " -",
                           const std::string& key_value_del = " ",
                           const std::string& value_start_flag = "\"",
                           const std::string& value_end_flag = "\"",
                           bool param_format_word = true);

    ~CmdLineParser() {}

    inline void setFormat(bool is_case_sensitive = false,
                                 const std::string& params_del = " -",
                                 const std::string& key_value_del = " ",
                                 const std::string& value_start_flag = "\"",
                                 const std::string& value_end_flag = "\"",
                                 bool param_format_word = true)
    {
        is_case_sensitive_ = is_case_sensitive;
        params_del_ = params_del;
        key_value_del_ = key_value_del;
        value_start_flag_ = value_start_flag;
        value_end_flag_ = value_end_flag;
        param_format_word_ = param_format_word;
    }
    inline bool isCaseSensitive() const { return is_case_sensitive_; }
    inline std::string getCmd() const { return isCaseSensitive() ? cmd_ : util::strToLower(cmd_); }
    inline std::string getCmdLine() const { return raw_cmdline_; }
    inline std::string getRet() const { return ret_; }
    std::string getContent() const;
    
    bool parse(const std::string& cmdline);
    
    inline bool hasKey(const std::string& key) const { return (key_value_map_.find(getFilterKeyWithCaseSensitive(key)) != key_value_map_.end()) ? true : false; }
    inline void removeKey(const std::string& key) { key_value_map_.erase(getFilterKeyWithCaseSensitive(key)); }

    std::string getValue(const std::string& key) const;

    template <typename T>
    bool getValue(const std::string& key, T& out_value) const;

    template <typename T>
    T getValue(const std::string& key, const T& default_value) const;
private:
    inline void clear() { cmd_ = ""; raw_cmdline_ = ""; key_value_map_.clear(); }
    bool parseGroupList(StrList& group_list);
    std::string getFilterKeyWithCaseSensitive(const std::string& key) const;
    bool isParamFormatOk(const std::string& param) const;
    bool setCmd(const std::string& cmd);
    bool setKeyValue(const std::string& key, const std::string& value);
private:
    bool is_case_sensitive_;
    std::string raw_cmdline_;
    std::string params_del_;
    std::string key_value_del_;
    std::string value_start_flag_;
    std::string value_end_flag_;
    bool param_format_word_;
    std::string cmd_;
    std::string ret_;
    KeyValueMap key_value_map_;
private:
    DISALLOW_COPY_AND_ASSIGN(CmdLineParser)
};

template<typename T>
bool CmdLineParser::getValue(const std::string& key, T& out_value) const
{
    KeyValueMap::const_iterator pos = key_value_map_.find(getFilterKeyWithCaseSensitive(key));
    if (pos != key_value_map_.end())
    {
        if (lexicalCast<T>(pos->second, out_value))
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}

template <typename T>
T CmdLineParser::getValue(const std::string& key, const T& default_value) const
{
    KeyValueMap::const_iterator pos = key_value_map_.find(getFilterKeyWithCaseSensitive(key));
    if (pos != key_value_map_.end())
        return lexicalCastDefault<T>(pos->second, default_value);
    else
        return default_value;
}

//////////////////////////////////////////////////
// class name : CmdLineMaker
// description :
// author :
// time : 2013-01-08-21.36
//////////////////////////////////////////////////
typedef std::pair<std::string, std::string> StrPair;
typedef std::list<StrPair> ArgList;

class CmdLineMaker
{
public:
    explicit CmdLineMaker(bool is_case_sensitive = false,
                 const std::string& params_del = " -",
                 const std::string& key_value_del = " ");
    ~CmdLineMaker();

    inline bool isCaseSensitive() const { return is_case_sensitive_; }
    inline std::string getCmd() const { return cmd_; }
    inline void setCmd(const std::string& cmd) { cmd_ = isCaseSensitive() ? cmd : util::strToLower(cmd); }
    std::string getCmdLine();
    void removeKey(const std::string& key);
    void sort(int argc, const char* argv[]);
    
    template<class Iter>
    void sort(Iter begin, Iter end)
    {
        ArgList arg_list_temp;
        Iter iter_x = begin;
        while (iter_x != end)
        {
            ArgList::iterator it = findKey(*iter_x);
            if (it != arg_list_.end())
            {
                arg_list_temp.push_back(*it);
                arg_list_.erase(it);
            }
            ++iter_x;
        }
        copy(arg_list_.begin(), arg_list_.end(), std::back_inserter(arg_list_temp));
        arg_list_.swap(arg_list_temp);
    }

    template<typename T>
    inline void addKeyValue(const std::string& key, const T& value)
    {
        std::string format_key = getFilterKeyWithCaseSensitive(key);
        ArgList::iterator it = findKey(format_key);
        if (it != arg_list_.end())
            return;
        arg_list_.push_back(std::make_pair(format_key, toString(value)));
    }
private:
    std::string getFilterKeyWithCaseSensitive(const std::string& key) const;
    ArgList::iterator findKey(const std::string& key);
private:
    std::string cmd_;
    bool is_case_sensitive_;
    std::string params_del_;
    std::string key_value_del_;
    ArgList arg_list_;
private:
    DISALLOW_COPY_AND_ASSIGN(CmdLineMaker)
};

} // namespace util

#endif // UTIL_CMDLINE_HPP
