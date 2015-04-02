#include "regex.hpp"
#include "regex/deelx.h"

namespace util
{

struct Regex::RegexData
{
    explicit RegexData(int flag) :
        regexer_(0, flag)
    {}
    RegexData(const std::string& expression, int flag) :
        regexer_(expression.c_str(), flag)
    {}
    CRegexpT<char> regexer_;
    MatchResult result_;
    std::string str_;
};

Regex::Regex(int flag) :
    pdata_(new RegexData(flag))
{}

Regex::Regex(const std::string& expression, int flag) :
    pdata_(new RegexData(expression, flag))
{}

Regex::~Regex()
{}

void Regex::compile(const std::string& expression, int flag)
{
    pdata_->regexer_.Compile(expression.c_str(), flag);
}

bool Regex::match(const std::string& str)
{
    pdata_->str_ = str;
    pdata_->result_ = pdata_->regexer_.MatchExact(pdata_->str_.c_str());
    return 0 != pdata_->result_.IsMatched() ? true : false;
}

bool Regex::search(const std::string& str)
{
    pdata_->str_ = str;
    pdata_->result_ = pdata_->regexer_.Match(pdata_->str_.c_str());
    return 0 != pdata_->result_.IsMatched() ? true : false;
}

size_t Regex::getMatchedGroupCnt() const
{
    return pdata_->result_.MaxGroupNumber() + 1;
}

std::string Regex::getMatchedGroup(size_t index) const
{
    int start = pdata_->result_.GetGroupStart(index);
    int end = pdata_->result_.GetGroupEnd(index);
    return (start >= 0 && end > start) ? pdata_->str_.substr(start, end - start) : "";
}

std::string Regex::getMatchedGroup(const std::string& name) const
{
    size_t num = pdata_->regexer_.GetNamedGroupNumber(name.c_str());
    return num < getMatchedGroupCnt() ? getMatchedGroup(num) : "";
}

} // namespace util
