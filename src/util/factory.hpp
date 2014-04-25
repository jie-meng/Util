#ifndef UTIL_FACTORY_HPP
#define UTIL_FACTORY_HPP

#include "base.hpp"
#include <map>
#include "typeinfo.hpp"

namespace util
{

////////////////////////////////////////////////////////////////////////////////
// class template DefaultFactoryError
// Manages the "Unknown Type" error in an object factory
////////////////////////////////////////////////////////////////////////////////

template <typename IdentifierType, class AbstractProduct>
struct DefaultFactoryError
{
    struct Exception : public std::exception
    {
        const char* what() const throw() { return "Unknown Type"; }
    };

    static AbstractProduct* onUnknownType(IdentifierType)
    {
        throw Exception();
    }
};

////////////////////////////////////////////////////////////////////////////////
// class template DefaultFactoryError
// Manages the "Unknown Type" error in an object factory
////////////////////////////////////////////////////////////////////////////////

template <typename IdentifierType, class AbstractProduct>
struct FactoryCreateFail
{
    static AbstractProduct* onUnknownType(IdentifierType)
    { return 0; }
};

////////////////////////////////////////////////////////////////////////////////
// class template Factory
// Implements a generic object factory
////////////////////////////////////////////////////////////////////////////////
//AbstractProduct* (*)()
template
<
    class AbstractProduct,
    typename IdentifierType,
    typename ProductCreator = UtilFunction<AbstractProduct* ()>,
    template<typename, class>
        class FactoryErrorPolicy = FactoryCreateFail
>
class Factory
    : public FactoryErrorPolicy<IdentifierType, AbstractProduct>
{
public:
    bool Register(const IdentifierType& id, ProductCreator creator)
    {
        return associations_.insert(
            typename IdToProductMap::value_type(id, creator)).second;
    }

    bool unRegister(const IdentifierType& id)
    {
        return associations_.erase(id) == 1;
    }

    AbstractProduct* createObject(const IdentifierType& id)
    {
        ProductCreator creator = creator(id);
        if (creator)
            return creator();
        else
            FactoryErrorPolicy<IdentifierType, AbstractProduct>::onUnknownType(id);
    }

    ProductCreator creator(const IdentifierType& id)
    {
        typename IdToProductMap::const_iterator i = associations_.find(id);
        if (i != associations_.end())
            return i->second;

        return ProductCreator();
    }

private:
    typedef std::map<IdentifierType, ProductCreator> IdToProductMap;
    IdToProductMap associations_;
};

////////////////////////////////////////////////////////////////////////////////
// class template CloneFactory
// Implements a generic cloning factory
////////////////////////////////////////////////////////////////////////////////

template
<
    class AbstractProduct,
    class ProductCreator = UtilFunction<AbstractProduct* (const AbstractProduct*)>,
    template<typename, class>
        class FactoryErrorPolicy = FactoryCreateFail
>
class CloneFactory
    : public FactoryErrorPolicy<TypeInfo, AbstractProduct>
{
public:
    bool Register(const TypeInfo& ti, ProductCreator creator)
    {
        return associations_.insert(
            typename IdToProductMap::value_type(ti, creator)).second;
    }

    bool unRegister(const TypeInfo& id)
    {
        return associations_.erase(id) == 1;
    }

    AbstractProduct* createObject(const AbstractProduct* model)
    {
        if (model == 0) return 0;

        typename IdToProductMap::const_iterator i =
            associations_.find(typeid(*model));
        if (i != associations_.end())
        {
            return (i->second)(model);
        }
        return FactoryErrorPolicy<TypeInfo, AbstractProduct>::onUnknownType(typeid(*model));
    }

private:
    typedef std::map<TypeInfo, ProductCreator> IdToProductMap;
    IdToProductMap associations_;
};

} // namespace util

#endif // UTIL_FACTORY_HPP
