#!/usr/bin/env python3
"""Strict stock-IDO replay of ZOID_ProjectWindow with its genuine window getter."""
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

NAME = 'brake_light_update'
CONTEXT = {'body_vector.c': 'src/blob/func_800A61B0.c', 'get_window.c': 'src/blob/func_800A6078.c'}
STRICT = [NAME, 'func_800A61B0', 'func_800A6078']
HISTORICAL = 'cloud/work/frontier/w9d/brake_light_update/best.c'

def digest(b): return hashlib.sha256(b).hexdigest()
def recipe(): return json.loads((HERE / 'group.json').read_text())
def tokens(s): return re.sub(r'\s+', '', re.sub(r'/\*.*?\*/|//[^\n]*', '', s, flags=re.S))
def once(s, a, b):
    if s.count(a) != 1: raise RuntimeError('ambiguous source anchor: ' + a)
    return s.replace(a, b, 1)

def baseline(): return (ROOT / HISTORICAL).read_text()
def source(): return (HERE / 'candidate.c').read_text()
def flatten(s): return once(s, '    p = func_800A6078(idx);\n', '    p = &D_8017A510[idx];\n')

def verify_source_change():
    s = once(baseline(), '    s32 pad;\n', '')
    s = once(s, 'void brake_light_update(', 'void *func_800A6078(s32 idx);\n\nvoid brake_light_update(')
    s = once(s, '    inv = 1.0f / r[2];\n    p = &D_8017A510[idx];', '    p = func_800A6078(idx);\n    inv = 1.0f / r[2];')
    if tokens(s) != tokens(source()): raise RuntimeError('candidate exceeds the documented getter and padding-removal changes')

def materialize(d):
    for dest, path in CONTEXT.items(): shutil.copyfile(ROOT / path, d / dest)
    for n in ['candidate.c', 'group.json']: shutil.copyfile(HERE / n, d / n)

def inputs():
    targets, syms = score.targets(), score.image_symbols()
    return dict(protocol_version=2, population='game_blob', compiler='ido5.3', recipe=recipe(),
        packet_sha256={p: digest((HERE / p).read_bytes()) for p in ['candidate.c', 'group.json', 'replay.py', 'semantics.c']},
        context_sha256={p: digest((ROOT / p).read_bytes()) for p in sorted([HISTORICAL] + list(CONTEXT.values()))},
        targets={n: dict(address=hex(syms[n]), bytes=len(targets[n])*4, sha256=digest(b''.join(struct.pack('>I', w) for w in targets[n]))) for n in STRICT})

def diagnostic(s):
    # Preserve diagnostic categories without publishing literal target data bytes.
    return re.sub(r'retail [0-9a-fA-F]+, got [0-9a-fA-F]+', 'retail [redacted], got [redacted]', s)

def result(obj, n):
    with contextlib.redirect_stdout(io.StringIO()): r = score.compare(obj, n, show=0)
    return dict(differing_words=r.differing, target_words=r.total, extra_nonzero_words=r.extra_words,
                unresolved=r.unresolved, unverified=r.unverified, errors=[diagnostic(e) for e in r.errors], strict_match=r.accepted())

def sections(obj):
    data, secs = score._elf(obj)
    return {s['name']: dict(bytes=s['size'], sha256=digest(data[s['off']:s['off'] + s['size']]))
            for s in secs if s['name'] in {'.text', '.rel.text', '.rodata', '.rel.rodata', '.data', '.rel.data'}}

def layout(obj):
    words = score.text_words(obj); start = score.symbols(obj)[NAME] // 4
    frames = [65536-(w&65535) for w in words[start:start+60] if w>>16 == 0x27bd and w&0x8000]
    elf = owndata._Object(obj)
    funcs = [sym for i, sec in enumerate(elf.sections) if sec['type'] == owndata.SHT_SYMTAB
             for sym in elf.symbols(i) if sym['type'] == owndata.STT_FUNC and 0 < sym['section'] < len(elf.sections)]
    return dict(emitted_functions=sorted(sym['name'] for sym in funcs),
                function_extents={sym['name']: dict(offset=sym['value'], bytes=sym['size']) for sym in funcs},
                root_frame_bytes=frames[0] if frames else None)

def group_result(obj):
    return dict(scores={n: result(obj, n) for n in STRICT}, sections=sections(obj), layout=layout(obj))

def semantic_check(d, mutation=None):
    b = baseline(); body = b[b.index('void brake_light_update('):]
    (d / 'historical_body.inc').write_text(body.replace(NAME, 'historical_project'))
    for n in ['candidate.c', 'semantics.c']: shutil.copyfile(HERE / n, d / n)
    if mutation is not None: (d / 'candidate.c').write_text(mutation)
    exe = d / 'semantics'
    subprocess.run([shutil.which('cc') or 'cc', '-std=c11', '-O2', '-fno-strict-aliasing', '-fsanitize=undefined', '-fno-sanitize-recover=undefined', str(d / 'semantics.c'), '-lm', '-o', str(exe)], check=True, capture_output=True, text=True)
    run = subprocess.run([str(exe)], capture_output=True, text=True)
    if mutation is not None:
        if run.returncode != 3 or run.stderr != 'project-window differential mismatch\n' or run.stdout:
            raise RuntimeError('wrong-depth mutation did not produce the expected differential failure: ' + repr((run.returncode, run.stdout, run.stderr)))
        return 'wrong-depth mutation rejected by differential assertion'
    if run.returncode or run.stderr: raise RuntimeError('host semantics failed: ' + run.stderr)
    return run.stdout.strip()

def verify():
    verify_source_change(); spec = recipe(); s = source()
    if spec['members'] != [NAME] or spec['claims'] != [NAME] or spec['context'] != STRICT[1:]: raise RuntimeError('complete claims/context changed')
    variants = dict(flattened_getter=flatten(s), historical_padding=baseline(),
        scalar_screen_coordinates=once(s, '    f32 scr[2];', '    f32 sx, sy;').replace('scr[0]', 'sx').replace('scr[1]', 'sy'),
        wrong_depth=s.replace('2.5f', '0.01f'),
        wrong_gc_vertical_scale=once(s, 'p->unk30 - r[1]', 'p->unk30 - 0.93333f * r[1]'))
    report = dict(inputs=inputs(), positive={}, negative_controls={}, preserved_helpers=STRICT[1:],
        new_individually_strict_bytes=448, accepted_image_bytes=0, whole_unit_gate='not run', image_gate='not run', rom_gate='not run',
        semantic_scope='Bounded finite inputs with valid window index and nonoverlapping output buffers; exact native output is independently checked. No original-spelling or arbitrary-NaN claim.')
    with tempfile.TemporaryDirectory(prefix='rush-project-window-') as tmp:
        d = Path(tmp); materialize(d)
        for label in ['first', 'repeat']:
            obj = d / (label + '.o'); score.compile_group(d, obj); report['positive'][label] = group_result(obj)
        reordered = once(s, '    p = func_800A6078(idx);\n    inv = 1.0f / r[2];', '    inv = 1.0f / r[2];\n    p = func_800A6078(idx);')
        (d / 'candidate.c').write_text(reordered); obj = d / 'getter_after_inverse.o'; score.compile_group(d, obj)
        report['positive']['getter_after_inverse'] = group_result(obj)
        for label, variant in variants.items():
            (d / 'candidate.c').write_text(variant); obj = d / (label + '.o'); score.compile_group(d, obj)
            report['negative_controls'][label] = group_result(obj)
        (d / 'candidate.c').write_text(s)
        getter = (ROOT / CONTEXT['get_window.c']).read_text()
        (d / 'get_window.c').write_text(once(getter, 'arg0 * 0x48', 'arg0 * 0x44'))
        obj = d / 'wrong_window_stride.o'; score.compile_group(d, obj)
        report['negative_controls']['wrong_window_stride'] = group_result(obj)
        report['host_semantics'] = semantic_check(d)
        report['host_semantic_control'] = semantic_check(d, variants['wrong_depth'])
    pos = report['positive']; neg = report['negative_controls']
    if pos['first'] != pos['repeat'] or pos['first'] != pos['getter_after_inverse']: raise RuntimeError('positive object sections differ')
    if any(not r['strict_match'] for r in pos['first']['scores'].values()): raise RuntimeError('strict closure failed')
    if pos['first']['layout']['root_frame_bytes'] != 72: raise RuntimeError('native frame changed')
    for label, group in pos.items():
        if group['layout']['emitted_functions'] != sorted(STRICT): raise RuntimeError('unexpected emitted functions: ' + label)
        for n in STRICT:
            if group['layout']['function_extents'][n]['bytes'] != report['inputs']['targets'][n]['bytes']:
                raise RuntimeError('function extent changed: ' + label + ':' + n)
    for label, group in neg.items():
        if group['scores'][NAME]['strict_match']: raise RuntimeError('negative incorrectly accepted: ' + label)
        if label != 'wrong_window_stride' and any(not group['scores'][n]['strict_match'] for n in STRICT[1:]): raise RuntimeError('control damaged preserved helpers')
    if neg['historical_padding']['scores'][NAME]['differing_words'] != 74 or neg['flattened_getter']['scores'][NAME]['differing_words'] != 86: raise RuntimeError('historical controls changed')
    return report

def main():
    ap = argparse.ArgumentParser(description=__doc__); ap.add_argument('--check', action='store_true'); ap.add_argument('--materialize', type=Path); args = ap.parse_args()
    if args.materialize:
        args.materialize.mkdir(parents=True, exist_ok=True); materialize(args.materialize); return
    report = verify()
    if args.check and report != json.loads((HERE / 'proof.json').read_text()): raise RuntimeError('fresh receipt differs')
    print(json.dumps(report, indent=2, sort_keys=True))

if __name__ == '__main__': main()
