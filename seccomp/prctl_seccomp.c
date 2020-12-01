/*************************************************************************
	> File Name: prctl_seccomp.c
	> Author: 
	> Mail: 
	> Created Time: 2019年09月14日 星期六 16时19分46秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/prctl.h>
#include<linux/seccomp.h>
#include<linux/filter.h>

int main(){
    struct sock_filter sfi[]={
       {0x20,0x00 ,0x00 ,0x00000004},
       {0x15,0x00 ,0x05 ,0xc000003e},
       {0x20,0x00 ,0x00 ,0x00000000},
       {0x35,0x00 ,0x01 ,0x40000000},
       {0x15,0x00 ,0x02 ,0xffffffff},
       {0x15,0x01 ,0x00 ,0x0000003b},
       {0x06,0x00 ,0x00 ,0x7fff0000},
       {0x06,0x00 ,0x00 ,0x00000000},
    };

    struct sock_fprog sfp={8,sfi};
   // prctl(PR_SET_SECCOMP,SECCOMP_MODE_STRICT); 
    prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0);  //注意，一定要这一句接下来的才会生效
    prctl(PR_SET_SECCOMP,SECCOMP_MODE_FILTER,&sfp);

    char filename[]="/bin/sh";
    char *argv[]={"/bin/sh",NULL};
    char *envp[]={NULL};
    write(1,"give u a shell\n",16);
    syscall(0x000003b,filename,argv,envp);
    return 0;
}
