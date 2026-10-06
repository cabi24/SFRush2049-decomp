"""Explicit external effect model and endian-neutral target-memory schema.

Boundary services model ordinary calls, not complete helper/game execution.
The complete private90308 and root AF06C run in the native instruction engine.
"""
from native import STACK, MASK, signed, fbits, fvalue

PLAYERS,GROUPS,EXTRAS,SCENES = 0x80152818,0x80154660,0x80154FD8,0x8012E700
NODES,MATRICES,INPUT = 0x80600000,0x80601000,0x80603000
HEAD,FREE,USED,PEAK,RING,RING_TABLE = 0x801391F0,0x801392C8,0x8012E66C,0x8012E678,0x8013C094,0x8013C238
# Sizes are target O32 sizes. Only globals actually read/written are mapped.
SCHEMA = [
 ('D_80156994',0x80156994,1),('D_8014978C',0x8014978C,1),
 ('D_8010FFC0',0x8010FFC0,1),('D_8014A108',0x8014A108,2),
 ('D_8013C094',RING,2),('D_8011735C',0x8011735C,4),
 ('D_8011B554',0x8011B554,4),('D_8011B550',0x8011B550,4),
 ('D_801543CC',0x801543CC,4),
 ('D_801239A8',0x801239A8,4),('D_801239AC',0x801239AC,4),
 ('D_801239B0',0x801239B0,4),('D_801239B4',0x801239B4,4),
 ('D_801239B8',0x801239B8,4),('D_801239BC',0x801239BC,4),
 ('D_801239C0',0x801239C0,4),('D_801239C4',0x801239C4,4),
 ('D_80123C00',0x80123C00,4),('D_80123C04',0x80123C04,4),
 ('D_8011418C',0x8011418C,36),('D_801141B0',0x801141B0,12),
 ('D_801428FC',0x801428FC,8),('D_80142904',0x80142904,2),
 ('D_80142906',0x80142906,2),('D_80142908',0x80142908,2),
 ('D_80152818',PLAYERS,6*952),('D_80154660',GROUPS,6*400),
 ('D_80154FD8',EXTRAS,6*60),('D_8012E700',SCENES,128*68),
 ('D_8013C238',RING_TABLE,50*4),('D_801391F0',HEAD,4),
 ('D_801392C8',FREE,4),('D_8012E66C',USED,2),('D_8012E678',PEAK,2),
 ('nodes',NODES,8*24),('matrices',MATRICES,50*48),('input',INPUT,12),
]

class Memory:
    def __init__(self):
        self.regions=[(a,bytearray((i*29+73)&255 for i in range(n))) for _,a,n in SCHEMA]
        self.regions += [(STACK-1024,bytearray(2048))]
        self.reads,self.writes,self.pc=[],[],None
    def region(self,a,n):
        assert a%n==0,('unaligned',hex(a),n,self.pc)
        for base,data in self.regions:
            if base<=a and a+n<=base+len(data):return data,a-base
        raise AssertionError(('unmapped',hex(a),n,self.pc and hex(self.pc)))
    def get(self,a,n=4):
        data,off=self.region(a,n);self.reads.append((self.pc,a,n));return int.from_bytes(data[off:off+n],'big')
    def put(self,a,v,n=4):
        data,off=self.region(a,n);v&=(1<<(8*n))-1;data[off:off+n]=v.to_bytes(n,'big');self.writes.append((self.pc,a,n,v))
    def rf(self,a):return fvalue(self.get(a))
    def wf(self,a,v):self.put(a,fbits(v))
    def snapshot(self):return tuple((a,bytes(data)) for a,data in self.regions if a!=STACK-1024)

class Fixture:
    def __init__(self,image=None,mode=1,index=0,scale=1.0,sound=0,count=2,shortcut=False,
                 alloc=(True,True),ring=0,seed=0x12345678,enabled=1,gate=None,selector=None,
                 position=(1.25,-2.5,3.75),old_scene=-1,mutation=None,scene_start=16):
        assert 0<=index<6 and 0<=ring<50
        self.memory=Memory();m=self.memory
        self.mode,self.index,self.scale,self.sound,self.input=mode,index,fvalue(fbits(scale)),sound,INPUT
        self.alloc,self.alloc_count,self.node_count=tuple(alloc),0,0
        self.mutation,self.created,self.scene_start=mutation,0,scene_start
        self.trace=[]
        for name,a,n in SCHEMA:
            if image is not None and 0x80086A50<=a and a+n<=0x80086A50+len(image):
                data,off=m.region(a,1);data[off:off+n]=image[a-0x80086A50:a-0x80086A50+n]
        for a,v,n in ((0x80156994,0 if shortcut else 1,1),(0x8014978C,0,1),
                      (0x8010FFC0,enabled,1),(0x8014A108,count,2),(RING,ring,2),
                      (0x8011735C,seed,4),(0x8011B554,0x12345678,4),(0x8011B550,0xCAFEBABE,4),
                      (HEAD,0,4),(FREE,NODES,4),(USED,0,2),(PEAK,0,2)):
            m.put(a,v,n)
        if gate is not None:m.put(0x80156994,gate,1)
        if selector is not None:m.put(0x8014978C,selector,1)
        constants={0x801543CC:12.25,0x801239A8:0.35,0x801239AC:12.5,0x801239B0:6.25,
                   0x801239B4:4.75,0x801239B8:1.125,0x801239BC:.2,0x801239C0:1.0,
                   0x801239C4:2.0,0x80123C00:.125,0x80123C04:5.0}
        if image is None:
            for a,v in constants.items():m.wf(a,v)
        for i in range(9):m.wf(0x8011418C+4*i,1.0 if i%4==0 else 0.125*(i+1))
        for i in range(3):m.wf(0x801141B0+4*i,0.)
        for i in range(7):m.put(0x801428FC+2*i,0x8100+i,2)
        for p in range(6):
            for j in range(3):m.wf(PLAYERS+952*p+8+4*j,(p+1)*10.0+j*1.25)
            for i in range(4):m.put(GROUPS+400*p+84*i,old_scene)
            m.put(GROUPS+400*p+336,old_scene);m.put(EXTRAS+60*p,old_scene)
        for i in range(50):
            m.put(RING_TABLE+4*i,MATRICES+48*i)
            for j in range(12):m.wf(MATRICES+48*i+4*j,float(i+j)/16.)
        for i in range(8):m.put(NODES+24*i,NODES+24*(i+1) if i<7 else 0)
        if mode==0 and not shortcut:
            for j,v in enumerate(position):m.wf(INPUT+4*j,v)
        else:m.put(INPUT,index,2)
        m.reads.clear();m.writes.clear()
    def service(self,d,*args):
        args=tuple(x&MASK for x in args);m=self.memory;result=0
        if d==0x80090284:
            assert not args
            ok=self.alloc[self.alloc_count] if self.alloc_count<len(self.alloc) else True
            self.alloc_count+=1
            result=m.get(FREE) if ok else 0
            self.trace.append(('allocate',result))
            if result:
                self.node_count+=1
                assert self.node_count<=8
                m.put(FREE,m.get(result))
                m.put(USED,m.get(USED,2)+1,2)
                if signed(m.get(PEAK,2),16)<signed(m.get(USED,2),16):m.put(PEAK,m.get(USED,2),2)
                m.put(result,0);m.put(result+4,0,2);m.put(result+6,-1,2);m.put(result+8,0,2)
                m.put(result+12,0);m.put(result+16,0);m.put(result+20,0)
        elif d==0x8008D6B0:
            a,b=args;self.trace.append(('matrix_copy',a,b))
            for i in range(9):m.put(b+4*i,m.get(a+4*i))
        elif d==0x80090088:
            a,b,c=args;self.trace.append(('remove',signed(a,16),b,c))
            # The focused boundary permits removal effects on scene records.
            if 0<=signed(a,16)<128:m.put(SCENES+68*signed(a,16)+20,0xFFFF,2)
        elif d==0x8008E26C:
            a,b,c,e=args;result=self.scene_start+self.created;self.created+=1
            assert 0<=result<128
            self.trace.append(('scene_create',a,b,signed(c,16),e,tuple(m.get(b+4*i) for i in range(12)),result))
            record=SCENES+68*result
            for i in range(17):m.put(record+4*i,0)
            m.put(record,e);m.put(record+8,b);m.wf(record+12,1.);m.wf(record+16,1.)
            m.put(record+20,a,2)
            for i in range(3):m.put(record+22+2*i,-1,2)
        elif d==0x8008EA10:
            a,b,c,e=args;self.trace.append(('event',signed(a,16),b,c,e))
        elif d==0x800AED64:
            assert len(args)==10
            self.trace.append(('sound',args[:],tuple(m.get(args[0]+4*i) for i in range(3))))
            result=0xC0FFEE
        else:raise AssertionError(('unmodeled service',hex(d),args))
        if self.mutation:self.mutation(self,d)
        return result
