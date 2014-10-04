/** @file xchg.S 
 *  @brief Serve an atomic instruction for lock/unlock 
 *	
 *  We use the XCHG instruction as means to lock the 
 *  synchronisation construct in question
 *
 *  @author  Yuhang Jiang (yuhangj) 
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs No known bugs
 */

.global atom_xchg

atom_xchg:
 	movl 8(%esp),%eax  #LOCK = 1
 	movl 4(%esp),%ecx  #mp->lock
 	XCHG (%ecx), %eax  #eax = mp->lock
 	ret
