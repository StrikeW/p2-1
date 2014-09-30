/** @file thr_internals.h
 *
 *  @brief This file may be used to define things
 *         internal to the thread library.
 */



#ifndef THR_INTERNALS_H
#define THR_INTERNALS_H

#include<thr_global_data.h>


void set_cur_stack_base_addr( void *stack_low);

//pcb_t* construct_pcb();

tcb_t* construct_tcb();

int get_tid(tcb_t *thr);

int isStackSpaceAvailable();


int thread_fork(uintptr_t stack_base, void *(*func) (void *), void *arg, tcb_t *curr_tcb);

#endif /* THR_INTERNALS_H */
