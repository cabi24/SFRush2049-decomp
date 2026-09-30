import re,sys,json
sys.path.insert(0,'/home/user/SFRush2049-decomp/cloud/work/r5_g')
from gscore import *
GD=ROOT+'/cloud/work/r5_g/grp2'
BASE=open(ROOT+'/cloud/work/ipa-groups/func_800AD4C8/group.c').read()
def extract(S,name):
    m=re.search(r'^s16 '+name+r'\([^;\n]*\{\n.*?\n\}\n',S,re.S|re.M)
    return m.group(0)
def put(newbody,name,S=None):
    S=S or BASE
    open(GD+'/group.c','w').write(S.replace(extract(S,name),newbody))
def offs(fn):
    return [o for o in spoffs(GD,fn)]
def frame(fn='input_process_controller'):
    r=subprocess.run(['python3',ROOT+'/cloud/work/r5_g/gsbs.py',GD,fn,'--as1=-r4300_mul','--hi','6'],capture_output=True,text=True).stdout
    m=re.search(r'\|\s+\d+ addiu sp,sp,-(\d+)',r)
    return int(m.group(1)) if m else None
