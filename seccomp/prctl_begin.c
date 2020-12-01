/*************************************************************************
	> File Name: prctl_begin.c
	> Author: 
	> Mail: 
	> Created Time: 2019年09月14日 星期六 16时13分54秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/prctl.h>

int main(){
    
    prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0);
    char filename[]="/bin/sh";
    char *argv[]={"/bin/sh",NULL};
    char *envp[]={NULL};
    write(1,"give u a shell\n",16);
    syscall(0x3b,filename,argv,envp);
    return 0;
}
