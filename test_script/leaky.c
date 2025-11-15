/*
 * ===================================================================
 * TEST FILE: main.c (Intentional FD Leak Test)
 * ===================================================================
 * This program intentionally leaks file descriptors to test
 * Valgrind's --track-fds=all flag.
 *
 * It does not use any custom libraries.
 */

// Standard libraries needed for this test
#include <stdio.h>    // For printf
#include <fcntl.h>      // For open()
#include <unistd.h>     // For close()
#include <sys/stat.h>   // For open()

// Fake PID (same as old test)
#define PID 21771

int main(void)
{
	// --- Counters for the final summary ---
	long total_opened = 0;
	long total_closed = 0;
	int  leaked_fds = 0;

	int fd1;
	int fd2;

	printf("==%d== Intentional FD Leak Test Runner\n", PID);
	printf("==%d== \n", PID);

	/*
	 * 1. Open FDs and "forget" to close them
	 */
	printf("==%d== Test 1: Leaking FDs\n", PID);
	
	fd1 = open("/dev/null", O_RDONLY);
	if (fd1 >= 0) {
		printf("==%d==  Opened FD %d (will not be closed)\n", PID, fd1);
		total_opened++;
		leaked_fds++;
	} else {
		printf("==%d==  open() failed for fd1\n", PID);
	}

	fd2 = open("/dev/null", O_RDONLY);
	if (fd2 >= 0) {
		printf("==%d==  Opened FD %d (will not be closed)\n", PID, fd2);
		total_opened++;
		leaked_fds++;
	} else {
		printf("==%d==  open() failed for fd2\n", PID);
	}
	
	printf("==%d== ...Test 1 Complete\n", PID);
	printf("==%d== \n", PID);


	/*
	 * --- Final Summary ---
	 */
	printf("==%d== FILE DESCRIPTOR SUMMARY:\n", PID);
	printf("==%d==    in use at exit: %d open file descriptors\n", 
		PID, leaked_fds);
	printf("==%d==  total fd usage: %ld opened, %ld closed\n",
		PID, total_opened, total_closed);
	printf("==%d== \n", PID);
	
	if (leaked_fds > 0) {
		printf("==%d== LEAK SUMMARY:\n", PID);
		printf("==%d==    definitely lost: %d file descriptors\n", 
			PID, leaked_fds);
	} else {
		printf("==%d== All file descriptors were closed -- no leaks are possible\n", PID);
	}

	return 0;
}
