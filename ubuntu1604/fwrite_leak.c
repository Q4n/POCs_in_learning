#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

char buf[0x1000]="secret";
char buf1[0x1000]="aaaaaaaaaaa";


// in libc 2.23
int main(){
    _IO_FILE *ptr=(_IO_FILE*)stdout;
    fprintf(stderr,"ptr:0x%llx\n",ptr);
    
    ptr->_flags=0xfbad3887;
    ptr->_IO_write_end=ptr->_IO_write_ptr=buf+0x100;
    ptr->_IO_write_base=buf;

    fwrite(buf1,0x5,1,stdout); //类似与华哥的办法啦

    return 0;
}
