/*
tested on windows 10 x64 19042.630 Release

lfh POC 

注意：
	attach 进行调试

refer: 
*/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

LPVOID ptr[0x20];

void init() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
}
LPVOID Alloc(SIZE_T cb) {
    return calloc(1, cb);
}
void Free(void* ptr) {
    return free(ptr);
}
void log() {
    for (int i = 0; i < 0x20; i++) {
        if (ptr[i]) {
            printf("idx: %d --> %p\n", i, ptr[i]);
        }
    }
    putchar('\n');
}
int main()
{
    init();

    for (int i = 0; i < 18; i++) {
        Alloc(0x200);
    }
    ptr[0] = Alloc(0x68); // lfh off
    ptr[1] = Alloc(0x68);
    ptr[2] = Alloc(0x68);
    ptr[6] = Alloc(0x200); // lfh on
    log();
    getchar();
    return 0;
}
