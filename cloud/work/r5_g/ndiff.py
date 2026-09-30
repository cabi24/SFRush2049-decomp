import sys,re,subprocess
ROOT='/home/user/SFRush2049-decomp'
gd,fn=sys.argv[1],sys.argv[2]
r=subprocess.run(['python3',ROOT+'/cloud/work/r5_g/gsbs.py',gd,fn,'--as1=-r4300_mul','--hi','9999'],capture_output=True,text=True).stdout.splitlines()
print(r[0]); bad=0
for l in r[1:]:
    if '|' not in l: continue
    a,b=l.split('|',1)
    a=re.sub(r'^\S\s+\S+\s+','',a).strip(); b=re.sub(r'^\s*\S+\s+','',b).strip()
    na=re.sub(r'\$f\d+','$fX',a); nb=re.sub(r'\$f\d+','$fX',b)
    if na!=nb:
        bad+=1; print(l)
print('nonfloat-diff rows',bad)
