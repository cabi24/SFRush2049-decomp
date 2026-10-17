"""Bounded native/source controller snapshot verification; research, not a match claim.

Queue and initialization boundaries are deterministic stubs. The real 19-word
normalization helper is executed, including its internal integer/FP convention.
"""
from pathlib import Path
import sys, struct, random, json, itertools
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT/'tools/cloud'));import score
ENTRY,SCALE,INIT,RECV,SEND=0x800E7134,0x800C9590,0x800E7038,0x80007270,0x800075E0
CAL,RAW,NATIVE,MODE,HELD,PREV,OUTHELD,OUTPRESS,STICK,FLAG,QUEUE=0x80149AF8,0x80149B50,0x80156CF0,0x80149B74,0x80149B30,0x80149B10,0x8013FED0,0x801403C0,0x80140620,0x80111954,0x801497A8
STACK,RETURN,MASK=0x81010000,0x81234560,0xffffffff

def require(c,m):
 if not c:raise ValueError(m)
def signed(x,n=32):
 x&=(1<<n)-1;return x-(1<<n) if x>>(n-1) else x
def bits(x):return struct.unpack('>I',struct.pack('>f',x))[0]
def real(x):return struct.unpack('>f',struct.pack('>I',x))[0]
def f32(x):return real(bits(x))
def load(m,a,n):
 require(a%n==0 and all(a+i in m for i in range(n)),f'invalid read {a:x}/{n}')
 return int.from_bytes(bytes(m[a+i] for i in range(n)),'big')
def store(m,a,n,v):
 require(a%n==0 and all(a+i in m for i in range(n)),f'invalid write {a:x}/{n}')
 for i,b in enumerate((v&((1<<(8*n))-1)).to_bytes(n,'big')):m[a+i]=b

def fixtures():
 axes=(-128,-127,-71,-70,-57,-56,0,56,57,70,71,126,127)
 rng=random.Random(0x800E7134)
 for x,y,c,init in itertools.product(axes,axes,(70,126,127),(0,1)):
  yield {'init':init,'pads':[(int((i+init)%3!=0),rng.getrandbits(19),x if i%2==0 else y,y if i%2==0 else x) for i in range(4)],'cal':[(c,70 if i%2 else 127) for i in range(4)],'mode':[i%2 for i in range(4)],'held':[rng.getrandbits(32) for i in range(4)],'prev':[rng.getrandbits(32) for i in range(4)]}

def state(c):
 regions=[(CAL,8),(RAW,8),(NATIVE,64),(MODE,4),(HELD,16),(PREV,16),(OUTHELD,16),(OUTPRESS,16),(STICK,32),(FLAG,1),(STACK-256,512)]
 m={a+i:0xa5 for a,n in regions for i in range(n)};store(m,FLAG,1,c['init'])
 for i,(connected,buttons,x,y) in enumerate(c['pads']):
  store(m,NATIVE+16*i,1,connected);store(m,NATIVE+16*i+4,4,buttons);store(m,NATIVE+16*i+8,1,x);store(m,NATIVE+16*i+9,1,y)
  for j in range(2):store(m,CAL+2*i+j,1,c['cal'][i][j])
  store(m,MODE+i,1,c['mode'][i]);store(m,HELD+4*i,4,c['held'][i]);store(m,PREV+4*i,4,c['prev'][i])
 return m

def visible(m):return {a:v for a,v in m.items() if not STACK-256<=a<STACK+256}
def initialize(m):
 store(m,FLAG,1,1)
 for i in range(4):
  store(m,MODE+i,1,1)
  for j in range(2):store(m,CAL+2*i+j,1,70)

def model(c):
 m=state(c);events=[]
 if not c['init']:initialize(m);events.append('init')
 events.append('lock')
 for i,(connected,buttons,x,y) in enumerate(c['pads']):
  held=buttons if connected else 0
  axes=(x,y) if connected else (0,0)
  for j in range(2):store(m,RAW+2*i+j,1,axes[j])
  if not connected:
   for j in range(2):store(m,CAL+2*i+j,1,70)
  else:
   if load(m,MODE+i,1):
    if x< -56:held|=0x1000
    if x>56:held|=0x2000
    if y< -56:held|=0x800
    if y>56:held|=0x400
   if held&0x40000:
    for j in range(2):store(m,CAL+2*i+j,1,70)
   for j in range(2):
    cal=signed(load(m,CAL+2*i+j,1),8)
    if abs(axes[j])>cal and cal<127:store(m,CAL+2*i+j,1,cal+1)
  press=(c['held'][i]^held)&held
  store(m,OUTHELD+4*i,4,held);store(m,PREV+4*i,4,c['prev'][i]|press);store(m,HELD+4*i,4,held);store(m,OUTPRESS+4*i,4,press)
  for j in range(2):store(m,STICK+8*i+4*j,4,bits(max(-1.,min(1.,f32(axes[j]/signed(load(m,CAL+2*i+j,1),8))))))
 events.append('unlock');return visible(m),events

def execute(code,c):
 m=state(c);r=[(0xa5000000+i*13)&MASK for i in range(32)];f=[bits(i*.25) for i in range(32)]
 r[0]=0;r[29]=STACK;r[31]=RETURN;before=r[:];fbefore=f[:];pc=ENTRY;pending=None;fcc=False;events=[];cov=set();branches=set()
 for step in range(5000):
  if pc==RETURN:break
  require(pc in code,f'escaped instruction {pc:x}');cov.add(pc);w=code[pc];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63;imm=signed(w,16);a=(r[rs]+imm)&MASK;old=pending;pending=None;nxt=pc+4
  if op==0:
   if fn==0:r[rd]=r[rt]<<sh
   elif fn==8:pending=r[rs]
   elif fn==33:r[rd]=r[rs]+r[rt]
   elif fn==35:r[rd]=r[rs]-r[rt]
   elif fn==36:r[rd]=r[rs]&r[rt]
   elif fn==37:r[rd]=r[rs]|r[rt]
   elif fn==38:r[rd]=r[rs]^r[rt]
   elif fn==42:r[rd]=int(signed(r[rs])<signed(r[rt]))
   else:raise ValueError(f'unsupported SPECIAL {fn}')
  elif op==3:
   dest=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2);r[31]=pc+8;pending=dest if dest in code else ('call',dest,pc+8)
  elif op==9:r[rt]=a
  elif op==10:r[rt]=int(signed(r[rs])<imm)
  elif op==13:r[rt]=r[rs]|(w&65535)
  elif op==15:r[rt]=(w&65535)<<16
  elif op in (4,5,20,21):
   take=(r[rs]==r[rt]) if op in (4,20) else (r[rs]!=r[rt]);branches.add((pc,take))
   if take:pending=pc+4+imm*4
   elif op>=20:nxt+=4
  elif op in (32,35,36):
   n=4 if op==35 else 1;v=load(m,a,n);r[rt]=signed(v,8) if op==32 else v
  elif op in (40,43):store(m,a,1 if op==40 else 4,r[rt])
  elif op==53:f[rt]=load(m,a,4);f[rt+1]=load(m,a+4,4)
  elif op in (57,61):
   store(m,a,4,f[rt])
   if op==61:store(m,a+4,4,f[rt+1])
  elif op==17:
   if rs==4:f[rd]=r[rt]
   elif rs==20 and fn==32:f[sh]=bits(float(signed(f[rd])))
   elif rs==8:
    take=fcc if rt&1 else not fcc;branches.add((pc,take))
    if take:pending=pc+4+imm*4
    elif rt&2:nxt+=4
   elif rs==16:
    if fn==2:f[sh]=bits(f32(real(f[rd])*real(f[rt])))
    elif fn==3:f[sh]=bits(f32(real(f[rd])/real(f[rt])))
    elif fn==6:f[sh]=f[rd]
    elif fn==7:f[sh]=f[rd]^0x80000000
    elif fn==60:fcc=real(f[rd])<real(f[rt])
    else:raise ValueError(f'unsupported COP1 {fn}')
   else:raise ValueError(f'unsupported COP1 format {rs}')
  else:raise ValueError(f'unsupported op {op}')
  r=[v&MASK for v in r];r[0]=0;require(old is None or pending is None,'nested branch in delay slot')
  if isinstance(old,tuple):
   dest=old[1]
   if dest==INIT:initialize(m);events.append('init')
   else:
    require(dest in (RECV,SEND) and r[4]==QUEUE and r[5]==0 and r[6]==int(dest==RECV),'queue protocol')
    events.append('lock' if dest==RECV else 'unlock')
   for k in list(range(1,16))+[24,25]:r[k]=(0xbad00000+k*31)&MASK
   for k in range(20):f[k]=bits(float(k))
   pc=old[2]
  else:pc=old if old is not None else nxt
 else:raise ValueError('execution bound')
 require(r[16:24]==before[16:24] and r[28:]==before[28:] and f[20:]==fbefore[20:],'saved-register ABI')
 return visible(m),events,cov,branches

def candidate(obj):
 obj=Path(obj);words=score.text_words(obj);symbols=score.symbols(obj);code={}
 for name,address in [('func_800E7134',ENTRY),('func_800C9590',SCALE)]:
  start=symbols[name];data,secs=score._elf(obj);ti=score._text_index(secs);found=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']==name and s['type']==2 and s['section']==ti]
  size=found[0]['size'];resolved,masks,unresolved,unverified,errors=score.relocate(obj,words,start,start+size,score.image_symbols());require(not(masks or unresolved or unverified or errors),'uncertain candidate relocation')
  code.update({address+4*i:w for i,w in enumerate(resolved[start//4:(start+size)//4])})
 return code

def verify(obj):
 code={address+4*i:w for name,address in [('func_800E7134',ENTRY),('func_800C9590',SCALE)] for i,w in enumerate(score.targets()[name])};other=candidate(obj);cov=[set(),set()];branches=[set(),set()];count=0
 for case in fixtures():
  expected=model(case)
  for j,program in enumerate((code,other)):
   actual=execute(program,case);require(actual[:2]==expected,f'behavior mismatch program {j} case {count}');cov[j]|=actual[2];branches[j]|=actual[3]
  count+=1
 return {'status':'PASS','fixtures':count,'executions':2*count,'native_words_executed':len(cov[0]),'candidate_words_executed':len(cov[1]),'native_missing_offsets':[hex(a-ENTRY) for a in sorted(set(code)-cov[0])],'branch_outcomes':[len(b) for b in branches],'model':'stable pad snapshots; deterministic init and message-queue stubs; real normalization helper executed','matching_claim':False}
