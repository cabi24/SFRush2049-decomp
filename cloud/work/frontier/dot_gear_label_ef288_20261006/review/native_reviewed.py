"""Fail-closed MIPS caller interpreter. Native words are loaded at replay time.

Contract hooks model external calls; no queue, font-bank or renderer internals
are emulated. Callback-order and memory changes are compared to host C.
"""
BASE=0x800EF288
STACK=0x200100
STOP=0x300000
COUNT=0x80151AD0
ENABLE=0x80161394
MODELS=0x8014A250
CARS=0x80152818
POSITIONS=0x80115BE8
ASSET=0x8017A4E0
LABELS=0x300100
LABEL0=0x300500
LABEL1=0x300600
QUEUE=0x801461D0
RENDER=0x800B65B4
RECV=0x80007270
SLOT=0x800B4200
JAM=0x800075E0
COLOR=0x800B74A0
WIDTH=0x800B3FA4
DRAW=0x800B71D4
MASK=0xFFFFFFFF

def signed(v,bits=32):
    v &= (1<<bits)-1
    return v-(1<<bits) if v & (1<<(bits-1)) else v

class Machine:
    def __init__(self,words,args,selector_reg=18):
        self.words=words;self.args=args;self.selector_reg=selector_reg
        self.r=[(0x55550000+i*97)&MASK for i in range(32)];self.r[0]=0
        self.r[4]=0xFEDCBA98;self.r[29]=STACK;self.r[31]=STOP
        self.initial=self.r[:];self.f=[0]*32;self.mem={};self.events=[]
        self.visited=set();self.branches={};self.width_count=0;self.draw_count=0
        self.pc=BASE;self.pending=None
        for addr,n in [(STACK-0x1000,0x1100),(COUNT,2),(ENABLE,1),
                       (MODELS,0x808*4),(CARS,0x3B8*4),(POSITIONS,128),
                       (ASSET,20),(LABELS,820),(LABEL0,16),(LABEL1,16)]:
            self.mem.update({addr+i:0 for i in range(n)})
        self.put(COUNT,2,args[0]);self.put(ENABLE,1,args[1])
        self.put(ASSET+4,4,LABELS);self.put(LABELS+816,4,LABEL0)
        for i in range(4):
            self.put(MODELS+i*0x808+10,1,args[2]>>i&1)
            self.put(MODELS+i*0x808+0x730,1,args[11+i])
            self.put(MODELS+i*0x808+0x7C6,2,3-i)
            self.put(CARS+(3-i)*0x3B8+0xEF,1,args[3]>>i&1)
            for j in range(4):
                self.put(POSITIONS+i*32+j*8,4,args[4]+i*43+j*17)
                self.put(POSITIONS+i*32+j*8+6,2,args[4]+i*23+j*11)
    def get(self,a,n):
        if a%n or not all(a+i in self.mem for i in range(n)):
            raise AssertionError(('invalid read',hex(self.pc),hex(a),n))
        return int.from_bytes(bytes(self.mem[a+i] for i in range(n)),'big')
    def put(self,a,n,v):
        if a%n or not all(a+i in self.mem for i in range(n)):
            raise AssertionError(('invalid write',hex(self.pc),hex(a),n))
        for i,b in enumerate((v&((1<<(8*n))-1)).to_bytes(n,'big')):self.mem[a+i]=b
    def hook(self):
        p=self.pc;a=self.args;r=self.r
        if p==RENDER:
            if self.f[12] not in (0,0xBF800000):raise AssertionError('render float')
            self.events.append([1,0 if self.f[12]==0 else -1,0,0])
            if not self.f[12] and a[5]==1:self.put(COUNT,2,a[6])
        elif p==RECV:
            if r[4:7]!=[QUEUE,0,1]:raise AssertionError('acquire ABI')
            self.events.append([2,1,0,0])
        elif p==SLOT:self.events.append([3,signed(r[self.selector_reg]),0,0])
        elif p==JAM:
            if r[4:7]!=[QUEUE,0,0]:raise AssertionError('release ABI')
            self.events.append([4,0,0,0])
            if a[5]==2:self.put(COUNT,2,a[6])
        elif p==COLOR:self.events.append([5,signed(r[4]),0,0])
        elif p==WIDTH:
            v=a[8] if self.width_count&1 else a[7];self.width_count+=1
            if r[4] not in (LABEL0,LABEL1):raise AssertionError('label pointer')
            self.events.append([6,int(r[4]==LABEL1),signed(r[5]),signed(v)])
            if a[5]==3:self.put(LABELS+816,4,LABEL1)
        elif p==DRAW:
            content=1000 if r[6]==LABEL0 else 1001 if r[6]==LABEL1 else self.get(r[6],1)
            if r[6] not in (LABEL0,LABEL1) and self.get(r[6]+1,1)!=0:raise AssertionError('termination')
            self.events.append([7,signed(r[4],16),signed(r[5],16),content]);self.draw_count+=1
            if self.draw_count==1 and a[5]==4:
                self.put(MODELS+0x730,1,7)
                row=signed(self.get(COUNT,2),16)-1
                self.put(POSITIONS+row*32,4,a[9]);self.put(POSITIONS+row*32+6,2,a[10])
            if self.draw_count==4 and a[5]==5:self.put(COUNT,2,a[6])
        else:return False
        ret=r[31];scratch=[1,2,3,*range(4,16),24,25]
        if p==SLOT and self.selector_reg!=4:scratch += [i for i in (16,17,19) if i!=self.selector_reg]
        for i in scratch:r[i]=(0xACDC0000+i*91+len(self.events))&MASK
        # Ordinary callees may use their four argument-home slots.
        for i in range(16):self.put(r[29]+i,1,0xE0+i)
        r[2]=(v&MASK) if p==WIDTH else ((-17)&MASK)
        self.pc=ret;return True
    def run(self):
        for _ in range(20000):
            if self.pc==STOP:break
            if self.hook():continue
            at=self.pc
            if not BASE<=at<BASE+len(self.words)*4 or (at-BASE)%4:raise AssertionError(('PC',hex(at)))
            self.visited.add(at-BASE)
            w=self.words[(at-BASE)//4];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63;im=w&65535;off=signed(im,16)
            r=self.r;branch=None;skip=False
            if op==0:
                if fn==0:r[rd]=r[rt]<<sh&MASK
                elif fn==2:r[rd]=r[rt]>>sh
                elif fn==3:r[rd]=signed(r[rt])>>sh&MASK
                elif fn==0x21:r[rd]=(r[rs]+r[rt])&MASK
                elif fn==0x23:r[rd]=(r[rs]-r[rt])&MASK
                elif fn==0x24:r[rd]=r[rs]&r[rt]
                elif fn==0x25:r[rd]=r[rs]|r[rt]
                elif fn==0x2A:r[rd]=int(signed(r[rs])<signed(r[rt]))
                elif fn==0x2B:r[rd]=int(r[rs]<r[rt])
                elif fn==8:branch=r[rs]
                else:raise AssertionError(('special',hex(at),fn))
            elif op==3:
                branch=(at+4)&0xF0000000|((w&0x3FFFFFF)<<2);r[31]=at+8
                if branch not in (RENDER,RECV,SLOT,JAM,COLOR,WIDTH,DRAW):raise AssertionError(('call',hex(branch)))
            elif op==9:r[rt]=(r[rs]+off)&MASK
            elif op==10:r[rt]=int(signed(r[rs])<off)
            elif op==11:r[rt]=int(r[rs]<(off&MASK))
            elif op==12:r[rt]=r[rs]&im
            elif op==13:r[rt]=r[rs]|im
            elif op==15:r[rt]=im<<16
            elif op==17:
                if rs!=4:raise AssertionError(('cop1',hex(at),rs))
                self.f[rd]=r[rt]
            elif op in (1,4,5,6,7,20,21,22,23):
                if op==1:
                    if rt not in (0,1):raise AssertionError(('regimm',rt))
                    cond=signed(r[rs])>=0 if rt==1 else signed(r[rs])<0
                elif op in (4,20):cond=r[rs]==r[rt]
                elif op in (5,21):cond=r[rs]!=r[rt]
                elif op in (6,22):cond=signed(r[rs])<=0
                else:cond=signed(r[rs])>0
                self.branches.setdefault(at-BASE,set()).add(cond)
                if cond:branch=at+4+4*off
                elif op in (20,21,22,23):skip=True
            elif op in (32,33,35,36,37):
                n={32:1,33:2,35:4,36:1,37:2}[op];v=self.get((r[rs]+off)&MASK,n)
                r[rt]=(signed(v,n*8)&MASK) if op in (32,33) else v
            elif op in (40,41,43):
                n={40:1,41:2,43:4}[op];addr=(r[rs]+off)&MASK
                if not STACK-0x1000<=addr<=STACK+0x100-n:raise AssertionError(('unexpected native store',hex(addr)))
                self.put(addr,n,r[rt])
            else:raise AssertionError(('opcode',hex(at),op))
            old=self.pending;self.pending=branch;self.pc=old if old is not None else at+(8 if skip else 4);r[0]=0
        else:raise AssertionError('step bound')
        for i in [*range(16,24),29,30]:
            if self.r[i]!=self.initial[i]:raise AssertionError(('callee saved',i))
        self.events.append([8,signed(self.r[2]),signed(self.get(COUNT,2),16),0])
        return self.events
