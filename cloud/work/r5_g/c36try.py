import subprocess,re,sys
ROOT='/home/user/SFRush2049-decomp'
G=ROOT+'/cloud/work/r5_g/c36'
S0=open(G+'/group.c').read()
def sc(s):
    open(G+'/group.c','w').write(s)
    r=subprocess.run(['python3',ROOT+'/cloud/work/r5_g/gsbs.py',G,'func_800C36A0','--as1=-r4300_mul','--hi','9999'],capture_output=True,text=True).stdout
    m=re.search(r'aligned differing rows: (\d+) size (\d+)',r)
    fr=re.search(r'\|\s+\d+ addiu sp,sp,-(\d+)',r)
    return (int(m.group(1)),int(m.group(2)),int(fr.group(1)) if fr else None) if m else r[-300:]
def restore(): open(G+'/group.c','w').write(S0)
