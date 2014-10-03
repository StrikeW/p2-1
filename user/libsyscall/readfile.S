/** @file 
 *  @brief 
 *  @author 
 *  @author 
 *
 *  @bugs
 *
 */

#include <syscall_int.h>
.global readfile

readfile:
    push %esi
    leal 8(%esp), %esi
    int $READFILE_INT
    pop %esi
    ret

