A=arch
A==ARCH_X86_64?next:dead
A=sys_number
A==0xdeadbeef?go:dead

dead:
    return KILL
go:
    return ERRNO(65536)

# max: 65535
