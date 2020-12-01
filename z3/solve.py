from z3 import *

def str2list(String):
    tmp= String.replace("\n"," ").split(" ")
    return [int(i,16) for i in (tmp)]

s = [BitVec('flag[%d]'%i,8) for i in range(39)]
solver = Solver()

for i,c in enumerate('TWCTF{'):
    solver.add(s[i] == ord(c))
solver.add(s[38]==ord("}"))
solver.add(s[37] == 0x35)
solver.add(s[7] == 0x66)
solver.add(s[11] == 0x38 )
solver.add(s[12] == 0x37 )
solver.add(s[23] == 0x32 )
solver.add(s[31] == 0x34)

code="""00000080 00000080 000000FF 00000080 000000FF 000000FF 000000FF 000000FF 00000080 000000FF 000000FF 00000080 00000080 000000FF 000000FF 00000080
000000FF 000000FF 00000080 000000FF 00000080 00000080 000000FF 000000FF 000000FF 000000FF 00000080 000000FF 000000FF 000000FF 00000080 000000FF"""
code=code.replace("\n"," ").split(" ")
code= [int(i,16) for i in (code)]

for i in range(32):
    if code[i]!=0x80:
        solver.add(s[i+6]>=ord('0'))
        solver.add(s[i+6]<=ord('9'))
    else:
        solver.add(s[i+6]>=0x61)
        solver.add(s[i+6]<=0x66)

code="""0000015E 000000DA 0000012F 00000131 00000100 00000131 000000FB 00000102"""
code=str2list(code)
for i in range(8):
    solver.add(s[i*4+6]+s[i*4+7]+s[i*4+8]+s[i*4+9]==code[i])

code="""00000052 0000000C 00000001 0000000F 0000005C 00000005 00000053 00000058"""
code=str2list(code)
for i in range(8):
    solver.add(s[i*4+6]^s[i*4+7]^s[i*4+8]^s[i*4+9]==code[i])

code="""00000129 00000103 0000012B 00000131 00000135 0000010B 000000FF 000000FF"""
code=str2list(code)
for i in range(8):
    solver.add(s[6+i]+s[6+i+8]+s[6+i+16]+s[6+i+24]==code[i])

code="""00000001 00000057 00000007 0000000D 0000000D 00000053 00000051 00000051"""
code=str2list(code)
for i in range(8):
    solver.add(s[6+i]^s[6+i+8]^s[6+i+16]^s[6+i+24]==code[i])

code="""00000003 00000002 00000002 00000000 00000003 00000002 00000001 00000003
00000003 00000001 00000001 00000003 00000001 00000002 00000002 00000003"""
code=str2list(code)

while True:
    flag=[]
    check=solver.check()
    if check==sat:
        m= solver.model()
        for i in s:
            flag.append(str(m[i]))
        res=''
        for i in flag:
            if i == "None":
                res+='*'
            else:
                res+=chr(int(i))
        for i,c in enumerate("0123456789abcdef"):
            if res.count(c,6,6+32)!=code[i]:
                print res
                break
        else:
            print 'OK'
            print res
            exit(0)
        solver.add(Not(And([s[i]==ord(res[i]) for i in range(6,6+32)])))
    else:
        print check
