/** @file  thr_create.c
 *  @brief The file contains the
 *         implementation of thr_create function
 *
 *  @author  Yuhang Jiang (yuhangj)
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs No known bugs
 */

#include <syscall_int.h>

#include "thread.h"
#include "simics.h"
#include "variable_queue.h"
#include "thr_internals.h"

/** @brief Function to create a new thread
 *  
 *  Creates a TCB and a stack for the new thread. Calls the fork_thread 
 *  and inserts the TCB into the thread list. Updates the stack top to 
 *  account for this thread creation
 *
 *  @param *func(void*) the function to execute with the new thread
 *  @param *args argument to pass to function
 *  @return the thread id of the newly created thread, -1 on failure
 */

int thr_create( void *(*func)(void *), void *args )
{
    /* lock global mutex */
    mutex_lock(&thread_list);
    lprintf ("starting cur_stack_base_addr: %x",(int)cur_stack_base_addr);
    if (new_pages((void*)cur_stack_base_addr - thread_stack_size, 
        thread_stack_size) < 0)
    {
	mutex_unlock(&thread_list);
        lprintf("PAGE ALLOCATION FAILED");
        return -1;
    } 
  
    /* Construct the thread  curr_tcb */
    tcb_t *curr_tcb = construct_tcb();

    /* Update the tcb with the parent tid*/
    curr_tcb->parentid = gettid();
    int tid = thread_fork(cur_stack_base_addr, func, args, curr_tcb);
    if (tid < 0) {
	mutex_unlock(&thread_list);
        return tid;
    }
    curr_tcb->tid = tid;

    mutex_init(&curr_tcb->join_exit_mtx);
    cond_init(&curr_tcb->join_cond);

    /* Inserting thread tcb to the thread list */
    Q_INSERT_TAIL(&tcb_list, curr_tcb, tcb_link);

    curr_tcb->state = RUNNABLE;
    
    /* Update the stack top */
    cur_stack_base_addr = cur_stack_base_addr - thread_stack_size;
    lprintf (" cur_stack_base_addr is %x", (int) cur_stack_base_addr);
    /* unlock global mutex */
    mutex_unlock(&thread_list);
    return tid;
}

/** @brief Function to create a new thread
 *  
 *  Register page fault handler and execute the thread function
 *
 *  @param *func(void*) the function to execute with the new thread
 *  @param *arg argument to pass to function
 *  @param curr_tcb the child thread tcb
 *  @return the thread id of the newly created thread, -1 on failure
 */
void fork_child_handler(void *(*func) (void *), void *arg, tcb_t *curr_tcb)
{
    void  *status;
    //lprintf("argument address is %u",(int)arg);
    lprintf(" parent id is %d",curr_tcb->parentid);
   
    /* Get the tid */
    curr_tcb->tid = gettid();
    lprintf ("child tid = %d", curr_tcb->tid);
   
    while(curr_tcb->state == BLOCKED){
        lprintf("Suspend child until parent make it RUNNABLE!!!");
        yield(-1);
    }
    /* execute the thread function*/
    status = (func)(arg); 

    /* install page fault handler */
    swexn((void *)(curr_tcb->stack_start_addr - thread_stack_size),
          page_fault_handler, NULL, NULL);
    thr_exit((void*)status);
}

/** @brief Function to yield execution to the thread with the passed tid
 *
 *  @param tid the thread id of the thread that is 
 *             yielded to 
 *  @return 1 on success, negative on failure 
 *
 */
int thr_yield( int tid )
{
    return yield(tid);
}

/** @brief Function to obtain thread id
 *  
 *  @param void
 *  @return threadid
 */
int thr_getid(void)
{
    return gettid();
}
