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
    static AbstractProduct* onUnknownType(IdentifierType)
    {
        throw Exception("Unknown Type");
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
    { 
        return 0; 
    }
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
        ProductCreator product_creator = creator(id);
        return product_creator ? product_creator() : FactoryErrorPolicy<IdentifierType, AbstractProduct>::onUnknownType(id);
    }
private:
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
    class ProductCreator = UtilFunction<AbstractProduct* (AbstractProduct*)>,
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

    AbstractProduct* createObject(AbstractProduct* model)
    {
        if (model == 0) return 0;

        typename IdToProductMap::iterator i =
            associations_.find(TypeInfo(typeid(*model)));
        
        if (i != associations_.end())
            return (i->second)(model);
        
        return FactoryErrorPolicy<TypeInfo, AbstractProduct>::onUnknownType(typeid(*model));
    }

private:
    typedef std::map<TypeInfo, ProductCreator> IdToProductMap;
    IdToProductMap associations_;
};

} // namespace util

#endif // UTIL_FACTORY_HPP
