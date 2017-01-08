#ifndef UTIL_ITERATOR_HPP
#define UTIL_ITERATOR_HPP

#include "base.hpp"

namespace util
{

template <typename T>
class Iterator
{
public:
    template <class Collection>
    Iterator(Collection& coll, int extra) :
        pimpl_(new Impl<Collection>(coll))
    {}
    
    Iterator(const Iterator<T>& other) :
        pimpl_(other.pimpl_->clone())
    {}

    Iterator<T>& operator=(const Iterator<T>& rhs)
    {
    	if (this != &rhs)
            pimpl_.reset(rhs.pimpl_->clone());
        return *this;
    }

    bool hasNext()
    {
        return pimpl_->hasNext();
    }
    
    T next()
    {
        return pimpl_->next();
    }
private:
    template <typename U>
    class ImplBase
    {
    public:
        virtual ~ImplBase() {}
        virtual bool hasNext() = 0;
        virtual U next() = 0;
        virtual ImplBase<U>* clone() const = 0;
    };

    template <class Collection>
    class Impl : public ImplBase<T>
    {
    public:
        explicit Impl(Collection& coll) :
            coll_(coll),
            it_(coll.begin())
        {} 
        
        virtual bool hasNext()
        {
            return it_ != coll_.end();
        }
        
        virtual T next()
        {
            return *it_++;
        }
        
        virtual ImplBase<T>* clone() const
        {
            auto p = new Impl<Collection>(coll_);
            p->it_ = it_;
            return p;
        }
    private:
        Collection& coll_;
        typename Collection::iterator it_;
    };
private:
    UtilAutoPtr< ImplBase<T> > pimpl_;
};

template <typename T, class Collection>
inline Iterator<T> createIterator(Collection& coll)
{
    return Iterator<T>(coll, 0);
}

} // namespace util

#endif // UTIL_ITERATOR_HPP
