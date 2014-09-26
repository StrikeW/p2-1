#include"thr_global_data.h"
 
   

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
   lprintf( "DEBUG: construct_tcb start");
   tcb_t *new_thr = (tcb_t *) malloc( sizeof (tcb_t));
   
   new_thr->exit_status = NULL;
   new_thr->stack_start_addr = cur_stack_base_addr;
   new_thr->list = (list_head*) malloc (sizeof ( list_head));
   INIT(new_thr->list);
   //LIST_HEAD(new_thr.list);
   lprintf( "DEBUG: construct_tcb end");
   
   return new_thr;
}

int get_tid(tcb_t *thr)
{
   lprintf( "DEBUG: get_tid start");
   return 1;
   lprintf( "DEBUG: get_tid end");
}

int isStackSpaceAvailable()
{
   lprintf( "DEBUG: sStackSpaceAvailable start");
   return 1;
   lprintf( "DEBUG: sStackSpaceAvailable end");
}




   


