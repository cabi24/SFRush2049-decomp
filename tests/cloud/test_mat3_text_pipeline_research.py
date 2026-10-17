"""Partial text recovery is reproducible without claiming any new match bytes."""
from pathlib import Path
import shutil,subprocess,sys
ROOT=Path(__file__).resolve().parents[2];HERE=ROOT/'cloud/work/frontier/mat3_text_pipeline_research_20261017'
PRELUDE='''import copy,importlib.util,json,sys
from pathlib import Path
p=Path(sys.argv[1]);sys.path.insert(0,str(p));spec=importlib.util.spec_from_file_location('text_verify',p/'verify.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
r=json.loads((p/'verification.json').read_text())
'''
def run(source,packet=HERE):
 p=subprocess.run([sys.executable,'-O','-c',PRELUDE+source,str(packet)],cwd=ROOT,capture_output=True,text=True)
 assert p.returncode==0,(p.stdout,p.stderr)
 return p.stdout

def test_text_pipeline_optimized_full_replay(tmp_path):
 assert run(f"m.verify(Path({str(tmp_path)!r}));print('complete replay passes')\n")=='complete replay passes\n'

def test_text_pipeline_optimized_refusals():
 script='''
count=0
def refuse(fn):
 global count
 try:fn()
 except RuntimeError:count+=1;return
 raise RuntimeError('false receipt was accepted')
m.verify_report(r);m.verify_scope(json.loads((p/'group/group.json').read_text()))
for name in r['rows']:
 for key,value in [('differing',r['rows'][name]['comparison']['differing']+1),('total',r['rows'][name]['comparison']['total']+1),('extra_words',r['rows'][name]['comparison']['extra_words']+1),('unresolved',['bad']),('unverified',r['rows'][name]['comparison']['unverified']+['bad']),('errors',['bad'])]:
  z=copy.deepcopy(r);z['rows'][name]['comparison'][key]=value;refuse(lambda:m.verify_report(z))
 z=copy.deepcopy(r);z['rows'][name]['symbol_bytes']+=4;refuse(lambda:m.verify_report(z))
for change in [lambda x:x.update(claims=['audio_doppler_calc']),lambda x:x.update(new_candidate_bytes=152),lambda x:x.update(accepted_image_bytes=152),lambda x:x.update(installable=True),lambda x:x.update(canonical_or_ROM_verification=True),lambda x:x['rows'].pop('func_800B66B0'),lambda x:x['inventory']['functions'].pop('du_gfx_lock'),lambda x:x['inventory']['functions']['du_gfx_unlock'].update(bytes=4),lambda x:x['inventory'].update(zero_alignment_bytes=0),lambda x:x['renderer'].update(fixtures=0),lambda x:x['renderer'].update(claims=['audio_doppler_calc']),lambda x:x['renderer'].update(cfg_unreachable=[]),lambda x:x['renderer_negatives']['wrong_alpha_flag'].update(fixture=0),lambda x:x['storage']['positive'].update(returncode=3),lambda x:x['storage']['negative']['wrong_capacity'].update(stderr='generic error'),lambda x:x['layout'].update(short_color=x['layout']['positive']),lambda x:x['one_caller_control']['audio_doppler_calc'].update(symbol_bytes=1124)]:
 z=copy.deepcopy(r);change(z);refuse(lambda:m.verify_report(z))
s=json.loads((p/'group/group.json').read_text())
for change in [lambda x:x.update(claims=['func_800B66B0']),lambda x:x['members'].pop(),lambda x:x['files'].append('extra.c'),lambda x:x['keep'].pop(),lambda x:x.update(flags='-O2'),lambda x:x.update(allow_unverified=True),lambda x:x.update(unprototyped=['osRecvMesg'])]:
 z=copy.deepcopy(s);change(z);refuse(lambda:m.verify_scope(z))
for change in [lambda x:x.update(object_sha256='0'*64),lambda x:x['call_graph']['external'].append('false'),lambda x:x['rows']['audio_doppler_calc']['comparison']['notes'].append('false'),lambda x:x['inputs'].update(protected_own_data_manifest_sha256='0'*64)]:
 z=copy.deepcopy(r);change(z);refuse(lambda:m.verify_seal(z,r))
print('refusals',count)
'''
 assert run(script)=='refusals 301\n'

def test_text_pipeline_relocated_integrity(tmp_path):
 root=tmp_path/'relocated';packet=root/'cloud/work/frontier'/HERE.name;packet.parent.mkdir(parents=True);shutil.copytree(HERE,packet,ignore=shutil.ignore_patterns('__pycache__'))
 for path in ['tools/cloud','asm/us/blob','asm/us/blob_data','tests/cloud/test_mat3_text_pipeline_research.py','blob_matched.lock.json']:
  src=ROOT/path;dst=root/path
  if not src.exists():continue
  dst.parent.mkdir(parents=True,exist_ok=True)
  if src.is_dir():shutil.copytree(src,dst,ignore=shutil.ignore_patterns('__pycache__','ido'))
  else:shutil.copyfile(src,dst)
 assert run(f"m.verify(Path({str(tmp_path/'replay')!r}));print('relocated replay passes')\n",packet)=='relocated replay passes\n'
 # Real entry-point negatives check sealed inputs and trace binding before compilation.
 for name,update in [('verification.json',lambda x:x['inputs']['files'].update({'README.md':'0'*64})),('trace_fidelity.json',lambda x:x.update(stock_object_sha256='0'*64)),('trace_fidelity.json',lambda x:x['inputs']['files'].update({'README.md':'0'*64}))]:
  import json
  file=packet/name;old=file.read_bytes();obj=json.loads(old);update(obj);file.write_text(json.dumps(obj))
  assert run(f"\ntry:m.verify(Path({str(tmp_path/'refusal')!r}))\nexcept RuntimeError:print('rejected')\nelse:raise RuntimeError('accepted altered binding')\n",packet)=='rejected\n'
  file.write_bytes(old)
