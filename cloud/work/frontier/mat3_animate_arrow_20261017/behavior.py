"""Independent field/geometry model versus extracted host C, with an injected atan boundary.

The production magnitude and world-vector bodies execute. The polygon boundary
captures all arguments. This is bounded host behavior evidence, not native execution.
The separate strict replay is the native instruction/data proof.
"""
from pathlib import Path
import ctypes,itertools,math,struct,subprocess
HERE=Path(__file__).resolve().parent
PALETTE=((0,0,224),(224,0,0),(224,224,0),(0,224,0),(224,224,224))
ALPHA=(32,48,80,128,160,192,239)
CORNERS=((1.,1.),(-1.,1.),(-1.,-1.),(1.,-1.))
def require(c,m):
 if not c:raise ValueError(m)
def f(x):return struct.unpack('f',struct.pack('f',x))[0]
def mul(a,b):return f(a*b)
def add(a,b):return f(a+b)
def model(v,angle):
 enabled,show,hidden,state,game,amode,alpha,cidx,mode,view,views,player,slot,team,fade,op=v
 visible=enabled and show and (game!=6 or (not hidden and state<0))
 if not visible:return [0x8123,0,0,0,0,0,0,0],[]
 a=255
 if game==6:
  a=max(32,alpha if amode in (1,2) else ALPHA[cidx])
 if mode==2:
  rgb=PALETTE[team if game==6 else slot]
  a=min(a,112) if fade else a
  if game==2 and player>0:a=128
 else:rgb=PALETTE[4]
 flags=(1<<view)|(0x3600 if game==6 else 0x1600)
 vertices=None;radius=f(1.2);scale=f(1.1);half=f(1.5707964);inv=f(.63661975)
 if game==6:
  x,y,side=None,None,None
  vertical=f(16.5 if views>=2 else 14.5);threshold=mul(scale,.75 if views>=2 else f(1.1))
  if abs(angle)>half:
   reduced=add(angle,f(math.pi) if angle<0 else -f(math.pi))
   x=mul(mul(mul(mul(radius,inv),19.),reduced),inv);y=mul(mul(scale,inv),vertical);side=0
  elif abs(angle)>mul(radius,f(.48)) and abs(angle)<half:
   side=3 if angle<0 else 1;x=mul(mul(radius,inv),19. if angle<0 else -19.)
   y=mul(mul(mul(mul(scale,inv),vertical),f(abs(angle)-threshold)),f(1./f(half-threshold)))
  if side is not None:
   vertices=[None]*4
   for i,(cx,cy) in enumerate(CORNERS):vertices[(side+i)%4]=[add(cx,x),f(cy-y),f(f(31.415927)/radius)]
   flags|=0x10
 if vertices is None:
  # Identity camera basis uses the genuine N64 sign conventions.
  delta=(3.,7.,16.);distance=f(math.sqrt(add(add(mul(3.,3.),mul(7.,7.)),mul(16.,16.))));size=f(distance/25.)
  vertices=[[add(mul(cx,size),5.),add(add(add(mul(cy,size),10.),size),5.),20.] for cx,cy in CORNERS]
 return [flags,1,*rgb,a,4,0],[z for row in vertices for z in row]
def fixtures():
 base=[1,1,0,-1,6,1,255,0,2,0,2,1,0,0,0,1]
 # Every gate, alpha source, palette index, fade, window, and draw sector.
 for game,mode,slot,team,fade,amode,alpha,view,angle in itertools.product((0,2,6),(0,2),(0,3),(0,3),(0,1),(0,1,2),(0,31,112,113,255),(0,3),(-2.,-1.,0.,1.,2.)):
  v=base[:];v[4]=game;v[8]=mode;v[12]=slot;v[13]=team;v[14]=fade;v[5]=amode;v[6]=alpha;v[9]=view;v[7]=(alpha+slot)%7;v[10]=1 if view==0 else 4;v[11]=view%2;v[15]=view%2
  yield v,f(angle)
 for ix,value in ((0,0),(1,0),(2,1),(3,0),(3,1)):
  for game in (2,6):v=base[:];v[ix]=value;v[4]=game;yield v,0.
WRAPPER=r'''
#include <math.h>
#include <string.h>
CAR_DATA D_80152818[4]; MODELDAT D_8014A250[4];Camera D_80150B70[4];View D_8017A510[4];Mesh D_8015B268[4];
s8 D_801613A8,D_8017A63C,D_801613C0[4][4],D_8012E67C[4];s32 state_word_a,D_8014A110;
Color4 D_8011B558[5]={{{0,0,224,255}},{{224,0,0,255}},{{224,224,0,255}},{{0,224,0,255}},{{224,224,224,255}}};
Color4 D_8011B578={{255,255,255,255}};u8 D_8011B56C[7]={32,48,80,128,160,192,239};
static float injected_angle;static unsigned *result;static float *points;static Mesh *expected_mesh;
f32 func_8008C768(f32 y,f32 x){(void)y;(void)x;return injected_angle;}
void func_8008C074(Mesh *mesh,s32 count,f32 (*xyz)[3],u16 texture,Color4 *color,u16 flags,s32 indexed){
 result[0]=flags;result[1]++;result[2]=color->c.r;result[3]=color->c.g;result[4]=color->c.b;result[5]=color->c.a;result[6]=count;result[7]=texture||indexed||mesh!=expected_mesh;
 memcpy(points,xyz,12*sizeof(float));
}
void run_case(const int *v,float angle,unsigned *out,float *xyz){
 int i;Effect visual;CAR_DATA *car;MODELDAT *model;Camera *camera;View *view;
 memset(D_80152818,0,sizeof D_80152818);memset(D_8014A250,0,sizeof D_8014A250);memset(D_80150B70,0,sizeof D_80150B70);memset(D_801613C0,0,sizeof D_801613C0);memset(&visual,0,sizeof visual);memset(out,0,8*sizeof *out);memset(xyz,0,12*sizeof *xyz);
 visual.player=v[11];visual.view=v[9];visual.mesh=2;car=&D_80152818[v[11]];model=&D_8014A250[v[11]];camera=&D_80150B70[v[9]];view=&D_8017A510[v[9]];
 D_801613A8=v[0];state_word_a=v[1]?0x400000:0;car->hidden=v[2];model->state=v[3];D_8014A110=v[4];car->flags=1;car->alpha_mode=v[5];car->alpha=v[6];car->color_index=v[7];model->mode=v[8];model->player=v[12];D_8012E67C[v[12]]=v[13];D_801613C0[v[12]][v[9]]=v[14];D_8017A63C=v[10];
 car->pos[0]=5.;car->pos[1]=10.;car->pos[2]=20.;camera->pos.x=2.;camera->pos.y=3.;camera->pos.z=4.;for(i=0;i<3;i++)camera->basis[i][i]=camera->mat[i][i]=1.;view->radius=1.2f;view->scale=1.1f;
 D_8015B268[2].flags=0x123;injected_angle=angle;result=out;points=xyz;expected_mesh=&D_8015B268[2];entity_render_setup(&visual,v[15]);if(!out[1])out[0]=D_8015B268[2].flags;
}
'''
def verify(out,source=None):
 out=Path(out);out.mkdir(parents=True,exist_ok=True)
 source=source if source is not None else (HERE/'group/group.c').read_text()
 vector=(HERE/'closure/world_vector/source.c').read_text().split('void func_8008C544',1)[1]
 magnitude=(HERE/'closure/magnitude/source.c').read_text().split('f32 func_8008B3C8',1)[1]
 (out/'host.c').write_text('#include <math.h>\n'+source+'\nvoid func_8008C544'+vector+'\nf32 func_8008B3C8'+magnitude+WRAPPER)
 subprocess.run(['cc','-shared','-fPIC','-O2','-fno-strict-aliasing','-ffp-contract=off','-Werror','-o',str(out/'host.so'),str(out/'host.c'),'-lm'],check=True,capture_output=True)
 lib=ctypes.CDLL(str((out/'host.so').resolve()));lib.run_case.argtypes=[ctypes.POINTER(ctypes.c_int),ctypes.c_float,ctypes.POINTER(ctypes.c_uint),ctypes.POINTER(ctypes.c_float)]
 total=0;sectors=set()
 for v,a in fixtures():
  iv=(ctypes.c_int*len(v))(*v);ov=(ctypes.c_uint*8)();xyz=(ctypes.c_float*12)();lib.run_case(iv,a,ov,xyz);want,points=model(v,a)
  require(list(ov)==want,f'behavior fields case {total}: {list(ov)} != {want}')
  require(not points or all(abs(x-y)<=2e-5 for x,y in zip(xyz,points)),f'behavior geometry case {total}')
  sectors.add((bool(want[1]),want[0]&0x7010));total+=1
 return {'fixtures':total,'observed_draw_classes':len(sectors),'scope':'host field/geometry model; deterministic atan boundary; genuine magnitude and world-vector bodies; captured polygon arguments'}
