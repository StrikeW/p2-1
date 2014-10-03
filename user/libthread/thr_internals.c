/** @file  mutex.c
 *  @brief implementation of thread internal
 *         functions
 *
 *  @author  Yuhang Jiang (yuhangj) 
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs No known bugs
 *
 */


#include "variable_queue.h"
#include "thr_internals.h"
   



/** @brief  Function to construct tcb 
 *    
 *  A tcb is allocated for the thread with
 *  execution state as blocked.
 *  The TCB is intiatlised
 *  so that it can be inserted into
 *  a thread list. 
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



uintptr_t ret_free_stack_addr()
{
   
   assert(header.link == NULL);
   uintptr_t addr;
   free_stack_addr_t *temp = header.link->prev;
   header.link->prev = temp->prev;
   temp->prev->next = header.link;
   addr = temp->free_addr;
   free (temp);
   header.size-=1;
   return addr; 
}

void store_free_stack_addr(uintptr_t addr)
{
   free_stack_addr_t *temp = malloc(sizeof (free_stack_addr_t));
   temp->free_addr = 0;
   temp->next = temp;
   temp->prev = temp;
 
 
   if ( header.link == NULL)
   {
         header.link = temp;
         return;
   }
   temp->next = header.link;
   temp->prev = header.link->prev;
   header.link->prev = temp;
   (temp->prev)->next = temp;
   header.link = temp;
   header.size+=1;
}
   


