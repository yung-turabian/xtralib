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

// Ensure no conflicts if bool is already defined
#ifndef bool
		typedef enum {true, false} bool;
#else
		typedef enum {xtra_true, xtra_false} xtra_bool;
#endif

// utility macro for `defer(1)`
#define fnname(name) #name

// A macro alt for a for-loop, pass simply start and end and use `it` as
// the iterator
#define forx(start, end) for(int it = start; it < end; it++)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;



typedef void (*deferred_function)(void);

/**
 * @brief A safer atexit() function
 */
void
defer(deferred_function func)
{
		if(atexit(func) != 0) {
				fprintf(stderr, "Function: %s; failed to defer\n", fnname(atexit));
				exit(EXIT_FAILURE);
		}
}



#endif 
