#include <iostream>
#include <windows.h>

LPVOID ptr[0x100];
LPVOID ptr1[0x100];
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
void log(int size) {
    for (int i = 0; i < size; i++) {
        if (ptr[i]) {
            printf("idx: %d --> %p\n", i, ptr[i]);
        }
    }
    putchar('\n');
}

void diff(int size) {
    for (int i = 0; i < size; i++) {
        if (ptr[i]) {
            printf("idx: %d --> new: %p\t\tvs old: %p\n", i, ptr[i], ptr1[i]);
        }
    }
    putchar('\n');
}

void bakup2ptr1() {
    memcpy_s((void*)&ptr1, sizeof(ptr1), (void *)&ptr, sizeof(ptr));
}
/*
LFH:
    Size < 0x4000
    NTheap LFH: HeapBase(_HEAP)->FrontEndHeap->LocalData->SegmentInfo[]->Hint,ActiveSubsegment,CachedItems->AggregateExchg :: Depth
    segmentheap LFH: HeapBase(_SEGMENT_HEAP)->LfhContext.Buckets->AffinitySlots[]->State->*SubsegmentList(_HEAP_LFH_SUBSEGMENT)::*Count
*/
int main() {
    init();

    getchar();
    for (int i = 0; i < 16; i++) {
        ptr[i] = Alloc(0x150); // enable LFH(0x150)
    }
    for (int i = 16; i < 32; i++) {
        ptr[i] = Alloc(0x150); // LFH alloc
    }
    bakup2ptr1();
    log(0x20);

    getchar();

    for (int i = 16; i < 32; i++)
    {
        Free(ptr[i]);
        // 释放 LFH分配器分配出来的chunk
        ptr[i] = Alloc(0x150); 
        // 从结果上来看(分析新/老的16-32), 有一定的可能分配出上面的Free的LFH分配器分配的chunk
        //      看起来就是暴力怼LFH
        // 想优化结果, 可以去heap中查看LFH剩余的chunk数目
    }
    diff(0x20);
    getchar();

    bakup2ptr1();
    for (int i = 0; i < 16; i++) {
        Free(ptr[i]); // 去释放前16个, 这些不是LFH分配器分配出来的, 应该不会放回LFH中, 而是会给正常的使用
        ptr[i] = 0;
    }

    for (int i = 0; i < 16; i++)
    {
        ptr[i] = Alloc(0x140); // 这里分配了一个小的size, 看样子是对之前free的进行了切割
    }
    diff(0x20); // 这里分析新/老的0-16, 发现貌似不走LFH
    getchar();

	return 0;
}