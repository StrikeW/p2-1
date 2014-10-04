/** @file  thr_internals.h
 *  @brief Thread internal data structures and internal functions
 *
 *  @author  Yuhang Jiang (yuhangj)
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs No known bugs
 */

#ifndef THR_INTERNALS_H
#define THR_INTERNALS_H

#include <stdint.h>
#include <stdio.h>
#include <simics.h>
#include <syscall.h>
#include <stdlib.h>
#include <thr_macros.h>
 
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
    EXIT_SUCC
} exec_state_t;

/* thread create return */
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
    int join_tid;
    void *exit_status;

    exec_state_t state;
    uintptr_t stack_start_addr;

    cond_t join_cond;
    mutex_t join_exit_mtx;
    Q_NEW_LINK(_tcb) tcb_link;
}tcb_t;

Q_NEW_HEAD(_tcb_list, _tcb);

_tcb_list tcb_list;

tcb_t* construct_tcb();
void set_cur_stack_base_addr( void *stack_low );
void store_free_stack_addr( uintptr_t addr );
int thread_fork( uintptr_t stack_base, void *(*func) (void *), void *arg, tcb_t *curr_tcb);
uintptr_t ret_free_stack_addr();

#endif /* THR_INTERNALS_H */
