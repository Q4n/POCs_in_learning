reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\segmentheap_attack.exe" /v FrontEndHeapDebugOptions /t REG_DWORD /d 0x8 /f
pause

