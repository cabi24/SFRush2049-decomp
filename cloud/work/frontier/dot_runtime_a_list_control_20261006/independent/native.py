"""Independent bounded MIPS execution; target words loaded from protected inputs.

Only finite nonnegative float-to-u32 cases, trap-disabled round-to-nearest
multiplication and round-toward-zero conversion are modeled. External rendering,
font and fade helpers are side-effecting O32 contracts. Hidden executes native.
"""
import random, struct
from semantics import signed,fbits,fvalue,f32
ENTRY=0x803AE940
HIDDEN,UPDATE,RENAME,FADE,FONT,WIDTH=0x80094F88,0x80094EC8,0x800EF5B0,0x800BEA30,0x800B42F0,0x800B3FA4
MODE,FIRST,TOTAL,ACTIVE,SELECTED,INDICES,NAMES,POINTS,RESOURCES,TEXTURES=0x803B7714,0x803BA878,0x803BA898,0x803BA908,0x803BA85A,0x803BA8B0,0x803B8294,0x803B9224,0x8017A4E0,0x80140BF0
OWNER,SP,STOP,BASE,POINTERS,STRINGS=0x81001000,0x81010000,0x81234560,0x81020000,0x81020100,0x81021000
MASK=0xffffffff
class Native:
 def __init__(self,words,hidden):
  self.code={ENTRY+i*4:w for i,w in enumerate(words)}
  self.code.update({HIDDEN+i*4:w for i,w in enumerate(hidden)})
  self.coverage=set();self.branches=set();self.reads=set();self.writes=set();self.size=len(words)*4
 def run(self,c,salt=0):
  pulse,item,group,texture,mode,first,total,active,selected,width,height,fade,hook,hide=c
  rng=random.Random(salt^0xae940);memory={};trace=[];updates=0
  for a,n in ((OWNER,64),(SP-128,144),(MODE,4),(FIRST,2),(TOTAL,2),(ACTIVE,1),(SELECTED,2),(INDICES,64),(NAMES,128),(POINTS,256),(RESOURCES,20),(TEXTURES,512),(BASE,4),(POINTERS,512),(STRINGS,1024)):
   assert not set(range(a,a+n))&memory.keys()
   memory.update((a+i,rng.randrange(256)) for i in range(n))
  def mem(a,n,v=None):
   assert a%n==0 and all(a+i in memory for i in range(n)),('memory',hex(a),n)
   if v is None:return int.from_bytes(bytes(memory[a+i] for i in range(n)),'big')
   for i,byte in enumerate((v&((1<<(8*n))-1)).to_bytes(n,'big')):memory[a+i]=byte
  for a,n,v in ((14,2,-321),(16,2,654),(20,2,width),(22,2,height),(24,1,77),(25,1,2),(26,1,hide),(44,4,0xabcd0000|pulse|item<<4|group<<8|texture<<12),(52,2,5)):mem(OWNER+a,n,v)
  for a,n,v in ((MODE,4,mode),(FIRST,2,first),(TOTAL,2,total),(ACTIVE,1,active),(SELECTED,2,selected),(BASE,2,55),(BASE+2,2,2),(RESOURCES+12,4,BASE),(RESOURCES+16,4,POINTERS)):mem(a,n,v)
  for i in range(128):mem(POINTERS+i*4,4,STRINGS+i*8)
  for i in range(32):mem(INDICES+i*2,2,i*2+1);mem(NAMES+i*4,4,STRINGS+i*8)
  for i in range(16):
   mem(TEXTURES+i*32+21,1,i*11)
   for j in range(4):mem(POINTS+i*16+j*4,2,i*100+j*23-150);mem(POINTS+i*16+j*4+2,2,i*70+j*17-90)
  initial=dict(memory)
  def emit(event,arg):
   b=[signed(mem(OWNER+a,2),16) for a in (14,16,20,22)]+[mem(OWNER+24,1),mem(OWNER+25,1),signed(mem(OWNER+26,1),8),signed(mem(OWNER+44,4)),mem(OWNER+52,2)]
   trace.extend([event,arg]+b+[signed(mem(FIRST,2),16),signed(mem(TOTAL,2),16),signed(mem(SELECTED,2),16),signed(mem(ACTIVE,1),8)]+[mem(TEXTURES+i*32+21,1) for i in range(16)])
  def effect(stage):
   if hook!=stage:return
   for a,n,v in ((OWNER+14,2,-11),(OWNER+16,2,17),(OWNER+20,2,-31),(OWNER+22,2,19),(OWNER+44,4,0x12345678),(OWNER+52,2,7),(FIRST,2,2),(TOTAL,2,12),(SELECTED,2,3),(ACTIVE,1,-1),(BASE+2,2,9)):mem(a,n,v)
  r=[rng.getrandbits(32) for _ in range(32)];f=[rng.getrandbits(32) for _ in range(32)]
  r[0]=0;r[4]=OWNER;r[29]=SP;r[31]=STOP;before=r[:];saved_f=f[20:];fcsr=0
  allowed=set(range(SP-80,SP))|set(range(OWNER+14,OWNER+18))|set(range(OWNER+24,OWNER+27))|{TEXTURES+i*32+21 for i in range(16)}
  mutable=allowed|set(range(OWNER+20,OWNER+24))|set(range(OWNER+44,OWNER+48))|set(range(OWNER+52,OWNER+54))|set(range(FIRST,FIRST+2))|set(range(TOTAL,TOTAL+2))|set(range(SELECTED,SELECTED+2))|{ACTIVE,BASE+2,BASE+3}
  pc=ENTRY;pending=None;steps=0
  while pc!=STOP:
   steps+=1;assert steps<700
   if pc==HIDDEN:assert pending is None and r[4]==OWNER and signed(r[5]) in (0,1);emit(1,signed(r[5]))
   if pc in (UPDATE,RENAME,FADE,FONT,WIDTH):
    assert pending is None;ret=r[31];rv=rng.getrandbits(32);fv=None
    if pc==UPDATE:
     assert r[4]==OWNER;emit(2,0);updates+=1
     if updates==1:effect(1)
    elif pc==RENAME:
     assert r[4]==OWNER and r[6]==1 and STRINGS<=r[5]<STRINGS+256 and (r[5]-STRINGS)%8==0
     emit(3,(r[5]-STRINGS)//8);effect(2)
    elif pc==FADE:emit(4,0);effect(3);fv=fade
    elif pc==FONT:assert r[4]==11;emit(5,11);effect(4)
    else:
     assert r[5]==MASK and STRINGS<=r[4]<STRINGS+1024 and (r[4]-STRINGS)%8==0
     index=(r[4]-STRINGS)//8;emit(6,index);effect(5);rv=index*3-7
    for i in (1,2,3,*range(4,16),24,25):r[i]=rng.getrandbits(32)
    f[:20]=[rng.getrandbits(32) for _ in range(20)]
    r[2]=rv&MASK
    if fv is not None:f[0]=fv
    pc=ret;continue
   assert pc in self.code,('unbound PC',hex(pc));self.coverage.add(pc)
   word=self.code[pc];op=word>>26;rs=word>>21&31;rt=word>>16&31;rd=word>>11&31;sh=word>>6&31;fn=word&63;imm=word&65535;si=signed(imm,16);address=(r[rs]+si)&MASK
   old,pending=pending,None;nextpc=pc+4
   if word==0:pass
   elif op==0:
    if fn==0:r[rd]=r[rt]<<sh
    elif fn==2:r[rd]=r[rt]>>sh
    elif fn==3:r[rd]=signed(r[rt])>>sh
    elif fn==8:pending=r[rs]
    elif fn==33:r[rd]=r[rs]+r[rt]
    elif fn==35:r[rd]=r[rs]-r[rt]
    elif fn==37:r[rd]=r[rs]|r[rt]
    elif fn==43:r[rd]=int(r[rs]<r[rt])
    else:raise AssertionError(('special',hex(pc),fn))
   elif op==1:
    assert rt in (0,1);take=(signed(r[rs])<0) if rt==0 else signed(r[rs])>=0;self.branches.add((pc,take))
    if take:pending=pc+4+si*4
   elif op==3:r[31]=pc+8;pending=((pc+4)&0xf0000000)|((word&0x3ffffff)<<2)
   elif op in (4,5,20,21):
    take=r[rs]==r[rt] if op in (4,20) else r[rs]!=r[rt];self.branches.add((pc,take))
    if take:pending=pc+4+si*4
    elif op in (20,21):nextpc+=4
   elif op==9:r[rt]=address
   elif op==10:r[rt]=int(signed(r[rs])<si)
   elif op==11:r[rt]=int(r[rs]<(si&MASK))
   elif op==12:r[rt]=r[rs]&imm
   elif op==13:r[rt]=r[rs]|imm
   elif op==14:r[rt]=r[rs]^imm
   elif op==15:r[rt]=imm<<16
   elif op==17:
    if rs==0:r[rt]=f[rd]
    elif rs==2:assert rd==31;r[rt]=fcsr
    elif rs==4:f[rd]=r[rt]
    elif rs==6:assert rd==31;fcsr=r[rt]
    elif rs==16:
     if fn==1:f[sh]=fbits(fvalue(f[rd])-fvalue(f[rt]))
     elif fn==2:f[sh]=fbits(fvalue(f[rd])*fvalue(f[rt]))
     elif fn==36:
      assert fcsr&3==1
      v=fvalue(f[rd]);assert v==v and abs(v)!=float('inf')
      if -2147483648<=v<2147483648:f[sh]=int(v)&MASK
      else:f[sh]=0x80000000;fcsr|=0x40
     else:raise AssertionError(('float',hex(pc),fn))
    else:raise AssertionError(('cop1',hex(pc),rs))
   elif op in (32,33,35,36,37):
    n={32:1,33:2,35:4,36:1,37:2}[op];v=mem(address,n);self.reads.add((address,n));r[rt]=signed(v,n*8) if op in (32,33) else v
   elif op in (40,41,43):
    n={40:1,41:2,43:4}[op];assert all(address+i in allowed for i in range(n)),('write',hex(pc),hex(address),n);self.writes.add((address,n));mem(address,n,r[rt])
   else:raise AssertionError(('opcode',hex(pc),op))
   r[:]=[v&MASK for v in r];r[0]=0;pc=old if old is not None else nextpc
  assert pending is None and f[20:]==saved_f and fcsr==0
  assert all(r[i]==before[i] for i in (*range(16,24),26,27,28,29,30,31))
  assert all(v==initial[a] for a,v in memory.items() if a not in mutable)
  emit(7,signed(r[2]));return trace
