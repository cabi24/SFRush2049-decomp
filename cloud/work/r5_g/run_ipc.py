import sys,re
sys.path.insert(0,'/home/user/SFRush2049-decomp/cloud/work/r5_g')
from autoclimb import *
BASE=open(ROOT+'/cloud/work/ipa-groups/func_800AD4C8/group.c').read()
def extract(S,name):
    return re.search(r'^s16 '+name+r'\([^;\n]*\{\n.*?\n\}\n',S,re.S|re.M).group(0)
txt=open(R+sys.argv[3]).read()
def getbase(t): return BASE.replace(extract(BASE,'input_process_controller'),t)
run('input_process_controller',R+'clb_ipcauto'+sys.argv[1],txt,getbase,int(sys.argv[1]),int(sys.argv[2]),'ipc'+sys.argv[1])
