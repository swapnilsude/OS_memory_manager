# Memory Manager
The manager reads an input file which contains set of “malloc”, “free” and “print” commands. Each command in the file will be of format “<command id> <command>”. There are three types of commands: 
    malloc <size>
    free <command id>
    print
Refer test2.txt for example content.
The memory manager here implements worst fit algorithm. The memory manager is to be started with the command “mem_mgr<input  file><mem_pool_size>”.
    mem_pool_size is  the size  of memory available when the program starts.
The program can handle the comments (enclosed within /* */) in the <input file>. When a hole is created as a result of free command and the hole is adjacent to another hole then the two holes are coalesced  into  a  larger  hole. Memory manager assumes that the start address is 0.

### To create binary file file:
    make all
### To clean up the binary file:
    make clean
### To test:
    make test

### Usage:
    mem_mgr <input file> <mem_pool_size>

## Ongoing work:
Adding other memory allocation algorithms like First Fit, Best Fit
