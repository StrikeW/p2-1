/** @file  mutex.c
 *  @brief 
 *  @author  Yuhang Jiang (yuhangj) 
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs
 *
 */


//#include "thr_global_data.h"
#include "variable_queue.h"
#include "thr_internals.h"
   


tcb_t* construct_tcb()
{ 
   tcb_t *new_thr = (tcb_t *) malloc(sizeof(tcb_t));
   new_thr->state = BLOCKED;
   new_thr->join_tid = DEFAULT_JOIN_TID;
   Q_INIT_ELEM(new_thr, tcb_link);

   new_thr->exit_status = NULL;
   new_thr->stack_start_addr = cur_stack_base_addr;

   return new_thr;
}




   


