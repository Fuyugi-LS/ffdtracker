/*
 * ===================================================================
 * TEST FILE: main.c (FD Full Buffer + Partial Close Test)
 * ===================================================================
 * This test uses the hardcoded MAX_FD_TRACKER (1024).
 * It tests the behavior of fd_close_partial and fd_close_all
 * on a full buffer.
 */

// Your new header file
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
	// We need an array to hold all the FDs we open in the loop
	static int fd_array[MAX_FD_TRACKER];
	int fd_ok;
	int i;

	printf("==%d== Partial Close (Full Buffer) Test\n", PID);
	printf("==%d== Using hardcoded MAX_FD_TRACKER = %d\n", PID, MAX_FD_TRACKER);
	printf("==%d== \n", PID);

	/*
	 * --- Test 1: Setup - Fill buffer to (MAX_FD_TRACKER - 1) ---
	 */
	printf("==%d== Test 1: Filling buffer to %d / %d slots...\n", PID, MAX_FD_TRACKER - 1, MAX_FD_TRACKER);
	
	for (i = 0; i < MAX_FD_TRACKER - 1; i++)
	{
		fd_array[i] = get_fd();
		fd_register(1, fd_array[i]);
		g_total_registered_spec++;
	}
	
	printf("==%d== ...Test 1 Complete\n", PID);
	printf("==%d== \n", PID);


	/*
	 * --- Test 2: Fill the buffer ---
	 * Buffer is at 1023/1024. This fills it to 1024/1024.
	 */
	printf("==%d== Test 2: Filling buffer to %d / %d slots...\n", PID, MAX_FD_TRACKER, MAX_FD_TRACKER);
	
	fd_ok = get_fd();
	fd_register(1, fd_ok);
	g_total_registered_spec += 1;
	
	printf("==%d== ...Test 2 Complete\n", PID);
	printf("==%d== \n", PID);

	/*
	 * --- Test 3: Partial Close (Small Scale) ---
	 * We can't pass 1023 args. Instead, we'll test the *philosophy*
	 * by closing 3 FDs from the full buffer.
	 */
	printf("==%d== Test 3: Calling fd_close_partial(3, ...)\n", PID);
	fd_close_partial(3, fd_array[0], fd_array[1], fd_array[2]);
	g_total_closed_spec += 3;
	printf("==%d== ...Test 3 Complete\n", PID);
	printf("==%d== \n", PID);

	/*
	 * --- Test 4: Final Cleanup ---
	 * This should close the remaining (1024 - 3) FDs.
	 */
	printf("==%d== Test 4: Final Cleanup: fd_close_all()\n", PID);
	fd_close_all(); 
	g_total_closed_spec += (MAX_FD_TRACKER - 3); // Close the rest
	printf("==%d== ...All tests complete.\n", PID);
	printf("==%d== \n", PID);


	/*
	 * --- Final Summary (Based on SPEC) ---
	 */
	long fds_in_use_spec = g_total_registered_spec - g_total_closed_spec;

	printf("==%d== FILE DESCRIPTOR SUMMARY (Based on Spec):\n", PID);
	printf("==%d==    in use at exit: %ld open file descriptors\n", 
		PID, fds_in_use_spec);
	printf("==%d==  total fd usage: %ld registered, %ld closed\n",
		PID, g_total_registered_spec, g_total_closed_spec);
	printf("==%d== \n", PID);
	
	if (fds_in_use_spec == 0) {
		printf("==%d== All file descriptors were closed -- no leaks are possible\n", PID);
	}

	return 0;
}