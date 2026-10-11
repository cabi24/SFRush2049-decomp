"""Bounded cleanup-lifetime model and fail-closed MIPS interpreter.

Native instructions are supplied only by authenticated target readers. Helpers
are explicit O32 call hooks, not claims about their actual implementations.
"""
import random
import struct
ENTRY=0x8039D05C
EXIT=0x81234560
SP=0x81010000
MODE=0x80156994
TRACK=0x8014978C
COUNT=0x8014A108
BOXES=0x803B9FC0
SLOTS=0x803AF9A8
ACTIVE=0x803B9B90
PAIRS=0x803B25CC
PANELS=0x803B2618
SINGLE=0x803B25C8
BLIT=0x803B28AC
TIMER=0x80154188
STATE=0x80151AD0
QUEUES=0x803BA1D0
FLAG=0x803B28A8
CALLS={0x800A4AC4:'wheel',0x800DC720:'boxes',0x80090254:'handle',0x800B358C:'blit',0x800B557C:'ambient',0x800A5A40:'reset',0x800A5B3C:'finish',0x80091C04:'queue'}
ARGS={'wheel','boxes','handle','blit','queue'}
VOLATILE=(2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25)

def require(ok,message):
    if not ok:raise ValueError(message)
def signed(x,bits=32):
    x&=(1<<bits)-1
    return x-(1<<bits) if x&(1<<(bits-1)) else x

def read(m,a,n):
    require(a%n==0 and all(a+j in m for j in range(n)),('unmapped read',hex(a),n))
    return int.from_bytes(bytes(m[a+j] for j in range(n)),'big')
def write(m,a,n,x):
    require(a%n==0 and all(a+j in m for j in range(n)),('unmapped write',hex(a),n))
    for j,v in enumerate((x&((1<<(n*8))-1)).to_bytes(n,'big')):m[a+j]=v

def cases():
    # Every signed-byte selector; boundary signed counts and deliberately sparse
    # handles plus dense cases exercise every cleanup and no-op branch.
    for k in range(256):
        yield {'seed':k,'mode':signed(k,8),'track':signed(k*37,8),'count':(-32768,-1,0,1,2,3,4)[k%7],
               'density':k%3,'mutation_call':-1,'mutation_count':0}
    rng=random.Random(0xD05C)
    for k in range(256):
        yield {'seed':k+256,'mode':rng.choice((0,1,-1)),'track':rng.choice((-128,5,6,7,127)),
               'count':rng.randrange(1,5),'density':k%3,'mutation_call':rng.choice((1,2,3,4,5,8,12,20,40,80,120)),
               'mutation_count':rng.choice((-1,0,1,2,3,4))}

def state(case):
    spans=[(SP-32,SP+16),(MODE,MODE+1),(TRACK,TRACK+1),(COUNT,COUNT+2),
           (BOXES-8,BOXES+24),(SLOTS-8,SLOTS+4*44*64+8),(ACTIVE-8,ACTIVE+24),
           (PAIRS-8,PAIRS+40),(PANELS-8,PANELS+11*52+8),(SINGLE-8,SINGLE+12),
           (BLIT-8,BLIT+12),(TIMER-4,TIMER+8),(STATE-4,STATE+6),(QUEUES-8,QUEUES+24),(FLAG-4,FLAG+5)]
    m={a:0xa5 for lo,hi in spans for a in range(lo,hi)};rng=random.Random(case['seed'])
    def handle():
        return -1 if case['density']==0 or (case['density']==1 and rng.randrange(3)==0) else rng.choice((-32768,-2,0,1,32767,65535,0x12345678))
    write(m,MODE,1,case['mode']);write(m,TRACK,1,case['track']);write(m,COUNT,2,case['count'])
    for i in range(4):
        write(m,BOXES+4*i,4,0 if rng.randrange(2) else 0x81100000+16*i)
        write(m,ACTIVE+4*i,4,rng.getrandbits(32));write(m,QUEUES+4*i,4,0x9000+i)
        for j in range(44):write(m,SLOTS+i*2816+j*64+4,4,handle())
        for j in range(2):write(m,PAIRS+i*8+j*4,4,handle())
    for i in range(11):write(m,PANELS+i*52,2,handle());write(m,PANELS+i*52+2,2,rng.getrandbits(16))
    write(m,SINGLE,4,handle());write(m,BLIT,4,0x81110000 if rng.randrange(2) else 0)
    return m

def hook(m,case,calls,name,arg):
    calls.append((name,arg if name in ARGS else None))
    if len(calls)==case['mutation_call']:write(m,COUNT,2,case['mutation_count'])

def oracle(case):
    m=state(case);calls=[]
    call=lambda n,a=None:hook(m,case,calls,n,a)
    call('wheel',62)
    if signed(read(m,MODE,1),8):call('wheel',80);call('wheel',61)
    else:call('wheel',80 if signed(read(m,TRACK,1),8)>=6 else 81)
    for i in range(4):
        a=BOXES+4*i;h=read(m,a,4)
        if h:call('boxes',h);write(m,a,4,0)
    player=0
    while player<signed(read(m,COUNT,2),16):
        require(player<4,'model player domain')
        for j in range(44):
            a=SLOTS+player*2816+j*64+4;h=signed(read(m,a,4))
            if h!=-1:call('handle',signed(h,16));write(m,a,4,-1)
        write(m,ACTIVE+player*4,4,0)
        for j in range(2):
            a=PAIRS+player*8+j*4;h=signed(read(m,a,4))
            if h!=-1:call('handle',signed(h,16));write(m,a,4,-1)
        if player==0:
            for j in range(11):
                a=PANELS+j*52;h=signed(read(m,a,2),16)
                if h!=-1:call('handle',h);write(m,a,2,-1);write(m,a+2,2,-1)
            h=signed(read(m,SINGLE,4))
            if h!=-1:call('handle',signed(h,16));write(m,SINGLE,4,-1)
        player+=1
    h=read(m,BLIT,4)
    if h:call('blit',h);write(m,BLIT,4,0)
    call('ambient');write(m,TIMER,4,0x42b40000);write(m,STATE,2,1)
    call('reset');call('finish');player=0
    while player<signed(read(m,COUNT,2),16):
        require(player<4,'model queue domain');call('queue',signed(read(m,QUEUES+player*4,4)));player+=1
    write(m,FLAG,1,0)
    return m,calls

def run(words,case,salt):
    rng=random.Random(salt^0xd05c);r=[rng.getrandbits(32) for _ in range(32)];r[0]=0;r[29]=SP;r[31]=EXIT;initial=r[:]
    f=[rng.getrandbits(32) for _ in range(32)];m=state(case);pc=ENTRY;pending=None;coverage=set();branches=set();reads=[];writes=[];calls=[]
    def store(a,n,x):write(m,a,n,x);writes.append((a,n,x&((1<<(n*8))-1)))
    for step in range(20000):
        if pc==EXIT:
            require(pending is None,'unfinished delay slot');require(all(r[i]==initial[i] for i in (28,29,31)),'preserved private register changed')
            return {'memory':m,'calls':calls,'coverage':coverage,'branches':branches,'reads':reads,'writes':writes,'registers':r,'initial':initial}
        if pc in CALLS:
            require(pending is None,'hook in delay slot');ret=r[31];name=CALLS[pc]
            arg=signed(r[4]) if name in ('wheel','handle','queue') else r[4]
            hook(m,case,calls,name,arg)
            for j in VOLATILE:r[j]=rng.getrandbits(32)
            for j in range(20):f[j]=rng.getrandbits(32)
            pc=ret;continue
        require(ENTRY<=pc<ENTRY+len(words)*4 and pc%4==0,('escaped code',hex(pc)))
        coverage.add(pc);w=words[(pc-ENTRY)//4];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;fn=w&63;im=signed(w,16);delayed=pending;pending=None;transfer=None;annul=False
        if w==0:pass
        elif op==0:
            if fn==0:r[rd]=(r[rt]<<((w>>6)&31))&0xffffffff
            elif fn==3:r[rd]=(signed(r[rt])>>((w>>6)&31))&0xffffffff
            elif fn==33:r[rd]=(r[rs]+r[rt])&0xffffffff
            elif fn==35:r[rd]=(r[rs]-r[rt])&0xffffffff
            elif fn==37:r[rd]=r[rs]|r[rt]
            elif fn==42:r[rd]=int(signed(r[rs])<signed(r[rt]))
            elif fn==43:r[rd]=int(r[rs]<r[rt])
            elif fn==8:require(rs==31 and w&0x1fffff==8,'unsupported jr');transfer=r[rs]
            else:raise ValueError(('unknown special',hex(pc),fn))
        elif op==15:require(rs==0,'bad lui');r[rt]=(w&65535)<<16
        elif op==9:r[rt]=(r[rs]+im)&0xffffffff
        elif op==10:r[rt]=int(signed(r[rs])<im)
        elif op in (32,33,35,36,37):
            n={32:1,33:2,35:4,36:1,37:2}[op];a=(r[rs]+im)&0xffffffff;v=read(m,a,n);reads.append((a,n));r[rt]=(signed(v,n*8) if op in (32,33) else v)&0xffffffff
        elif op in (40,41,43):store((r[rs]+im)&0xffffffff,{40:1,41:2,43:4}[op],r[rt])
        elif op==17:require(rs==4 and w&0x7ff==0,'unsupported COP1');f[rd]=r[rt]
        elif op==57:store((r[rs]+im)&0xffffffff,4,f[rt])
        elif op in (1,4,5,6,7,20,21,22,23):
            if op==1:require(rt in (0,1),'unknown REGIMM');taken=signed(r[rs])>=0 if rt else signed(r[rs])<0
            elif op in (6,22):require(rt==0,'bad blez');taken=signed(r[rs])<=0
            elif op in (7,23):require(rt==0,'bad bgtz');taken=signed(r[rs])>0
            else:taken=(r[rs]==r[rt]) if op in (4,20) else r[rs]!=r[rt]
            branches.add((pc,taken));annul=op in (20,21,22,23) and not taken;transfer=pc+4+im*4 if taken else pc+8
        elif op in (2,3):
            transfer=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
            if op==3:require(transfer in CALLS,'unexpected helper');r[31]=pc+8
        else:raise ValueError(('unknown opcode',hex(pc),op))
        r[0]=0
        if delayed is not None:require(transfer is None,'branch in delay slot');pc=delayed
        elif annul:pc+=8
        else:pc,pending=pc+4,transfer
    raise ValueError('execution limit')

def verify(native,linked):
    covered=set();branches=set();mutated=0;rows=list(cases())
    for i,c in enumerate(rows):
        a=run(native,c,i);b=run(linked,c,i);expected,calls=oracle(c)
        for key in ('memory','calls','coverage','branches','reads','writes'):require(a[key]==b[key],('native/GNU mismatch',i,key))
        require(a['calls']==calls,('model calls',i,a['calls'],calls))
        require(all(a['memory'][p]==v for p,v in expected.items() if not SP-24<=p<SP),('model/guard memory',i))
        require(all(a['registers'][j]==a['initial'][j] for j in (28,29,31)),'ABI mismatch')
        covered.update(a['coverage']);branches.update(a['branches']);mutated+=0<c['mutation_call']<=len(calls)
    require(len(covered)==169,('incomplete instruction coverage',len(covered)))
    return {'cases_per_native_GNU_and_model':len(rows),'instructions_covered':len(covered),'branch_outcomes':len(branches),
            'dynamic_player_count_mutations':mutated,'full_memory_call_read_write_trace_equality':True,
            'player_domain':'selected counts -32768,-1,0,1,2,3,4; all signed-byte mode selectors',
            'helper_boundary':'eight external O32 hooks, arbitrary integer/FPU volatile clobbers; optional count mutation',
            'preserved_private_registers':['gp','sp','ra'],'standalone_O32_replacement':False}
