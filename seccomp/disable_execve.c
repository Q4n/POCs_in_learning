#include <linux/seccomp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>

static void install_seccomp() {
 // static unsigned char filter[] = {32,0,0,0,4,0,0,0,21,0,0,3,62,0,0,192,32,0,0,0,0,0,0,0,21,0,1,0,59,0,0,0,6,0,0,0,0,0,255,127,6,0,0,0,0,0,0,0};
    unsigned char filter[] = {32,0,0,0,4,0,0,0,21,0,0,3,62,0,0,192,32,0,0,0,0,0,0,0,21,0,2,0,59,0,0,0,6,0,0,0,0,0,255,127,6,0,0,0,0,0,0,0,6,0,0,0,0,0,5,0};

  struct prog {
    unsigned short len;
    unsigned char *filter;
  } rule = {
    .len = sizeof(filter) >> 3,
    .filter = filter
  };
  if(prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0) { perror("prctl(PR_SET_NO_NEW_PRIVS)"); exit(2); }
  if(prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &rule) < 0) { perror("prctl(PR_SET_SECCOMP)"); exit(2); }
}



int main(){
    install_seccomp();
    write(1,"give u a shell\n",16);
    int res=system("sh");
    write(1,"here?\n",6);

    printf("res: 0x%hhx\n",res);
}
