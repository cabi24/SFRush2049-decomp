"""Fail-closed target interpreter and explicit external boundary model.
Only complete authenticated functions are loaded; unsupported opcodes, unknown
calls, invalid pointers and exhausted searches fail rather than being guessed.
"""
import ctypes as C
import hashlib
import math
import random
import struct
MASK=0xFFFFFFFF
ENTRY,STACK,RETURN=0x80390F60,0x70008000,0xFFFFFFFC
CTRL,PLAYERS,COUNT,DELTA=0x80399AE0,0x80152818,0x8014A108,0x8002EB94
RESOURCES=(0x80142A82,0x80142A84,0x80142A8E)
TABLES=(0x100000,0x101000)
OBJECTS=0x110000
SCENES=0x120000
N=8

def signed(x,n=32):
    x&=(1<<n)-1
    return x-(1<<n) if x&(1<<(n-1)) else x

def bits(x):return struct.unpack('>I',struct.pack('>f',x))[0]
def real(x):return struct.unpack('>f',struct.pack('>I',x))[0]

class World:
    def setup(self,case):
        self.case,self.trace,self.calls,self.random_calls=case,[],0,0
        self.seed=case.get('seed',12345)
        self.put(COUNT,case.get('count',len(case.get('players',[{}]))),2)
        self.put(DELTA,bits(case.get('delta',0.25)))
        for a,v in zip(RESOURCES,case.get('resources',[0x1234,0xFEDC,0x8000])):self.put(a,v,2)
        self.put(CTRL,case.get('entry_count',4),1)
        for j in range(3):self.put(CTRL+1+j,case.get('selected',[0,1,2])[j],1)
        for j,v in enumerate(case.get('timers',[20.,20.,20.])):self.put(CTRL+4+4*j,bits(v))
        self.put(CTRL+16,TABLES[0])
        for ti,table in enumerate(TABLES):
            entries=case.get('entries' if ti==0 else 'alternate_entries',[])
            for j in range(N):
                e=entries[j] if j<len(entries) else {}
                self.put(table+8*j,OBJECTS+0x100*e.get('object',j))
                self.put(table+8*j+4,e.get('state',1),2)
                self.put(table+8*j+6,e.get('eligibility',1),2)
        objects=case.get('objects',[])
        for j in range(N):
            o=objects[j] if j<len(objects) else {};a=OBJECTS+0x100*j
            self.put(a+4,o.get('flags',0x80),1)
            self.put(a+12,o.get('scene',j))
            self.put(a+80,o.get('animation',-1),2)
            self.put(SCENES+68*j,0xF1234F56)
            self.put(SCENES+68*j+20,0x5678,2)
        players=case.get('players',[{}])
        for j in range(N):
            x=players[j] if j<len(players) else {};a=PLAYERS+952*j
            self.put(a+908,x.get('status',0x80))
            self.put(a+928,x.get('countdown',0),1)
            self.put(a+929,x.get('alpha',255),1)
            self.put(a+930,x.get('phase',0),1)
            self.put(a+936,bits(x.get('timer',1.0)))
        self.initial=self.raw_state()
    def raw_state(self):
        return ([self.get(CTRL+j,1) for j in range(4)]+[self.get(CTRL+4+4*j) for j in range(4)],
          [(self.get(t+8*j),self.get(t+8*j+4,2),self.get(t+8*j+6,2)) for t in TABLES for j in range(N)],
          [(self.get(OBJECTS+256*j+4,1),self.get(OBJECTS+256*j+12),self.get(OBJECTS+256*j+80,2)) for j in range(N)],
          [(self.get(PLAYERS+952*j+908),self.get(PLAYERS+952*j+928,1),self.get(PLAYERS+952*j+929,1),self.get(PLAYERS+952*j+930,1),self.get(PLAYERS+952*j+936)) for j in range(N)],
          [(self.get(SCENES+68*j),self.get(SCENES+68*j+20,2)) for j in range(N)],
          [self.get(COUNT,2),self.get(DELTA)]+[self.get(a,2) for a in RESOURCES],self.seed)
    def state(self):return self.raw_state()
    def event(self,kind,a,b,c):
        self.trace.append((kind,a,b,c,self.state()))
        self.calls+=1
        out=0
        if kind==0:
            self.seed=(self.seed*1103515245+12345)&MASK
            fraction=((self.seed>>16)&32767)/32768.0
            values=self.case.get('fractions',[])
            if self.random_calls<len(values):fraction=values[self.random_calls]
            out=bits(real(bits(real(a)*fraction)))
            self.random_calls+=1
        elif kind==1:
            index=signed(a,16);assert 0<=index<N,('scene setter index',index)
            self.put(SCENES+68*index+20,b,2)
        elif kind==2:
            index=signed(a);assert 0<=index<N and b==0 and c==15,('show arguments',a,b,c)
            self.put(SCENES+68*index,self.get(SCENES+68*index)&~(0x80000000|(c<<8)))
        else:raise AssertionError(('unknown boundary',kind))
        if self.case.get('mutation') and kind==1:
            # Widened boundary probe, not a claim that the real setter changes
            # the entry table. It tests mandatory reloads after ordinary calls.
            self.put(CTRL+16,TABLES[1])
            self.put(COUNT,2,2)
            self.put(DELTA,bits(0.5))
            for j in range(3): self.put(CTRL+1+j,7,1)
        if self.case.get('rng_mutation') and kind==0:
            self.put(CTRL+16,TABLES[1])
            self.put(CTRL,5,1)
            for j,v in enumerate([3,4,2]): self.put(CTRL+1+j,v,1)
            self.put(COUNT,2,2)
            self.put(DELTA,bits(0.5))
            for a in RESOURCES: self.put(a,0xDEAD,2)
            self.put(PLAYERS+908,0x80000001)
            self.put(PLAYERS+930,1,1)
            self.put(PLAYERS+929,24,1)
            self.put(PLAYERS+936,bits(2.))
        self.trace[-1]=self.trace[-1]+(out,)
        return out

class Machine(World):
    def __init__(self,code,start,data,case):
        self.code,self.start=code,start
        self.memory,self.writable={},set()
        self.coverage,self.branches=set(),set()
        self.r=[(0xA5000000+i*397)&MASK for i in range(32)]
        self.f=[bits(1.0+i*0.125) for i in range(32)]
        self.r[0],self.r[29],self.r[31]=0,STACK,RETURN
        self.lo=self.hi=0;self.condition=False
        for a,raw in data:
            for i,b in enumerate(raw):self.memory[a+i]=b
        for a,n in [(CTRL,20),(PLAYERS,952*N),(COUNT,2),(DELTA,4),(OBJECTS,0x100*N),(SCENES,68*N)]+[(t,8*N) for t in TABLES]+[(a,2) for a in RESOURCES]:self.map(a,n)
        self.map(STACK-4096,8192)
        self.original_r,self.original_f=self.r[:],self.f[:]
        self.setup(case)
        mutable=[(CTRL,20),(COUNT,2),(DELTA,4)]+[(a,2) for a in RESOURCES]+[(t,8*N) for t in TABLES]
        for j in range(N):
            mutable += [(OBJECTS+256*j+4,1),(OBJECTS+256*j+12,4),(OBJECTS+256*j+80,2),(SCENES+68*j,4),(SCENES+68*j+20,2)]
            mutable += [(PLAYERS+952*j+off,n) for off,n in [(908,4),(928,1),(929,1),(930,1),(936,4)]]
        allowed={a+i for a,n in mutable for i in range(n)}
        self.unchanged={a:b for a,b in self.memory.items() if a not in allowed and not STACK-4096<=a<STACK+4096}
    def map(self,a,n):
        for i in range(n):self.memory[a+i]=0xA5;self.writable.add(a+i)
    def get(self,a,n=4):
        assert a%n==0,('unaligned read',hex(a),n)
        assert all(a+i in self.memory for i in range(n)),('unmapped read',hex(a),n)
        return int.from_bytes(bytes(self.memory[a+i] for i in range(n)),'big')
    def put(self,a,x,n=4):
        assert a%n==0,('unaligned write',hex(a),n)
        assert all(a+i in self.writable for i in range(n)),('unmapped/readonly write',hex(a),n)
        for i,b in enumerate((x&((1<<(8*n))-1)).to_bytes(n,'big')):self.memory[a+i]=b
    def call(self,target):
        r,f=self.r,self.f
        if target==0x8008B2E4:out=self.event(0,f[12],0,0)
        elif target==0x80090770:out=self.event(1,r[4],r[5],0)
        elif target==0x8008B0D8:out=self.event(2,r[4],r[5],r[6])
        else:raise AssertionError(('unknown call',hex(target)))
        for k in list(range(1,16))+[24,25]:r[k]=(0xBAD00000+k*79)&MASK
        for k in range(20):f[k]=0x7FC00000+k
        self.lo,self.hi,self.condition=0xBAD0,0xBAD1,True
        if target==0x8008B2E4:f[0]=out
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
            if isinstance(old,tuple):
                if old[1] in self.code: pc=old[1]
                else: self.call(old[1]);pc=old[2]
            else:pc=old if old is not None else next_pc
        else:raise AssertionError('step limit')
        assert r[29]==STACK and r[28]==self.original_r[28]
        if self.start == ENTRY:assert r[16:24]==self.original_r[16:24] and r[30]==self.original_r[30]
        if self.start == ENTRY:assert f[20:]==self.original_f[20:]
        assert all(self.memory[a]==b for a,b in self.unchanged.items()),'unexpected non-field memory write'
        return self.state(),self.trace

class Object(C.Structure):
    _fields_=[('unknown00',C.c_ubyte*4),('flags',C.c_ubyte),('unknown05',C.c_ubyte*7),('scene',C.c_int),('unknown10',C.c_ubyte*64),('animation',C.c_short)]
class Entry(C.Structure):
    _fields_=[('object',C.POINTER(Object)),('state',C.c_short),('eligibility',C.c_short)]
class Control(C.Structure):
    _fields_=[('count',C.c_byte),('selected_fast',C.c_byte),('selected_second',C.c_byte),('selected_first',C.c_byte),('timer_fast',C.c_float),('timer_first',C.c_float),('timer_second',C.c_float),('entries',C.POINTER(Entry))]
class Host(World):
    def __init__(self,lib,case):
        self.lib,self.slots,self.pointer_maps=lib,{},{}
        self.control=Control.in_dll(lib,'D_80399AE0')
        self.objects=(Object*N)()
        self.tables=[(Entry*N)(),(Entry*N)()]
        self.players=(C.c_ubyte*(952*N)).in_dll(lib,'D_80152818')
        self.scenes=(C.c_ubyte*(68*N))()
        self.regions=[self.control,self.objects,*self.tables,self.players,self.scenes]
        for x in self.regions:C.memset(C.addressof(x),0xA5,C.sizeof(x))
        def slot(a,actual,n):self.slots[a,n]=(actual,n)
        def ptr(a,actual,mapping):self.pointer_maps[a]=(actual,mapping)
        cb=C.addressof(self.control)
        for j in range(4):slot(CTRL+j,cb+j,1)
        for j in range(3):slot(CTRL+4+4*j,cb+4+4*j,4)
        ptr(CTRL+16,cb+Control.entries.offset,{a:C.addressof(t) for a,t in zip(TABLES,self.tables)})
        for addr,name,n in [(COUNT,'D_8014A108',2),(DELTA,'D_8002EB94',4)]+[(a,'D_%08X'%a,2) for a in RESOURCES]:
            x=(C.c_ubyte*n).in_dll(lib,name);slot(addr,C.addressof(x),n)
        for ti,t in enumerate(self.tables):
            for j in range(N):
                ptr(TABLES[ti]+8*j,C.addressof(t[j])+Entry.object.offset,{OBJECTS+256*k:C.addressof(self.objects[k]) for k in range(N)})
                for off,name in [(4,'state'),(6,'eligibility')]:slot(TABLES[ti]+8*j+off,C.addressof(t[j])+getattr(Entry,name).offset,2)
        for j in range(N):
            for off,n in [(4,1),(12,4),(80,2)]:slot(OBJECTS+256*j+off,C.addressof(self.objects[j])+off,n)
            for off,n in [(908,4),(928,1),(929,1),(930,1),(936,4)]:slot(PLAYERS+952*j+off,C.addressof(self.players)+952*j+off,n)
            for off,n in [(0,4),(20,2)]:slot(SCENES+68*j+off,C.addressof(self.scenes)+68*j+off,n)
        self.error=None
        def callback(*args):
            try:return self.event(*args)
            except BaseException as exc:self.error=exc;return 0
        self.callback=C.CFUNCTYPE(C.c_uint32,C.c_uint32,C.c_uint32,C.c_uint32,C.c_uint32)(callback)
        C.c_void_p.in_dll(lib,'boundary').value=C.cast(self.callback,C.c_void_p).value
        self.setup(case)
    def get(self,a,n=4):
        if a in self.pointer_maps:
            assert n==4
            actual,mapping=self.pointer_maps[a];v=C.c_void_p.from_address(actual).value
            return next(k for k,x in mapping.items() if x==v)
        actual,size=self.slots[a,n];return {1:C.c_uint8,2:C.c_uint16,4:C.c_uint32}[size].from_address(actual).value
    def put(self,a,v,n=4):
        if a in self.pointer_maps:
            assert n==4
            actual,mapping=self.pointer_maps[a];C.c_void_p.from_address(actual).value=mapping[v];return
        actual,size=self.slots[a,n];{1:C.c_uint8,2:C.c_uint16,4:C.c_uint32}[size].from_address(actual).value=v
    def run(self):
        self.lib.func_80390F60()
        if self.error:raise self.error
        for j in range(N):
            for name in ['unknown00','unknown05','unknown10']:assert set(getattr(self.objects[j],name))=={0xA5}
            for lo,hi in [(0,908),(912,928),(931,936),(940,952)]:assert bytes(self.players[952*j+lo:952*j+hi])==b'\xA5'*(hi-lo)
        return self.state(),self.trace

def fixtures():
    yield {}
    for count in (-32768,-1,0,1,2,4,8):yield dict(count=count)
    for status in (0,1,0x80000000,0xFFFFFFFF):
      for phase in (-128,-1,0,1,2,3,4,127):
       for timer in (-1.,-0.,0.25,1.):
        for alpha in (0,7,8,16,17,23,24,25,247,248,255):
         yield dict(players=[dict(status=status,phase=phase,timer=timer,alpha=alpha,countdown=3)])
    for n in range(8):yield dict(players=[dict(status=1,phase=3,countdown=n)])
    for n in (-128,-1,0,1,127):yield dict(players=[dict(status=0,countdown=n)])
    for which in range(3):
      for timer in (-2.,-1.,0.,0.25,0.5,2.):
       for flags in (0,2,0x80,0xFF):
        timers=[20.,20.,20.];timers[which]=timer
        yield dict(timers=timers,objects=[dict(flags=flags)]*N)
      for start in range(4):
       for eligible in range(4):
        timers=[20.,20.,20.];timers[which]=0.
        entries=[dict(state=1,eligibility=0) for _ in range(4)];entries[eligible]['eligibility']=1
        yield dict(timers=timers,entries=entries,fractions=[start/4.])
      kind=[4,2,3][which]
      for start in (0.,0.75):
       timers=[20.,20.,20.];timers[which]=0.
       yield dict(timers=timers,entries=[dict(state=kind),dict(state=kind),dict(state=1),dict(state=kind)],fractions=[start])
      for timer in (-2.,0.):
       timers=[20.,20.,20.];timers[which]=timer
       yield dict(timers=timers,rng_mutation=True,alternate_entries=[dict(object=(j+1)%N) for j in range(N)])
      for current in range(4):
       timers=[20.,20.,20.];timers[which]=-2.
       selected=[0,1,2];selected[{0:0,1:2,2:1}[which]]=current
       yield dict(timers=timers,selected=selected,entries=[dict(state=kind,eligibility=kind)]*4)
      yield dict(timers=[0. if j==which else 20. for j in range(3)],mutation=True,
                 alternate_entries=[dict(object=(j+1)%N) for j in range(N)])
    # Independent children and root can all expire in one pass with enough
    # candidates; repeated invocations exercise sentinel and resume paths.
    yield dict(timers=[0.,0.,0.],entry_count=8,repeat=3)
    yield dict(timers=[-2.,-2.,-2.],repeat=4)
    for count in (-128,-1,0):
      yield dict(entry_count=count,timers=[20.,20.,20.])
      yield dict(entry_count=count,timers=[-2.,-2.,-2.])
    rng=random.Random(390960)
    for _ in range(200):
        players=[]
        for j in range(rng.randrange(0,9)):
            phase=rng.choice([-1,0,1,2,3,4]);status=rng.getrandbits(32)
            players.append(dict(status=status,phase=phase,countdown=rng.randrange(8) if phase==3 and status&1 else rng.randrange(-128,128),alpha=rng.randrange(256),timer=rng.choice([-1.,0.,0.25,1.,20.])))
        yield dict(players=players,timers=[rng.choice([0.,20.,-2.]) for j in range(3)],entry_count=8,seed=rng.getrandbits(32),delta=rng.choice([0.,0.125,0.25,0.5]),repeat=2)

def verify_host(code,data,lib,cases=None):
    cases=list(fixtures() if cases is None else cases)
    covered,branches=set(),set();runs=0
    for num,case in enumerate(cases):
        native=Machine(code,ENTRY,data,case);host=Host(lib,case)
        for k in range(case.get('repeat',1)):
            native.r[31]=RETURN
            a=native.run();b=host.run();runs+=1
            assert a==b,('host semantic mismatch',num,k,case,a,b)
            covered|=native.coverage;branches|=native.branches
    return dict(fixtures=len(cases),root_runs=runs,native_instructions=len(covered),native_branches=len(branches),native_unexecuted=[hex(p) for p in sorted(set(code)-covered)])
