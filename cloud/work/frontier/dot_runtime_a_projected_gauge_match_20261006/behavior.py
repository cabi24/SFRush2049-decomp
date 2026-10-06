"""Independent scalar oracle and sanitizer-backed unchanged-source host tests."""
import ctypes, hashlib, itertools, struct, subprocess
from pathlib import Path
HERE=Path(__file__).resolve().parent

def signed(v,bits=32):
 v&=(1<<bits)-1
 return v-(1<<bits) if v&(1<<(bits-1)) else v

def f32(v):return struct.unpack('>f',struct.pack('>f',v))[0]
def bits(v):return struct.unpack('>I',struct.pack('>f',v))[0]
def value(v):return struct.unpack('>f',struct.pack('>I',v&0xffffffff))[0]
def trunc(v,d):return (1 if v>=0 else -1)*(abs(v)//d)

def oracle(c):
 p,k,segment,count,flag,hide,w,h,z,alpha,car,px,py,hook=c
 trace=[];updates=0;anim=signed(k<<16|p<<4|segment);enabled=1
 x,y,left,right,top,bot,opacity=-321,654,-17,-18,-19,-20,77
 def effect(stage):
  nonlocal w,h,anim,alpha,car
  if hook==stage:w,h=77,-13;anim=signed(anim^0xffff00ff);alpha=199;car=2
 def update():
  nonlocal updates
  trace.append(3);updates+=1
  if updates==1:effect(1)
 def hidden(request):
  nonlocal hide
  trace.extend([1,request])
  if hide!=request:hide=request;update()
  return hide
 if p>=count:hidden(1);enabled=0
 elif flag:hidden(1)
 elif not hidden(int((-1.0<value(z)<1.0) or alpha==0)):
  trace.extend([2,p,(k+19)*64+48]);effect(2)
  quarter=trunc(h,4)
  x=signed(px-trunc(w,2),16);y=signed(py-trunc(quarter,2),16)
  left=0;right=signed(w-1,16);top=0;bot=signed(quarter-1,16);opacity=alpha
  if 2.0<value(z):top=signed(top+trunc(h,2),16);bot=signed(bot+trunc(h,2),16)
  if segment==1:
   top=signed(top+quarter,16);bot=signed(bot+quarter,16)
   if k in (13,14):
    scalar=f32(.75+(p+car)/(4 if k==13 else 8));denom=.25 if k==13 else .5
    result=f32(f32(f32(f32(scalar-.75)/denom)*f32(w-1))/15.0)
    assert -32768<=int(result)<=32767
    right=int(result)
  update()
 return trace+[4,1,x,y,left,right,top,bot,opacity,hide,enabled,anim,w,h]

def cases():
 def c(p=0,k=13,segment=1,count=4,flag=0,hide=0,w=123,h=63,z=3,alpha=255,car=0,px=100,py=-100,hook=0):return (p,k,segment,count,flag,hide,w,h,bits(z),alpha,car,px,py,hook)
 for p,k,segment,car,hook in itertools.product(range(4),(12,13,14,15),(0,1,2),range(13),range(3)):
  yield c(p,k,segment,car=car,hide=-1,hook=hook)
 for p,count in itertools.product((0,1,3,4,15),(-32768,-1,0,1,2,4,32767)):
  if p>=4 and p<count:continue
  yield c(p,count=count)
 for flag in range(-128,128):yield c(flag=flag)
 for z,alpha,hide in itertools.product((-3,-1,-0.0,0.0,1,2,3),(0,1,127,128,255),(-1,0,1)):
  yield c(z=z,alpha=alpha,hide=hide)
 for h in range(-32768,32768):yield c(k=12,segment=0,h=h)
 for w,h,px,py in itertools.product((-32768,-32767,-3,-1,0,1,3,32767),(-32768,-3,-1,0,1,3,32767),(-32768,0,32767),(-32768,0,32767)):
  yield c(k=12,segment=0,w=w,h=h,px=px,py=py)

def host(build,source):
 build.mkdir(parents=True,exist_ok=True)
 out=build/'host.so';r=subprocess.run(['gcc','-std=c89','-O2','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-shared','-fPIC','-fstrict-aliasing','-ffp-contract=off','-fsanitize=undefined,bounds,float-cast-overflow','-fno-sanitize-recover=all','-DCANDIDATE="'+str(source)+'"',str(HERE/'host.c'),'-o',str(out)],capture_output=True,text=True)
 assert r.returncode==0,r.stderr
 lib=ctypes.CDLL(str(out));fn=lib.host_run;fn.argtypes=[ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int)];fn.restype=ctypes.c_int
 def run(c):
  out=(ctypes.c_int*256)();n=fn((ctypes.c_int*14)(*c),out);return list(out[:n])
 return run

def verify(build,source):
 run=host(build,source);digest=hashlib.sha256();n=0
 for n,c in enumerate(cases(),1):
  expected=oracle(c);actual=run(c);assert actual==expected,(c,expected,actual)
  digest.update(struct.pack('>%di'%len(expected),*expected))
 return {'host_cases':n,'all_signed_halfword_heights':65536,'trace_sha256':digest.hexdigest(),'native_execution':False,'helper_implementation':'bounded side-effecting hooks'}
