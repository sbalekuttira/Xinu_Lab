#include <xinu.h>

uint32 pipwrite(struct dentry *devptr, char* buf, uint32 len) {

pipid32 pipe_id;
pipe_id=did32_to_pipid32(devptr->dvnum);
    
// LAB2: TODO

struct pipe_t *pip_ptr;
intmask mask;

int i=0;
int count=0;


mask=disable();


if( pipe_id < 0 || pipe_id >MAXPIPES || len<0)
	{

		restore(mask);
		return SYSERR;


	}

pip_ptr=&pipe_tables[pipe_id];

if(pip_ptr->writer!=currpid)
	{
		
	restore(mask);
	return SYSERR;
	
	}

if(pip_ptr->state==PIPE_READ_DISCONNECTED)
	{
	
	restore(mask);
	return SYSERR;
	
	}


for( i=0;i<len;i++)
 {
		
wait(pip_ptr->write_sem);
		
			

if(pip_ptr->state==PIPE_FREE)
{
						
restore(mask);
return count;
}
				
if(pip_ptr->state==PIPE_READ_DISCONNECTED)	//disconnected by reader
{
				
pipdisconnect(pipe_id);
restore(mask);
return OK;	
}

pip_ptr->write_pos=(pip_ptr->write_pos+1)%PIPE_SIZE;
		
pip_ptr->pipe_buffer[pip_ptr->write_pos]=buf[i];
			
count++;
			


signal(pip_ptr->read_sem);
			
			
}


kprintf("\n\n\n write done   \n\n");
kprintf("\nBytes Written :% d\n",count);

restore(mask);
return OK;
}