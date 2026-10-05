"""Fail-closed bounded MIPS-II executor for the actual camera and four real helper bodies.

The remaining four external calls use explicit deterministic contract models;
these are test doubles, never compiler/matching context. No instruction words
are embedded here. Finite binary32 arithmetic is rounded after every operation.
"""
import math,struct
from tools.cloud import score
MASK=0xffffffff
BASE=0x800e95dc
CAR=0x100000
POS=0x102000
Q=0x103000
HP=0x104000
HN=0x105000
STACK=0x200800
STOP=0x70000000
REAL=['func_800E92C8','func_8009E820','func_800A61B0','func_800CFDEC']
ADDR=score.image_symbols()
TARGETS=score.targets()
DATA=score.own_data()

def signed(v,n=32):v&=(1<<n)-1;return v-(1<<n) if v&(1<<(n-1)) else v
def bits(v):return struct.unpack('>I',struct.pack('>f',v))[0]
def value(v):return struct.unpack('>f',struct.pack('>I',v&MASK))[0]
def f32(v):return value(bits(v))

class Machine:
 def __init__(self,words,case):
  self.mem={};self.case=case;self.code={BASE+4*i:w for i,w in enumerate(words)};self.coverage=set();self.events=[];self.steps=0;self.writes=[]
  for n in REAL:
   self.code.update({ADDR[n]+4*i:w for i,w in enumerate(TARGETS[n])})
  self.r=[0xc5000000+i for i in range(32)];self.r[0]=0;self.f=[bits(i+.25) for i in range(32)];self.cc=False;self.lo=0;self.hi=0
  self.r[4]=case[0]&MASK;self.r[5]=CAR;self.r[6]=POS;self.r[7]=0x2468;self.r[29]=STACK;self.r[31]=STOP
  self.saved_r=self.r[:];self.saved_f=self.f[:]
  for base,size in [(CAR,0x400),(POS,12),(Q,0x50),(HP,4),(HN,0x30),(STACK-0x800,0x880),
                    (ADDR['D_8012E690'],48),(ADDR['D_801526A8'],48),(ADDR['D_80150B70'],608),
                    (ADDR['D_8012E6C8'],8),(ADDR['D_8012E6E8'],16),(ADDR['D_801526F8'],16),
                    (ADDR['D_801526E0'],16),(ADDR['D_80152720'],16),(ADDR['D_801108C8'],16),
                    (ADDR['D_8002EB94'],4),(ADDR['state_word_a'],4)]:self.map(base,size)
  for a in [0x801244c8,0x801244cc,0x801244d0]:
   self.map(a,4);self.put(a,4,int.from_bytes(DATA.read(a,4),'big'))
  c=case;self.slot=c[1]
  for a,w,v in [(CAR+0x35c,1,c[1]),(CAR+0x35d,1,c[6]),(CAR+0x35e,1,7),(CAR+0x380,4,Q),(Q+0x48,4,HP),(HP,4,HN),(HN+0x2c,4,c[4]),(ADDR['state_word_a'],4,c[3]),(ADDR['D_8002EB94'],4,c[8])]:self.put(a,w,v)
  for i in range(3):self.put(CAR+8+4*i,4,c[11+i]);self.put(POS+4*i,4,c[14+i])
  for i in range(3):self.put(CAR+20+4*i,4,c[20+i])
  self.put(CAR+168,4,c[23]);self.put(CAR+172,4,c[24])
  for i in range(9):self.put(CAR+44+4*i,4,c[25+i])
  for i,b in enumerate([c[9],c[10],bits(2),bits(3)]):self.put(ADDR['D_801108C8']+4*i,4,b)
  for i in range(4):
   for n,vs in [('D_8012E690',[100+i,200+i,300+i]),('D_801526A8',[400+i,500+i,600+i])]:
    for j,v in enumerate(vs):self.put(ADDR[n]+12*i+4*j,4,bits(v))
   for j,v in enumerate([700+i,800+i,900+i]):self.put(ADDR['D_80150B70']+152*i+132+4*j,4,bits(v))
   self.put(ADDR['D_8012E6C8']+2*i,2,3)
   for n,v in [('D_8012E6E8',10+i),('D_801526F8',20+i),('D_801526E0',12+i),('D_80152720',.25)]:self.put(ADDR[n]+4*i,4,bits(v))
  for j in range(3):self.put(ADDR['D_8012E690']+12*self.slot+4*j,4,c[17+j])
  for n,v,w in [('D_8012E6C8',c[2],2),('D_8012E6E8',c[7],4),('D_801526F8',c[34],4),('D_801526E0',c[35],4)]:self.put(ADDR[n]+w*self.slot,w,v)
  self.before=dict(self.mem);self.writes=[]
 def map(self,a,n):
  for i in range(n):assert a+i not in self.mem,hex(a+i);self.mem[a+i]=0xa5
 def get(self,a,n):
  assert a%n==0 and all(a+i in self.mem for i in range(n)),('read',hex(a),n)
  return int.from_bytes(bytes(self.mem[a+i] for i in range(n)),'big')
 def put(self,a,n,v):
  assert a%n==0 and all(a+i in self.mem for i in range(n)),('write',hex(a),n)
  for i,b in enumerate((v&((1<<(8*n))-1)).to_bytes(n,'big')):self.mem[a+i]=b
  self.writes.append((a,n))
 def floats(self,p):return [self.get(p+4*i,4) for i in range(3)]
 def event(self,n,a=None,b=None):self.events += [n]+(self.floats(a) if a else [0]*3)+(self.floats(b) if b else [0]*3)
 def external(self,p):
  r=self.r
  if p==ADDR['vector_normalize_length']:
   assert r[4]==POS and r[5]==ADDR['D_80150B70']+152*self.slot+96;self.event(1,r[4]);v=self.floats(r[4])
   for i,x in enumerate(v):self.put(r[5]+4*i,4,x)
  elif p==ADDR['func_800E9234']:
   assert r[4]==CAR;self.event(2)
  elif p==ADDR['func_800CDE38']:
   assert r[4]==HP and self.case[4];self.event(3)
  elif p==ADDR['func_800E8D50']:
   assert r[4]==CAR and r[5]==POS and r[6]==0x2468;self.event(4,r[5],r[7])
   a=self.floats(r[5]);b=self.floats(r[7])
   for i in range(3):self.put(ADDR['D_80150B70']+152*self.slot+132+4*i,4,bits(value(a[i])+value(b[i])))
  else:raise AssertionError(('unexpected call',hex(p)))
  # Full ordinary caller-save clobbers, including floating registers.
  for i in [1,*range(2,16),24,25]:r[i]=0xbad00000+i
  for i in range(20):self.f[i]=bits(123+i)
  if p==ADDR['func_800CDE38']:r[2]=self.case[5]&MASK
 def wr(self,i,v):
  if i:self.r[i]=v&MASK
 def step(self,pc,delay=False):
  assert pc in self.code,hex(pc);self.steps+=1;assert self.steps<5000;self.coverage.add(pc)
  w=self.code[pc];op,rs,rt,rd,sh,fn=w>>26,w>>21&31,w>>16&31,w>>11&31,w>>6&31,w&63
  imm=w&65535;si=signed(imm,16);a,b=self.r[rs],self.r[rt];address=(a+si)&MASK;branch=None;skip=False
  if op==0:
   if fn==0:self.wr(rd,b<<sh)
   elif fn==2:self.wr(rd,b>>sh)
   elif fn==3:self.wr(rd,signed(b)>>sh)
   elif fn==8:branch=a
   elif fn==16:self.wr(rd,self.hi)
   elif fn==18:self.wr(rd,self.lo)
   elif fn in (24,25):
    v=(signed(a)*signed(b)) if fn==24 else a*b;self.lo=v&MASK;self.hi=(v>>32)&MASK
   elif fn==33:self.wr(rd,a+b)
   elif fn==35:self.wr(rd,a-b)
   elif fn==36:self.wr(rd,a&b)
   elif fn==37:self.wr(rd,a|b)
   elif fn==42:self.wr(rd,int(signed(a)<signed(b)))
   elif fn==43:self.wr(rd,int(a<b))
   else:raise AssertionError(('SPECIAL',fn,hex(pc)))
  elif op==9:self.wr(rt,address)
  elif op==10:self.wr(rt,int(signed(a)<si))
  elif op==11:self.wr(rt,int(a<(si&MASK)))
  elif op==12:self.wr(rt,a&imm)
  elif op==13:self.wr(rt,a|imm)
  elif op==15:self.wr(rt,imm<<16)
  elif op in (4,5,6,7,20,21,22,23):
   take={4:a==b,5:a!=b,6:signed(a)<=0,7:signed(a)>0}[op-16 if op>=20 else op]
   if take:branch=pc+4+4*si
   elif op>=20:skip=True
   else:branch=pc+8
  elif op in (2,3):
   branch=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
   if op==3:self.r[31]=pc+8
  elif op in (32,33,35,36,37,40,41,43):
   n={32:1,33:2,35:4,36:1,37:2,40:1,41:2,43:4}[op]
   if op>=40:self.put(address,n,b)
   else:self.wr(rt,signed(self.get(address,n),8*n) if op<36 else self.get(address,n))
  elif op in (49,53,57,61):
   if op==49:self.f[rt]=self.get(address,4)
   elif op==57:self.put(address,4,self.f[rt])
   elif op==53:self.f[rt]=self.get(address,4);self.f[rt+1]=self.get(address+4,4)
   else:self.put(address,4,self.f[rt]);self.put(address+4,4,self.f[rt+1])
  elif op==17:
   if rs==0:self.wr(rt,self.f[rd])
   elif rs==4:self.f[rd]=b
   elif rs==8:
    take=self.cc if rt&1 else not self.cc
    if take:branch=pc+4+4*si
    elif rt&2:skip=True
    else:branch=pc+8
   elif rs==20:
    assert fn==32;self.f[sh]=bits(signed(self.f[rd]))
   elif rs==16:
    x,y=value(self.f[rd]),value(self.f[rt])
    if fn==0:self.f[sh]=bits(x+y)
    elif fn==1:self.f[sh]=bits(x-y)
    elif fn==2:self.f[sh]=bits(x*y)
    elif fn==3:self.f[sh]=bits(x/y)
    elif fn==4:self.f[sh]=bits(math.sqrt(x))
    elif fn==5:self.f[sh]=self.f[rd]&0x7fffffff
    elif fn==6:self.f[sh]=self.f[rd]
    elif fn==7:self.f[sh]=self.f[rd]^0x80000000
    elif fn==13:assert -2147483648<=x<2147483648;self.f[sh]=int(x)&MASK
    elif fn==50:self.cc=x==y
    elif fn==60:self.cc=x<y
    elif fn==62:self.cc=x<=y
    else:raise AssertionError(('float',fn,hex(pc)))
   else:raise AssertionError(('COP1',rs,hex(pc)))
  else:raise AssertionError(('opcode',op,hex(pc)))
  if branch is not None:
   assert not delay,'branch in delay';assert self.step(pc+4,True)==pc+8
   if op==3 and branch not in self.code:self.external(branch);return self.r[31]
   return branch
  return pc+8 if skip else pc+4
 def run(self):
  pc=BASE
  while pc!=STOP:pc=self.step(pc)
  for i in [*range(16,24),28,29,30]:assert self.r[i]==self.saved_r[i],('saved',i)
  assert self.f[20:]==self.saved_f[20:]
  for a in range(CAR,CAR+0x380):
   if a not in (CAR+0x35d,CAR+0x35e):assert self.mem[a]==self.before[a],('car changed',hex(a))
  for a in range(STACK-0x800,STACK-0x300):assert self.mem[a]==self.before[a],'stack canary'
  allowed=[(STACK-0x300,STACK+32),(POS,POS+12),(CAR+0x35d,CAR+0x35f)]
  for n,width in [('D_8012E690',12),('D_801526A8',12),('D_8012E6C8',2),('D_8012E6E8',4),('D_80152720',4)]:
   a=ADDR[n]+width*self.slot;allowed.append((a,a+width))
  for off in [96,132]:
   a=ADDR['D_80150B70']+152*self.slot+off;allowed.append((a,a+12))
  for a in self.mem:
   if not any(lo<=a<hi for lo,hi in allowed):assert self.mem[a]==self.before[a],('untouched memory',hex(a))
  out=[signed(self.get(CAR+0x35d,1),8)&MASK,signed(self.get(CAR+0x35e,1),8)&MASK]+self.floats(POS)
  for i in range(4):
   out += [signed(self.get(ADDR['D_8012E6C8']+2*i,2),16)&MASK,self.get(ADDR['D_8012E6E8']+4*i,4),self.get(ADDR['D_80152720']+4*i,4)]
   out += self.floats(ADDR['D_8012E690']+12*i)+self.floats(ADDR['D_801526A8']+12*i)
   out += self.floats(ADDR['D_80150B70']+152*i+132)+self.floats(ADDR['D_80150B70']+152*i+96)
  out += [len(self.events)]+self.events
  return out+[0]*(128-len(out))

def reachable(words):
 """Static intraprocedural CFG. Calls return; unknown branch conditions fork."""
 code={BASE+4*i:w for i,w in enumerate(words)};todo=[BASE];seen=set()
 while todo:
  p=todo.pop()
  if p in seen:continue
  assert p in code,hex(p);seen.add(p);w=code[p];op=w>>26;rs=w>>21&31;rt=w>>16&31;si=signed(w&65535,16)
  if op==0 and w&63==8:
   seen.add(p+4);continue
  if op==3:seen.add(p+4);todo.append(p+8);continue
  if op==2:seen.add(p+4);todo.append(((p+4)&0xf0000000)|((w&0x3ffffff)<<2));continue
  if op in (4,5,6,7,20,21,22,23) or (op==17 and rs==8):
   seen.add(p+4)
   always=(op in (4,20) and rs==rt);never=(op in (5,21) and rs==rt)
   if not always:todo.append(p+8)
   if not never:todo.append(p+4+4*si)
   continue
  todo.append(p+4)
 return sorted(p-BASE for p in seen)
