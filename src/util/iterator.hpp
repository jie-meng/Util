#ifndef UTIL_ITERATOR_HPP
#define UTIL_ITERATOR_HPP

#include "base.hpp"

namespace util
{

template <typename T>
class Iterator : public Interface
{
public:
    virtual bool hasNext() = 0;
    virtual T& next() = 0;
};

template <typename T, class Collection>
class CollectionIterator : public Iterator<T>
{
public:
    explicit CollectionIterator(Collection& coll) :
        coll_(coll),
        it_(coll.begin())
    {}
    virtual bool hasNext() { return it_ != coll_.end(); }
    virtual T& next() { return *it_++; }
private:
    Collection& coll_;
    typename Collection::iterator it_;
};

template <typename K, typename V>
class MapIterator : public Interface
{
public:
    virtual bool hasNext() = 0;
    virtual std::pair<K, V> nextKeyValue() = 0;
};

template <typename K, typename V, class Map>
class MapCollectionIterator : public MapIterator<K, V>
{
public:
    explicit MapCollectionIterator(Map& map) :
        map_(map),
        it_(map.begin())
    {}
    virtual bool hasNext() { return it_ != map_.end(); }
    virtual std::pair<K, V> nextKeyValue() { return *it_++; }
private:
    Map& map_;
    typename Map::iterator it_;
};

} // namespace util

#endif // UTIL_ITERATOR_HPP
