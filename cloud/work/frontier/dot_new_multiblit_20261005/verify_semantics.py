#!/usr/bin/env python3
"""Independent normalized oracle, actual host C, native and GNU-linked execution."""
import ctypes
from pathlib import Path
import random
import subprocess
import sys
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
FN='sound_control'
MAX=8
MASK=0xffffffff
OBJ,REC,STACK,STOP=0x20000,0x10000,0x60000,0xfffffff0
YES,NO=0x6000,0x6010
FIELDS=[(0,4,0),(4,4,0),(8,4,0),(12,2,0),(14,2,1),(16,2,1),(18,2,0),
 (20,2,1),(22,2,1),(24,1,0),(25,1,0),(26,1,1),(27,1,1),(28,2,1),(30,2,1),
 (32,2,1),(34,2,1),(36,2,1),(40,4,0),(44,4,0),(48,4,1),(52,2,0),(56,4,0),(60,4,0)]
def signed(x,bits=32):
 x&=(1<<bits)-1
 return x-(1<<bits) if x&(1<<(bits-1)) else x

def hsh(values):
 h=2166136261
 for x in values:h=((h^(x&MASK))*16777619)&MASK
 return h

def cases():
 rows=[]
 for i in range(MAX):rows += [1,10+i,-20-i,30+i,40+i,-1,-2,-3,-4,0xfedcba98,0xabcdef01,0x87654321,1,0]
 # Zero/negative/narrowing guards, every failure location, sentinel alternatives.
 for n in [-32768,-1,0,1,2,7,8,32768,65535,65536,65537]:
  for mutate in [0,1]:yield [n,-32768,32767,mutate]+rows
 for n in range(1,MAX+1):
  for fail in range(-1,n):
   for sentinel in [False,True]:
    for mutate in [0,1]:
     r=rows.copy()
     for i in range(MAX):
      r[i*14]=(-1 if sentinel else 1)
      r[i*14+12]=2 if fail==i else 1
     yield [n,32767,-32768,mutate]+r
 rng=random.Random(0xb37e8)
 edges=[-32768,-32767,-1,0,1,32766,32767]
 for k in range(3000):
  r=[]
  for i in range(MAX):
   r.extend([rng.choice([-1,0,1])]+[rng.choice(edges) if k<500 else rng.randint(-32768,32767) for _ in range(8)]+
       [signed(rng.getrandbits(32)) for _ in range(3)]+[rng.randrange(3),0])
  yield [rng.randrange(MAX+1),rng.choice(edges),rng.choice(edges),rng.randrange(2)]+r

def oracle(c):
 n,x,y,mut=c[:4];n=signed(n,16);x=signed(x,16);y=signed(y,16)
 records=[list(c[4+i*14:4+(i+1)*14]) for i in range(MAX)]
 objects=[];events=[];root=0;last=None
 def event(kind,i,args=(0,0,0,0),result=0):events.append([kind,i,*[v&MASK for v in args],hsh(objects[i]),result&MASK])
 for i in range(max(0,n)):
  d=records[i];name=MASK if d[0]==-1 else (0 if d[0]==0 else 0x100+i)
  cx=x+signed(d[1],16);cy=y+signed(d[2],16)
  flags=d[11]&0x80000000
  b=[name,0x300+i,0x400+i,100+i,signed(cx,16),signed(cy,16),200+i,300+i,400+i,17+i,1,-1,1,-2,-3,-4,-5,-6,0,MASK,-1,500+i,0x500+i,0]
  objects.append(b);event(1,i,(name,cx,cy,flags))
  if mut:d[3]=123+i;d[4]=234+i;d[10]^=0x5a
  b[9]=d[10]&255;b[6]=d[9]&65535
  b[15]=signed(d[7],16);b[13]=signed(d[5],16);b[16]=signed(d[8],16);b[14]=signed(d[6],16)
  if signed(d[3],16)>=0:b[7]=signed(d[3],16)
  if signed(d[4],16)>=0:b[8]=signed(d[4],16)
  event(2,i)
  if mut:d[11]^=0x40000000
  if d[12]:
   b[19]=d[11]&0x7fffffff
   if d[0]==-1:
    b[2]=0x600 if d[12]==1 else 0x601;b[1]=0x200+i
    event(2,i)
    if mut:d[11]^=0x40000000
   else:
    b[18]=0x600 if d[12]==1 else 0x601
    result=(-7 if mut else 1) if d[12]==1 else 0;event(3,i,result=result)
    b[20]=700+i;b[9]^=0x33
    if mut and i+1<MAX:records[i+1][1]=-1234
    if not result:
     event(4,i);b[12]=0;break
  if i==0:root=0x200+i
  else:objects[last][23]=0x200+i
  last=i
 return [x&MASK for x in [root,len(objects),len(events)]+[v for b in objects for v in b]+[v for e in events for v in e]]

class Native:
 def __init__(self,words=None):
  self.symbols=score.image_symbols();self.start=self.symbols[FN]
  self.words=score.targets()[FN] if words is None else words
  self.code={self.start+4*i:w for i,w in enumerate(self.words)}
  self.coverage=set()
 def run(self,c):
  n,x,y,mut=c[:4];regions={OBJ:bytearray(MAX*64),REC:bytearray(MAX*36),STACK:bytearray(512)}
  def mem(a,size,v=None):
   assert a%size==0,('unaligned',hex(a),size)
   for base,data in regions.items():
    off=a-base
    if 0<=off and off+size<=len(data):
     if v is None:return int.from_bytes(data[off:off+size],'big')
     data[off:off+size]=(v&((1<<(size*8))-1)).to_bytes(size,'big');return
   raise AssertionError(('unmapped',hex(a),size))
  for i in range(MAX):
   d=c[4+i*14:4+(i+1)*14];p=REC+36*i
   mem(p,4,MASK if d[0]==-1 else (0 if d[0]==0 else 0x100+i))
   for j in range(8):mem(p+4+2*j,2,d[1+j])
   mem(p+20,4,d[9]);mem(p+24,4,d[10]);mem(p+28,4,0 if d[12]==0 else (YES if d[12]==1 else NO));mem(p+32,4,d[11])
  def ptr(v):
   if OBJ<=v<OBJ+MAX*64 and (v-OBJ)%64==0:return 0x200+(v-OBJ)//64
   return 0x600 if v==YES else (0x601 if v==NO else v)
  def snapshot(i):
   p=OBJ+64*i;out=[]
   for off,size,sign in FIELDS:
    v=mem(p+off,size);v=signed(v,size*8) if sign else v
    if off in (4,8,40,60):v=ptr(v)
    out.append(v&MASK)
   return out
  events=[];used=0
  def event(kind,i,args=(0,0,0,0),result=0):events.append([kind,i,*[v&MASK for v in args],hsh(snapshot(i)),result&MASK])
  r=[0xcab00000+i for i in range(32)];r[0]=0;r[4:8]=[x&MASK,y&MASK,REC,n&MASK];r[29]=STACK+256;r[31]=STOP
  preserved={i:r[i] for i in [*range(16,24),28,30]};pc=self.start;pending=None;steps=0
  funcs=self.symbols
  while pc!=STOP:
   if pc in (funcs['func_800B3704'],funcs['Input_ApplyPadConfig'],funcs['sound_stop'],YES,NO):
    assert pending is None
    value=0x12345678
    if pc==funcs['func_800B3704']:
     i=used;used+=1;assert i<MAX;p=OBJ+64*i;d=REC+36*i
     values=[r[4],0x300+i,0x400+i,100+i,signed(r[5],16),signed(r[6],16),200+i,300+i,400+i,17+i,1,-1,1,-2,-3,-4,-5,-6,0,MASK,-1,500+i,0x500+i,0]
     regions[OBJ][64*i:64*(i+1)]=bytes([0xa5])*64
     for (off,size,_),v in zip(FIELDS,values):mem(p+off,size,v)
     event(1,i,(r[4],r[5],r[6],r[7]));value=p
     if mut:mem(d+8,2,123+i);mem(d+10,2,234+i);mem(d+24,4,mem(d+24,4)^0x5a)
    else:
     p=r[4];assert OBJ<=p<OBJ+used*64 and (p-OBJ)%64==0;i=(p-OBJ)//64;d=REC+36*i
     if pc==funcs['Input_ApplyPadConfig']:
      event(2,i)
      if mut:mem(d+32,4,mem(d+32,4)^0x40000000)
     elif pc==funcs['sound_stop']:event(4,i);mem(p+27,1,0)
     else:
      value=(-7 if mut else 1) if pc==YES else 0;event(3,i,result=value);mem(p+48,4,700+i);mem(p+24,1,mem(p+24,1)^0x33)
      if mut and i+1<MAX:mem(d+36+4,2,-1234)
    ret=r[31]
    for k in [*range(1,16),24,25]:r[k]=0xc10b0000+k
    r[2]=value&MASK;pc=ret;continue
   assert pc in self.code and steps<3000,(hex(pc),steps)
   self.coverage.add(pc-self.start);w=self.code[pc];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63;imm=w&65535;si=signed(imm,16)
   old,pending=pending,None;nextpc=pc+4;a=(r[rs]+si)&MASK
   if w==0:pass
   elif op==0:
    if fn==0:r[rd]=r[rt]<<sh
    elif fn==3:r[rd]=signed(r[rt])>>sh
    elif fn==8:pending=r[rs]
    elif fn==9:pending=r[rs];r[rd]=pc+8
    elif fn==33:r[rd]=r[rs]+r[rt]
    elif fn==36:r[rd]=r[rs]&r[rt]
    elif fn==37:r[rd]=r[rs]|r[rt]
    elif fn==42:r[rd]=int(signed(r[rs])<signed(r[rt]))
    else:raise AssertionError(('special',fn))
   elif op==1:
    assert rt in (0,1,2,3)
    taken=(signed(r[rs])<0) if rt in (0,2) else (signed(r[rs])>=0)
    if taken:pending=pc+4+si*4
    elif rt in (2,3):nextpc+=4
   elif op in (2,3):
    if op==3:r[31]=pc+8
    pending=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
   elif op in (4,5,20,21):
    taken=(r[rs]==r[rt]) if op in (4,20) else (r[rs]!=r[rt])
    if taken:pending=pc+4+si*4
    elif op in (20,21):nextpc+=4
   elif op in (6,7):
    if (signed(r[rs])<=0 if op==6 else signed(r[rs])>0):pending=pc+4+si*4
   elif op==9:r[rt]=a
   elif op==13:r[rt]=r[rs]|imm
   elif op==15:r[rt]=imm<<16
   elif op in (32,33,35,36,37):
    size={32:1,33:2,35:4,36:1,37:2}[op];v=mem(a,size);r[rt]=signed(v,size*8) if op in (32,33) else v
   elif op in (40,41,43):
    size={40:1,41:2,43:4}[op]
    assert STACK<=a<STACK+512 or OBJ<=a<OBJ+used*64
    mem(a,size,r[rt])
   else:raise AssertionError(('unknown',op,hex(pc)))
   r=[v&MASK for v in r];r[0]=0;pc=old if old is not None else nextpc;steps+=1
  assert r[29]==STACK+256 and all(r[k]==v for k,v in preserved.items())
  for i in range(used):
   assert mem(OBJ+i*64+38,2)==0xa5a5 and mem(OBJ+i*64+54,2)==0xa5a5
  return [ptr(r[2]),used,len(events)]+[v for i in range(used) for v in snapshot(i)]+[v for e in events for v in e]

def verify(directory,linked_words):
 so=directory/'host.so'
 subprocess.run(['cc','-std=c99','-O1','-g','-fsanitize=undefined','-fno-sanitize-recover=all','-Wall','-Wextra','-Werror','-fPIC','-shared',str(HERE/'semantic_test.c'),'-o',str(so)],check=True)
 host=ctypes.CDLL(str(so)).run_case;host.argtypes=[ctypes.POINTER(ctypes.c_int32),ctypes.POINTER(ctypes.c_uint32)];host.restype=ctypes.c_int
 native,linked=Native(),Native(linked_words);count=0
 for c in cases():
  expected=oracle(c);a=native.run(c);b=linked.run(c)
  inputs=(ctypes.c_int32*len(c))(*c);out=(ctypes.c_uint32*1024)();size=host(inputs,out);h=list(out[:size])
  assert a==expected,(count,'native',c,a,expected)
  assert b==expected,(count,'linked')
  assert h==expected,(count,'host',c,h,expected)
  count+=1
 assert set(range(0,468,4))-native.coverage=={344}
 # Mutation controls are compiled from actual source, never used as match candidates.
 source=(ROOT/'cloud/matches/sound_control.c').read_text()
 mutations={
  'missing_highbit_mask':('mblit[i].animid & 0x7fffffffU','mblit[i].animid'),
  'wrong_width_guard':('mblit[i].width>=0','mblit[i].width>0'),
  'wrong_crop_field':('curblit->Left=mblit[i].left','curblit->Left=mblit[i].right'),
  'wrong_return_root':(' return rootblit;\n}', ' return lastblit;\n}'),
  'omitted_failure_cleanup':('sound_stop(curblit);',';'),
  'wrong_callback_result':('if(!curblit->AnimFunc(curblit))','if(curblit->AnimFunc(curblit))'),
 }
 rejected={}
 # Include the candidate by absolute path from copied harness, so each mutated test
 # still compiles a complete genuine function against the same external contracts.
 harness=(HERE/'semantic_test.c').read_text()
 corpus=list(cases())
 for label,(old,new) in mutations.items():
  assert source.count(old)==1
  p=directory/(label+'.c');p.write_text(source.replace(old,new))
  h=directory/(label+'_host.c');h.write_text(harness.replace('"../../../matches/sound_control.c"','"'+str(p)+'"'))
  lib=directory/(label+'.so')
  subprocess.run(['cc','-std=c99','-O1','-fsanitize=undefined','-fno-sanitize-recover=all','-fPIC','-shared',str(h),'-o',str(lib)],check=True)
  fn=ctypes.CDLL(str(lib)).run_case;fn.argtypes=host.argtypes;fn.restype=host.restype
  for i,c in enumerate(corpus):
   inp=(ctypes.c_int32*len(c))(*c);out=(ctypes.c_uint32*1024)();length=fn(inp,out)
   if list(out[:length])!=oracle(c):rejected[label]=i;break
  assert label in rejected,label
 return {'cases':count,'native_runs':count*2,'host_ubsan':True,'covered_native_instructions':len(native.coverage),'total_native_instructions':117,'unreachable_instruction_offsets':[344],'wrong_contract_rejections':rejected,
         'domain':'signed-half coordinates and count guards; valid 0..8 descriptor lists; symbolic callbacks and constructor; no gameplay claim'}
