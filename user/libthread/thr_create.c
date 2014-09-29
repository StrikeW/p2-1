#include "thread.h"
#include <syscall_int.h>

int thr_create( void *(*func)(void *), void *args )
{
    int status;
    //lprintf ("DEBUG:  thr_create start");
    if (new_pages((void*)cur_stack_base_addr - PAGE_SIZE, PAGE_SIZE) < 0)
    {
        lprintf("PAGE ALLOCATION FAILED");
        return -1;
    } 
  
    if (new_pages((void*)cur_stack_base_addr - 2*PAGE_SIZE, PAGE_SIZE) < 0)
    {
        lprintf("PAGE ALLOCATION for Exception handler FAILED");
        return -1;
    }
   
    int tid = thread_fork(cur_stack_base_addr, func, args);
    if (tid < 0) {
        return tid;
    }
    //lprintf ("In Parent");	    
    int ret = deschedule(&status);
    if ( ret < 0) {
        return ret;
    }
    cur_stack_base_addr = cur_stack_base_addr - 2*PAGE_SIZE;
    //lprintf (" cur_stack_base_addr is %x", (int) cur_stack_base_addr);
    //lprintf ("DEBUG:  thr_create stop");
    return tid;
}


void fork_handler(void *(*func) (void *), void *arg)
{
 	void  *status;
    //lprintf ("In thread, need to register handler");
    while(1) 
    {
        if (thr_yield(main_thread_id) != 0) {
	       break;
        }
    }
   
    tcb_t *curr_tcb = construct_tcb();
    curr_tcb->tid = gettid();
    //lprintf ("child tid = %x", curr_tcb->tid);
    curr_tcb->main_thread = 0;
    insert_rear(head_thr_list, curr_tcb->list);	

    curr_tcb->state = RUNNING;
    status = (func)(arg);
    //swexn((void *)(curr_tcb->stack_start_addr - thread_stack_size),
      //    page_fault_handler, NULL, NULL);
    make_runnable(main_thread_id);
    thr_exit((void*)status);
}
