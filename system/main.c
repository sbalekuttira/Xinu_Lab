/*  main.c  - main */

#include <xinu.h>



void cpu_bound(void)
{

int i,loop1=1000;
int j,loop2=2000;
int z;
for(i=0;i<loop1;i++)
{

for(j=0;j<loop2;j++)
{

z=i*j;

}

}

}

void iobound(void)
{
int i,loop1=1000;
int j,loop2=2000;
int z;
for(i=0;i<loop1;i++)
{

for(j=0;j<loop2;j++)
{

sleepms(10);

}

}

}


process	main(void)
{

	/* Run the Xinu shell */

	recvclr();
	
	resume(create(cpu_bound,4096,1,1,"cpu1",0));
	resume(create(cpu_bound,4096,1,1,"cpu2",0));
	resume(create(cpu_bound,4096,1,1,"cpu3",0));
	resume(create(cpu_bound,4096,1,1,"cpu4",0));
	resume(create(cpu_bound,4096,1,1,"cpu5",0));
	resume(create(cpu_bound,4096,1,1,"cpu6",0));



//	resume(create(sleepy,4096,40,"sleepy",0));
	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(2000);
		kprintf("\n\nMain process recreating shell jaxqueen\n\n");
		resume(create(shell, 4096,1, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}
