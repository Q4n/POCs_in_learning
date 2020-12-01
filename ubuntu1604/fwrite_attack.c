#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

char buf[0x1000]="secret";
char buf1[0x1000]="hacked";

// flag位： 不能包含0x8、0x800、0x1000（最好就直接使用原本的flag）
// 构造_fileno为1
// 构造_IO_write_base=write_s，_IO_write_ptr=write_e。


// in libc 2.23
int main(){
    _IO_FILE *ptr=(_IO_FILE*)stdout;
    fprintf(stderr,"ptr:0x%llx\n",ptr);
    
    ptr->_IO_write_ptr=buf;
    ptr->_IO_write_end=buf+0x20;

    fwrite(buf1,0x50,1,stdout); //任意地址写，将buf1的内容写入buf, 但是不会输出
    fprintf(stderr,"%s",buf); //hacked
    return 0;
}
