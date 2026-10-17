"""Genuine-helper complete proof and optimized integrity controls."""
import importlib.util
import json
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_mat3_project_window_20261017'
spec = importlib.util.spec_from_file_location('mat3_project_window_packet', HERE / 'replay.py')
packet = importlib.util.module_from_spec(spec)
spec.loader.exec_module(packet)


def test_project_window_source_and_receipt():
    packet.verify_source_change()
    packet.verify_report(json.loads((HERE / 'proof.json').read_text()))


def test_project_window_guards_under_optimized_python():
    program = r'''
from pathlib import Path
import copy,importlib.util,json,sys
p=Path(sys.argv[1]);s=importlib.util.spec_from_file_location('v',p/'replay.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m)
r=json.loads((p/'proof.json').read_text());m.verify_report(r);count=0

def refusal(change, reason):
    global count
    z=copy.deepcopy(r);change(z)
    try:m.verify_report(z)
    except RuntimeError as e:
        if reason not in str(e):raise RuntimeError('wrong refusal: '+str(e))
        count+=1;return
    raise RuntimeError('false proof accepted: '+reason)

# Mutate every repeated positive row together: repeat equality alone cannot
# reject an inconsistent strict boolean or an incorrect complete extent.
for n in m.STRICT:
    for key,value in [('differing_words',1),('extra_nonzero_words',1),('unresolved',['bad']),('unverified',['bad']),('errors',['bad']),('strict_match',False)]:
        refusal(lambda z:[g['scores'][n].update({key:value}) for g in z['positive'].values()], 'strict')
    refusal(lambda z:[g['scores'][n].update(target_words=1) for g in z['positive'].values()], 'word count')
    refusal(lambda z:[g['layout']['function_extents'][n].update(bytes=4) for g in z['positive'].values()], 'extent')
for change,reason in [
    (lambda z:[g['layout'].update(root_frame_bytes=64) for g in z['positive'].values()], 'frame'),
    (lambda z:[g['scores'].pop(m.NAME) for g in z['positive'].values()], 'score inventory'),
    (lambda z:[g['layout']['emitted_functions'].pop() for g in z['positive'].values()], 'function inventory'),
    (lambda z:[g['layout']['function_extents'].pop(m.NAME) for g in z['positive'].values()], 'function inventory'),
    (lambda z:z.update(new_individually_strict_bytes=452), 'byte credit'),
    (lambda z:z.update(accepted_image_bytes=448), 'byte credit'),
    (lambda z:z['preserved_helpers'].pop(), 'helper claims'),
    (lambda z:z.update(whole_unit_gate='pass'), 'integration'),
    (lambda z:z.update(image_gate='pass'), 'integration'),
    (lambda z:z.update(rom_gate='pass'), 'integration'),
    (lambda z:z['positive'].pop('repeat'), 'build inventory'),
    (lambda z:z['negative_controls'].pop('wrong_depth'), 'build inventory'),
    (lambda z:z.update(host_semantics='one case'), 'semantic'),
    (lambda z:z.update(host_semantic_control='any failure'), 'semantic'),
]:refusal(change,reason)
for label in m.NEGATIVES:
    for change,reason in [
        (lambda g:g['scores'][m.NAME].update(strict_match=True), 'strict'),
        (lambda g:g['scores'][m.NAME].update(target_words=113), 'word count'),
        (lambda g:g['layout']['function_extents'][m.NAME].update(bytes=4), 'extent'),
        (lambda g:g['scores']['func_800A61B0'].update(differing_words=1), 'strict'),
        (lambda g:g['layout']['function_extents']['func_800A61B0'].update(bytes=144), 'extent'),
        (lambda g:g['layout'].update(root_frame_bytes=8), 'frame'),
        (lambda g:g['scores'][m.NAME].update(differing_words=g['scores'][m.NAME]['differing_words']+1), 'negative outcome'),
        (lambda g:g['scores'][m.NAME]['unverified'].append('bad'), 'negative outcome'),
        (lambda g:g['scores'][m.NAME]['errors'].append('bad'), 'negative outcome'),
        (lambda g:g['scores'][m.NAME].update(extra_nonzero_words=g['scores'][m.NAME]['extra_nonzero_words']+1), 'negative outcome'),
        (lambda g:g['scores'][m.NAME]['unresolved'].append('bad'), 'negative outcome'),
    ]:refusal(lambda z:change(z['negative_controls'][label]),reason)
for change in [
    lambda z:z['inputs']['compiler_sha256'].update(cc='0'*64),
    lambda z:z['inputs']['tooling_sha256'].update({'tools/cloud/score.py':'0'*64}),
    lambda z:z['inputs']['packet_sha256'].update({'candidate.c':'0'*64}),
    lambda z:z['inputs']['packet_sha256'].update({'README.md':'0'*64}),
    lambda z:z['inputs']['targets'][m.NAME].update(sha256='0'*64),
    lambda z:z['inputs']['recipe'].update(claims=[]),
    lambda z:z['inputs']['recipe'].update(flags='-O2'),
]:refusal(change,'binding')
if count!=111:raise RuntimeError('unexpected refusal count: '+str(count))
if m.root_frame([0,0x27bdffb8],0,4) is not None:raise RuntimeError('frame scan escaped root extent')
if m.root_frame([0x27bdffb8,0],0,4)!=72:raise RuntimeError('in-extent frame lost')
print('111 optimized integrity refusals and bounded frame controls pass')
'''
    r = subprocess.run([sys.executable, '-O', '-c', program, str(HERE)], capture_output=True, text=True)
    assert r.returncode == 0, r.stdout+r.stderr
    assert r.stdout == '111 optimized integrity refusals and bounded frame controls pass\n'


def test_project_window_optimized_stock_compiler_replay():
    r = subprocess.run([sys.executable, '-O', str(HERE/'replay.py'), '--check'], capture_output=True, text=True)
    assert r.returncode == 0, r.stdout+r.stderr
    assert json.loads(r.stdout) == json.loads((HERE/'proof.json').read_text())


def test_project_window_missing_ido_diagnostic(tmp_path):
    program = r'''
from pathlib import Path
import importlib.util,sys
p=Path(sys.argv[1]);s=importlib.util.spec_from_file_location('v',p/'replay.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m)
m.score.IDO=Path(sys.argv[2])
try:m.inputs()
except SystemExit as e:
    if 'run tools/cloud/setup.sh' not in str(e):raise
else:raise RuntimeError('missing IDO accepted')
print('standard missing-IDO diagnostic passes')
'''
    r = subprocess.run([sys.executable, '-O', '-c', program, str(HERE), str(tmp_path/'missing_ido')], capture_output=True, text=True)
    assert r.returncode == 0, r.stdout+r.stderr
    assert r.stdout == 'standard missing-IDO diagnostic passes\n'
