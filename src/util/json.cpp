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

Json::~Json()
{
}

Json Json::parse(const std::string& jstr)
{
    Json json;
    json.pdata_->js_ = json::parse(jstr);
    return json;
}

Json::Json(const Json& js) :
    pdata_(new JsonData())
{
    pdata_->js_ = js.pdata_->js_;
}

Json& Json::operator=(const Json& js)
{
    if (&js != this)
    {
        pdata_->js_ = js.pdata_->js_;    
    }
    
    return *this;
}

Json Json::operator[](const std::string& key)
{
    Json json;
    json.pdata_->js_ = pdata_->js_[key];

    return json;
}

Json Json::get(const std::string& key) const
{
    Json js;
    js.pdata_->js_ = pdata_->js_[key];
    return js;
}

void Json::set(const std::string& key, int value)
{
    pdata_->js_[key] = value;
}

void Json::set(const std::string& key, double value)
{
   pdata_->js_[key] = value; 
}

void Json::set(const std::string& key, const std::string& value)
{
    pdata_->js_[key] = value;
}

void Json::set(const std::string& key, const Json& json)
{
    pdata_->js_[key] = json.pdata_->js_;
}

void Json::set(const std::string& key, const vector<int>& values)
{
    pdata_->js_[key] = values;
}

void Json::set(const std::string& key, const vector<double>& values)
{
    pdata_->js_[key] = values;
}

void Json::set(const std::string& key, const vector<std::string>& values)
{
    pdata_->js_[key] = values;
}

void Json::set(const std::string& key, const vector<Json>& values)
{
    vector<json> vec;
    for (const Json& j : values)
        vec.push_back(j.pdata_->js_);
    
    pdata_->js_[key] = vec;
}

string Json::toString() const
{
    return util::toString(pdata_->js_);
}

} //namespace util
