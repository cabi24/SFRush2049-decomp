"""Complete Init_MDrive proof with optimized guards and bounded extents."""
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_mat3_physics_20261017/mdrive'
spec = importlib.util.spec_from_file_location('init_mdrive_replay', HERE / 'replay.py')
packet = importlib.util.module_from_spec(spec)
spec.loader.exec_module(packet)


def test_complete_init_mdrive_packet(tmp_path):
    receipt = json.loads((HERE / 'proof.json').read_text())
    packet.check_saved(receipt)
    compiler = shutil.which('cc')
    if not compiler:
        pytest.skip('host C compiler unavailable')
    assert packet.semantic_check(tmp_path) == receipt['host_semantics']
    if not (packet.score.IDO / 'cc').exists():
        pytest.skip('IDO missing for strict compiler replay')
    # compile_group uses a unique private cwd, so the -O3 intermediate names
    # cannot race another pytest worker. Full control replay stays in replay.py.
    obj = tmp_path / 'group.o'
    packet.score.compile_group(HERE, obj)
    assert packet.result(obj) == receipt['positive']['group_first']


def test_init_mdrive_optimized_guards():
    program = r'''
import copy,importlib.util,json,sys
from pathlib import Path
p=Path(sys.argv[1]);s=importlib.util.spec_from_file_location('packet',p/'replay.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m)
r=json.loads((p/'proof.json').read_text());m.check_saved(r);count=0
def reject(fn):
 global count
 try:fn()
 except RuntimeError:count+=1;return
 raise RuntimeError('verification accepted an invalid proof')
for category in ['positive','negative_controls']:
 for label in r[category]:
  row=r[category][label]
  for field,value in [('strict_match',not row['strict_match']),('differing_words',row['differing_words']+1),('target_words',176),('frame_bytes',80),('extra_nonzero_words',1),('unresolved',['bad']),('unverified',['bad']),('errors',['bad'])]:
   z=copy.deepcopy(r);z[category][label][field]=value;reject(lambda:m.verify_report(z))
  for change in [lambda z:z['functions'].update({m.NAME:704}),lambda z:z['functions'].update({'invented_helper':4}),lambda z:z.update(root_frame_bytes=80)]:
   z=copy.deepcopy(r);change(z['layouts'][label]);reject(lambda:m.verify_report(z))
for change in [
 lambda z:z.update(claims=[]),lambda z:z.update(name='wrong'),
 lambda z:z.update(target_bytes=704),lambda z:z.update(individually_strict_bytes=704),
 lambda z:z.update(accepted_image_bytes=700),lambda z:z.update(image_gate='passed'),
 lambda z:z.update(rom_gate='passed'),lambda z:z.update(flags='-O2'),
 lambda z:z.update(as1_extra_flag=''),lambda z:z['positive'].pop('single'),
 lambda z:z['negative_controls'].pop('wrong_velocity_threshold'),
 lambda z:z['layouts'].pop('group_repeat'),lambda z:z.update(host_semantics='wrong'),
 lambda z:z['inputs']['recipe'].update(context=['invented']),
 lambda z:z['inputs']['recipe'].update(keep=[]),
 lambda z:z['inputs']['recipe'].update(allow_unverified=True),
]:
 z=copy.deepcopy(r);change(z);reject(lambda:m.verify_report(z))
for section in ['files','compiler_executables','verification_tools']:
 z=copy.deepcopy(r);key=next(iter(z['inputs'][section]));z['inputs'][section][key]='0'*64;reject(lambda:m.check_saved(z))
for change in [lambda z:z.update(protocol_version=2),lambda z:z['targets'][m.NAME].update(size=704),lambda z:z['references'].update({'injected':0})]:
 z=copy.deepcopy(r);change(z['inputs']);reject(lambda:m.check_saved(z))
z=copy.deepcopy(r);z['inputs']['compiler_executables']['ujoin']='0'*64;reject(lambda:m.check_saved(z))
if count!=111:raise RuntimeError('mutation coverage changed: '+str(count))
print('111 optimized verification refusals pass')
'''
    run = subprocess.run([sys.executable, '-O', '-c', program, str(HERE)],capture_output=True,text=True)
    assert run.returncode == 0, run.stdout+run.stderr
    assert run.stdout == '111 optimized verification refusals pass\n'


def test_init_mdrive_full_optimized_replay():
    run = subprocess.run([sys.executable,'-O',str(HERE/'replay.py'),'--check'],capture_output=True,text=True)
    assert run.returncode == 0, run.stdout+run.stderr
    assert json.loads(run.stdout) == json.loads((HERE/'proof.json').read_text())


def test_init_mdrive_missing_compiler_diagnostic(monkeypatch,tmp_path):
    monkeypatch.setattr(packet.score,'IDO',tmp_path/'missing_ido')
    with pytest.raises(SystemExit,match='run tools/cloud/setup.sh'):
        packet.inputs()
