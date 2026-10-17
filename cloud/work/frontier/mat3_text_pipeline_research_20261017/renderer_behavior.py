"""Bounded donor renderer versus independent glyph/event model and native MIPS."""
from pathlib import Path
import sys,struct,itertools,json,importlib.util
HERE=Path(__file__).resolve().parent;R=HERE.parents[3];sys.path.insert(0,str(R));from tools.cloud import score
ENTRY=0x800B6788;STACK=0x81010000;RETURN=0x81234560;MASK=0xffffffff
TEXT,HEADER,GLYPHS,KERNS,TEXTURES=0x80500000,0x80501000,0x80502000,0x80503000,0x80504000
S=score.image_symbols();G=lambda n:S[n]
SETUP=G('sound_update_channel');SCISSOR=G('func_8008A3E4');SET=G('func_800878E0');CLEAR=G('func_8008705C');COLORS=G('func_8008A148');PAGE=G('object_render');DRAW=G('func_80087110')
ARGC={SETUP:1,SCISSOR:4,SET:1,CLEAR:1,COLORS:4,PAGE:11,DRAW:6}
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
def visible(m):return {a:v for a,v in m.items() if not STACK-512<=a<STACK+256}
def fixtures():
 texts=[(),(65,),(66,65),(65,66),(66,67,68,65),(65,32,66),(65,10,66),(69,65),(0,65),(256,66),(65,65535,68)]
 for wide,nonprop,kern,mode,alpha,position,cell in itertools.product((0,1),(0,1),(0,1),(1,2),(None,0.,16.5,254.9),((-31,17),(319,-23)),(9,3)):
  for chars in texts:
   if not wide and any(x>255 for x in chars):continue
   data=(bytes([255])+b''.join(x.to_bytes(2,'big') for x in (*chars,0))) if wide else bytes([*chars,0])
   yield dict(wide=wide,nonprop=nonprop,kern=kern,mode=mode,alpha=alpha,position=position,data=data,spacing=-2 if position[0]<0 else 3,leading=2,cell=cell)

def state(c):
 regions=[(TEXT,len(c['data'])),(HEADER,16),(GLYPHS,48),(KERNS,16),(TEXTURES,72),(STACK-512,768)]
 for n,size in [('D_801497F0',4),('D_80149800',4),('D_80149878',512),('D_80149820',8),('D_8002AFC0',4),('D_8002AFC4',4),('D_80149B88',4),('D_8012E6D0',4),('D_80149B08',4),('D_80149B28',4),('D_80149B48',8),('D_80114748',4),('D_80149D92',2),('D_80149D9E',2),('D_80149B60',1),('D_80149B70',1)]:regions.append((G(n),size))
 m={a+i:0 for a,n in regions for i in range(n)}
 for i,b in enumerate(c['data']):m[TEXT+i]=b
 for n,v in [('D_801497F0',HEADER),('D_80149800',GLYPHS),('D_8002AFC0',320),('D_8002AFC4',240),('D_80149B88',0x8000|(32 if c['alpha'] is not None else 0)),('D_8012E6D0',0x80200000),('D_80149B08',4 if c['mode']==1 else 3),('D_80149B28',0 if c['mode']==1 else 1)]:store(m,G(n),4,v)
 for i,v in enumerate([0,c['mode'],7,3,2,12,c['cell'],4,3,c['nonprop'],c['kern'],2,4,0,0,0]):m[HEADER+i]=v
 for i in range(256):store(m,G('D_80149878')+2*i,2,-1)
 for i in range(4):
  store(m,G('D_80149878')+2*(65+i),2,i);store(m,GLYPHS+12*i,4,KERNS+4*i)
  for off,v in enumerate([65+i,i%2,1+i,2,4+2*i,8+i,0,0]):m[GLYPHS+12*i+4+off]=v
  for prev,v in enumerate([3,2,1,2]):m[KERNS+4*i+prev]=v+i
 for i in range(2):
  store(m,G('D_80149820')+4*i,4,TEXTURES+36*i);store(m,TEXTURES+36*i+16,2,32);store(m,TEXTURES+36*i+18,2,32);store(m,TEXTURES+36*i+24,4,0x80600000+0x1000*i)
 for i,v in enumerate([230,221,213,199,11,22,33,44]):m[G('D_80149B48')+i]=v
 store(m,G('D_80114748'),4,bits(c['alpha'] if c['alpha'] is not None else -1.))
 store(m,G('D_80149D92'),2,c['position'][0]);store(m,G('D_80149D9E'),2,c['position'][1]);store(m,G('D_80149B60'),1,c['leading']);store(m,G('D_80149B70'),1,c['spacing'])
 return m

def event(m,d,args):
 args=tuple(a&MASK for a in args)
 if d==COLORS:
  rgba=tuple(load(m,args[0]+i,1) for i in range(8));store(m,G('D_8012E6D0'),4,args[0]);return (d,args,rgba)
 return (d,args)

def model(c):
 m=state(c);attr=load(m,G('D_80149B88'));events=[]
 def call(d,*args):events.append(event(m,d,args))
 call(SETUP,0);call(SCISSOR,0,0,319,239);call(SET,attr);call(CLEAR,~attr);store(m,G('D_8012E6D0'),4,0)
 if c['alpha'] is not None:m[G('D_80149B48')+3]=int(f32(c['alpha']))
 mode,size=(4,0) if c['mode']==1 else (3,1);call(COLORS,G('D_80149B48'),mode,size,0)
 def page(i):call(PAGE,0x80600000+0x1000*i,mode,size,32,32,0,0,31,31,0,0)
 page(0);current_page=0;x,y=c['position'];previous=None
 if c['wide']:
  payload=c['data'][1:];codes=[int.from_bytes(payload[i:i+2],'big') for i in range(0,len(payload),2)]
 else:codes=list(c['data'][:-1])
 for code in codes:
  if code==10:y+=10+c['leading'];x=c['position'][0];previous=None;continue
  index=code-65
  if code==32 or not 0<=index<4:
   x+=c['cell']+c['spacing'] if c['nonprop'] else 4;previous=None;continue
  if previous is not None and previous>0 and c['kern'] and not c['nonprop']:x-=[3,2,1,2][previous]+index
  texture=index%2
  if texture!=current_page:page(texture);current_page=texture
  left=1+index;right=4+2*index;top=2;bottom=8+index;width=right-left+1
  draw_x=x+int((c['cell']-width)/2) if c['nonprop'] else x
  call(DRAW,draw_x,y,draw_x+width-1,y+bottom-top,left,top)
  x+=(c['cell'] if c['nonprop'] else width)+c['spacing'];previous=index
 return visible(m),events

def execute(code,c):
 m=state(c);events=[];r=[0xa5000000+13*i for i in range(32)];f=[bits(i*.25) for i in range(32)];fbefore=f[:]
 r[0]=0;r[18]=TEXT;r[16]=len(c['data']);r[29]=STACK;r[31]=RETURN;before=r[:]
 pc=ENTRY;pending=None;lo=0;fcc=False;fcsr=0;coverage=set();branches=set()
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
   elif rs==2:r[rt]=fcsr
   elif rs==6:fcsr=r[rt]
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
    elif fn==36:
     require(0<=real(f[rd])<255 and fcsr&3==1,"float conversion outside bounded contract");f[sh]=int(real(f[rd]));fcsr&=~0x78
    elif fn==6:f[sh]=f[rd]
    elif fn==7:f[sh]=f[rd]^0x80000000
    elif fn in (50,60,62):fcc=real(f[rd])==real(f[rt]) if fn==50 else (real(f[rd])<real(f[rt]) if fn==60 else real(f[rd])<=real(f[rt]))
    else:raise ValueError(f'unexpected COP1 S {fn}')
   else:raise ValueError(f'unexpected COP1 {rs}/{fn}')
  else:raise ValueError(f'unexpected opcode {op}')
  r=[x&MASK for x in r];r[0]=0;require(old is None or pending is None,'nested branch delay slot')
  if isinstance(old,tuple):
   dest=old[1];require(dest in ARGC,f'unknown call {dest:x}');args=[r[8]] if dest==SETUP else r[4:4+min(ARGC[dest],4)]
   args += [load(m,r[29]+16+4*i) for i in range(max(0,ARGC[dest]-4))]
   events.append(event(m,dest,args));result=0
   for j in list(range(1,16))+[24,25]:r[j]=(0xbad00000+31*j)&MASK
   for j in range(20):f[j]=bits(j*.5)
   r[2]=result&MASK;pc=old[2]
  else:pc=old if old is not None else nxt
 else:raise ValueError('execution bound exceeded')
 require(r[29]==STACK and r[31]==RETURN and f[20:]==fbefore[20:],'private stack/FPR boundary')
 return visible(m),events,coverage,branches

def candidate(obj):
 obj=Path(obj);words=score.text_words(obj);start=score.symbols(obj)['audio_doppler_calc'];data,secs=score._elf(obj);ti=score._text_index(secs)
 symbol=next(s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']=='audio_doppler_calc' and s['type']==2 and s['section']==ti)
 end=start+symbol['size'];got,masks,u,v,e=score.relocate(obj,words,start,end,score.image_symbols());require(not(masks or u or v or e),'uncertain relocation')
 return {ENTRY+4*i:w for i,w in enumerate(got[start//4:end//4])}
def cfg_unreachable(code):
 seen=set();todo=[(ENTRY,None)]
 while todo:
  pc,pending=todo.pop()
  if pc==RETURN or (pc,pending) in seen:continue
  require(pc in code,'CFG escaped function');seen.add((pc,pending));w=code[pc];op=w>>26;rs=w>>21&31;rt=w>>16&31;fn=w&63;imm=signed(w,16)
  if pending is not None:todo.append((pending,None));continue
  if op==0 and fn==8:
   require(rs==31,'unrecognized CFG register jump');todo.append((pc+4,RETURN))
  elif op in (2,3):
   dest=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
   todo.append((pc+4,pc+8 if op==3 else dest))
  elif op in (1,4,5,6,7,20,21,22,23) or op==17 and rs==8:
   likely=op>=20 or op==1 and rt in (2,3) or op==17 and rt&2
   decision=None
   if op in (4,5,20,21) and rs==rt:decision=op in (4,20)
   elif op in (6,7,22,23) and rs==0:decision=op in (6,22)
   elif op==1 and rs==0:decision=rt in (1,3)
   if decision is not False:todo.append((pc+4,pc+4+imm*4))
   if decision is not True:
    if likely:todo.append((pc+8,None))
    else:todo.append((pc+4,pc+8))
  else:todo.append((pc+4,None))
 return [hex(p-ENTRY) for p in sorted(set(code)-{pc for pc,_ in seen})]

def verify(obj):
 native={ENTRY+4*i:w for i,w in enumerate(score.targets()['audio_doppler_calc'])};compiled=candidate(obj);coverage=[set(),set()];branches=[set(),set()];count=0
 for i,c in enumerate(fixtures()):
  expected=model(c)
  for j,code in enumerate((native,compiled)):
   actual=execute(code,c)
   if actual[:2]!=expected:
    diff=[hex(a) for a in actual[0] if actual[0][a]!=expected[0][a]]
    raise ValueError(f'renderer mismatch program{j} case{i},bytes{diff[:12]}\nactual{actual[1]}\nexpected{expected[1]}')
   coverage[j]|=actual[2];branches[j]|=actual[3]
  count+=1
 return {'status':'PASS_BOUNDED','fixtures':count,'executed_words':[len(x) for x in coverage],'native_missing':[hex(a-ENTRY) for a in sorted(set(native)-coverage[0])],'branches':[len(x) for x in branches],'cfg_unreachable':cfg_unreachable(native),'claims':[]}
if __name__=='__main__':print(json.dumps(verify(sys.argv[1]),indent=2))
