/** @file  thr_join_exit.c
 *
 *  @brief contains implmentation of thread
 *  join and thread  exit 
 *
 *  @author  Yuhang Jiang (yuhangj)
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs No known bugs
 *
 */

#include<thread.h>

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

    /* If we cannot find that tid, the main thread has called 
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


