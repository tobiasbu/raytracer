
#ifndef _GGE_CONFIG_HPP_
#define _GGE_CONFIG_HPP_

#if defined(_WIN32) || defined(_WIN64)
#define GGE_WINDOWS
#if defined(__MINGW32__)  // Defined for both 32 bit/64 bit MinGW
#define GGE_MINGW
#elif defined(_MSC_VER)
#define GGE_MSVC
#endif
#endif

#if defined(_MSC_VER) && _MSC_VER == 1900
#define GGE_MSVC2015
#endif

#if defined(_MSC_VER) && _MSC_VER == 1800
#define GGE_MSVC2013
#endif

#if defined(GGE_MSVC)
#define GGE_LOCAL_THREAD __declspec(thread)
#else
#define GGE_LOCAL_THREAD __thread
#endif

#ifdef GGE_MSVC2013
#define GGE_CONSTEXPR const
#else
#define GGE_CONSTEXPR constexpr
#endif

#if defined(GGE_MSVC)
#define GGE_FORCEINLINE __forceinline
#else
#define GGE_FORCEINLINE __attribute__((always_inline)) inline
#endif

#endif