import sys,itertools,subprocess,re,os
# mut2.py base.c fn [flags]; slots /*@N: a || b */ ; prints best; writes base.c.best.c / .match.c
src=open(sys.argv[1]).read(); fn=sys.argv[2]; fl=sys.argv[3] if len(sys.argv)>3 else '-O2'
slots=re.findall(r'/\*@(\d+): (.*?)\*/',src,flags=re.S)
opts=[[x.strip('\n') for x in v.split('||')] for k,v in slots]
best=None; tmp=os.path.abspath(sys.argv[1])+'.tmp.c'
for combo in itertools.product(*opts):
    s=src
    for (k,_),c in zip(slots,combo):
        s=re.sub(r'/\*@%s: .*?\*/'%k,lambda m:c.strip().replace('\\n','\n'),s,count=1,flags=re.S)
    open(tmp,'w').write(s)
    r=subprocess.run(['python3','tools/cloud/score.py','fn',tmp,fn,'--flags','-g0 %s -mips2 -G 0 -non_shared'%fl],capture_output=True,text=True,cwd='/home/user/SFRush2049-decomp')
    out=r.stdout+r.stderr
    if 'MATCH' in out and 'differ' not in out:
        print('MATCH',combo); open(sys.argv[1]+'.match.c','w').write(s); sys.exit()
    m=re.search(r'(\d+)/\d+ words differ',out)
    n=int(m.group(1)) if m else 9999
    open(sys.argv[1]+'.log','a').write('%d %s\n'%(n,combo))
    if best is None or n<best[0]: best=(n,combo); open(sys.argv[1]+'.best.c','w').write(s)
print(best)
