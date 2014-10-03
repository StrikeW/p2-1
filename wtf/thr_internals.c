//#include "thr_global_data.h"
#include "variable_queue.h"
#include "thr_internals.h"
   

/*
pcb_t* construct_pcb()
{
   lprintf ("DEBUG:  construct_pcb start");
   pcb_t *main_pcb = (pcb_t *) malloc(sizeof pcb_t);
   head = &main_pcb->main_tcb;
   head->link->next = head;
   head->link->prev = head;
   head->tid = 0;
   head->exit_status = NULL;
   head->stack_start_addr = THR_PTR_ALIGN(cur_stack_base_addr); 
   lprintf(" main_pcb = %x, head = %x", (int)main_pcb, (int) head); 
   lprintf(" head->link->next= %x, head_>link->prev = %x", (int)head->link->next, (int) head->link->prev ); 
   lprintf ("DEBUG:  construct_pcb end");
   return main_pcb;
}
 
*/

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




   


