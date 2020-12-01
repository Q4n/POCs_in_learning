#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main(){
    char s[0x100];
    memset(s,0,0x100);
    strcpy(s,"hello world");
    puts(s);
    setbuf(stdout,s);
    // puts("hack!");
    fwrite("hack",1,4,stdout);
    // puts("wdnmd");
    fwrite("wdnmd",1,5,stdout);
}