import os,re,struct,subprocess,sys,json,collections
sys.path.insert(0,'.')
from tools.conveyor.pipeline.targets import scan_extent
J=os.environ['CLAUDE_JOB_DIR']+'/tmp/'
rom=open('baserom.us.z64','rb').read(); B=0x80000400; seg=rom[0x1000:0x2F4E0]
W=lambda a:struct.unpack_from('>I',seg,a-B)[0]
# tiles over the whole boot code (counted + uncounted) for completeness
def tiles(lo,hi):
    out=[];pc=lo
    while pc<hi:
        n=scan_extent(seg,pc,base=B)
        if not isinstance(n,int): break
        out.append((pc,n*4)); pc+=n*4
        while pc<hi and W(pc)==0: pc+=4
    return out
T=tiles(0x8000F3A4,0x80028000)
MASK={4:0xFC000000,5:0xFFFF0000,6:0xFFFF0000}
def funcs(obj):
    text=subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',obj,'/dev/stdout'],capture_output=True).stdout
    sy=subprocess.run(['mips-linux-gnu-readelf','-sW',obj],capture_output=True,text=True).stdout
    secs=subprocess.run(['mips-linux-gnu-readelf','-SW',obj],capture_output=True,text=True).stdout
    tidx=None
    for l in secs.splitlines():
        m=re.match(r'\s*\[\s*(\d+)\]\s+\.text\s',l)
        if m: tidx=m.group(1)
    fs=[]
    for l in sy.splitlines():
        p=l.split()
        if len(p)>=8 and p[3]=='FUNC' and p[6]==tidx: fs.append((int(p[1],16),int(p[2]),p[7]))
    rel=subprocess.run(['mips-linux-gnu-readelf','-rW',obj],capture_output=True,text=True).stdout
    relocs={}; cur=None
    for l in rel.splitlines():
        if l.startswith('Relocation section'): cur=('.rel.text' in l and '.rel.text.' not in l) or l.split("'")[1]=='.rel.text'; continue
        p=l.split()
        if cur and len(p)>=3 and re.fullmatch(r'[0-9a-f]{8}',p[0]):
            t={'R_MIPS_26':4,'R_MIPS_HI16':5,'R_MIPS_LO16':6}.get(p[2])
            if t: relocs[int(p[0],16)]=t
    fs.sort(); out=[]
    for i,(v,s,n) in enumerate(fs):
        end=v+s if s else (fs[i+1][0] if i+1<len(fs) else len(text))
        ws=[struct.unpack_from('>I',text,o)[0] for o in range(v,min(end,len(text)),4)]
        while ws and ws[-1]==0: ws.pop()
        out.append((n,ws,{(o-v)//4:t for o,t in relocs.items() if v<=o<end}))
    return out
res={}
for V in ('K','L','I-O1','I-O2','J-O1','J-O2','K-O1','K-O2'):
    lib=collections.defaultdict(list)
    for f in sorted(os.listdir(J+'ul/'+V)):
        for n,ws,rl in funcs(J+'ul/'+V+'/'+f):
            if ws: lib[len(ws)].append((n,f,ws,rl))
    hits={}
    for a,s in T:
        tw=[W(a+4*i) for i in range(s//4)]
        while tw and tw[-1]==0: tw.pop()
        for n,f,ws,rl in lib.get(len(tw),[]):
            if all((tw[i]&MASK.get(rl.get(i),0xFFFFFFFF))==(ws[i]&MASK.get(rl.get(i),0xFFFFFFFF)) for i in range(len(tw))):
                hits.setdefault(a,[]).append(n); 
    res[V]=hits
    print(V,'tiles',len(T),'identified',len(hits),'bytes',sum(s for a,s in T if a in hits))
both=set().union(*[set(h) for h in res.values()])
print('union identified',len(both),'bytes',sum(s for a,s in T if a in both),'of',sum(s for a,s in T))
json.dump({'tiles':[[hex(a),s] for a,s in T],**{V:{hex(a):v for a,v in h.items()} for V,h in res.items()}},open(J+'ulmatch.json','w'),indent=0)
