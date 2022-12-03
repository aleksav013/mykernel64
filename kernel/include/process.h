#ifndef PROCESS_H
#define PROCESS_H

#include <registers.h>
#include <libk/list.h>

#define STATUS_READY 0
#define STATUS_RUNNING 1
#define STATUS_WAITING 2

struct process_t {
	registers_t registers;
	list_t* next;
	uint32_t status;
	uint32_t pid;
	uint32_t time_using_cpu;
};
typedef struct process_t process_t;

extern process_t process_list;
extern process_t current_process;

void create_process(uint64_t rip, uint64_t param1, uint64_t param2);

#endif
