import subprocess,re,sys,itertools
G='/home/user/SFRush2049-decomp/cloud/work/r5_g/hbgrp/'
def score(src, member='func_800AC9BC'):
    open(G+'group.c','w').write(src)
    r=subprocess.run(['python3','cloud/work/tools/zbuild.py',G[:-1],'--as1=-r4300_mul'],cwd='/home/user/SFRush2049-decomp',capture_output=True,text=True).stdout
    for l in r.splitlines():
        if l.strip().startswith(member):
            m=re.search(r'(MATCH|(\d+)/(\d+) words differ)',l)
            return l.strip()
    return r[-300:]
