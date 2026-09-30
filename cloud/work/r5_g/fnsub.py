import re,sys
sys.path.insert(0,'/home/user/SFRush2049-decomp/cloud/work/r5_g')
from gscore import *
GD=ROOT+'/cloud/work/r5_g/grp1'
ORIG=open(ROOT+'/cloud/work/ipa-groups/func_800AD4C8/group.c').read()
def extract(name):
    m=re.search(r'^s16 '+name+r'\([^;\n]*\{\n.*?\n\}\n',ORIG,re.S|re.M)
    return m.group(0)
def put(newbody,name):
    s=ORIG.replace(extract(name),newbody)
    open(GD+'/group.c','w').write(s)
