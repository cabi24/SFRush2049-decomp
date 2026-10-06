"""Bounded native execution, separate oracle, and unchanged sanitized C host."""
import ctypes,hashlib,itertools,random,struct,subprocess
from pathlib import Path
HERE=Path(__file__).resolve().parent
ENTRY,SIZE=0x803A3A6C,816
HIDDEN,UPDATE,PROJECT=0x80094F88,0x80094EC8,0x800A6244
COUNT,FLAGS,SLOTS,LOW,HIGH,VIEW,CAR,COLORS,PALETTE=0x8014A108,0x803BA028,0x803AF9A8,0x803B97A0,0x803B97A4,0x80150B70,0x803B9FD0,0x803B9B60,0x801226C0
TABLES=(0x80111611,0x80111655,0x80111699)
OWNER,SP,STOP=0x81001000,0x81010000,0x81234560
MASK=0xffffffff

def signed(v,bits=32):
 v&=(1<<bits)-1
 return v-(1<<bits) if v&(1<<(bits-1)) else v

def fbits(v):return struct.unpack('>I',struct.pack('>f',v))[0]
def fvalue(v):return struct.unpack('>f',struct.pack('>I',v))[0]
def half(v):return (-1 if v<0 else 1)*(abs(v)//2)
def initial_colors():return [[(i*61+j*19+k+17)&255 for k in range(4)] for i in range(4) for j in range(3)]
def table(p,car,ch):return (p*(3,5,7)[ch]+car+(0,5,10)[ch])%16

def oracle(c):
 p,k,count,flag,hide,w,h,z,alpha,low,high,car,px,py,hook,junk=c
 b=[-321,654,w,h,77,hide,1,signed(k<<16|p|junk&0xff00fff0),-1]
 colors=initial_colors();trace=[];updates=0
 def emit(event,arg):trace.extend([event,arg]+b+[count,alpha if p<4 else -1,car if p<4 else -1]+sum(colors,[]))
 def effect(stage):
  nonlocal alpha,car
  if hook==stage:
   b[2:4]=[-31,19];b[7]=signed(b[7]^0xa5551230)
   if p<4:alpha=201;car=2
  if stage==1 and hook==3:b[5]=0
  if stage==1 and hook==4:b[5]=-1
 def update():
  nonlocal updates
  emit(2,0);updates+=1
  if updates==1:effect(1)
 def hidden(v):
  emit(1,v)
  if v!=b[5]:b[5]=v;update()
  return b[5]
 if p>=count:
  hidden(1);b[6]=0;emit(4,1);return trace
 assert p<4
 if flag:
  hidden(1);emit(4,1);return trace
 if hidden(int((fvalue(low)<fvalue(z)<fvalue(high)) or alpha==0)):
  emit(4,1);return trace
 assert k in (7,8,9)
 trace.extend([5,p,p*2816+(k+19)*64+48,p*152,0]+[signed(fbits(p*1000+(k+19)*20+i)) for i in (12,13,14)]+[signed(fbits(p*20+i)) for i in (10,11,12)])
 emit(3,0);effect(2)
 b[0]=signed(px-half(b[2]),16);b[1]=signed(py-half(b[3]),16);b[4]=alpha
 ch=k-7;pal=table(p,car,ch);colors[p*3+ch]=[(pal*13+1)&255,(pal*7+2)&255,(pal*3+3)&255,alpha]
 b[8]=(p*3+ch)*4;b[7]=signed((b[7]&0x0fffffff)|((pal<<28)&MASK));update();emit(4,1);return trace

class Native:
 def __init__(self,words,hidden):
  assert len(words)==SIZE//4 and len(hidden)==15
  self.code={ENTRY+4*i:w for i,w in enumerate(words)};self.code.update({HIDDEN+4*i:w for i,w in enumerate(hidden)})
  self.coverage=set();self.branches=set()
 def run(self,c,salt=0):
  p,k,count,flag,hide,w,h,z,alpha,low,high,car,px,py,hook,junk=c
  rng=random.Random(salt^0x3a6c);memory={} if not hasattr(self,"template") else self.template.copy();trace=[];updates=0;stores=[];reads=[]
  if not hasattr(self,"template"):
   ranges=[(OWNER,64),(SP-256,272),(COUNT,2),(FLAGS,4),(SLOTS,4*2816),(LOW,8),(VIEW,4*152),(CAR,4),(COLORS,48),(PALETTE,64)]+[(a,52) for a in TABLES]
   for a,n in ranges:
    assert not(set(range(a,a+n))&memory.keys()),('overlap',hex(a))
    memory.update((a+i,rng.randrange(256)) for i in range(n))
  def mem(a,n,v=None,track=False):
   assert a%n==0 and all(a+i in memory for i in range(n)),('invalid memory',hex(a),n)
   if v is None:
    if track:reads.append((a,n))
    return int.from_bytes(bytes(memory[a+i] for i in range(n)),'big')
   if track:stores.append((a,n,v&((1<<(8*n))-1)))
   for i,byte in enumerate((v&((1<<(8*n))-1)).to_bytes(n,'big')):memory[a+i]=byte
  if not hasattr(self,"template"):
   for i in range(4):
    mem(FLAGS+i,1,flag if i==p else i+3);mem(CAR+i,1,car if i==p else (i+3)%13)
    for j in range(44):
     for q in range(16):mem(SLOTS+i*2816+j*64+q*4,4,fbits(i*1000+j*20+q))
     mem(SLOTS+i*2816+j*64+60,1,i*41+j*7+1)
    for j in range(12):mem(VIEW+i*152+j*4,4,fbits(i*20+j+1))
    for ch,a in enumerate(TABLES):
     for j in range(13):mem(a+i*13+j,1,table(i,j,ch))
   for i,col in enumerate(initial_colors()):
    for j,v in enumerate(col):mem(COLORS+i*4+j,1,v)
   for i in range(16):
    for j,v in enumerate((i*13+1,i*7+2,i*3+3,i*5+4)):mem(PALETTE+i*4+j,1,v)
   self.template=dict(memory)
  for i in range(4):
   mem(FLAGS+i,1,flag if i==p else i+3);mem(CAR+i,1,car if i==p else (i+3)%13)
  mem(COUNT,2,count);mem(LOW,4,low);mem(HIGH,4,high)
  if p<4:mem(SLOTS+p*2816+k*64+8,4,z);mem(SLOTS+p*2816+k*64+60,1,alpha)
  for a,n,v in ((14,2,-321),(16,2,654),(20,2,w),(22,2,h),(24,1,77),(26,1,hide),(40,4,ENTRY),(44,4,k<<16|p|junk&0xff00fff0),(4,4,0)):mem(OWNER+a,n,v)
  before_memory=dict(memory)
  def emit(event,arg):
   b=[signed(mem(OWNER+a,2),16) for a in (14,16,20,22)]+[mem(OWNER+24,1),signed(mem(OWNER+26,1),8),int(mem(OWNER+40,4)!=0),signed(mem(OWNER+44,4)),mem(OWNER+4,4)-COLORS if COLORS<=mem(OWNER+4,4)<COLORS+48 else -1]
   trace.extend([event,arg]+b+[signed(mem(COUNT,2),16),mem(SLOTS+p*2816+k*64+60,1) if p<4 else -1,signed(mem(CAR+p,1),8) if p<4 else -1]+[mem(COLORS+i,1) for i in range(48)])
  def effect(stage):
   if hook==stage:
    mem(OWNER+20,2,-31);mem(OWNER+22,2,19);mem(OWNER+44,4,mem(OWNER+44,4)^0xa5551230)
    if p<4:mem(SLOTS+p*2816+k*64+60,1,201);mem(CAR+p,1,2)
   if stage==1 and hook==3:mem(OWNER+26,1,0)
   if stage==1 and hook==4:mem(OWNER+26,1,-1)
  r=[rng.getrandbits(32) for _ in range(32)];f=[rng.getrandbits(32) for _ in range(32)];r[0]=0;r[4]=OWNER;r[29]=SP;r[31]=STOP
  before=list(r);saved_f=f[20:];pc=ENTRY;pending=None;cond=False;steps=0
  allowed=set(range(SP-96,SP+4))|set(range(OWNER+14,OWNER+18))|{OWNER+24,OWNER+26}|set(range(OWNER+4,OWNER+8))|set(range(OWNER+40,OWNER+48))|set(range(COLORS,COLORS+48))
  while pc!=STOP:
   steps+=1;assert steps<1000
   if pc==HIDDEN:
    assert pending is None and r[4]==OWNER;emit(1,signed(r[5]))
   if pc in (PROJECT,UPDATE):
    assert pending is None
    ret=r[31]
    if pc==UPDATE:
     assert r[4]==OWNER;emit(2,0);updates+=1
     if updates==1:effect(1)
    else:
     assert p<4 and k in (7,8,9)
     assert r[4]==p and r[5]==SLOTS+p*2816+(k+19)*64+48 and r[6]==VIEW+p*152 and r[7]==0
     out=mem(r[29]+16,4);assert out==SP-20 and all(out+i in allowed for i in range(4))
     trace.extend([5,signed(r[4]),r[5]-SLOTS,r[6]-VIEW,r[7]]+[signed(mem(r[5]+4*i,4)) for i in range(3)]+[signed(mem(r[6]+36+4*i,4)) for i in range(3)])
     emit(3,0);mem(out,2,px);mem(out+2,2,py);effect(2)
    for i in (1,2,3,*range(4,16),24,25):r[i]=rng.getrandbits(32)
    f[:20]=[rng.getrandbits(32) for _ in range(20)];pc=ret;continue
   assert pc in self.code,('unknown code',hex(pc))
   self.coverage.add(pc);wrd=self.code[pc];op=wrd>>26;rs=wrd>>21&31;rt=wrd>>16&31;rd=wrd>>11&31;sh=wrd>>6&31;fn=wrd&63;imm=wrd&65535;si=signed(imm,16);address=(r[rs]+si)&MASK
   old,pending=pending,None;nextpc=pc+4
   if wrd==0:pass
   elif op==0:
    if fn==0:r[rd]=r[rt]<<sh
    elif fn==2:r[rd]=r[rt]>>sh
    elif fn==3:r[rd]=signed(r[rt])>>sh
    elif fn==8:pending=r[rs]
    elif fn==33:r[rd]=r[rs]+r[rt]
    elif fn==35:r[rd]=r[rs]-r[rt]
    elif fn==36:r[rd]=r[rs]&r[rt]
    elif fn==37:r[rd]=r[rs]|r[rt]
    elif fn==42:r[rd]=int(signed(r[rs])<signed(r[rt]))
    else:raise AssertionError(('special',hex(pc),fn))
   elif op==1:
    assert rt==1;take=signed(r[rs])>=0;self.branches.add((pc,take))
    if take:pending=pc+4+si*4
   elif op==3:
    r[31]=pc+8;pending=((pc+4)&0xf0000000)|((wrd&0x3ffffff)<<2);assert pending in (HIDDEN,UPDATE,PROJECT)
   elif op in (4,5,20,21):
    take=(r[rs]==r[rt]) if op in (4,20) else (r[rs]!=r[rt]);self.branches.add((pc,take))
    if take:pending=pc+4+si*4
    elif op in (20,21):nextpc+=4
   elif op==9:r[rt]=address
   elif op==11:r[rt]=int(r[rs]<(si&MASK))
   elif op==12:r[rt]=r[rs]&imm
   elif op==13:r[rt]=r[rs]|imm
   elif op==15:r[rt]=imm<<16
   elif op==17:
    if rs==8:
     assert rt in (0,1);take=cond if rt==1 else not cond;self.branches.add((pc,take))
     if take:pending=pc+4+si*4
    elif rs==16:
     assert fn==60;cond=fvalue(f[rd])<fvalue(f[rt])
    else:raise AssertionError(('cop1',hex(pc),rs))
   elif op in (32,33,35,36):
    n={32:1,33:2,35:4,36:1}[op];v=mem(address,n,track=True);r[rt]=signed(v,n*8) if op in (32,33) else v
   elif op in (40,41,43):
    n={40:1,41:2,43:4}[op];assert all(address+i in allowed for i in range(n)),('write confinement',hex(address));mem(address,n,r[rt],True)
   elif op==49:f[rt]=mem(address,4,track=True)
   else:raise AssertionError(('opcode',hex(pc),op))
   r[:]=[v&MASK for v in r];r[0]=0;pc=old if old is not None else nextpc
  assert pending is None and f[20:]==saved_f
  assert all(r[i]==before[i] for i in (*range(16,24),26,27,28,29,30,31)),('saved register',c)
  mutable=allowed|set(range(OWNER+20,OWNER+24))|{CAR+p,SLOTS+p*2816+k*64+60}
  assert all(v==before_memory[a] for a,v in memory.items() if a not in mutable)
  emit(4,signed(r[2]));return trace

def cases():
 def c(p=0,k=7,count=4,flag=0,hide=0,w=123,h=-55,z=3,alpha=201,lo=-.001,hi=.001,car=0,px=111,py=-222,hook=0,junk=0x52000aa0):return(p,k,count,flag,hide,w,h,fbits(z),alpha,fbits(lo),fbits(hi),car,px,py,hook,junk)
 for p,k,car in itertools.product(range(4),(7,8,9),range(13)):
  for hook in range(5):yield c(p,k,car=car,hide=-1,hook=hook)
 for p,k in itertools.product(range(4),(7,8,9)):
  for z,lo,hi in ((-.002,-.001,.001),(-.001,-.001,.001),(-0.,-.001,.001),(0.,-.001,.001),(.001,-.001,.001),(.002,-.001,.001),(3,4,2),(-3,-4,-2),(3,3,3)):
   for alpha,hook in itertools.product((0,1,127,128,255),(0,3,4)):yield c(p,k,z=z,lo=lo,hi=hi,alpha=alpha,hook=hook,hide=-1)
 for p,count in itertools.product((0,1,3,4,15),(-32768,-1,0,1,2,4,32767)):
  if p>=4 and p<count:continue
  yield c(p,count=count,hook=0)
 for flag in range(-128,128):yield c(flag=flag)
 for k in (0,6,10,24,43):yield c(k=k,flag=1);yield c(k=k,count=0)
 for w,h,px,py in itertools.product((-32768,-32767,-3,-1,0,1,3,32767),(-32768,-3,-1,0,1,3,32767),(-32768,0,32767),(-32768,0,32767)):
  yield c(w=w,h=h,px=px,py=py)

def host(build,source=None,label='host'):
 out=build/(label+'.so');cmd=['gcc','-std=c89','-O2','-Wall','-Wextra','-Werror','-Wno-maybe-uninitialized','-shared','-fPIC','-fstrict-aliasing','-ffp-contract=off','-fsanitize=undefined,bounds','-fno-sanitize-recover=all']
 if source:cmd.append('-DCANDIDATE="'+str(source)+'"')
 p=subprocess.run(cmd+[str(HERE/'host.c'),'-o',str(out)],capture_output=True,text=True);assert p.returncode==0,p.stderr
 lib=ctypes.CDLL(str(out));fn=lib.host_run;fn.argtypes=[ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int)];fn.restype=ctypes.c_int
 def run(c):
  out=(ctypes.c_int*2048)();n=fn((ctypes.c_int*16)(*c),out);return list(out[:n])
 return run

def verify(build,native,relocated,linked,hidden):
 engines=[Native(w,hidden) for w in (native,relocated,linked)];compiled=host(build);samples=list(cases());digest=hashlib.sha256()
 for i,c in enumerate(samples):
  want=oracle(c)
  for label,e in zip(('native','project','GNU'),engines):
   got=e.run(c,i);assert got==want,(label,c,next((j for j,(a,b) in enumerate(zip(got,want)) if a!=b),min(len(got),len(want))),got,want)
  got=compiled(c);assert got==want,('host',c,got,want)
  digest.update(struct.pack('>%di'%len(want),*want))
 missing=sorted(set(range(ENTRY,ENTRY+SIZE,4))-engines[0].coverage)
 return {'cases':len(samples),'native_executions':len(samples)*3,'host_executions':len(samples),'target_instructions_executed':len(set(range(ENTRY,ENTRY+SIZE,4))&engines[0].coverage),'unexecuted_target_offsets':[hex(a-ENTRY) for a in missing],'hidden_instructions_executed':len(set(range(HIDDEN,HIDDEN+60,4))&engines[0].coverage),'branches':[[hex(a),v] for a,v in sorted(engines[0].branches)],'trace_sha256':digest.hexdigest()},samples
