"""Fail-closed full-body native/candidate replay. No target words stored here.

Ordinary helpers have explicit bounded contracts; rounded host atan2/supplied
outputs do not claim equivalence to the N64 approximation or its dependencies.
"""
import hashlib
import math
import random
import struct

MASK=0xFFFFFFFF
ENTRY,STACK,RETURN=0x8038DA78,0x70008000,0xFFFFFFFC
PLAYERS,VEHICLES,COUNT=0x80152818,0x8014A250,0x801543CA
RECORD,PREVIOUS=0x100000,0x110000

def signed(x,n=32):
    x&=(1<<n)-1
    return x-(1<<n) if x&(1<<(n-1)) else x

def bits(x):return struct.unpack('>I',struct.pack('>f',x))[0]
def real(x):return struct.unpack('>f',struct.pack('>I',x))[0]

class Machine:
    def __init__(self,code,start,data,case,private):
        self.code,self.start,self.case,self.private=code,start,case,private
        self.memory,self.writable,self.state={},{},set()
        self.coverage,self.branches,self.trace=set(),set(),[]
        self.r=[(0xA5000000+i*397)&MASK for i in range(32)]
        self.f=[bits(1.0+i*0.125) for i in range(32)]
        self.r[0],self.r[29],self.r[31]=0,STACK,RETURN
        self.lo=self.hi=0;self.condition=False;self.calls=0
        for a,raw in data:
            for i,b in enumerate(raw):self.memory[a+i]=b
        for a,n in [(PLAYERS,952*4),(VEHICLES,2056*4),(COUNT,2),(RECORD,104),(PREVIOUS,12)]:
            self.map(a,n,True)
        self.map(STACK-4096,8192,False)
        self.previous=RECORD+44 if case.get('alias_previous',0) else PREVIOUS
        self.r[17 if private else 4]=RECORD
        self.r[18 if private else 5]=self.previous
        self.original_r,self.original_f=self.r[:],self.f[:]
        self.put(COUNT,case.get('count',1),2)
        self.put(RECORD+4,case.get('owner',-1),1)
        self.put(RECORD+6,case.get('kind',0),1)
        self.put(RECORD+7,case.get('flags',0),1)
        self.put(RECORD+16,bits(case.get('radius',0.5)))
        for i,x in enumerate(case.get('position',(10.,0.,0.))):self.put(RECORD+32+4*i,bits(x))
        for i,x in enumerate(case.get('previous',(0.,0.,0.))):self.put(self.previous+4*i,bits(x))
        entries=case.get('players',[{}])
        for i in range(4):
            c=entries[i] if i<len(entries) else {};p=PLAYERS+952*i;v=VEHICLES+2056*i
            self.put(p+0x35B,c.get('owner',i),1)
            self.put(p+0x308,c.get('active',1),1)
            self.put(p+0x384,c.get('kind',0),1)
            self.put(v+0x640,c.get('blocked',0),1)
            self.put(v+0x6C4,c.get('state',-1),2)
            for j,x in enumerate(c.get('position',(5.,0.,0.))):self.put(p+8+4*j,bits(x))
            for j,x in enumerate(c.get('uv',(1.,0.,0.,0.,1.,0.,0.,0.,1.))):self.put(p+44+4*j,bits(x))
        self.state_order=sorted(self.state)
    def map(self,a,n,state):
        for i in range(n):
            self.memory[a+i]=(i*13+7)&255;self.writable[a+i]=True
            if state:self.state.add(a+i)
    def get(self,a,n=4):
        assert a%n==0,('unaligned read',hex(a),n)
        assert all(a+i in self.memory for i in range(n)),('unmapped read',hex(a),n)
        return int.from_bytes(bytes(self.memory[a+i] for i in range(n)),'big')
    def put(self,a,x,n=4):
        assert a%n==0,('unaligned write',hex(a),n)
        assert all(a+i in self.writable for i in range(n)),('unmapped/readonly write',hex(a),n)
        for i,b in enumerate((x&((1<<(8*n))-1)).to_bytes(n,'big')):self.memory[a+i]=b
    def vec(self,a,n=3):return tuple(self.get(a+4*i) for i in range(n))
    def float_at(self,a):return real(self.get(a))
    def state_digest(self):return hashlib.sha256(bytes(self.memory[a] for a in self.state_order)).hexdigest()
    def snapshot(self,name,*args):self.trace.append((name,*args,self.state_digest()))
    def call(self,target):
        r,f=self.r,self.f;fret=None
        if target==0x800A61B0:
            src,dst,mat=r[4:7];v=self.vec(src);m=self.vec(mat,9)
            self.snapshot('vector_transform',v,mat,m)
            for i in range(3):
                products=[real(bits(real(v[j])*real(m[3*i+j]))) for j in range(3)]
                self.put(dst+4*i,bits(real(bits(products[0]+products[1]))+products[2]))
        elif target==0x8008C768:
            self.snapshot('angle',f[12],f[14])
            fret=bits(self.case.get('angle',math.atan2(real(f[12]),real(f[14]))))
            self.calls+=1
            if self.case.get('mutation'):
                # Deliberately widened service effects probe later reads. These
                # writes are not asserted to be actual transform/atan effects.
                self.put(COUNT,self.case.get('mutated_count',3),2)
                self.put(RECORD+7,0xA0+self.calls,1)
                self.put(RECORD+32,bits(25.0))
        else:raise AssertionError(('unknown call',hex(target)))
        for k in list(range(1,16))+[24,25]:r[k]=(0xBAD00000+k*79)&MASK
        for k in range(20):f[k]=0x7FC00000+k
        r[2]=0xBAD00002
        if fret is not None:f[0]=fret
    def run(self):
        pc,pending=self.start,None;r,f=self.r,self.f
        for steps in range(20000):
            if pc==RETURN:break
            assert pc in self.code,('unknown pc',hex(pc))
            self.coverage.add(pc);w=self.code[pc];op=w>>26
            rs,rt,rd,sh=(w>>21)&31,(w>>16)&31,(w>>11)&31,(w>>6)&31
            imm,si=w&65535,signed(w,16);a=(r[rs]+si)&MASK
            old,pending,next_pc=pending,None,pc+4
            if op==0:
                fn=w&63
                if fn==0:r[rd]=(r[rt]<<sh)&MASK
                elif fn==2:r[rd]=r[rt]>>sh
                elif fn==3:r[rd]=(signed(r[rt])>>sh)&MASK
                elif fn==0x21:r[rd]=(r[rs]+r[rt])&MASK
                elif fn==0x23:r[rd]=(r[rs]-r[rt])&MASK
                elif fn==0x24:r[rd]=r[rs]&r[rt]
                elif fn==0x25:r[rd]=r[rs]|r[rt]
                elif fn==0x2a:r[rd]=int(signed(r[rs])<signed(r[rt]))
                elif fn==0x2b:r[rd]=int(r[rs]<r[rt])
                elif fn==0x19:
                    v=r[rs]*r[rt];self.lo=v&MASK;self.hi=(v>>32)&MASK
                elif fn==0x12:r[rd]=self.lo
                elif fn==8:pending=r[rs]
                else:raise AssertionError(('SPECIAL',hex(pc),hex(w)))
            elif op==3:
                r[31]=pc+8;pending=('call',((pc+4)&0xF0000000)|((w&0x3FFFFFF)<<2),pc+8)
            elif op==2:pending=((pc+4)&0xF0000000)|((w&0x3FFFFFF)<<2)
            elif op==9:r[rt]=a
            elif op==10:r[rt]=int(signed(r[rs])<si)
            elif op==11:r[rt]=int(r[rs]<(si&MASK))
            elif op==12:r[rt]=r[rs]&imm
            elif op==13:r[rt]=r[rs]|imm
            elif op==15:r[rt]=imm<<16
            elif op in (4,5,6,7,20,21,22,23):
                if op in (4,20):take=r[rs]==r[rt]
                elif op in (5,21):take=r[rs]!=r[rt]
                elif op in (6,22):take=signed(r[rs])<=0
                else:take=signed(r[rs])>0
                self.branches.add((pc,take))
                if take:pending=pc+4+si*4
                elif op>=20:next_pc+=4
            elif op==1:
                assert rt in (0,1,2,3)
                take=signed(r[rs])<0 if rt in (0,2) else signed(r[rs])>=0
                self.branches.add((pc,take))
                if take:pending=pc+4+si*4
                elif rt>=2:next_pc+=4
            elif op in (32,33,35,36,37):
                n=4 if op==35 else 2 if op in (33,37) else 1
                v=self.get(a,n);r[rt]=(signed(v,n*8)&MASK) if op in (32,33) else v
            elif op in (40,41,43):self.put(a,r[rt],{40:1,41:2,43:4}[op])
            elif op==49:f[rt]=self.get(a)
            elif op==57:self.put(a,f[rt])
            elif op==53:f[rt]=self.get(a);f[rt+1]=self.get(a+4)
            elif op==61:self.put(a,f[rt]);self.put(a+4,f[rt+1])
            elif op==17:
                fn=w&63
                if rs==4:f[rd]=r[rt]
                elif rs==0:r[rt]=f[rd]
                elif rs==8:
                    assert rt in (0,1,2,3)
                    take=self.condition if rt&1 else not self.condition
                    self.branches.add((pc,take))
                    if take:pending=pc+4+si*4
                    elif rt&2:next_pc+=4
                elif rs==16:
                    if fn==0:f[sh]=bits(real(f[rd])+real(f[rt]))
                    elif fn==1:f[sh]=bits(real(f[rd])-real(f[rt]))
                    elif fn==2:f[sh]=bits(real(f[rd])*real(f[rt]))
                    elif fn==3:f[sh]=bits(real(f[rd])/real(f[rt]))
                    elif fn==4:f[sh]=bits(math.sqrt(real(f[rd])))
                    elif fn==5:f[sh]=f[rd]&0x7FFFFFFF
                    elif fn==6:f[sh]=f[rd]
                    elif fn==7:f[sh]=f[rd]^0x80000000
                    elif fn==13:f[sh]=math.trunc(real(f[rd]))&MASK
                    elif fn==0x32:self.condition=real(f[rd])==real(f[rt])
                    elif fn==0x3c:self.condition=real(f[rd])<real(f[rt])
                    elif fn==0x3e:self.condition=real(f[rd])<=real(f[rt])
                    else:raise AssertionError(('COP1 single',hex(pc),hex(w)))
                elif rs==20 and fn==32:f[sh]=bits(signed(f[rd]))
                else:raise AssertionError(('COP1',hex(pc),hex(w)))
            else:raise AssertionError(('opcode',hex(pc),hex(w)))
            r[0]=0
            assert old is None or pending is None,'branch in delay slot'
            if isinstance(old,tuple):self.call(old[1]);pc=old[2]
            else:pc=old if old is not None else next_pc
        else:raise AssertionError('step limit')
        assert r[29]==STACK and r[28]==self.original_r[28]
        if not self.private:assert r[16:24]==self.original_r[16:24] and r[30]==self.original_r[30]
        if not self.private:assert f[20:]==self.original_f[20:]
        return self.r[2],self.state_digest(),self.trace

def fixtures():
    yield {}
    for count in (-32768,-1,0,1,2,3,4):yield dict(count=count)
    for kind in (-128,-1,0,1,2,3,4,5,6,7,8,127):yield dict(kind=kind)
    for key,values in [('owner',(-128,-1,0,1,127)),('active',(-128,-1,0,1,127)),
       ('blocked',(-128,-1,0,1,127)),('state',(-32768,-2,-1,0,1,32767)),
       ('kind',(-128,-1,0,4,5,6,127))]:
        for x in values:yield dict(players=[{key:x}])
    for radius in (-7.,-0.5,0.,0.5,5.):
        for x in (-0.25,0.,2.5,10.,10.25):
            for y in (-9.,-4.125,-4.,-0.5,0.,0.5,0.625,7.):
                for z in (-7.,-6.75,-6.625,0.,6.625,6.75,7.):
                    yield dict(radius=radius,players=[dict(position=(x,y,z))])
    for x in (0.,-0.,0.015625,0.03125,0.03162277862429619,0.0625):
        for z in (0.,0.015625,0.03125):yield dict(position=(x,1.,z))
    for flags in (0,1,2,0x7F,0x80,0xFF):
        for angle in (-2.,-1.3500001430511475,-1.350000023841858,-1.3499999046325684,
                      -0.,0.,1.3499999046325684,1.350000023841858,1.3500001430511475,2.):
            yield dict(flags=flags,angle=angle,players=[dict(kind=5)])
    for xs in ((8.,5.,2.,1.),(1.,2.,5.,8.),(5.,5.,5.,5.),(-1.,12.,5.,3.),(0.,0.,0.,0.)):
        for pk in (0,5):
            for alias in (0,1):
                yield dict(count=4,alias_previous=alias,angle=0.,players=[dict(position=(x,0.,0.),kind=pk) for x in xs])
    for count in (0,1,2,3,4):
        yield dict(count=1,mutation=1,mutated_count=count,angle=0.,players=[dict(kind=5)]*4)
    yield dict(count=4,players=[dict(position=(8.,0.,0.)),dict(position=(5.,0.,0.),kind=5),dict(position=(2.,0.,0.)),dict(position=(1.,0.,0.),kind=5)])
    rng=random.Random(0xDA78)
    for _ in range(250):
        yield dict(count=rng.randrange(1,5),alias_previous=rng.randrange(2),flags=rng.randrange(256),
            radius=rng.choice([0.,0.5,1.,3.]),owner=rng.randrange(-1,4),
            previous=tuple(rng.randrange(-8,9)*0.25 for j in range(3)),
            position=tuple(rng.randrange(-32,33)*0.25 for j in range(3)),
            players=[dict(position=tuple(rng.randrange(-24,25)*0.25 for j in range(3)),
                kind=rng.choice([0,5]),owner=rng.randrange(4),active=rng.choice([0,1,-1]),
                state=rng.choice([-1,-1,0]),blocked=rng.choice([0,0,1]),
                uv=tuple(rng.randrange(-8,9)*0.125 for j in range(9))) for i in range(4)])

def verify(native,tables,code,start,candidate_data,cases=None):
    nc,cc,nb,cb=set(),set(),set(),set();count=0;digest=hashlib.sha256()
    for case in fixtures() if cases is None else cases:
        a=Machine(native,ENTRY,tables,case,True)
        b=Machine(code,start,candidate_data,case,False)
        aa=a.run();bb=b.run()
        if aa!=bb:
            changed=[(hex(x),a.memory[x],b.memory[x]) for x in a.state_order if a.memory[x]!=b.memory[x]]
            raise AssertionError(('semantic mismatch',case,aa,bb,changed[:20]))
        digest.update(repr(aa).encode())
        nc.update(a.coverage);cc.update(b.coverage);nb.update(a.branches);cb.update(b.branches);count+=1
    return {'paired_cases':count,'behavior_sha256':digest.hexdigest(),
      'native_executed_offsets':[hex(x-ENTRY) for x in sorted(nc)],
      'native_unexecuted_offsets':[hex(x-ENTRY) for x in sorted(set(native)-nc)],
      'candidate_executed_offsets':[hex(x-start) for x in sorted(cc)],
      'candidate_unexecuted_offsets':[hex(x-start) for x in sorted(set(code)-cc)],
      'native_branch_outcomes':len(nb),'candidate_branch_outcomes':len(cb),
      'domain':'aligned initialized disjoint record/players/vehicles; previous aliases record+44 or disjoint; signed count<=4; zero/finite normal inputs and intermediates/results; default rounding; no overflow, underflow, NaN, infinity, denormal or exception/FCSR effects; bounded helper contracts'}
