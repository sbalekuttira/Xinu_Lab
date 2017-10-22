/* kill.c - kill */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  kill  -  Kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
syscall	kill(
	  pid32		pid		/* ID of process to kill	*/
	)
{
    // LAB2: TODO: Modify this function to clean-up the pipe.

	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;	
	struct pipe_t *pip_ptr;
	int32	i;			/* Index into descriptors	*/

	mask = disable();
	if (isbadpid(pid) || (pid == NULLPROC)
	    || ((prptr = &proctab[pid])->prstate) == PR_FREE) {
		restore(mask);
		return SYSERR;
	}
	

	for(i=0;i<MAXPIPES;i++)
		{
			pip_ptr=&pipe_tables[i];

		if(pip_ptr->owner==pid)
			{pip_ptr->state=PIPE_DISCONNECTED;
				pipdelete(pipid32_to_did32(i));
			}


		if(pip_ptr->reader==pid||pip_ptr->writer==pid)
			{
							
				pipdisconnect(pipid32_to_did32(i));

			}
		}








	if (--prcount <= 1) {		/* Last user process completes	*/
		xdone();
	}

	send(prptr->prparent, pid);
	for (i=0; i<3; i++) {
		close(prptr->prdesc[i]);
	}

	freestk(prptr->prstkbase, prptr->prstklen);

	switch (prptr->prstate) {
	case PR_CURR:
		prptr->prstate = PR_FREE;	/* Suicide */
		resched();

	case PR_SLEEP:
	case PR_RECTIM:
		unsleep(pid);
		prptr->prstate = PR_FREE;
		break;

	case PR_WAIT:
		semtab[prptr->prsem].scount++;
		/* Fall through */

	case PR_READY:
		getitem(pid);		/* Remove from queue */
		/* Fall through */

	default:
		prptr->prstate = PR_FREE;
	}

	restore(mask);
	return OK;
}
