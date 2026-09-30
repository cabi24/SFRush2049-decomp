import sys,subprocess,re,os
# multi.py hdrfile fn variants.txt [flags]; variants separated by lines '----'; header + variant body
hdr=open(sys.argv[1]).read(); fn=sys.argv[2]; vs=open(sys.argv[3]).read().split('\n----\n'); fl=sys.argv[4] if len(sys.argv)>4 else '-O2'
tmp=os.path.abspath(sys.argv[3])+'.tmp.c'; res=[]
for i,v in enumerate(vs):
    open(tmp,'w').write(hdr+'\n'+v)
    r=subprocess.run(['python3','tools/cloud/score.py','fn',tmp,fn,'--flags','-g0 %s -mips2 -G 0 -non_shared'%fl],capture_output=True,text=True,cwd='/home/user/SFRush2049-decomp')
    out=r.stdout+r.stderr
    if 'MATCH' in out and 'differ' not in out: print('MATCH variant',i); open(sys.argv[3]+'.match%d.c'%i,'w').write(hdr+'\n'+v); continue
    m=re.search(r'(\d+)/\d+ words differ',out); res.append((int(m.group(1)) if m else 9999,i))
print(sorted(res))
