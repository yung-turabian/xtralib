/**
 * @file reis.h
 * @author yung-turabian
 * @date 4 8 2024
 * @brief Shared typedefs and functions.
 */
#ifndef __REISLIB_COMMMON_H__
#define __REISLIB_COMMMON_H__

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
typedef float   f32;
typedef double  f64;

// Ensure no conflicts if bool is already defined
#if !defined(__cplusplus)
		#if !defined(bool) 
				typedef enum __attribute__((__packed__)) {false, true} bool;
		#endif
#endif



// utility macro for `defer(1)`
#define fnname(name) #name

typedef void (*deferred_function)(void);

/**
 * @brief An atexit() wrapper function
 * @return bool
 */
static inline int defer(deferred_function func)
{
		int ret;
		if((ret = atexit(func)) == 0)
				return ret;

		return ret;
}

#endif 
