#include <iostream>
#include <windows.h>

LPVOID ptr[0x100];
LPVOID ptr1[0x100];
HANDLE hHeap;

void init() {
    hHeap = HeapCreate(HEAP_GROWABLE, 0, 0); //HeapCreate(2u, 0i64, 0i64); ����һ����������
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
}

LPVOID Alloc(SIZE_T cb) {
    return HeapAlloc(hHeap, HEAP_ZERO_MEMORY, cb); // HeapAlloc(hHeap, 8u, cb); ������calloc
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
        // �ͷ� LFH���������������chunk
        ptr[i] = Alloc(0x150); 
        // �ӽ��������(������/�ϵ�16-32), ��һ���Ŀ��ܷ���������Free��LFH�����������chunk
        //      ���������Ǳ����LFH
        // ���Ż����, ����ȥheap�в鿴LFHʣ���chunk��Ŀ
    }
    diff(0x20);
    getchar();

    bakup2ptr1();
    for (int i = 0; i < 16; i++) {
        Free(ptr[i]); // ȥ�ͷ�ǰ16��, ��Щ����LFH���������������, Ӧ�ò���Ż�LFH��, ���ǻ��������ʹ��
        ptr[i] = 0;
    }

    for (int i = 0; i < 16; i++)
    {
        ptr[i] = Alloc(0x140); // ���������һ��С��size, �������Ƕ�֮ǰfree�Ľ������и�
    }
    diff(0x20); // ���������/�ϵ�0-16, ����ò�Ʋ���LFH
    getchar();

	return 0;
}