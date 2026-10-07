"""Bounded sanitized host-C checks against a separate callback-state oracle."""
import ctypes,hashlib,itertools,random,struct,subprocess
from pathlib import Path
HERE=Path(__file__).resolve().parent
def signed(v,b=32):
 v&=(1<<b)-1
 return v-(1<<b) if v&(1<<(b-1)) else v
def fbits(x):return struct.unpack('>I',struct.pack('>f',x))[0]
def fvalue(x):return struct.unpack('>f',struct.pack('>I',x))[0]
def f32(x):return fvalue(fbits(x))
def half(v):return (-1 if v<0 else 1)*(abs(v)//2)
def oracle(c):
 pulse,item,group,texture,mode,first,total,active,selected,w,h,fade,hook,hide=c
 b=[-321,654,w,h,77,2,hide,signed(0xabcd0000|pulse|item<<4|group<<8|texture<<12),5]
 flags=[i*11 for i in range(16)];trace=[];updates=0;base=2
 def emit(event,arg):trace.extend([event,arg]+b+[first,total,selected,active]+flags)
 def effect(stage):
  nonlocal first,total,selected,active,base
  if hook==stage:b[:4]=[-11,17,-31,19];b[7]=0x12345678;b[8]=7;first,total,selected,active,base=2,12,3,-1,9
 def update():
  nonlocal updates
  emit(2,0);updates+=1
  if updates==1:effect(1)
 def hidden(value):
  emit(1,value)
  if b[6]!=value:b[6]=value;update()
  return b[6]
 if hidden(int(mode==1)):emit(7,1);return trace
 if group==1:
  if hidden(int(item==2)):emit(7,1);return trace
 if group==0:
  if item==2:
   if hidden(int(first==0)):emit(7,1);return trace
  elif item==3:
   if hidden(int(total-first<5)):emit(7,1);return trace
 emit(3,texture*2+int(active!=0 or item>=2));effect(2)
 if pulse:
  emit(4,0);effect(3);value=f32(fvalue(fade)*255.0);assert 0<=value<2**32;b[4]=int(value)&255
 b[0]=signed(group*100+item*23-150-half(b[2]),16)
 b[1]=signed(group*70+item*17-90-half(b[3]),16)
 if group==0:
  emit(5,11);effect(4)
  if item==0:
   index=selected*2+1+base;emit(6,index);effect(5);b[0]=signed(b[0]-(index*3-7),16)
  if item<=1:b[1]=signed(b[1]+(selected-first)*13,16)
 if item==1:b[5]=1
 elif item==3:flags[b[8]]|=8
 update();emit(7,1);return trace

def cases():
 for pulse,item,group,texture in itertools.product((0,1,15),range(4),(0,1,2,15),(0,1,15)):
  for mode,first,total,active in ((0,0,4,0),(0,1,5,0),(0,1,6,-1),(0,-32768,32767,127),(1,1,12,1),(2,7,12,-128)):
   for hook in range(6):yield (pulse,item,group,texture,mode,first,total,active,3,101,-55,fbits(.5),hook,-1)
 for h in (-32768,-32767,-3,-1,0,1,3,32767):
  for w in (-32768,-32767,-3,-1,0,1,3,32767):
   for fade in (0.,.001,.5,.99999994,1.,2.):yield (1,0,0,3,0,1,12,0,3,w,h,fbits(fade),0,0)
 for active in range(-128,128):yield (1,0,0,1,0,1,12,active,3,101,55,fbits(.1),0,0)

def host(build,source=None,label='host'):
 out=build/(label+'.so');cmd=['gcc','-std=c89','-O2','-Wall','-Wextra','-Werror','-shared','-fPIC','-ffp-contract=off','-fsanitize=undefined,bounds,float-cast-overflow','-fno-sanitize-recover=all']
 if source:cmd+=['-DCANDIDATE="'+str(source)+'"']
 p=subprocess.run(cmd+[str(HERE/'host.c'),'-o',str(out)],capture_output=True,text=True);assert p.returncode==0,p.stderr
 fn=ctypes.CDLL(str(out)).host_run;fn.argtypes=[ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int)];fn.restype=ctypes.c_int
 def run(c):
  result=(ctypes.c_int*1024)();n=fn((ctypes.c_int*14)(*c),result);assert 0<n<=1024;return list(result[:n])
 return run

def verify(build,source):
 run=host(build,source);samples=list(cases());digest=hashlib.sha256()
 for i,c in enumerate(samples):
  got=run(c);want=oracle(c);assert got==want,(i,c,got,want);digest.update(repr(want).encode())
 return {'host_cases':len(samples),'oracle_trace_sha256':digest.hexdigest(),'sanitizers':['undefined','bounds','float-cast-overflow'],'hooks':['Hidden update','texture selection','fade read','font selection','string width'],'table_domain':'group/texture 0..15, item 0..3, selected 0..31; synthetic data only'},samples
