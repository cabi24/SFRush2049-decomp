"""Bounded callback proof. Projection is a typed output contract, not simulated."""
import ctypes, hashlib, itertools, random, struct, subprocess
from pathlib import Path
HERE=Path(__file__).resolve().parent
ENTRY,SIZE=0x803AE63C,772
HIDDEN,UPDATE,PROJECT=0x80094F88,0x80094EC8,0x800A6244
SLOTS,LOW,HIGH,CUT,VIEW=0x803B6B14,0x803B99D4,0x803B99D8,0x803B99DC,0x80150B70
VALUES=(0x8013F1D8,0x80142726,0x80152030,0x80150EFC)
OWNER,SP,STOP=0x81001000,0x81010000,0x81234560
MASK=0xffffffff
FIELDS=(14,16,20,22,28,30,32,34)
def signed(v,b=32):
 v&=(1<<b)-1
 return v-(1<<b) if v&(1<<(b-1)) else v
def bits(v):return struct.unpack('>I',struct.pack('>f',v))[0]
def value(v):return struct.unpack('>f',struct.pack('>I',v))[0]
def div(v,n):return (-1 if v<0 else 1)*(abs(v)//n)
def initial(c):return [-321,654,c[3],c[4],-123,456,-789,1234,77,c[2],signed((c[0]<<16)|c[1]|0xa5f0)]
def oracle(c):
 k,seg,hide,w,h,z,alpha,low,high,cut,stat,px,py,hook=c
 b=initial(c);trace=[];updates=0;stats=[signed(stat+i*17,8) for i in range(4)]
 def emit(event,arg):trace.extend([event,arg]+b+[signed(z),alpha]+stats)
 def effect(stage):
  nonlocal z,alpha
  if hook==stage:
   b[2:4]=[-31,19];b[10]=signed(b[10]^0xa5551230);alpha=255;z=bits(9.0)
   stats[:]=[-7,11,-13,17]
  if stage==1 and hook==3:b[9]=0
  if stage==1 and hook==4:b[9]=-1
 def update():
  nonlocal updates
  emit(2,0);updates+=1
  if updates==1:effect(1)
 v=int((value(low)<value(z)<value(high)) or not alpha)
 emit(1,v)
 if v!=b[9]:b[9]=v;update()
 if b[9]:emit(4,1);return trace
 trace.extend([5,0,(k+21)*64+48,0,0]+[signed(bits((k+21)*20+i)) for i in (12,13,14)])
 emit(3,0);effect(2)
 b[0]=signed(px-div(b[2],2),16);b[1]=signed(py-div(div(b[3],4),2),16)
 b[4:8]=[0,signed(div(b[3],4)-1,16),0,signed(b[2]-1,16)]
 b[8]=254 if alpha==255 else alpha
 if value(cut)<value(z):
  b[4]=signed(b[4]+div(b[3],2),16);b[5]=signed(b[5]+div(b[3],2),16)
 if seg==1:
  b[4]=signed(b[4]+div(b[3],4),16);b[5]=signed(b[5]+div(b[3],4),16)
  if k in (14,15,18,19):b[7]=signed(div((b[2]-1)*stats[(14,15,18,19).index(k)],{14:3,15:4,18:5,19:2}[k]),16)
 update();emit(4,1);return trace

class Native:
 def __init__(self,words,hidden):
  assert len(words)==SIZE//4 and len(hidden)==15
  self.code={ENTRY+4*i:w for i,w in enumerate(words)}
  self.code.update({HIDDEN+4*i:w for i,w in enumerate(hidden)})
  self.coverage=set();self.branches=set()
 def run(self,c,salt=0):
  k,seg,hide,w,h,z,alpha,low,high,cut,stat,px,py,hook=c
  rng=random.Random(salt^0xe63c);memory={};trace=[];updates=0
  slot=SLOTS+(k+21)*64
  for a,n in [(OWNER,64),(SP-128,144),(slot,64),(LOW,12),(VIEW,152)]+[(v,1) for v in VALUES]:
   assert not(set(range(a,a+n))&memory.keys())
   memory.update((a+i,rng.randrange(256)) for i in range(n))
  def mem(a,n,v=None):
   assert a%n==0 and all(a+i in memory for i in range(n)),('memory',hex(a),n)
   if v is None:return int.from_bytes(bytes(memory[a+i] for i in range(n)),'big')
   for i,byte in enumerate((v&((1<<(8*n))-1)).to_bytes(n,'big')):memory[a+i]=byte
  b=initial(c)
  for a,v in zip(FIELDS,b):mem(OWNER+a,2,v)
  mem(OWNER+24,1,77);mem(OWNER+26,1,hide);mem(OWNER+44,4,b[10])
  for i in range(16):mem(slot+4*i,4,bits((k+21)*20+i))
  mem(slot+8,4,z);mem(slot+60,1,alpha)
  for a,v in ((LOW,low),(HIGH,high),(CUT,cut)):mem(a,4,v)
  for i,a in enumerate(VALUES):mem(a,1,stat+i*17)
  before_memory=dict(memory)
  def emit(event,arg):
   trace.extend([event,arg]+[signed(mem(OWNER+a,2),16) for a in FIELDS]+[mem(OWNER+24,1),signed(mem(OWNER+26,1),8),signed(mem(OWNER+44,4)),signed(mem(slot+8,4)),mem(slot+60,1)]+[signed(mem(a,1),8) for a in VALUES])
  def effect(stage):
   if hook==stage:
    mem(OWNER+20,2,-31);mem(OWNER+22,2,19);mem(OWNER+44,4,mem(OWNER+44,4)^0xa5551230)
    mem(slot+60,1,255);mem(slot+8,4,bits(9.0))
    for a,v in zip(VALUES,(-7,11,-13,17)):mem(a,1,v)
   if stage==1 and hook==3:mem(OWNER+26,1,0)
   if stage==1 and hook==4:mem(OWNER+26,1,-1)
  r=[rng.getrandbits(32) for _ in range(32)];f=[rng.getrandbits(32) for _ in range(32)]
  r[0]=0;r[4]=OWNER;r[29]=SP;r[31]=STOP;before=r[:];saved_f=f[20:]
  pc=ENTRY;pending=None;cond=False;steps=0;lo=hi=0
  allowed=set(range(SP-88,SP+4))|set(range(OWNER+14,OWNER+18))|set(range(OWNER+28,OWNER+36))|{OWNER+24,OWNER+26}
  while pc!=STOP:
   steps+=1;assert steps<1000
   if pc==HIDDEN:assert pending is None and r[4]==OWNER;emit(1,signed(r[5]))
   if pc in (PROJECT,UPDATE):
    assert pending is None;ret=r[31]
    if pc==UPDATE:
     assert r[4]==OWNER;emit(2,0);updates+=1
     if updates==1:effect(1)
    else:
     assert r[4]==0 and r[5]==slot+48 and r[6]==VIEW and r[7]==0
     out=mem(r[29]+16,4);assert out==SP-12
     trace.extend([5,0,r[5]-SLOTS,0,0]+[signed(mem(r[5]+4*i,4)) for i in range(3)])
     emit(3,0);mem(out,2,px);mem(out+2,2,py);effect(2)
    for i in (1,2,3,*range(4,16),24,25):r[i]=rng.getrandbits(32)
    f[:20]=[rng.getrandbits(32) for _ in range(20)];lo=hi=rng.getrandbits(32);pc=ret;continue
   assert pc in self.code,('unknown code',hex(pc));self.coverage.add(pc)
   word=self.code[pc];op=word>>26;rs=word>>21&31;rt=word>>16&31;rd=word>>11&31;sh=word>>6&31;fn=word&63;imm=word&65535;si=signed(imm,16);address=(r[rs]+si)&MASK
   old,pending=pending,None;nextpc=pc+4
   if word==0:pass
   elif op==0:
    if fn==0:r[rd]=r[rt]<<sh
    elif fn==2:r[rd]=r[rt]>>sh
    elif fn==3:r[rd]=signed(r[rt])>>sh
    elif fn==8:pending=r[rs]
    elif fn==18:r[rd]=lo
    elif fn==25:lo=(r[rs]*r[rt])&MASK;hi=(r[rs]*r[rt])>>32
    elif fn==26:
     assert signed(r[rt])!=0;lo=div(signed(r[rs]),abs(signed(r[rt])))*(-1 if signed(r[rt])<0 else 1);hi=signed(r[rs])-lo*signed(r[rt])
    elif fn==33:r[rd]=r[rs]+r[rt]
    elif fn==35:r[rd]=r[rs]-r[rt]
    elif fn==37:r[rd]=r[rs]|r[rt]
    else:raise AssertionError(('special',hex(pc),fn))
   elif op==1:
    assert rt==1;take=signed(r[rs])>=0;self.branches.add((pc,take))
    if take:pending=pc+4+si*4
   elif op==3:r[31]=pc+8;pending=((pc+4)&0xf0000000)|((word&0x3ffffff)<<2)
   elif op in (4,5,20,21):
    take=(r[rs]==r[rt]) if op in (4,20) else r[rs]!=r[rt];self.branches.add((pc,take))
    if take:pending=pc+4+si*4
    elif op in (20,21):nextpc+=4
   elif op==9:r[rt]=address
   elif op==11:r[rt]=int(r[rs]<(si&MASK))
   elif op==12:r[rt]=r[rs]&imm
   elif op==15:r[rt]=imm<<16
   elif op==17:
    if rs==8:
     assert rt in (0,1,2,3);take=cond if rt&1 else not cond;self.branches.add((pc,take))
     if take:pending=pc+4+si*4
     elif rt&2:nextpc+=4
    elif rs==16:assert fn==60;cond=value(f[rd])<value(f[rt])
    else:raise AssertionError(('cop1',hex(pc),rs))
   elif op in (32,33,35,36):
    n={32:1,33:2,35:4,36:1}[op];v=mem(address,n);r[rt]=signed(v,n*8) if op in (32,33) else v
   elif op in (40,41,43):
    n={40:1,41:2,43:4}[op];assert all(address+i in allowed for i in range(n)),('write',hex(address));mem(address,n,r[rt])
   elif op==49:f[rt]=mem(address,4)
   else:raise AssertionError(('opcode',hex(pc),op))
   r[:]=[v&MASK for v in r];r[0]=0;pc=old if old is not None else nextpc
  assert pending is None and f[20:]==saved_f
  assert all(r[i]==before[i] for i in (*range(16,24),26,27,28,29,30,31))
  mutable=allowed|set(range(OWNER+20,OWNER+24))|set(range(OWNER+44,OWNER+48))|set(range(slot+8,slot+12))|{slot+60,*VALUES}
  assert all(v==before_memory[a] for a,v in memory.items() if a not in mutable)
  emit(4,signed(r[2]));return trace

def cases():
 def c(k=14,seg=1,hide=-1,w=123,h=-55,z=3,alpha=201,low=-.001,high=.001,cut=1,stat=1,px=111,py=-222,hook=0):return (k,seg,hide,w,h,bits(z),alpha,bits(low),bits(high),bits(cut),stat,px,py,hook)
 for k,seg,hook in itertools.product((0,13,14,15,16,18,19,20,31,255,256),(0,1,2,15),range(5)):yield c(k,seg,hook=hook)
 for k,stat in itertools.product((14,15,18,19),range(-128,128)):yield c(k,stat=stat)
 for z,alpha,hide,hook in itertools.product((-3,-.001,-0.,.001,1,3),(0,1,254,255),(-1,0,1),(0,3,4)):yield c(z=z,alpha=alpha,hide=hide,hook=hook)
 for k,w,h,px,py in itertools.product((14,15,18,19),(-32768,-3,0,1,32767),(-32768,-3,0,1,32767),(-32768,32767),(-32768,32767)):yield c(k,w=w,h=h,px=px,py=py,stat=-128)
 for low,high,cut in ((3,2,1),(3,3,3),(-4,-2,-3),(4,2,3)):yield c(low=low,high=high,cut=cut)

def verify(native,linked,hidden):
 engines=[Native(w,hidden) for w in (native,linked)];digest=hashlib.sha256();samples=list(cases())
 for i,c in enumerate(samples):
  want=oracle(c)
  for label,e in zip(('native','GNU'),engines):assert e.run(c,i)==want,(label,c)
  digest.update(struct.pack('>%di'%len(want),*want))
 missing=sorted(set(range(ENTRY,ENTRY+SIZE,4))-engines[0].coverage)
 return {'cases':len(samples),'executions':len(samples)*2,'target_instructions_executed':SIZE//4-len(missing),'unexecuted_offsets':[hex(a-ENTRY) for a in missing],'hidden_instructions_executed':len(set(range(HIDDEN,HIDDEN+60,4))&engines[0].coverage),'branches':[[hex(a),v] for a,v in sorted(engines[0].branches)],'trace_sha256':digest.hexdigest()},samples
