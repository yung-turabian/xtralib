/**
 * @file xtra.h
 * @author yung-turabian
 * @date 4 8 2024
 * @brief General functions.
 *
 */

#ifndef XTRA_H
#define XTRA_H

#include <stdlib.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

// Ensure no conflicts if bool is already defined
#if !defined(__cplusplus)
		#if !defined(bool) 
				typedef enum {false, true} bool;
		#else
				typedef enum {xfalse, xtrue} xtra_bool;
		#endif
#endif

// utility macro for `defer(1)`
#define fnname(name) #name

// A macro alt for a for-loop, pass simply start and end and use `it` as
// the iterator
#define FOR(start, end) for(u64 it = (u64)(start); it < (u64)(end); it++)





typedef void (*deferred_function)(void);

/**
 * @brief A safer atexit() function, borrows the returns
 * @return 
 */
static inline int
defer(deferred_function func)
{
		int ret;
		if((ret = atexit(func)) == 0)
				return ret;

		return ret;
}



#endif 
