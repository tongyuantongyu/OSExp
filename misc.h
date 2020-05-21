//
// Created by TYTY on 2020-05-04 004.
//

#ifndef OSEXP__MISC_H_
#define OSEXP__MISC_H_

#include <bit>

#if __GNUC__ >= 3
# define UNLIKELY(cond) __builtin_expect(!!(cond), 0)
# define LIKELY(cond) __builtin_expect(!!(cond), 1)
#else
# define UNLIKELY(cond) (cond)
# define LIKELY(cond) (cond)
#endif

#ifdef _MSC_VER
#include <type_traits>
typedef std::make_signed<size_t>::type ssize_t;
#endif

#if __cpp_lib_int_pow2 >= 202002L
  #define b_floor std::bit_floor
#else
  #ifdef __cpp_lib_int_pow2
    #define b_floor std::floor2
  #elif _LIBCPP_STD_VER > 17
    #define b_floor std::floor2
  #endif
#endif

#endif //OSEXP__MISC_H_
