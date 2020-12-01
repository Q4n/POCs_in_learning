/*************************************************************************
	> File Name: parmlimit_seccomp.c
	> Author: 
	> Mail: 
	> Created Time: 2019年09月14日 星期六 10时39分13秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<seccomp.h>
#include<linux/seccomp.h>

int main(){
    scmp_filter_ctx ctx=seccomp_init(SCMP_ACT_ALLOW);
    seccomp_rule_add(ctx,SCMP_ACT_KILL,SCMP_SYS(write),1,SCMP_A2(SCMP_CMP_EQ,0X10));
    seccomp_load(ctx);
    
    write(1,"i love seccomp!\n",0x20);
    write(1,"it is easy\n",0x10);
    return 0;
}
