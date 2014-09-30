#include "thr_internals.h"




int thr_init( unsigned int size );
int thr_create( void *(*func)(void *), void *args );
void fork_handler(void *(*func) (void *), void *arg, tcb_t*);
void thr_exit( void *status );
int thr_yield( int tid );
void page_fault_handler(int *ret_addr, void *arg, ureg_t *ureg);
