"""Bounded GhostEnd model. Gameplay/free implementations are explicit stubs.

The independent model uses fixture records rather than decoding source pointers.
Only the finalizer instructions execute. Calls record their arguments and clobber
native caller-owned registers; allocator/handle mutations are outside this proof.
"""
import random,struct
ENTRY,STACK,RETURN=0x800D510C,0x81010000,0x81234560
LAST,MODE,BUSY,COUNT,TABLE,MODELS,CARS=0x80140800,0x8014A110,0x80114738,0x8014A108,0x80152698,0x8014A250,0x80152818
END,FREE,LOCK,UNLOCK,QUEUE=0x800CBE8C,0x80095FD8,0x80007270,0x800075E0,0x80152770
MASK=0xffffffff

def require(ok,msg):
    if not ok:raise RuntimeError(msg)
def signed(x,n=32):
    x&=(1<<n)-1
    return x-(1<<n) if x>>(n-1) else x

def load(m,a,n):
    require(a%n==0 and all(a+i in m for i in range(n)),f'invalid read {a:x}/{n}')
    return int.from_bytes(bytes(m[a+i] for i in range(n)),'big')
def store(m,a,n,v):
    require(a%n==0 and all(a+i in m for i in range(n)),f'invalid write {a:x}/{n}')
    for i,b in enumerate((v&((1<<(8*n))-1)).to_bytes(n,'big')):m[a+i]=b

def fixtures():
    rng=random.Random(ENTRY)
    for case in range(192):
        mode=2 if case<160 else (-1,0,1,3,4,5,6,7)[case%8]
        count=(-1,0,1,2,3,4)[case%6]
        records=[]
        for i in range(4):
            records.append({'present':bool(rng.randrange(2)), 'flag':[-128,-1,0,1,127][(case+i)%5],
                            'ready':[-128,-1,0,1,127][rng.randrange(5)],
                            'time':int.from_bytes(struct.pack('>f',rng.uniform(0,3600)),'big'),
                            'old_time':int.from_bytes(struct.pack('>f',rng.uniform(0,3600)),'big'),
                            'size':rng.randrange(65536),'old_size':rng.randrange(65536),
                            'run':rng.randrange(65536)})
        yield {'mode':mode,'count':count,'records':records}

def addresses(i):
    base=0x82000000+0x1000*i
    return base,base+0x100,base+0x200,base+0x300,base+0x400

def state(c):
    regions=[(LAST,4),(MODE,4),(BUSY,1),(COUNT,2),(TABLE,16),(MODELS,4*2056),(CARS,4*952),(STACK-512,1024)]
    regions += [(0x82000000+0x1000*i,0x800) for i in range(4)]
    m={a+i:0xa5 for a,n in regions for i in range(n)}
    store(m,LAST,4,0x87654320);store(m,MODE,4,c['mode']);store(m,BUSY,1,0x7f);store(m,COUNT,2,c['count'])
    for i,r in enumerate(c['records']):
        handle,data,descriptor,header,allocation=addresses(i)
        store(m,TABLE+4*i,4,handle if r['present'] else 0)
        store(m,handle,4,data);store(m,data+40,4,descriptor);store(m,descriptor,4,header)
        store(m,header+5,1,r['flag']);store(m,header+56,4,r['old_time']);store(m,header+76,4,allocation)
        store(m,header+80,4,r['size']);store(m,header+84,4,r['old_size'])
        store(m,MODELS+2056*i+1820,2,r['run']);store(m,CARS+952*i+239,1,r['ready']);store(m,CARS+952*i+240,4,r['time'])
    return m

def visible(m):return {a:v for a,v in m.items() if not STACK-512<=a<STACK+512}

def model(c):
    m=state(c);events=[];store(m,LAST,4,0)
    if c['mode']==2:
        for i in range(max(c['count'],0)):
            r=c['records'][i]
            if not r['present']:continue
            handle,data,descriptor,header,allocation=addresses(i)
            store(m,TABLE+4*i,4,0);store(m,MODELS+2056*i+1820,2,0)
            if r['flag']>0:events.append(('end',handle))
            elif r['flag']<0:
                store(m,header+5,1,0)
                if r['ready']:
                    store(m,header+56,4,r['time']);store(m,header+84,4,r['size']);store(m,LAST,4,header)
                else:
                    events.extend([('lock',QUEUE,0,1),('free',allocation,0),('unlock',QUEUE,0,0)])
                    store(m,header+76,4,0)
        store(m,COUNT,2,1);store(m,BUSY,1,0)
    return visible(m),events

def execute(words,c):
    code={ENTRY+4*i:w for i,w in enumerate(words)};m=state(c)
    r=[0xa5000000+13*i for i in range(32)];f=[0xa6000000+17*i for i in range(32)]
    r[0]=0;r[29]=STACK;r[31]=RETURN;before=r[:];fbefore=f[:]
    pc=ENTRY;pending=None;lo=0;events=[];coverage=set();branches=set()
    for step in range(100000):
        if pc==RETURN:break
        require(pc in code,f'escaped instruction {pc:x}');coverage.add(pc)
        w=code[pc];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63;imm=signed(w,16)
        a=(r[rs]+imm)&MASK;old=pending;pending=None;nxt=pc+4
        if op==0:
            if fn==0:r[rd]=r[rt]<<sh
            elif fn==3:r[rd]=signed(r[rt])>>sh
            elif fn==8:pending=r[rs]
            elif fn==18:r[rd]=lo
            elif fn==25:lo=r[rs]*r[rt]&MASK
            elif fn==33:r[rd]=r[rs]+r[rt]
            elif fn==35:r[rd]=r[rs]-r[rt]
            elif fn==37:r[rd]=r[rs]|r[rt]
            elif fn==42:r[rd]=int(signed(r[rs])<signed(r[rt]))
            else:raise RuntimeError(f'unexpected SPECIAL {fn}')
        elif op==3:
            dest=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2);r[31]=pc+8;pending=('call',dest,pc+8)
        elif op==9:r[rt]=a
        elif op==10:r[rt]=int(signed(r[rs])<imm)
        elif op==15:r[rt]=(w&65535)<<16
        elif op==1:
            take=signed(r[rs])<0 if rt in (0,2) else signed(r[rs])>=0
            require(rt in (0,1,2,3),'REGIMM');branches.add((pc,take))
            if take:pending=pc+4+imm*4
            elif rt>=2:nxt+=4
        elif op in (4,5,6,20,21,22):
            take=(r[rs]==r[rt]) if op in (4,20) else ((r[rs]!=r[rt]) if op in (5,21) else signed(r[rs])<=0)
            branches.add((pc,take))
            if take:pending=pc+4+imm*4
            elif op>=20:nxt+=4
        elif op in (32,33,35,36,37):
            n=4 if op==35 else (2 if op in (33,37) else 1);v=load(m,a,n);r[rt]=signed(v,8*n) if op in (32,33) else v
        elif op in (40,41,43):store(m,a,4 if op==43 else (2 if op==41 else 1),r[rt])
        elif op==49:f[rt]=load(m,a,4)
        elif op==57:store(m,a,4,f[rt])
        elif op==53:f[rt]=load(m,a,4);f[rt+1]=load(m,a+4,4)
        elif op==61:store(m,a,4,f[rt]);store(m,a+4,4,f[rt+1])
        else:raise RuntimeError(f'unexpected opcode {op}')
        r=[x&MASK for x in r];r[0]=0;require(old is None or pending is None,'nested delay slot')
        if isinstance(old,tuple):
            dest=old[1];result=0xbadfeed
            require(load(m,BUSY,1)==1,'call outside busy interval')
            if dest==END:events.append(('end',r[4]))
            elif dest==FREE:
                events.append(('free',r[5],r[6]))
                # Genuine native internal heap release owns s0/s1 in its caller.
                r[16]=0xbad10010;r[17]=0xbad10011
            elif dest in (LOCK,UNLOCK):events.append(('lock' if dest==LOCK else 'unlock',r[4],r[5],r[6]))
            else:raise RuntimeError(f'unknown callee {dest:x}')
            for j in list(range(1,16))+[24,25]:r[j]=0xbad00000+31*j
            for j in range(20):f[j]=0xbad30000+j
            r[2]=result;pc=old[2]
        else:pc=old if old is not None else nxt
    else:raise RuntimeError('execution bound')
    require(r[16:24]==before[16:24] and r[28:]==before[28:] and f[20:]==fbefore[20:],'saved register/stack ABI')
    return visible(m),events,coverage,branches

def verify(native,compiled):
    coverage=[set(),set()];branches=[set(),set()];count=0
    for c in fixtures():
        expected=model(c)
        for i,words in enumerate((native,compiled)):
            m,e,cov,br=execute(words,c);require((m,e)==expected,f'fixture {count} model mismatch')
            coverage[i]|=cov;branches[i]|=br
        count+=1
    reachable=[structural_reachability(w) for w in (native,compiled)]
    for i in range(2):require({a-ENTRY for a in coverage[i]}==reachable[i],'reachable instruction coverage')
    return {'fixtures':count,'executions':2*count,'instruction_coverage':[len(x) for x in coverage],
            'unreachable_instruction_offsets':[[4*j for j in range(len(w)) if 4*j not in reachable[i]] for i,w in enumerate((native,compiled))],
            'branch_outcomes':[len(x) for x in branches],'callee_side_effects_validated':False,'root_only':True}

def structural_reachability(words):
    """Conservative root-only CFG; copied branch-likely prefixes can be dead."""
    pending=[0];seen=set();reachable=set()
    while pending:
        off=pending.pop()
        if off in seen:continue
        require(0<=off<4*len(words) and off%4==0,'CFG escape')
        seen.add(off);reachable.add(off);w=words[off//4];op=w>>26;rs=w>>21&31;rt=w>>16&31;fn=w&63
        if op==0 and fn==8:
            require(rs==31,'non-return indirect jump');reachable.add(off+4)
        elif op==3:
            reachable.add(off+4);pending.append(off+8)
        elif op in (1,4,5,6,7,20,21,22,23):
            reachable.add(off+4);target=off+4+signed(w,16)*4
            if op in (4,20) and rs==rt:pending.append(target)
            elif op in (5,21) and rs==rt:pending.append(off+8)
            else:pending.extend([target,off+8])
        else:pending.append(off+4)
    return reachable
