
import sys

def transfer_va2pa(va = 0x1000):
    offset = []
    t = va & 0b111111111111
    offset.append(t)
    va = va>>12
    while va:
        offset.append(va&0b111111111)
        va = va >>9
    if len(offset)!=5:
        for i in range(5-len(offset)):
            offset.append(0)
    print('offset: ', offset[::-1])

if __name__ == "__main__":
    if len(sys.argv) == 2:
        transfer_va2pa(int(sys.argv[1],16))
    else:
        print("[err] Usage: python va2pa.py 0x1000")
