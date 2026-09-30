import sys,json,glob,os,re
sys.path.insert(0,'tools/cloud'); import score
syms={k:int(v,16) for k,v in json.load(open('asm/us/blob/symbols.json'))['symbols'].items()}
T=score.targets()
done=set()
for f in glob.glob('src/blob/*.c')+glob.glob('cloud/matches/*')+glob.glob('src/blob/groups/*/*.c'):
    done.add(os.path.basename(f).rsplit('.',1)[0])
    if 'groups' in f:
        done|=set(re.findall(r'\b([A-Za-z_]\w*)\s*\(',open(f,errors='ignore').read()))
for d in glob.glob('cloud/work/near-miss/*'): done.add(os.path.basename(d))
for f in glob.glob('cloud/work/ipa-groups/*/group.json'):
    j=json.load(open(f))
    for k in ('members','claims','context'):
        v=j.get(k,[]); done|=set(v if isinstance(v,list) else v.keys())
out=[]
for n,w in T.items():
    a=syms.get(n)
    if a and 0x800C0000<=a<0x800E8000 and n not in done and 3<=len(w)<=400: out.append((len(w),n,a))
for l,n,a in sorted(out): print(l,n,hex(a))
