#ifndef UTIL_REGEX_HPP
#define UTIL_REGEX_HPP

#include "base.hpp"

namespace util
{

class Regex
{
public:
    enum RegexFlag
	{
		NoFlag         = 0,
		SingleLine     = 0x01,
		MultiLine      = 0x02,
		Global         = 0x04,
		IgnoreCase     = 0x08,
		RightToLeft    = 0x10,
		Extended       = 0x20
	};

    explicit Regex(int flag = NoFlag);
    explicit Regex(const std::string& expression, int flag = NoFlag);
    ~Regex();
    void compile(const std::string& expression, int flag = NoFlag);
    bool match(const std::string& str);
    bool search(const std::string& str);
    size_t getMatchedGroupCnt() const;
    std::string getMatchedGroup(size_t index) const;
    std::string getMatchedGroup(const std::string& name) const;
private:
    struct RegexData;
    UtilAutoPtr<RegexData> pdata_;
private:
    DISALLOW_COPY_AND_ASSIGN(Regex)
};

} // namespace util

#endif // UTIL_REGEX_HPP
