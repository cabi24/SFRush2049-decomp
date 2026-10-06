"""Bounded full E114/native-to-compiled behavior with explicit helper contracts.

No native target words are stored. Helpers are deterministic effect models, not
claims that private DA78/D498 or unrelated services have been reconstructed.
"""
import hashlib
import math
import random
import struct

MASK=0xFFFFFFFF
ENTRY,STACK,RETURN=0x8038E114,0x70008000,0xFFFFFFFC
PLAYERS,VEHICLES=0x80152818,0x8014A250
RECORDS,OBJECTS,RELEASES,INPUTS,EFFECTS=0x100000,0x110000,0x120000,0x130000,0x140000

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
        self.original_r,self.original_f=self.r[:],self.f[:]
        self.lo=self.hi=0;self.condition=False;self.nalloc=0
        for a,raw in data:
            for i,b in enumerate(raw):self.memory[a+i]=b
        for a,n in [(PLAYERS,952*2),(VEHICLES,2056*2),(RECORDS,104*3),
          (OBJECTS,60*12),(RELEASES,8*3),(INPUTS,64*2),(EFFECTS,32*3),
          (0x8039A520,20),(0x8039A5F8,20),(0x8002EB94,4),(0x80142764,4),
          (0x8012E700,68*16),(0x8012E67C,2),(0x80151AE8,8*2),(0x150000,36*4),
          (0x160000,36*4),(0x80399AF8,4),(0x80394F88,2),(0x80140BDC,1),
          (0x803942E8,4),(0x801141B0,36),(0x80394CFC,12),(0x80394D08,12)]:
            self.map(a,n,True)
        self.map(STACK-4096,8192,False)
        self.put(0x8002EB94,bits(case.get('dt',0.125)))
        self.put(0x80142764,bits(2.0))
        self.put(0x8039A530,RECORDS if case.get('count',1) else 0)
        self.put(0x8039A608,0)
        self.put(0x80394F88,case.get('resource',0),2)
        self.put(0x80140BDC,case.get('resource_count',2),1)
        self.put(0x803942E8,0x12345678)
        for i in range(2):
            p=PLAYERS+952*i;v=VEHICLES+2056*i;inp=INPUTS+64*i
            self.put(p+0x380,inp if case.get('input',1) else 0)
            for off,key,d in [(0x384,'player_kind',3),(0x385,'ammo',2),
              (0x308,'active',1),(0x359,'blocked',0),(0x3A1,'alpha',48)]:self.put(p+off,case.get(key,d),1)
            self.put(v+0x640,case.get('vehicle_blocked',0),1)
            self.put(v+8,case.get('model',i),1)
            self.put(p+0x3A4,case.get('latched',0));self.put(p+0x38C,case.get('status',0))
            self.put(p+0x34C,0x12345678+i);self.put(inp+4,case.get('pressed',0));self.put(inp+0x3C,1)
            self.put(0x8012E67C+i,i,1);self.put(0x80399AF8+2*i,((1-i)<<10)|i,2)
            self.put(0x80151AE8+8*i,0x150000+0x10000*i)
            for j in range(3):
                self.put(p+8+4*j,bits((i+1)*(j+2)*0.25));self.put(p+20+4*j,bits((j-1)*0.75))
                for k in range(3):self.put(p+44+12*j+4*k,bits(1.0 if j==k else (j-k)*0.125))
        for i in range(12):
            self.put(OBJECTS+i*60+4,i+1)
            for j in range(9):self.put(OBJECTS+i*60+8+4*j,bits((j+1)*0.125))
        for i in range(16):self.put(0x8012E700+68*i+12,bits(i*0.25))
        for j in range(9):self.put(0x801141B0+4*j,bits(1.0 if j%4==0 else 0.0))
        for i in range(3):
            p=RECORDS+104*i
            self.put(p,RECORDS+104*(i+1) if i+1<case.get('count',1) else 0)
            self.put(p+4,case.get('owner',0),1);self.put(p+6,case.get('kind',0),1);self.put(p+7,case.get('flags',0),1)
            for off,key,d in [(8,'life',2.0),(12,'timer',0.25),(16,'extent',0.75)]:self.put(p+off,bits(case.get(key,d)))
            for j in range(3):
                self.put(p+20+4*j,bits(case.get('velocity',12.0)+(j-1)*0.75))
                self.put(p+32+4*j,bits((j+1)*0.25+i));self.put(p+44+4*j,bits((j-1)*0.375))
                for k in range(3):self.put(p+56+12*j+4*k,bits(1.0 if j==k else (j-k)*0.125))
            self.put(p+92,EFFECTS+32*i if case.get('effect',1) else 0)
            self.put(p+96,OBJECTS+120*i if case.get('primary',1) else 0)
            self.put(p+100,OBJECTS+120*i+60)
            self.put(EFFECTS+32*i+8,0x200)
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
    def owner_record(self):
        if self.private:return self.r[21]
        # Current source record identified from the live input to this hook.
        return self.current_record
    def snapshot(self,name,*args):self.trace.append((name,*args,self.state_digest()))
    def call(self,target):
        r,f=self.r,self.f;ret=0xBAD00002;fret=None;mut=self.case.get('mutation',0)
        sp=r[29]
        if target==0x8038DA78:
            rec,prev=(r[17],r[18]) if self.private else r[4:6]
            self.current_record=rec
            self.snapshot('private_collision',rec,'record_previous' if prev==rec+44 else 'previous',self.vec(prev))
            if mut:
                self.put(rec,0xDEAD0000)
                self.put(rec+32,bits(self.float_at(rec+32)+0.25))
            ret=PLAYERS+952 if self.case.get('hit',0) else 0
            if self.private:
                for k in list(range(16,24))+[30]:r[k]=0xBAD10000+k
                for k in range(20,32):f[k]=bits(10.0+k)
        elif target==0x8038D498:
            rec=r[18] if self.private else r[4]
            self.snapshot('private_kind7',rec)
            if mut:self.put(rec+96,OBJECTS+60*10);self.put(rec,0xDEAD0000)
            if self.private:
                for k in list(range(16,24))+[30]:r[k]=0xBAD10000+k
                for k in range(20,32):f[k]=bits(10.0+k)
        elif target==0x8038D200:
            rec,mode=(r[17],r[4]) if self.private else r[4:6]
            self.snapshot('private_transform',rec,mode)
            ob=self.get(rec+96)
            for i in range(3):self.put(ob+44+4*i,self.get(rec+32+4*i))
            if mode!=2:
                for i in range(9):self.put(ob+8+4*i,bits(1.0 if i%4==0 else 0.0) if mode==1 else self.get(rec+56+4*i))
            if mut:self.put(rec+7,self.get(rec+7,1)|0x10,1)
            if self.private:r[16]=0xBAD10000;r[18]=0xBAD10002
        elif target==0x8038D328:
            args=r[17:21] if self.private else r[4:8]
            self.snapshot('private_create',*args)
            ret=OBJECTS+60*(6+self.nalloc);self.nalloc+=1
            if self.case.get('null_object'):ret=0
            if self.private:r[16]=0xBAD10000
        elif target==0x8038E088:
            rec=r[17] if self.private else r[4]
            self.snapshot('private_destroy',rec)
            if mut:self.put(rec,0xDEAD0000);self.put(rec+7,0xEE,1)
            if self.private:r[16]=0xBAD10000
        elif target==0x8038DDDC:
            self.snapshot('steer',r[4])
            if mut:self.put(r[4]+28,bits(7.5));self.put(r[4]+80,bits(-0.5))
        elif target==0x800ADD58:
            self.snapshot('world_collision',self.vec(r[4]),r[5],r[7],self.get(sp+16),self.get(sp+20))
            for i in range(9):self.put(r[6]+4*i,bits(1.0 if i%4==0 else (i-4)*0.0625))
            if mut:self.put(r[5]+4,bits(self.float_at(r[5]+4)+0.375))
            ret=self.case.get('collision',0)
        elif target in (0x800A61B0,0x8009E820):
            src,dst,mat=r[4:7];v=self.vec(src);m=self.vec(mat,9)
            self.snapshot('vector_transform',target,v,m)
            for i in range(3):
                ix=[3*i+j for j in range(3)] if target==0x800A61B0 else [i+3*j for j in range(3)]
                products=[real(bits(real(v[j])*real(m[ix[j]]))) for j in range(3)]
                self.put(dst+4*i,bits(real(bits(products[0]+products[1]))+products[2]))
        elif target==0x8008D6B0:
            src,dst=r[4:6];self.snapshot('copy',src,dst)
            for i in range(9):self.put(dst+4*i,self.get(src+4*i))
            if mut:self.put(src,bits(self.float_at(src)+0.125))
        elif target==0x80090F44:
            self.snapshot('pitch',f[12],r[5])
            for i in range(9):self.put(r[5]+4*i,bits(self.float_at(r[5]+4*i)+real(f[12])*(i+1)*0.0625))
        elif target==0x8008B32C:
            src,dst,scale=r[4:7];v=self.vec(src,9);self.snapshot('scale',src,dst,scale)
            for i in range(9):self.put(dst+4*i,bits(real(v[i])*real(scale)))
        elif target==0x8008E06C:
            self.snapshot('color',r[4],self.get(r[5]))
            if mut:
                self.put(r[5],self.get(r[5])^0x01020304)
                p=PLAYERS+952*self.case.get('owner',0)
                self.put(p+0x34C,self.get(p+0x34C)^0x03020100)
        elif target==0x8008D870:self.snapshot('texture',*r[4:7])
        elif target==0x800B61A8:
            self.snapshot('sound',*r[4:8])
            if mut:self.put(PLAYERS+952*self.case.get('owner',0)+20,bits(2.75))
        elif target==0x800AFA84:
            self.snapshot('release',r[4],r[5])
            if mut:self.put(r[5],0xDEAD0000)
        elif target==0x8008E3C0:
            self.snapshot('allocate',r[4]);assert r[4]==0x8039A5F8
            ret=RELEASES+self.nalloc*8;self.nalloc+=1
            if self.case.get('null_release'):ret=0
        elif target==0x8038D798:
            self.snapshot('area_effect',self.vec(r[4]),self.vec(r[5]),r[6],r[7],self.get(sp+16))
            if mut:self.put(r[4],bits(self.float_at(r[4])+0.5))
        elif target==0x800AF06C:self.snapshot('burst',self.vec(r[4]),*r[5:8])
        elif target==0x8038D3A4:self.snapshot('player_damage',*r[4:7])
        elif target==0x800ABCC8:self.snapshot('rest_effect',r[4],r[5],self.vec(r[6]),r[7],*(self.get(sp+i) for i in (16,20,24)))
        elif target==0x8008B2E4:
            self.snapshot('random',f[12]);fret=bits(self.case.get('random',0.75))
        elif target==0x800AEFE0:self.snapshot('bounce_effect',self.vec(r[4]),r[5],r[6],r[7],*(self.get(sp+i) for i in range(16,40,4)))
        elif target==0x800B24EC:
            self.snapshot('resource',*r[4:8],self.get(sp+16));self.put(r[5],13,2)
        elif target==0x800A78BC:
            self.snapshot('quad',r[4],self.vec(r[5],12),r[6],r[7],self.get(sp+16),self.get(sp+20))
            ret=self.case.get('quad_result',0x170000)
        else:raise AssertionError(('unknown call',hex(target)))
        for k in list(range(1,16))+[24,25]:r[k]=(0xBAD00000+k*79)&MASK
        for k in range(20):f[k]=0x7FC00000+k
        r[2]=ret
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
        return self.state_digest(),self.trace

def fixtures():
    yield dict(count=0)
    for kind in (0,1,2,3,4,5,6,7,8,9,127,128,255):
        for collision in (0,1):
            for hit in (0,1):
                for mutation in (0,1):
                    yield dict(kind=kind,collision=collision,hit=hit,mutation=mutation)
        for key,values in [('life',(0.0,0.125,0.1875,0.25)),('flags',(1,2,0x10,0x20,0x40,0x70,0xFF)),
              ('primary',(0,1)),('effect',(0,1)),('owner',(0,1)),('timer',(0.0,0.02,0.0333333015)),
              ('resource',(0,0x8001)),('quad_result',(0,0x170000)),('velocity',(-12.0,0.0,9.0))]:
            for value in values:
                for collision in (0,1):yield dict(kind=kind,collision=collision,**{key:value})
    for latched in (0,1,2,3):
        for primary in (0,1):
            if latched==2 and not primary:continue
            for key,values in [('player_kind',(2,3)),('active',(0,1)),('blocked',(0,1)),
                ('vehicle_blocked',(0,1)),('pressed',(0,1)),('input',(0,1)),
                ('ammo',(-1,0,1)),('alpha',(0,47,48,255)),('model',(0,6,12))]:
                for value in values:
                    for mutation in (0,1):
                        yield dict(kind=3,timer=0.0,latched=latched,primary=primary,status=1,mutation=mutation,**{key:value})
    for timer in (0.0,0.02,0.0333333015,0.04):yield dict(kind=7,dt=0.0,timer=timer)
    for flags in range(256):yield dict(kind=7,flags=flags,mutation=flags&1)
    for collision in (0,1):
        for flags in (0,2):
            for velocity in (-12.0,0.0,9.0,10.0,11.0,12.0):
                yield dict(kind=3,collision=collision,flags=flags,velocity=velocity)
    yield dict(kind=0,count=3,mutation=1)
    yield dict(kind=3,count=3,life=0.0,mutation=1)
    rng=random.Random(0xE114)
    for _ in range(200):
        yield dict(kind=rng.randrange(9),flags=rng.randrange(256),mutation=rng.randrange(2),
            collision=rng.randrange(2),hit=rng.randrange(2),effect=rng.randrange(2),
            life=rng.choice([0.0,0.125,0.1875,1.0]),velocity=rng.choice([-12.,0.,12.]),
            timer=rng.choice([0.,0.02,0.25]),pressed=rng.randrange(2),latched=rng.randrange(4),
            resource=rng.choice([0,3]),quad_result=0x170000*rng.randrange(2))

def verify(native,tables,code,start,candidate_data,cases=None):
    nc,cc,nb,cb=set(),set(),set(),set();count=0
    for case in fixtures() if cases is None else cases:
        # Kind3 requires a primary on the common transform path. Missing initial
        # primary is valid only in state1, where the native explicitly creates it.
        if case.get('kind',0)==3 and not case.get('primary',1):
            if not(case.get('timer',0.25)==0.0 and case.get('latched',0)==1):continue
        a=Machine(native,ENTRY,tables,case,True)
        b=Machine(code,start,candidate_data,case,False)
        aa=a.run();bb=b.run()
        if aa!=bb:
            detail=[]
            for i,(x,y) in enumerate(zip(a.trace,b.trace)):
                if x!=y:detail.append((i,x,y));break
            changed=[(hex(x),a.memory[x],b.memory[x]) for x in a.state_order if a.memory[x]!=b.memory[x]]
            raise AssertionError(('semantic mismatch',case,detail,changed[:20],len(a.trace),len(b.trace)))
        nc.update(a.coverage);cc.update(b.coverage);nb.update(a.branches);cb.update(b.branches);count+=1
    return {'paired_cases':count,'native_executed_offsets':[hex(x-ENTRY) for x in sorted(nc)],
      'native_unexecuted_offsets':[hex(x-ENTRY) for x in sorted(set(native)-nc)],
      'candidate_executed_offsets':[hex(x-start) for x in sorted(cc)],
      'candidate_unexecuted_offsets':[hex(x-start) for x in sorted(set(code)-cc)],
      'native_branch_outcomes':len(nb),'candidate_branch_outcomes':len(cb),
      'domain':'initialized disjoint aligned acyclic storage, valid indices, successful allocations, finite normal/default rounding inputs and results; helper effect models only'}
