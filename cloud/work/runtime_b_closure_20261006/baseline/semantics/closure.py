"""Bounded genuine FCE0-root execution, with real private child control flow.

Instruction semantics adapted from the six standalone native.py engines, but no
private hook is retained. Native words are authenticated by adapter.py at runtime;
this file embeds no native instructions. External services remain bounded models.
"""
import hashlib
import math
import struct

MASK = 0xffffffff
ENTRY, STACK, RETURN = 0x8038FCE0, 0x70008000, 0xfffffffc
PLAYERS, VEHICLES, COUNT, SCENES = 0x80152818, 0x8014A250, 0x801543CA, 0x8012E700
RECORDS, OBJECTS, RELEASES, INPUTS, EFFECTS, DEBRIS, QUADS = (0x100000,0x110000,0x120000,0x130000,0x140000,0x150000,0x170000)
POOLS = {0x8039A520:(RECORDS,104,32), 0x80394F70:(OBJECTS,60,64),
         0x8039A5F8:(RELEASES,8,32), 0x8039AE80:(DEBRIS,72,32)}
MEMBERS = {'D200':296,'D328':124,'D498':768,'DA78':868,'E088':140,'E114':5196,'F938':928,'FCE0':3056}
INTERNAL = {0x80380000+int(k,16) for k in MEMBERS}


def signed(x,n=32):
    x &= (1<<n)-1
    return x-(1<<n) if x&(1<<(n-1)) else x


def bits(x):
    assert math.isfinite(x), ('nonfinite arithmetic',x)
    try: out=struct.unpack('>I',struct.pack('>f',x))[0]
    except OverflowError: raise AssertionError('binary32 overflow')
    assert (out&0x7f800000) != 0x7f800000, 'binary32 overflow'
    assert not (out&0x7fffffff and out&0x7f800000==0), 'binary32 subnormal result'
    assert x==0 or out&0x7fffffff, 'binary32 underflow to zero'
    return out


def real(x):
    assert (x&0x7f800000) != 0x7f800000, 'nonfinite operand'
    assert not (x&0x7fffffff and x&0x7f800000==0), 'subnormal operand'
    return struct.unpack('>f',struct.pack('>I',x&MASK))[0]


def f32(x): return real(bits(x))


class Machine:
    def __init__(self,code,start,data,case,private=False):
        self.code,self.start,self.case,self.private=code,start,case,private
        self.memory,self.writable,self.state={},{},set()
        self.coverage,self.branches,self.trace,self.internal_edges=set(),set(),[],set()
        self.nrandom=self.neffect=self.nquad=self.nscene=0
        self.lo=self.hi=0;self.condition=False;self.frame=0;self.steps=0
        for a,raw in data:
            for i,b in enumerate(raw):
                assert a+i not in self.memory or self.memory[a+i]==b, ('conflicting readonly inputs',hex(a+i))
                self.memory[a+i]=b
        # Pool prefixes must include actual free-head at +0x14.
        regions=[(PLAYERS,952*4),(VEHICLES,2056*4),(COUNT,2),(SCENES,68*128),
          (INPUTS,64*4),(EFFECTS,32*32),(QUADS,88*32),(0x80394F90,52*4),
          (0x8002EB94,4),(0x80142764,4),(0x80399B18,68),
          (0x8012E67C,4),(0x80151AE8,8*4),(0x180000,36*4*4),
          (0x80399AF8,8),(0x80394F88,2),(0x80140BDC,1),(0x803942E8,4),
          (0x8011418C,36),(0x801141B0,36)]
        for pool,(base,size,count) in POOLS.items(): regions.extend([(pool,24),(base,size*count)])
        for a,n in regions:self.map(a,n,True)
        self.map(STACK-16384,32768,False)
        self.put(COUNT,case.get('count',1),2)
        self.put(0x8002EB94,bits(case.get('dt',0.125)))
        self.put(0x80142764,bits(2.0))
        self.put(0x80394F88,case.get('resource',0),2)
        self.put(0x80140BDC,4,1);self.put(0x803942E8,0x12345678)
        for i in range(17):self.put(0x80399B18+4*i,100+i)
        for a in (0x8011418C,0x801141B0):self.matrix(a)
        self.live={pool:set() for pool in POOLS}
        for pool,(base,size,count) in POOLS.items():
            self.put(pool,0,1);self.put(pool+16,0);self.put(pool+20,base)
            for i in range(count):self.put(base+size*i,base+size*(i+1) if i+1<count else 0)
        for i in range(128):self.put(SCENES+68*i+12,bits(case.get('scene_scale',2.0)))
        entries=case.get('players',[])
        for i in range(4):
            c=dict(case);c.update(entries[i] if i<len(entries) else {})
            p,v,inp=PLAYERS+952*i,VEHICLES+2056*i,INPUTS+64*i
            owner=c.get('owner',i);assert 0<=owner<4, 'F938 owner intersection is 0..3'
            self.put(p+0x35B,owner,1);self.put(p+0x380,inp)
            for off,key,default in [(0x308,'active',1),(0x359,'blocked',0),(0x384,'kind',0),
                (0x385,'ammo',2),(0x3A1,'alpha',64),(0x3A2,'transition',0)]:self.put(p+off,c.get(key,default),1)
            for off,key,default in [(0x38c,'status',0),(0x3a4,'latched',0)]:self.put(p+off,c.get(key,default))
            self.put(p+0x34c,0x12345678+i);self.put(p+0x386,1500,2);self.put(p+0x388,0,2)
            for off,key,default in [(0x390,'transition_timer',1.),(0x394,'scale_timer',0.125),
                (0x398,'scale',0.25),(0x39c,'status_pitch',0.25),(0x3a8,'transition_time',0.),
                (0x3ac,'cooldown',0.),(0x3b0,'pitch',0.125),(0x3b4,'yaw',-0.25)]:self.put(p+off,bits(c.get(key,default)))
            self.put(inp+4,c.get('pressed',1));self.put(inp+8,c.get('held',0))
            self.put(inp+0x30,c.get('reset_mask',2));self.put(inp+0x3c,1)
            self.put(v+8,c.get('model',i),1);self.put(v+0x640,c.get('vehicle_blocked',0),1)
            self.put(v+0x6c4,c.get('vehicle_state',-1),2)
            for off in (0xfc,0x108,0x654):self.put(v+off,bits(1.))
            self.vector(p+8,c.get('position',(i*3.0+0.5,0.75,i*4.0+1.0)))
            self.vector(p+20,c.get('velocity',(0.,0.,0.)))
            self.matrix(p+44)
            self.vector(v+0x124,(1.,2.,3.));self.vector(v+0x13c,(-1.,0.,-3.))
            self.put(0x80394f90+52*i,c.get('status_scene',-1))
            self.put(0x8012e67c+i,i,1);self.put(0x80399af8+2*i,(i<<10)|i,2)
            self.put(0x80151ae8+8*i,0x180000+36*4*i)
        self.quad_live=set()
        for i in range(case.get('debris',0)):
            node=self.allocate(0x8039ae80);self.put(node+4,80+i)
            self.put(node+56,bits(case.get('debris_life',0.125)+i*0.25))
            self.matrix(node+8);self.vector(node+44,(2.,3.,4.));self.vector(node+60,(1.,2.,3.))
        for i in range(case.get('releases',0)):
            node=self.allocate(0x8039a5f8);quad=self.new_quad();self.put(node+4,quad)
        for rec in case.get('records',[]):self.seed_record(rec)
        self.state_order=sorted(self.state)
        self.reset_registers()

    def reset_registers(self):
        self.r=[(0xa5000000+i*397)&MASK for i in range(32)]
        self.f=[bits(1.+i*0.125) for i in range(32)]
        self.r[0],self.r[29],self.r[31]=0,STACK,RETURN
        self.original_r,self.original_f=self.r[:],self.f[:]

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
    def vector(self,a,v):
        for i,x in enumerate(v):self.put(a+4*i,bits(x))
    def matrix(self,a):self.vector(a,(1.,0.,0.,0.,1.,0.,0.,0.,1.))
    def float_at(self,a):return real(self.get(a))
    def state_digest(self):return hashlib.sha256(bytes(self.memory[a] for a in self.state_order)).hexdigest()
    def snapshot(self,name,*args):self.trace.append((name,*args,self.state_digest()))
    def pointer(self,a):return 'stack-local' if STACK-16384<=a<STACK+16384 else a

    def allocate(self,pool):
        assert pool in POOLS,('unknown pool',hex(pool))
        node=self.get(pool+20)
        if node:
            assert node not in self.live[pool],'allocate live node'
            self.put(pool+20,self.get(node));self.put(node,self.get(pool+16));self.put(pool+16,node)
            self.live[pool].add(node)
        return node

    def release(self,pool,node):
        assert pool in POOLS and node in self.live[pool],('invalid pool release',hex(pool),hex(node))
        cur=self.get(pool+16);prev=pool+16;seen=set()
        while cur!=node:
            assert cur and cur not in seen,'corrupt active pool list'
            seen.add(cur);prev=cur;cur=self.get(cur)
        self.put(prev,self.get(node));self.put(node,self.get(pool+20));self.put(pool+20,node)
        self.live[pool].remove(node)

    def new_quad(self):
        assert self.nquad<32,'bounded quad storage exhausted'
        node=QUADS+88*self.nquad;self.nquad+=1
        self.put(node,1,2);self.quad_live.add(node)
        return node

    def seed_record(self,c):
        node=self.allocate(0x8039a520);owner=c.get('owner',0);assert 0<=owner<4
        for off,key,default in [(4,'owner',0),(5,'hit_mask',0),(6,'kind',0),(7,'flags',0)]:self.put(node+off,c.get(key,default),1)
        for off,key,default in [(8,'life',2.),(12,'timer',0.25),(16,'extent',0.75)]:self.put(node+off,bits(c.get(key,default)))
        self.vector(node+20,c.get('velocity',(12.,11.25,12.75)))
        self.vector(node+32,c.get('position',(0.,0.,0.)))
        self.vector(node+44,c.get('previous',(-1.,0.,-1.)));self.matrix(node+56)
        self.put(node+92,0)
        for off in (96,100):
            if off==96 and not c.get('primary',True):self.put(node+off,0);continue
            ob=self.allocate(0x80394f70);self.put(node+off,ob);self.put(ob+4,64+len(self.live[0x80394f70]))
            self.matrix(ob+8);self.vector(ob+44,c.get('center',(0.,0.,0.)))

    def call(self,target):
        assert target not in INTERNAL, ('private child hook forbidden',hex(target))
        r,f=self.r,self.f;sp=r[29];ret=0xbad00002;fret=None;mut=self.case.get('mutation',False)
        if target==0x8008e3c0:
            pool=r[4];self.snapshot('allocate',pool)
            fail=(pool==0x8039a520 and self.case.get('allocation_fail')) or (pool==0x8039ae80 and self.case.get('debris_fail'))
            ret=0 if fail else self.allocate(pool)
            assert ret or pool in (0x8039a520,0x8039ae80),'required allocation exhausted'
        elif target==0x800afa84:
            self.snapshot('release',r[4],r[5]);self.release(r[4],r[5])
        elif target==0x8008d0c0:
            quad=r[4];self.snapshot('release_quad',quad)
            assert quad in self.quad_live,('not a live quad pointer',hex(quad))
            self.put(quad,0,2);self.quad_live.remove(quad)
        elif target==0x8008d6b0:
            src,dst=r[4:6];value=self.vec(src,9)
            self.snapshot('copy',self.pointer(src),self.pointer(dst),value)
            for i,x in enumerate(value):self.put(dst+4*i,x)
            if mut and PLAYERS<=src<PLAYERS+952*4:
                self.put(src,bits(self.float_at(src)+0.125))
        elif target in (0x80090f44,0x80090e9c,0x8009ea68):
            dst=r[5];v=self.vec(dst,9);angle=real(f[12])
            self.snapshot('matrix_effect',target,f[12],self.pointer(dst),v)
            for i,x in enumerate(v):
                value=real(x)*angle if target==0x8009ea68 else real(x)+angle*(i+1)*0.0625
                self.put(dst+4*i,bits(value))
        elif target in (0x800a61b0,0x8009e820):
            src,dst,mat=r[4:7];v=self.vec(src);m=self.vec(mat,9)
            self.snapshot('vector_transform',target,self.pointer(src),self.pointer(dst),self.pointer(mat),v,m)
            for i in range(3):
                ix=[3*i+j for j in range(3)] if target==0x800a61b0 else [i+3*j for j in range(3)]
                p=[f32(real(v[j])*real(m[ix[j]])) for j in range(3)]
                self.put(dst+4*i,bits(f32(p[0]+p[1])+p[2]))
        elif target==0x8008b32c:
            src,dst,scale=r[4:7];v=self.vec(src,9)
            self.snapshot('scale',self.pointer(src),self.pointer(dst),scale,v)
            for i,x in enumerate(v):self.put(dst+4*i,bits(real(x)*real(scale)))
        elif target==0x8008b2e4:
            self.snapshot('random',f[12]);self.nrandom+=1
            fret=bits(real(f[12])*(self.nrandom%4)*0.25)
        elif target==0x8008c768:
            self.snapshot('angle',f[12],f[14]);fret=bits(self.case.get('angle',math.atan2(real(f[12]),real(f[14]))))
        elif target==0x8008e398:
            self.snapshot('create_scene',r[4],self.pointer(r[5]),r[6],r[7],self.vec(r[5],9) if r[5] else None)
            self.nscene+=1;ret=self.nscene;assert ret<64,'scene storage exhausted'
        elif target==0x80090254:
            assert r[4]==(signed(r[4],16)&MASK),'scene delete narrowing'
            self.snapshot('delete_scene',r[4])
        elif target==0x8008e06c:
            assert r[4]==(signed(r[4],16)&MASK),'scene color narrowing'
            color=self.get(r[5]);self.snapshot('color',r[4],color)
            index=signed(r[4],16);assert 0<=index<128,'scene color index outside fixture'
            self.put(SCENES+68*index+60,color)
        elif target==0x8008d870:self.snapshot('texture',*r[4:7])
        elif target==0x800b61a8:
            self.snapshot('sound',*r[4:8]);ret=1
            if mut:
                owner=signed(r[5]);assert 0<=owner<4
                self.put(PLAYERS+952*owner+20,bits(2.75))
        elif target==0x8038f568:
            self.snapshot('player_update',r[4])
            if mut:self.put(r[4]+0x3b0,bits(0.25));self.put(r[4]+0x3b4,bits(0.125))
            if 'callback_count' in self.case:self.put(COUNT,self.case['callback_count'],2)
        elif target==0x8038d054:
            self.snapshot('attached_effect',r[4],r[5],r[6],self.vec(r[5]))
            assert r[5]==r[6], 'genuine duplicate D054 position argument lost'
            self.put(sp+8,r[6])  # Verified callee argument-home write.
            assert self.neffect<32,'effect storage exhausted'
            ret=EFFECTS+32*self.neffect;self.neffect+=1;self.put(ret+8,0x200)
        elif target==0x8038cb20:self.snapshot('frame_tail')
        elif target==0x8038dddc:
            self.snapshot('steer',r[4])
            if mut:self.put(r[4]+28,bits(7.5));self.put(r[4]+80,bits(-0.5))
        elif target==0x800add58:
            self.snapshot('world_collision',self.vec(r[4]),r[5],self.pointer(r[6]),r[7],self.get(sp+16),self.get(sp+20))
            self.matrix(r[6]);ret=self.case.get('collision',0)
            if mut:self.put(r[5]+4,bits(self.float_at(r[5]+4)+0.375))
        elif target==0x8038d798:
            self.snapshot('area_effect',self.vec(r[4]),self.vec(r[5]),r[6],r[7],self.get(sp+16))
            if mut:self.put(r[4],bits(self.float_at(r[4])+0.5))
        elif target==0x800af06c:self.snapshot('burst',self.vec(r[4]),*r[5:8])
        elif target==0x8038d3a4:
            owner,player,damage=r[4:7];self.snapshot('damage',owner,player,signed(damage))
            assert PLAYERS<=player<PLAYERS+952*4 and (player-PLAYERS)%952==0
            self.put(player+0x388,damage,2)
            self.put(player+0x386,max(0,signed(self.get(player+0x386,2),16)-signed(damage)),2)
            if mut:self.put(player+8,bits(self.float_at(player+8)+0.25))
        elif target==0x800abcc8:self.snapshot('rest_effect',r[4],r[5],self.vec(r[6]),r[7],*(self.get(sp+i) for i in (16,20,24)))
        elif target==0x800aefe0:self.snapshot('bounce_effect',self.vec(r[4]),r[5],r[6],r[7],*(self.get(sp+i) for i in range(16,40,4)))
        elif target==0x800b24ec:
            self.snapshot('resource',*r[4:8],self.get(sp+16));self.put(r[5],13,2);ret=0
        elif target==0x800a78bc:
            self.snapshot('quad',r[4],self.vec(r[5],12),r[6],r[7],self.get(r[7]),self.get(sp+16),self.get(sp+20))
            ret=0 if self.case.get('quad_fail') else self.new_quad()
        else:raise AssertionError(('unknown external call',hex(target)))
        for k in list(range(1,16))+[24,25]:r[k]=(0xbad00000+k*79)&MASK
        for k in range(20):f[k]=0x7fc00000+k
        r[2]=ret
        if fret is not None:f[0]=fret

    def run(self):
        pc,pending=self.start,None;r,f=self.r,self.f
        for steps in range(200000):
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
                elif fn==4:r[rd]=(r[rt]<<(r[rs]&31))&MASK
                elif fn==6:r[rd]=r[rt]>>(r[rs]&31)
                elif fn==7:r[rd]=(signed(r[rt])>>(r[rs]&31))&MASK
                elif fn==3:r[rd]=(signed(r[rt])>>sh)&MASK
                elif fn==0x21:r[rd]=(r[rs]+r[rt])&MASK
                elif fn==0x23:r[rd]=(r[rs]-r[rt])&MASK
                elif fn==0x24:r[rd]=r[rs]&r[rt]
                elif fn==0x25:r[rd]=r[rs]|r[rt]
                elif fn==0x26:r[rd]=r[rs]^r[rt]
                elif fn==0x27:r[rd]=~(r[rs]|r[rt])&MASK
                elif fn==0x2a:r[rd]=int(signed(r[rs])<signed(r[rt]))
                elif fn==0x2b:r[rd]=int(r[rs]<r[rt])
                elif fn==0x19:
                    v=r[rs]*r[rt];self.lo=v&MASK;self.hi=(v>>32)&MASK
                elif fn==0x12:r[rd]=self.lo
                elif fn==8:pending=r[rs]
                elif fn==9:
                    assert rd==31, 'unsupported non-RA JALR'
                    target=r[rs];r[rd]=pc+8;pending=('call',target,pc+8)
                else:raise AssertionError(('unsupported SPECIAL',hex(pc),fn))
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
            elif op==53:
                assert a%8==0 and rt%2==0, 'invalid doubleword FP load'
                f[rt]=self.get(a);f[rt+1]=self.get(a+4)
            elif op==61:
                assert a%8==0 and rt%2==0, 'invalid doubleword FP store'
                self.put(a,f[rt]);self.put(a+4,f[rt+1])
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
                    elif fn==13:
                        value=math.trunc(real(f[rd]));assert -2147483648<=value<2147483648, 'out-of-range integer conversion'
                        f[sh]=value&MASK
                    elif fn==0x32:self.condition=real(f[rd])==real(f[rt])
                    elif fn==0x3c:self.condition=real(f[rd])<real(f[rt])
                    elif fn==0x3e:self.condition=real(f[rd])<=real(f[rt])
                    else:raise AssertionError(('unsupported COP1 single',hex(pc),fn))
                elif rs==20 and fn==32:f[sh]=bits(signed(f[rd]))
                else:raise AssertionError(('unsupported COP1',hex(pc),rs))
            else:raise AssertionError(('unsupported opcode',hex(pc),op))
            r[0]=0
            assert old is None or pending is None,'branch in delay slot'
            if isinstance(old,tuple):
                if old[1] in self.code:
                    self.internal_edges.add((pc-4,old[1]));pc=old[1]
                else:self.call(old[1]);pc=old[2]
            else:
                if old is not None and old not in self.code and old!=RETURN:
                    self.call(old);pc=r[31]
                else:pc=old if old is not None else next_pc
        else:raise AssertionError('step limit')
        assert r[29]==STACK and r[28]==self.original_r[28], 'root stack/global-pointer restoration'
        assert r[16:24]==self.original_r[16:24] and r[30]==self.original_r[30], 'root nonvolatile GPR preservation'
        assert f[20:]==self.original_f[20:], 'root nonvolatile FPR preservation'
        self.steps+=steps
        return self.state_digest(),self.trace
