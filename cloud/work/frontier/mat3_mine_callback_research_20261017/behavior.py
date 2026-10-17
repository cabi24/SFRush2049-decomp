"""Bounded Mine callback semantics, separate field model and MIPS execution.

Allocation, road query, list removal, display and explosion calls are deterministic
boundaries. The real nine-float CopyUV leaf executes. No runtime claim.
"""
from pathlib import Path
import sys,struct,itertools,random
ROOT=Path(__file__).resolve().parents[4];sys.path.insert(0,str(ROOT))
from tools.cloud import score
ENTRY=0x8010C974;COPY=0x8008D6B0
RELEASE,ROAD,NEW,TEXTURE,HIDE,SHOW,FREE,RADIUS,BLAST,REMOVE=(0x8009079C,0x800C4200,0x8008E26C,0x8008D870,0x8008AE8C,0x8008B0D8,0x800AFA84,0x8038D798,0x800AF06C,0x80090088)
PAUSE,DT,THRESHOLD,SLOTS,OBJECTS,BANKS,TEAM,COLORS=0x801170FC,0x8002EB94,0x80124954,0x80151528,0x8012E700,0x80151AE8,0x8012E67C,0x80121D54
MODEL_BASE,MODEL_RING,TEX_BASE,TEX_TEAM=0x80399B2C,0x80399B30,0x80399B00,0x80399AF8
COL_POOL,TARGET_POOL=0x80151AA8,0x80143FC8
TARGET,DATA,VISUAL,NODES=0x80500000,0x80510000,0x80520000,0x80530000
STACK,RETURN,MASK=0x81010000,0x81234560,0xffffffff

def require(c,m):
 if not c:raise ValueError(m)
def signed(x,n=32):
 x&=(1<<n)-1;return x-(1<<n) if x>>(n-1) else x
def bits(x):return struct.unpack('>I',struct.pack('>f',x))[0]
def real(x):return struct.unpack('>f',struct.pack('>I',x))[0]
def f32(x):return real(bits(x))
def load(m,a,n=4):
 require(a%n==0 and all(a+i in m for i in range(n)),f'invalid read {a:x}/{n}')
 return int.from_bytes(bytes(m[a+i] for i in range(n)),'big')
def store(m,a,n,v):
 require(a%n==0 and all(a+i in m for i in range(n)),f'invalid write {a:x}/{n}')
 for i,b in enumerate((v&((1<<(8*n))-1)).to_bytes(n,'big')):m[a+i]=b
def getf(m,a):return real(load(m,a))
def setf(m,a,v):store(m,a,4,bits(v))
def visible(m):return {a:v for a,v in m.items() if not STACK-1024<=a<STACK+256}

def base_fixtures():
 rng=random.Random(ENTRY)
 cases=itertools.product((0,1),(0,1),(0,1,3,7),(0,3),(False,True),(0,1))
 for op,pause,flags,occupancy,road,expired in cases:
  yield dict(op=op,pause=pause,flags=flags,occupancy=occupancy,road=road,age=120.2 if expired else 1.0,
    owner=rng.randrange(4),team=rng.randrange(4),dt=rng.choice((0.0,.016,.04,.3)),timer=rng.choice((0.,.24,.25)),
    phase=rng.randrange(2),step=rng.choice([(-1,-1),(0,-1),(19,24),(20,25),(44,45),(45,0)]),
    elapsed=[rng.choice((.01,.0333333,.04)) for _ in range(2)],alpha=[rng.randrange(8,256) for _ in range(2)],
    scale=[rng.choice((1.,1.25,2.)) for _ in range(2)],uv=[f32(rng.uniform(-1,1)) for _ in range(9)],
    pos=[f32(rng.uniform(-20,20)) for _ in range(3)],floor=[f32(rng.uniform(-1,1)) for _ in range(9)])

def fixtures():
 base=list(base_fixtures())
 yield from base
 # Exercise every possible empty slot, age ordering/ties, and initialization
 # when every prior age is non-positive. These are valid three-mine states.
 seed=next(c for c in base if c['op'] and not c['pause'] and not c['flags'] and c['occupancy']==3)
 for empty in range(4):
  for ages in ((2.,8.,5.,7.),(8.,8.,8.,8.),(-2.,0.,-1.,0.),(1.,2.,3.,9.)):
   c=dict(seed,empty_slot=empty,prior_ages=ages,road=bool(empty%2));yield c
 # Active mines may occupy any of the four slots. Cleanup accepts already
 # removed ring/base objects; keep both ring animations dormant in those cases.
 seed=next(c for c in base if c['op'] and not c['pause'] and c['flags']==7)
 for slot in range(4):
  for removed in range(8):
   c=dict(seed,active_slot=slot,step=(-1,-1),timer=0.,dt=0.,removed=removed,alpha=(8,9));yield c
 # Exact alpha threshold and alternating ring activation boundaries.
 seed=next(c for c in base if c['op'] and not c['pause'] and c['flags']==3)
 for alpha in (0,8,9,16,255):
  for step in ((19,-1),(20,-1),(24,-1),(25,-1),(-1,25),(45,-1)):
   yield dict(seed,alpha=(alpha,alpha),step=step,elapsed=(0.,0.),dt=0.)

def state(c):
 regions=[(TARGET,5*112),(DATA,136),(VISUAL,24),(NODES,24),(PAUSE,4),(DT,4),(THRESHOLD,4),
  (SLOTS,4*20),(OBJECTS,32*68),(BANKS,32),(TEAM,4),(COLORS,12),(MODEL_BASE,8),(TEX_BASE,2),(TEX_TEAM,8),
  (COL_POOL,24),(TARGET_POOL,24),(STACK-1024,1280)]
 m={a+i:0xa5 for a,n in regions for i in range(n)}
 for a,n,v in [(PAUSE,4,c['pause']),(VISUAL+12,4,TARGET),(TARGET+4,1,c['flags']),(TARGET+12,4,3),
  (TARGET+101,1,c['owner']),(TARGET+108,4,DATA),(TARGET,4,0),(MODEL_BASE,4,77),(MODEL_RING,4,88),
  (TEX_BASE,2,0x407),(TARGET_POOL,1,0),(TARGET_POOL+16,4,TARGET),(TARGET_POOL+20,4,0),
  (COL_POOL,1,0),(COL_POOL+16,4,NODES),(COL_POOL+20,4,0)]:store(m,a,n,v)
 setf(m,DT,c['dt']);setf(m,THRESHOLD,.0333333);setf(m,TARGET+104,c['age'])
 for i in range(4):
  store(m,TEAM+i,1,c['team'] if i==c['owner'] else i);store(m,TEX_TEAM+2*i,2,0x800+3*i)
  store(m,BANKS+8*i,4,0x80400000+i*0x10000)
  for j in range(3):store(m,COLORS+3*i+j,1,(31*i+71*j+19)&255)
  for j in range(4):store(m,SLOTS+i*20+j*4,4,0)
  store(m,SLOTS+i*20+16,4,0)
 for i in range(9):setf(m,TARGET+20+4*i,c['uv'][i])
 for i in range(3):setf(m,TARGET+68+4*i,c['pos'][i])
 for i in range(5):setf(m,TARGET+112*i+104,c['age'] if i==0 else c.get('prior_ages',(2.,8.,5.,7.))[i-1])
 count=c['occupancy'];items=[TARGET+112*(i+1) for i in range(count)]
 if c['flags']&1:items=[TARGET]+items[:3]
 slots=list(range(4))
 if not c['flags']&1 and count==3:slots.remove(c.get('empty_slot',3))
 if c['flags']&1:
  own=c.get('active_slot',0);slots=[own]+[i for i in slots if i!=own]
 for i,t in zip(slots,items):store(m,SLOTS+c['owner']*20+4*i,4,t)
 store(m,SLOTS+c['owner']*20+16,4,len(items))
 for i in range(3):
  store(m,NODES+8*i,4,NODES+8*(i+1) if i<2 else 0)
  store(m,NODES+8*i+4,4,TARGET if i!=1 else TARGET+112)
 for i,v in enumerate((9,10,11)):store(m,DATA+4*i,4,-1 if c.get('removed',0)&(1<<i) else v)
 for i in range(24):setf(m,DATA+16+4*i,f32((i-9)*.125))
 setf(m,DATA+12,c['timer']);store(m,DATA+132,2,c['phase']);store(m,DATA+134,2,1)
 for i in range(2):
  setf(m,DATA+112+4*i,c['elapsed'][i]);store(m,DATA+120+i,1,c['step'][i]);store(m,DATA+122+i,1,c['alpha'][i]);setf(m,DATA+124+4*i,c['scale'][i])
 for i in range(32):store(m,OBJECTS+i*68,4,0x100+i)
 return m

class Boundaries:
 def __init__(self,m,c):self.m=m;self.c=c;self.events=[];self.next_object=12
 def call(self,dest,args):
  m=self.m;c=self.c;a=args;self.events.append((dest,tuple(v&MASK for v in a)))
  if dest==RELEASE:store(m,a[0]+20,4,0);store(m,a[0]+6,2,-1)
  elif dest==ROAD:
   require(a[0]==TARGET+68,'road position contract')
   for i in range(3):setf(m,a[1]+4*i,10.+i)
   for i in range(9):setf(m,a[2]+4*i,c['floor'][i])
   return 1 if c['road'] else 0
  elif dest==NEW:
   result=self.next_object;self.next_object+=1;require(self.next_object<=16,'allocation bound')
   store(m,OBJECTS+result*68,4,a[3]);store(m,OBJECTS+result*68+8,4,a[1]);store(m,OBJECTS+result*68+60,4,0)
   return result
  elif dest==FREE:
   require(a[0] in (COL_POOL,TARGET_POOL),'pool base contract');prev=None;cur=load(m,a[0]+16)
   while cur:
    nxt=load(m,cur)
    if cur==a[1]:
     store(m,a[0]+16 if prev is None else prev,4,nxt);store(m,cur,4,load(m,a[0]+20));store(m,a[0]+20,4,cur);break
    prev=cur;cur=nxt
   require(cur==a[1],'free absent node')
  elif dest in (TEXTURE,HIDE,SHOW,RADIUS,BLAST,REMOVE):pass
  elif dest==COPY:
   for i in range(9):store(m,a[1]+4*i,4,load(m,a[0]+4*i))
  else:raise ValueError(f'unknown boundary {dest:x}')
  return 0xbadfeed

# Stack addresses are implementation details for RoadPosition's two outputs.
def public_events(events):
 return [(dest,arguments[:1] if dest==ROAD else arguments) for dest,arguments in events]

def model(c):
 m=state(c);b=Boundaries(m,c);call=b.call
 if not c['op']:call(RELEASE,[VISUAL,1]);return visible(m),public_events(b.events)
 if c['pause']:return visible(m),[]
 owner=c['owner'];slot=SLOTS+20*owner
 if not c['flags']&1:
  if load(m,slot+16)==3:
   live=[(getf(m,t+104),i,t) for i in range(4) if (t:=load(m,slot+4*i)) and getf(m,t+104)>0]
   if live:
    # First maximum wins, matching strict greater-than and iteration order.
    oldest=max(live,key=lambda x:x[0]);setf(m,oldest[2]+104,121.)
  for i in range(4):
   if load(m,slot+4*i)==0:store(m,slot+4*i,4,TARGET);store(m,slot+16,4,load(m,slot+16)+1);break
  if call(ROAD,[TARGET+68,STACK-100,STACK-160]):
   for i in range(9):setf(m,TARGET+20+4*i,c['floor'][i])
  base=call(NEW,[77,0,3,0x2080]);store(m,DATA,4,base)
  store(m,OBJECTS+base*68+60,4,0xffffffff);store(m,OBJECTS+3*68,4,load(m,OBJECTS+3*68)|0x2000);store(m,OBJECTS+3*68+60,4,0xffffffff)
  call(TEXTURE,[base,0x80410000+7*36,-1]);store(m,DATA+132,2,0);setf(m,DATA+12,0.)
  color=bytes([load(m,COLORS+c['team']*3+j,1) for j in range(3)]+[255]);word=int.from_bytes(color,'big')
  for i in range(2):call(COPY,[TARGET+20,DATA+16+48*i])
  for i in range(2):
   for j in range(3):setf(m,DATA+16+48*i+36+4*j,f32(f32(getf(m,DATA+16+48*i+12+4*j)*(.75 if i==0 else 1.))+c['pos'][j]))
   rid=call(NEW,[88,DATA+16+48*i,-1,0x42000]);store(m,DATA+4+4*i,4,rid);store(m,OBJECTS+rid*68+60,4,word)
  for i in range(2):
   store(m,DATA+122+i,1,255);store(m,DATA+120+i,1,0 if i==0 else -1);setf(m,DATA+124+4*i,1.);setf(m,DATA+112+4*i,0.)
  call(HIDE,[load(m,DATA+8),0,15]);store(m,TARGET+4,1,load(m,TARGET+4,1)|3)
 setf(m,DATA+12,f32(getf(m,DATA+12)+getf(m,DT)))
 if getf(m,DATA+12)>=.25:
  phase=load(m,DATA+132,2);store(m,DATA+132,2,0 if phase else 1)
  texture=0x407 if phase else 0x800+3*c['team'];call(TEXTURE,[load(m,DATA),0x80400000+(texture>>10)*0x10000+(texture&1023)*36,-1]);setf(m,DATA+12,0.)
 for i in range(2):
  count=signed(load(m,DATA+120+i,1),8);rid=load(m,DATA+4+4*i)
  if count<0:continue
  if count>=45:
   setf(m,DATA+124+4*i,1.);store(m,DATA+122+i,1,255);store(m,OBJECTS+rid*68+63,1,255);call(COPY,[TARGET+20,DATA+16+48*i]);store(m,DATA+120+i,1,-1);call(HIDE,[rid,0,15]);continue
  if getf(m,DATA+112+4*i)>=f32(.0333333):
   call(COPY,[TARGET+20,DATA+16+48*i])
   for k in range(9):setf(m,DATA+16+48*i+4*k,f32(getf(m,DATA+16+48*i+4*k)*getf(m,DATA+124+4*i)))
   setf(m,DATA+112+4*i,0.);setf(m,DATA+124+4*i,f32(getf(m,DATA+124+4*i)+.25));count+=1;store(m,DATA+120+i,1,count)
  setf(m,DATA+112+4*i,f32(getf(m,DATA+112+4*i)+getf(m,DT)))
  if count>=20:
   alpha=load(m,DATA+122+i,1);alpha-=8 if alpha>8 else 0;store(m,DATA+122+i,1,alpha);store(m,OBJECTS+rid*68+63,1,alpha)
  if count>=25 and signed(load(m,DATA+120+(1-i),1),8)<0:store(m,DATA+120+(1-i),1,0);call(SHOW,[load(m,DATA+4+4*(1-i)),0,15])
 setf(m,TARGET+104,f32(getf(m,TARGET+104)+getf(m,DT)));flags=load(m,TARGET+4,1)
 if not flags&2 or flags&4 or getf(m,TARGET+104)>120.:
  node=load(m,COL_POOL+16)
  while node:
   nxt=load(m,node)
   if load(m,node+4)==TARGET:call(FREE,[COL_POOL,node])
   node=nxt
  for i in range(4):
   if load(m,slot+4*i)==TARGET:store(m,slot+4*i,4,0);store(m,slot+16,4,load(m,slot+16)-1);break
  if getf(m,TARGET+104)>120.:call(RADIUS,[TARGET+68,TARGET+68,owner,bits(400.),1600])
  call(BLAST,[TARGET+68,0,bits(.4),1])
  for off in (4,8,0):
   rid=signed(load(m,DATA+off))
   if rid!=-1:call(REMOVE,[rid,0,0]);store(m,DATA+off,4,-1)
  store(m,DATA+134,2,0);call(REMOVE,[3,0,0]);store(m,TARGET+4,1,0);call(FREE,[TARGET_POOL,TARGET]);call(RELEASE,[VISUAL,1])
 return visible(m),public_events(b.events)

ARGC={RELEASE:2,ROAD:3,NEW:4,TEXTURE:3,HIDE:3,SHOW:3,FREE:2,RADIUS:5,BLAST:4,REMOVE:3,COPY:2}
def execute(code,c):
 m=state(c);b=Boundaries(m,c);r=[0xa5000000+13*i for i in range(32)];f=[bits(i*.25) for i in range(32)];fbefore=f[:]
 r[0]=0;r[4]=VISUAL;r[5]=c['op'];r[29]=STACK;r[31]=RETURN;before=r[:]
 pc=ENTRY;pending=None;lo=0;fcc=False;coverage=set();branches=set()
 for step in range(30000):
  if pc==RETURN:break
  require(pc in code,f'escaped instruction {pc:x}')
  coverage.add(pc);w=code[pc];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63;imm=signed(w,16)
  a=(r[rs]+imm)&MASK;old=pending;pending=None;nxt=pc+4
  if op==0:
   if fn==0:r[rd]=r[rt]<<sh
   elif fn==2:r[rd]=r[rt]>>sh
   elif fn==3:r[rd]=signed(r[rt])>>sh
   elif fn==4:r[rd]=r[rt]<<(r[rs]&31)
   elif fn==8:pending=r[rs]
   elif fn==18:r[rd]=lo
   elif fn in (24,25):lo=(r[rs]*r[rt])&MASK
   elif fn in (32,33):r[rd]=r[rs]+r[rt]
   elif fn in (34,35):r[rd]=r[rs]-r[rt]
   elif fn==36:r[rd]=r[rs]&r[rt]
   elif fn==37:r[rd]=r[rs]|r[rt]
   elif fn==38:r[rd]=r[rs]^r[rt]
   elif fn==39:r[rd]=~(r[rs]|r[rt])
   elif fn==42:r[rd]=int(signed(r[rs])<signed(r[rt]))
   elif fn==43:r[rd]=int(r[rs]<r[rt])
   else:raise ValueError(f'unexpected SPECIAL {fn}')
  elif op==1:
   require(rt in (0,1,2,3),'REGIMM kind');take=signed(r[rs])>=0 if rt&1 else signed(r[rs])<0;branches.add((pc,take))
   if take:pending=pc+4+imm*4
   elif rt&2:nxt+=4
  elif op==2:pending=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
  elif op==3:
   dest=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2);r[31]=pc+8;pending=('call',dest,pc+8)
  elif op==9:r[rt]=a
  elif op==10:r[rt]=int(signed(r[rs])<imm)
  elif op==11:r[rt]=int(r[rs]<(imm&MASK))
  elif op==12:r[rt]=r[rs]&(w&65535)
  elif op==13:r[rt]=r[rs]|(w&65535)
  elif op==15:r[rt]=(w&65535)<<16
  elif op in (4,5,6,7,20,21,22,23):
   kind=op&15;take=(r[rs]==r[rt]) if kind==4 else ((r[rs]!=r[rt]) if kind==5 else (signed(r[rs])<=0 if kind==6 else signed(r[rs])>0));branches.add((pc,take))
   if take:pending=pc+4+imm*4
   elif op>=20:nxt+=4
  elif op in (32,33,35,36,37):
   n=4 if op==35 else (2 if op in (33,37) else 1);v=load(m,a,n);r[rt]=signed(v,8*n) if op in (32,33) else v
  elif op in (40,41,43):store(m,a,4 if op==43 else (2 if op==41 else 1),r[rt])
  elif op==49:f[rt]=load(m,a)
  elif op==57:store(m,a,4,f[rt])
  elif op==17:
   if rs==0:r[rt]=f[rd]
   elif rs==4:f[rd]=r[rt]
   elif rs==8:
    take=fcc if rt&1 else not fcc;branches.add((pc,take))
    if take:pending=pc+4+imm*4
    elif rt&2:nxt+=4
   elif rs==16:
    if fn==0:f[sh]=bits(f32(real(f[rd])+real(f[rt])))
    elif fn==1:f[sh]=bits(f32(real(f[rd])-real(f[rt])))
    elif fn==2:f[sh]=bits(f32(real(f[rd])*real(f[rt])))
    elif fn==3:f[sh]=bits(f32(real(f[rd])/real(f[rt])))
    elif fn==6:f[sh]=f[rd]
    elif fn==7:f[sh]=f[rd]^0x80000000
    elif fn in (50,60,62):fcc=real(f[rd])==real(f[rt]) if fn==50 else (real(f[rd])<real(f[rt]) if fn==60 else real(f[rd])<=real(f[rt]))
    else:raise ValueError(f'unexpected COP1 S {fn}')
   else:raise ValueError(f'unexpected COP1 {rs}/{fn}')
  else:raise ValueError(f'unexpected opcode {op}')
  r=[x&MASK for x in r];r[0]=0;require(old is None or pending is None,'nested branch delay slot')
  if isinstance(old,tuple):
   dest=old[1];require(dest in ARGC,f'unknown call {dest:x}');args=r[4:4+min(ARGC[dest],4)]
   if ARGC[dest]>4:args.append(load(m,r[29]+16))
   if dest==COPY:b.events.append((dest,tuple(args)));pc=COPY;continue
   result=b.call(dest,args)
   for j in list(range(1,16))+[24,25]:r[j]=(0xbad00000+31*j)&MASK
   for j in range(20):f[j]=bits(j*.5)
   r[2]=result&MASK;pc=old[2]
  else:pc=old if old is not None else nxt
 else:raise ValueError('execution bound exceeded')
 require(r[16:24]==before[16:24] and r[28:]==before[28:] and f[20:]==fbefore[20:],'saved register/stack ABI')
 return visible(m),public_events(b.events),coverage,branches

def candidate(obj):
 obj=Path(obj);words=score.text_words(obj);start=score.symbols(obj)['func_8010C974'];data,secs=score._elf(obj);ti=score._text_index(secs)
 found=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']=='func_8010C974' and s['type']==2 and s['section']==ti]
 require(len(found)==1,'ambiguous extent');end=start+found[0]['size'];got,masks,u,v,e=score.relocate(obj,words,start,end,score.image_symbols());require(not(masks or u or v or e),'uncertain relocation')
 return {ENTRY+4*i:w for i,w in enumerate(got[start//4:end//4])}

def verify(obj):
 require(score.own_data().read(THRESHOLD,4)==struct.pack('>f',.0333333),'native threshold')
 native={ENTRY+4*i:w for i,w in enumerate(score.targets()['func_8010C974'])};compiled=candidate(obj)
 helper={COPY+4*i:w for i,w in enumerate(score.targets()['math_utility'])};native.update(helper);compiled.update(helper)
 coverage=[set(),set()];branches=[set(),set()];count=0
 for i,c in enumerate(fixtures()):
  expected=model(c)
  for j,code in enumerate((native,compiled)):
   actual=execute(code,c)
   if actual[:2]!=expected:
    diff=[hex(a) for a in actual[0] if actual[0][a]!=expected[0][a]]
    raise ValueError(f'behavior mismatch program {j} case {i}, bytes {diff[:12]}, events {actual[1]==expected[1]}\nactual {actual[1]}\nexpected {expected[1]}')
   coverage[j]|=actual[2];branches[j]|=actual[3]
  count+=1
 return dict(status='PASS_BOUNDED',fixtures=count,executed_words=[len(x) for x in coverage],mine_executed_words=[sum(ENTRY<=a<ENTRY+2636 for a in x) for x in coverage],copyuv_executed_words=[sum(COPY<=a<COPY+76 for a in x) for x in coverage],native_missing=[hex(a-ENTRY) for a in sorted(set(native)-coverage[0])],branches=[len(x) for x in branches],claims=[])
if __name__=='__main__':
 import json
 print(json.dumps(verify(sys.argv[1]),indent=2))
