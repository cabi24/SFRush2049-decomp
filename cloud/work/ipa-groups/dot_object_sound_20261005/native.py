"""Fail-closed MIPS replay for the complete callback and two real native callees.

No native instructions or ROM data are embedded. Trigonometric and final sound
boundaries are explicit contracts. FCSR flags and NaN payloads are not modeled.
"""
import math
import struct

MASK=0xffffffff
STACK,ACTOR,NODE,HEAD,FREE,STOP=0x700000,0x710000,0x720000,0x720100,0x720200,0xfffffffc

def signed(n,bits=32):
    n&=(1<<bits)-1
    return n-(1<<bits) if n&(1<<(bits-1)) else n

def bits(x):return struct.unpack('>I',struct.pack('>f',x))[0]
def floating(n):return struct.unpack('>f',struct.pack('>I',n))[0]
def f(x):return floating(bits(x))

class Machine:
    def __init__(self,addresses,bodies,literals):
        self.a=addresses;self.start=addresses['func_8010DBB8'];self.literals=literals
        self.code={addresses[name]+4*i:w for name,body in bodies.items() for i,w in enumerate(body)}
        self.coverage={name:set() for name in bodies}
        self.ranges={name:(addresses[name],addresses[name]+4*len(body)) for name,body in bodies.items()}

    def run(self,case):
        available,player,index,flags,values=case;a=self.a
        regions={STACK:bytearray([0xd3]*1024),ACTOR:bytearray([0xa5]*96),
                 NODE:bytearray([0x6b]*24),HEAD:bytearray([0x79]*24),FREE:bytearray([0x53]*24),
                 a['player_array']:bytearray([0x3c]*(8*952)),a['D_80117530']:bytearray([0x17]*(16*48)),
                 a['D_801391F0']:bytearray(4),a['D_801392C8']:bytearray(4),
                 a['D_8012E66C']:bytearray(2),a['D_8012E678']:bytearray(2)}
        regions.update({p:bytearray(raw) for p,raw in self.literals.items()})
        reads,writes,calls=[],[],[]
        def mem(address,width,value=None):
            assert address%width==0,('unaligned',hex(address),width)
            for base,data in regions.items():
                off=address-base
                if 0<=off and off+width<=len(data):
                    if value is None:
                        reads.append((address,width));return int.from_bytes(data[off:off+width],'big')
                    writes.append((address,width));data[off:off+width]=(value&((1<<(8*width))-1)).to_bytes(width,'big');return
            raise AssertionError(('unmapped',hex(address),width))
        mem(ACTOR+4,1,flags);mem(ACTOR+16,2,index);mem(ACTOR+92,1,player)
        for i in range(9):mem(ACTOR+20+4*i,4,bits(values[i]))
        for i in range(3):
            mem(a['player_array']+player*952+20+4*i,4,bits(values[9+i]));mem(ACTOR+56+4*i,4,bits(values[12+i]))
        for i in range(16):
            mem(a['D_80117530']+i*48+12,4,10000+i*73);mem(a['D_80117530']+i*48+28,4,-2000+i*31)
        mem(NODE,4,FREE);mem(a['D_801392C8'],4,NODE if available else 0);mem(a['D_801391F0'],4,HEAD)
        mem(a['D_8012E66C'],2,123);mem(a['D_8012E678'],2,123)
        before={k:bytes(v) for k,v in regions.items()};reads.clear();writes.clear()
        r=[0xa5000000+i for i in range(32)];fp=[bits(123.5+i) for i in range(32)]
        r[0],r[4],r[29],r[31]=0,ACTOR,STACK+512,STOP
        saved,saved_f=r[:],fp[:]
        pc,pending,condition,steps=self.start,None,False,0
        def clobber():
            for j in list(range(1,16))+[24,25]:r[j]=0xbad00000+j
            for j in range(20):fp[j]=bits(-321.25-j)
        while pc!=STOP:
            if pc in (a['sinf'],a['cosf']):
                assert pending is None
                arg=floating(fp[12]);fn=math.sin if pc==a['sinf'] else math.cos
                result=bits(fn(arg));calls.append(('sin' if pc==a['sinf'] else 'cos',bits(arg)))
                clobber();fp[0]=result;pc=r[31];continue
            if pc==a['stat_lap_split']:
                assert pending is None
                calls.append(('sound',r[4],r[5],r[7],*(mem(r[6]+4*j,4) for j in range(3)),
                              int(mem(a['D_801391F0'],4)==NODE and mem(NODE,4)==HEAD and mem(NODE+12,4)==ACTOR)))
                clobber();r[2]=(-12345)&MASK;pc=r[31];continue
            assert pc in self.code and steps<2000,('invalid control',hex(pc))
            for name,(lo,hi) in self.ranges.items():
                if lo<=pc<hi:self.coverage[name].add(pc-lo)
            w=self.code[pc];op=w>>26;rs,rt,rd,sh=(w>>21)&31,(w>>16)&31,(w>>11)&31,(w>>6)&31
            imm=w&65535;si=signed(imm,16);address=(r[rs]+si)&MASK
            old,pending,next_pc=pending,None,pc+4
            if w==0:pass
            elif op==0:
                fn=w&63
                if fn==0:r[rd]=r[rt]<<sh
                elif fn==3:r[rd]=signed(r[rt])>>sh
                elif fn==8:pending=r[rs]
                elif fn==33:r[rd]=r[rs]+r[rt]
                elif fn==35:r[rd]=r[rs]-r[rt]
                elif fn==37:r[rd]=r[rs]|r[rt]
                elif fn==42:r[rd]=int(signed(r[rs])<signed(r[rt]))
                else:raise AssertionError(('special',hex(w)))
            elif op in (2,3):
                if op==3:r[31]=pc+8
                pending=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
            elif op in (4,5,20,21):
                take=(r[rs]==r[rt]) if op in (4,20) else (r[rs]!=r[rt])
                if take:pending=pc+4+4*si
                elif op in (20,21):next_pc+=4
            elif op==9:r[rt]=address
            elif op==12:r[rt]=r[rs]&imm
            elif op==13:r[rt]=r[rs]|imm
            elif op==15:r[rt]=imm<<16
            elif op in (32,33,35,36):
                width={32:1,33:2,35:4,36:1}[op];v=mem(address,width)
                r[rt]=v if op==36 else signed(v,width*8)
            elif op in (40,41,43):mem(address,{40:1,41:2,43:4}[op],r[rt])
            elif op==49:fp[rt]=mem(address,4)
            elif op==57:mem(address,4,fp[rt])
            elif op==17:
                fn=w&63
                if rs==4:fp[rd]=r[rt]
                elif rs==8:
                    assert rt in (0,1,2,3)
                    if condition==bool(rt&1):pending=pc+4+4*si
                    elif rt&2:next_pc+=4
                elif rs==16:
                    x,y=floating(fp[rd]),floating(fp[rt])
                    if fn==0:fp[sh]=bits(x+y)
                    elif fn==1:fp[sh]=bits(x-y)
                    elif fn==2:fp[sh]=bits(x*y)
                    elif fn==6:fp[sh]=fp[rd]
                    elif fn==60:condition=x<y
                    else:raise AssertionError(('COP1 operation',hex(w)))
                else:raise AssertionError(('COP1 format',hex(w)))
            else:raise AssertionError(('opcode',hex(w),hex(pc)))
            r=[v&MASK for v in r];r[0]=0;pc=old if old is not None else next_pc;steps+=1
        assert r[29]==saved[29] and r[16:24]==saved[16:24] and r[28]==saved[28] and r[30]==saved[30]
        assert fp[20:]==saved_f[20:]
        assert regions[STACK][:432]==before[STACK][:432] and regions[STACK][520:]==before[STACK][520:]
        mutable={STACK,ACTOR,NODE,a['D_801391F0'],a['D_801392C8'],a['D_8012E66C'],a['D_8012E678']}
        assert all(bytes(regions[k])==v for k,v in before.items() if k not in mutable)
        orig=bytearray(before[ACTOR]);now=bytearray(regions[ACTOR])
        for off,length in ((4,1),(20,36),(90,2)):orig[off:off+length]=now[off:off+length]
        assert orig==now
        sound=[c[1:] for c in calls if c[0]=='sound']
        out=[mem(ACTOR+90,2),mem(ACTOR+4,1)]+[mem(ACTOR+20+4*i,4) for i in range(9)]
        out+=[len(sound)]+(list(sound[0]) if sound else [0]*7)
        out+=[int(mem(a['D_801391F0'],4)==NODE),int(mem(a['D_801392C8'],4)==FREE),mem(a['D_8012E66C'],2),mem(a['D_8012E678'],2)]
        out+=([mem(NODE+4,2),mem(NODE+20,4),mem(NODE+16,4),int(mem(NODE+12,4)==ACTOR),int(mem(NODE,4)==HEAD),
               int(mem(NODE+6,2)==65535 and mem(NODE+8,2)==0 and mem(NODE+10,2)==0x6b6b)] if available else [0]*6)
        return out,calls,reads,writes,{k:bytes(v) for k,v in regions.items()}
