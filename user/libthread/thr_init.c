#include "thread.h"
#include "variable_queue.h"
#include "thr_internals.h"

int thr_init( unsigned int size )
{
    thread_stack_size = THR_SIZE_ALIGN(size);
    lprintf ("DEBUG:  thr_init start");

    tcb_t *curr_tcb = construct_tcb();

    curr_tcb->stack_start_addr = main_thr_high_addr;
    curr_tcb->parentid = 0;
    curr_tcb->state = RUNNABLE;
    curr_tcb->join_tid = DEFAULT_JOIN_TID;
    
    main_thread_id = curr_tcb->tid = gettid();

    //head_thr_list = curr_tcb->list;

    Q_INIT_HEAD(&tcb_list);

    lprintf ("main thread tid = %x",main_thread_id);
    MULTI_THREADING_ENABLE();
    lprintf ("DEBUG:  thr_init end");
    mutex_init(&thread_list);
    return 0;
}






