<div align="center">
# ✨ libfdtracker ✨
A robust, lightweight, and Valgrind-approved file descriptor tracker in C.
<p>
<img src="https://img.shields.io/badge/Norme-42_Compliant-blue?style=flat&logo=42" alt="Norminette Compliant">
<img src="https://img.shields.io/badge/Valgrind-Clean-brightgreen?style=flat" alt="Valgrind Clean">
<img src="https://img.shields.io/badge/Language-C-purple?style=flat" alt="Language C">
<img src="https://img.shields.io/badge/License-MIT-lightblue?style=flat" alt="License MIT">
</p>
</div>

Tired of hunting down file descriptor leaks? libfdtracker is a simple, norminette-compliant static library that wraps your FD management in a robust tracking system. It's designed to be lightweight, fast, and, most importantly, 100% leak-free.

This project provides a simple API to register, close, and manage opened file descriptors, complete with robust error handling for edge cases like buffer overflows or trying to track too many FDs.

# 🔮 Core Features
- Leak-Proof: 100% clean reports from valgrind --track-fds=all.
- Norminette Compliant: Passes all norminette checks for clean, compliant C code.
- Lightweight & Fast: Designed with performance in mind. A simple, static array tracks all FDs.
- Granular Control: Includes functions to close all tracked FDs (fd_close_all) or only specific ones (fd_close_partial).
- Robust Error Handling: Built-in checks for buffer overflow (fd_perror) and invalid configuration.

⚙️ Installation & Usage
1. Build the Library  
A Makefile is included to compile all source files into a static library (.a file).
```
# Compile all .c files into .o files and archive them into the library
make
```
This will create libfdtracker.a in the root directory.

2. Link Against Your Project
```
# Example: Compiling a test program named 'basic.c'
# -Isrc tells the compiler where to find the headers
# -L. tells the compiler to look for libraries in the current directory
# -lfdtracker links the 'libfdtracker.a' library
cc -Wall -Wextra -Werror basic.c -Isrc -L. -lfdtracker -o my_program
```

3. Use in Your Code  
Include the memtracker.h header in your C files to use the tracker's functions.
```
 /* basic.c */
 #include "memtracker.h" // Your header (fdtracker.h)
 #include <fcntl.h>
 #include <sys/stat.h>

 int main(void)
 {
     // Open file descriptors
     int fd1 = open("/dev/null", O_RDONLY);
     int fd2 = open("/dev/null", O_RDONLY);

     // Register the FDs with the tracker
     // (2 FDs total)
     fd_register(2, fd1, fd2);

     // ... do work with your FDs ...

     // Clean up everything at the end
     fd_close_all();
     return (0);
 }
```

# 💎 API Reference

`void fd_register(size_t size, int fd, ...);`  
Registers one or more file descriptors to the memory tracker.  
@size: The total number of FDs to be registered.  
@fd: The first file descriptor to register.  
@...: The subsequent (size - 1) FDs to register.

`void fd_close_all(void);`
Closes all tracked file descriptors. It iterates through the entire tracker, calls close() on every valid FD, and clears its slot.

`void fd_close_partial(size_t size, int fd, ...);`  
Closes one or more specific file descriptors from the tracker.  
@size: The total number of FDs to be closed.  
@fd: The first FD to find and close.  
@...: The subsequent (size - 1) FDs to find and close.

# LICENSE
MIT License. For detailed information, read LICENSE.md
