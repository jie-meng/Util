#include "json.hpp"
#include "json/json.hpp"

using namespace std;
using namespace nlohmann;

namespace util
{

// // other stuff
// j.size();     // 3 entries
// j.empty();    // false
// j.type();     // json::value_t::array
// j.clear();    // the array is empty again

// // convenience type checkers
// j.is_null();
// j.is_boolean();
// j.is_number();
// j.is_object();
// j.is_array();
// j.is_string();

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

Json::JsonType Json::getType()
{
    return (JsonType)pdata_->js_.type();
}

bool Json::isNull() const
{
    return pdata_->js_.is_null();
}

bool Json::isBoolean() const
{
    return pdata_->js_.is_boolean();
}

bool Json::isNumber() const
{
    return pdata_->js_.is_number();
}

bool Json::isNumberInteger() const
{
    return pdata_->js_.is_number_integer();
}

bool Json::isNumberFloat() const
{
    return pdata_->js_.is_number_float();
}

bool Json::isObject() const
{
    return pdata_->js_.is_object();
}

bool Json::isArray() const
{
    return pdata_->js_.is_array();
}

bool Json::isString() const
{
    return pdata_->js_.is_string();
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

void Json::set(const std::string& key, bool value)
{
    pdata_->js_[key] = value; 
}

void Json::set(const std::string& key, const char* value)
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

void Json::set(const std::string& key, const std::vector<bool>& values)
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
