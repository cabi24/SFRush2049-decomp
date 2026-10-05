"""Bounded integer MIPS-II native oracle with audited O32 call models.

No native instructions are embedded. Unknown instructions, escaped flow,
unmapped/unaligned memory and O32 callee-save damage fail closed. This does
not emulate actual external callee internals, asynchronous threads, or a ROM.
"""
import struct
MASK=0xffffffff
BASE=0x80108da8
BLIT=0x100000
STACK=0x200100
RETURN=0x70000000
COUNT=0x80151ad0
CARS=0x801543ca
FLAG=0x80156ce8
STATE=0x801174b4
OBJECTS=0x80152818
TABLE=0x80116028
OUT_W=0x8011617c
OUT_H=0x80116180
UPDATE=0x80094ec8
SELECT=0x800ef5b0

def signed(n,bits=32):
    n &= (1<<bits)-1
    return n-(1<<bits) if n&(1<<(bits-1)) else n

def coordinate(row,slot,axis):
    return 10000*(row+1)+100*slot+7*axis-20000

class Machine:
    def __init__(self,words,args):
        self.words=words; self.mem={}; self.events=[]; self.updates=0
        self.mutation=args[7]; self.steps=0; self.visited=set(); self.global_reads=[]
        self.r=[0xa5000000+i for i in range(32)]; self.r[0]=0
        self.r[4]=BLIT; self.r[29]=STACK; self.r[31]=RETURN
        self.original=list(self.r)
        for base,size in [(BLIT,48),(STACK-128,160),(OBJECTS,952*6),(TABLE,128),
                          (COUNT,2),(CARS,2),(FLAG,1),(STATE,4),(OUT_W,4),(OUT_H,4)]:
            for i in range(size): self.mem[base+i]=0
        for off,width,val in [(44,4,args[0]),(40,4,0x400000),(26,1,args[6]),
                              (14,2,-123),(16,2,234),(20,2,-300),(22,2,32767),(24,1,17)]:
            self.put(BLIT+off,width,val)
        for addr,width,val in [(COUNT,2,args[1]),(STATE,4,args[2]),(CARS,2,args[3]),
                               (FLAG,1,args[4]),(OUT_W,4,777),(OUT_H,4,-888)]:
            self.put(addr,width,val)
        if args[0]<6: self.put(OBJECTS+args[0]*952+239,1,args[5])
        for row in range(4):
            for slot in range(4):
                for axis in range(2): self.put(TABLE+32*row+8*slot+4*axis,4,coordinate(row,slot,axis))
    def get(self,addr,width):
        assert addr%width==0 and all(addr+i in self.mem for i in range(width)),hex(addr)
        if addr in (COUNT,CARS): self.global_reads.append(addr)
        return int.from_bytes(bytes(self.mem[addr+i] for i in range(width)),'big')
    def put(self,addr,width,val):
        assert addr%width==0 and all(addr+i in self.mem for i in range(width)),hex(addr)
        for i,v in enumerate((val&((1<<(8*width))-1)).to_bytes(width,'big')):self.mem[addr+i]=v
    def field(self,off,width=2,sign=True):
        x=self.get(BLIT+off,width); return signed(x,8*width) if sign else x
    def external(self,pc):
        assert self.r[4]==BLIT
        kind={UPDATE:1,SELECT:2}[pc]
        self.events += [kind,self.field(26,1),self.field(14),self.field(16),
                        self.field(20),self.field(22),self.field(24,1,False),int(bool(self.field(40,4)))]
        if kind==1:
            self.updates+=1
            if self.updates==1:
                if self.mutation in (1,4): self.put(BLIT+26,1,0)
                if self.mutation==2:self.put(BLIT+26,1,-1)
                if self.mutation in (3,4):
                    self.put(COUNT,2,4 if self.mutation==3 else 2)
                    self.put(BLIT+20,2,-3000);self.put(BLIT+22,2,5000)
        else:
            assert self.r[5]==0x80120e34 and self.r[6]==0
            self.put(BLIT+20,2,1234);self.put(BLIT+22,2,-2345)
        for i in [1,*range(2,16),24,25]:self.r[i]=0xbad00000+i
    def wr(self,i,val):
        if i:self.r[i]=val&MASK
    def step(self,pc,delay=False):
        assert BASE<=pc<BASE+408 and pc%4==0,hex(pc)
        self.steps+=1;self.visited.add(pc);assert self.steps<250
        w=self.words[(pc-BASE)//4]
        op,rs,rt,rd,sh,fn=w>>26,w>>21&31,w>>16&31,w>>11&31,w>>6&31,w&63
        imm=w&65535;simm=signed(imm,16);a,b=self.r[rs],self.r[rt]
        branch=None;skip=False
        if op==0:
            if fn==0:self.wr(rd,b<<sh)
            elif fn==33:self.wr(rd,a+b)
            elif fn==35:self.wr(rd,a-b)
            elif fn==37:self.wr(rd,a|b)
            elif fn==38:self.wr(rd,a^b)
            elif fn==42:self.wr(rd,int(signed(a)<signed(b)))
            elif fn==8:branch=a
            else:raise AssertionError(('unknown SPECIAL',fn))
        elif op==9:self.wr(rt,a+simm)
        elif op==10:self.wr(rt,int(signed(a)<simm))
        elif op==11:self.wr(rt,int(a<(simm&MASK)))
        elif op==12:self.wr(rt,a&imm)
        elif op==15:self.wr(rt,imm<<16)
        elif op in (4,5,20,21):
            take=(a==b)==(op in (4,20))
            if take:branch=pc+4+4*simm
            elif op in (20,21):skip=True
            else:branch=pc+8
        elif op==3:
            branch=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2);self.r[31]=pc+8
        elif op in (32,33,35,40,41,43):
            addr=(a+simm)&MASK;width={32:1,33:2,35:4,40:1,41:2,43:4}[op]
            if op<40:self.wr(rt,signed(self.get(addr,width),width*8))
            else:self.put(addr,width,b)
        else:raise AssertionError(('unknown opcode',op))
        if branch is not None:
            assert not delay,'branch in delay slot';assert self.step(pc+4,True)==pc+8
            if branch in (UPDATE,SELECT):self.external(branch);return self.r[31]
            return branch
        return pc+8 if skip else pc+4
    def run(self):
        pc=BASE
        while pc!=RETURN:pc=self.step(pc)
        assert self.r[29]==self.original[29] and self.r[16:24]==self.original[16:24]
        assert self.r[28]==self.original[28] and self.r[30]==self.original[30]
        out=[signed(self.r[2]),self.field(26,1),int(bool(self.field(40,4))),self.field(14),self.field(16),
             self.field(20),self.field(22),self.field(24,1,False),signed(self.get(OUT_W,4)),
             signed(self.get(OUT_H,4)),signed(self.get(COUNT,2),16),len(self.events)//8]+self.events
        return out+[0]*(36-len(out))

def reference(args):
    slot,count,state,cars,enabled,mode,hide,mutation=args
    b={'hide':hide,'callback':1,'x':-123,'y':234,'width':-300,'height':32767,'alpha':17}
    events=[];updates=0;outw=777;outh=-888
    def call(kind):
        nonlocal count,updates
        events.extend([kind,b['hide'],b['x'],b['y'],b['width'],b['height'],b['alpha'],b['callback']])
        if kind==2:b['width'],b['height']=1234,-2345
        else:
            updates+=1
            if updates==1:
                if mutation in (1,4):b['hide']=0
                if mutation==2:b['hide']=-1
                if mutation in (3,4):
                    count=4 if mutation==3 else 2
                    b['width'],b['height']=-3000,5000
    if slot>=count or state&8 or cars<2:
        b['callback']=0
        if b['hide']!=1:b['hide']=1;call(1)
        ret=b['hide']
    else:
        desired=int(enabled==0 or mode==1)
        if desired!=b['hide']:b['hide']=desired;call(1)
        if not b['hide']:
            b['x']=signed(coordinate(count-1,slot,0),16)
            b['y']=signed(coordinate(count-1,slot,1),16)
            if count>=2:call(2);b['alpha']=96
            call(1)
            outw,outh=b['width'],b['height']
        ret=1
    out=[ret,b['hide'],b['callback'],b['x'],b['y'],b['width'],b['height'],b['alpha'],
         outw,outh,count,len(events)//8]+events
    return out+[0]*(36-len(out))
