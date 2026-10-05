"""Fail-closed bounded MIPS-II replay, including the real accepted normalizer.

The collision routine is an explicit external boundary model, not a replay of
its internals. No native instructions are embedded. FCSR and exceptional/NaN
arithmetic are outside this finite-domain proof.
"""
import math
import struct
BASE=0x800c69c0
NORMALIZE=0x8008e0b8
COLLISION=0x800add58
THRESHOLD=0x8012394c
FIRST=0x100000
SECOND=0x100040
STACK=0x200400
RETURN=0x70000000
MASK=0xffffffff

def bits(x):return struct.unpack('>I',struct.pack('>f',x))[0]
def value(x):return struct.unpack('>f',struct.pack('>I',x))[0]
def f32(x):return value(bits(x))
def signed(x,width=32):return (x&((1<<width)-1))-(1<<width) if x&(1<<(width-1)) else x

class Machine:
    def __init__(self,words,normalizer,threshold,case):
        self.code={BASE+4*i:w for i,w in enumerate(words)}
        self.code.update({NORMALIZE+4*i:w for i,w in enumerate(normalizer)})
        self.case=case; self.frame=-signed(words[0]&65535,16)
        assert words[0]>>16==0x27bd and 0<self.frame<=192
        self.r=[0xa5000000+i for i in range(32)];self.r[0]=0
        self.r[4]=FIRST;self.r[5]=FIRST if case['alias'] else SECOND
        self.r[29]=STACK;self.r[31]=RETURN
        self.fp=[bits(1.0+i) for i in range(32)];self.condition=False
        self.initial_r=list(self.r);self.initial_fp=list(self.fp)
        self.memory={STACK+i:(i*71)&255 for i in range(-320,64)}
        self.memory.update({FIRST+i:(i*23)&255 for i in range(-16,28)})
        self.memory.update({SECOND+i:(i*43)&255 for i in range(-16,28)})
        for base,vector in [(FIRST,case['first']),(SECOND,case['second'])]:
            for i,x in enumerate(vector):self.put(base+4*i,bits(x),initial=True)
        for i,b in enumerate(threshold):self.memory[THRESHOLD+i]=b
        self.original=dict(self.memory);self.events=[];self.visited=set();self.branches=set()
        self.steps=0;self.reads=[];self.writes=[];self.pc=BASE
    def get(self,a):
        assert a%4==0 and all(a+i in self.memory for i in range(4)),hex(a)
        self.reads.append(a)
        return int.from_bytes(bytes(self.memory[a+i] for i in range(4)),'big')
    def put(self,a,x,initial=False):
        assert a%4==0 and all(a+i in self.memory for i in range(4)),hex(a)
        if not initial:
            assert FIRST<=a<FIRST+12 or STACK-self.frame-40<=a<STACK+8,hex(a)
            self.writes.append(a)
        for i,b in enumerate((x&MASK).to_bytes(4,'big')):self.memory[a+i]=b
    def vector(self,a):return [self.get(a+4*i) for i in range(3)]
    def collision(self):
        first=FIRST;second=FIRST if self.case['alias'] else SECOND
        assert self.r[4]==second and self.r[5]==first
        assert self.r[7]==bits(0.5) and self.get(self.r[29]+16)==1 and self.get(self.r[29]+20)==6
        basis=self.r[6];assert self.r[29]+24<=basis and basis+36<=STACK
        self.events.append(('collision',self.vector(first),self.vector(second)))
        for i in range(9):self.put(basis+4*i,bits((i+1)*0.125))
        if self.case['mutate']:
            for i,x in enumerate(self.case['replacement']):self.put(first+4*i,bits(x))
        return_to=self.r[31]
        for i in [1,*range(2,16),24,25]:self.r[i]=0xbad00000+i
        for i in range(20):self.fp[i]=bits(11.0+i)
        self.r[2]=0x400000 if self.case['hit'] else 0
        return return_to
    def step(self,pc,delay=False):
        assert pc in self.code,hex(pc)
        self.steps+=1;assert self.steps<600
        self.visited.add(pc);self.pc=pc
        if pc==NORMALIZE:self.events.append(('normalize',self.vector(self.r[4])))
        w=self.code[pc];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63
        imm=w&65535;si=signed(imm,16);a,b=self.r[rs],self.r[rt]
        branch=None;skip=False
        def wr(r,x):
            if r:self.r[r]=x&MASK
        if op==0:
            if fn==0:wr(rd,b<<sh)
            elif fn==33:wr(rd,a+b)
            elif fn==35:wr(rd,a-b)
            elif fn==37:wr(rd,a|b)
            elif fn==8:branch=a
            else:raise AssertionError(('SPECIAL',fn,hex(pc)))
        elif op==9:wr(rt,a+si)
        elif op==10:wr(rt,int(signed(a)<si))
        elif op==11:wr(rt,int(a<(si&MASK)))
        elif op==15:wr(rt,imm<<16)
        elif op in (4,5,20,21):
            take=(a==b)==(op in (4,20));self.branches.add((pc,take))
            if take:branch=pc+4+4*si
            elif op in (20,21):skip=True
            else:branch=pc+8
        elif op==3:
            branch=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
            assert branch in (COLLISION,NORMALIZE),hex(branch)
            self.r[31]=pc+8
        elif op==35:wr(rt,self.get((a+si)&MASK))
        elif op==43:self.put((a+si)&MASK,b)
        elif op==49:self.fp[rt]=self.get((a+si)&MASK)
        elif op==57:self.put((a+si)&MASK,self.fp[rt])
        elif op==17:
            if rs==4:self.fp[rd]=b
            elif rs==0:wr(rt,self.fp[rd])
            elif rs==8:
                assert rt in (0,1,2,3)
                take=self.condition==bool(rt&1);self.branches.add((pc,take))
                if take:branch=pc+4+4*si
                elif rt&2:skip=True
                else:branch=pc+8
            elif rs==16:
                x,y=value(self.fp[rd]),value(self.fp[rt])
                if fn in (0,1,2,3,4,6):
                    if fn==0:z=x+y
                    elif fn==1:z=x-y
                    elif fn==2:z=x*y
                    elif fn==3:assert y!=0;z=x/y
                    elif fn==4:assert x>=0;z=math.sqrt(x)
                    else:z=x
                    assert math.isfinite(z);self.fp[sh]=bits(z)
                elif fn==62:self.condition=x<=y
                else:raise AssertionError(('float',fn,hex(pc)))
            else:raise AssertionError(('COP1',rs,hex(pc)))
        else:raise AssertionError(('opcode',op,hex(pc)))
        if branch is not None:
            assert not delay,'branch in delay slot'
            assert self.step(pc+4,True)==pc+8
            return self.collision() if branch==COLLISION else branch
        return pc+8 if skip else pc+4
    def run(self):
        pc=BASE
        while pc!=RETURN:pc=self.step(pc)
        assert self.r[2]==0 and self.r[29]==STACK
        assert all(self.r[i]==self.initial_r[i] for i in [*range(16,24),28,30])
        assert self.fp[20:]==self.initial_fp[20:]
        allowed=set(range(FIRST,FIRST+12))|set(range(STACK-self.frame-40,STACK+8))
        assert all(v==self.memory[k] for k,v in self.original.items() if k not in allowed)
        return self.vector(FIRST),self.vector(SECOND),self.events

def oracle(case,threshold):
    first=list(map(bits,case['first']));second=list(map(bits,case['second']))
    actual_second=first if case['alias'] else second
    events=[('collision',list(first),list(actual_second))]
    if case['mutate']:first=list(map(bits,case['replacement']))
    actual_second=first if case['alias'] else second
    if case['hit']:
        delta=[f32(value(x)-value(y)) for x,y in zip(actual_second,first)]
        events.append(('normalize',list(map(bits,delta))))
        square=[f32(x*x) for x in delta]
        length=f32(math.sqrt(f32(f32(square[0]+square[1])+square[2])))
        if length>threshold:
            inv=f32(1.0/length);delta=[f32(x*inv) for x in delta]
        first=[bits(f32(f32(x*0.25)+value(y))) for x,y in zip(delta,first)]
    return first,second,events
