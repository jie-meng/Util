#ifndef UTIL_ANY_HPP
#define UTIL_ANY_HPP

// See http://www.boost.org/libs/any for Documentation.

// what:  variant type boost::any
// who:   contributed by Kevlin Henney,
//        with features contributed and bugs found by
//        Ed Brey, Mark Rodgers, Peter Dimov, and James Curran
// when:  July 2001
// where: tested with BCC 5.5, MSVC 6.0, and g++ 2.95

#include <algorithm>
#include <typeinfo>
#include "base.hpp"

namespace util
{
    class any
    {
    public: // structors

        any()
          : content(0)
        {
        }

        template<typename ValueType>
        any(const ValueType & value)
          : content(new holder<ValueType>(value))
        {
        }

        any(const any & other)
          : content(other.content ? other.content->clone() : 0)
        {
        }

        ~any()
        {
            delete content;
        }

    public: // modifiers

        any & swap(any & rhs)
        {
            std::swap(content, rhs.content);
            return *this;
        }

        template<typename ValueType>
        any & operator=(const ValueType & rhs)
        {
            any(rhs).swap(*this);
            return *this;
        }

        any & operator=(any rhs)
        {
            rhs.swap(*this);
            return *this;
        }

    public: // queries

        bool empty() const
        {
            return !content;
        }

        const std::type_info & type() const
        {
            return content ? content->type() : typeid(void);
        }

        //joshua
        std::string toString() const
        {
            return content ? content->toString() : "";
        }

//#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    private: // types
//#else
//   public: // types (public so any_cast can be non-friend)
//#endif

        class placeholder
        {
        public: // structors

            virtual ~placeholder()
            {
            }

        public: // queries

            virtual const std::type_info & type() const = 0;

            virtual placeholder * clone() const = 0;

            //joshua
            virtual std::string toString() const = 0;
        };

        template<typename ValueType>
        class holder : public placeholder
        {
        public: // structors

            holder(const ValueType & value)
              : held(value)
            {
            }

        public: // queries

            virtual const std::type_info & type() const
            {
                return typeid(ValueType);
            }

            virtual placeholder * clone() const
            {
                return new holder(held);
            }

            //joshua
            virtual std::string toString() const
            {
                return util::toString(held);
            }

        public: // representation

            ValueType held;

        private: // intentionally left unimplemented
            holder & operator=(const holder &);
        };

//#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS

    private: // representation

        template<typename ValueType>
        friend ValueType * any_cast(any *);

//        template<typename ValueType>
//        friend ValueType * unsafe_any_cast(any *);

//#else

//    public: // representation (public so any_cast can be non-friend)

//#endif

        placeholder * content;

    };

    class bad_any_cast : public std::bad_cast
    {
    public:
        virtual const char * what() const throw()
        {
            return "util::bad_any_cast: "
                   "failed conversion using util::any_cast";
        }
    };

    template<typename ValueType>
    ValueType * any_cast(any * operand)
    {
        return operand &&
//#ifdef BOOST_AUX_ANY_TYPE_ID_NAME
//            std::strcmp(operand->type().name(), typeid(ValueType).name()) == 0
//#else
            operand->type() == typeid(ValueType)
//#endif
            ? &static_cast<any::holder<ValueType> *>(operand->content)->held
            : 0;
    }

    template<typename ValueType>
    inline const ValueType * any_cast(const any * operand)
    {
        return any_cast<ValueType>(const_cast<any *>(operand));
    }

    template<typename ValueType>
    ValueType any_cast(any & operand)
    {
//        typedef BOOST_DEDUCED_TYPENAME remove_reference<ValueType>::type nonref;

//#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
//        // If 'nonref' is still reference type, it means the user has not
//        // specialized 'remove_reference'.
//
//        // Please use BOOST_BROKEN_COMPILER_TYPE_TRAITS_SPECIALIZATION macro
//        // to generate specialization of remove_reference for your class
//        // See type traits library documentation for details
//        BOOST_STATIC_ASSERT(!is_reference<nonref>::value);
//#endif
        typedef ValueType nonref;

        nonref * result = any_cast<nonref>(&operand);
        if(!result)
            throw bad_any_cast();
            //boost::throw_exception(bad_any_cast());
        return *result;
    }

    template<typename ValueType>
    inline ValueType any_cast(const any & operand)
    {
        //typedef BOOST_DEDUCED_TYPENAME remove_reference<ValueType>::type nonref;

//#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
//        // The comment in the above version of 'any_cast' explains when this
//        // assert is fired and what to do.
//        BOOST_STATIC_ASSERT(!is_reference<nonref>::value);
//#endif
        typedef ValueType nonref;

        //return any_cast<const nonref &>(const_cast<any &>(operand));
        return any_cast<nonref>(const_cast<any &>(operand));
    }

    // Note: The "unsafe" versions of any_cast are not part of the
    // public interface and may be removed at any time. They are
    // required where we know what type is stored in the any and can't
    // use typeid() comparison, e.g., when our types may travel across
    // different shared libraries.
//    template<typename ValueType>
//    inline ValueType * unsafe_any_cast(any * operand)
//    {
//        return &static_cast<any::holder<ValueType> *>(operand->content)->held;
//    }

//    template<typename ValueType>
//    inline const ValueType * unsafe_any_cast(const any * operand)
//    {
//        return unsafe_any_cast<ValueType>(const_cast<any *>(operand));
//    }

    //joshua
    inline std::string toString(const any& a) { return a.toString(); }

} // namespace util

#endif // UTIL_ANY_HPP
