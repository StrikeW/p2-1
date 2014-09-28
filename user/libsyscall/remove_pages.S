#include <syscall_int.h>
.global remove_pages

remove_pages:
    push %esi
    movl 8(%esp), %esi
    int $REMOVE_PAGES_INT 
    pop %esi
    ret
