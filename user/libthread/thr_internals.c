/** @file  mutex.c
 *  @brief Implementation of thread internal functions
 *
 *  @author  Yuhang Jiang (yuhangj) 
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs No known bugs
 */

#include "variable_queue.h"
#include "thr_internals.h"
   
/** @brief  Function to construct tcb 
 *    
 *  A tcb is allocated for the thread with execution state as 
 *  blocked. The TCB is intiatlised so that it can be inserted 
 *  into a thread list. 
 *    
 *  @param  none
 *  @return a tcb data structure
 */
tcb_t* construct_tcb()
{ 
   tcb_t *new_thr = (tcb_t *) malloc(sizeof(tcb_t));
   new_thr->state = BLOCKED;
   new_thr->join_tid = DEFAULT_JOIN_TID;
   new_thr->exit_status = NULL;
   Q_INIT_ELEM(new_thr, tcb_link);
   return new_thr;
}

   


