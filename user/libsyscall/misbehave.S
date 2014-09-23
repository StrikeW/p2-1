#include <syscall_int.h>
.global misbehave

misbehave:
    push %esi
    movl 8(%esp), %esi
    int $MISBEHAVE_INT 
    pop %esi
    ret

