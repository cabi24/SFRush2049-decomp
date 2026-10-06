"""Fail-closed, bounded constructor executor. No native words are embedded.

External routines are explicit caller-contract hooks, not executions of the
allocator, scheduler, or list implementation. Only the caller is interpreted.
"""
import struct
MASK=0xffffffff
BASE=0x800D197C
OBJECT=0x100100
STACK=0x200100
STOP=0x300000
QUEUE=0x80142728
LIST=0x80149860
TIME=0x80152748
RECV=0x80007270
ALLOC=0x800D18D8
INSERT=0x80091FBC
JAM=0x800075E0

def signed(v): return v-0x100000000 if v&0x80000000 else v

def fixture(seed, count):
    vals=[(seed*0x9e3779b1+i*0x1020304)&MASK for i in range(32)]
    # Binary32 is transported, never arithmetically evaluated by this routine.
    return {'count':count,'kind':vals[0],'value':vals[1],'arg2':vals[2],
            'arg3':vals[3],'args':vals[4:8],'start':vals[8],
            'initial':bytes(((seed*17+i*31)&255) for i in range(60)),
            'head':0 if seed%3==0 else 0x410000+4*(seed%17),
            'tail':0 if seed%3==0 else 0x420000+4*(seed%13),
            'handle_insert':vals[9],'handle_release':vals[10]}

def oracle(case):
    out=bytearray(case['initial'])
    out[52]=case['kind']&255
    for off,key in [(16,'start'),(20,'value'),(48,'arg2'),(44,'arg3')]:
        struct.pack_into('>I',out,off,case[key])
    out[14]=out[24]=0
    for i in range(4):
        struct.pack_into('>I',out,28+4*i,case['args'][i] if i<case['count'] else MASK)
    struct.pack_into('>I',out,56,MASK)
    at_insert=bytes(out)
    out[13]=1
    struct.pack_into('>I',out,8,case['handle_release'])
    return bytes(out),case['handle_insert'],at_insert

class Machine:
    def __init__(self, words, case):
        self.words=words; self.case=case; self.pc=BASE; self.pending=None
        self.mem={OBJECT+i:b for i,b in enumerate(case['initial'])}
        self.mem.update({STACK+i:(i*37)&255 for i in range(-128,64)})
        self.mem.update({LIST+i:(i*19)&255 for i in range(16)})
        self.mem.update({QUEUE+i:(i*13)&255 for i in range(24)})
        self.mem.update({TIME+i:0 for i in range(4)})
        self.put(LIST+8,4,case['head']);self.put(LIST+12,4,case['tail'])
        self.put(STACK+16,4,case['count']&MASK)
        for i,v in enumerate(case['args']):self.put(STACK+20+4*i,4,v)
        self.original=dict(self.mem)
        self.r=[(0x52310000+i*777)&MASK for i in range(32)]
        self.r[0]=0;self.r[4:8]=[case[k] for k in ['kind','value','arg2','arg3']]
        self.r[29]=STACK;self.r[31]=STOP;self.entry=list(self.r)
        self.f=[(0x45310000+i*101)&MASK for i in range(32)]
        self.visited=set();self.branches={};self.events=[];self.writes=[]
    def get(self,a,n):
        assert a%n==0 and all(a+i in self.mem for i in range(n)),hex(a)
        return int.from_bytes(bytes(self.mem[a+i] for i in range(n)),'big')
    def put(self,a,n,v):
        assert a%n==0 and all(a+i in self.mem for i in range(n)),hex(a)
        for i,b in enumerate((v&((1<<(n*8))-1)).to_bytes(n,'big')):self.mem[a+i]=b
    def body(self):return bytes(self.mem[OBJECT+i] for i in range(60))
    def hook(self):
        c=self.case
        if self.pc==RECV:
            assert self.r[4:7]==[QUEUE,0,1]
            self.events.append('lock');self.put(TIME,4,c['start']);result=0
        elif self.pc==ALLOC:
            assert self.events==['lock']
            self.events.append('allocate');result=OBJECT
        elif self.pc==INSERT:
            assert self.events==['lock','allocate']
            assert self.r[4:7]==[LIST,OBJECT,c['head']]
            assert self.body()==oracle(c)[2]
            self.events.append('prepend');self.put(OBJECT+8,4,c['handle_insert']);result=0
        elif self.pc==JAM:
            assert self.events==['lock','allocate','prepend']
            assert self.r[4:7]==[QUEUE,0,0]
            assert self.get(OBJECT+13,1)==1
            self.events.append('unlock');self.put(OBJECT+8,4,c['handle_release']);result=0
        else:return False
        ret=self.r[31]
        for i in [2,3,*range(4,16),24,25]:self.r[i]=(0xABC00000+i*91+len(self.events))&MASK
        for i in range(16):self.put(self.r[29]+i,1,0xB0+i)
        self.r[2]=result;self.pc=ret;return True
    def run(self):
        for _ in range(2000):
            if self.pc==STOP:break
            if self.hook():continue
            assert BASE<=self.pc<BASE+len(self.words)*4 and (self.pc-BASE)%4==0
            at=self.pc;self.visited.add(at-BASE);w=self.words[(at-BASE)//4]
            op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63
            imm=w&65535;off=imm-65536 if imm&32768 else imm
            branch=None;skip=False
            if op==0:
                if fn==0:self.r[rd]=(self.r[rt]<<sh)&MASK
                elif fn==0x21:self.r[rd]=(self.r[rs]+self.r[rt])&MASK
                elif fn==0x24:self.r[rd]=self.r[rs]&self.r[rt]
                elif fn==0x25:self.r[rd]=self.r[rs]|self.r[rt]
                elif fn==0x2a:self.r[rd]=int(signed(self.r[rs])<signed(self.r[rt]))
                elif fn==8:branch=self.r[rs]
                else:raise AssertionError(('special',hex(w)))
            elif op==3:
                branch=((at+4)&0xf0000000)|((w&0x3ffffff)<<2);self.r[31]=at+8
                assert branch in (RECV,ALLOC,INSERT,JAM)
            elif op==9:self.r[rt]=(self.r[rs]+off)&MASK
            elif op==10:self.r[rt]=int(signed(self.r[rs])<off)
            elif op==15:self.r[rt]=imm<<16
            elif op in (4,5,6,7,20,21):
                cond={4:self.r[rs]==self.r[rt],5:self.r[rs]!=self.r[rt],
                      6:signed(self.r[rs])<=0,7:signed(self.r[rs])>0,
                      20:self.r[rs]==self.r[rt],21:self.r[rs]!=self.r[rt]}[op]
                self.branches.setdefault(at-BASE,set()).add(cond)
                if cond:branch=at+4+4*off
                elif op in (20,21):skip=True
            elif op in (35,36,49):
                v=self.get((self.r[rs]+off)&MASK,1 if op==36 else 4)
                if op==49:self.f[rt]=v
                else:self.r[rt]=v
            elif op in (40,43,57):
                a=(self.r[rs]+off)&MASK;n=1 if op==40 else 4
                assert OBJECT<=a and a+n<=OBJECT+60 or STACK-40<=a and a+n<=STACK+16
                self.writes.append((at-BASE,a,n));self.put(a,n,self.f[rt] if op==57 else self.r[rt])
            else:raise AssertionError(('opcode',hex(w),hex(at)))
            old=self.pending;self.pending=branch
            self.pc=old if old is not None else at+(8 if skip else 4)
            self.r[0]=0
        else:raise AssertionError('step bound')
        expected,result,_=oracle(self.case)
        assert self.events==['lock','allocate','prepend','unlock']
        assert self.body()==expected and self.r[2]==result
        for r in [*range(16,24),28,29,30,31]:assert self.r[r]==self.entry[r],r
        for a,v in self.original.items():
            if OBJECT<=a<OBJECT+60 or TIME<=a<TIME+4 or STACK-40<=a<STACK+16:continue
            assert self.mem[a]==v,hex(a)
        return self.visited,self.branches
