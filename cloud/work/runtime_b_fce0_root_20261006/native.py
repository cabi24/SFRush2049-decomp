"""Fail-closed whole-FCE0 execution, with explicit external semantic boundaries.

No target words are embedded. Zero/normal finite binary32 arithmetic, default rounding,
acyclic lists and disjoint valid storage only. This does not execute E114/F938.
"""
import hashlib
import math
import random
import struct

MASK = 0xFFFFFFFF
ENTRY, STACK, RETURN = 0x8038FCE0, 0x70008000, 0xFFFFFFFC
PLAYERS, VEHICLES = 0x80152818, 0x8014A250
RECORDS, DEBRIS, RELEASES, INPUTS = 0x100000, 0x110000, 0x120000, 0x130000

def signed(x, n=32):
    x &= (1 << n)-1
    return x-(1 << n) if x & (1 << (n-1)) else x

def bits(x): return struct.unpack('>I',struct.pack('>f',x))[0]
def real(x): return struct.unpack('>f',struct.pack('>I',x))[0]

class Machine:
    def __init__(self,code,start,data,case,private):
        self.code,self.start,self.case,self.private=code,start,case,private
        self.memory,self.writable,self.state={},{},set()
        self.coverage,self.branches,self.trace=set(),set(),[]
        self.r=[(0xA5000000+i*397)&MASK for i in range(32)]
        self.f=[bits(1.0+i*0.125) for i in range(32)]
        self.r[0],self.r[29],self.r[31]=0,STACK,RETURN
        self.original_r,self.original_f=self.r[:],self.f[:]
        self.lo=self.hi=0;self.condition=False;self.nalloc=0;self.ndebris=0;self.nrandom=0
        for address,raw in data:
            for i,b in enumerate(raw):self.memory[address+i]=b
        for address,size in [(PLAYERS,952*4),(VEHICLES,2056*13),(RECORDS,104*4),
          (DEBRIS,72*8),(RELEASES,8*4),(INPUTS,64*4),(0x8039AE80,20),
          (0x8039A520,20),(0x8039A5F8,20),(0x8002EB94,4),(0x80142764,4),
          (0x801543CA,2),(0x80399B18,68)]:
            self.map(address,size,True)
        self.map(STACK-4096,8192,False)
        self.put(0x8039A530,0)
        self.put(0x8002EB94,bits(case.get('dt',0.125)))
        self.put(0x80142764,bits(2.0))
        self.put(0x801543CA,case.get('count',1),2)
        for i in range(4):
            p=PLAYERS+952*i;v=VEHICLES+2056*i;inp=INPUTS+64*i
            self.put(p+0x35B,i,1);self.put(p+0x380,inp)
            for off,key,default in [(0x384,'kind',0),(0x385,'ammo',2),
              (0x308,'active',1),(0x359,'blocked',0)]:self.put(p+off,case.get(key,default),1)
            self.put(v+0x640,case.get('vehicle_blocked',0),1)
            self.put(v+8,case.get('model',i),1)
            self.put(p+0x3A4,case.get('latched',0))
            self.put(p+0x38C,case.get('status',0))
            self.put(p+0x3AC,bits(case.get('cooldown',0.0)))
            self.put(p+0x3B0,bits(0.125));self.put(p+0x3B4,bits(-0.25))
            self.put(inp+4,case.get('pressed',1));self.put(inp+8,case.get('held',0))
            self.put(inp+0x30,case.get('reset_mask',2));self.put(inp+0x3C,1)
            for j in range(3):
                self.put(p+8+4*j,bits((i+1)*(j+2)*0.25))
                self.put(p+20+4*j,bits((j-1)*0.75))
                for k in range(3):self.put(p+44+12*j+4*k,bits(1.0 if j==k else (j-k)*0.125))
        for i in range(17):self.put(0x80399B18+4*i,100+i)
        dn=case.get('debris',0)
        self.put(0x8039AE90,DEBRIS if dn else 0)
        for i in range(dn):
            p=DEBRIS+72*i;self.put(p,DEBRIS+72*(i+1) if i+1<dn else 0)
            self.put(p+4,[0x12348000,0x7654FFFF,0x24687FFF,10][i])
            self.put(p+56,bits(case.get('debris_life',0.125)+(i%2)*0.5))
            for j in range(3):self.put(p+60+4*j,bits((j+1)*0.5));self.put(p+44+4*j,bits(j*2.0))
        rn=case.get('releases',0)
        self.put(0x8039A608,RELEASES if rn else 0)
        for i in range(rn):
            self.put(RELEASES+8*i,RELEASES+8*(i+1) if i+1<rn else 0)
            self.put(RELEASES+8*i+4,0x12340000+i)
        self.state_order=sorted(self.state)

    def map(self,a,n,state):
        for i in range(n):
            self.memory[a+i]=(i*13+7)&255
            self.writable[a+i]=True
            if state:self.state.add(a+i)
        # All fixtures begin initialized, even fields intentionally left unchanged.
    def get(self,a,n=4):
        assert a%n==0,('unaligned read',hex(a),n)
        assert all(a+i in self.memory for i in range(n)),('unmapped read',hex(a),n)
        return int.from_bytes(bytes(self.memory[a+i] for i in range(n)),'big')
    def put(self,a,x,n=4):
        assert a%n==0,('unaligned write',hex(a),n)
        assert all(a+i in self.writable for i in range(n)),('unmapped/readonly write',hex(a),n)
        for i,b in enumerate((x&((1<<(8*n))-1)).to_bytes(n,'big')):self.memory[a+i]=b
    def float_at(self,a):return real(self.get(a))
    def state_digest(self):return hashlib.sha256(bytes(self.memory[a] for a in self.state_order)).hexdigest()

    def call(self,target):
        r,f=self.r,self.f;ret=0xBAD00002;fret=None
        mutation=self.case.get('mutation',0)
        if target==0x8038F938:
            status,player,vehicle,owner=(r[16],r[18],r[19],r[21]) if self.private else r[4:8]
            self.trace.append(('private_status',status,player,vehicle,owner,self.state_digest()))
            if mutation:
                self.put(player+0x3B0,bits(0.375));self.put(player+0x3B4,bits(-0.5))
                self.put(player+0x38C,self.get(player+0x38C)^0x10)
            if self.private:
                for k in (16,17,19,20,22):r[k]=0xBAD10000+k
                f[20]=bits(0.0)
        elif target==0x8038F568:
            self.trace.append(('player_update',r[4],self.state_digest()))
            if mutation:
                self.put(r[4]+0x3B0,bits(0.25));self.put(r[4]+0x3B4,bits(0.125))
            if 'callback_count' in self.case:self.put(0x801543CA,self.case['callback_count'],2)
        elif target==0x80090254:
            self.trace.append(('delete',r[4],self.state_digest()))
        elif target==0x800AFA84:
            pool,p=r[4:6];self.trace.append(('release',pool,p,self.state_digest()))
            # The next link was captured before the callbacks. Destroy it here.
            if mutation:self.put(p,0xDEAD0000)
        elif target==0x8008D0C0:
            self.trace.append(('release_handle',r[4],self.state_digest()))
        elif target==0x8008E3C0:
            pool=r[4];self.trace.append(('allocate',pool,self.state_digest()))
            if pool==0x8039A520:
                ret=0 if self.case.get('allocation_fail') else RECORDS+self.nalloc*104
                self.nalloc+=1
                if ret:
                    self.put(ret,self.get(pool+16));self.put(pool+16,ret)
            elif pool==0x8039AE80:
                ret=0 if self.case.get('debris_fail') else DEBRIS+72*(4+self.ndebris)
                self.ndebris+=1
                if ret:self.put(ret,self.get(pool+16));self.put(pool+16,ret)
            else:raise AssertionError(('pool',hex(pool)))
        elif target==0x8008D6B0:
            self.trace.append(('copy',r[4],r[5],self.state_digest()))
            for i in range(9):self.put(r[5]+4*i,self.get(r[4]+4*i))
            if RECORDS <= r[5]-56 < RECORDS+104*4 and 'copied_kind' in self.case:
                self.put(r[5]-56+6,self.case['copied_kind'],1)
            if mutation:
                # Source pointee mutation after the copy tests subsequent rereads.
                self.put(r[4],bits(self.float_at(r[4])+0.125))
        elif target in (0x80090F44,0x80090E9C,0x8009EA68):
            self.trace.append(('transform',target,f[12],r[5],self.state_digest()))
            angle=real(f[12]);base=r[5]
            # Explicit bounded side-effect model, not a trigonometric helper proof.
            for i in range(9):
                val=self.float_at(base+4*i)
                val=val*angle if target==0x8009EA68 else val+angle*(i+1)*0.0625
                self.put(base+4*i,bits(val))
        elif target==0x8008B2E4:
            self.trace.append(('random',f[12],self.state_digest()))
            self.nrandom+=1;fret=bits(real(f[12])*(self.nrandom%4)*0.25)
        elif target==0x8008E398:
            self.trace.append(('create',*r[4:8],self.state_digest()));ret=0xFFFF8001
        elif target==0x800B61A8:
            self.trace.append(('sound',*r[4:8],self.state_digest()))
        elif target==0x8038D054:
            self.trace.append(('effect',*r[4:7],self.state_digest()));ret=0x160000
        elif target in (0x8038E114,0x8038CB20):
            self.trace.append(('tail',target,self.state_digest()))
            if target==0x8038E114 and self.private:
                for k in list(range(16,24))+[30]:r[k]=0xBAD20000+k
                for k in range(20,32):f[k]=0xBAD30000+k
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
                    elif fn==6:f[sh]=f[rd]
                    elif fn==7:f[sh]=f[rd]^0x80000000
                    elif fn==0x32:self.condition=real(f[rd])==real(f[rt])
                    elif fn==0x3c:self.condition=real(f[rd])<real(f[rt])
                    elif fn==0x3e:self.condition=real(f[rd])<=real(f[rt])
                    else:raise AssertionError(('COP1 single',hex(pc),hex(w)))
                else:raise AssertionError(('COP1',hex(pc),hex(w)))
            else:raise AssertionError(('opcode',hex(pc),hex(w)))
            r[0]=0
            assert old is None or pending is None,'branch in delay slot'
            if isinstance(old,tuple):self.call(old[1]);pc=old[2]
            else:pc=old if old is not None else next_pc
        else:raise AssertionError('step limit')
        assert r[29]==STACK and r[28]==self.original_r[28]
        assert r[16:24]==self.original_r[16:24] and r[30]==self.original_r[30]
        assert f[20:]==self.original_f[20:]
        return self.state_digest(),self.trace


def fixtures():
    for count in (-32768,-1,0,1,4):
        for debris in (0,1,4):
            for releases in (0,1,4):yield {'count':count,'debris':debris,'releases':releases}
    for kind in range(9):
        for ammo in (-128,-1,0,1,127):
            for cooldown in (-0.125,0.0,0.0625,0.125,0.25):
                for mutation in (0,1):
                    yield dict(kind=kind,ammo=ammo,cooldown=cooldown,mutation=mutation,status=3)
        for key,values in [('latched',(0,1)),('active',(0,1)),('blocked',(-1,0,1)),
                           ('vehicle_blocked',(-1,0,1)),('pressed',(0,1,2,3)),
                           ('held',(0,1)),('allocation_fail',(False,True)),
                           ('debris_fail',(False,True)),('model',(0,6,12))]:
            for value in values:
                for mutation in (0,1):yield dict(kind=kind,mutation=mutation,**{key:value})
    for count in (0,1,3,4):yield dict(kind=0,count=4,callback_count=count)
    for count in (0,4):yield dict(kind=0,count=1,callback_count=count)
    for copied_kind in (3,9,127,255):yield dict(kind=0,copied_kind=copied_kind)
    rng=random.Random(0xFCE0)
    for _ in range(500):
        yield dict(kind=rng.randrange(9),ammo=rng.choice([-1,0,1,3]),
          cooldown=rng.choice([-1.,0.,0.0625,0.125,0.25]),count=rng.randrange(1,5),
          pressed=rng.randrange(4),held=rng.randrange(2),status=rng.randrange(4),
          mutation=rng.randrange(2),debris=rng.randrange(5),releases=rng.randrange(5),
          model=rng.randrange(13),allocation_fail=rng.randrange(2),debris_fail=rng.randrange(2))


def verify(native_code,native_data,candidate_code,start,candidate_data,cases=None):
    coverage=[set(),set()];branches=[set(),set()];count=0
    for case in fixtures() if cases is None else cases:
        a=Machine(native_code,ENTRY,native_data,case,True)
        b=Machine(candidate_code,start,candidate_data,case,False)
        ar=a.run();br=b.run()
        if ar!=br:
            mismatch=next(((x,y) for x,y in zip(ar[1],br[1]) if x!=y),None)
            if mismatch is None:mismatch=('length/final',len(ar[1]),len(br[1]),ar[0],br[0])
            differences=[hex(p) for p in a.state_order if a.memory[p]!=b.memory[p]]
            raise AssertionError(('semantic mismatch',case,mismatch,differences[:30]))
        for k,m in enumerate((a,b)):coverage[k].update(m.coverage);branches[k].update(m.branches)
        count+=1
    return {'paired_cases':count,'executions':count*2,
      'native_executed_offsets':[hex(p-ENTRY) for p in sorted(coverage[0])],
      'candidate_executed_offsets':[hex(p-start) for p in sorted(coverage[1])],
      'native_unexecuted_offsets':[hex(p-ENTRY) for p in sorted(set(native_code)-coverage[0])],
      'candidate_unexecuted_offsets':[hex(p-start) for p in sorted(set(candidate_code)-coverage[1])],
      'native_branch_outcomes':len(branches[0]),'candidate_branch_outcomes':len(branches[1])}
