// refer: https://ctftime.org/writeup/21116

int main() {
    __asm__ (
        "call here;"
        ".incbin \"./secret\";" // include file into code
        "here:"
        "pop %rsi;"
        "xorq %rdi,%rdi;"
        "movq $0x200,%rdx;"
        "movq $1,%rax;"
        "syscall;" // call write(1,file,0x200);
    );
    return 0;
}