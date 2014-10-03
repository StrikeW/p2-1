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
   


