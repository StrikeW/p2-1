#include <syscall_int.h>
.global get_cursor_pos

get_cursor_pos:
    push %esi  
    leal 8(%esp), %esi
    int $GET_CURSOR_POS_INT 
    pop %esi
    ret

