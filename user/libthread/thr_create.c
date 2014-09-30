#include "thread.h"
#include <syscall_int.h>

int thr_create( void *(*func)(void *), void *args )
{
    //int reject = 0;
     mutex_lock(&thread_list);
    lprintf ("DEBUG:  thr_create start");
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
    tcb_t *curr_tcb = construct_tcb();
      
    curr_tcb->parentid = gettid();
    int tid = thread_fork(cur_stack_base_addr, func, args, curr_tcb);
    lprintf ("DEBUG:  after thread fork");
    if (tid < 0) {
        return tid;
    }
    lprintf ("In Parent");	    
    int reject = 0;
    int ret = deschedule(&reject);
    if ( ret < 0) {
        return ret;
    }
    
    insert_rear(head_thr_list, curr_tcb->list);	
    cur_stack_base_addr = cur_stack_base_addr - 2*PAGE_SIZE;
    lprintf (" cur_stack_base_addr is %x", (int) cur_stack_base_addr);
     mutex_unlock(&thread_list);
    lprintf ("DEBUG:  thr_create stop");
    return tid;
}


void fork_handler(void *(*func) (void *), void *arg, tcb_t *curr_tcb)
{
    void  *status;
    //lprintf("argument address is %u",(int)arg);
    lprintf ("In thread, need to register handler");
    lprintf(" parent id is %d",curr_tcb->parentid);
    
    while(1) 
    {
        if (thr_yield(main_thread_id) != 0) {
	       break;
        }
    }


    curr_tcb->tid = gettid();
    //lprintf ("child tid = %x", curr_tcb->tid);

    status = (func)(arg); 
    //(func) (arg);
    lprintf("thread finished");

    //swexn((void *)(curr_tcb->stack_start_addr - thread_stack_size),
      //    page_fault_handler, NULL, NULL);
    //make_runnable(main_thread_id);
    thr_exit((void*)status);
}

int thr_yield( int tid ){
    return yield(tid);
}
