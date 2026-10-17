"""Independent packed-data model versus native/compiled ReadPathData.

Only memcpy executes as a callee. Four gameplay callees are deterministic
call-boundary stubs with register clobbers; their side effects and timing are
not validated. No target bytes are stored in this packet.
"""
import random, struct
ENTRY,STACK,RETURN=0x800BAAA0,0x81010000,0x81234560
RESTART,WAVE,RES,ALT,TOTAL,VERTICES=0x80114650,0x80153F20,0x801407F0,0x80153E80,0x801527A4,0x801409E8
DRONES,COUNTS,POINTS,BUFFER=0x8012E5E8,0x801527D0,0x80153EF8,0x82000000
MEMCPY,TIMING,SYNC,ALTSYNC,BOUNDS=0x80007C68,0x800B93A8,0x800BA7C4,0x800B9D68,0x800B9740
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
    for case in range(160):
        nalt=[0,1,2,5,255][case%5] if case<10 else rng.randrange(8)
        altcounts=[rng.randrange(8) for _ in range(nalt)]
        main=rng.randrange(1,9)
        if (main+sum(altcounts))%2==0:main+=1
        total=main+sum(altcounts)
        drones=[rng.randrange(9) for _ in range(4)]
        size=812+16+16*nalt+2+6*total+sum(8+8*n for n in drones)
        raw=bytearray(rng.randbytes(size))
        def put(off,n,v):raw[off:off+n]=v.to_bytes(n,'big')
        put(812,2,main);put(820,1,nalt)
        for i,n in enumerate(altcounts):put(828+16*i+10,2,n)
        off=828+16*nalt;put(off,2,total);off+=2+6*total
        require(off%4==0,'fixture alignment')
        for n in drones:put(off,2,n);off+=8+8*n
        yield {'restart':0 if case<128 else (-128,-1,1,127)[case%4], 'raw':bytes(raw)}

def state(c):
    regions=[(RESTART,1),(WAVE,4),(RES,16),(ALT,4),(TOTAL,2),(VERTICES,4),
             (DRONES,32),(COUNTS,8),(POINTS,16),(BUFFER,len(c['raw'])),(STACK-256,512)]
    m={a+i:0xa5 for a,n in regions for i in range(n)}
    store(m,RESTART,1,c['restart']);store(m,WAVE,4,BUFFER)
    for i,b in enumerate(c['raw']):m[BUFFER+i]=b
    return m

def visible(m):return {a:v for a,v in m.items() if not STACK-256<=a<STACK+256}

def model(c):
    m=state(c);events=[]
    if c['restart']:return visible(m),[('timing',),('sync',-1),('altsync',),('bounds',),('timing',)]
    raw=c['raw'];nalt=raw[820];main=int.from_bytes(raw[812:814],'big')
    header=828+16*nalt;vertex=header+2
    altcounts=[int.from_bytes(raw[838+16*i:840+16*i],'big') for i in range(nalt)]
    first_drone=vertex+6*(main+sum(altcounts))
    events.append(('copy',RES,BUFFER+812,16))
    for i,b in enumerate(raw[812:828]):m[RES+i]=b
    store(m,RES+4,4,BUFFER+vertex);store(m,VERTICES,4,BUFFER+vertex)
    store(m,RES+12,4,BUFFER+828 if nalt else 0);store(m,ALT,4,BUFFER+828 if nalt else 0)
    events.append(('copy',TOTAL,BUFFER+header,2))
    store(m,TOTAL,2,int.from_bytes(raw[header:header+2],'big'))
    consumed=main
    for i,n in enumerate(altcounts):
        store(m,BUFFER+828+16*i+12,4,BUFFER+vertex+6*consumed);consumed+=n
    off=first_drone
    for p in range(4):
        n=int.from_bytes(raw[off:off+2],'big');events.append(('copy',DRONES+8*p,BUFFER+off,8))
        for j,b in enumerate(raw[off:off+8]):m[DRONES+8*p+j]=b
        store(m,COUNTS+2*p,2,n);store(m,POINTS+4*p,4,BUFFER+off+8);store(m,DRONES+8*p+4,4,BUFFER+off+8)
        off+=8+8*n
    events.extend([('sync',-1),('altsync',),('bounds',),('timing',)])
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
        elif op in (4,5,6,20,21,22):
            take=(r[rs]==r[rt]) if op in (4,20) else ((r[rs]!=r[rt]) if op in (5,21) else signed(r[rs])<=0)
            branches.add((pc,take))
            if take:pending=pc+4+imm*4
            elif op>=20:nxt+=4
        elif op in (32,33,35,36,37):
            n=4 if op==35 else (2 if op in (33,37) else 1);v=load(m,a,n);r[rt]=signed(v,8*n) if op in (32,33) else v
        elif op in (40,41,43):store(m,a,4 if op==43 else (2 if op==41 else 1),r[rt])
        elif op==53:f[rt]=load(m,a,4);f[rt+1]=load(m,a+4,4)
        elif op==61:store(m,a,4,f[rt]);store(m,a+4,4,f[rt+1])
        else:raise RuntimeError(f'unexpected opcode {op}')
        r=[x&MASK for x in r];r[0]=0;require(old is None or pending is None,'nested delay slot')
        if isinstance(old,tuple):
            dest=old[1];result=0xbadfeed
            if dest==MEMCPY:
                dst,src,n=r[4:7];require(n in (2,8,16),'memcpy extent');events.append(('copy',dst,src,n))
                payload=[load(m,src+j,1) for j in range(n)]
                for j,b in enumerate(payload):store(m,dst+j,1,b)
                result=dst
            else:
                require(dest in (TIMING,SYNC,ALTSYNC,BOUNDS),'unknown gameplay call')
                events.append(('sync',signed(r[4],16)) if dest==SYNC else ({TIMING:'timing',ALTSYNC:'altsync',BOUNDS:'bounds'}[dest],))
                # Internal timing convention: loader owns these saved registers.
                for j in range(16,23):r[j]=0xbad10000+j
                for j in range(20,26):f[j]=0xbad20000+j
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
    return {'fixtures':count,'executions':2*count,'instruction_coverage':[len(x) for x in coverage],
            'branch_outcomes':[len(x) for x in branches],'callee_side_effects_validated':False}
