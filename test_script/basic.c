/*
 * ===================================================================
 * TEST FILE: main.c (Valgrind-Style Output for FD Tracker)
 * ===================================================================
 * This file only includes memtracker.h and standard libraries.
 * It must be linked against your tracker implementation.
 * (e.g., gcc main.c fd_tracker.c -o my_program)
 */

// This is the header you provided
#include "fdtracker.h" 

// Standard libraries needed for a "user" to run tests
#include <stdio.h>    // For printf
#include <fcntl.h>      // For open()
#include <unistd.h>     // For close()
#include <sys/stat.h>   // For open()

// Fake PID (same as old test)
#define PID 21771

int main(void)
{
	// --- Counters for the final summary ---
	long total_registered = 0;
	long total_closed = 0;
	int  leaked_fds = 0; // FDs we open but fail to register

	int fd;
	int fd1;
	int fd2;
	int i;

	printf("==%d== FD Tracker Test Runner\n", PID);
	printf("==%d== Using MAX_FD_TRACKER = %d\n", PID, MAX_FD_TRACKER);
	printf("==%d== \n", PID);

	/*
	 * 1. Normal registration
	 */
	printf("==%d== Test 1: Normal Registration\n", PID);
	fd1 = open("/dev/null", O_RDONLY);
	if (fd1 >= 0) {
		printf("==%d==  Registering FD %d\n", PID, fd1);
		fd_register(1, fd1);
		total_registered++;
	}
	fd2 = open("/dev/null", O_RDONLY);
	if (fd2 >= 0) {
		printf("==%d==  Registering FD %d\n", PID, fd2);
		fd_register(1, fd2);
		total_registered++;
	}
	printf("==%d== ...Test 1 Complete\n", PID);
	printf("==%d== \n", PID);

	/*
	 * 3. close all (for Test 1)
	 */
	printf("==%d== Test 2: Close All (Cleanup Test 1)\n", PID);
	fd_close_all();
	total_closed += 2; // We registered 2 FDs
	printf("==%d== ...Test 2 Complete\n", PID);
	printf("==%d== \n", PID);


	/*
	 * 2. up to 1024 times registration
	 */
	printf("==%d== Test 3: %d Registrations (Fill Buffer)\n", PID, MAX_FD_TRACKER);
	for (i = 0; i < MAX_FD_TRACKER; i++)
	{
		fd = open("/dev/null", O_RDONLY);
		if (fd < 0) {
			printf("==%d==  OPEN FAILED (system FD limit?) at iteration %d\n", PID, i);
			break;
		}
		fd_register(1, fd);
		total_registered++;
	}
	printf("==%d== ...Test 3 Complete (Registered %d items)\n", PID, i);
	printf("==%d== \n", PID);


	/*
	 * 3. close all (for Test 3)
	 */
	printf("==%d== Test 4: Close All (Cleanup Test 3)\n", PID);
	fd_close_all();
	total_closed += i; // Close all FDs that were successfully open/reg
	printf("==%d== ...Test 4 Complete\n", PID);
	printf("==%d== \n", PID);


	/*
	 * 4. up to 1025 times registration
	 */
	printf("==%d== Test 5: %d Registrations (Test Overflow)\n", PID, MAX_FD_TRACKER + 1);
	printf("==%d== (This test should produce one 'BUFFER_OVERFLOW' error)\n", PID);
	for (i = 0; i < MAX_FD_TRACKER + 1; i++)
	{
		fd = open("/dev/null", O_RDONLY);
		if (fd < 0) {
			printf("==%d==  OPEN FAILED (system FD limit?) at iteration %d\n", PID, i);
			break;
		}
		
		// This will call your tracker for the 1025th FD
		fd_register(1, fd);

		if (i < MAX_FD_TRACKER) {
			total_registered++;
		} else {
			// This FD was opened, but we assume it wasn't tracked
			// We expect the tracker to *not* close it
			// We will *not* close it either, so Valgrind sees it as a leak
			printf("==%d==  Leaking FD %d (untracked)\n", PID, fd);
			leaked_fds++;
		}
	}
	printf("==%d== ...Test 5 Complete\n", PID);
	printf("==%d== \n", PID);


	/*
	 * 3. close all (for Test 5)
	 */
	printf("==%d== Test 6: Close All (Cleanup Test 5)\n", PID);
	fd_close_all();
	total_closed += MAX_FD_TRACKER; // Should close the 1024 it tracked
	printf("==%d== ...Test 6 Complete\n", PID);
	printf("==%d== \n", PID);


	/*
	 * --- Final Summary ---
	 */
	long fds_in_use = total_registered - total_closed;

	printf("==%d== FILE DESCRIPTOR SUMMARY:\n", PID);
	printf("==%d==    in use at exit: %ld open file descriptors\n", 
		PID, fds_in_use + leaked_fds);
	printf("==%d==  total fd usage: %ld registered, %ld closed\n",
		PID, total_registered, total_closed);
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
