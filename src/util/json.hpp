#ifndef UTIL_JSON_HPP
#define UTIL_JSON_HPP

#include <vector>
#include "base.hpp"

namespace util
{

class Json
{
public:
    enum JsonType
    {
        null,            ///< null value
        object,          ///< object (unordered set of name/value pairs)
        array,           ///< array (ordered collection of values)
        string,          ///< string value
        boolean,         ///< boolean value
        number_integer,  ///< number value (signed integer)
        number_unsigned, ///< number value (unsigned integer)
        number_float,    ///< number value (floating-point)
        discarded        ///< discarded by the the parser callback function
    };
    
    static Json parse(const std::string& jstr);

    Json();
    ~Json();
    Json(const Json& js);
    Json& operator=(const Json& js);
    Json operator[](const std::string& key);

    JsonType getType();
    bool isNull() const;
    bool isBoolean() const;
    bool isNumber() const;
    bool isNumberInteger() const;
    bool isNumberFloat() const;
    bool isObject() const;
    bool isArray() const;
    bool isString() const;

    Json get(const std::string& key) const;

    void set(const std::string& key, int value);
    void set(const std::string& key, double value);
    void set(const std::string& key, bool value);
    void set(const std::string& key, const char* value);
    void set(const std::string& key, const std::string& value);
    void set(const std::string& key, const Json& json);
    
    void set(const std::string& key, const std::vector<int>& values);
    void set(const std::string& key, const std::vector<double>& values);
    void set(const std::string& key, const std::vector<bool>& values);
    void set(const std::string& key, const std::vector<std::string>& values);
    void set(const std::string& key, const std::vector<Json>& values);

    std::string toString() const;
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