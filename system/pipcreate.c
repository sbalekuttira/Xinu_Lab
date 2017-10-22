#include <xinu.h>


pid32 did32_to_pipid32(did32 devpipe) {
    ASSERT(devpipe >= PIPELINE0 && devpipe <= PIPELINE9);
    return devpipe - PIPELINE0;
}

did32 pipid32_to_did32(pipid32 pip) {
    ASSERT(pip >= 0 && pip <= 9);
    return PIPELINE0 + pip;
}



local	pipid32	newpipe_id(void)
{
	uint32	i;					
	static	pid32 nextpip_id = 0;	
						



	for (i = 0; i < MAXPIPES; i++) {
		nextpip_id %= MAXPIPES;	                    /* Wrap around to beginning */
		if (pipe_tables[nextpip_id].state == PIPE_FREE) {
			pipe_tables[nextpip_id].state=PIPE_USED;
	
			return nextpip_id++;
		} else {
			nextpip_id++;
		}
	}
	return (pid32) SYSERR;
}






did32 pipcreate() {
   
uint32 i;
struct pipe_t *pip_ptr;
pipid32 pip_id;
intmask mask;

mask =disable();
pip_id=newpipe_id();

if(pip_id==SYSERR )						//no processes left
	{
		restore(mask);
		return SYSERR;
	}

pip_ptr=&pipe_tables[pip_id];
pip_ptr->state=PIPE_USED;
pip_ptr->owner=currpid;
pip_ptr->read_pos=-1;
pip_ptr->write_pos=-1;


for(i=0;i<PIPE_SIZE;i++)
	{
	pip_ptr->pipe_buffer[i]=NULL;
	}


pip_ptr->read_sem=semcreate(0);
pip_ptr->write_sem=semcreate(PIPE_SIZE);



	
restore(mask);


return pipid32_to_did32(pip_id);
}




 // LAB2: TODO
   // return -1;
//}
