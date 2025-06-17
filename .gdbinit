set height unlimited
set pagination off
set confirm off

set trace-commands on
set print inferior-events on

set sysroot /

directory ~/staging/glibc-2.31
directory ~/staging/glibc-2.31/elf
directory ~/staging/glibc-2.31/intl
directory ~/staging/glibc-2.31/nptl
directory ~/staging/glibc-2.31/dlfcn
directory ~/staging/glibc-2.31/misc
directory ~/staging/glibc-2.31/csu
directory ~/staging/glibc-2.31/ctype
directory ~/staging/glibc-2.31/stdlib
directory ~/staging/glibc-2.31/string
directory ~/staging/glibc-2.31/sysdeps
directory ~/staging/glibc-2.31/libio
directory ~/staging/glibc-2.31/stdio-common
directory ~/staging/glibc-2.31/sysdeps/x86_64/multiarch
directory ~/staging/gcc-9-9.4.0/src/libsanitizer/sanitizer_common
directory ~/staging/gcc-9-9.4.0/src/libsanitizer/asan
directory ~/staging/gcc-9-9.4.0/src/libsanitizer/ubsan

br main

#define hook-next
#  refresh
#end

set mem inaccessible-by-default off
#set follow-fork-mode parent
set follow-fork-mode child
set follow-exec-mode new
#set follow-exec-mode same
set detach-on-fork off

set detach-on-fork off
set schedule-multiple on
set follow-fork-mode parent
set non-stop on
set target-async on

#catch fork

#Links:
#  - https://sourceware.org/gdb/onlinedocs/gdb/Forks.html
#  - https://sourceware.org/gdb/onlinedocs/gdb/Inferiors-and-Programs.html
