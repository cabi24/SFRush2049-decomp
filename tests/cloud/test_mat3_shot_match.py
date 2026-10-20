"""Full Shot closure, complete extents and optimized refusal controls."""
import importlib.util
import json
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_mat3_shot_match_20261017'
spec = importlib.util.spec_from_file_location('mat3_shot_match_packet', HERE / 'replay.py')
packet = importlib.util.module_from_spec(spec)
spec.loader.exec_module(packet)


def test_shot_match_source_and_receipt():
    packet.verify_source_change()
    packet.verify_report(json.loads((HERE / 'proof.json').read_text()))


def test_shot_match_guards_under_optimized_python():
    program = r'''
from pathlib import Path
from types import SimpleNamespace
import copy,importlib.util,json,sys
p=Path(sys.argv[1]);s=importlib.util.spec_from_file_location('v',p/'replay.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m)
r=json.loads((p/'proof.json').read_text());m.verify_report(r);count=0
# Reuse once-verified immutable inputs only within mutation tests. Fresh CLI
# replay below independently fingerprints the actual tools and native inputs.
bound=m.inputs();m.inputs=lambda:copy.deepcopy(bound)

def refusal(change, reason):
    global count
    z=copy.deepcopy(r);change(z)
    try:m.verify_report(z)
    except RuntimeError as e:
        if reason not in str(e):raise RuntimeError('wrong refusal: '+str(e))
        count+=1;return
    raise RuntimeError('false proof accepted: '+reason)

for name in m.strict_names()+['func_800AC9BC']:
    for key,value in [('differing_words',999),('extra_nonzero_words',1),('unresolved',['bad']),('unverified',['bad']),('errors',['bad']),('strict_match',not r['positive']['first']['scores'][name]['strict_match'])]:
        refusal(lambda z:[row['scores'][name].update({key:value}) for row in z['positive'].values()], 'strict' if name!='func_800AC9BC' or key=='strict_match' else 'score outcome')
    refusal(lambda z:[row['scores'][name].update(target_words=1) for row in z['positive'].values()], 'word count')
for name in m.EXPECTED_FUNCTIONS:
    for key,value in [('bytes',4),('frames',[4])]:
        refusal(lambda z:[z['complete_layouts'][label]['functions'][name].update({key:value}) for label in ['first','repeat']], 'extent/frame')
for label in m.ROOT_DIFFERENCES:
    # Historical and meaningful-state outputs are deliberately compared for
    # identity; mutate both together so the semantic score guards are exercised.
    labels=['historical','meaningful_state'] if label in ['historical','meaningful_state'] else [label]
    for name in m.strict_names()+['func_800AC9BC']:
        for key,value,reason in [('target_words',1,'word count'),('differing_words',999,'strict' if r['negative_controls'][label]['scores'][name]['strict_match'] else 'score outcome'),('unverified',['bad'],'strict' if r['negative_controls'][label]['scores'][name]['strict_match'] else 'score outcome')]:
            refusal(lambda z:[z['negative_controls'][k]['scores'][name].update({key:value}) for k in labels],reason)
    for name in r['complete_layouts'][label]['functions']:
        refusal(lambda z:z['complete_layouts'][label]['functions'][name].update(bytes=4),'extent/frame')
for change,reason in [
    (lambda z:z.update(new_individually_strict_bytes=3584),'credit'),
    (lambda z:z.update(accepted_image_bytes=3580),'credit'),
    (lambda z:z['preserved_members'].pop(),'credit'),
    (lambda z:z.update(whole_unit_gate='pass'),'integration'),
    (lambda z:z.update(image_gate='pass'),'integration'),
    (lambda z:z.update(rom_gate='pass'),'integration'),
    (lambda z:z['positive'].pop('repeat'),'build inventory'),
    (lambda z:z['negative_controls'].pop('historical'),'build inventory'),
    (lambda z:z['complete_layouts'].pop('first'),'layout inventory'),
    (lambda z:[v['scores'].pop(m.NAME) for v in z['positive'].values()],'score inventory'),
    (lambda z:[z['complete_layouts'][k]['functions'].pop('lsq') for k in ['first','repeat']],'extent/frame'),
    (lambda z:[z['complete_layouts'][k]['gaps'].append(dict(offset=0,bytes=4,nonzero_words=1)) for k in ['first','repeat']],'residue'),
    (lambda z:z['complete_layouts']['unbounded_static_residue'].update(gaps=[]),'residue'),
    (lambda z:[z['complete_layouts'][k].update(text_bytes=4) for k in ['first','repeat']],'text extent'),
    (lambda z:[v['layout'].update(root_frame_bytes=4) for v in z['positive'].values()],'legacy'),
    (lambda z:z.update(host_semantics='one case'),'semantic'),
    (lambda z:z.update(host_semantic_control='arbitrary failure'),'semantic'),
    (lambda z:z.update(fp_scope='all floating-point inputs'),'floating-point'),
    (lambda z:z['complete_layouts']['repeat'].update(object_sha256='0'*64),'repeat'),
]:refusal(change,reason)
for name in bound['compiler_sha256']:
    refusal(lambda z:z['inputs']['compiler_sha256'].update({name:'0'*64}),'binding')
for key in ['packet_sha256','context_sha256','tooling_sha256','prepared_sha256','target_metadata']:
    name=next(iter(bound[key]));refusal(lambda z:z['inputs'][key].update({name:'0'*64}),'binding')
for change in [
    lambda z:z['inputs'].update(owned_manifest_sha256='0'*64),
    lambda z:z['inputs']['targets'][m.NAME].update(sha256='0'*64),
    lambda z:z['inputs']['recipe'].update(claims=[]),
    lambda z:z['inputs']['recipe'].update(flags='-O2'),
]:refusal(change,'binding')
original=m.recipe;m.recipe=lambda:dict(original(),claims=[])
try:m.verify_report(r)
except RuntimeError as e:
    if 'source/context/claim recipe' not in str(e):raise
    count+=1
else:raise RuntimeError('changed source recipe accepted')
m.recipe=original
if m.frame_scan([0,0x27bdffb8],0,4)!=[]:raise RuntimeError('frame scan escaped root extent')
if m.frame_scan([0x27bdffb8,0],0,4)!=[72]:raise RuntimeError('in-extent frame lost')
d=Path('/tmp/isolated-shot-model');expected=f"semantics: {d/'semantics.c'}:80: compare: Assertion `memcmp(&a,&b,sizeof a)==0' failed.\n"
m.verify_semantic_negative(SimpleNamespace(returncode=-6,stdout='',stderr=expected),d)
for code,out,err in [(1,'',expected),(-6,'unexpected',expected),(-6,'','runtime error: signed overflow\n'),(-6,'',expected.replace(':80:',':81:')),(-6,'',expected+'extra\n'),(0,'','')]:
    try:m.verify_semantic_negative(SimpleNamespace(returncode=code,stdout=out,stderr=err),d)
    except RuntimeError as e:
        if 'exact differential assertion' not in str(e):raise
        count+=1
    else:raise RuntimeError('setup/sanitizer failure counted as model control')
print(str(count)+' optimized integrity refusals and bounded frame controls pass')
'''
    run = subprocess.run([sys.executable, '-O', '-c', program, str(HERE)], capture_output=True, text=True)
    assert run.returncode == 0, run.stdout+run.stderr
    assert run.stdout == '995 optimized integrity refusals and bounded frame controls pass\n'


def test_shot_match_optimized_stock_compiler_replay():
    run = subprocess.run([sys.executable, '-O', str(HERE/'replay.py'), '--check'], capture_output=True, text=True)
    assert run.returncode == 0, run.stdout+run.stderr
    assert json.loads(run.stdout) == json.loads((HERE/'proof.json').read_text())


def test_shot_match_missing_ido_diagnostic(tmp_path):
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
    run = subprocess.run([sys.executable, '-O', '-c', program, str(HERE), str(tmp_path/'missing_ido')], capture_output=True, text=True)
    assert run.returncode == 0, run.stdout+run.stderr
    assert run.stdout == 'standard missing-IDO diagnostic passes\n'
