#ifndef UTIL_BASE_MEMORY_HPP
#define UTIL_BASE_MEMORY_HPP

#include <cstdlib>

namespace util
{

//delete
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
    Memory() :count_(0), buf_(0) {}
    explicit Memory(size_t length) : count_(length), buf_(0) { create(count_); }
    ~Memory() { safeDeleteArray(buf_); }

    T* create(size_t length)
    {
        safeDeleteArray(buf_);
        count_ = length;
        buf_ = new T[length];
        if (buf_)
            clear();
        return buf_;
    }

    T* buf() const { return buf_; }
    void clear() const { memset(buf_, 0, size()); }
    size_t count() const { return count_; }
    size_t size() const { return count() * sizeof(T); }
private:
    size_t count_;
    T* buf_;
};

} // namespace util

#endif // UTIL_MEMORY_HPP
