/** @file xchg.S 
 *  @brief 
 *  @author  Yuhang Jiang (yuhangj) 
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs
 *
 */

 .global atom_xchg

 atom_xchg:
 	movl 8(%esp),%eax  #LOCK = 1
 	movl 4(%esp),%ecx  #mp->lock
 	XCHG (%ecx), %eax  #eax = mp->lock
 	ret
