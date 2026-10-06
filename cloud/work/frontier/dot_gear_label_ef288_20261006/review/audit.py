"""Independent mutation fixture for the frozen, unchanged C candidate.

Uses the reviewed fail-closed MIPS engine with independent state and call hooks.
Never compiles target C or embeds native words. Native input is loaded through
an unchanged canonical scorer supplied by --source-root.
"""
import argparse,ctypes,hashlib,json,os,pathlib,random,sys,tempfile
from native_reviewed import *
P=pathlib.Path(__file__).resolve().parent
WORK=pathlib.Path(os.environ.get('RUSH_GEAR_REVIEW_WORK',pathlib.Path(tempfile.gettempdir())/'gear-source-review')).resolve()
WORK.mkdir(parents=True,exist_ok=True)
OUTPUT=pathlib.Path(os.environ.get('RUSH_REVIEW_OUTPUT',WORK)).resolve()
OUTPUT.mkdir(parents=True,exist_ok=True)
LABELS1=0x301000
LABEL2=0x302000
class AuditMachine(Machine):
 def __init__(self,words,args):
  super().__init__(words,[4,1,0,0,0,0,0,0,0,0,0,0,0,0,0]);self.a=args
  for addr,n in ((LABELS1,820),(LABEL2,16)):
   self.mem.update({addr+i:0 for i in range(n)})
  self.put(LABELS1+816,4,LABEL2)
  self.put(COUNT,2,args[0]);self.put(ENABLE,1,args[1])
  for i in range(4):
   self.put(MODELS+i*0x808+10,1,args[15] if args[2]>>i&1 else 0)
   self.put(MODELS+i*0x808+0x730,1,args[11+i])
   self.put(MODELS+i*0x808+0x7C6,2,3-i)
   self.put(CARS+(3-i)*0x3B8+0xEF,1,args[16] if args[3]>>i&1 else 0)
   for j in range(4):
    self.put(POSITIONS+i*32+j*8,4,args[4]+i*43+j*17)
    self.put(POSITIONS+i*32+j*8+6,2,args[5]+i*23+j*11)
 def mutate(self):
  a=self.a
  if len(self.events)!=a[8]:return
  t=a[9];v=a[10];i=a[17]
  if t==1:self.put(COUNT,2,v)
  elif t==2:self.put(LABELS+816,4,LABEL1)
  elif t==3:self.put(ASSET+4,4,LABELS1)
  elif t==4:self.put(MODELS+i*0x808+0x730,1,v)
  elif t==5:
   for i in range(4):
    for j in range(4):
     self.put(POSITIONS+i*32+j*8,4,a[18]+43*i+17*j)
     self.put(POSITIONS+i*32+j*8+6,2,a[19]+23*i+11*j)
  elif t==6:self.put(MODELS+i*0x808+10,1,v)
  elif t==7:self.put(CARS+(3-i)*0x3B8+0xEF,1,v)
  elif t==8:self.put(ENABLE,1,v)
 def hook(self):
  p=self.pc;r=self.r;a=self.a;v=-0x1234
  if p==RENDER:
   assert self.f[12] in (0,0xBF800000),'float argument'
   self.events.append([1,0 if self.f[12]==0 else -1,0,0])
  elif p==RECV:
   assert r[4:7]==[QUEUE,0,1],'lock arguments'
   self.events.append([2,1,0,0])
  elif p==SLOT:self.events.append([3,signed(r[18]),0,0])
  elif p==JAM:
   assert r[4:7]==[QUEUE,0,0],'unlock arguments'
   self.events.append([4,0,0,0])
  elif p==COLOR:self.events.append([5,signed(r[4]),0,0])
  elif p==WIDTH:
   assert r[4] in (LABEL0,LABEL1,LABEL2) and signed(r[5])==-1,'width arguments'
   v=a[6+(self.width_count&1)];self.width_count+=1
   self.events.append([6,{LABEL0:1000,LABEL1:1001,LABEL2:1002}[r[4]],-1,signed(v)])
  elif p==DRAW:
   # The ABI conveys sign-extended halfwords, not merely matching low bits.
   assert r[4]==(signed(r[4],16)&MASK) and r[5]==(signed(r[5],16)&MASK),'draw coordinate ABI'
   if r[6] in (LABEL0,LABEL1,LABEL2):value={LABEL0:1000,LABEL1:1001,LABEL2:1002}[r[6]]
   else:
    assert self.get(r[6]+1,1)==0,'glyph termination'
    value=self.get(r[6],1)
   self.events.append([7,signed(r[4]),signed(r[5]),value])
  else:return False
  self.mutate()
  ret=r[31]
  for i in [1,2,3,*range(4,16),24,25]+([16,17,19] if p==SLOT else []):r[i]=(0xB0A00000+i*331+len(self.events))&MASK
  for i in range(20):self.f[i]=(0xBAAF0000+i)&MASK
  for i in range(16):self.put(r[29]+i,1,0x71+i)
  r[2]=v&MASK;self.pc=ret;return True

def cases():
 base=[4,1,0,0,32767,32767,-1,-2147483648,0,0,2,-1,0,3,127,1,1,0,2147483647,-32768]
 yield ('baseline',base[:])
 for count in (-32768,-257,-1,0,1,2,3,4):
  for enable in (-128,-1,0,1,127):
   for x in (-2147483648,-65537,-32769,-32768,-4,0,32764,32767,32768,65536,2147483647):
    a=base[:];a[0]=count;a[1]=enable;a[4]=x;a[5]=x;yield('signed-count-enable-coordinates',a)
 for hidden in range(16):
  for kind in range(16):
   for value in (-128,-1,0,1,2,127):
    a=base[:];a[2]=hidden;a[3]=kind;a[15]=value;a[16]=value;yield('skip-masks-signed-bytes',a)
 for g in range(-128,128):
  a=base[:];a[11:15]=[g,signed(g+1,8),signed(g+2,8),signed(g+3,8)];yield('all-gear-byte-values',a)
 for callback in range(1,45):
  for mutation in range(1,9):
   for value in ((1,2,3,4) if mutation==1 else (-128,-1,0,1,2,127)):
    a=base[:];a[8]=callback;a[9]=mutation;a[10]=value;a[17]=(callback+mutation)%4;yield('callback-mutations',a)
 for callback in (1,2,3,4,14,24,34,44):
  for count in (-32768,-1,0):
   a=base[:];a[8]=callback;a[9]=1;a[10]=count;yield('nonpositive-count-after-callback',a)
 rng=random.Random(0xEF288)
 for _ in range(3000):
  a=base[:];a[0]=rng.randrange(1,5);a[1]=rng.randrange(-128,128);a[2]=rng.randrange(16);a[3]=rng.randrange(16)
  for i in (4,5,6,7,18,19):a[i]=signed(rng.randrange(1<<32))
  a[8]=rng.randrange(1,45);a[9]=rng.randrange(1,9);a[10]=rng.randrange(1,5) if a[9]==1 else rng.randrange(-128,128)
  a[11:15]=[rng.randrange(-128,128) for _ in range(4)];a[15]=rng.randrange(-128,128);a[16]=rng.randrange(-128,128);a[17]=rng.randrange(4)
  yield('seeded-random',a)

def main():
 parser=argparse.ArgumentParser();parser.add_argument('--source-root',type=pathlib.Path,required=True);ns=parser.parse_args()
 sys.path.insert(0,str(ns.source_root/'tools/cloud'));import score
 words=score.targets()['func_800EF288'];native_sha=hashlib.sha256(b''.join(w.to_bytes(4,'big') for w in words)).hexdigest()
 lib=ctypes.CDLL(str(WORK/'host_audit.so'));lib.run_audit.argtypes=[ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int)];lib.run_audit.restype=ctypes.c_int
 counts={};visited=set();branches={};trace=hashlib.sha256()
 for ix,(name,a) in enumerate(cases()):
  cargs=(ctypes.c_int*20)(*a);out=(ctypes.c_int*512)();n=lib.run_audit(cargs,out);expected=[list(out[i*4:i*4+4]) for i in range(n)]
  m=AuditMachine(words,a);got=m.run()
  assert got==expected,(ix,name,a,got,expected)
  for r in (28,31):assert m.r[r]==m.initial[r]
  counts[name]=counts.get(name,0)+1;visited|=m.visited
  for off,values in m.branches.items():branches.setdefault(off,set()).update(values)
  trace.update(json.dumps([a,got],separators=(',',':')).encode())
 result={'base':'6b2e9e506fe3d2267a710e41c85af5364ccd00c7','function':'func_800EF288','native_bytes':len(words)*4,'native_sha256':native_sha,'source_sha256':hashlib.sha256((P/'candidate.c').read_bytes()).hexdigest(),'reviewed_interpreter_sha256':hashlib.sha256((P/'native_reviewed.py').read_bytes()).hexdigest(),'cases':sum(counts.values()),'case_families':counts,'native_instruction_words_visited':len(visited),'native_instruction_words':len(words),'unvisited_offsets':sorted(set(range(0,len(words)*4,4))-visited),'branches':{hex(k):sorted(v) for k,v in sorted(branches.items())},'trace_sha256':trace.hexdigest(),'host_sanitizer_cases':10000,'target_compilation_performed':False,'limitations':['Finite contract-hook behavior checks; does not execute selector, queues, renderer, width implementation or gameplay.','Uses audited producer MIPS engine with independent fixture and callbacks; not a second independent emulator.','Host ABI represents pointers at host width; source record scalar offsets are statically asserted; no native GearAssets sizeof claim.','Test storage is four model records, four cars, and four layout rows; no original capacities or arbitrary-index validity proved.']}
 (OUTPUT/'review.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
