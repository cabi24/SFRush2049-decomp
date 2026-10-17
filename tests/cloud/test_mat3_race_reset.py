"""Complete reset proof with exact ELF extents and optimized integrity refusals."""
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_mat3_physics_20261017/groups/codex_vsync_a145'
spec = importlib.util.spec_from_file_location('race_reset_replay', HERE / 'replay.py')
packet = importlib.util.module_from_spec(spec)
spec.loader.exec_module(packet)


def test_complete_race_reset_packet(tmp_path):
    receipt = json.loads((HERE / 'proof.json').read_text())
    assert packet.inputs() == receipt['inputs']
    for name in ('group.c', 'func_800D6530.c'):
        assert (HERE/name).read_bytes() == (ROOT/'src/blob/groups/codex_vsync_a145'/name).read_bytes()
    compiler = shutil.which('cc')
    if not compiler:
        pytest.skip('host C compiler unavailable')
    exe = tmp_path / 'semantics'
    subprocess.run([compiler, '-std=c11', '-O2', '-Wall', '-Wextra', '-Werror',
                    '-fsanitize=undefined', '-fno-sanitize-recover=undefined',
                    str(HERE/'semantics.c'), '-o', str(exe)], check=True)
    checked = subprocess.run([str(exe)], check=True, capture_output=True, text=True)
    assert checked.stdout == '256 bounded host cases passed\n'
    assert checked.stderr == ''
    if not (packet.score.IDO/'cc').exists():
        pytest.skip('IDO missing for strict compiler replay')
    obj = tmp_path / 'group.o'
    packet.score.compile_group(HERE, obj)
    assert packet.group_result(obj) == receipt['positive']['group_first']
    assert packet.object_layout(obj) == receipt['object_layout']


def test_race_reset_guards_under_optimized_python(tmp_path):
    program = r'''
import copy, importlib.util, json, sys
from pathlib import Path
p=Path(sys.argv[1]);s=importlib.util.spec_from_file_location('reset_guard',p/'replay.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m)
r=json.loads((p/'proof.json').read_text());count=0
def refuse(fn, reason):
    global count
    try:fn()
    except RuntimeError as e:
        if reason not in str(e):raise RuntimeError('wrong refusal: '+str(e))
        count+=1;return
    raise RuntimeError('false proof accepted: '+reason)
for name in m.OLD+[m.NAME]:
    for field,value,reason in [('differing_words',1,'strict result inconsistent'),('extra_nonzero_words',1,'strict result inconsistent'),('unresolved',['bad'],'strict result inconsistent'),('unverified',['bad'],'strict result inconsistent'),('errors',['bad'],'strict result inconsistent'),('target_words',1,'protected target extent')]:
        z=copy.deepcopy(r)
        for rows in z['positive'].values():rows[name][field]=value
        refuse(lambda:m.verify_report(z),reason)
    z=copy.deepcopy(r)
    for label in z['positive']:z['layouts'][label]['function_extents_bytes'][name]+=4
    z['object_layout']=z['layouts']['group_first']
    refuse(lambda:m.verify_report(z),'incomplete ELF extent')
for name in m.OLD:
    z=copy.deepcopy(r);z['negative_controls']['wrong_option_count'][name]['extra_nonzero_words']=1
    refuse(lambda:m.verify_report(z),'strict result inconsistent')
for change,reason in [
    (lambda z:z.update(new_individually_strict_bytes=1024),'credit changed'),
    (lambda z:z.update(accepted_image_bytes=1020),'credit changed'),
    (lambda z:z['claims'].pop(),'complete group claims'),
    (lambda z:z['positive'].pop('group_repeat'),'positive inventory'),
    (lambda z:z['negative_controls'].pop('wrong_option_count'),'negative inventory'),
    (lambda z:z['layouts'].pop('group_repeat'),'layout inventory'),
    (lambda z:z['positive']['group_repeat'].pop(m.OLD[0]),'member inventory'),
    (lambda z:z['layouts']['group_repeat']['emitted_functions'].pop(),'emitted inventory'),
    (lambda z:z['layouts']['group_repeat'].update(root_frame_bytes=64),'native frame'),
    (lambda z:z.update(rom_gate='passed'),'integration scope'),
    (lambda z:z.update(flags='-O2'),'compiler flags'),
]:
    z=copy.deepcopy(r);change(z);refuse(lambda:m.verify_report(z),reason)
for key in ['compiler_executables','verification_tools']:
    z=copy.deepcopy(r);first=next(iter(z['inputs'][key]));z['inputs'][key][first]='0'*64
    refuse(lambda:m.check_saved(z),'saved inputs changed')
for mode in ['source','target','protocol']:
    z=copy.deepcopy(r)
    if mode=='source':z['inputs']['files'][m.SOURCE]='0'*64
    elif mode=='target':z['inputs']['targets'][m.NAME]['words_sha256']='0'*64
    else:z['inputs']['protocol_version']=2
    refuse(lambda:m.check_saved(z),'saved inputs changed')
if count!=71:raise RuntimeError('negative scope changed: '+str(count))
print('71 optimized integrity refusals pass')
'''
    run = subprocess.run([sys.executable, '-O', '-c', program, str(HERE)],
                         capture_output=True, text=True)
    assert run.returncode == 0, run.stdout+run.stderr
    assert run.stdout == '71 optimized integrity refusals pass\n'


def test_race_reset_full_optimized_replay():
    run = subprocess.run([sys.executable, '-O', str(HERE/'replay.py'), '--check'],
                         capture_output=True, text=True)
    assert run.returncode == 0, run.stdout+run.stderr
    assert json.loads(run.stdout) == json.loads((HERE/'proof.json').read_text())


def test_race_reset_missing_compiler_diagnostic(monkeypatch, tmp_path):
    monkeypatch.setattr(packet.score, 'IDO', tmp_path/'missing_ido')
    with pytest.raises(SystemExit, match='run tools/cloud/setup.sh'):
        packet.inputs()
