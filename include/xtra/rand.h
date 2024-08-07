#include <time.h>
#include <stdlib.h>
#include <string.h>

#define INT_MAX 2147483647

#define CHARSET "abcdefghijklmnopqrstuvwxyz"
#define CHARSET_LEN 26

typedef float       f32;
typedef double      f64;
typedef long double f128;


/**
 * @brief The random seed generator for most functions.
 */
size_t 
rng(size_t lb, size_t ub) {
#if defined(__linux__) || defined(__APPLE__)
		struct timespec spec;
		timespec_get(&spec, TIME_UTC);
		long long ns;
		ns = spec.tv_nsec; //fractional
		int seed = lb + ((22695477 * ns) % 4294967296) % (ub - lb + 1); 
		return seed;
#endif
#ifdef __MINGW32
		return NULL;
#endif
}

/**
 * @brief Uses the defined INT_MAX instead of passing bounds
 */
int 
randi() {
		return (int)rng(0, INT_MAX-1);
}

int 
randib(int lb, int ub) {
		return (int)rng(lb, ub);
}

// generate random char
char 
randc() {
	return CHARSET[rng(0, CHARSET_LEN)];
}

// generate random string of desired length, MUST FREE STR!
char* 
rands(size_t len) {
	char *str = malloc(len + 1);
		
	size_t i;
	for(i=0;i<len;i++) {
		str[i] = randc();
	}
	str[len+1] = '\0';

	return str;
}
