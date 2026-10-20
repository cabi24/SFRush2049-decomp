#!/usr/bin/env python3
"""Reproduce the natural-C ShotToWorldCollision match and complete group closure."""
import argparse
import contextlib
import hashlib
import io
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = Path(os.environ.get('N64_RESEARCH_REPO', HERE.parents[3])).resolve()
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
    return dict(protocol_version=3, population='game_blob', compiler='ido5.3', recipe=recipe(),
        packet_sha256={p: digest((HERE / p).read_bytes()) for p in ['candidate.c', 'group.json', 'replay.py', 'semantics.c', 'README.md']},
        context_sha256={p: digest((ROOT / p).read_bytes()) for p in paths},
        compiler_sha256={n: digest(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','ujoin','uld','usplit','umerge','uopt','ugen','as1']},
        # w34i: blob_matched.lock.json and SHA256SUMS change with every splice (handoff §5): not bound.
        tooling_sha256={n: digest((ROOT/n).read_bytes()) for n in ['tools/cloud/score.py','tools/cloud/owndata.py','tools/conveyor/pipeline/blob_unit.py']},
        target_metadata={n:digest((score.ASM_DIR/n).read_bytes()) for n in ['symbols.json']},
        owned_manifest_sha256=digest((owndata.artifact_dir(score.ASM_DIR)/'SHA256SUMS').read_bytes()),
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
    complete = complete_layout(obj)
    funcs = sorted(complete['functions'])
    frames = complete['functions'][NAME]['frames']
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
        verify_semantic_negative(run, d)
        return 'wrong-radius mutation rejected by differential assertion'
    if run.returncode or run.stderr: raise RuntimeError('host semantics failed: ' + run.stderr)
    return run.stdout.strip()

def verify_semantic_negative(run, d):
    expected = f"semantics: {d / 'semantics.c'}:80: compare: Assertion `memcmp(&a,&b,sizeof a)==0' failed.\n"
    require(run.returncode == -6 and run.stdout == '' and run.stderr == expected,
            'wrong-radius control did not fail at the exact differential assertion')


def verify():
    verify_source_change(); spec = recipe(); verify_scope(spec)
    if spec['members'] != OLD + [NAME] or spec['claims'] != OLD + [NAME]: raise RuntimeError('complete member claims changed')
    if any(n in spec['keep'] for n in REMOVED): raise RuntimeError('obsolete stand-in kept')
    source = (HERE / 'candidate.c').read_text()
    variants = dict(historical=baseline(), meaningful_state=state_local(baseline()),
        z_arguments_only=z_arguments(state_local(baseline())),
        interpolation_only=interpolation(state_local(baseline())),
        state_32bit=once(source, '    s16 state;', '    s32 state;'),
        wrong_radius=once(source, 'radius * radius)', 'radius)'))
    report = dict(inputs=inputs(), positive={}, negative_controls={}, complete_layouts={}, preserved_members=OLD,
        new_individually_strict_bytes=3580, accepted_image_bytes=0, whole_unit_gate='not run', image_gate='not run', rom_gate='not run',
        fp_scope='Exact stock-IDO target output and bounded finite host differential cases; not a claim about arbitrary NaN payloads or original historical source spelling.')
    with tempfile.TemporaryDirectory(prefix='rush-shot-match-') as tmp:
        d = Path(tmp); materialize(d)
        for label in ['first', 'repeat']:
            obj = d / (label + '.o'); score.compile_group(d, obj)
            report['positive'][label] = dict(scores=group_result(obj), sections=sections(obj), layout=layout(obj))
            report['complete_layouts'][label] = complete_layout(obj)
        for label, s in variants.items():
            (d / 'candidate.c').write_text(s); obj = d / (label + '.o'); score.compile_group(d, obj)
            report['negative_controls'][label] = dict(scores=group_result(obj), sections=sections(obj))
            report['complete_layouts'][label] = complete_layout(obj)
        (d / 'candidate.c').write_text(source)
        (d / 'group.c').write_text(once(sources()['group.c'], 'f32 lsq(', 'static f32 lsq('))
        obj = d / 'unbounded_static_residue.o'; score.compile_group(d, obj)
        report['negative_controls']['unbounded_static_residue'] = dict(scores=group_result(obj), sections=sections(obj))
        report['complete_layouts']['unbounded_static_residue'] = complete_layout(obj)
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
    verify_report(report)
    return report


def require(value, message):
    if not value:
        raise RuntimeError(message)


EXPECTED_RECIPE = {'members': ['func_800AD650', 'handbrake_apply', 'func_800C36A0', 'camera_trigger_check', 'camera_victory', 'func_800C3AD0', 'input_deadzone_apply'], 'files': ['candidate.c', 'group.c', 'camera_victory.c', 'camera_trigger_check.c', 'effect_cleanup.c', 'func_800C54F0.c', 'func_800A61B0.c', 'math_utility.c', 'func_800B61A8.c', 'func_8009E820.c', 'camera_first_person.c', 'func_800BF780.c', 'steering.c', 'normalize.c', 'decode.c'], 'keep': ['func_800AD5D0', 'handbrake_apply', 'func_800AC9BC', '__standin_c36a0_a', '__standin_c36a0_b', 'camera_trigger_check', 'camera_victory', 'effect_cleanup', 'func_800C54F0', 'func_800A61B0', 'math_utility', 'func_800B61A8', 'func_8009E820', 'input_deadzone_apply', 'camera_first_person', 'func_800BF780', 'steering_sensitivity', 'traction_control', 'func_8008E0B8', 'func_8008E098', 'func_800ADCE0'], 'flags': '-g0 -O3 -mips2 -G 0 -non_shared', 'unprototyped': [], 'context': ['func_800AD4C8', 'input_process_controller', 'func_800AD5D0', 'func_800AC9BC', 'effect_cleanup', 'func_800C54F0', 'func_800A61B0', 'math_utility', 'func_800B61A8', 'func_8009E820', 'camera_first_person', 'func_800BF780', 'steering_sensitivity', 'traction_control', 'vector_diff_process', 'func_8008E0B8', 'func_8008E098', 'func_800ADCE0'], 'provenance': 'N64 w11b/w12a candidate; MAT3 ShotToWorldCollision/CylinderInSurf donor cross-reference; natural C closure, 2026-10-17.', 'note': 'Preserves all six canonical members and adds input_deadzone_apply. Real callers replace the three AD4C8/input_process_controller stand-ins. Existing lsq is non-static and unkept to delimit deleted-procedure residue. func_800AC9BC and the unrelated C36A0 stand-ins are inherited and unclaimed. All other context is strict; full-build, image and ROM gates are pending. Materialized from canonical sources by replay.py.', 'claims': ['func_800AD650', 'handbrake_apply', 'func_800C36A0', 'camera_trigger_check', 'camera_victory', 'func_800C3AD0', 'input_deadzone_apply']}
EXPECTED_FUNCTIONS = {'input_deadzone_apply': {'bytes': 3580, 'frames': [400]}, 'handbrake_apply': {'bytes': 216, 'frames': [24]}, 'func_800ADCE0': {'bytes': 120, 'frames': []}, 'input_process_controller': {'bytes': 1452, 'frames': [184]}, 'math_utility': {'bytes': 76, 'frames': []}, 'steering_sensitivity': {'bytes': 928, 'frames': [104]}, 'traction_control': {'bytes': 992, 'frames': [224]}, 'func_8008E0B8': {'bytes': 140, 'frames': [40]}, 'lsq': {'bytes': 8, 'frames': []}, 'func_800AD4C8': {'bytes': 264, 'frames': []}, 'func_800AD5D0': {'bytes': 128, 'frames': []}, 'func_800AD650': {'bytes': 228, 'frames': []}, 'func_800C3AD0': {'bytes': 1448, 'frames': [160]}, 'func_800A61B0': {'bytes': 148, 'frames': []}, 'func_8009E820': {'bytes': 148, 'frames': []}, 'func_800AC9BC': {'bytes': 224, 'frames': []}, 'func_800C36A0': {'bytes': 1072, 'frames': [96]}, '__standin_c36a0_a': {'bytes': 104, 'frames': [64]}, '__standin_c36a0_b': {'bytes': 104, 'frames': [64]}, 'camera_victory': {'bytes': 1468, 'frames': [584]}, 'camera_trigger_check': {'bytes': 1232, 'frames': [320]}, 'effect_cleanup': {'bytes': 96, 'frames': [24]}, 'func_800C54F0': {'bytes': 244, 'frames': [24]}, 'func_800B61A8': {'bytes': 84, 'frames': [24]}, 'camera_first_person': {'bytes': 944, 'frames': [240]}, 'func_800BF780': {'bytes': 184, 'frames': []}, 'vector_diff_process': {'bytes': 80, 'frames': [40]}, 'func_8008E098': {'bytes': 32, 'frames': []}}
ROOT_DIFFERENCES = {'historical': 122, 'meaningful_state': 122, 'z_arguments_only': 115, 'interpolation_only': 12, 'state_32bit': 276, 'wrong_radius': 29, 'unbounded_static_residue': 0}
HOST_RESULT = '70898 finite differential cases; all 65536 s16 returns; 480 traversal exits; 140670 accepted hits; 1800 snap calls; 1800 normalizations; UBSan clean'


def verify_scope(spec):
    require(spec == EXPECTED_RECIPE, 'complete source/context/claim recipe changed')


def frame_scan(words, start, size):
    return [65536-(word & 65535) for word in words[start//4:(start+size)//4]
            if word >> 16 == 0x27bd and word & 32768]


def complete_layout(obj):
    elf = owndata._Object(obj)
    symbols = [s for i, section in enumerate(elf.sections) if section['type'] == owndata.SHT_SYMTAB
               for s in elf.symbols(i) if s['type'] == owndata.STT_FUNC and 0 < s['section'] < len(elf.sections)]
    words = score.text_words(obj)
    functions, gaps = {}, []
    cursor = 0
    for symbol in sorted(symbols, key=lambda row: row['value']):
        require(elf.sections[symbol['section']]['name'] == '.text', 'function escaped text section')
        start, size = symbol['value'], symbol['size']
        require(start >= cursor and start % 4 == size % 4 == 0, 'overlapping or unaligned function')
        if start > cursor:
            gaps.append(dict(offset=cursor, bytes=start-cursor, nonzero_words=sum(w != 0 for w in words[cursor//4:start//4])))
        functions[symbol['name']] = dict(bytes=size, frames=frame_scan(words, start, size))
        cursor = start+size
    if cursor < len(words)*4:
        gaps.append(dict(offset=cursor, bytes=len(words)*4-cursor, nonzero_words=sum(w != 0 for w in words[cursor//4:])))
    return dict(functions=functions, gaps=gaps, text_bytes=len(words)*4, object_sha256=digest(obj.read_bytes()))


def verify_report(report):
    verify_scope(recipe())
    require(report['inputs'] == inputs(), 'source/compiler/tool/target binding changed')
    require(report['new_individually_strict_bytes'] == 3580 and report['accepted_image_bytes'] == 0
            and report['preserved_members'] == OLD, 'credit or preserved claim changed')
    require(all(report[key] == 'not run' for key in ['whole_unit_gate','image_gate','rom_gate']), 'integration claim changed')
    require(set(report['positive']) == {'first','repeat'} and set(report['negative_controls']) == set(ROOT_DIFFERENCES), 'build inventory changed')
    builds = {**{k:v['scores'] for k,v in report['positive'].items()},
              **{k:v['scores'] for k,v in report['negative_controls'].items()}}
    require(set(report['complete_layouts']) == set(builds), 'layout inventory changed')
    require(report['positive']['first'] == report['positive']['repeat']
            and report['complete_layouts']['first'] == report['complete_layouts']['repeat'], 'complete repeat changed')
    require(report['negative_controls']['historical'] == report['negative_controls']['meaningful_state'], 'state cleanup output changed')
    for label, rows in builds.items():
        require(set(rows) == set(strict_names()) | {'func_800AC9BC'}, 'score inventory changed')
        for name, row in rows.items():
            target = report['inputs']['targets'][name]['bytes']
            require(row['target_words']*4 == target, 'protected word count changed')
            expected_differences = 52 if name == 'func_800AC9BC' else ROOT_DIFFERENCES.get(label,0) if name == NAME else 0
            expected_extra = int(label == 'unbounded_static_residue' and name == 'effect_cleanup')
            accepted = not any(row[key] for key in ['differing_words','extra_nonzero_words','unresolved','unverified','errors'])
            require(row['strict_match'] == accepted, 'inconsistent strict field')
            require(row['differing_words'] == expected_differences and row['extra_nonzero_words'] == expected_extra
                    and not row['unresolved'] and not row['unverified'] and not row['errors'], 'documented score outcome changed')
        expected = {name:dict(row) for name,row in EXPECTED_FUNCTIONS.items()}
        if label == 'state_32bit': expected[NAME] = dict(bytes=3580, frames=[408])
        if label == 'unbounded_static_residue': expected.pop('lsq')
        layout_row = report['complete_layouts'][label]
        require(layout_row['functions'] == expected, 'complete function extent/frame changed')
        for name in strict_names()+['func_800AC9BC']:
            require(layout_row['functions'][name]['bytes'] == report['inputs']['targets'][name]['bytes'], 'native full extent changed')
        nonzero = [gap for gap in layout_row['gaps'] if gap['nonzero_words']]
        require((len(nonzero) == 1 and nonzero[0]['bytes'] == 8 and nonzero[0]['nonzero_words'] == 1)
                if label == 'unbounded_static_residue' else not nonzero, 'unaccounted emitted residue changed')
        require(sum(row['bytes'] for row in expected.values())+sum(gap['bytes'] for gap in layout_row['gaps']) == layout_row['text_bytes'], 'text extent accounting changed')
    expected_legacy = dict(emitted_functions=sorted(EXPECTED_FUNCTIONS), root_frame_bytes=400, deleted_helper_has_symbol=True)
    require(all(row['layout'] == expected_legacy for row in report['positive'].values()), 'legacy function/frame boundary changed')
    require(report['fp_scope'] == 'Exact stock-IDO target output and bounded finite host differential cases; not a claim about arbitrary NaN payloads or original historical source spelling.', 'floating-point scope changed')
    require(report['host_semantics'] == HOST_RESULT and report['host_semantic_control'] == 'wrong-radius mutation rejected by differential assertion', 'host semantic scope changed')


def main():
    ap = argparse.ArgumentParser(description=__doc__); ap.add_argument('--check', action='store_true'); ap.add_argument('--materialize', type=Path); args = ap.parse_args()
    if args.materialize:
        args.materialize.mkdir(parents=True, exist_ok=True); materialize(args.materialize); return
    if args.check: verify_report(json.loads((HERE / 'proof.json').read_text()))
    report = verify()
    if args.check and report != json.loads((HERE / 'proof.json').read_text()): raise RuntimeError('fresh receipt differs')
    print(json.dumps(report, indent=2, sort_keys=True))

if __name__ == '__main__': main()
