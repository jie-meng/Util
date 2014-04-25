#ifndef UTIL_STRATEGY_HPP
#define UTIL_STRATEGY_HPP

#include "base.hpp"

namespace util
{

template<class T>
class StrategyHolder
{
public:
    StrategyHolder() : pstrategy_(0) {}
    explicit StrategyHolder(UtilAutoPtr<T> pstrategy) : pstrategy_(pstrategy) {}
    ~StrategyHolder() {}

    inline void set(UtilAutoPtr<T> pstrategy) { pstrategy_ = pstrategy; }
    inline T* get() { return pstrategy_.get(); }
private:
    UtilAutoPtr<T> pstrategy_;
private:
    DISALLOW_COPY_AND_ASSIGN(StrategyHolder)
};

} // namespace util

#endif // UTIL_STRATEGY_HPP
