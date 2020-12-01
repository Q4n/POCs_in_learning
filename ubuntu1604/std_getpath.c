#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{      
    int ret;
    char *ttyname0,*ttyname1,*ttyname2;
     
    ttyname0 = ttyname(0);
    ttyname1 = ttyname(1);
    ttyname2 = ttyname(3);
   
    printf("file0 : %s\n",ttyname0);
    printf("file1 : %s\n",ttyname1);
    printf("file2 : %s\n",ttyname2);
   
    return 0;
}