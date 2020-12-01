#include<stdio.h>

size_t user_cs,user_ss,user_sp,user_rflags;
int main(){
    // __asm__(
    //     "mov rdi,0x0068732f6e69622f;"
    //     "push rdi;"
    //     "mov rdi,rsp;"
    //     "xor rax,rax;"
    //     "mov al,0x3b;"
    //     "xor rsi,rsi;"
    //     "syscall;"
    // );
    
    __asm__("mov user_cs, cs;"
            "mov user_ss, ss;"
            "mov user_sp, rsp;"
            "pushf;"
            "pop user_rflags;"
            );
}