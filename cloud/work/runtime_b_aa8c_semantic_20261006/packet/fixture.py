"""Explicit memory/service contracts shared by two independent control-flow engines.

The scene services are effect models, not claims that their internals were run.
Scene creation retains the supplied matrix pointer. Copy/scale have sequential
per-float behavior, and scene positioning follows the retained-pointer alias.
"""
import math
from native import STACK, MASK, signed, fvalue, fbits, f32
PLAYERS,PHYSICS,CACHE,SLOTS,GROUPS,SCENE = 0x80152818,0x8014A250,0x80399118,0x80399120,0x80399550,0x80139320
SOURCE,DEST = 0x80600000,0x80601000

class Memory:
    def __init__(self,image):
        self.regions=[]
        for address,size in ((PLAYERS,6*0x3B8),(PHYSICS,6*0x808),(SCENE,6*0x40),
            (CACHE,0x80399A70-CACHE),(0x80399B08,0x60),(0x801427C0,448),
            (0x80151AE8,64*8),(0x8002EB94,4),(SOURCE,0x2000),(STACK-256,512)):
            self.regions.append((address,bytearray((i*29+73)&255 for i in range(size))))
        self.regions.append((0x8038A400,bytearray(image)))
        self.reads,self.writes,self.pc=[],[],None
    def region(self,a,n):
        assert a%n==0,('unaligned',hex(a),n)
        for b,d in self.regions:
            if b<=a and a+n<=b+len(d): return d,a-b
        raise AssertionError(('unmapped',hex(a),n,self.pc and hex(self.pc)))
    def get(self,a,n=4):
        d,o=self.region(a,n);self.reads.append((self.pc,a,n));return int.from_bytes(d[o:o+n],'big')
    def put(self,a,v,n=4):
        d,o=self.region(a,n);v&=(1<<(8*n))-1;d[o:o+n]=v.to_bytes(n,'big');self.writes.append((self.pc,a,n,v))
    def rf(self,a): return fvalue(self.get(a))
    def wf(self,a,v): self.put(a,fbits(v))
    def snapshot(self): return tuple((a,bytes(d)) for a,d in self.regions if a!=STACK-256)

class Fixture:
    def __init__(self,image,owner=0,mode=0,cached=9,update=1,inhibit=0,model=0,trigger=0,
                 view=0,view_type=0,flags=1,alpha=32,slot_active=0,slot_timer=0.0,
                 state138=-1,state139=0,state13A=0,value13C=2.,value140=.1,value144=0.,
                 dt=.016,randoms=(.125,.625,.875,.375),source_alias=None,mutation=None,
                 live_mask=31,extra_live=True,handle_values=None,extra_value=None):
        assert owner in range(6)
        self.memory=Memory(image);m=self.memory
        self.owner,self.update=owner,update
        self.descriptor=PLAYERS+owner*0x3B8+0x2F0
        self.trace,self.objects,self.random_count=[],{},0
        self.randoms,self.created,self.mutation=randoms,0,mutation
        self.service_count=0
        self.offset_reads=[]
        for p in range(6):
            player,physics,scene=PLAYERS+p*0x3B8,PHYSICS+p*0x808,SCENE+p*0x40
            m.put(player+0x2F6,100+p,2);m.put(player+0x2F8,p,2)
            m.put(player+0x384,mode,1);m.put(player+0x385,3,1)
            m.put(player+0x35C,view,1);m.put(player+0x35D,view_type,1)
            m.put(player+0x34C,0x12345678);m.put(player+0x38C,flags)
            m.put(player+0x3A0,trigger,1);m.put(player+0x3A1,alpha,1)
            m.put(physics+8,model,1);m.put(physics+0x640,inhibit,1)
            m.put(CACHE+p,cached,1)
            m.put(scene+2,10+p,2);m.put(scene+0x1A,100+p,2)
            source=SOURCE+p*48
            if source_alias=='slot': source=SLOTS+p*0x10C+0x14
            elif source_alias=='group': source=GROUPS+p*0x148+0x14
            self.objects[10+p]={'transform':source,'flags':0,'color':0,'model':1}
            self.objects[100+p]={'transform':DEST+p*48,'flags':0,'color':0,'model':1}
            for i,v in enumerate((1.0,.125,-.25,.5,1.25,.375,-.125,.25,1.5,10.,-20.,30.)):
                if not source_alias: m.wf(source+i*4,v)
                m.wf(DEST+p*48+i*4,v+2.)
        for p in range(4):
            for base,stride in ((SLOTS,0x10C),(GROUPS,0x148)):
                b=base+p*stride
                for i in range(5):
                    handle=200+p*20+i+(0 if base==SLOTS else 5)
                    m.put(b+4*i,handle if live_mask & (1<<i) else -1)
                    if handle_values is not None:m.put(b+4*i,handle_values[i])
                    self.objects[handle]={'transform':b+0x14+48*i,'flags':0x42000,'color':0,'model':i}
                    for j in range(12): m.wf(b+0x14+48*i+4*j,1.+(j+3*i)/16.)
            s,g=SLOTS+p*0x10C,GROUPS+p*0x148
            m.put(s+0x104,slot_active);m.wf(s+0x108,slot_timer)
            m.put(g+0x104,210+p*20 if extra_live else -1)
            if extra_value is not None:m.put(g+0x104,extra_value)
            self.objects[210+p*20]={'transform':g+0x108,'flags':0x42000,'color':0,'model':6}
            for j in range(12):m.wf(g+0x108+4*j,2.+j/8.)
            m.put(g+0x138,state138,1);m.put(g+0x139,state139,1);m.put(g+0x13A,state13A,2)
            m.wf(g+0x13C,value13C);m.wf(g+0x140,value140);m.wf(g+0x144,value144)
        if source_alias:
            for p in range(4):
                source=self.objects[10+p]['transform']
                for i,v in enumerate((1.0,.125,-.25,.5,1.25,.375,-.125,.25,1.5,10.,-20.,30.)):m.wf(source+i*4,v)
        for i in range(224):m.put(0x801427C0+2*i,0x8000+i,2)
        # Separate ten-word and seven-word ID tables: never treat them as one C array.
        for i in range(10):m.put(0x80399B18+4*i,700+i)
        for i in range(7):m.put(0x80399B40+4*i,900+i)
        for i in range(5):m.put(0x80399B08+2*i,((i%3)<<10)|(11+i),2)
        for i in range(64):m.put(0x80151AE8+8*i,0x80500000+0x10000*i)
        m.wf(0x8002EB94,dt)
        m.reads.clear();m.writes.clear()
    def object(self,h):
        h=signed(h)
        assert h in self.objects,('invalid scene handle',h)
        return self.objects[h]
    def service(self,d,a,b,c,e):
        a,b,c,e=[x&MASK for x in (a,b,c,e)]
        m=self.memory;result=0
        if d==0x80090254:
            self.trace.append(('remove',signed(a,16),
                tuple(m.get(base+p*stride+4*i) for base,stride in ((SLOTS,0x10C),(GROUPS,0x148)) for p in range(4) for i in range(5)),
                tuple((m.get(GROUPS+p*0x148+0x104),m.get(GROUPS+p*0x148+0x139,1)) for p in range(4))))
            self.objects.pop(signed(a,16),None)
        elif d in (0x8008AE8C,0x8008B0D8):
            name='hide' if d==0x8008AE8C else 'show'
            self.trace.append((name,signed(a),b,c))
            obj=self.object(a)
            # Boolean visibility contract and mask are kept explicitly, without
            # pretending to verify all native scene-flag mode encodings here.
            obj[(name,b,c)]=True
        elif d==0x8008E06C:
            self.trace.append(('color',signed(a,16),b));self.object(signed(a,16))['color']=b
        elif d==0x8008B3A0:
            result=self.object(signed(a,16))['transform'];self.trace.append(('transform',signed(a,16),result))
        elif d==0x8008D6B0:
            self.trace.append(('copy',a,b))
            for i in range(9):m.put(b+4*i,m.get(a+4*i))
        elif d==0x8008E398:
            result=400+self.created;self.created+=1
            self.trace.append(('create',signed(a),b,signed(c,16),e,tuple(m.get(b+4*i) for i in range(12)),result))
            self.objects[result]={'transform':b,'flags':e,'color':0,'model':a&65535}
        elif d==0x8008B2E4:
            fraction=self.randoms[self.random_count%len(self.randoms)]
            result=fbits(fvalue(a)*fraction);self.random_count+=1
            self.trace.append(('random',a,result))
        elif d==0x80090770:
            self.trace.append(('model',signed(a,16),b&65535));self.object(signed(a,16))['model']=b&65535
        elif d==0x8008D6FC:
            self.trace.append(('position',signed(a,16),b,c))
            dst=self.object(signed(a,16))['transform']
            if b:
                for i in range(3):m.put(dst+0x24+4*i,m.get(b+4*i))
            if c:
                for i in range(9):m.put(dst+4*i,m.get(c+4*i))
        elif d==0x8009EA68:
            self.trace.append(('roll',a,b))
            angle=fvalue(a)
            if angle < -f32(.0001) or angle > f32(.0001):
                cs,sn=f32(math.cos(angle)),f32(math.sin(angle))
                # Deterministic in-place effect model at a checked service boundary.
                for i in range(3):
                    x,y=m.rf(b+4*i),m.rf(b+12+4*i)
                    m.wf(b+4*i,f32(x*cs)-f32(y*sn))
                    m.wf(b+12+4*i,f32(x*sn)+f32(y*cs))
        elif d==0x8008B32C:
            self.trace.append(('scale',a,b,c))
            for i in range(9):m.wf(b+4*i,m.rf(a+4*i)*fvalue(c))
        elif d==0x8008D870:
            self.trace.append(('texture',signed(a,16),b,signed(c)))
            self.object(signed(a,16))['texture']=b
        elif d==0:raise AssertionError(('invalid C semantic domain',a))
        else:raise AssertionError(('unmodeled service',hex(d)))
        self.service_count+=1
        if self.mutation:self.mutation(self,d)
        return result&MASK
