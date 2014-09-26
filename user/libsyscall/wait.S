#include <syscall_int.h>
.global wait

wait:
    push %esi
    movl 8(%esp), %esi 
    int $WAIT_INT
    pop %esi
    ret

