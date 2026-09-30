import sys,itertools,subprocess,re
# usage: mut.py base.c fn  ; template with @@N@@ slots defined in base file as /*MUT k: a | b | c */ lines
src=open(sys.argv[1]).read(); fn=sys.argv[2]; fl=sys.argv[3] if len(sys.argv)>3 else '-O2'
slots=re.findall(r'/\*@(\d+): (.*?)\*/',src)
opts=[]
for k,v in slots: opts.append([x.strip('\n') for x in v.split('||')])
best=None
for combo in itertools.product(*opts):
    s=src
    for (k,_),c in zip(slots,combo):
        s=re.sub(r'/\*@%s: .*?\*/'%k,lambda m:c.replace('\\n','\n'),s,flags=re.S)
    open('/home/user/SFRush2049-decomp/cloud/work/newtargets_hi/mut_tmp.c','w').write(s)
    r=subprocess.run(['sh','/home/user/SFRush2049-decomp/cloud/work/newtargets_hi/s.sh','mut_tmp.c',fn,fl],capture_output=True,text=True)
    out=r.stdout+r.stderr
    if 'MATCH' in out and 'differ' not in out:
        print('MATCH',combo); open(sys.argv[1]+'.match.c','w').write(s); break
    m=re.search(r'(\d+)/\d+ words differ',out)
    n=int(m.group(1)) if m else 9999
    if best is None or n<best[0]: best=(n,combo); open(sys.argv[1]+'.best.c','w').write(s)
print(best)
