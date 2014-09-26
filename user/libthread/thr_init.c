#include"thread.h"



int thr_init( unsigned int size )
{
    thread_stack_size = THR_SIZE_ALIGN(size);
    lprintf ("DEBUG:  thr_init start");
    tcb_t *curr_tcb = construct_tcb(); 
    curr_tcb->main_thread = 1;
    curr_tcb->stack_start_addr = main_thr_high_addr;
    curr_tcb->state = RUNNING;
    head_thr_list = curr_tcb->list;
    main_thread_id = curr_tcb->tid = gettid();
    lprintf ("main thread tid = %x",main_thread_id);
    MULTI_THREADING_ENABLE();
    lprintf ("DEBUG:  thr_init end");
    return 0;
}






