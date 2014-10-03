/** @file 
 *  @brief 
 *  @author 
 *  @author 
 *
 *  @bugs
 *
 */

#include <syscall_int.h>

.global vanish


vanish:
    int $VANISH_INT 
    ret

