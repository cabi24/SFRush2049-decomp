"""Fresh supplemental replay recovered from the reviewer's original command text."""
import ast,hashlib,json,os,subprocess,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[4]
P=Path(__file__).resolve().parent
assert hashlib.sha256((P/'nonmatch/func_80017D38.c').read_bytes()).hexdigest()=='ad5137579da173b15c903c8aa35c69cd26416d51def3b65267045062543bff65'
assert hashlib.sha256((P/'test_semantics.py').read_bytes()).hexdigest()=='52f4f790b12cd27558a6cea5d7d9af6becff7133f9101933a3eb6ea99e77200e'
tree=ast.parse((P/'test_semantics.py').read_text())
body=next(n.value.value for n in ast.walk(tree) if isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='body' for t in n.targets))
body=body.replace('static unsigned mode,allocations', 'static int override_offsets,override_transpose,override_velocity;\nstatic unsigned mode,allocations',1)
needle=' put(streams[0][current_track],0,(u8)key,(u8)vel,9);'
assert body.count(needle)==1
body=body.replace(needle,' if(override_offsets)for(c=0;c<2;c++)for(i=0;i<64;i++){contexts[c].tracks[i].transpose=(s8)override_transpose;contexts[c].tracks[i].velocity_offset=(s8)override_velocity;}\n'+needle,1)
needle='\n}\nstatic void check('
assert body.count(needle)==1
body=body.replace(needle,'\n if(mode==14){for(c=1;c<=3;c++){i=(current_track+c*17)%64;put(streams[0][i],0,60,100,9);contexts[0].tracks[i].events=(u8*)streams[0][i];}}\n}\nstatic void check(',1)
body=body[:body.index('int main(void)')]+r'''
int main(void){int t,v,wk,wv;unsigned s;
 override_offsets=1;mode=0;
 for(t=-128;t<=127;t++)for(v=-128;v<=127;v++){
  override_transpose=t;override_velocity=v;check(60,100,(unsigned)((t+128)*256+v+128)%64);
  wk=60+t;if(wk<0)wk=0;if(wk>127)wk=127;
  wv=100+v;if(wv<0)wv=0;if(wv>127)wv=127;
  assert(calls==2&&trace[1].kind==2&&trace[1].arg[1]==(u32)wk&&trace[1].arg[2]==(u32)wv);
 }
 override_offsets=0;mode=14;
 for(s=0;s<64;s++){check(60,100,s);assert(allocations==4&&constructors==4&&calls==8);}
 return 0;}
'''
source=P/'nonmatch/func_80017D38.c'
text='#include <assert.h>\n#include <string.h>\n#include "'+str(source)+'"\n'+body
with tempfile.TemporaryDirectory(prefix='17d38-extra-') as d:
 p=Path(d);(p/'test.c').write_text(text)
 flags=['cc','-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror','-O2','-fsanitize=address,undefined','-no-pie']
 subprocess.run(flags+[str(p/'test.c'),'-o',str(p/'test')],check=True)
 subprocess.run([str(p/'test')],check=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1'))
 print(json.dumps(dict(result='PASS',source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),test_source_sha256=hashlib.sha256((P/'test_semantics.py').read_bytes()).hexdigest(),signed_offset_pairs=65536,simultaneous_four_track_cases=64,source_calls=65600,compiler=' '.join(flags),qualification='Temporary harness extends the pinned independent C model; full source-native inspection supports correspondence. This is bounded regression agreement, not a universal semantic proof.'),indent=2))
