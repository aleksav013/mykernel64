#ifndef MATH_H
#define MATH_H

#include <types.h>

#define min(a, b) ((a) > (b) ? (b) : (a))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define upper_div(a, b) \
	((((a) / (b)*b) < (a)) ? (((a) / (b)) + 1) : ((a) / (b)))

int64_t abs(int64_t val);

#endif
