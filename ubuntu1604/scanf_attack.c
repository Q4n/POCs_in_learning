#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

char buf[0x1000];
char buf1[0x1000];


// 设置_IO_buf_base为write_s，_IO_buf_end为write_end（_IO_buf_end-_IO_buf_base要大于0）
// flag位不能含有4（_IO_NO_READS），_fileno要为0。（最好就直接使用原本的flag）
// 设置_IO_read_end等于_IO_read_ptr。


// in libc 2.23
int main(){
    _IO_FILE *ptr=(_IO_FILE*)stdin;
    fprintf(stderr,"ptr:0x%llx\n",ptr);
    
    memset(buf,(int)'a',0x10);
    ptr->_IO_buf_base=buf; //buf_begin
    ptr->_IO_buf_end=buf+0x110; //buf_end(size must > write size)
    ptr->_IO_read_end=ptr->_IO_read_ptr=ptr->_IO_read_base=buf;

    scanf("%1s",buf1); // 'hackme'+'a'*0x4a
    puts(buf); //'hackme'+'a'*0x4a
    return 0;
}

// 可以输入任意长度，不受fmt限制