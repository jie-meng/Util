#include "cmdline.hpp"
#include <vector>
#include "collection.hpp"
#include "regex.hpp"

namespace util
{

//////////////////////////////////////////////////
// class name : CmdLine
// description :
// author :
// time : 2013-01-08-21.36
//////////////////////////////////////////////////
CmdLine::CmdLine(CmdFunc func, const std::string& prompt, const std::string quit_cmd, bool is_case_sensitive) :
    cmdfunc_(func),
    prompt_(prompt),
    quit_cmd_(quit_cmd),
    is_case_sensitive_(is_case_sensitive)
{}

CmdLine::~CmdLine()
{}

void CmdLine::start()
{
    while(true)
    {
        //input
        print(prompt_);
        std::string cmd = getLine();

        if (strCompare(cmd, quit_cmd_))
        {
            if (cmd.length() > 0)
                cmdfunc_(cmd);
        }
        else
        {
            //quit
            break;
        }
    }
}

//////////////////////////////////////////////////
// class name : CmdLineParser
// description :
// author :
// time : 2013-01-08-21.36
//////////////////////////////////////////////////
typedef std::vector<std::string> StrVec;

CmdLineParser::CmdLineParser(bool is_case_sensitive,
                             const std::string& params_del,
                             const std::string& key_value_del,
                             const std::string& value_start_flag,
                             const std::string& value_end_flag,
                             bool param_format_word) :
                                 is_case_sensitive_(is_case_sensitive),
                                 params_del_(params_del),
                                 key_value_del_(key_value_del),
                                 value_start_flag_(value_start_flag),
                                 value_end_flag_(value_end_flag),
                                 param_format_word_(param_format_word)
{}

bool CmdLineParser::parse(const std::string& cmdline)
{
    clear();

    raw_cmdline_ = util::strTrim(cmdline);
    if (0 == raw_cmdline_.length())
    {
        ret_ = "cmdline is empty.";
        return false;
    }

    StrList group_list;
    util::strSplit(raw_cmdline_, params_del_, group_list);

    if (group_list.empty())
    {
        ret_ = "no cmd or params.";
        return false;
    }

    if (!setCmd(group_list.front()))
        return false;

    if(!parseGroupList(group_list))
    {
        clear();
        return false;
    }

    ret_ = "ok.";
    return true;
}

std::string CmdLineParser::getValue(const std::string& key) const
{
    KeyValueMap::const_iterator pos = key_value_map_.find(getFilterKeyWithCaseSensitive(key));
    if (pos != key_value_map_.end())
        return pos->second;
    else
        return "";
}

std::string CmdLineParser::content() const
{
    std::string content = "cmd[" + cmd_ + "]";
    KeyValueMap::const_iterator it;
    for (it = key_value_map_.begin(); it != key_value_map_.end(); ++it)
        content += "\nkey[" + it->first + "] value[" + it->second + "]";
    return content;
}

bool CmdLineParser::parseGroupList(StrList& group_list)
{
    if (group_list.size() > 1)
    {
        StrList::iterator it = group_list.begin();
        while(it != group_list.end())
        {
            if (it == group_list.begin())
            {
                //skip cmd
                ++it;
                continue;
            }

            StrVec key_value_vec;
            util::strSplit(*it, key_value_del_, key_value_vec, 2);
            if (2 == key_value_vec.size())
            {
                std::string tmp_value = util::strTrimLeft(key_value_vec[1]);
                if (util::strStartWith(tmp_value, value_start_flag_))
                {
                    if (util::strEndWith(util::strTrimRight(key_value_vec[1]), value_end_flag_) &&
                        util::strTrim(key_value_vec[1]).size() != 1)
                    {
                        std::string final_value = util::strTrim(tmp_value);
                        final_value = final_value.substr(1);
                        final_value = final_value.substr(0, final_value.length() - 1);

                        if(!setKeyValue(key_value_vec[0], final_value))
                            return false;
                    }
                    else
                    {
                        ++it;
                        while(it != group_list.end())
                        {
                            tmp_value += params_del_ + *it;
                            if (util::strEndWith(util::strTrimRight(tmp_value), value_end_flag_))
                            {
                                std::string final_value = util::strTrim(tmp_value);
                                final_value = final_value.substr(1);
                                final_value = final_value.substr(0, final_value.length() - 1);

                                if(!setKeyValue(key_value_vec[0], final_value))
                                    return false;
                                break;
                            }
                            else
                            {
                                ++it;
                            }
                        }

                        if (it == group_list.end())
                            break;
                    }
                }
                else
                {
                    if(!setKeyValue(key_value_vec[0], key_value_vec[1]))
                        return false;
                }
            }
            else if (key_value_vec.size() == 1)
            {
                if(!setKeyValue(key_value_vec[0], ""))
                    return false;
            }

            ++it;
        }
    }
    return true;
}

std::string CmdLineParser::getFilterKeyWithCaseSensitive(const std::string& key) const
{
    std::string format_key(key);
    if (!isCaseSensitive())
        format_key = util::strToLower(key);
    return format_key;
}

bool CmdLineParser::isParamFormatOk(const std::string& param) const
{
    if (!param_format_word_)
        return true;

    return Regex("\\w+").match(param) ? true : false;
}

bool CmdLineParser::setCmd(const std::string& cmd)
{
    std::string format_cmd = util::strTrim(cmd);
    if (!isCaseSensitive())
        format_cmd = util::strToLower(format_cmd);

    if (util::strContains(format_cmd, key_value_del_) ||
        util::strContains(format_cmd, value_start_flag_) ||
        util::strContains(format_cmd, value_end_flag_) ||
        !isParamFormatOk(format_cmd))
    {
        ret_ = "invalid cmd \"" + cmd + "\".";
        return false;
    }

    cmd_ = format_cmd;
    return true;
}

bool CmdLineParser::setKeyValue(const std::string& key, const std::string& value)
{
    std::string format_key = util::strTrim(key);
    if (!isCaseSensitive())
        format_key = util::strToLower(format_key);

    if (!isParamFormatOk(format_key))
    {
        ret_ = "invalid key \"" + key + "\".";
        return false;
    }

    key_value_map_[format_key] = util::strTrim(value);
    return true;
}

//////////////////////////////////////////////////
// class name : CmdLineMaker
// description :
// author :
// time : 2013-01-08-21.36
//////////////////////////////////////////////////

CmdLineMaker::CmdLineMaker(bool is_case_sensitive,
                 const std::string& params_del,
                 const std::string& key_value_del) :
                     is_case_sensitive_(is_case_sensitive),
                     params_del_(params_del),
                     key_value_del_(key_value_del)
{}

CmdLineMaker::~CmdLineMaker()
{}

std::string CmdLineMaker::getFilterKeyWithCaseSensitive(const std::string& key) const
{
    std::string format_key(key);
    if (!isCaseSensitive())
        format_key = util::strToLower(key);
    return format_key;
}

std::string CmdLineMaker::getCmdLine()
{
    std::string cmdline(cmd_);
    ArgList::iterator it;
    for (it = arg_list_.begin(); it != arg_list_.end(); ++it)
    {
        if ("" == it->second)
            cmdline += params_del_ + it->first;
        else
            cmdline += params_del_ + it->first + key_value_del_ + it->second;
    }
    return util::strTrim(cmdline);
}

ArgList::iterator CmdLineMaker::findKey(const std::string& key)
{
    ArgList::iterator it;
    for (it = arg_list_.begin(); it != arg_list_.end(); ++it)
    {
        if (it->first == getFilterKeyWithCaseSensitive(key))
            break;
    }
    return it;
}

void CmdLineMaker::sort(int argc, const char* argv[])
{
    std::vector<std::string> str_vec;
    for (int i=0; i<argc; ++i)
        str_vec.push_back(argv[i]);
    sort(str_vec.begin(), str_vec.end());
}

void CmdLineMaker::removeKey(const std::string& key)
{
    ArgList::iterator it = findKey(key);
    if (it != arg_list_.end())
        arg_list_.erase(it);
}

} // namespace util

