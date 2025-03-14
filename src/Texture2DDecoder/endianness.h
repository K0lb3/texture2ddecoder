/*
 *
 * License Information
 *
 * endianness.h is derived from https://gist.github.com/jtbr/7a43e6281e6cca353b33ee501421860c
 * The file is licensed under the MIT License shown below.
 *
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _ENDIANNESS_H
#define _ENDIANNESS_H

#include <stdlib.h>
#include <stdint.h>

/* Detect platform endianness at compile time */

// Cross-platform header discovery using __has_include
#ifdef __has_include
#  if __has_include(<endian.h>)
#    include <endian.h>          // GLIBC (Linux), BSDs
#  elif __has_include(<sys/endian.h>)
#    include <sys/endian.h>      // OpenBSD, newer macOS
#  elif __has_include(<machine/endian.h>)
#    include <machine/endian.h>  // Older macOS, BSD variants
#  elif __has_include(<sys/param.h>)
#    include <sys/param.h>       // MinGW, some BSDs
#  elif __has_include(<sys/isadefs.h>)
#    include <sys/isadefs.h>     // Solaris
#  endif
#endif

// Comprehensive endianness detection
#if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)

/* Big Endian Detection */
#if (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) || \
    (defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN) ||             \
    (defined(_BYTE_ORDER) && _BYTE_ORDER == _BIG_ENDIAN) ||                \
    (defined(BYTE_ORDER) && BYTE_ORDER == BIG_ENDIAN) ||                   \
    (defined(__sun) && defined(__SVR4) && defined(_BIG_ENDIAN)) ||         \
    defined(__ARMEB__) || defined(__THUMBEB__) ||                          \
    defined(__AARCH64EB__) ||                                              \
    defined(__MIPSEB__) || defined(__MIPSEB) || defined(_MIPSEB) ||        \
    defined(__PPC__) || defined(__PPC64__) ||                              \
    defined(__sparc__) || defined(__sparc) ||                              \
    defined(__s390__) || defined(__s390x__) ||                             \
    defined(__hppa__) || defined(__HPPA__) ||                              \
    defined(_M_PPC) || defined(_ARCH_PPC) ||                               \
    defined(__BIG_ENDIAN__) ||                                            \
    defined(__TCS__) ||                                                    \
    defined(_AIX) || defined(__TOS_AIX__) ||                               \
    defined(__m68k__) || defined(__MC68K__) ||                             \
    defined(__ICL)                                                         
#define __BIG_ENDIAN__

/* Little Endian Detection */
#elif (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || \
      (defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN) ||             \
      (defined(_BYTE_ORDER) && _BYTE_ORDER == _LITTLE_ENDIAN) ||                \
      (defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN) ||                   \
      (defined(__sun) && defined(__SVR4) && defined(_LITTLE_ENDIAN)) ||         \
      defined(__ARMEL__) || defined(__THUMBEL__) ||                             \
      defined(__AARCH64EL__) ||                                                 \
      defined(__MIPSEL__) || defined(__MIPSEL) || defined(_MIPSEL) ||           \
      defined(__i386__) || defined(__x86_64__) ||                               \
      defined(__ia64__) || defined(__IA64__) ||                                 \
      defined(_M_IX86) || defined(_M_X64) || defined(_M_IA64) ||                \
      defined(_M_ARM) || defined(_M_ARM64) ||                                   \
      defined(__alpha__) ||                                                     \
      defined(__riscv) ||                                                       \
      defined(__LITTLE_ENDIAN__) ||                                             \
      defined(__amd64__) || defined(__amd64) ||                                 \
      defined(__e2k__) ||                                                       \
      defined(__MICROBLAZE__) ||                                                \
      defined(__ARCEL__) ||                                                     \
      defined(_WIN32) || defined(_WIN64)                                        
#define __LITTLE_ENDIAN__

/* Universal Fallback */
#else
#  warning "Cannot detect endianness! Assuming little-endian."
#  define __LITTLE_ENDIAN__  // Most modern systems are little-endian
#endif

#endif // !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)

#ifdef bswap16
#undef bswap16
#endif
#ifdef bswap32
#undef bswap32
#endif
#ifdef bswap64
#undef bswap64
#endif

/* Define byte-swap functions, using fast processor-native built-ins where possible */
// needs to be first because msvc doesn't short-circuit after failing defined(__has_builtin)
#if defined(_MSC_VER)
#define bswap16(x) _byteswap_ushort((x))
#define bswap32(x) _byteswap_ulong((x))
#define bswap64(x) _byteswap_uint64((x))
#elif (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
#define bswap16(x) __builtin_bswap16((x))
#define bswap32(x) __builtin_bswap32((x))
#define bswap64(x) __builtin_bswap64((x))
#elif defined(__has_builtin) && __has_builtin(__builtin_bswap64)
/* for clang; gcc 5 fails on this and && shortcircuit fails; must be after GCC check */
#define bswap16(x) __builtin_bswap16((x))
#define bswap32(x) __builtin_bswap32((x))
#define bswap64(x) __builtin_bswap64((x))
#else
/* even in this case, compilers often optimize by using native instructions */
static inline uint16_t bswap16(uint16_t x) {
    return (((x >> 8) & 0xffu) | ((x & 0xffu) << 8));
}
static inline uint32_t bswap32(uint32_t x) {
    return (((x & 0xff000000u) >> 24) | ((x & 0x00ff0000u) >> 8) | ((x & 0x0000ff00u) << 8) |
            ((x & 0x000000ffu) << 24));
}
static inline uint64_t bswap64(uint64_t x) {
    return (((x & 0xff00000000000000ull) >> 56) | ((x & 0x00ff000000000000ull) >> 40) |
            ((x & 0x0000ff0000000000ull) >> 24) | ((x & 0x000000ff00000000ull) >> 8) |
            ((x & 0x00000000ff000000ull) << 8) | ((x & 0x0000000000ff0000ull) << 24) |
            ((x & 0x000000000000ff00ull) << 40) | ((x & 0x00000000000000ffull) << 56));
}
#endif


/* Defines network - host byte swaps as needed depending upon platform endianness */
// note that network order is big endian)

#if defined(__LITTLE_ENDIAN__)
#define ntoh16(x) bswap16((x))
#define hton16(x) bswap16((x))
#define ntoh32(x) bswap32((x))
#define hton32(x) bswap32((x))
#define ntoh64(x) bswap64((x))
#define hton64(x) bswap64((x))
#define lton16(x) (x)
#define lton32(x) (x)
#define lton64(x) (x)
#define ltonf(x) (x)
#define ltond(x) (x)
#define bton16(x) bswap16((x))
#define bton32(x) bswap32((x))
#define bton64(x) bswap64((x))
#define btonf(x) htonf((x))
#define btond(x) htond((x))
#elif defined(__BIG_ENDIAN__)
#define ntoh16(x) (x)
#define hton16(x) (x)
#define ntoh32(x) (x)
#define hton32(x) (x)
#define ntoh64(x) (x)
#define hton64(x) (x)
#define bton16(x) (x)
#define bton32(x) (x)
#define bton64(x) (x)
#define btonf(x) (x)
#define btond(x) (x)
#define lton16(x) bswap16((x))
#define lton32(x) bswap32((x))
#define lton64(x) bswap64((x))
#define ltonf(x) htonf((x))
#define ltond(x) htond((x))
#else
#warning "UNKNOWN Platform / endianness; network / host byte swaps not defined."
#endif


//! Convert 32-bit float from host to network byte order
static inline float htonf(float f) {
#ifdef __cplusplus
    static_assert(sizeof(float) == sizeof(uint32_t), "Unexpected float format");
    uint32_t val = hton32(*(reinterpret_cast<const uint32_t *>(&f)));
    return *(reinterpret_cast<float *>(&val));
#else
    uint32_t val = hton32(*(const uint32_t *)(&f));
    return *((float *)(&val));
#endif
}
#define ntohf(x) htonf((x))

//! Convert 64-bit double from host to network byte order
static inline double htond(double f) {
#ifdef __cplusplus
    static_assert(sizeof(double) == sizeof(uint64_t), "Unexpected double format");
    uint64_t val = hton64(*(reinterpret_cast<const uint64_t *>(&f)));
    return *(reinterpret_cast<double *>(&val));
#else
    uint64_t val = hton64(*(const uint64_t *)(&f));
    return *((double *)(&val));
#endif
}
#define ntohd(x) htond((x))

#endif  //_ENDIANNESS_H
