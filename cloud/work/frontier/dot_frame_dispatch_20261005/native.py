"""Bounded fail-closed MIPS-II model of the complete frame dispatcher.

Only manifest-verified or freshly GNU-linked words are input. Call hooks assert
an O32 interface and deliberately clobber caller-save state; they do not model
the particle system or separately loaded overlay's internals.
"""
import random
import struct

MASK=0xffffffff
BASE=0x800b7ff8
SIZE=208
STACK=0x100100
RETURN=0x70000000
STATE,PAUSE,MODE,DELTA,PI,ACCUM=0x801174b4,0x801170fc,0x8014a110,0x8002eb94,0x80123de0,0x80116178
PARTICLE,OVERLAY=0x800b7a40,0x80391b00

def signed(x,bits=32):
    x&=(1<<bits)-1
    return x-(1<<bits) if x>>(bits-1) else x

def word(value): return struct.unpack('>I',struct.pack('>f',value))[0]
def value(bits): return struct.unpack('>f',struct.pack('>I',bits&MASK))[0]
def f32(x): return value(word(x))

class Machine:
    def __init__(self,words,case):
        assert len(words)==SIZE//4
        self.words,self.case=words,case
        self.r=[0x12340000+i for i in range(32)]
        self.r[0],self.r[29],self.r[31]=0,STACK,RETURN
        self.fr=[0x3f010000+i for i in range(32)]
        self.original,self.original_fr=list(self.r),list(self.fr)
        self.mem={a:0xa5 for a in range(STACK-32,STACK+32)}
        for a in (STATE,PAUSE,MODE,DELTA,PI,ACCUM):
            for i in range(4):self.mem[a+i]=0x5a
        for a,x in zip((STATE,PAUSE,MODE,DELTA,ACCUM),case[:5]): self.put(a,x)
        self.put(PI,0x40490fdb)
        self.before=dict(self.mem)
        self.reads=[];self.writes=[];self.events=[];self.visited=set();self.branches={};self.steps=0
    def get(self,a):
        assert a%4==0 and all(a+i in self.mem for i in range(4)),hex(a)
        return int.from_bytes(bytes(self.mem[a+i] for i in range(4)),'big')
    def put(self,a,x):
        assert a%4==0 and all(a+i in self.mem for i in range(4)),hex(a)
        for i,b in enumerate((x&MASK).to_bytes(4,'big')):self.mem[a+i]=b
    def reg(self,n,x):
        if n:self.r[n]=x&MASK
    def hook(self,address):
        assert len(self.events)<6
        kind=1 if address==PARTICLE else 2
        if kind==1: assert self.r[4]==0
        self.events.extend([kind,self.get(MODE),self.get(ACCUM)])
        if kind==1:
            for bit,a,x in zip((1,2,4),(MODE,ACCUM,DELTA),self.case[6:]):
                if self.case[5]&bit:self.put(a,x)
        for n in [1,*range(2,16),24,25]:self.r[n]=0xbad00000+n
        for n in range(20):self.fr[n]=0x3e100000+n
    def step(self,pc,delay=False):
        assert BASE<=pc<BASE+SIZE and pc%4==0,hex(pc)
        self.steps+=1;assert self.steps<120
        self.visited.add(pc-BASE)
        w=self.words[(pc-BASE)//4]
        op,rs,rt,rd,sh,fn=w>>26,w>>21&31,w>>16&31,w>>11&31,w>>6&31,w&63
        imm=w&65535;si=signed(imm,16);a,b=self.r[rs],self.r[rt]
        branch=None;likely=False;take=None
        if op==0:
            if fn==0:self.reg(rd,b<<sh)
            elif fn==37:self.reg(rd,a|b)
            elif fn==8:branch=a
            else:raise AssertionError(('SPECIAL',fn))
        elif op==9:self.reg(rt,a+si)
        elif op==15:self.reg(rt,imm<<16)
        elif op in (4,5,20,21):
            take=(a==b) if op in (4,20) else (a!=b);likely=op in (20,21)
        elif op==1:
            assert rt in (0,1);take=signed(a)<0 if rt==0 else signed(a)>=0
        elif op==3:
            branch=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2);self.r[31]=pc+8
        elif op in (35,43,49,57):
            addr=(a+si)&MASK
            if op in (35,49):
                self.reads.append(addr);x=self.get(addr)
                if op==35:self.reg(rt,x)
                else:self.fr[rt]=x
            else:
                self.writes.append(addr);self.put(addr,b if op==43 else self.fr[rt])
        elif op==17:
            assert rs==16 and fn in (0,2),('COP1',rs,fn)
            x,y=value(self.fr[rd]),value(self.fr[rt])
            self.fr[sh]=word(x+y if fn==0 else x*y)
        else:raise AssertionError(('opcode',op))
        if take is not None:
            self.branches.setdefault(pc-BASE,set()).add(take)
            branch=pc+4+4*si if take else pc+8
        if branch is not None:
            assert not delay,'control transfer in delay slot'
            if not likely or take:assert self.step(pc+4,True)==pc+8
            if branch in (PARTICLE,OVERLAY):self.hook(branch);return self.r[31]
            return branch
        return pc+4
    def run(self):
        pc=BASE
        while pc!=RETURN:pc=self.step(pc)
        assert self.r[29]==STACK and self.r[31]==RETURN
        assert self.r[16:24]==self.original[16:24] and self.r[28]==self.original[28] and self.r[30]==self.original[30]
        assert self.fr[20:]==self.original_fr[20:]
        eligible=self.case[0]!=MASK and bool(self.case[0]&0x600000)
        writes=[STACK-4]+([ACCUM] if eligible and self.case[1]==0 else [])
        assert self.writes==writes
        assert self.reads.count(DELTA)==int(eligible and self.case[1]==0)
        mutable=set(range(STACK-4,STACK))|set(range(ACCUM,ACCUM+4))
        if self.case[5]&1 and self.events:mutable|=set(range(MODE,MODE+4))
        if self.case[5]&4 and self.events:mutable|=set(range(DELTA,DELTA+4))
        assert all(self.mem[a]==x for a,x in self.before.items() if a not in mutable)
        return [self.get(ACCUM),self.get(MODE),len(self.events)//3]+self.events+[0]*(6-len(self.events))

def reference(case):
    state,pause,mode,delta,accum,mask,new_mode,new_accum,new_delta=case
    events=[]
    if state!=MASK and state&0x600000:
        if pause==0:accum=word(value(accum)+f32(value(0x40490fdb)*value(delta)))
        if mode in (0,1,2,3,4):
            events.extend([1,mode,accum])
            if mask&1:mode=new_mode
            if mask&2:accum=new_accum
        if mode in (4,6):events.extend([2,mode,accum])
    return [accum,mode,len(events)//3]+events+[0]*(6-len(events))

def corpus():
    rows=[]
    for state in (0,MASK,0x200000,0x400000,0x600000,0x1fffff,0x80000000,0x80200000):
        for pause in (0,1,MASK):
            for mode in (0,1,2,3,4,5,6,7,MASK,0x80000000):
                for mask in (0,1,2,4,7):
                    for new_mode in (0,4,6,7):
                        rows.append((state,pause,mode,word(1/60),word(-1.5),mask,new_mode,word(19.25),word(-.5)))
    rng=random.Random(0xb7ff8)
    for _ in range(4096):
        rows.append((rng.getrandbits(32),rng.choice([0,0,1,MASK]),rng.choice([0,1,2,3,4,5,6,7,MASK]),
                     word(rng.uniform(-2,2)),word(rng.uniform(-100,100)),rng.randrange(8),
                     rng.choice([0,4,6,7,MASK]),word(rng.uniform(-100,100)),word(rng.uniform(-2,2))))
    for delta in (0,0x80000000,1,0x80000001,0x00800000,0x80800000):
        for accum in (0,0x80000000,1,0x80000001):rows.append((0x200000,0,4,delta,accum,0,0,0,0))
    return rows
