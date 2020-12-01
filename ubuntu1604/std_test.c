#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
    close(0);
    close(1);
    close(2);

    // open("/dev/tty",O_RDONLY); // open stdin
    // open("/dev/tty",O_RDWR); // 顺序打开
    // open("/dev/tty",O_WRONLY); // open stdout

    // fopen("/dev/tty","r"); // open stdin
    // fopen("/dev/tty","a"); // open stdout
    // fopen("/dev/tty","a+"); // 顺序打开
    // fopen("/dev/tty","w"); // open stdout

    system("sh");
    // ls >&0 直接重定向到stdin

    return 0;
}
