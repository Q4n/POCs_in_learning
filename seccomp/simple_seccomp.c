/*************************************************************************
	> File Name: simple_seccomp.c
	> Author: 
	> Mail: 
	> Created Time: 2019年09月14日 星期六 10时25分04秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<seccomp.h>
#include<linux/seccomp.h>
int main(){
    scmp_filter_ctx ctx=seccomp_init(SCMP_ACT_ALLOW);
    seccomp_rule_add(ctx,SCMP_ACT_KILL,SCMP_SYS(execve),0);
    seccomp_load(ctx);
    write(1,"I will give u a shell!\n",25);
    char filename[]="/bin/sh";
    char *argv[]={"/bin/sh",NULL};
    char *envp[]={NULL};
    syscall(0x3b,filename,argv,envp);
    return 0;
}
