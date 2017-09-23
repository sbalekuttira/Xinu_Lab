/*  main.c  - main */

#include <xinu.h>
void myprocess(void){
kprintf("hello world\n");
}

process	main(void)
{

	/* Run the Xinu shell */

	recvclr();
	resume(create(myprocess, 4096, 25, "helloworld", 0));
	
	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell jaxqueen\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}
