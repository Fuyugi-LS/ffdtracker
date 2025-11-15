/*
 * ===================================================================
 * TEST FILE: main.c (FD Overflow + Partial Close Test)
 * ===================================================================
 * This test is designed to be compiled with:
 * cc -D MAX_FD_TRACKER=16 *.c -Isrc -Lsrc -lfdtracker -o a.out
 *
 * It tests the buffer-full state and then verifies fd_close_partial.
 */

// We assume MAX_FD_TRACKER is set at compile time.
#include "fdtracker.h"

// Standard libraries
#include <stdio.h>
#include <stdlib.h>   // For exit()
#include <fcntl.h>    // For open()
#include <unistd.h>   // For close()
#include <sys/stat.h> // For open()

// Fake PID
#define PID 90001

// --- Global state for this test harness ---
static long g_total_registered_spec = 0; // FDs *we think* we registered
static long g_total_closed_spec = 0;     // FDs *we think* we closed
static int g_leaked_by_test = 0; // FDs *we* assume failed registration

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
	// We MUST hardcode 15 FDs to test fd_close_partial(15, ...)
	int fd1, fd2, fd3, fd4, fd5, fd6, fd7, fd8;
	int fd9, fd10, fd11, fd12, fd13, fd14, fd15;
	int fd_ok;

	printf("==%d== Partial Close Overflow Test\n", PID);
	printf("==%d== Using compile-time MAX_FD_TRACKER = %d\n", PID, MAX_FD_TRACKER);
	printf("==%d== \n", PID);

	/*
	 * --- Test 1: Setup - Fill buffer to (MAX_FD_TRACKER - 1) ---
	 * We register 15 FDs. (Assuming MAX_FD_TRACKER=16)
	 */
	printf("==%d== Test 1: Filling buffer to %d / %d slots...\n", PID, MAX_FD_TRACKER - 1, MAX_FD_TRACKER);
	fd1 = get_fd(); fd2 = get_fd(); fd3 = get_fd(); fd4 = get_fd();
	fd5 = get_fd(); fd6 = get_fd(); fd7 = get_fd(); fd8 = get_fd();
	fd9 = get_fd(); fd10 = get_fd(); fd11 = get_fd(); fd12 = get_fd();
	fd13 = get_fd(); fd14 = get_fd(); fd15 = get_fd();
	
	fd_register(15, fd1, fd2, fd3, fd4, fd5, fd6, fd7, fd8, fd9,
					fd10, fd11, fd12, fd13, fd14, fd15);
	g_total_registered_spec += 15;
	
	printf("==%d== ...Test 1 Complete\n", PID);
	printf("==%d== \n", PID);


	/*
	 * --- Test 2: Fill the buffer ---
	 * Buffer is at 15/16. We call fd_register(1, fd_ok).
	 * This fills the buffer to 16/16.
	 */
	printf("==%d== Test 2: Filling buffer to %d / %d slots...\n", PID, MAX_FD_TRACKER, MAX_FD_TRACKER);
	
	fd_ok = get_fd();
	fd_register(1, fd_ok);
	g_total_registered_spec += 1;
	
	printf("==%d== ...Test 2 Complete\n", PID);
	printf("==%d== \n", PID);

	/*
	 * --- Test 3: Partial Close ---
	 * We close the *first* 15 FDs, leaving fd_ok.
	 */
	printf("==%d== Test 3: Calling fd_close_partial(15, ...)\n", PID);
	fd_close_partial(15, fd1, fd2, fd3, fd4, fd5, fd6, fd7, fd8, fd9,
						 fd10, fd11, fd12, fd13, fd14, fd15);
	g_total_closed_spec += 15;
	printf("==%d== ...Test 3 Complete\n", PID);
	printf("==%d== \n", PID);

	/*
	 * --- Test 4: Final Cleanup ---
	 * Only fd_ok should be left. fd_close_all() should close it.
	 */
	printf("==%d== Test 4: Final Cleanup: fd_close_all()\n", PID);
	fd_close_all(); 
	g_total_closed_spec += 1; // We expect fd_ok to be closed
	printf("==%d== ...All tests complete.\n", PID);
	printf("==%d== \n", PID);


	/*
	 * --- Final Summary (Based on SPEC) ---
	 */
	long fds_in_use_spec = g_total_registered_spec - g_total_closed_spec;
	fds_in_use_spec += g_leaked_by_test;

	printf("==%d== FILE DESCRIPTOR SUMMARY (Based on Spec):\n", PID);
	printf("==%d==    in use at exit: %ld open file descriptors\n", 
		PID, fds_in_use_spec);
	printf("==%d==  total fd usage: %ld registered, %ld closed\n",
		PID, g_total_registered_spec, g_total_closed_spec);
	printf("==%d== \n", PID);
	
	if (fds_in_use_spec > 0) {
		printf("==%d== LEAK SUMMARY (Based on Spec):\n", PID);
		printf("==%d==    definitely lost: %d file descriptors\n", 
			PID, g_leaked_by_test);
	} else {
		printf("==%d== All file descriptors were closed -- no leaks are possible\n", PID);
	}

	return 0;
}
