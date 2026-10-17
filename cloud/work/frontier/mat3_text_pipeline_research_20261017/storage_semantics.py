#!/usr/bin/env python3
"""Bounded native text-storage model; no ROM, graphics or real queue execution."""
from pathlib import Path
import argparse,hashlib,json,os,re,subprocess,tempfile
P=Path(__file__).resolve().parent
FLAGS=['-std=c11','-O2','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fno-strict-aliasing','-fsanitize=undefined,address','-fno-sanitize-recover=all']
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def body(source,name):
 m=re.search(r'void '+name+r'\([^;]*?\)\s*\{',source,re.S)
 if not m:raise RuntimeError('missing real default helper')
 start=m.start();depth=1;i=m.end()
 while depth:
  if source[i]=='{':depth+=1
  elif source[i]=='}':depth-=1
  i+=1
 return source[start:i]+'\n'
def require(v,msg):
 if not v:raise RuntimeError(msg)
def run(group,negative=None):
 source=(group/'text_storage.c').read_text();length=(group/'func_800B66B0.c').read_text();defaults=body((group/'world.c').read_text(),'func_800B6748')
 controls={
  'wrong_capacity':('D_80149DD0 + size >= 800','D_80149DD0 + size > 800'),
  'wrong_spacing_scale':('D_80149DD8[D_80149DD0++] = D_80149B70;','D_80149DD8[D_80149DD0++] = D_80149B70 / 2;'),
  'wrong_coordinate_shift':('(D_80149D92 >> 8) & 255','(D_80149D92 >> 7) & 255'),
  'wrong_depth_boundary':('D_80114744 >= 0.0f','D_80114744 > 0.0f'),
  'lost_full_unlock':('''        du_gfx_unlock();
        return;''','''        return;'''),
 }
 if negative:
  old,new=controls[negative];require(source.count(old)==1,'control site count: '+negative);source=source.replace(old,new)
 # Only host ABI adaptation: the native u32-size memcpy declaration is renamed
 # to a wrapper that calls the host size_t libc function. Native C is unchanged.
 source=source.replace('memcpy','root_memcpy')
 with tempfile.TemporaryDirectory(prefix='text-storage-host-') as t:
  t=Path(t)
  (t/'storage_under_test.c').write_text(source);(t/'length_under_test.c').write_text(length);(t/'defaults_under_test.c').write_text(defaults);(t/'storage_semantics.c').write_bytes((P/'storage_semantics.c').read_bytes())
  c=subprocess.run(['cc',*FLAGS,str(t/'storage_semantics.c'),'-o',str(t/'test')],capture_output=True,text=True)
  require(c.returncode==0,'host compile failed: '+c.stderr)
  r=subprocess.run([str(t/'test')],env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0'),capture_output=True,text=True)
  if negative:
   fixture={'wrong_capacity':5769,'wrong_spacing_scale':1,'wrong_coordinate_shift':1,'wrong_depth_boundary':1921,'lost_full_unlock':5769}[negative]
   require(r.returncode==3 and r.stderr==f'storage differential failure at fixture {fixture}\n' and not r.stdout,'unexpected negative failure: '+negative+' '+r.stderr)
  else:require(r.returncode==0 and r.stdout=='storage semantics passed: 737280 fixtures\n' and r.stderr=='','positive failure: '+r.stderr)
  return {'returncode':r.returncode,'stdout':r.stdout,'stderr':r.stderr}
def main():
 a=argparse.ArgumentParser();a.add_argument('--group',type=Path,default=P/'native_cache_order');a.add_argument('--out',type=Path,default=P/'storage_semantics.json');x=a.parse_args()
 result={'scope':'Finite record bytes/cache/queue-call model. Current settings are stable during each call. Real root, TextLen and SetDefaults execute; graphics and queue I/O are captured. Finished copies have bounded readable input and at most255 copied bytes. No arbitrary string lengths, invalid pointers, concurrent settings updates or real rendering claim.','host_notes':'Native float bit-pun retained under explicit -fno-strict-aliasing. Fatal UBSan and ASan; leak checking disabled because ptrace runtime does not support LeakSanitizer and harness allocates no dynamic memory.','flags':FLAGS,'inputs':{n:sha(x.group/n) for n in ['text_storage.c','func_800B66B0.c','world.c']},'harness_sha256':sha(P/'storage_semantics.c'),'runner_sha256':sha(Path(__file__)),'positive':run(x.group),'negative':{n:run(x.group,n) for n in ['wrong_capacity','wrong_spacing_scale','wrong_coordinate_shift','wrong_depth_boundary','lost_full_unlock']}}
 x.out.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
if __name__=='__main__':main()
