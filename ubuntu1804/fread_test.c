#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char ubuf[0x100];
    puts("here i'm, u must read 0x100");
    size_t res =fread(ubuf,1,0x100,stdin);
    if (!res){
        ferror()
    }
    printf("len: %ld\nubuf: %s",strlen(ubuf),ubuf);
    getchar();
    return 0;
}
// fread must read 0x100 :(
