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
#else  //STL-TR1
#include <tr1/memory>
#include <tr1/functional>
#define UtilFunction       std::tr1::function
#define UtilBind           std::tr1::bind
#define UtilAutoPtr        std::auto_ptr
#define UtilSharedPtr      std::tr1::shared_ptr
using namespace std::tr1::placeholders;
#endif

#ifndef NULL
#define NULL  0
#endif
#define DISALLOW_COPY_AND_ASSIGN(TypeName) TypeName(const TypeName&); TypeName& operator=(const TypeName&);
#define SINGLETON(TypeName) static TypeName& getInstance() { static TypeName s_instance; return s_instance; }

//base classes & functions
#include "constants.hpp"
#include "memory.hpp"
#include "in.hpp"
#include "out.hpp"
#include "baseclass.hpp"
#include "string.hpp"
#include "time.hpp"

#endif // UTIL_BASE_BASE_HPP
