#ifndef UTIL_BASE_BASECLASS_HPP
#define UTIL_BASE_BASECLASS_HPP

namespace util
{

//Interface
class Interface
{
public:
    virtual ~Interface() {}
};

//Uncopyable
class Uncopyable
{
public:
    Uncopyable() {}
    ~Uncopyable() {}
private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator=(const Uncopyable&);
};

//singleton
template <class T>
class Singleton
{
public:
    static T& getInstance() { static T s_instance; return s_instance; }
private:
    Singleton();
    ~Singleton();
};

//NullType
class NullType;

//EmptyType
class EmptyType {};

////////////////////////////////////////////////////////////////////////////////
// class template Int2Type
// Converts each integral constant into a unique type
// Invocation: Int2Type<v> where v is a compile-time constant integral
// Defines 'value', an enum that evaluates to v
////////////////////////////////////////////////////////////////////////////////
template <int v>
struct Int2Type
{
    enum { value = v };
};

////////////////////////////////////////////////////////////////////////////////
// class template Type2Type
// Converts each type into a unique, insipid type
// Invocation Type2Type<T> where T is a type
// Defines the type OriginalType which maps back to T
////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct Type2Type
{
    typedef T OriginalType;
};

////////////////////////////////////////////////////////////////////////////////
// select
////////////////////////////////////////////////////////////////////////////////
template <bool flag, typename T, typename U>
struct Select
{
    typedef T Result;
};
template <typename T, typename U>
struct Select<false, T, U>
{
    typedef U Result;
};

////////////////////////////////////////////////////////////////////////////////
// ScopedFlag
////////////////////////////////////////////////////////////////////////////////
template<typename T>
class ScopedFlag
{
public:
    ScopedFlag(T& rv, const T& in, const T& out) :
        rv_(rv),
        out_(out)
    { rv_ = in; }
    ~ScopedFlag() { rv_ = out_; }
private:
    T& rv_;
    T out_;
};

////////////////////////////////////////////////////////////////////////////////
// Exception
////////////////////////////////////////////////////////////////////////////////
class Exception : public std::exception
{
public:
    explicit Exception(const std::string& message) throw() : name_("Exception"), message_(message)
    {}
    virtual ~Exception() throw() {}
public:
    inline std::string getName() const { return name_; }
    inline std::string getMessage() const { return message_; }
    virtual const char* what() const throw()
    { name_message_ = name_ + " : " + message_; return name_message_.c_str(); }
    virtual Exception* clone() const { return new Exception(getMessage()); }
protected:
    inline void setName(const std::string& name) { name_ = name; }
private:
    std::string name_;
    std::string message_;
    mutable std::string name_message_;
};

} // namespace util

#endif // UTIL_BASE_BASECLASS_HPP
