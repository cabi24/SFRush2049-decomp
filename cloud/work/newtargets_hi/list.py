import re,glob,json,os
R='/home/user/SFRush2049-decomp/'
sym=json.load(open(R+'asm/us/blob/symbols.json'))['symbols']
addr={k:int(v,16) for k,v in sym.items()}
done=set()
for f in glob.glob(R+'src/blob/**/*.c',recursive=True)+glob.glob(R+'cloud/matches/*.c'):
    done|=set(re.findall(r'^[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;{]*\)\s*\{',open(f).read(),re.M))
for d in glob.glob(R+'cloud/work/near-miss/*/'): done.add(os.path.basename(d.rstrip('/')))
for g in glob.glob(R+'cloud/work/ipa-groups/*/group.json'):
    j=json.load(open(g)); s=json.dumps(j); done|=set(re.findall(r'\w+',s))
out=[]
for f in glob.glob(R+'asm/us/blob/blob_*.s'):
    cur=None;n=0
    for l in open(f):
        m=re.match(r'\.section \.text\.(\w+)',l)
        if m:
            if cur: out.append((cur,n))
            cur=m.group(1);n=0
        elif l.strip().startswith('.word'): n+=1
    if cur: out.append((cur,n))
for name,n in sorted(out,key=lambda x:addr.get(x[0],0)):
    a=addr.get(name)
    if a and 0x800E8000<=a<=0x801249F0 and 20<=n<=200 and name not in done:
        print(name,hex(a),n)
