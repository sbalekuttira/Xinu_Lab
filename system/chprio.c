/* chprio.c - chprio */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  chprio  -  Change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
pri16	chprio(
	  pid32		pid,		/* ID of process to change	*/
	  pri16		newprio		/* New priority			*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	pri16	oldprio;		/* Priority to return		*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return (pri16) SYSERR;
	}
	prptr = &proctab[pid];
	oldprio = prptr->prprio;
	prptr->prprio = newprio;
	restore(mask);
	return oldprio;
}

/* change priority of group Extra added by sablekut */
pri16   chgprio(
          int         groups,            /* PS or TS group      */
          pri16         newprio         /* New priority                 */
        )
{
        intmask mask;                   /* Saved interrupt mask         */
       // struct  procent *prptr;         /* Ptr to process's table entry */
        pri16   oldprio;                /* Priority to return           */
//kprintf("\n\n inside chgprio new group %d and  prio %d  \n",groups,newprio );
        mask = disable();
      //  if (groups!=1 || groups!=0) {
        //        restore(mask);
          //      return (pri16) SYSERR;
     //   }
       
//kprintf("\n \n outside mask \n \n"); 


	if(groups==1)
	{

        oldprio = group_TS.pr_init;
     	 
	}
	else if(groups==0)
	{
	oldprio = group_PS.pr_init;


	}

	
	if(groups==1)

	 	group_TS.pr_init = newprio;
	else if(groups==0)

		group_PS.pr_init =newprio;


//	kprintf("\n \n chgprio new prio of TS group is %d and new prio of PS group is %d \n" , group_TS.priority,group_PS.priority);  
        restore(mask);
        return oldprio;
}



















