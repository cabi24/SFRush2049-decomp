#!/usr/bin/env python3
"""Reproduce the natural-C ShotToWorldCollision match and complete group closure."""
import argparse
import contextlib
import hashlib
import io
import json
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor.pipeline import blob_unit

NAME = 'input_deadzone_apply'
OLD = ['func_800AD650', 'handbrake_apply', 'func_800C36A0', 'camera_trigger_check', 'camera_victory', 'func_800C3AD0']
COPIES = ['effect_cleanup', 'func_800C54F0', 'func_800A61B0', 'math_utility', 'func_800B61A8', 'func_8009E820']
BASE = 'cloud/work/frontier/w12a/ida/'
GROUP = 'src/blob/groups/func_800AD4C8/'
CONTEXT = [('group.c', GROUP + 'group.c', None),
           ('camera_victory.c', GROUP + 'camera_victory.c', None),
           ('camera_trigger_check.c', GROUP + 'camera_trigger_check.c', None)] + [
           (n + '.c', 'src/blob/' + n + '.c', None) for n in COPIES] + [
    ('camera_first_person.c', 'src/blob/groups/frontier_camera_first_person/camera_first_person.c', ['camera_first_person']),
    ('func_800BF780.c', 'src/blob/func_800BF780.c', None),
    ('steering.c', 'src/blob/groups/frontier_steering_sensitivity/group.c', ['vector_diff_process', 'steering_sensitivity', 'traction_control']),
    ('normalize.c', 'src/blob/groups/frontier_vector_normalize/group.c', None),
    ('decode.c', 'src/blob/groups/dot_run_decode/group.c', None),
]
REMOVED = ['__standin_func_800AD4C8', '__standin_input_process_controller_a', '__standin_input_process_controller_b']
CALL = 'input_process_controller(poly, cur, end, np, vc, (f32 *)m, 0, (s16 *)&idx, radius * radius)'
OLD_CALL = '                if (' + CALL + ' != 0) {'
NEW_CALL = '                state = ' + CALL + ';\n                if (state != 0) {'
END_FLOOR = '''    if (end[2] < 0.0f && (f32)(s32)end[2] != end[2]) {
        z = end[2] - 1.0f;
    } else {
        z = end[2];
    }
'''

def digest(b): return hashlib.sha256(b).hexdigest()
def recipe(): return json.loads((HERE / 'group.json').read_text())
def strict_names(): return recipe()['members'] + [n for n in recipe()['context'] if n != 'func_800AC9BC']
def tokens(s): return re.sub(r'\s+', '', re.sub(r'/\*.*?\*/|//[^\n]*', '', s, flags=re.S))
def once(s, a, b):
    if s.count(a) != 1: raise RuntimeError('ambiguous source anchor: ' + a)
    return s.replace(a, b, 1)

def baseline():
    return (ROOT / (BASE + 'hdr_ipcorder.h')).read_text() + (ROOT / (BASE + 'best.c')).read_text()

def state_local(s): return once(once(s, '    s16 W;', '    s16 state;'), OLD_CALL, NEW_CALL)

def z_arguments(s):
    s = once(s, '    z = FLOORF(cur[2]);\n', '')
    s = once(s, 'node = handbrake_apply(D_80124EEC, (s32)x, (s32)z, &quad);', 'node = handbrake_apply(D_80124EEC, (s32)x, (s32)(z = FLOORF(cur[2])), &quad);')
    s = once(s, END_FLOOR, '')
    return once(s, 'endNode = handbrake_apply(D_80124EEC, (s32)x, (s32)z, &endQuad);', 'endNode = handbrake_apply(D_80124EEC, (s32)x, (s32)(z = FLOORF(end[2])), &endQuad);')

def interpolation(s):
    s, count = re.subn(r'np\[(\d)\] = d\[\1\] \* t \+ cur\[\1\];', lambda m: 'np[{0}] = cur[{0}] + d[{0}] * t;'.format(m[1]), s)
    if count != 8: raise RuntimeError('interpolation anchors changed')
    return s

def verify_source_change():
    if tokens(interpolation(z_arguments(state_local(baseline())))) != tokens((HERE / 'candidate.c').read_text()):
        raise RuntimeError('candidate exceeds the three documented source changes')

def sources():
    result = {}
    for dest, path, keep in CONTEXT:
        s = (ROOT / path).read_text()
        if keep is not None: s, _ = blob_unit.transform(s.replace('\\\n', ' '), set(keep))
        if dest == 'group.c':
            old = 's16 input_process_controller(f32 *p1, f32 *p2, f32 *out, Poly *poly, s16 *outIdx, s32 flag, f32 *vcOut, f32 *mat, f32 rad2)'
            new = 's16 input_process_controller(Poly *poly, f32 *p1, f32 *p2, f32 *out, f32 *vcOut, f32 *mat, s32 flag, s16 *outIdx, f32 rad2)'
            if s.count(old) != 2: raise RuntimeError('callee signature anchors changed')
            s = s.replace(old, new)
            s = once(s, 'static f32 lsq(', 'f32 lsq(')
            for n in REMOVED:
                s, count = re.subn(r'void ' + n + r'\(void\)\n\{[^{}]*\}', 'void ' + n + '(void);', s)
                if count != 1: raise RuntimeError('stand-in removal anchor changed: ' + n)
        result[dest] = s
    return result

def materialize(d):
    for n, s in sources().items(): (d / n).write_text(s)
    for n in ['candidate.c', 'group.json']: shutil.copyfile(HERE / n, d / n)

def inputs():
    paths = sorted(set([BASE + 'hdr_ipcorder.h', BASE + 'best.c', GROUP + 'group.json'] + [p for _, p, _ in CONTEXT]))
    targets, syms = score.targets(), score.image_symbols()
    return dict(protocol_version=2, population='game_blob', compiler='ido5.3', recipe=recipe(),
        packet_sha256={p: digest((HERE / p).read_bytes()) for p in ['candidate.c', 'group.json', 'replay.py', 'semantics.c']},
        context_sha256={p: digest((ROOT / p).read_bytes()) for p in paths},
        prepared_sha256={p: digest(s.encode()) for p, s in sources().items()},
        targets={n: dict(address=hex(syms[n]), bytes=len(targets[n])*4, sha256=digest(b''.join(struct.pack('>I', w) for w in targets[n]))) for n in strict_names() + ['func_800AC9BC']})

def result(obj, n):
    with contextlib.redirect_stdout(io.StringIO()): r = score.compare(obj, n, show=0)
    return dict(differing_words=r.differing, target_words=r.total, extra_nonzero_words=r.extra_words,
                unresolved=r.unresolved, unverified=r.unverified, errors=r.errors, strict_match=r.accepted())

def group_result(obj): return {n: result(obj, n) for n in strict_names() + ['func_800AC9BC']}

def sections(obj):
    data, secs = score._elf(obj)
    return {s['name']: dict(bytes=s['size'], sha256=digest(data[s['off']:s['off'] + s['size']]))
            for s in secs if s['name'] in {'.text', '.rel.text', '.rodata', '.rel.rodata', '.data', '.rel.data'}}

def layout(obj):
    funcs = sorted(n for n, _, _ in owndata._Object(obj).functions())
    words = score.text_words(obj); start = score.symbols(obj)[NAME] // 4
    frames = [65536-(w&65535) for w in words[start:start+100] if w>>16 == 0x27bd and w&0x8000]
    return dict(emitted_functions=funcs, root_frame_bytes=frames[0] if frames else None, deleted_helper_has_symbol='lsq' in funcs)

def semantic_check(d, candidate=None):
    s = baseline(); body = s[s.index('Poly *input_deadzone_apply('):]
    (d / 'historical_body.inc').write_text(body.replace('input_deadzone_apply', 'historical_shot'))
    for n in ['candidate.c', 'semantics.c']: shutil.copyfile(HERE / n, d / n)
    if candidate is not None: (d / 'candidate.c').write_text(candidate)
    exe = d / 'semantics'
    subprocess.run([shutil.which('cc') or 'cc', '-std=c11', '-O2', '-fno-strict-aliasing', '-fsanitize=undefined', '-fno-sanitize-recover=undefined', '-Wno-pointer-to-int-cast', '-Wno-unknown-pragmas', str(d / 'semantics.c'), '-lm', '-o', str(exe)], check=True, capture_output=True, text=True)
    run = subprocess.run([str(exe)], capture_output=True, text=True)
    if candidate is not None:
        if run.returncode == 0: raise RuntimeError('semantic mutation was not detected')
        return 'wrong-radius mutation rejected by differential assertion'
    if run.returncode or run.stderr: raise RuntimeError('host semantics failed: ' + run.stderr)
    return run.stdout.strip()

def verify():
    verify_source_change(); spec = recipe()
    if spec['members'] != OLD + [NAME] or spec['claims'] != OLD + [NAME]: raise RuntimeError('complete member claims changed')
    if any(n in spec['keep'] for n in REMOVED): raise RuntimeError('obsolete stand-in kept')
    source = (HERE / 'candidate.c').read_text()
    variants = dict(historical=baseline(), meaningful_state=state_local(baseline()),
        z_arguments_only=z_arguments(state_local(baseline())),
        interpolation_only=interpolation(state_local(baseline())),
        state_32bit=once(source, '    s16 state;', '    s32 state;'),
        wrong_radius=once(source, 'radius * radius)', 'radius)'))
    report = dict(inputs=inputs(), positive={}, negative_controls={}, preserved_members=OLD,
        new_individually_strict_bytes=3580, accepted_image_bytes=0, whole_unit_gate='not run', image_gate='not run', rom_gate='not run',
        fp_scope='Exact stock-IDO target output and bounded finite host differential cases; not a claim about arbitrary NaN payloads or original historical source spelling.')
    with tempfile.TemporaryDirectory(prefix='rush-shot-match-') as tmp:
        d = Path(tmp); materialize(d)
        for label in ['first', 'repeat']:
            obj = d / (label + '.o'); score.compile_group(d, obj)
            report['positive'][label] = dict(scores=group_result(obj), sections=sections(obj), layout=layout(obj))
        for label, s in variants.items():
            (d / 'candidate.c').write_text(s); obj = d / (label + '.o'); score.compile_group(d, obj)
            report['negative_controls'][label] = dict(scores=group_result(obj), sections=sections(obj))
        (d / 'candidate.c').write_text(source)
        (d / 'group.c').write_text(once(sources()['group.c'], 'f32 lsq(', 'static f32 lsq('))
        obj = d / 'unbounded_static_residue.o'; score.compile_group(d, obj)
        report['negative_controls']['unbounded_static_residue'] = dict(scores=group_result(obj), sections=sections(obj))
        report['host_semantics'] = semantic_check(d)
        report['host_semantic_control'] = semantic_check(d, variants['wrong_radius'])
    pos = report['positive']
    if pos['first'] != pos['repeat']: raise RuntimeError('repeat object sections or scores changed')
    if any(not pos['first']['scores'][n]['strict_match'] for n in strict_names()): raise RuntimeError('strict closure failed')
    if pos['first']['layout']['root_frame_bytes'] != 400 or not pos['first']['layout']['deleted_helper_has_symbol']: raise RuntimeError('frame/helper boundary changed')
    neg = report['negative_controls']
    if neg['historical'] != neg['meaningful_state']: raise RuntimeError('state cleanup changed full executable output')
    for label in variants:
        if neg[label]['scores'][NAME]['strict_match']: raise RuntimeError('control incorrectly accepted: ' + label)
    if neg['historical']['scores'][NAME]['differing_words'] != 122 or neg['z_arguments_only']['scores'][NAME]['differing_words'] != 115: raise RuntimeError('historical stage score changed')
    if all(r['strict_match'] for n, r in neg['unbounded_static_residue']['scores'].items() if n in strict_names()): raise RuntimeError('residue control failed to detect extra output')
    if pos['first']['scores']['func_800AC9BC']['differing_words'] != 52: raise RuntimeError('inherited nonclaim changed')
    return report

def main():
    ap = argparse.ArgumentParser(description=__doc__); ap.add_argument('--check', action='store_true'); ap.add_argument('--materialize', type=Path); args = ap.parse_args()
    if args.materialize:
        args.materialize.mkdir(parents=True, exist_ok=True); materialize(args.materialize); return
    report = verify()
    if args.check and report != json.loads((HERE / 'proof.json').read_text()): raise RuntimeError('fresh receipt differs')
    print(json.dumps(report, indent=2, sort_keys=True))

if __name__ == '__main__': main()
