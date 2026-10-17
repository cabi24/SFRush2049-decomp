"""Additive genuine text-placement boundary, partial source and finite semantics."""
from pathlib import Path
import importlib.util
import json
import os
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT/'cloud/work/frontier/dot_mat3_tb_draw_addendum_20261017'


def run(packet, *args, environment=None):
    return subprocess.run([sys.executable, '-O', str(packet/'replay.py'), *args],
                          env=environment, capture_output=True, text=True)


def test_tb_draw_full_optimized_replay():
    result = run(PACKET)
    assert result.returncode == 0, result.stdout+result.stderr
    assert result.stdout == 'PASS partial research: 43 native bodies, 37 strict, 14/99 root, full Just data; zero credit\n'


def test_tb_draw_optimized_integrity_guards():
    script = r'''
from pathlib import Path
import copy,importlib.util,json,sys
p=Path(sys.argv[1]);s=importlib.util.spec_from_file_location('packet',p/'replay.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m)
r=json.loads((p/'proof.json').read_text());m.verify_report(r);bound=m.inputs();m.inputs=lambda:bound;count=0

def refuse(change,reason):
 global count
 z=copy.deepcopy(r);change(z)
 try:m.verify_report(z)
 except RuntimeError as e:
  if reason not in str(e):raise RuntimeError('wrong refusal '+str(e))
  count+=1;return
 raise RuntimeError('false proof accepted '+reason)

for label in m.VARIANTS:
 names=m.inherited()['native_members'] if label=='baseline' else m.recipe()['members']
 for name in names:
  if name in m.scope()['strict']:
   refuse(lambda z:z['builds'][label]['rows'][name].update(differing=1),'strict')
   refuse(lambda z:z['builds'][label]['rows'][name].update(unverified=['injected']),'strict')
   refuse(lambda z:z['builds'][label]['rows'][name].update(bytes=4),'extent')
  else:
   refuse(lambda z:z['builds'][label]['rows'][name].update(differing=1),'partial')
  refuse(lambda z:z['builds'][label]['rows'][name].update(total=1),'protected extent')
 refuse(lambda z:z['builds'][label]['rows'].pop('du_gfx_lock'),'inventory')
 refuse(lambda z:z['builds'][label]['rows']['du_gfx_lock'].update(bytes=12),'residue')
for key,value in [('differing',1),('extra_words',1),('unresolved',['bad']),('unverified',['bad']),('errors',['bad'])]:
 refuse(lambda z:[z['builds'][label]['rows']['object_manager_update'].update({key:value}) for label in ['first','repeat']],'strict')
for change,reason in [
 (lambda z:z.update(claims=[m.NAME]),'credit'),
 (lambda z:z.update(new_candidate_bytes=396),'credit'),
 (lambda z:z.update(accepted_image_bytes=396),'credit'),
 (lambda z:z['builds']['first']['rows'][m.NAME].update(frames=[32]),'partial'),
 (lambda z:z['builds'].pop('private_just'),'inventory'),
 (lambda z:z['layout'].update(native=[4,0,4]),'layout'),
 (lambda z:z['layout'].update(wrong_capacity=[8,0,4]),'layout'),
 (lambda z:z.update(wrong_data='any exception'),'negative'),
 (lambda z:z['just_data']['first'].update(checked_bytes=4),'Just proof'),
 (lambda z:z['just_data']['first'].update(sha256='0'*64),'Just proof'),
 (lambda z:z['just_data']['first'].update(native_equal=False),'Just proof'),
 (lambda z:z['semantics'].update(fixtures=1),'scope'),
 (lambda z:z['semantics']['results']['wrong_draw_bound'].update(returncode=1),'negative'),
 (lambda z:z['semantics']['results']['positive'].update(stderr='runtime error'),'positive'),
 (lambda z:z['inputs']['compiler'].update(ujoin='0'*64),'binding'),
 (lambda z:z['inputs']['inherited_sources'].update({'text_storage.c':'0'*64}),'binding'),
 (lambda z:z['inputs']['targets'][m.NAME].update(sha256='0'*64),'binding'),
]:refuse(change,reason)
if m.frames([0,0x27bdffe8],0,4):raise RuntimeError('frame scan escaped function')
if m.frames([0x27bdffe8,0],0,4)!=[24]:raise RuntimeError('frame scan lost native frame')
print('optimized precise refusals',count)
'''
    result = subprocess.run([sys.executable, '-O', '-c', script, str(PACKET)], capture_output=True, text=True)
    assert result.returncode == 0, result.stdout+result.stderr
    assert result.stdout == 'optimized precise refusals 1142\n'


def test_tb_draw_relocated_source_seal(tmp_path):
    target = tmp_path/'relocated'
    shutil.copytree(PACKET, target, ignore=shutil.ignore_patterns('__pycache__'))
    env = dict(os.environ, N64_RESEARCH_REPO=str(ROOT))
    result = run(target, environment=env)
    assert result.returncode == 0, result.stdout+result.stderr
    path = target/'tb_draw.c'
    path.write_text(path.read_text()+'\n/* stale source */\n')
    result = run(target, environment=env)
    assert result.returncode != 0 and 'saved input binding changed' in result.stderr, result.stdout+result.stderr


def test_tb_draw_missing_ido_diagnostic(tmp_path):
    result = run(PACKET, environment=dict(os.environ, IDO_DIR=str(tmp_path/'missing')))
    assert result.returncode != 0 and 'run tools/cloud/setup.sh' in result.stderr, result.stdout+result.stderr
    assert 'Traceback' not in result.stderr
