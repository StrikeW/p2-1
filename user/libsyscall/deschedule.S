#include <syscall_int.h>
.global deschedule


deschedule:
    push %esi
    movl 8(%esp), %esi
    int $DESCHEDULE_INT
    pop %esi
    ret

