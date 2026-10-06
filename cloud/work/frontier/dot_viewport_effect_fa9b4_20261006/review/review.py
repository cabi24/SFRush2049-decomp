#!/usr/bin/env python3
"""Independent fixtures and callback schedules against frozen caller.

Uses producer's fail-closed MIPS interpreter, not an independent CPU oracle.
Compiles host C only; the target object is reused without target compilation.
"""
import ctypes as C, hashlib, importlib.util, json, os, random, struct, sys, tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent; ROOT=HERE.parents[4]
WORK=Path(os.environ.get('RUSH_VIEWPORT_REVIEW_WORK',Path(tempfile.gettempdir())/'viewport-source-review')).resolve()
WORK.mkdir(parents=True,exist_ok=True)
OUTPUT=Path(os.environ.get('RUSH_REVIEW_OUTPUT',WORK)).resolve()
OUTPUT.mkdir(parents=True,exist_ok=True)
TOOL=Path(os.environ.get('RUSH_TOOL_ROOT',str(ROOT))).resolve()
PACKET=Path(os.environ.get('RUSH_PACKET_ROOT',str(HERE.parent))).resolve()
REFERENCE=Path(os.environ.get('RUSH_REFERENCE_ROOT',str(TOOL))).resolve()
spec=importlib.util.spec_from_file_location('producer',PACKET/'verify_semantics.py');v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)
score,words=v.load(REFERENCE,TOOL)
obj=Path(os.environ.get('RUSH_VIEWPORT_OBJECT',str(WORK/'candidate.o'))).resolve();raw=score.text_words(obj);compiled,masks,unresolved,unverified,errors=score.relocate(obj,raw,0,len(raw)*4,score.image_symbols())
assert not any((masks,unresolved,unverified,errors))
FROZEN='e843f3500c6a952dd067949a7d5f894febfb8d635d77baa480778360dc95f4ee'
assert hashlib.sha256((PACKET/'candidate.c').read_bytes()).hexdigest()==FROZEN
REGIONS=[(v.P,'D_80152818',4*952),(v.M,'D_8014A250',4*2056),(v.S,'D_8014A118',4*76),(v.E,'D_80150B70',4*152),
 (v.R,'D_80152698',16),(v.FLAGS,'D_801392D8',16),(v.STATUS,'D_80153E88',32),
 (v.MODE,'D_8014A110',4),(v.COUNT,'D_80152744',1),(v.SOUND,'D_8010FFC0',1),(v.FINAL,'D_80114650',1),
 (v.HUMAN,'D_8014A108',2),(v.OPTIONS,'D_801174B4',4)]
class Host:
 def __init__(self,path):
  self.lib=C.CDLL(str(path));self.lib.run.argtypes=[C.POINTER(C.c_int),C.POINTER(C.c_uint32)];self.lib.run.restype=C.c_int
  assert self.lib.layouts()==1
  self.globals={a:(C.c_ubyte*n).in_dll(self.lib,name) for a,name,n in REGIONS if a!=v.R}
  self.lib.review_ref.argtypes=[C.c_int,C.c_int];self.lib.review_header.argtypes=[C.c_int,C.c_int]
  self.lib.review_event.argtypes=[C.POINTER(C.c_uint32)]
  self.callback=C.CFUNCTYPE(None,C.c_int)(self.hook);self.lib.register_hook.argtypes=[C.CFUNCTYPE(None,C.c_int)];self.lib.register_hook(self.callback)
 def snapshot(self):
  data=[]
  for a,name,n in REGIONS:
   if a==v.R:b=bytearray().join((0x500000+256*i if self.lib.review_ref_state(i) else 0).to_bytes(4,'big') for i in range(4))
   else:
    b=bytearray(self.globals[a]);swaps=[]
    if a==v.M:swaps=[(i*2056+0x7c6,2) for i in range(4)]+[(i*2056+0x22c+j*4,4) for i in range(4) for j in range(3)]
    elif a==v.S:swaps=[(i*76+64,2) for i in range(4)]
    elif a==v.E:swaps=[(i*152+k+j*4,4) for i in range(4) for k in (36,132) for j in range(3)]
    elif a in (v.FLAGS,v.MODE,v.OPTIONS):swaps=[(j,4) for j in range(0,n,4)]
    elif a==v.HUMAN:swaps=[(0,2)]
    if sys.byteorder=='little':
     for k,w in swaps:b[k:k+w]=b[k:k+w][::-1]
   data.append(bytes(b))
  b=bytearray(1024)
  for i in range(4):
   x=0x500000+i*256
   for off,value in ((0,x+16),(56,x+80),(80,x+96)):
    b[i*256+off:i*256+off+4]=value.to_bytes(4,'big')
   b[i*256+101]=self.lib.review_header_state(i)&255
  data.append(bytes(b));return tuple(data)
 def write(self,a,w,value):
  if v.R<=a<v.R+16:
   i=(a-v.R)//4;self.lib.review_ref(i,int(value!=0));return
  if 0x500000<=a<0x500400:
   self.lib.review_header((a-0x500000)//256,value);return
  for base,name,n in REGIONS:
   if base<=a and a+w<=base+n:
    arr=self.globals[base];payload=(value&((1<<(8*w))-1)).to_bytes(w,sys.byteorder)
    arr[a-base:a-base+w]=payload;return
  raise AssertionError(('unmapped host write',hex(a)))
 def hook(self,ordinal):
  try:
   e=(C.c_uint32*11)();self.lib.review_event(e)
   self.trace.append((tuple(e),self.snapshot()))
   for a,w,value in self.schedule.get(ordinal,[]):self.write(a,w,value)
  except BaseException as exc:self.exc=exc
 def run(self,c,schedule):
  self.trace=[];self.schedule=schedule;self.exc=None
  out=(C.c_uint32*2048)();n=self.lib.run((C.c_int*128)(*c),out)
  if self.exc:raise self.exc
  self.trace.append((None,self.snapshot()))
  return self.trace,list(out)[:n]
class Scheduled(v.Machine):
 def __init__(self,words,c,schedule):super().__init__(words,c);self.schedule=schedule;self.trace=[]
 def snapshot(self):return tuple(bytes(d) for a,d in self.m.regions[:-1])
 def hook(self,target,r,f):
  super().hook(target,r,f)
  self.trace.append((tuple(self.events[-1]),self.snapshot()))
  for a,w,value in self.schedule.get(len(self.events),[]):self.m.put(a,value,w)
  # An ordinary callee may home all four register arguments, and all six
  # stack arguments of the ten-argument positional-audio boundary.
  for off in range(0,40 if v.CALLS[target]==13 else 16,4):self.m.put(r[29]+off,0xcaff0000+off)
 def run(self):
  result=super().run();self.trace.append((None,self.snapshot()));return self.trace,result

def simple(mode=4,count=1,transition=-1,slot=0,model_mode=2):
 c=[0]*128;c[:6]=[mode,count,4,0,1,0];c[7]=0;c[8]=0
 for i in range(4):
  b=32+i*16;c[b:b+14]=[transition,-1,-17,112,slot,model_mode,65535,0xffffffff,0,1,-1,0x3f800000,0xc0200000,0x80000000]
 return [v.signed(x) for x in c]

def write_choices(rng):
 i=rng.randrange(4)
 choices=[(v.MODE,4,rng.choice([-2147483648,-1,0,1,2,3,4,5,6,7,2147483647])),
 (v.COUNT,1,rng.choice([-128,-1,0,1,2,3,4])),(v.HUMAN,2,rng.choice([-32768,-1,0,1,2,3,4,32767])),
 (v.OPTIONS,4,rng.getrandbits(32)),(v.SOUND,1,rng.choice([-128,-1,0,1,127])),(v.FINAL,1,rng.choice([-128,-1,0,1,127])),
 (v.P+i*952+0xed,1,rng.choice([-128,-1,0,1,127])),(v.P+i*952+0x35c,1,rng.choice([-128,-1,0,1,2,3])),
 (v.P+i*952+0x35d,1,rng.randrange(256)),(v.P+i*952+0x35e,1,rng.randrange(256)),
 (v.M+i*2056+0x7c6,2,rng.randrange(4)),(v.M+i*2056+0x7cc,1,rng.choice([-128,-1,0,1,2,127])),
 (v.S+i*76+64,2,rng.choice([0,1,32767,32768,65534,65535])),(v.FLAGS+i*4,4,rng.getrandbits(32)),
 (v.STATUS+i*8+7,1,rng.choice([0,1,5,6,127,128,255])),(v.R+i*4,4,0x500000+i*256 if rng.randrange(2) else 0),
 (0x500000+i*256+101,1,rng.choice([-128,-1,0,1,127])),
 (v.E+i*152+36+rng.randrange(3)*4,4,rng.choice([0,0x80000000,0x00800000,0x7f7fffff,0xc0200000]))]
 return rng.choice(choices)

def cases():
 # Exact thresholds, all no-player entry modes, object sign/null cases, unsigned-half wrap.
 for mode in [-2147483648,-1,0,1,2,3,4,5,6,7,2147483647]:
  for count in [-128,-1,0,1,4]:yield f'entry-{mode}-{count}',simple(mode,count),{}
 for transition in [-128,-1,0,1,127]:
  for effect in [-128,-1,0,3]:
   for count in [0,65534,65535]:
    c=simple(4,1,transition);c[33]=effect;c[38]=count;yield 'transition-effect-wrap',c,{}
 for model_mode in [-128,-1,0,1,2,127]:
  for mode in [2,4]:
   for sound in [-128,0,1]:
    for ref in [0,1]:
     for flag in [-128,-1,0,127]:
      c=simple(mode,1,-1,0,model_mode);c[4]=sound;c[41]=ref;c[42]=flag;yield 'sound-contract',c,{}
 # Exhaust all relevant hooks in a negative-event invocation with mutation of captured/reloaded slot,
 # live counts, status dispatch and mode. Multiple changes per invocation are intentional.
 for ordinal in range(1,30):
  for mode in [0,2,4,5,6]:
   c=simple(mode,4,-1,0);c[33]=1
   schedule={ordinal:[(v.M+0x7c6,2,3),(v.MODE,4,4),(v.STATUS+7,1,6),(v.STATUS+3*8+7,1,255)],
    ordinal+1:[(v.COUNT,1,1),(v.M+0x7cc,1,0),(v.SOUND,1,1)],ordinal+2:[(v.MODE,4,2),(v.FINAL,1,-1)]}
   yield f'multicall-slot-{ordinal}-{mode}',c,schedule
 rng=random.Random(0xA11FA9B4)
 for i in range(2000):
  c=v.fixture(rng);c[7]=0;c[8]=0;c[2]=rng.choice([-32768,-1,0,1,3,4,32767])
  schedule={k:[write_choices(rng) for _ in range(rng.randrange(1,6))] for k in range(1,36) if rng.randrange(3)==0}
  yield f'independent-random-{i}',c,schedule

def first_difference(a,b):
 if len(a)!=len(b):return ('trace-length',len(a),len(b))
 for i,(x,y) in enumerate(zip(a,b)):
  if x[0]!=y[0]:return ('event',i,x[0],y[0])
  if x[1]!=y[1]:
   for j,(p,q) in enumerate(zip(x[1],y[1])):
    if p!=q:return ('state',i,j,[(k,p[k],q[k]) for k in range(len(p)) if p[k]!=q[k]][:16])
 return None

def main():
 host=Host(Path(os.environ.get('REVIEW_HOST_LIBRARY',str(WORK/'review.so'))));ids=set();coverage=set();branches=set();multi=0;n=0;actions=0
 for label,c,schedule in cases():
  native=Scheduled(words,c,schedule);ntrace,nout=native.run()
  cand=Scheduled(compiled,c,schedule);ctrace,cout=cand.run()
  htrace,hout=host.run(c,schedule)
  assert ntrace==htrace,(label,'host',first_difference(ntrace,htrace))
  assert ntrace==ctrace,(label,'object',first_difference(ntrace,ctrace))
  assert nout==hout==cout,(label,'output')
  n+=1;multi+=int(len(schedule)>1);actions+=sum(len(schedule.get(i,[])) for i in range(1,len(native.events)+1))
  ids.update(e[0] for e in native.events);coverage.update(native.coverage);branches.update(native.branch_edges)
 result={'status':'PASS bounded independent fixtures and repeated-callback mutation review','cases':n,'multi_boundary_schedules':multi,
 'applied_mutations':actions,'outgoing_argument_home_poison':True,'native_executions':n*2,'external_calls_covered':sorted(ids),'native_words_covered':len(coverage),'branch_outcomes':len(branches),
 'source_sha256':FROZEN,'producer_verifier_sha256':hashlib.sha256((PACKET/'verify_semantics.py').read_bytes()).hexdigest(),'object_sha256':hashlib.sha256(obj.read_bytes()).hexdigest(),
 'observations':'Ordered call arguments and complete nonstack data regions checked at every external boundary and after return, against literal source host and reused O3 object.',
 'limitations':['Reuses producer MIPS interpreter; independent fixtures/schedules and host inspection, not an independent CPU implementation.',
 'Synthetic ordinary-ABI boundaries; helper bodies not executed; nonnegative valid slots 0..3, B residency assumed.',
 'No native target compilation, source shaping, production changes, publication, or CI watching.']}
 (OUTPUT/'review.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
