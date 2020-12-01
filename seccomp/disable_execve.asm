A=arch
A==ARCH_X86_64?next:dead
A=sys_number
A==execve?err:next

ok:
    return ALLOW
dead:
    return KILL
err:
    return ERRNO(0)
