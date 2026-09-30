import sys, itertools, subprocess, re, shutil, os
sys.path.insert(0,'.'); import gen
def score(o, tag='t'):
    d=f'/tmp/claude-0/r5a_{tag}'; os.makedirs(d,exist_ok=True)
    shutil.copy('g05c/f.c',d+'/f.c'); open(d+'/r.c','w').write(gen.gen(o)); shutil.copy('g05c/group.json',d+'/group.json')
    p=subprocess.run(['python3','/home/user/SFRush2049-decomp/cloud/work/tools/extscore.py',d,'--only','func_80086A50'],capture_output=True,text=True,cwd='/home/user/SFRush2049-decomp')
    m=re.search(r'(\d+)/387 words differ',p.stdout); 
    return int(m.group(1)) if m else 999
if __name__=='__main__':
    roles=['c','a','b']
    best=None
    for combo in itertools.product([0,1],repeat=3):
        o=dict(zip(roles,combo)); o['d_4']=0
        print(combo, score(o),flush=True)
