#include "json.hpp"
#include "json/json.hpp"

using namespace std;
using namespace nlohmann;

namespace util
{

struct Json::JsonData
{
    json js_;
};

Json::Json() :
    pdata_(new JsonData())
{
}

Json::Json(const std::string& jstr) :
    pdata_(new JsonData())
{
    pdata_->js_ = json::parse(jstr);
}

Json Json::get(const std::string& key) const
{
    Json js;
    js.pdata_->js_ = pdata_->js_[key];
    return js;
}

void Json::set(const std::string&key, const Json& json)
{
    pdata_->js_[key] = json.pdata_->js_;
}

void Json::set(const std::string&key, int value)
{
    pdata_->js_[key] = value;
}

void Json::set(const std::string&key, size_t value)
{
    pdata_->js_[key] = value;
}

void Json::set(const std::string&key, float value)
{
    pdata_->js_[key] = value;
}

void Json::set(const std::string&key, double value)
{
   pdata_->js_[key] = value; 
}

void Json::set(const std::string&key, const std::string& value)
{
    pdata_->js_[key] = value;
}

void Json::set(const std::string&key, const vector<int>& value)
{
    pdata_->js_[key] = value;
}

void Json::set(const std::string&key, const vector<size_t>& value)
{
    pdata_->js_[key] = value;
}

void Json::set(const std::string&key, const vector<float>& value)
{
    pdata_->js_[key] = value;
}

void Json::set(const std::string&key, const vector<double>& value)
{
    pdata_->js_[key] = value;
}

void Json::set(const std::string&key, const vector<std::string>& value)
{
    pdata_->js_[key] = value;
}

// void Json::set(const std::string&key, const vector<Json>& value)
// {
//     vector<json> vec;
//     for (const Json& j : value)
//         vec.push_back(j.pdata_->js_);
    
//     pdata_->js_[key] = json::parse(vec.begin(), vec.end());
// }

string Json::toString() const
{
    return util::toString(pdata_->js_);
}

} //namespace util
