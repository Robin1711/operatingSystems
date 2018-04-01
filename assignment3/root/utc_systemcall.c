#include <lib.h>
#include <stdio.h>

int main(void) {
	message m;

	int return_sys = _syscall(PM_PROC_NR, UTCTIME, &m);
	printf("testing, %d\n", return_sys);
	time_t output = m.m2_l1;
	time_t rawtime;
	int wrong_time = time(&rawtime);
	printf("utc: %d, time: %d\n", output, wrong_time);
	return 0; 
}
