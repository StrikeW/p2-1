/** @file  thr_init.c
 *  @brief Thread initialisation
 *
 *  @author  Yuhang Jiang (yuhangj)
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs No known bugs
 */

#include "thread.h"
#include "variable_queue.h"
#include "thr_internals.h"

/** @brief Function to initialise the thread library
 *  
 *  This function intialises the stack size for every thread as 
 *  provided by the user. It also allocates tcb for the main thread 
 *  and initialises the thread list. 
 * 
 *  @param size the user provided stack size
 *  @return 0 on success, -1 on failure
 */
int thr_init( unsigned int size )
{
    thread_stack_size = THR_SIZE_ALIGN(size);
    lprintf ("DEBUG:  thr_init start");

    tcb_t *curr_tcb = construct_tcb();

    curr_tcb->stack_start_addr = main_thr_high_addr;
    curr_tcb->parentid = 0;
    curr_tcb->state = RUNNABLE;
    curr_tcb->join_tid = DEFAULT_JOIN_TID;
    
    curr_tcb->tid = gettid();

    Q_INIT_HEAD(&tcb_list);
    
    Q_INSERT_TAIL(&tcb_list, curr_tcb, tcb_link);
    MULTI_THREADING_ENABLE();
    lprintf ("DEBUG:  thr_init end");
    mutex_init(&thread_list);
    return 0;
}






