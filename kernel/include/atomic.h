#ifndef ATOMIC
#define ATOMIC

#include <types.h>

struct mutex_t {
	volatile uint64_t* addr;
};
typedef struct mutex_t mutex_t;


void init_mutex(mutex_t* mutex);
bool test_and_set(mutex_t mutex, bool value);
void lock(mutex_t mutex);
void unlock(mutex_t mutex);

#endif
