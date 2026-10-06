#!/usr/bin/env python3
"""Independent case selection and comparison of AF06C host/native behavior."""
import argparse,hashlib,json,os,shutil,subprocess,sys,tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
_bootstrap=argparse.ArgumentParser(add_help=False)
_bootstrap.add_argument('--packet-dir',type=Path,default=(HERE.parent if (HERE.parent/'candidate.c').is_file() else HERE.parent/'tagged-effect-service/packet'))
PACKET=_bootstrap.parse_known_args()[0].packet_dir.resolve()
sys.path.insert(0,str(PACKET))
from native import Machine,fbits,fvalue
from fixture import Fixture,PLAYERS,GROUPS,EXTRAS,SCENES,NODES,MATRICES,INPUT,HEAD,RING,RING_TABLE
from host_adapter import Host
from audit_contracts import load,BASE

def mutate_at(destination,occurrence,changes):
 def change(f,d):
  if d!=destination:return
  f.audit_hits=getattr(f,'audit_hits',0)+1
  if f.audit_hits!=occurrence:return
  for address,value,width in changes:f.memory.put(address,value,width)
 return change

def cases():
 out=[]
 def add(name,**kw):out.append((name,kw))
 for mode in (0,1,-1,0x10000):
  for ring in (0,49):
   for point in (0x80090284,0x8008D6B0,0x8008E26C):
    add('live-ring-%x-%d-%d'%(point,mode,ring),mode=mode,count=4,ring=ring,sound=-1,
        mutation=mutate_at(point,1,[(RING,48 if ring==0 else 49,2)]))
   add('head-after-scene-%d-%d'%(mode,ring),mode=mode,count=4,ring=ring,sound=1,
       mutation=mutate_at(0x8008E26C,1,[(HEAD,NODES+7*24,4)]))
 for mode in (1,-1):
  for dest,occ in ((0x80090284,1),(0x8008D6B0,1),(0x8008D6B0,2),(0x8008E26C,1)):
   add('input-reload-%x-%d-%d'%(dest,occ,mode),mode=mode,index=1,count=2,
       mutation=mutate_at(dest,occ,[(INPUT,3,2)]))
 for mode in (0,1):
  for enabled in (0,-128,127):
   add('live-sound-flag-%d-%d'%(mode,enabled),mode=mode,count=4,sound=1,enabled=1,
       mutation=mutate_at(0x8008E26C,1,[(0x8010FFC0,enabled,1)]))
 for index in (0,1,5):
  add('snapshot-color-%d'%index,index=index,count=2,
      mutation=mutate_at(0x80090284,1,[(0x8011B554,0xFFEEDDCC,4)]))
  add('live-player-position-%d'%index,index=index,count=2,
      mutation=mutate_at(0x8008D6B0,1,[(PLAYERS+952*index+8,fbits(56.75),4),(PLAYERS+952*index+12,fbits(-33.125),4)]))
  add('shortcut-gate-after-create-%d'%index,shortcut=True,index=index,
      mutation=mutate_at(0x8008E26C,4,[(0x80156994,1,1)]))
  add('shortcut-selector-six-%d'%index,shortcut=True,index=index,
      mutation=mutate_at(0x8008E26C,4,[(0x8014978C,6,1)]))
  add('shortcut-negative-selector-%d'%index,shortcut=True,index=index,
      mutation=mutate_at(0x8008E26C,4,[(0x8014978C,255,1)]))
  add('shortcut-count-reread-%d'%index,shortcut=True,index=index,count=4,
      mutation=mutate_at(0x8008E26C,4,[(0x8014A108,3,2),(0x80156994,1,1)]))
  add('shortcut-snapshot-range-%d'%index,shortcut=True,index=index,
      mutation=mutate_at(0x8008E26C,1,[(0x801239A8,fbits(99.),4),(0x801239AC,fbits(77.),4),(0x801239B0,fbits(-33.),4),(0x80156994,1,1)]))
  add('failed-shortcut-event-only-%d'%index,shortcut=True,index=index,alloc=(False,),
      mutation=mutate_at(0x80090284,1,[(0x80156994,1,1)]))
 for p in ((100.25,-100.5,50.125),(-0.,0.,-1.17549435e-38)):
  add('position-pointer-mutation-'+str(p),mode=0,sound=1,
      mutation=mutate_at(0x8008D6B0,1,[(INPUT+4*i,fbits(v),4) for i,v in enumerate(p)]))
 for old in (0,23,0x1234FFFF,0xFFFF8000):
  for shortcut in (False,True):add('signed-scene-%d-%d'%(old,shortcut),old_scene=old,shortcut=shortcut)
 for seed in (0,1,0xFFFFFFFF,0x80000000,0x7FFFFFFF,0x12345678,0xCAFEBABE):
  for index in (0,5):add('rng-rounding-%x-%d'%(seed,index),shortcut=True,seed=seed,index=index)
 for pivot in (.5,1.):
  for delta in (-2,-1,0,1,2):
   for sound in (0,1,-1):
    for enabled in (0,1,-128):
     add('sound-ulp-%g-%d-%d-%d'%(pivot,delta,sound,enabled),mode=0,scale=fvalue(fbits(pivot)+delta),sound=sound,enabled=enabled)
 for scale in (-100.,-0.,0.,fvalue(0x00800000),1e20):
  for mode in (0,1):add('sound-other-%g-%d'%(scale,mode),mode=mode,scale=scale,sound=1)
 for gate,selector in ((0,-128),(0,-1),(0,0),(0,5),(0,6),(0,127),(-128,0),(127,5)):
  for mode in (0,1):
   shortcut=gate==0 and 0<=selector<6
   add('signed-gate-%d-%d-%d'%(gate,selector,mode),mode=mode,gate=gate,selector=selector,shortcut=shortcut)
 return out

def compare(code,image,host,kw,coverage,branches):
 a,b=Fixture(image,**kw),Fixture(image,**kw)
 n=Machine(code,a);n.run();host.run(b)
 assert a.trace==b.trace,('trace',a.trace,b.trace)
 for (x,data),(y,other) in zip(a.memory.snapshot(),b.memory.snapshot()):
  assert x==y
  if data!=other:
   off=next(i for i in range(len(data)) if data[i]!=other[i])
   raise AssertionError(('memory',hex(x+off),data[off:off+16].hex(),other[off:off+16].hex()))
 assert (a.created,a.alloc_count,a.node_count)==(b.created,b.alloc_count,b.node_count)
 coverage.update(n.coverage);branches.update(n.branches)
 return a

def build(where,opt,source=None):
 source=source or (PACKET/'candidate.c').read_text()
 (where/'candidate.c').write_text(source);(where/'host.c').write_bytes((PACKET/'host.c').read_bytes())
 out=where/'host.so'
 subprocess.run([shutil.which('cc'),'-std=c99',opt,'-ffp-contract=off','-fPIC','-shared','-Wall','-Wextra','-Werror',str(where/'host.c'),'-o',str(out)],check=True)
 return Host(out)

def run(repo):
 code,image,identities=load(repo);coverage,branches=set(),set();passed=[]
 with tempfile.TemporaryDirectory(prefix='audit-host-',dir=os.environ['TMPDIR']) as temp:
  for opt in ('-O0','-O2'):
   where=Path(temp)/opt;where.mkdir();host=build(where,opt)
   for name,kw in cases():
    try:compare(code,image,host,kw,coverage,branches)
    except Exception as ex:raise AssertionError((name,opt,str(ex))) from ex
    passed.append((opt,name))
  original=(PACKET/'candidate.c').read_text()
  mutations=[
   ('shortcut-not-dependent-on-mode','if (D_80156994 == 0 && D_8014978C >= 0 && D_8014978C < 6)',
    'if (mode != 0 && D_80156994 == 0 && D_8014978C >= 0 && D_8014978C < 6)',dict(mode=0,shortcut=True)),
   ('inclusive-half-threshold','scale >= 0.5f','scale > 0.5f',dict(mode=0,sound=1,scale=.5)),
   ('inclusive-one-threshold','scale >= 1.0f','scale > 1.0f',dict(mode=0,sound=1,scale=1.)),
   ('snapshot-color','D_8012E700[extra->scene].color = color;','D_8012E700[extra->scene].color = D_8011B554; (void)color;',
    dict(mutation=mutate_at(0x80090284,1,[(0x8011B554,0xFFEEDDCC,4)]))),
   ('captured-private-range','scene->scale = range;','scene->scale = D_801239AC;',
    dict(shortcut=True,mutation=mutate_at(0x8008E26C,1,[(0x801239AC,fbits(77.),4),(0x80156994,1,1)]))),
   ('position-third-component','transform->position[2] = ((f32 *)input)[2];','transform->position[2] = ((f32 *)input)[1];',dict(mode=0)),
  ]
  rejected=[]
  for i,(name,before,after,kw) in enumerate(mutations):
   assert original.count(before)==1,(name,original.count(before))
   where=Path(temp)/('mutant%d'%i);where.mkdir();host=build(where,'-O0',original.replace(before,after))
   try:compare(code,image,host,kw,set(),set())
   except AssertionError:rejected.append(name)
   else:raise AssertionError(('mutant survived',name))
 return dict(status='PASS: bounded independent source/native challenges; not strict match or gameplay proof',base=BASE,
             native=identities,fixtures=len(passed),host_optimizations=['-O0','-O2'],
             native_instructions_covered=len(coverage),branch_outcomes=len(branches),mutants_rejected=rejected,
             target_compilations=0,packet={name:hashlib.sha256((PACKET/name).read_bytes()).hexdigest() for name in ['candidate.c','host.c','host_adapter.py','native.py','fixture.py']},
             limitations=['Valid aligned initialized storage and nonaliasing input/global objects','Finite normal-or-zero binary32 under default rounding; no FCSR/exceptions/subnormal arithmetic proof','External effect models rather than complete scene/audio execution','No source matching, original translation-unit admission, publication or CI claims'])
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--packet-dir',type=Path,default=PACKET);p.add_argument('--reference-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 result=run(a.reference_root);a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k not in ('native','packet')},indent=2))
