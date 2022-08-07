#include <types.h>

int64_t abs(int64_t val)
{
	if (val < 0) return -val;
	return val;
}
