
#include<thread.h>


void thr_exit( void *status )
{
    lprintf("in Thread exit");
    vanish();
}

int thr_yield( int tid ){
    return yield(tid);
}
