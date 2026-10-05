"""Bounded fail-closed integer/binary32 MIPS executor; all code comes from input.

Executes the complete layer helper, client wrapper, transform, lookup and
allocator. Only the two OS queues are modeled. The private helper ABI permits
s1/s2 and f20/f22/f24/f26 to be changed; s0 and f28 are incoming live values.
"""
import struct
MASK=0xffffffff
STACK,LAYER,ENTITIES,STOP=0x700000,0x710000,0x720000,0xfffffffc
NAMES=['mode_select_input','client_sync','entity_transform_calc','func_80091BA8','func_80091B00']
def signed(x,n=32):return (x&((1<<n)-1))-(1<<n) if x&(1<<(n-1)) else x&((1<<n)-1)
def floating(x):return struct.unpack('>f',struct.pack('>I',x))[0]
def bits(x):return struct.unpack('>I',struct.pack('>f',x))[0]

class Machine:
    def __init__(self,addresses,bodies):
        self.a=addresses;self.code={addresses[n]+i*4:w for n in NAMES for i,w in enumerate(bodies[n])}
        self.ranges={n:(addresses[n],addresses[n]+4*len(bodies[n])) for n in NAMES}
        self.coverage={n:set() for n in NAMES};self.branches={}
    def run(self,case):
        a=self.a;prefix=case['prefix'];assert 0<=prefix<=126
        regions={STACK:bytearray([0xd3]*1024),LAYER:bytearray([0xa5]*20),ENTITIES:bytearray([0xb6]*544),
            a['D_80142DD8']:bytearray([0xa5]*3072),a['D_80110244']:bytearray(4),a['D_80146104']:bytearray(4)}
        writes=[];reads=[]
        def mem(addr,width,value=None):
            assert addr%width==0,(hex(addr),'unaligned',width)
            for base,data in regions.items():
                off=addr-base
                if 0<=off and off+width<=len(data):
                    if value is None:reads.append((addr,width));return int.from_bytes(data[off:off+width],'big')
                    writes.append((addr,width));data[off:off+width]=(value&((1<<(width*8))-1)).to_bytes(width,'big');return
            raise AssertionError(('unmapped',hex(addr),width))
        for i,v in enumerate(case['layer']):mem(LAYER+4*i,4,v)
        mem(a['D_80110244'],4,ENTITIES);mem(a['D_80146104'],4,7)
        for i in range(8):
            mem(ENTITIES+68*i+12,4,256+i);mem(ENTITIES+68*i+26,1,case['counts'][i])
            for j in range(4):mem(ENTITIES+68*i+36+4*j,4,case['values'][i][j])
        for i in range(128):mem(a['D_80142DD8']+24*i+3,1,int(i<prefix))
        before={p:bytes(d) for p,d in regions.items()};reads.clear();writes.clear()
        r=[0xa5000000+i for i in range(32)];fp=[bits(10.25+i) for i in range(32)]
        r[0],r[16],r[29],r[31]=0,LAYER,STACK+512,STOP
        fp[12],fp[28]=case['input'];saved,savedfp=r[:],fp[:]
        pc=a[NAMES[0]];pending=None;condition=False;steps=0;events=[];lock_events=0;locked=False;deliveries=[]
        def clobber():
            for i in list(range(1,16))+[24,25]:r[i]=0xbad00000+i
            for i in range(20):fp[i]=bits(-400.5-i)
        while pc!=STOP:
            if pc in (a['osRecvMesg'],a['osJamMesg']):
                assert pending is None
                if pc==a['osRecvMesg']:
                    assert (r[4],r[5],r[6])==(a['D_80142728'],0,1) and not locked
                    kind=1;locked=True
                elif r[4]==a['D_80142728']:
                    assert (r[5],r[6])==(0,0) and locked
                    kind=2;locked=False
                else:
                    assert r[4]==a['D_801427A8'] and r[6]==0 and locked
                    kind=3;index=(r[5]-a['D_80142DD8'])//24;assert prefix<=index<prefix+2
                    deliveries.append(index)
                events.extend([kind]+[mem(LAYER+4*i,4) for i in range(3)])
                if kind!=3:
                    lock_events+=1
                    for event,field,value in case['mutations']:
                        if event==lock_events:mem(LAYER+field*4,4,value)
                ret=r[31];clobber();r[2]=0;pc=ret;continue
            assert pc in self.code and steps<10000,('invalid control',hex(pc))
            for name,(lo,hi) in self.ranges.items():
                if lo<=pc<hi:self.coverage[name].add(pc-lo)
            w=self.code[pc];op=w>>26;rs,rt,rd,shift=(w>>21)&31,(w>>16)&31,(w>>11)&31,(w>>6)&31
            imm=w&65535;si=signed(imm,16);addr=(r[rs]+si)&MASK
            old,pending,nextpc=pending,None,pc+4
            def branch(take,likely=False):
                nonlocal pending,nextpc
                self.branches.setdefault(pc,set()).add(bool(take))
                if take:pending=pc+4+4*si
                elif likely:nextpc+=4
            if w==0:pass
            elif op==0:
                fn=w&63
                if fn==0:r[rd]=r[rt]<<shift
                elif fn==3:r[rd]=signed(r[rt])>>shift
                elif fn==8:pending=r[rs]
                elif fn==33:r[rd]=r[rs]+r[rt]
                elif fn==35:r[rd]=r[rs]-r[rt]
                elif fn==36:r[rd]=r[rs]&r[rt]
                elif fn==37:r[rd]=r[rs]|r[rt]
                elif fn==42:r[rd]=int(signed(r[rs])<signed(r[rt]))
                else:raise AssertionError(('unknown special',hex(w)))
            elif op in (2,3):
                if op==3:r[31]=pc+8
                pending=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
            elif op in (4,5,20,21):branch((r[rs]==r[rt]) if op in (4,20) else r[rs]!=r[rt],op in (20,21))
            elif op==9:r[rt]=addr
            elif op==12:r[rt]=r[rs]&imm
            elif op==13:r[rt]=r[rs]|imm
            elif op==15:r[rt]=imm<<16
            elif op in (32,33,35,36):
                width={32:1,33:2,35:4,36:1}[op];v=mem(addr,width);r[rt]=v if op==36 else signed(v,width*8)
            elif op in (40,41,43):mem(addr,{40:1,41:2,43:4}[op],r[rt])
            elif op==49:fp[rt]=mem(addr,4)
            elif op==57:mem(addr,4,fp[rt])
            elif op in (53,61):
                assert rt%2==0
                if op==53:fp[rt]=mem(addr,4);fp[rt+1]=mem(addr+4,4)
                else:mem(addr,4,fp[rt]);mem(addr+4,4,fp[rt+1])
            elif op==17:
                fn=w&63
                if rs==0:r[rt]=fp[rd]
                elif rs==4:fp[rd]=r[rt]
                elif rs==8:assert rt in (0,1,2,3);branch(condition==bool(rt&1),bool(rt&2))
                elif rs==16:
                    if fn==6:fp[shift]=fp[rd]
                    elif fn==50:condition=floating(fp[rd])==floating(fp[rt])
                    elif fn==60:condition=floating(fp[rd])<floating(fp[rt])
                    else:raise AssertionError(('unknown fp',hex(w)))
                else:raise AssertionError(('unknown cop1',hex(w)))
            else:raise AssertionError(('unknown opcode',hex(w)))
            r=[v&MASK for v in r];r[0]=0;pc=old if old is not None else nextpc;steps+=1
        assert not locked
        assert r[29]==saved[29] and r[16]==saved[16] and r[19:24]==saved[19:24] and r[28]==saved[28] and r[30]==saved[30]
        assert fp[28:]==savedfp[28:]
        assert regions[STACK][:384]==before[STACK][:384] and regions[STACK][512:]==before[STACK][512:]
        allowed=lambda addr,width:(STACK+384<=addr and addr+width<=STACK+512) or (LAYER<=addr and addr+width<=LAYER+12) or (a['D_80142DD8']+24*prefix<=addr and addr+width<=a['D_80142DD8']+24*(prefix+2)) or any(addr==ENTITIES+68*i+26 and width==1 for i in range(8))
        assert all(allowed(addr,width) for addr,width in writes)
        new=[]
        for i in range(prefix,128):
            p=a['D_80142DD8']+24*i
            if mem(p+3,1)==0:continue
            new.extend([i,mem(p,2),mem(p+2,1),mem(p+3,1)]+[mem(p+4+j*4,4) for j in range(4)]+[(mem(p+20,4)-ENTITIES)//68])
        assert deliveries==list(range(prefix,prefix+len(new)//9))
        out=[mem(LAYER+4*i,4) for i in range(5)]+[mem(ENTITIES+68*i+26,1) for i in range(8)]+[len(new)//9]+new+[len(events)//4]+events
        return out,reads,writes
