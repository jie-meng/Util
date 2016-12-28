#ifndef UTIL_CFG_HPP
#define UTIL_CFG_HPP

#include <map>
#include "base.hpp"
#include "lexicalcast.hpp"

namespace util
{

class TextCfg
{
public:
    TextCfg() {}
    explicit TextCfg(const std::string& cfg_file) { load(cfg_file); }
    ~TextCfg() {}

    inline std::string cfgFile() const { return cfg_file_; }
    void load(const std::string& cfg_file);
    void save(const std::string& cfg_file);
    void save();

    inline std::string getValue(const std::string& key, const std::string& default_value) 
        { return getValue<std::string>(key, default_value); }

    template <typename T>
    T getValue(const std::string& key, const T& default_value);

    template <typename T>
    void setValue(const std::string& key, const T& value);
private:
    static inline bool isEmpty(const std::string& line) { return line.empty(); }
private:
    typedef std::map<std::string, std::string> CfgMap;
    CfgMap cfg_map_;
    std::string cfg_file_;
private:
    DISALLOW_COPY_AND_ASSIGN(TextCfg)
};

template <typename T>
T TextCfg::getValue(const std::string& key, const T& default_value)
{
    CfgMap::iterator it = cfg_map_.find(key);
    if (it == cfg_map_.end())
        return default_value;

    return lexicalCastDefault<T>(it->second, default_value);
}

template <typename T>
void TextCfg::setValue(const std::string& key, const T& value)
{
    if (util::strTrim(key).empty())
        return;
    cfg_map_[util::strTrim(key)] = util::strTrim(toString(value));
}

} // namespace util

#endif // UTIL_CFG_HPP
