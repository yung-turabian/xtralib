#ifndef __REISLIB_RAND_H__
#define __REISLIB_RAND_H__

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define REISLIB_INT_MAX 2147483647

#define CHARSET "abcdefghijklmnopqrstuvwxyz"
#define CHARSET_LEN 26

typedef float       f32;
typedef double      f64;
typedef long double f128;


/**
 * @brief The random seed generator for most functions.
 * A pseudo-random generator, older solution, a Linear Congruential Generator
 * @warning haven't tested on windows. And it needs to be seeded than gen.
 */
static inline size_t 
rng(size_t lb, size_t ub) {
#if defined(__linux__) || defined(__APPLE__)
		struct timespec spec;
		timespec_get(&spec, TIME_UTC);
		long long ns;
		ns = spec.tv_nsec; //fractional
		int seed = lb + ((22695477 * ns) % 4294967296) % (ub - lb + 1); 
		return seed;
#else
		return lb + ub * -1;
#endif
}

static inline size_t
unix_rng(size_t lb, size_t ub)
{
		int fd = open("/dev/urandom", O_RDONLY);
		if(fd < 0) {
				perror("open");
				return -1;
		}

		unsigned int random;
		ssize_t result = read(fd, &random, sizeof(random));
		close(fd);

		if(result != sizeof(random)) {
				perror("read");
				return -1;
		}
		return lb + (random % (ub - lb + 1));
}

/**
 * @brief Uses the defined INT_MAX instead of passing bounds
 */
static inline int 
randi() {
		return (int)rng(0, REISLIB_INT_MAX-1);
}

static inline int 
randib(int lb, int ub) {
		return (int)rng(lb, ub);
}

// generate random char
static inline char 
randc() {
	return CHARSET[rng(0, CHARSET_LEN - 1)];
}

// generate random string of desired length, MUST FREE STR!
static inline char* 
rands(size_t len) {
	char *str = (char*)malloc(len + 1);
		
	size_t i;
	for(i=0;i<len;i++) {
		str[i] = randc();
	}
	str[len+1] = '\0';

	return str;
}

#endif
