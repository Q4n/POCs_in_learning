#include<stdio.h>
#include<unistd.h>

// int main(){
//     puts("dada");
//     int pid=fork();
//     if(pid<0){
//         puts("Error!");
//     }else if(pid==0)
//     {
//         puts("son");
//     }
//     else
//     {
//         puts("dad!");
//     }
//     return 0;
    
// }

void son(){
    sleep(5);
    puts("i'm son");
    exit(2);
}

int main(){
    if(!fork()){
        son();
    }
    else{
        return 1;
    }
    // puts("i'm dad");
}