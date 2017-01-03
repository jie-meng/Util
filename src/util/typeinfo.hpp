#ifndef UTIL_TYPEINFO_HPP
#define UTIL_TYPEINFO_HPP

#include "base.hpp"
#include <typeinfo>
#include <cassert>

namespace util
{
    ////////////////////////////////////////////////////////////////////////////////
    // class TypeInfo
    // Purpose: offer a first-class, comparable wrapper over std::type_info
    ////////////////////////////////////////////////////////////////////////////////

    class TypeInfo
    {
    public:
        // Constructors
        TypeInfo(); // needed for containers
        TypeInfo(const std::type_info&); // non-explicit

        // Access for the wrapped std::type_info
        const std::type_info& get() const;
        // Compatibility functions
        bool before(const TypeInfo& rhs) const;
        std::string name() const;

    private:
        const std::type_info* pInfo_;
    };

// Implementation

    inline TypeInfo::TypeInfo()
    {
        class Nil {};
        pInfo_ = &typeid(Nil);
        assert(pInfo_);
    }

    inline TypeInfo::TypeInfo(const std::type_info& ti)
    : pInfo_(&ti)
    { assert(pInfo_); }

    inline bool TypeInfo::before(const TypeInfo& rhs) const
    {
        assert(pInfo_);
        return pInfo_->before(*rhs.pInfo_);
    }

    inline const std::type_info& TypeInfo::get() const
    {
        assert(pInfo_);
        return *pInfo_;
    }

    inline std::string TypeInfo::name() const
    {
        assert(pInfo_);
        return pInfo_->name();
    }

// Comparison operatorsUTIL_TYPEINFO_HPP

    inline bool operator==(const TypeInfo& lhs, const TypeInfo& rhs)
    { return lhs.get() == rhs.get(); }

    inline bool operator<(const TypeInfo& lhs, const TypeInfo& rhs)
    { return lhs.before(rhs); }

    inline bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs)
    { return !(lhs == rhs); }

    inline bool operator>(const TypeInfo& lhs, const TypeInfo& rhs)
    { return rhs < lhs; }

    inline bool operator<=(const TypeInfo& lhs, const TypeInfo& rhs)
    { return !(lhs > rhs); }

    inline bool operator>=(const TypeInfo& lhs, const TypeInfo& rhs)
    { return !(lhs < rhs); }
    
    inline std::string toString(const TypeInfo& ti) { return toString(ti.name()); }

} // namespace util

#endif // UTIL_TYPEINFO_HPP
