/* resched.c - resched, resched_cntl */

#include <xinu.h>

#define INT_MAX 2147483647
 				
extern uint32 myclock;          /*Extra added by sbalekut             */
struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}


	

	/* Point to process table entry for the current (old) process */
	/* for TS sched added by SBALEKUT */
	ptold = &proctab[currpid];
	
	if(ptold->prstate!=PR_FREE && ptold->prstate!=PR_CURR && ptold->prstate!=PR_READY)
		{

			ptold->blockingstate=TRUE;

		}




	if (ptold->group==1 && preempt>0 && ptold->prstate!=PR_CURR)
	{
		


 			if(ptold->prprio>= 0 && ptold-> prprio < 10)
                                 {
                                        ptold->tstrack=200;
                                        ptold->prprio=50;

                                }

                         else if (ptold->prprio >= 10 && ptold->prprio <20)

                                {

                                        ptold->tstrack=160;
                                         ptold->prprio=51;


                                 }



                         else if( ptold->prprio >=20 && ptold->prprio <30 )
                                {

                                         ptold->tstrack=120;
                                         ptold->prprio=52;

                                }

                                                                                //IO bound dispatch table added by SBALEKUT
                         else if (ptold->prprio >= 30 && ptold->prprio <40)

                                 {

                                         ptold->tstrack=80;
                                         ptold->prprio=54;


                                 }


			 else if( ptold->prprio >=40 && ptold->prprio <45 )
                                 {

                                         ptold->tstrack=40;
                                         ptold->prprio=55;

                                 }

                         else if (ptold->prprio == 45 )

                                 {

                                         ptold->tstrack=40;
                                          ptold->prprio=56;


                                 }



                         else if( ptold->prprio ==46 )
                                 {

                                         ptold->tstrack=40;
                                         ptold->prprio=57;

                                 }

                         else if (ptold->prprio >= 47 && ptold->prprio <59)

                                 {

                                         ptold->tstrack=40;
                                        ptold->prprio=58;


                                 }



			else
                                 {

                                         ptold->tstrack=20;
                                         ptold->prprio=59;

                                }

	}




			
              				
	else if(ptold->group==1) 			//CPU BOUND Added by SBALEKUT
		{				//	kprintf("Inside CPU BLock for process %s \n\n",ptold->prname); 
		
			if(ptold->prprio>= 0 && ptold-> prprio <= 9) 
				{		
					ptold->tstrack=200;
					ptold->prprio=0;

				}
	
				else if (ptold->prprio == 10)
	
				{

					ptold->tstrack=200;
					ptold->prprio=0;


				}
				


				else if( ptold->prprio >=11 && ptold->prprio <20 )
				{	

					ptold->tstrack=160;
					ptold->prprio=ptold->prprio-10;

				}	

				 else if( ptold->prprio >=20 && ptold->prprio <30 )              
      				  {
                
               				 ptold->tstrack=120;
                			ptold->prprio=ptold->prprio-10;

			         }
				 else if( ptold->prprio >=30 && ptold->prprio <40 )              
       				 {
                
               				 ptold->tstrack=80;
               				 ptold->prprio=ptold->prprio-10;

       				 }
				 else if( ptold->prprio >=40 && ptold->prprio <59 )              
       				 {
                
              				  ptold->tstrack=40;
               				 ptold->prprio=ptold->prprio-10;

       				 }

	
				else if (ptold->prprio == 59)

        			{

              				  ptold->tstrack=20;
               				 ptold->prprio=ptold->prprio-10;


       				 }

       			 
               				 

		}




	ptold->lasttime = myclock- (ptold->lasttime);
	

	if(currpid != NULLPROC && ptold->group!=1)                                                  /*Extra added by sbalekut */
	{

	ptold->prprio=ptold->prprio+ ((ptold->lasttime * 100) / ptold->rate); /*Extra added by sbalekut */ 
	
	}

	
/* Aging scheduler code written here. EXtra added by SBALEKUT */


int which_group;   //Which group is picked
uint32 TS_prio;		//TS group priority	
uint32 PS_prio;		//PS group priority
qid16 ready_iter;	//Iterator for readylist
uint32 TS_count=0;	//TS process counter
uint32 PS_count=0;	//PS proces counter


	  ready_iter=firstid(readylist);
/*Traversing ready list added by Sbalekut */ 
		while(ready_iter!=queuetail(readylist))
		{
		if(ready_iter!=NULLPROC && proctab[ready_iter].group==1)
			{
			TS_count=TS_count+1;


			}
		else if(ready_iter!=NULLPROC && proctab[ready_iter].group==0)
			{

			PS_count=PS_count+1;	

			}

		ready_iter=queuetab[ready_iter].qnext;
		}



//kprintf (" \n \n Inside  Aging old priority  for TS group  is %d and PS group  is %d \n",group_TS.priority, group_PS.priority);
	
	
	
	if(ptold->group==0)
	{

	group_PS.priority=group_PS.pr_init;
	}

	
	
	if(ptold->group==1)
        {

        group_TS.priority=group_TS.pr_init;
        }

	group_PS.priority+=PS_count;
	group_TS.priority+=TS_count;


//kprintf (" \n \n Inside Aging new prio for TS group is %d and PS group  is %d \n",group_TS.priority, group_PS.priority);


	
	if (ptold->prstate == PR_CURR)

 {  

		/* Old process will no longer remain current */


		ptold->prstate = PR_READY;
		
	
		if(currpid==NULLPROC)
			{
		insert(currpid, readylist, ptold->prprio);
			}


		else if(ptold->group==0)
			{


			 insert(currpid, readylist, INT_MAX-ptold->prprio);
			}
		else if (ptold->group==1)
			{

			insert(currpid, readylist, ptold->prprio);


			}
	}

	/* Force context switch to highest priority ready process */




//kprintf("Just before IF TS prio is %d and PS prio is %d", group_TS.priority,group_PS.priority); 

	if( group_TS.priority > group_PS.priority)
		{
		which_group=1;
	//	kprintf("Which group is %n ", which_group);

		}
	else
		{	which_group=0;

		}

 //kprintf("which group is %d ", which_group);

	if(which_group==0 && TS_count > 0 && PS_count==0)
	{

		which_group=1;						// if there are no processes in group

	}

	else if(which_group==1 && TS_count==0)
	{

	which_group=0;						//There will always be null process in PS

	}


 //kprintf("which group is %d ", which_group);

	
	ready_iter=firstid(readylist);
	currpid=ready_iter;


	while(ready_iter!=queuetail(readylist))

		{

		if(proctab[ready_iter].group == which_group)
			{
				
			currpid=getitem(ready_iter);
			break;
			}

		else
		{
			ready_iter=queuetab[ready_iter].qnext;

		}

	}


	





	ptnew = &proctab[currpid];

	ptnew->prstate = PR_CURR;
	

	if(ptnew->group==1)
		{

			preempt=ptnew->tstrack;

		}	
	else
		{

			preempt=QUANTUM;


		}
    
	/* For was blocekd added by SBALEKut */ 
 if(ptnew->blockingstate=TRUE && currpid!=NULLPROC && ptnew->group==0)
		{ 
			if(ptnew->prprio >= myclock)
				ptnew->prprio=ptnew->prprio;

			else
				ptnew->prprio=myclock;

			ptnew->blockingstate=FALSE;

		}


	ptnew->lasttime=myclock;
		/* Reset time slice for process	*/
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
