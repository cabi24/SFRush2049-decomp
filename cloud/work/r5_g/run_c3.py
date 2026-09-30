import sys
sys.path.insert(0,'/home/user/SFRush2049-decomp/cloud/work/r5_g')
from autoclimb import *
from c3try import extract,BASE
txt=open(R+'c3_b1.c').read()
def getbase(t): return BASE.replace(extract(BASE,'func_800C3AD0'),t)
run('func_800C3AD0',R+'clb_c3auto',txt,getbase,int(sys.argv[1]),int(sys.argv[2]),'c3')
