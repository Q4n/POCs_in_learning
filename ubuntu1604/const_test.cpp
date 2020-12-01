/*************************************************************************
	> File Name: const_test.cpp
	> Author: 
	> Mail: 
	> Created Time: 2020年11月09日 星期一 15时43分16秒
 ************************************************************************/

#include<iostream>
using namespace std;

#include <stdio.h>

int main(){
    const int a = 10;
    int *p = const_cast<int*>(&a);
    *p = 20;
    printf("a:%d p:%d\n",a,*p);
    return 0;
}
