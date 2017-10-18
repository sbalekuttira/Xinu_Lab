/*  main.c  - main */
#include <xinu.h>

#define PS 0
#define TS 1

void tc_cpuintensive() {
	int i, j;
	int LOOP1 = 10;
	int LOOP2 = 10000000;

	struct procent *pr = &proctab[currpid];

	int v = 0;
	for (i=0; i<LOOP1; i++) {
		for (j=0; j<LOOP2; j++) {
			// Insert code that performs memory copy (slow) and/or
			// ALU operations (fast).
			// Note: this loop consumes significant CPU cycles.
			v += i * j;
		}

		// Using kprintf print the pid followed the outer loop count i,
		// the process's priority and remaining time slice (preempt).
		kprintf("PID: %d, Loop: %d, Priority: %d, Remaining Time Slice: %d\n", currpid, i, pr->prprio, preempt);
	}

	kprintf("===== CPU BOUNDED PID %d ends\n", currpid);
}

void tc_iointensive(uint32 time) {
	int i;
	int LOOP1 = 30;

	struct procent *pr = &proctab[currpid];

	for (i=0; i<LOOP1; i++) {
		sleepms(time);

		// Using kprintf print the pid followed the outer loop count i,
		// the process's priority and remaining time slice (preempt).
		kprintf("PID: %d, Sleep time: %d, Loop: %d, Priority: %d, Remaining Time Slice: %d\n", currpid, time, i, pr->prprio, preempt);
	}

	// Print the CPU time consumed by the process that is recorded in the
	// prcputime field of the current process's process table entry.

	kprintf("===== IO BOUNDED PID %d\n", currpid);
}



void sleep_and_wakeup_cpuintensive() {
	sleepms(5000);
	tc_cpuintensive();

	return;
}

int main(void) {
	int i;

	kprintf("===PS TEST 1===\n");
	resched_cntl(DEFER_START);
	for (i = 0; i < 4; i++) {
		resume(create(tc_cpuintensive, 1024, 0, 25, "cpu-intense", 0, NULL));
	}
	resched_cntl(DEFER_STOP);
	sleepms(60000);
	kprintf("\n\n\n\n");

	kprintf("===PS TEST 2===\n");
	resched_cntl(DEFER_START);
	for (i = 0; i < 4; i++) {
    if (i % 2 == 0) {
			resume(create(tc_cpuintensive, 1024, 0, 90, "cpu-intense", 0, NULL));
		} else {
			resume(create(tc_cpuintensive, 1024, PS, 10, "cpu-intense", 0, NULL));
		}
	}
	resched_cntl(DEFER_STOP);
	sleepms(60000);
	kprintf("\n\n\n\n");

	kprintf("===PS TEST 3===\n");
	resched_cntl(DEFER_START);
	for (i = 0; i < 4; i++) {
		if (i % 2 == 0) {
			resume(create(sleep_and_wakeup_cpuintensive, 1024, PS, 25, "sleep_and_wakeup_cpuintensive", 0, NULL));
		} else {
			resume(create(tc_cpuintensive, 1024, PS, 25, "cpu-intense", 0, NULL));
		}
	}
	resched_cntl(DEFER_STOP);
	sleepms(60000);
	kprintf("\n\n\n\n");

	kprintf("===PS TEST 4===\n");
	resched_cntl(DEFER_START);
	for (i = 0; i < 4; i++) {
		if (i % 2 == 0) {
			resume(create(sleep_and_wakeup_cpuintensive, 1024, PS, 80, "sleep_and_wakeup_cpuintensive", 0, NULL));
		} else {
			resume(create(tc_cpuintensive, 1024, PS, 40, "cpu-intense", 0, NULL));
		}
	}
	resched_cntl(DEFER_STOP);
	sleepms(60000);
	kprintf("\n\n\n\n");

	// TS: TEST CASE 1
	kprintf("===TS TEST1===\n");
	resched_cntl(DEFER_START);
	for (i = 0; i < 6; i++) {
		resume(create(tc_cpuintensive, 1024, TS, 20, "cpu-intense", 0, NULL));
	}
	resched_cntl(DEFER_STOP);
	sleepms(60000);
	kprintf("\n\n\n\n");

	// TS: TEST CASE 2
	kprintf("===TS TEST2===\n");
	resched_cntl(DEFER_START);
	for (i = 0; i < 6; i++) {
		resume(create(tc_iointensive, 2048, TS, 20, "io-intense", 1, 32));
	}
	resched_cntl(DEFER_STOP);
	sleepms(60000);
	kprintf("\n\n\n\n");

	// TS: TEST CASE 3
	kprintf("===TS TEST3===\n");
	resched_cntl(DEFER_START);
	for (i = 0; i < 6; i++) {
		if (i % 2 == 0) {
			resume(create(tc_cpuintensive, 1024, TS, 20, "cpu-intense", 0));
		}
		else {
			resume(create(tc_iointensive, 1024, TS, 20, "io-intense", 1, 32));
		}
	}
	resched_cntl(DEFER_STOP);
	sleepms(60000);
	kprintf("\n\n\n\n"); 

	kprintf("===TS TEST4===\n");
	resched_cntl(DEFER_START);

	resume(create(tc_iointensive, 1024, TS, 20, "io-intense", 1, 71));
	resume(create(tc_iointensive, 1024, TS, 20, "io-intense", 1, 101));
	resume(create(tc_iointensive, 1024, TS, 20, "io-intense", 1, 157));

	for (i = 0; i < 3; i++) {
		resume(create(tc_cpuintensive, 1024, TS, 20, "cpu-intense", 0));
	}
	resched_cntl(DEFER_STOP);
	sleepms(60000);
	kprintf("\n\n\n\n");


	kprintf("===AGING TEST 1===\n");
	resched_cntl(DEFER_START);
	for (i = 0; i < 2; i++) {
		resume(create(tc_cpuintensive, 1024, PS, 25, "cpu-intense", 0, NULL));
	}
	for (i = 0; i < 2; i++) {
		resume(create(tc_cpuintensive, 1024, TS, 25, "cpu-intense", 0, NULL));
	}
	resched_cntl(DEFER_STOP);
	sleepms(60000);
	kprintf("\n\n\n\n");

	kprintf("===AGING TEST 2===\n");
	chgprio(PS, 18);
	resched_cntl(DEFER_START);
	for (i = 0; i < 2; i++) {
		resume(create(tc_cpuintensive, 1024, PS, 25, "cpu-intense", 0, NULL));
	}
	for (i = 0; i < 2; i++) {
		resume(create(tc_cpuintensive, 1024, TS, 25, "cpu-intense", 0, NULL));
	}
	resched_cntl(DEFER_STOP);
	sleepms(60000);
	kprintf("\n\n\n\n");

	kprintf("===AGING TEST 3===\n");
	chgprio(PS, 10);
	chgprio(TS, 18);
	resched_cntl(DEFER_START);
	for (i = 0; i < 2; i++) {
		resume(create(tc_cpuintensive, 1024, PS, 25, "cpu-intense", 0, NULL));
	}
	for (i = 0; i < 2; i++) {
		resume(create(tc_cpuintensive, 1024, TS, 25, "cpu-intense", 0, NULL));
	}
	resched_cntl(DEFER_STOP);
	sleepms(60000);
	kprintf("\n\n\n\n");


	return OK;
}

