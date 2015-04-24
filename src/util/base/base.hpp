#ifndef UTIL_BASE_BASE_HPP
#define UTIL_BASE_BASE_HPP

//base include
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <exception>

#ifdef _MSVC_
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

#ifdef _BOOST_

#include <memory>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#define UtilFunction       boost::function
#define UtilBind           boost::bind
#define UtilAutoPtr        std::auto_ptr
#define UtilSharedPtr      boost::shared_ptr

#ifndef NULL
#define NULL  0
#endif

#elif defined _CPP11_

#include <memory>
#include <functional>
#define UtilFunction       std::function
#define UtilBind           std::bind
#define UtilAutoPtr        std::unique_ptr
#define UtilUniquePtr      std::unique_ptr
#define UtilSharedPtr      std::shared_ptr


#ifndef NULL
#define NULL nullptr
#endif

using namespace std::placeholders;

template <typename T>
inline void autoPtrMove(UtilAutoPtr<T>& to, UtilAutoPtr<T>& from) { to = std::move(from); }

#else

//STL-TR1 by default
#include <tr1/memory>
#include <tr1/functional>
#define UtilFunction       std::tr1::function
#define UtilBind           std::tr1::bind
#define UtilAutoPtr        std::auto_ptr
#define UtilSharedPtr      std::tr1::shared_ptr

#ifndef NULL
#define NULL  0
#endif

using namespace std::tr1::placeholders;

template <typename T>
inline void autoPtrMove(UtilAutoPtr<T>& to, UtilAutoPtr<T>& from) { to = from; }

#endif

#ifdef DISALLOW_COPY_AND_ASSIGN
#undef DISALLOW_COPY_AND_ASSIGN
#endif
#define DISALLOW_COPY_AND_ASSIGN(TypeName) TypeName(const TypeName&); TypeName& operator=(const TypeName&);

#ifdef SINGLETON
#undef SINGLETON
#endif
#define SINGLETON(TypeName) static TypeName& getInstance() { static TypeName s_instance; return s_instance; }

//base classes & functions
#include "constants.hpp"
#include "memory.hpp"
#include "in.hpp"
#include "out.hpp"
#include "baseclass.hpp"
#include "string.hpp"
#include "time.hpp"
#include "platform.hpp"

#endif // UTIL_BASE_BASE_HPP
