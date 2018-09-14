#ifndef UTIL_BASE_MEMORY_HPP
#define UTIL_BASE_MEMORY_HPP

#include <cstdlib>
#include "baseclass.hpp"

namespace util
{

template <typename T>
inline void safeDelete(T*& ref_p) { if(ref_p) { delete ref_p; ref_p = 0; } }

template <typename T>
inline void safeDeleteArray(T*& ref_p) { if(ref_p) { delete [] ref_p; ref_p = 0; } }

template <typename T>
inline void deleteVoid(void* p) { if (p) delete (T*)p; }

template <typename T>
class Memory
{
public:
    Memory() : length_(0), buf_(0) {}
    explicit Memory(size_t length) : length_(length), buf_(0) { create(length_); }
    ~Memory() { safeDeleteArray(buf_); }

    T* create(size_t length)
    {
        safeDeleteArray(buf_);
        length_ = length;
        buf_ = new T[length];
        if (buf_)
            clear();
        return buf_;
    }

    T* buf(int index = 0) const
    {
        if (abs(index) > length())
            throw Exception("Memory.buf() Index Out of range.");

        return index < 0 ? buf_ + (length() + index) : buf_ + index;
    }

    T& operator[](size_t i)
    {
        return buf()[i];
    }

    const T& operator[](size_t i) const
    {
        return buf()[i];
    }

    void clear() const { memset(buf_, 0, size()); }
    size_t length() const { return length_; }
    size_t size() const { return length() * sizeof(T); }
private:
    size_t length_;
    T* buf_;
};

} // namespace util

#endif // UTIL_MEMORY_HPP
