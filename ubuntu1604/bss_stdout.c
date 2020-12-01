#include<stdio.h>

char bss[0x100];


void backdoor(){
    system("/bin/sh");
}

int main(){
    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);
    void *stdout1=bss-0x40;
    stdout1 = malloc(0x1000);
    void *pad=malloc(0x60);

    *(size_t*)(stdout1+0xd8)=pad;
    *(size_t*)(pad+0x38)=backdoor;
    printf("aaa%d\n");
    // 改bss上确实不行
}