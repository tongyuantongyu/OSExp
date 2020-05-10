//
// Created by TYTY on 2020-05-04 004.
//

#ifndef OSEXP__MISC_H_
#define OSEXP__MISC_H_

#if __GNUC__ >= 3
# define UNLIKELY(cond) __builtin_expect(!!(cond), 0)
# define LIKELY(cond) __builtin_expect(!!(cond), 1)
#else
# define UNLIKELY(cond) (cond)
# define LIKELY(cond) (cond)
#endif

#ifdef _MSC_VER
typedef std::make_signed<size_t>::type ssize_t;
#endif

#endif //OSEXP__MISC_H_
