"""Research-only resurrection pair: sealed results and optimized refusals."""
from pathlib import Path
import importlib.util
import json
import os
import shutil
import subprocess
import sys
import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/dot_mat3_resurrection_pair_20261017'
REPO = Path(os.environ.get('N64_RESEARCH_REPO', ROOT)).resolve()


def ready():
    ido = Path(os.environ.get('IDO_DIR', REPO / 'tools/cloud/ido'))
    if not (ido / 'cc').is_file():
        if os.environ.get('REQUIRE_TOOLCHAIN') == '1':
            pytest.fail('IDO is required but unavailable')
        pytest.skip('IDO unavailable')


def replay(packet, environment=None):
    return subprocess.run([sys.executable, '-O', str(packet / 'verify.py'), str(REPO)],
                          env=environment, text=True, capture_output=True)


def test_mat3_resurrection_pair():
    ready()
    result = replay(PACKET)
    assert result.returncode == 0, result.stdout + result.stderr
    assert 'caller 17/354' in result.stdout


def test_mat3_resurrection_pair_direct_guards():
    script = '''import copy,importlib.util,json,sys
from pathlib import Path
p=Path(sys.argv[1]);s=importlib.util.spec_from_file_location('pair',p/'verify.py');v=importlib.util.module_from_spec(s);s.loader.exec_module(v)
r=json.loads((p/'receipt.json').read_text());g=json.loads((p/'group/group.json').read_text())
v.verify_report(r);v.verify_scope(g);count=0
def reject(call):
 global count
 try:call()
 except RuntimeError:count+=1;return
 raise RuntimeError('optimized guard accepted altered proof')
changes=[lambda x:x.update(protocol_version=2),lambda x:x.update(status='MATCH'),lambda x:x.update(claims=['camera_follow_path']),lambda x:x.update(new_accepted_bytes=1484),lambda x:x.update(canonical_or_rom_verification=True),lambda x:x['control_results']['natural'].update(func_800C4F68='MATCH'),lambda x:x['control_results']['natural']['extents']['func_800C4F68'].update(bytes=1400),lambda x:x['control_results']['natural'].update(preserved_locked_matches=13),lambda x:x['expected_results']['natural']['inventory']['functions'].update(fake={'offset':0,'bytes':8})]
for change in changes:
 x=copy.deepcopy(r);change(x);reject(lambda:v.verify_report(x))
for name in v.MEMBERS:
 for field,value in [('unverified',['injected']),('differing',1)]:
  x=copy.deepcopy(r);x['expected_results']['natural']['comparisons'][name][field]=value;reject(lambda:v.verify_report(x))
 x=copy.deepcopy(r);x['expected_results']['natural']['extents'][name]['bytes']-=4;reject(lambda:v.verify_report(x))
for change in [lambda x:x.update(claims=['fake']),lambda x:x.update(members=[]),lambda x:x.update(context=[]),lambda x:x.update(files=['fake.c']),lambda x:x.update(keep=[]),lambda x:x.update(flags='-O2'),lambda x:x.update(allow_unverified=True),lambda x:x.update(unprototyped=['fake'])]:
 x=copy.deepcopy(g);change(x);reject(lambda:v.verify_scope(x))
print('optimized direct refusals',count)
'''
    result = subprocess.run([sys.executable, '-O', '-c', script, str(PACKET)],
                            text=True, capture_output=True)
    assert result.returncode == 0, result.stdout + result.stderr
    assert result.stdout == 'optimized direct refusals 53\n'


def test_mat3_resurrection_pair_relocated_refusals(tmp_path):
    ready()
    packet = tmp_path / 'relocated'
    shutil.copytree(PACKET, packet, ignore=shutil.ignore_patterns('__pycache__'))
    result = replay(packet)
    assert result.returncode == 0, result.stdout + result.stderr
    mutations = [
        ('status', 'false research credit', lambda x:x.update(status='MATCH')),
        ('credit', 'false research credit', lambda x:x.update(new_accepted_bytes=1484)),
        ('saved_summary', 'natural research result changed', lambda x:x['control_results']['natural'].update(func_800C4F68='MATCH')),
        ('extent', 'saved preserved extent changed', lambda x:x['expected_results']['natural']['extents']['func_800BFD8C'].update(bytes=848)),
        ('unverified', 'saved preserved match changed', lambda x:x['expected_results']['natural']['comparisons']['func_800BFD8C'].update(unverified=['injected'])),
        ('target', 'target row changed', lambda x:x['target_words_sha256'].update(camera_follow_path='0'*64)),
        ('environment', 'tool or environment changed', lambda x:x['environment_inputs']['compiler'].update(ujoin='0'*64)),
        ('object', 'complete sealed result changed', lambda x:x['expected_results']['natural'].update(object_sha256='0'*64)),
        ('context', 'complete sealed result changed', lambda x:x['expected_results']['natural']['comparisons']['func_800D348C'].update(differing=0)),
        ('inventory', 'complete sealed result changed', lambda x:x['expected_results']['natural']['inventory'].update(text_bytes=4))]
    path = packet / 'receipt.json'
    original = path.read_bytes()
    for name, message, mutate in mutations:
        data = json.loads(original);mutate(data);path.write_text(json.dumps(data))
        result = replay(packet)
        assert result.returncode != 0 and message in result.stderr, (name,result.stdout,result.stderr)
        path.write_bytes(original)
    for name, message in [('group/camera_pair.c','source mutation'),('verify.py','verify mutation'),('README.md','proof input changed')]:
        path = packet / name;original = path.read_bytes();path.write_bytes(original + b'\n')
        result = replay(packet)
        assert result.returncode != 0 and message in result.stderr, (name,result.stdout,result.stderr)
        path.write_bytes(original)
    path = packet / 'trace_fidelity.json';data = json.loads(path.read_text());data['object_sha256']='0'*64;path.write_text(json.dumps(data))
    result = replay(packet)
    assert result.returncode != 0 and 'trace pair changed' in result.stderr, result.stdout+result.stderr

    # Exercise actual scorer output and target inputs, not just saved report edits.
    runner = """import dataclasses,importlib.util,sys
from pathlib import Path
p=Path(sys.argv[1]);repo=Path(sys.argv[2]);mode=sys.argv[3]
sys.path.insert(0,str(repo/'tools/cloud'));import score
s=importlib.util.spec_from_file_location('pair',p/'verify.py');v=importlib.util.module_from_spec(s);s.loader.exec_module(v)
if mode=='target':
 original=score.targets;rows=dict(original());rows['camera_follow_path']=list(rows['camera_follow_path']);rows['camera_follow_path'][0]^=1;score.targets=lambda:rows
else:
 original=score.compare
 def changed(*args,**kwargs):
  row=original(*args,**kwargs);return dataclasses.replace(row,unverified=row.unverified+['injected unsupported relocation'])
 score.compare=changed
v.verify(repo)
"""
    for mode, message in [('target','target row changed'),('unverified','lost preserved match')]:
        result = subprocess.run([sys.executable,'-O','-c',runner,str(PACKET),str(REPO),mode],text=True,capture_output=True)
        assert result.returncode != 0 and message in result.stderr, (mode,result.stdout,result.stderr)


def test_mat3_resurrection_pair_missing_ido(tmp_path):
    result = replay(PACKET, dict(os.environ, IDO_DIR=str(tmp_path/'absent_ido')))
    assert result.returncode != 0 and 'missing: run tools/cloud/setup.sh (or set IDO_DIR)' in result.stderr, result.stdout+result.stderr
    assert 'Traceback' not in result.stderr, result.stderr
