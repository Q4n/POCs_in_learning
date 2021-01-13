/* 
refer:
    https://billyotry.github.io/2020/10/05/%5BConfidence%20CTF%5Dkvm/#%E5%88%86%E9%85%8D%E5%86%85%E5%AD%98
    https://lwn.net/Articles/658511/
    https://github.com/dpw/kvm-hello-world/blob/master/kvm-hello-world.c

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <linux/kvm.h>
#define KVM_FILE "/dev/kvm"

// need root
int main(){
    struct kvm_sregs sregs;
    int ret;
    int kvmfd = open(KVM_FILE, O_RDWR | O_CLOEXEC ); 
    printf("%d\n",kvmfd);
    
    ioctl(kvmfd,KVM_GET_API_VERSION,NULL);
    int vmfd = ioctl(kvmfd, KVM_CREATE_VM, 0);

    unsigned char *ram = mmap(NULL, 0x1000, PROT_READ| PROT_WRITE, MAP_SHARED| MAP_ANONYMOUS, -1, 0);

    // as -32 xx.c -o xx.o
    // objcopy -O binary xx.o xx.bin
    int kfd = open("test.bin",O_RDONLY);
    read(kfd,ram,4096);

    struct kvm_userspace_memory_region mem = {
        .slot = 0,
        .guest_phys_addr = 0,
        .memory_size = 0x1000,
        .userspace_addr = (unsigned long) ram,
    };

    ret = ioctl(vmfd,KVM_SET_USER_MEMORY_REGION, &mem );
    printf("%d\n",ret);

    int vcpufd = ioctl(vmfd, KVM_CREATE_VCPU, 0);

    int mmap_size = ioctl(kvmfd, KVM_GET_VCPU_MMAP_SIZE,NULL);

    struct kvm_run *run = mmap(NULL,mmap_size,PROT_READ|PROT_WRITE, MAP_SHARED, vcpufd, 0);
    ret = ioctl(vcpufd,KVM_GET_SREGS,&sregs);
    printf("%d\n",ret);

    sregs.cs.base = 0;
    sregs.cs.selector = 0;
    ret = ioctl(vcpufd,KVM_SET_SREGS,&sregs);
    printf("%d\n",ret);

    struct kvm_regs regs = {
        .rip=0,
    };
    ret = ioctl(vcpufd, KVM_SET_REGS,&regs);
    printf("%d\n",ret);
    while (1)
    {
        ret = ioctl(vcpufd,KVM_RUN, NULL);
        if (ret == -1 ){
            puts("Exit unk");
            return -1;
        }
        switch(run->exit_reason){
            case KVM_EXIT_HLT:
                puts("hlt");
                return 0;
            case KVM_EXIT_IO:
                puts("io");
                putchar(*(((char *)run)+run->io.data_offset));
                break;
            case KVM_EXIT_FAIL_ENTRY:
                puts("entry");
                return -1;
            default:
                puts("oth");
                printf("Exit reason: %d\n",run->exit_reason);
            return -1;
        }

    }
    
}