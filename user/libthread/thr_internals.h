/** @file thr_internals.h
 *
 *  @brief This file may be used to define things
 *         internal to the thread library.
 */

#ifndef THR_INTERNALS_H
#define THR_INTERNALS_H

#include <stdint.h>
#include <stdio.h>
#include <simics.h>
#include <thr_macros.h>
#include <syscall.h>
#include <list.h>

#define THREAD_INIT_SUCC  0
#define THREAD_INIT_FAIL  1

#define PAGE_ALIGN(addr) ((addr) & ~ (PAGE_SIZE -1))

#define THR_SIZE_ALIGN(size) \
    ( ( (unsigned int)(size) + (PAGE_SIZE-1) ) & ~(PAGE_SIZE-1))

#define THR_PTR_ALIGN(ptr) (((uintptr_t) (ptr)) & ~(PAGE_SIZE-1) )

#define SET_STACK_CURR_BASE(addr) (cur_stack_base_addr = (uintptr_t) (addr) )

#define GET_STACK_CURR_BASE(addr) (cur_stack_base_addr)

#define MULTI_THREADING_ENABLE()  ( multi_threading = 1 ;)  
#define MULTI_THREADING_DISABLE()  ( multi_threading = 0 ;)  

#define IS_MULTI_THREADING_ENABLED() ( multi_threading == 1;)

int main_thread_id;
uintptr_t cur_stack_base_addr;
list_head *head_thr_list;
uintptr_t main_thr_high_addr;
uintptr_t main_thr_low_addr;
int multi_threading = 0;
int thread_stack_size;

typedef enum
{
	INIT = 0,
	RUNNING,
	RUNNABLE,
	SLEEP,
	ZOMBIE
} exec_state_t;

typedef enum
{
	THR_ERROR = -1,
	THR_SUCCESS,
	THR_ENOMEM,
	THR_FORK_ERROR
} thr_err_t;

typedef struct _tcb 
{
	int tid;
	int parentid;
	list_head list;
	void *exit_status;
	uintptr_t stack_start_addr;
	exec_state_t state;
	int main_thread;   
}tcb_t;


tcb_t *head;

int thread_init(unsigned int size);

#endif /* THR_INTERNALS_H */
