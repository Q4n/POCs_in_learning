from z3 import *

box=[BitVec("flag[%d]"%i,8) for i in range(10)]
s=Solver()
s.add(box[1]+box[2]>=10)
s.add(box[1]+box[2]<=20)
s.add(box[2]!=14)
if s.check()==sat:
    print s.model()