#ifndef fjx_utils_utils_h
#define fjx_utils_utils_h

#if defined(__STDC__)
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#endif

static inline void *fjx_identity_ptr(void *data) { return data; }

#define fjx_container_of(ptr, type, member) \
    ((type *)((char *)fjx_identity_ptr(ptr) - offsetof(type, member)))

#if defined(unreachable)
#elif defined(__GNUC__)
#define unreachable() (__builtin_unreachable())
#elif defined(_MSC_VER)
#define unreachable() (__assume(false))
#else
[[noreturn]] inline void unreachable_impl() {}
#define unreachable() (unreachable_impl())
#endif

#endif
