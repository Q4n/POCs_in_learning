#include<stdio.h>
#include<stdlib.h>


int main(){
    free(malloc(0x40));
    free(malloc(0x50));
    free(malloc(0x400));
    fprintf(stderr,"%p\n",*(size_t*)malloc(0x40));
}
