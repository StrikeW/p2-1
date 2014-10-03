/** @file 
 *  @brief 
 *  @author 
 *  @author 
 *
 *  @bugs
 *
 */

#include <syscall_int.h>
.global set_cursor_pos

set_cursor_pos:
    push %esi
    leal 8(%esp), %esi
    int $SET_CURSOR_POS_INT
    pop %esi
    ret

