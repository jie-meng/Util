#ifndef UTIL_JSON_HPP
#define UTIL_JSON_HPP

#include <vector>
#include "base.hpp"

namespace util
{

class Json
{
public:
    Json();
    explicit Json(const std::string& jstr);
    
    Json get(const std::string& key) const;

    void set(const std::string&key, const Json& json);
    void set(const std::string&key, int value);
    void set(const std::string&key, size_t value);
    void set(const std::string&key, float value);
    void set(const std::string&key, double value);
    void set(const std::string&key, const std::string& value);

    void set(const std::string&key, const std::vector<int>& value);
    void set(const std::string&key, const std::vector<size_t>& value);
    void set(const std::string&key, const std::vector<float>& value);
    void set(const std::string&key, const std::vector<double>& value);
    void set(const std::string&key, const std::vector<std::string>& value);
    void set(const std::string&key, const std::vector<Json>& value);

    std::string toString() const;
public:
    //Json(const Json& js);
    //Json& operator= (const Json& js);
private:
    struct JsonData;
    UtilAutoPtr<JsonData> pdata_;
};

inline std::string toString(const Json& js)
{
    return js.toString();
}

} // namespace util

#endif // UTIL_JSON_HPP
