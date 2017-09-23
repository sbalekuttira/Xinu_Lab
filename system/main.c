/*  main.c  - main */

#include <xinu.h>



void myprocess(void)
{

int i;
int j;
for(i=0;i<1000;i++)
{

for(j=0;j<10;j++)
{



}

}

}

void sleepy(void)
{


kprintf("\n \n Suj \n ");
sleepms(200);
kprintf("\n \n \n sujay \n \n");


}


process	main(void)
{

	/* Run the Xinu shell */

	recvclr();
	
	resume(create(myprocess,4096,30,"myprocess",0));
	resume(create(sleepy,4096,40,"sleepy",0));
	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell jaxqueen\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}
