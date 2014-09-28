#include <syscall_int.h>

.global exec

/* assembly wrapper function for invoking exec syscall */
exec:
    push %esi
    leal 8(%esp), %esi 
    int $EXEC_INT /* invoke exec system call */
    pop %esi
    ret

