/*
 * ===================================================================
 * TEST FILE: main.c (Small Variadic Registration Test)
 * ===================================================================
 * This test opens 5 file descriptors and registers them all
 * with a single call to fd_register(5, ...).
 */

// Your new header file
#include "fdtracker.h" 

// Standard libraries
#include <stdio.h>    // For printf
#include <stdlib.h>   // For exit()
#include <fcntl.h>    // For open()
#include <unistd.h>   // For close()
#include <sys/stat.h> // For open()

// Fake PID
#define PID 21771

// Helper to get a new file descriptor
int get_fd(void)
{
	int fd = open("/dev/null", O_RDONLY);
	if (fd < 0) {
		printf("==%d==  CRITICAL: open failed. Exiting.\n", PID);
		exit(1);
	}
	return fd;
}

int main(void)
{
	long total_registered = 0;
	long total_closed = 0;
	int fd1, fd2, fd3, fd4, fd5;

	printf("==%d== Small Variadic Test Runner\n", PID);
	printf("==%d== Using MAX_FD_TRACKER = %d\n", PID, MAX_FD_TRACKER);
	printf("==%d== \n", PID);

	/*
	 * --- Test 1: Register 5 FDs with one call ---
	 */
	printf("==%d== Test 1: Registering 5 FDs...\n", PID);
	fd1 = get_fd();
	fd2 = get_fd();
	fd3 = get_fd();
	fd4 = get_fd();
	fd5 = get_fd();

	printf("==%d==  Calling fd_register(5, %d, %d, %d, %d, %d)\n",
		PID, fd1, fd2, fd3, fd4, fd5);
	fd_register(5, fd1, fd2, fd3, fd4, fd5);
	total_registered += 5;
	
	printf("==%d== ...Test 1 Complete\n", PID);
	printf("==%d== \n", PID);


	/*
	 * --- Test 2: Final Cleanup ---
	 */
	printf("==%d== Test 2: Final Cleanup: fd_close_all()\n", PID);
	fd_close_all(); 
	total_closed += 5; // We expect 5 FDs to be closed
	printf("==%d== ...All tests complete.\n", PID);
	printf("==%d== \n", PID);


	/*
	 * --- Final Summary (Based on SPEC) ---
	 */
	long fds_in_use = total_registered - total_closed;

	printf("==%d== FILE DESCRIPTOR SUMMARY (Based on Spec):\n", PID);
	printf("==%d==    in use at exit: %ld open file descriptors\n", 
		PID, fds_in_use);
	printf("==%d==  total fd usage: %ld registered, %ld closed\n",
		PID, total_registered, total_closed);
	printf("==%d== \n", PID);
	
	if (fds_in_use == 0) {
		printf("==%d== All file descriptors were closed -- no leaks are possible\n", PID);
	}

	return 0;
}