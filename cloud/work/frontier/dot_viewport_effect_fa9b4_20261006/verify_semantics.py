#!/usr/bin/env python3
"""Bounded complete FA9B4 words against literal included host C.

Production context is read at BASE; native words are not emitted. --native-object
reuses the one baseline object; this verifier never invokes the target compiler.
"""
import argparse, ctypes, hashlib, importlib.util, json, random, struct, subprocess, sys
from pathlib import Path
BASE='6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
NAME='render_viewport_init'; START=0x800FA9B4; STACK=0x70010000; RETURN=0xFFFFFFFC
MASK=0xffffffff; N=4
P=0x80152818; M=0x8014A250; S=0x8014A118; E=0x80150B70; R=0x80152698
FLAGS=0x801392D8; STATUS=0x80153E88; MODE=0x8014A110; COUNT=0x80152744
SOUND=0x8010FFC0; FINAL=0x80114650; HUMAN=0x8014A108; OPTIONS=0x801174B4
CALLS={0x800EC0DC:1,0x800C2BE0:2,0x8038FCE0:3,0x80390F60:4,0x800D169C:5,
 0x800F93A0:6,0x800F8EC8:7,0x800CADA4:8,0x800AEB54:9,0x800AF06C:10,
 0x800D5524:11,0x80092360:12,0x800AED64:13,0x800C3578:14,0x800F8E90:15,
 0x800B0180:16,0x800BEAA0:17,0x800BD2C8:18,0x800CF69C:19,0x800F0100:20}
HERE=Path(__file__).resolve().parent

def signed(v,bits=32):
 v&=(1<<bits)-1
 return v-(1<<bits) if v>>(bits-1) else v

def load(root,toolroot):
 sys.path.insert(0,str(toolroot/'tools/cloud')); import score
 def git(path):return subprocess.check_output(['git','-C',str(root),'show',BASE+':'+path])
 symbols=json.loads(git('asm/us/blob/symbols.json'))['symbols']
 assert int(symbols[NAME],16)==START
 data=git('asm/us/blob/blob_800f8d9c.s').decode()
 import re
 section=data.split('.section .text.'+NAME+',',1)[1].split('.section',1)[0]
 words=[int(x,16) for x in re.findall(r'\.word\s+0x([0-9A-Fa-f]+)',section)]
 assert len(words)==231 and score.targets()[NAME]==words
 return score,words

class Memory:
 def __init__(self,c):
  self.regions=[(P,bytearray([0x5a])*(N*0x3b8)),(M,bytearray([0xa5])*(N*0x808)),
   (S,bytearray([0x69])*(N*76)),(E,bytearray([0x96])*(N*152)),(R,bytearray(N*4)),
   (FLAGS,bytearray(N*4)),(STATUS,bytearray(N*8)),(MODE,bytearray(4)),(COUNT,bytearray(1)),
   (SOUND,bytearray(1)),(FINAL,bytearray(1)),(HUMAN,bytearray(2)),(OPTIONS,bytearray(4)),
   (0x500000,bytearray(N*256)),(STACK-2048,bytearray([0xcc])*4096)]
  self.reads=[];self.writes=[]
  for a,v,w in [(MODE,c[0],4),(COUNT,c[1],1),(HUMAN,c[2],2),(OPTIONS,c[3],4),(SOUND,c[4],1),(FINAL,c[5],1)]:self.put(a,v,w)
  for i in range(N):
   b=32+i*16
   for a,v,w in [(P+i*952+0xed,c[b],1),(P+i*952+0x35c,c[b+1],1),(P+i*952+0x35d,c[b+2],1),
    (P+i*952+0x35e,c[b+3],1),(M+i*2056+0x7c6,c[b+4],2),(M+i*2056+0x7cc,c[b+5],1),
    (S+i*76+64,c[b+6],2),(FLAGS+i*4,c[b+7],4),(STATUS+i*8+7,c[b+8],1),
    (R+i*4,0x500000+i*256 if c[b+9] else 0,4)]:self.put(a,v,w)
   x=0x500000+i*256
   self.put(x,x+16);self.put(x+16+40,x+80);self.put(x+80,x+96);self.put(x+96+5,c[b+10],1)
   for j in range(3):
    self.put(E+i*152+36+j*4,c[b+11+j]);self.put(E+i*152+132+j*4,0x40400000+j*0x40000)
    self.put(M+i*2056+0x22c+j*4,struct.unpack('>I',struct.pack('>f',i*10+j))[0])
  self.initial=[bytes(d) for a,d in self.regions];self.writes=[]
 def locate(self,a,w):
  assert a%w==0,('unaligned',hex(a),w)
  for base,data in self.regions:
   if base<=a and a+w<=base+len(data):return data,a-base
  raise AssertionError(('unmapped',hex(a),w))
 def get(self,a,w=4):
  data,off=self.locate(a,w);self.reads.append((a,w));return int.from_bytes(data[off:off+w],'big')
 def put(self,a,v,w=4):
  data,off=self.locate(a,w);v&=(1<<(w*8))-1;data[off:off+w]=v.to_bytes(w,'big');self.writes.append((a,w,v))
 def sign(self,a,w=4):return signed(self.get(a,w),w*8)&MASK

class Machine:
 def __init__(self,words,c):self.code=words;self.c=c;self.m=Memory(c);self.events=[];self.coverage=set();self.branch_edges=set()
 def hook(self,target,r,f):
  m,c=self.m,self.c;id=CALLS[target];e=[id]+[0]*10
  if id==10:
   assert STACK-2048<=r[4]<STACK and r[4]%2==0
   e[1:5]=[m.sign(r[4],2),r[5],r[6],r[7]]
  elif id==11:
   assert (r[4]-M)%2056==0 and 0<=r[4]-M<N*2056
   e[1]=(r[4]-M)//2056
  elif id==12:e[1:5]=r[4:8]
  elif id==13:
   assert (r[4]-M-0x22c)%2056==0
   e[1:5]=[(r[4]-M-0x22c)//2056,r[5],r[6],r[7]]
   e[5:11]=[m.get(r[29]+j) for j in range(16,40,4)]
  elif id in (14,15,16,17):e[1]=r[4]
  self.events.append(e)
  if len(self.events)==c[7]:
   j,mask=c[14],c[8]
   for bit,a,v,w in [(1,MODE,c[9],4),(2,COUNT,c[10],1),(4,M+j*2056+0x7c6,c[11],2),
    (8,M+j*2056+0x7cc,c[12],1),(16,SOUND,c[13],1),(32,STATUS+j*8+7,c[15],1),
    (64,FINAL,c[16],1),(128,P+j*952+0xed,c[17],1)]:
    if mask&bit:m.put(a,v,w)
  for i in list(range(1,16))+[24,25,31]:r[i]=(0xbd000000+len(self.events)*256+i)&MASK
  for i in range(20):f[i]=0x7fc10000+i
 def run(self):
  r=[0xa9000000+i for i in range(32)];r[0]=0;r[29]=STACK;r[31]=RETURN;initial=r[:]
  f=[0xdead0000+i for i in range(32)];fi=f[:];pc=START;pending=None;m=self.m
  for _ in range(15000):
   if pc==RETURN:break
   assert START<=pc<START+len(self.code)*4,('pc',hex(pc))
   self.coverage.add(pc);w=self.code[(pc-START)//4];op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31;sh=(w>>6)&31;fn=w&63;imm=w&65535;si=signed(imm,16)
   old,pending,nxt=pending,None,pc+4;a=(r[rs]+si)&MASK
   if op==0:
    if fn==0:r[rd]=(r[rt]<<sh)&MASK
    elif fn==2:r[rd]=r[rt]>>sh
    elif fn==3:r[rd]=(signed(r[rt])>>sh)&MASK
    elif fn==8:pending=r[rs]
    elif fn==0x21:r[rd]=(r[rs]+r[rt])&MASK
    elif fn==0x23:r[rd]=(r[rs]-r[rt])&MASK
    elif fn==0x24:r[rd]=r[rs]&r[rt]
    elif fn==0x25:r[rd]=r[rs]|r[rt]
    elif fn==0x2a:r[rd]=int(signed(r[rs])<signed(r[rt]))
    elif fn==0x2b:r[rd]=int(r[rs]<r[rt])
    else:raise AssertionError(('unknown SPECIAL',hex(pc),fn))
   elif op==2:pending=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
   elif op==3:
    target=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2);assert target in CALLS
    r[31]=pc+8;pending=('call',target,pc+8)
   elif op in (1,4,5,6,7,20,21,22,23):
    likely=op in (20,21,22,23) or (op==1 and rt in (2,3))
    if op==1:
     assert rt in (0,1,2,3);take=signed(r[rs])>=0 if rt&1 else signed(r[rs])<0
    elif op in (4,20):take=r[rs]==r[rt]
    elif op in (5,21):take=r[rs]!=r[rt]
    elif op in (6,22):take=signed(r[rs])<=0
    else:take=signed(r[rs])>0
    self.branch_edges.add((pc,take))
    if take:pending=pc+4+si*4
    elif likely:nxt+=4
   elif op==9:r[rt]=a
   elif op==10:r[rt]=int(signed(r[rs])<si)
   elif op==11:r[rt]=int(r[rs]<(si&MASK))
   elif op==12:r[rt]=r[rs]&imm
   elif op==13:r[rt]=r[rs]|imm
   elif op==15:r[rt]=imm<<16
   elif op==17:
    if rs==4:f[rd]=r[rt]
    elif rs==0:r[rt]=f[rd]
    else:raise AssertionError(('unknown COP1',hex(pc),rs))
   elif op in (32,33,35,36,37):
    width={32:1,33:2,35:4,36:1,37:2}[op];v=m.get(a,width)
    r[rt]=(signed(v,width*8)&MASK) if op in (32,33) else v
   elif op in (40,41,43):m.put(a,r[rt],{40:1,41:2,43:4}[op])
   elif op==49:f[rt]=m.get(a)
   elif op==57:m.put(a,f[rt])
   elif op==53:f[rt]=m.get(a);f[rt+1]=m.get(a+4)
   elif op==61:m.put(a,f[rt]);m.put(a+4,f[rt+1])
   else:raise AssertionError(('unknown opcode',hex(pc),op))
   r[0]=0;assert old is None or pending is None,'transfer in delay slot'
   if isinstance(old,tuple):self.hook(old[1],r,f);pc=old[2]
   else:pc=old if old is not None else nxt
  else:raise AssertionError('instruction limit')
  assert r[29]==STACK and r[16:24]==initial[16:24] and r[28]==initial[28] and r[30]==initial[30]
  assert f[20:]==fi[20:]
  out=[len(self.events)]+[v for e in self.events for v in e]+[m.get(MODE),m.sign(COUNT,1),m.sign(SOUND,1),m.sign(FINAL,1)]
  for i in range(N):
   out += [m.sign(P+i*952+0xed,1),m.sign(P+i*952+0x35d,1),m.sign(P+i*952+0x35e,1),
     m.sign(M+i*2056+0x7c6,2),m.sign(M+i*2056+0x7cc,1),m.get(S+i*76+64,2),
     m.get(FLAGS+i*4),m.get(STATUS+i*8+7,1)]+[m.get(E+i*152+132+j*4) for j in range(3)]
  return out

def fixture(rng):
 c=[0]*128;c[:6]=[rng.choice([-1,0,1,2,3,4,5,6,7]),rng.choice([-128,-1,0,1,2,3,4]),rng.randrange(5),rng.getrandbits(32),rng.choice([-128,-1,0,1,127]),rng.choice([-1,0,1])]
 c[7]=rng.randrange(1,35);c[8]=rng.randrange(256);c[9]=rng.choice([0,2,4,5,6]);c[10]=rng.randrange(5)
 c[11]=rng.randrange(N);c[12]=rng.choice([-128,-1,0,1,2,127]);c[13]=rng.choice([-1,0,1]);c[14]=rng.randrange(N)
 c[15]=rng.choice([0,1,6,255]);c[16]=rng.choice([-1,0,1]);c[17]=rng.choice([-128,-1,0,1,127])
 for i in range(N):
  b=32+i*16;c[b:b+11]=[rng.choice([-128,-1,0,1,127]),rng.choice([-128,-1,0,1,2,3]),rng.randrange(-128,128),rng.randrange(-128,128),rng.randrange(N),rng.choice([-128,-1,0,1,2,127]),rng.choice([0,1,65534,65535]),rng.getrandbits(32),rng.choice([0,1,5,6,255]),rng.randrange(2),rng.choice([-128,-1,0,127])]
  c[b+11:b+14]=[rng.choice([0,0x80000000,0x3f800000,0xc0200000,0x7f7fffff,0x00800000]) for j in range(3)]
 return [signed(x) for x in c]

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--reference-root',type=Path,required=True);ap.add_argument('--tool-root',type=Path,required=True);ap.add_argument('--output',type=Path,required=True);ap.add_argument('--host-library',type=Path,required=True);ap.add_argument('--native-object',type=Path);ap.add_argument('--cases',type=int,default=5000);a=ap.parse_args()
 score,words=load(a.reference_root,a.tool_root)
 lib=ctypes.CDLL(str(a.host_library.resolve()));lib.layouts.restype=ctypes.c_int;assert lib.layouts()==1
 lib.run.argtypes=[ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_uint32)];lib.run.restype=ctypes.c_int
 compiled=None
 if a.native_object:
  raw=score.text_words(a.native_object);compiled,masks,unresolved,unverified,errors=score.relocate(a.native_object,raw,0,len(raw)*4,score.image_symbols())
  assert not any((masks,unresolved,unverified,errors)),(masks,unresolved,unverified,errors)
 rng=random.Random(0xFA9B4);coverage=set();branches=set();ids=set();natives=0
 for n in range(a.cases):
  c=fixture(rng)
  if n<60:c[0]=[-1,0,1,2,3,4,5,6,7,2147483647][n%10];c[1]=[-128,-1,0,1,3,4][n//10];c[8]=0
  native=Machine(words,c);result=native.run();natives+=1
  coverage.update(native.coverage);branches.update(native.branch_edges);ids.update(e[0] for e in native.events)
  out=(ctypes.c_uint32*2048)();count=lib.run((ctypes.c_int*128)(*c),out);host=list(out)[:count]
  assert result==host,('host mismatch',n,c,result,host)
  if compiled is not None:
   cand=Machine(compiled,c);actual=cand.run();natives+=1
   assert actual==host,('compiled mismatch',n,c,actual,host)
   # Complete data region equality also checks untouched fields and sentinels.
   assert [(a,bytes(d)) for a,d in native.m.regions[:-1]]==[(a,bytes(d)) for a,d in cand.m.regions[:-1]],n
 for bad,expected in [(0xffffffff,'unknown opcode')]:
  altered=words[:];altered[0]=bad
  try:Machine(altered,fixture(rng)).run()
  except AssertionError as exc:assert expected in str(exc)
  else:raise AssertionError('invalid opcode accepted')
 m=Memory(fixture(rng))
 for addr,width in [(0x12340000,4),(P+1,4)]:
  try:m.get(addr,width)
  except AssertionError:pass
  else:raise AssertionError('invalid read accepted')
 result=dict(status='BOUNDED NATIVE/HOST SEMANTIC PASS',base=BASE,cases=a.cases,native_executions=natives,
  native_instruction_coverage=len(coverage),native_words=len(words),branch_outcomes=len(branches),external_calls_covered=sorted(ids),
  source_sha256=hashlib.sha256((HERE/'candidate.c').read_bytes()).hexdigest(),host_sha256=hashlib.sha256((HERE/'host.c').read_bytes()).hexdigest(),verifier_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
  target_sha256=hashlib.sha256(struct.pack('>'+str(len(words))+'I',*words)).hexdigest(),
  missed_instruction_offsets=[hex(pc-START) for pc in range(START,START+4*len(words),4) if pc not in coverage],
  negative_controls=['unsupported opcode rejected','unmapped read rejected','unaligned read rejected'],
  limits=['valid initialized object chains and indices 0..3','callbacks are conservative ordered stubs, not executed helper bodies','single bounded callback mutation per invocation','B image resident for mode 6 by lifecycle contract','finite normal-or-zero float payload copies','not full game/ROM proof or original C identity'])
 a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
