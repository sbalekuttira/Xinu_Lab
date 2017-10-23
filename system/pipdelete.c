#include <xinu.h>

status pipdelete(did32 devpipe) {


pipid32 pipe_id;

pipe_id=did32_to_pipid32(devpipe);
intmask	mask;				
struct	pipe_t *pip_ptr;		
int32	i;					

mask = disable();
if ( pipe_id < 0 ||  pipe_id >= MAXPIPES )
	 {  
		restore(mask);
		return SYSERR;
	}


//if ( ((pip_ptr = &pipe_tables[pipe_id])->owner)!=currpid || ((pip_ptr = &pipe_tables[pipe_id])->state) == PIPE_FREE) 
//	{ 
//		restore(mask);
	//	return SYSERR;
//	}



pip_ptr = &pipe_tables[pipe_id];
if (pip_ptr->state == PIPE_FREE) {
	restore(mask);
	return SYSERR;
	}


for(i=0;i<PIPE_SIZE;i++)
{
        pip_ptr->pipe_buffer[i]=NULL;
}


semdelete(pip_ptr->read_sem);
semdelete(pip_ptr->write_sem);
//kprintf("\n\n PIPE DELETED \n\n");

pip_ptr->read_pos=-1;
pip_ptr->write_pos=-1;

pip_ptr->state=PIPE_FREE;

	

restore(mask);
return OK;



    // LAB2: TODO
//	return OK;
}
