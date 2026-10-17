"""One source/data-bound test; optional fresh stock IDO replay of the full group."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import pytest
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/mat3_sound_init_match_20261017'

def test_sound_init_complete_group_and_controls(tmp_path):
    spec=importlib.util.spec_from_file_location('sound_init_replay',PACKET/'replay.py')
    replay=importlib.util.module_from_spec(spec);spec.loader.exec_module(replay)
    receipt=json.loads((PACKET/'verification.json').read_text())
    assert replay.source_bindings()==receipt['source_sha256']
    assert replay.target_bindings()==receipt['target_sha256']
    assert receipt['new_code_bytes']==1048 and receipt['prior_members']==24
    candidate=receipt['results']['candidate']
    assert len(candidate['all_members'])==25
    assert all(replay.complete(r) for r in candidate['all_members'].values())
    assert candidate['unclaimed_helper_residue_bytes']==8
    assert receipt['once_flag']['logical_bytes']==1
    assert receipt['once_flag']['verified_window_bytes']==4
    assert receipt['results']['without_count_conversion']['hud_setup']['differing']==24
    assert receipt['results']['external_once_flag']['hud_setup']['differing']==8
    assert set(receipt['results']['static_list_helper']['failed_old_members'])=={'func_800A370C'}
    for label in ['uninitialized_local_flag','wrong_initial_flag']:
        assert receipt['results'][label]['hud_setup']['unverified']
    ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))
    if not (ido/'cc').is_file() or not shutil.which('mips-linux-gnu-objdump'):
        pytest.skip('IDO and MIPS binutils required for fresh replay; source/data receipt checks passed')
    assert replay.verify(tmp_path)==receipt

def test_sound_init_guards_survive_optimized_python(tmp_path):
    import subprocess,sys
    program=r'''
import contextlib,copy,importlib.util,io,json,sys
from pathlib import Path
p,out=Path(sys.argv[1]),Path(sys.argv[2]);s=importlib.util.spec_from_file_location('sound_replay',p/'replay.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m)
r=json.loads((p/'verification.json').read_text());trace=json.loads((p/'compiler-decisions.json').read_text());count=0
def rejected(fn,reason):
 global count
 try:
  with contextlib.redirect_stdout(io.StringIO()):fn()
 except RuntimeError as e:
  if reason not in str(e):raise RuntimeError('wrong refusal: '+str(e))
  count+=1;return
 raise RuntimeError('accepted invalid proof: '+reason)
for name in [m.FN,'func_800A370C']:
 for key,value in [('differing',1),('extra_words',1),('unresolved',['bad']),('unverified',['bad']),('errors',['bad']),('symbol_bytes',1),('total',1),('native_bytes',1)]:
  bad=copy.deepcopy(r);row=bad['results']['candidate']['all_members'][name];row[key]=value
  if name==m.FN:bad['results']['candidate']['hud_setup']=copy.deepcopy(row)
  reason='native extent changed' if key in ['total','native_bytes'] else 'strict member changed'
  rejected(lambda:m.verify_report(bad),reason)
for field,value,reason in [('claims',[],'claim inventory changed'),('new_code_bytes',1056,'false credit'),('accepted_image_bytes',1048,'false credit')]:
 bad=copy.deepcopy(r);bad[field]=value;rejected(lambda:m.verify_report(bad),reason)
for action,reason in [
 (lambda b:b['results']['candidate'].update(unclaimed_helper_residue_bytes=0),'unclaimed helper residue changed'),
 (lambda b:b['results']['candidate']['context'].pop('CreatePlayer'),'complete inventory changed'),
 (lambda b:b['once_flag'].update(logical_bytes=4),'once-flag contract changed'),
 (lambda b:b['results']['wrong_initial_flag']['all_members'][m.FN].update(unverified=[]),'root duplicate changed'),
]:
 bad=copy.deepcopy(r);action(bad);rejected(lambda:m.verify_report(bad),reason)
for action,reason in [
 (lambda b:b['candidate'].update(allocation_decisions=[]),'empty allocation observation'),
 (lambda b:b['without_count_conversion'].update(source_sha256='0'*64),'trace variant source changed'),
 (lambda b:b['candidate']['inputs_sha256'].update({'replay.py':'0'*64}),'trace input binding changed'),
 (lambda b:b['candidate'].update(stock_object_sha256='0'*64),'trace object binding changed'),
 (lambda b:b['candidate'].update(traced_object_sha256='0'*64),'trace object binding changed'),
 (lambda b:b['candidate'].update(complete_stock_object_identity=False),'trace object binding changed'),
 (lambda b:b['candidate']['allocation_decisions'][0].update(forced='1'),'forced compiler observation'),
]:
 bad=copy.deepcopy(trace);action(bad);rejected(lambda:m.verify_trace_bindings(bad,r),reason)
rejected(lambda:m.check_trace_identity(b'stock',b'changed'),'instrumentation changed output')
original=m.source_bindings;m.source_bindings=lambda:{}
try:rejected(lambda:m.verify(out/'source'),'source receipt changed')
finally:m.source_bindings=original
original=m.target_bindings;m.target_bindings=lambda:{}
try:rejected(lambda:m.verify(out/'target'),'target receipt changed')
finally:m.target_bindings=original
original=Path.read_text
for mode,reason in [('claims','claim inventory changed'),('object','fresh sealed receipt differs'),('observations','compiler observation receipt changed')]:
 def changed(path,*a,**kw):
  text=original(path,*a,**kw)
  if mode=='claims' and path==m.GROUP/'group.json':
   data=json.loads(text);data['claims']=[];return json.dumps(data)
  if mode=='observations' and path==m.HERE/'verification.json':
   data=json.loads(text);data['compiler_observations_sha256']='0'*64;return json.dumps(data)
  if mode=='object' and path==m.HERE/'verification.json':
   data=json.loads(text);data['results']['candidate']['object_sha256']='0'*64;return json.dumps(data)
  return text
 Path.read_text=changed
 try:rejected(lambda:m.verify(out/mode),reason)
 finally:Path.read_text=original
if count!=36:raise RuntimeError('refusal count changed: '+str(count))
print('36 optimized refusal cases pass')
'''
    result=subprocess.run([sys.executable,'-O','-c',program,str(PACKET),str(tmp_path)],capture_output=True,text=True)
    assert result.returncode==0,result.stdout+result.stderr
    assert result.stdout=='36 optimized refusal cases pass\n'

def test_sound_init_relocated_packet(tmp_path):
    relocated=tmp_path/'different-root'/'packet'
    shutil.copytree(PACKET,relocated,ignore=shutil.ignore_patterns('__pycache__'))
    spec=importlib.util.spec_from_file_location('sound_relocated',PACKET/'replay.py')
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
    module.HERE=relocated;module.GROUP=relocated/'groups/menu_save_options'
    assert module.verify(tmp_path/'objects')==json.loads((relocated/'verification.json').read_text())
