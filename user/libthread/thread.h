#include"thr_internals.h"




int thr_init( unsigned int size );
int thr_create( void *(*func)(void *), void *args );
void fork_handler(void *(*func) (void *), void *arg);
