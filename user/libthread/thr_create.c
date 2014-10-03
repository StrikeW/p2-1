/** @file  thr_create.c
 *
 *  @brief 
 *
 *  @author  Yuhang Jiang (yuhangj)
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs
 *
 */
#include "thread.h"
#include <syscall_int.h>
#include "simics.h"
#include "variable_queue.h"
#include "thr_internals.h"

#include "simics.h"

#define THR_JOIN_FAILED -1

//#define DEBUG_LIST

/** @brief Function to create a new thread
 *  
 *  Creates a TCB and a stack for the new thread. 
 *  Calls the fork_thread and inserts the
 *  TCB into the thread list.
 *  Updates the stack top to account for this
 *  thread creation
 *
 *  @param *func(void*) the function to execute with the new thread
 *  @param *args argument to pass to function
 *  @return the thread id of the newly created thread, -1 on failure
 */

int thr_create( void *(*func)(void *), void *args )
{
    /* lock global mutex */
    mutex_lock(&thread_list);
    lprintf (" starting cur_stack_base_addr = %x",(int)cur_stack_base_addr);
    if (new_pages((void*)cur_stack_base_addr - thread_stack_size, thread_stack_size) < 0)
    {
        lprintf("PAGE ALLOCATION FAILED");
        return -1;
    } 
  
    /* Construct the thread  curr_tcb */
    tcb_t *curr_tcb = construct_tcb();

    /* Update the tcb with the parent tid*/
    curr_tcb->parentid = gettid();
    mutex_init(&curr_tcb->join_exit_mtx);
    cond_init(&curr_tcb->join_cond);
    int tid = thread_fork(cur_stack_base_addr, func, args, curr_tcb);
    lprintf ("DEBUG:  after thread fork");
    if (tid < 0) {
        return tid;
    }
    curr_tcb->tid = tid;

    /* TODO: move it to thread function  */
    
    /* Inserting thread tcb to the thread list */
    Q_INSERT_TAIL(&tcb_list, curr_tcb, tcb_link);

    /* TODO: Remove from DEBUG_LIST  */
#ifdef DEBUG_LIST
    tcb_t *tmp_tcb = NULL;
    tmp_tcb = Q_GET_FRONT(&tcb_list);
    while(tmp_tcb){
        if(tmp_tcb == Q_GET_FRONT(&tcb_list)){
            lprintf("Start:tid[%d]addr[%p]->", tmp_tcb->tid, tmp_tcb);
        }
        else{
            lprintf("->tid[%d]addr[%p]", tmp_tcb->tid, tmp_tcb);
        }
        tmp_tcb = Q_GET_NEXT(tmp_tcb, tcb_link);
    }

#endif

    curr_tcb->state = RUNNABLE;
    lprintf ("In Parent, child tid is %d", tid);
    //insert_rear(head_thr_list, curr_tcb->list);	
    
    /* Update the stack top */
    cur_stack_base_addr = cur_stack_base_addr - thread_stack_size;
    lprintf (" cur_stack_base_addr is %x", (int) cur_stack_base_addr);
    /* unlock global mutex */
    mutex_unlock(&thread_list);
    lprintf ("DEBUG:  thr_create end");
    return tid;
}

/** @brief Function to create a new thread
 *  
 *  Register page fault handler and 
 *  execute the thread function
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
   
    //mutex_init(&curr_tcb->join_exit_mtx);
    //cond_init(&curr_tcb->join_cond);
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

/** @brief Function to join the thread
 *  
 *  Find the thread to be joined and
 *  wait for it to exit and return 
 *  exit status.
 *
 *  @param tid thread id of the joining thread
 *  @param exit status of the joining thread
 *  @return THR_JOIN_SUCC on success and THR_JOIN_FAIL 
 *          on failure 
 *
 */
int thr_join( int tid, void **statusp ){

    int self_tid = thr_getid();

    lprintf("In thr_join, now thr[%d], join_tid[%d]", self_tid, tid);

    /* Cannot allow to join itself */
    if(tid == self_tid){
        lprintf("ERROR: cannot join itself!");
        return THR_JOIN_FAILED;
    }

    tcb_t *tmp_tcb = NULL;
    
    /* Search for the target thread */
    tmp_tcb = Q_GET_FRONT(&tcb_list);

    while(tmp_tcb){
        if(tmp_tcb->tid == tid){
            lprintf("Got the target tid[%d] addr[%p]", tid, tmp_tcb);
            break;
        }
        tmp_tcb = Q_GET_NEXT(tmp_tcb, tcb_link);
    }

    /* used for prevent status change(thr_exit) */
    mutex_lock(&tmp_tcb->join_exit_mtx);

    /* If we cannot find that join tid */
    if(tmp_tcb == NULL){
        lprintf("Cannot found the target_tcb!");
        mutex_unlock(&tmp_tcb->join_exit_mtx);
        return THR_JOIN_FAIL;
    }

    /* If threaded already joined */
    if(tmp_tcb->join_tid == DEFAULT_JOIN_TID){
        tmp_tcb->join_tid = self_tid;
    }
    else{
        lprintf("ERROR: Already joined thread!");
        mutex_unlock(&tmp_tcb->join_exit_mtx);
        return THR_JOIN_FAIL;
    }

/* If find a live thread, wait */
//!!! cannnot use tmp_tcb->state != DEAD here, maybe zombine 
// Maybe right now, such thread is suspended(Blocked)by the parent
// and waiting for being activated by parent thread.
    if(tmp_tcb->state == BLOCKED){
        lprintf("The joined thread hasn't be activated by parent now!");
        mutex_unlock(&tmp_tcb->join_exit_mtx);
        return THR_JOIN_FAIL;
    }

    if(tmp_tcb->state == EXIT_SUCC){
        lprintf("Joined thread already exit!! Won't wait anymore ");
        mutex_unlock(&tmp_tcb->join_exit_mtx);
        return THR_JOIN_SUCC;
    }

    if(tmp_tcb->state == RUNNABLE){
        lprintf("Your Joined thread is still running, waiting now!");
        cond_wait(&tmp_tcb->join_cond, &tmp_tcb->join_exit_mtx);
    }

    if(statusp != NULL){
        *statusp = tmp_tcb->exit_status;
    }
    mutex_unlock(&tmp_tcb->join_exit_mtx);


    return THR_JOIN_SUCC;
}

/** @brief Function for thread exit
 *  
 *  Remove tcb and destroy mutex and 
 *  release thread.
 *
 *  @param exit status of the terminating thread
 *  @return void 
 *
 */
void thr_exit( void *status )
{
    int tid = gettid();

    tcb_t *exit_thr = NULL;
    
    /* Search for the target thread */
    exit_thr = Q_GET_FRONT(&tcb_list);

    while(exit_thr){
        if(exit_thr->tid == tid){
            lprintf("Got the exit_tid[%d] addr[%p]", tid, exit_thr);
            break;
        }
        exit_thr = Q_GET_NEXT(exit_thr, tcb_link);
    }

    /* If we cannot find that tid, the fucking process called 
     * thr_exit */
    if(exit_thr == NULL){
        lprintf("Cannot found the target!");
        vanish();
    }
    lprintf("Thread %d exit", tid);
    mutex_lock(&exit_thr->join_exit_mtx);
    
    /* Set the thread as dead */
    exit_thr->state = EXIT_SUCC;
    exit_thr->exit_status = status;
    lprintf("exit_thr->join_tid:%d",exit_thr->join_tid);

    /* Shit! Cannot singal if join_cond == DEFAULT_JOIN_TID */
    if(exit_thr->join_tid != DEFAULT_JOIN_TID){
        lprintf("Need singal!!!!!!!!!");
        cond_signal(&exit_thr->join_cond);
    }

    mutex_unlock(&exit_thr->join_exit_mtx);

    vanish();
}

/** @brief Function to yield execution to the
 *         thread with the passed tid
 *  
 *
 *  @param tid the thread id of the thread that is 
 *             yielded to 
 *  @return 1 on success, negative on failure 
 *
 */
int thr_yield( int tid ){
    return yield(tid);
}



/** @brief Function to obtain thread id
 *  
 *
 *  @param void
 *  @return threadid
 *
 */
int thr_getid(void){
    return gettid();
}
