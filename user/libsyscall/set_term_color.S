#include <syscall_int.h>
.global set_term_color

set_term_color:
    push %esi
    movl 8(%esp), %esi
    int $SET_TERM_COLOR_INT
    pop %esi
    ret

