/** @file  thr_internals.h
 *  @brief Thread internal data structures
 *         and internal functions
 *
 *  @author  Yuhang Jiang (yuhangj)
 *  @author  Subramanian Natarajan (snatara1)
 *
 *
 *  @bugs
 *
 */

#ifndef THR_INTERNALS_H
#define THR_INTERNALS_H

#include <stdint.h>
#include <stdio.h>
#include <simics.h>
#include <thr_macros.h>
#include <syscall.h>
#include <_list.h>
#include <stdlib.h>
#include "mutex_type.h"
#include "cond_type.h"
#include "variable_queue.h"

/* global variables */
uintptr_t cur_stack_base_addr;
uintptr_t main_thr_high_addr;
uintptr_t main_thr_low_addr;

int multi_threading;
int thread_stack_size;
int parent;

mutex_t thread_list;

/* thread execution states */
typedef enum
{
    INIT = 0,
    RUNNABLE,
    BLOCKED,
    ZOMBIE,
    EXIT_SUCC
} exec_state_t;


typedef struct stack_free_list_addr
{
    int size;
    struct free_stack_addr *link;
} stack_free_list_addr_t;


typedef struct free_stack_addr 
{
    struct free_stack_addr *next;
    struct free_stack_addr *prev;
    uintptr_t free_addr;
}free_stack_addr_t;

stack_free_list_addr_t header;


/* thread data structure */
typedef struct _tcb 
{
    int tid;
    int parentid;
    int join_tid;
    void *exit_status;

    uintptr_t stack_start_addr;
    exec_state_t state;

    cond_t join_cond;
    mutex_t join_exit_mtx;
    Q_NEW_LINK(_tcb) tcb_link;
}tcb_t;

Q_NEW_HEAD(_tcb_list, _tcb);

_tcb_list tcb_list;

/* thread create return */
typedef enum
{
    THR_ERROR = -1,
    THR_SUCCESS,
    THR_ENOMEM,
    THR_FORK_ERROR
} thr_err_t;

#define DEFAULT_TID -1
#define DEFAULT_JOIN_TID -2

#define THR_JOIN_FAIL -1
#define THR_JOIN_SUCC 0

#define PAGE_ALIGN(addr) ((addr) & ~ (PAGE_SIZE -1))

#define THR_SIZE_ALIGN(size) \
    ( ( (unsigned int)(size) + (PAGE_SIZE-1) ) & ~(PAGE_SIZE-1))

#define THR_PTR_ALIGN(ptr) (((uintptr_t) (ptr)) & ~(PAGE_SIZE-1) )

#define SET_STACK_CURR_BASE(addr) (cur_stack_base_addr = (uintptr_t) (addr) )

#define GET_STACK_CURR_BASE(addr) (cur_stack_base_addr)

#define MULTI_THREADING_ENABLE()  ( multi_threading = 1 )  
#define MULTI_THREADING_DISABLE()  ( multi_threading = 0 )  
#define IS_MULTI_THREADING_ENABLED() ( multi_threading == 1)

void set_cur_stack_base_addr( void *stack_low);

tcb_t* construct_tcb();
uintptr_t ret_free_stack_addr();
void store_free_stack_addr(uintptr_t addr);
int thread_fork(uintptr_t stack_base, void *(*func) (void *), void *arg, tcb_t *curr_tcb);

#endif /* THR_INTERNALS_H */
