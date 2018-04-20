# OS kernel _by @yannbolliger_

This is a coursework project that implements a very basic operating systems
kernel for an ARMv7 architecture (Cortex-A8). The OS can be run by emulation
with QEMU.

## Features

- [concurrent process execution](./kernel/sched.h) with a [BFS
  scheduler](./kernel/sched_bfs.h)
- [virtual memory](./kernel/page.h) for the processes making use of the MMU of
  the architecture
- POSIX compliant subset of `fork` and `pipe` [syscalls](./user/libc.h)

The last point leads to a nice property: the user-land program
[`philosophers`](./user/philosophers.h) implements a solution to the _dining
philosophers_ problem. As this kernel by conforms to a minimal subset of POSIX,
the program compiles and runs on POSIX as well as on this kernel.

## Doc

The system's main documentation is in [`kernel/kernel.h`](./kernel/kernel.h).

In the folder `/test` there are basic unit tests that compile and run on
x86_64. These tests are used as regression tests for the most basic and not
too tightly hardware-coupled parts of the kernel (for example the run queue).

## Sources

- The BFS scheduler was inspired by the BFS scheduler for Linux by Con Kolivas:
  http://ck.kolivas.org/patches/bfs/bfs-faq.txt
  https://tampub.uta.fi/bitstream/handle/10024/96864/GRADU-1428493916.pdf

- POSIX documentation for <limits.h>, <unistd.h>:
  http://pubs.opengroup.org/onlinepubs/9699919799/

- The Linux kernel inspired the bitmaps used in "mem.c":
  https://github.com/torvalds/linux/blob/master/include/linux/bitmap.h

- Manpages section 2,3 for the interface of various system calls: read, pipe...
