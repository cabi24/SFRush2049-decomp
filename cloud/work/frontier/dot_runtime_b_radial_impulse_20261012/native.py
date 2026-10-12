"""Bounded field oracle and fail-closed MIPS execution for the radial impulse.

Native private s2 entry and ordinary-C a0 entry are deliberately distinct.
Helpers are deterministic boundary models, not executed retail implementations.
"""
import math
import struct

MASK=0xffffffff
EFFECT,OBJECT,STACK,RETURN=0x81000000,0x81001000,0x82000800,0x8fffffff
PLAYERS,VEHICLES,SCENES,COUNT=0x80152818,0x8014A250,0x8012e700,0x801543ca
DAMAGE,TRANSFORM=0x8038d3a4,0x800a61b0


def require(value,message):
    if not value:raise ValueError(message)


def signed(x,n=32):
    x&=(1<<n)-1
    return x-(1<<n) if x>>(n-1) else x


def bits(v):return struct.unpack('>I',struct.pack('>f',v))[0]
def real(v):return struct.unpack('>f',struct.pack('>I',v))[0]
def f32(v):return real(bits(v))
def add(a,b):return f32(a+b)
def mul(a,b):return f32(a*b)
def div(a,b):return f32(a/b)


class Memory:
    def __init__(self,regions):
        self.regions={base:bytearray(data) for base,data in regions.items()}
        self.writable=set(regions)
    def raw(self,a,n):
        for base,data in self.regions.items():
            if base<=a and a+n<=base+len(data):return data,a-base,base
        raise ValueError('unmapped memory '+hex(a))
    def get(self,a,n=4):
        require(a%n==0,'unaligned read')
        data,off,_=self.raw(a,n);return int.from_bytes(data[off:off+n],'big')
    def put(self,a,v,n=4):
        require(a%n==0,'unaligned write')
        data,off,base=self.raw(a,n);require(base in self.writable,'readonly write')
        data[off:off+n]=(v&((1<<(n*8))-1)).to_bytes(n,'big')
    def fp(self,a):return real(self.get(a))
    def store(self,a,v):self.put(a,bits(v))
    def visible(self):return tuple((a,bytes(b)) for a,b in sorted(self.regions.items()) if a in self.writable and a!=STACK-0x800)


def fixture(case):
    count,radius,owner,mask,variant,mutation=case
    sizes={EFFECT:112,OBJECT:64,PLAYERS:4*952,VEHICLES:4*2056,SCENES:3*68,COUNT:4,STACK-0x800:0x1000}
    m=Memory({a:bytes((i*13+variant*19)&255 for i in range(n)) for a,n in sizes.items()})
    m.put(COUNT,count,2);m.put(EFFECT+4,owner,1);m.put(EFFECT+5,mask,1);m.put(EFFECT+0x60,OBJECT)
    m.put(OBJECT+4,1);m.store(SCENES+68+12,radius)
    for j in range(3):m.store(OBJECT+44+4*j,(j-1)*1.5)
    for i in range(4):
        p,v=PLAYERS+952*i,VEHICLES+2056*i
        m.put(p+859,i,1);m.put(p+776,0 if variant%5==1 and i==2 else 1,1)
        m.put(v+1732,0 if variant%5==2 and i==1 else -1,2)
        # Exact radius-boundary and far-near cases, never zero distance.
        d=[(1.0,2.0,3.0),(4*radius,0.0,0.0),(4*radius+1,0.0,0.0),(-2.0,-1.0,1.0)][(i+variant)%4]
        if radius==0:d=(1.0,0.0,0.0)
        for j in range(3):
            m.store(p+8+4*j,add(m.fp(OBJECT+44+4*j),d[j]))
            m.store(v+292+4*j,(i*4+j-7)*0.25)
            m.store(v+316+4*j,(i*3-j+2)*0.125)
            for k in range(3):m.store(p+44+4*(j*3+k),float(j==k))
    return m


def helper(memory,target,args,mutation,events):
    if target==DAMAGE:
        source,player,magnitude=args
        require(source in [PLAYERS+952*i for i in range(4)] and player in [PLAYERS+952*i for i in range(4)],'damage pointer')
        events.append(('damage',source,player,magnitude,memory.visible()))
        if mutation:
            # Synchronous callee mutations exercise source snapshot and later rereads.
            memory.put(EFFECT+4,(signed(memory.get(EFFECT+4,1),8)+1)%4,1)
            memory.store(player+44,2.0)
            memory.store(VEHICLES+2056*memory.get(player+859,1)+292,17.5)
        return
    require(target==TRANSFORM,'unknown helper '+hex(target))
    source,destination,matrix=args
    vec=[memory.fp(source+4*j) for j in range(3)]
    mat=[memory.fp(matrix+4*j) for j in range(9)]
    events.append(('transform',tuple(bits(x) for x in vec),matrix,memory.visible()))
    # Explicit deterministic matrix stub, shared with the independent field oracle.
    out=[add(add(mul(vec[0],mat[j]),mul(vec[1],mat[3+j])),mul(vec[2],mat[6+j])) for j in range(3)]
    for j in range(3):memory.store(destination+4*j,out[j])
    if mutation:
        player=matrix-44
        memory.put(player+859,(memory.get(player+859,1)+1)%4,1)
        memory.put(COUNT,min(signed(memory.get(COUNT,2),16),3),2)


def oracle(case):
    m=fixture(case);events=[];mutation=case[-1]
    source=PLAYERS+952*signed(m.get(EFFECT+4,1),8)
    obj=m.get(EFFECT+0x60);radius=m.fp(SCENES+68*m.get(obj+4)+12)
    scale,magnitude=(1.,800) if radius<=8 else ((f32(.6),400) if radius<=20 else (.5,200))
    center=[m.fp(m.get(EFFECT+0x60)+44+j*4) for j in range(3)]
    i=0
    while i<signed(m.get(COUNT,2),16):
        p=PLAYERS+i*952;index=signed(m.get(p+859,1),8)
        if index!=signed(m.get(EFFECT+4,1),8) and signed(m.get(VEHICLES+2056*i+1732,2),16)==-1 and signed(m.get(p+776,1),8)!=0 and not (signed(m.get(EFFECT+5,1),8)&(1<<index)):
            delta=[f32(m.fp(p+8+j*4)-center[j]) for j in range(3)]
            distance=add(mul(delta[2],delta[2]),add(mul(delta[0],delta[0]),mul(delta[1],delta[1])))
            extent=mul(radius,4.)
            if distance<=mul(extent,extent):
                m.put(EFFECT+5,m.get(EFFECT+5,1)|(1<<index),1)
                helper(m,DAMAGE,(source,p,magnitude),mutation,events)
                distance=f32(math.sqrt(distance))
                impulse=[mul(mul(div(delta[j],distance),330000.),scale) for j in range(3)]
                impulse[1]=mul(add(mul(div(delta[1],distance),330000.),66000.),scale)
                # Stack vector addresses do not enter observable boundary snapshots.
                input_vector,output_vector=STACK-64,STACK-48
                for j in range(3):m.store(input_vector+j*4,impulse[j])
                helper(m,TRANSFORM,(input_vector,output_vector,p+44),mutation,events)
                delta=[m.fp(output_vector+j*4) for j in range(3)]
                v=VEHICLES+2056*signed(m.get(p+859,1),8)
                for j in range(3):m.store(v+292+j*4,add(delta[j],m.fp(v+292+j*4)))
                delta[1]=0.
                for j in range(3):m.store(v+316+j*4,add(delta[j],m.fp(v+316+j*4)))
        i+=1
    return m.visible(),events


class Machine:
    def __init__(self,code,start,rodata,case,private):
        self.code,self.pc,self.memory,self.case,self.private=code,start,fixture(case),case,private
        for address,data in rodata:
            require(address not in self.memory.regions,'rodata overlap')
            self.memory.regions[address]=bytearray(data)
        self.r=[(0xa5310000+i*37)&MASK for i in range(32)];self.f=[bits(1+i*.125) for i in range(32)]
        self.r[0]=0;self.r[29]=STACK;self.r[31]=RETURN;self.r[18 if private else 4]=EFFECT
        self.initial_r=self.r[:];self.initial_f=self.f[:];self.stack_before=bytes(self.memory.regions[STACK-0x800]);self.events=[];self.coverage=set();self.lo=0;self.fcc=False
    def run(self):
        pending=None
        for step in range(5000):
            pc=self.pc
            if pc==RETURN:break
            require(pc in self.code,'unknown instruction address '+hex(pc));self.coverage.add(pc)
            w=self.code[pc];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63;imm=signed(w,16)
            r,f,m=self.r,self.f,self.memory;a=(r[rs]+imm)&MASK;old=pending;pending=None;nextpc=pc+4
            if op==0:
                if fn==0:r[rd]=r[rt]<<sh&MASK
                elif fn==3:r[rd]=signed(r[rt])>>sh&MASK
                elif fn==4:r[rd]=r[rt]<<(r[rs]&31)&MASK
                elif fn==8:pending=r[rs]
                elif fn==18:r[rd]=self.lo
                elif fn==25:self.lo=r[rs]*r[rt]&MASK
                elif fn==33:r[rd]=(r[rs]+r[rt])&MASK
                elif fn==35:r[rd]=(r[rs]-r[rt])&MASK
                elif fn==36:r[rd]=r[rs]&r[rt]
                elif fn==37:r[rd]=r[rs]|r[rt]
                elif fn==42:r[rd]=int(signed(r[rs])<signed(r[rt]))
                elif fn==43:r[rd]=int(r[rs]<r[rt])
                else:raise ValueError('unsupported SPECIAL '+str(fn))
            elif op==3:r[31]=pc+8;pending=('call',((pc+4)&0xf0000000)|((w&0x3ffffff)<<2),pc+8)
            elif op==9:r[rt]=a
            elif op==10:r[rt]=int(signed(r[rs])<imm)
            elif op==12:r[rt]=r[rs]&(w&65535)
            elif op==13:r[rt]=r[rs]|(w&65535)
            elif op==15:r[rt]=(w&65535)<<16
            elif op in (4,5,6,7,20,21,22,23):
                condition={4:r[rs]==r[rt],5:r[rs]!=r[rt],6:signed(r[rs])<=0,7:signed(r[rs])>0}[op&15]
                if condition:pending=pc+4+imm*4
                elif op>=20:nextpc+=4
            elif op in (32,33,35,36,37):
                n=4 if op==35 else 2 if op in (33,37) else 1;v=m.get(a,n);r[rt]=(signed(v,n*8) if op in (32,33) else v)&MASK
            elif op in (40,41,43):m.put(a,r[rt],{40:1,41:2,43:4}[op])
            elif op in (49,53):
                f[rt]=m.get(a)
                if op==53:f[rt+1]=m.get(a+4)
            elif op in (57,61):
                m.put(a,f[rt])
                if op==61:m.put(a+4,f[rt+1])
            elif op==17:
                if rs==4:f[rd]=r[rt]
                elif rs==8:
                    condition=self.fcc if rt&1 else not self.fcc
                    if condition:pending=pc+4+imm*4
                    elif rt&2:nextpc+=4
                elif rs==16:
                    x,y=real(f[rd]),real(f[rt])
                    if fn==0:f[sh]=bits(add(x,y))
                    elif fn==1:f[sh]=bits(f32(x-y))
                    elif fn==2:f[sh]=bits(mul(x,y))
                    elif fn==3:f[sh]=bits(div(x,y))
                    elif fn==4:f[sh]=bits(f32(math.sqrt(x)))
                    elif fn==6:f[sh]=f[rd]
                    elif fn==7:f[sh]=f[rd]^0x80000000
                    elif fn==60:self.fcc=x<y
                    elif fn==62:self.fcc=x<=y
                    else:raise ValueError('unsupported COP1 operation '+str(fn))
                else:raise ValueError('unsupported COP1 format '+str(rs))
            else:raise ValueError('unsupported opcode '+str(op))
            r[0]=0;require(old is None or pending is None,'branch inside delay slot')
            if isinstance(old,tuple):
                helper(m,old[1],tuple(r[4:7]),self.case[-1],self.events)
                for i in list(range(1,16))+[24,25]:r[i]=(0xbad00000+i*97)&MASK
                for i in range(20):f[i]=0x7fc00000+i
                self.pc=old[2]
            else:self.pc=old if old is not None else nextpc
        else:raise ValueError('execution bound')
        require(self.r[29]==STACK and self.r[28]==self.initial_r[28],'stack/gp corruption')
        frame=104 if self.private else 184;now=bytes(self.memory.regions[STACK-0x800])
        require(now[:0x800-frame]==self.stack_before[:0x800-frame] and now[0x810:]==self.stack_before[0x810:],'stack red-zone corruption')
        if self.private:require(self.r[18]==self.initial_r[18],'private effect register corruption')
        else:
            require(self.r[16:24]==self.initial_r[16:24] and self.r[30]==self.initial_r[30],'callee-save GPR corruption')
            require(self.f[20:]==self.initial_f[20:],'callee-save FPR corruption')
        return self.memory.visible(),self.events


def cases(deep):
    if not deep:return [(4,8.,0,0,0,0),(4,20.,1,0x80,3,1),(0,21.,2,255,1,0),(3,21.,0,0,4,0)]
    return [(count,radius,owner,mask,variant,mutation) for count in (0,1,3,4) for radius in (0.,4.,8.,8.5,20.,20.5)
            for owner in (0,2) for mask in (0,1,4,0x80,0xff) for variant in range(5) for mutation in (0,1)]


def verify(native_code,candidate_code,candidate_data,deep=False):
    cov=[set(),set()];count=0
    for case in cases(deep):
        expect=oracle(case)
        for i,(code,data,private) in enumerate(((native_code,[(0x80394dc8,struct.pack('>3f',.6,66000,330000))],True),(candidate_code,candidate_data,False))):
            machine=Machine(code,0x8038d498,data,case,private);actual=machine.run()
            require(actual==expect,'semantic mismatch '+str((case,'native' if private else 'candidate')))
            cov[i].update(machine.coverage)
        count+=1
    missing_native=sorted(set(native_code)-cov[0]);missing_candidate=sorted(set(candidate_code)-cov[1])
    require(missing_native==[0x8038d520,0x8038d548],'unexplained native coverage gap')
    require(len(missing_candidate)==1,'unexplained candidate coverage gap')
    return dict(fixtures=count,executions=count*2,native_instructions_executed=len(cov[0]),candidate_instructions_executed=len(cov[1]),native_unreachable_offsets=[x-0x8038d498 for x in missing_native],candidate_unreachable_offsets=[x-0x8038d498 for x in missing_candidate])
