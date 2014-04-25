#ifndef UTIL_COLLECTION_HPP
#define UTIL_COLLECTION_HPP

#include "base.hpp"
#include <map>

namespace util
{

template <class Iter>
void printElements(Iter begin, Iter end, const std::string& del = " ")
{
    int d = 0;
    d = static_cast<int>(std::distance(begin, end));
    if (d > 0)
    {
        for (; begin != end; ++begin)
        {
            print(*begin);
            print(del);
        }
        print("\n");
    }
}

template <class T>
void printElements(const T& coll, const std::string& del = " ")
{
    if (coll.size() > 0)
    {
        typename T::const_iterator it;
        for (it = coll.begin(); it != coll.end(); ++it)
        {
            print(*it);
            print(del);
        }
        print("\n");
    }
}

template <class K, class V>
class RegisterMap
{
    typedef std::map<K, V> RegMap;
public:
    RegisterMap() {}
    ~RegisterMap() {}

    bool registerKey(const K& key, const V& value)
    { return reg_map_.insert(typename RegMap::value_type(key, value)).second; }

    bool unRegisterKey(const K& key)
    { return 1 == reg_map_.erase(key); }

    bool getValue(const K& key, V& out_value)
    {
        typename RegMap::iterator it = reg_map_.find(key);
        if (it == reg_map_.end())
            return false;
        out_value = it->second;
        return true;
    }
    void clear() { reg_map_.clear(); }
private:
    RegMap reg_map_;
};

} // namespace util

#endif // UTIL_COLLECTION_HPP
