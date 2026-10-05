"""Bounded unchanged host-C and native/GNU-linked contract replay."""
import ctypes,hashlib,itertools,json,random,shutil,subprocess
from pathlib import Path
from steady_camera_native import Machine,bits,REAL,ADDR,BASE
from tools.cloud import score
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]

def host(directory,source=None):
 directory.mkdir(parents=True,exist_ok=True)
 shutil.copyfile(HERE/'host.c',directory/'host.c')
 (directory/'candidate.c').write_text(source if source is not None else (HERE/'candidate.c').read_text())
 original=(ROOT/'src/blob/groups/func_800E92C8/group.c').read_text()
 a=original.index('void func_800E92C8(Cr *car, f32 ab, f32 AB, f32 *camoff) {');b=original.index('void func_800E95DC(',a)
 (directory/'rear_camera.inc').write_text(original[a:b])
 cmd=['cc','-std=c89','-pedantic','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror','-Wno-unused-variable',
      '-ffp-contract=off','-fno-fast-math','-fsanitize=undefined','-fno-sanitize-recover=all',str(directory/'host.c'),'-lm','-o',str(directory/'host.so')]
 p=subprocess.run(cmd,capture_output=True,text=True);assert p.returncode==0,p.stderr
 dll=ctypes.CDLL(str(directory/'host.so'));dll.run_case.argtypes=[ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)]
 return dll

def cases():
 rng=random.Random(0xe95dc)
 # Initialization, both interpolation legs, selection legs and valid default legs.
 for index,(mode,slot,leg,flag,exists,selection) in enumerate(itertools.product([0,1,-1],range(4),[-1,0,1,2,3],[0,8],[0,1],[1,2,4])):
  elapsed=[0,.125,.875,1.0,1.4,2.0][index%6];dt=[0,.016666668,.25,-.125][index%4]
  c=[mode,slot,leg,flag,exists,selection,[2,4,8][index%3],bits(elapsed),bits(dt),bits(1.4),bits(1.0)]
  c += [bits(rng.uniform(-500,500)) for i in range(9)]
  speed=[0,.5,1,2,99,100,101,150][index%8]
  c += [bits(speed),bits(rng.uniform(-20,20)),bits(0),bits(3),bits(4)]
  # Orthogonal identity or axis permutation, plus nonuniform matrices.
  matrix=[[1,0,0,0,1,0,0,0,1],[0,0,-1,0,1,0,1,0,0],[.75,.125,0,0,1,0,0,0,.5]][index%3]
  c += list(map(bits,matrix));c += [bits([-20,20,30][index%3]),bits([3,8,16][index%3])]
  yield [x&0xffffffff for x in c]

def call(dll,c):
 x=(ctypes.c_uint32*len(c))(*c);o=(ctypes.c_uint32*128)();dll.run_case(x,o);return list(o)

def verify(directory,linked,limit=None):
 dll=host(directory/'host');coverage=set();digest=hashlib.sha256();count=0;selected=list(cases())
 if limit:selected=selected[:limit]
 native=score.targets()['func_800E95DC']
 for c in selected:
  expected=oracle(c)
  host_result=call(dll,c)
  assert host_result==expected,('host/oracle',count,c,[(i,hex(a),hex(b)) for i,(a,b) in enumerate(zip(host_result,expected)) if a!=b][:20])
  for label,words in [('native',native),('GNU-linked',linked)]:
   machine=Machine(words,c);actual=machine.run()
   assert actual==expected,(label,count,c,[(i,hex(a),hex(b)) for i,(a,b) in enumerate(zip(actual,expected)) if a!=b][:20])
   coverage.update(machine.coverage)
  digest.update(bytes.fromhex(''.join('%08x'%x for x in expected)));count+=1
 return {'cases':count,'native_and_GNU_runs':2*count,'host_c89_ubsan':'passed','host_fp_contract':'off','native_saved_registers_and_canaries':'passed',
         'real_native_helper_bodies':REAL,'native_covered_words':sum(BASE<=p<BASE+1684 for p in coverage),'native_total_words':421,'independent_scalar_oracle':'passed','native_cfg_reachable_offsets':__import__('steady_camera_native').reachable(native),
         'native_executed_offsets':sorted(p-BASE for p in coverage if BASE<=p<BASE+1684),
         'helper_coverage':{n:sum(ADDR[n]<=p<ADDR[n]+4*len(score.targets()[n]) for p in coverage) for n in REAL},
         'output_sha256':digest.hexdigest(),
         'external_models':['vector_normalize_length copies its observed input to its output','func_800E9234 records invocation without effects','func_800CDE38 returns configured view selection','func_800E8D50 records inputs and writes camera position as position plus offset'],
         'limits':'Finite binary32 values, valid four-player storage, nonzero leg durations, acyclic selector pointers. Four named external calls are deterministic test models; their internals/gameplay and concurrent mutation are not proven.'}

def oracle(c):
 """Independent scalar camera-leg model; deliberately omits overwritten first-leg delta."""
 from steady_camera_native import value,f32,signed
 def add(a,b):return f32(a+b)
 def sub(a,b):return f32(a-b)
 def mul(a,b):return f32(a*b)
 def div(a,b):return f32(a/b)
 def rot(v,m,transpose):
  return [add(add(mul(v[0],m[i if transpose else 3*i]),mul(v[1],m[i+3 if transpose else 3*i+1])),mul(v[2],m[i+6 if transpose else 3*i+2])) for i in range(3)]
 def rear():
  import math
  mode=signed(c[6],8);v=list(map(value,c[20:23]));matrix=list(map(value,c[25:34]));ab=value(c[34]);height=value(c[35])
  speed=0.0 if mode==8 else f32(math.sqrt(add(mul(v[0],v[0]),mul(v[2],v[2]))))
  if speed<=100:
   temp=[0.0,height,-ab] if mode==8 else rot([0.0,height,-ab],matrix,True);temp[1]=abs(temp[1])
  if speed<=1:return temp
  if mode==4:
   out=rot([value(c[24]),value(c[23]),0.0],matrix,True);out[0]=-out[0];out[2]=-out[2]
  else:
   if ab<0:v[1]=-v[1]
   vec=rot(v,matrix,False);vec[2]=abs(vec[2]);out=rot(vec,matrix,True);out[0]=-out[0];out[2]=-out[2]
  out=[mul(x,div(ab,speed)) for x in out]
  width=f32(math.sqrt(add(mul(out[0],out[0]),mul(out[2],out[2]))))
  if width>f32(.001):
   projected=div(mul(-out[1],height),ab);scale=div(sub(width,projected),width)
   out[0]=mul(out[0],scale);out[2]=mul(out[2],scale)
  out[1]=abs(sub(div(mul(width,height),ab),out[1]))
  if speed<100:
   k=div(sub(speed,1.0),sub(100.0,1.0));out=[add(mul(sub(out[i],temp[i]),k),temp[i]) for i in range(3)]
  return out
 slot=c[1];leg=signed(c[2],16);pos=list(map(value,c[14:17]));cached=list(map(value,c[17:20]));car=list(map(value,c[11:14]))
 elapsed=value(c[7]);mode=signed(c[6],8);mode2=7;frame=[700.0+slot,800.0+slot,900.0+slot];basis=[0xa5a5a5a5]*3;clear=[400.0+slot,500.0+slot,600.0+slot];elastic=.25;events=[]
 def event(n,a=None,b=None):events.extend([n]+list(map(bits,a or [0,0,0]))+list(map(bits,b or [0,0,0])))
 if signed(c[0],16)==0:
  cached=pos[:];frame=pos[:];pos=[sub(pos[i],car[i]) for i in range(3)];event(1,pos);basis=list(map(bits,pos));leg=0;elapsed=0.0
 elif leg in (0,1):
  r=rear()
  if leg==0:target=[sub(car[0],mul(value(c[31]),40.0)),add(car[1],5.0),sub(car[2],mul(value(c[33]),40.0))]
  else:target=[add(pos[i],r[i]) for i in range(3)]
  ratio=div(elapsed,value(c[9+leg]));delta=[mul(sub(target[i],cached[i]),ratio) for i in range(3)]
  endpoint=[add(cached[i],delta[i]) for i in range(3)];offset=[sub(endpoint[i],pos[i]) for i in range(3)];elastic=0.0
  event(4,pos,offset);frame=[add(pos[i],offset[i]) for i in range(3)];frame[1]=add(frame[1],4.0);elapsed=add(elapsed,value(c[8]))
  if value(c[9+leg])<=elapsed:leg+=1;elapsed=0.0;cached=frame[:]
 elif leg==2:
  event(2)
  if not c[3]&8:
   if c[4]:event(3);chosen=signed(c[5])
   else:chosen=2
   mode=mode2=signed(chosen,8)
   if mode==1:clear=[0.0,0.0,0.0]
 out=[mode&0xffffffff,mode2&0xffffffff]+list(map(bits,pos))
 for i in range(4):
  if i==slot:out+=[leg&0xffffffff,bits(elapsed),bits(elastic)]+list(map(bits,cached+clear+frame))+basis
  else:out+=[3,bits(10+i),bits(.25)]+list(map(bits,[100+i,200+i,300+i,400+i,500+i,600+i,700+i,800+i,900+i]))+[0xa5a5a5a5]*3
 out += [len(events)]+events
 return out+[0]*(128-len(out))

def negative_controls(directory):
 source=(HERE/'candidate.c').read_text()
 choices={
  'wrong_rear_distance':(' * 40.0f',' * 30.0f'),
  'wrong_height_lift':('v84[1] += 4.0f','v84[1] += 3.0f'),
  'missed_equality_transition':('D_801108C8[D_8012E6C8[pl]] <= D_8012E6E8[pl]','D_801108C8[D_8012E6C8[pl]] < D_8012E6E8[pl]'),
  'wrong_initial_position':('res[2] = car->pos[2]','res[2] = car->pos[1]'),
  'wrong_view_reset':('if (car->mode == 1)','if (car->mode == 2)')}
 rejected={}
 for n,(a,b) in choices.items():
  assert a in source;dll=host(directory/n,source.replace(a,b))
  for c in cases():
   if call(dll,c)!=oracle(c):rejected[n]=c;break
  assert n in rejected,n
 return rejected
