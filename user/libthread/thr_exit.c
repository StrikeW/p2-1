#include<thread.h>

void thr_exit( void *status ){
    //int tid = gettid();
    //int join_tid;
    //tcb_t *thread_node;
    /*
    mutex_lock(&thread_list);
    thread_node =(tcb_t *) (remove_node((list_head*)head, tid));
    //thread_node-> tid = thr_getid();
    insert_rear(join_queue, (list_head*)thread_node);
    ((tcb_t *) thread_node)->exit_status = status;
    join_tid = thread_node->join_tid;
    mutex_unlock(&thread_list);
    make_runnable(join_tid);
    */
    lprintf ("VANISH");
    vanish();
}





