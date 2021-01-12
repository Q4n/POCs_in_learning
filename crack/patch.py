import re
import sys

def patch1(text:bytes):
    pattern1 = 'c8 00 00 00 74 2a 83 bc 24 08'
    print("[*] try to find ", pattern1)
    pattern1 = bytes.fromhex(pattern1)
    try:
        idx = text.index(pattern1)
    except Exception:
        print("[-] can't find pattern1-0")
    result = text[:idx+3] + b'\xeb' + text[idx+5:]
    try:
        idx = text.index(pattern1)
    except Exception:
        print("[-] can't find pattern1-1")
    result = text[:idx+3] + b'\xeb' + text[idx+5:]
    print("[+] patch1 done")
    return result
    
def patch2(text:bytes):
    pattern2 = '83 C4 18 B9 09 00 00 00 8B F0 F3 A5'
    print("[*] try to find ", pattern2)
    pattern2 = bytes.fromhex(pattern2)
    try:
        idx = text.index(pattern2)
    except Exception:
        print("[-] can't find pattern2")   
        
    result = text[:idx+9] + b'\x90\x90' + text[idx+12:]
    print("[+] patch2 done")
    return result
    
def patchfile():
    f = open("C:\\ProgramData\\Source Insight\\4.0\\si4.lic","r")
    text =  f.read()
    text = re.sub('Expiration=\".*?"',"Expiration=\"2099-12-31\"",text)
    f.close()
    with open("C:\\ProgramData\\Source Insight\\4.0\\si4.lic","w") as f:
        f.write(text)
    print("[+] patchfile done")

if __name__ == '__main__':
    print("Usage: \n\t1. 打开一次source insight4, 并且选择试用30天\n\t2. 接着关闭source insight4运行本软件\n\t")
    print("用法: main.py [path_to_sourceinsight4.exe]\n(大概)可以获得无限试用\n")
    t = input("press y to continue")
    if t != 'y':
        exit(0)
    if len(sys.argv) == 1:
        filename = 'sourceinsight4.exe'
    else:
        filename = sys.argv[1]
    
    text = None
    with open(filename,"rb") as f:
        text = f.read()
    
    text = patch1(text)
    text = patch2(text)
    patchfile()
    with open(filename, "wb") as f:
        f.write(text)
        