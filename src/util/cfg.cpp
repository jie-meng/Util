#include "cfg.hpp"
#include <vector>
#include <list>
#include <algorithm>
#include "file.hpp"

namespace util
{

typedef std::vector<std::string> StrVector;
typedef std::list<std::string> StrList;

void TextCfg::load(const std::string& cfg_file)
{
    cfg_file_ = cfg_file;
    cfg_map_.clear();

    if (isPathFile(cfg_file))
    {
        std::string text = readTextFile(cfg_file);
        StrList line_list;
        strSplit(text, "\n", line_list);

        StrVector vec;
        for (StrList::iterator it = line_list.begin(); it != line_list.end(); ++it)
        {
            vec.clear();
            strSplit(*it, "=", vec, 2);
            if (2 == vec.size())
                cfg_map_[strTrim(vec[0])] = strTrim(vec[1]);

        }
    }
}

void TextCfg::save(const std::string& cfg_file)
{
    StrList line_list;
    for (CfgMap::iterator it = cfg_map_.begin(); it != cfg_map_.end(); ++it)
        line_list.push_back(it->first + " = " + it->second);

    line_list.sort();
    StrList::iterator pend = remove_if(line_list.begin(), line_list.end(), TextCfg::isEmpty);
    std::string text = strJoin(line_list.begin(), pend, "\n");
    writeTextFile(cfg_file, text);
}

void TextCfg::save()
{
    save(cfg_file_);
}

} // namespace util
