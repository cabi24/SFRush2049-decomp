#!/usr/bin/env python3
"""Replay the native window initializer with genuine helpers and return contract."""
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
NAME = 'arb_rate_set'
STRICT = [NAME, 'func_800A5560', 'exhaust_smoke_effect']
CONTEXT = {'fill.c': 'src/blob/func_800A5560.c', 'set_window.c': 'src/blob/exhaust_smoke_effect.c'}
HISTORICAL = 'cloud/work/frontier/w9d/arb_rate_set/best.c'
OLD_PACK = 'GPACK_RGBA5551(D_8017A510[index].red,D_8017A510[index].green,D_8017A510[index].blue,1)'
NEW_PACK = '(u16)(((D_8017A510[index].red<<8)&0xf800)|((D_8017A510[index].green<<3)&0x7c0)|((D_8017A510[index].blue>>2)&0x3e))|1'

def digest(b): return hashlib.sha256(b).hexdigest()
def recipe(): return json.loads((HERE / 'group.json').read_text())
def source(): return (HERE / 'candidate.c').read_text()
def tokens(s): return re.sub(r'\s+', '', re.sub(r'/\*.*?\*/|//[^\n]*', '', s, flags=re.S))
def once(s, a, b):
    if s.count(a) != 1: raise RuntimeError('ambiguous source anchor: ' + a)
    return s.replace(a, b, 1)

def baseline():
    s = (ROOT / HISTORICAL).read_text()
    return once(s, 'void func_800A5560(u16 color) {\n    D_80124FC8 = (color << 16) | color;\n}', 'void func_800A5560(u16 color);')

def verify_source_change():
    s = once(baseline(), 'void arb_rate_set(', 'Descriptor72 *arb_rate_set(')
    s = s.rstrip()[:-1] + '    return &D_8017A510[index];\n}\n'
    s = once(s, OLD_PACK, NEW_PACK)
    if tokens(s) != tokens(source()): raise RuntimeError('source exceeds the documented return/packed-color changes')

def materialize(d):
    for dest, p in CONTEXT.items(): shutil.copyfile(ROOT / p, d / dest)
    for n in ['candidate.c', 'group.json']: shutil.copyfile(HERE / n, d / n)

def inputs():
    targets, syms = score.targets(), score.image_symbols()
    return dict(protocol_version=2, population='game_blob', compiler='ido5.3', recipe=recipe(),
        packet_sha256={p: digest((HERE / p).read_bytes()) for p in ['candidate.c', 'group.json', 'replay.py', 'semantics.c']},
        context_sha256={p: digest((ROOT / p).read_bytes()) for p in sorted([HISTORICAL] + list(CONTEXT.values()))},
        targets={n: dict(address=hex(syms[n]), bytes=len(targets[n])*4, sha256=digest(b''.join(struct.pack('>I', w) for w in targets[n]))) for n in STRICT})

def diagnostic(s):
    return re.sub(r'retail [0-9a-fA-F]+, got [0-9a-fA-F]+', 'retail [redacted], got [redacted]', s)

def result(obj, n):
    with contextlib.redirect_stdout(io.StringIO()): r = score.compare(obj, n, show=0)
    return dict(differing_words=r.differing, target_words=r.total, extra_nonzero_words=r.extra_words,
                unresolved=r.unresolved, unverified=r.unverified, errors=[diagnostic(e) for e in r.errors], strict_match=r.accepted())

def sections(obj):
    data, secs = score._elf(obj)
    return {s['name']: dict(bytes=s['size'], sha256=digest(data[s['off']:s['off']+s['size']]))
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
    s = baseline(); body = s[s.index('void arb_rate_set('):]
    (d / 'historical_body.inc').write_text(body.replace(NAME, 'historical_initialize'))
    for n in ['candidate.c', 'semantics.c']: shutil.copyfile(HERE / n, d / n)
    if mutation is not None: (d / 'candidate.c').write_text(mutation)
    exe = d / 'semantics'
    subprocess.run([shutil.which('cc') or 'cc', '-std=c11', '-O2', '-fno-strict-aliasing', '-fsanitize=undefined', '-fno-sanitize-recover=undefined', str(d / 'semantics.c'), '-lm', '-o', str(exe)], check=True, capture_output=True, text=True)
    run = subprocess.run([str(exe)], capture_output=True, text=True)
    if mutation is not None:
        if run.returncode != 3 or run.stderr != 'window-initializer differential mismatch\n' or run.stdout:
            raise RuntimeError('mutation did not produce the expected differential failure: ' + repr((run.returncode, run.stdout, run.stderr)))
        return 'mutation rejected with exact differential exit code and message'
    if run.returncode or run.stderr: raise RuntimeError('host semantics failed: ' + repr((run.returncode, run.stdout, run.stderr)))
    return run.stdout.strip()

def verify():
    verify_source_change(); spec = recipe(); s = source()
    if spec['members'] != [NAME] or spec['claims'] != [NAME] or spec['context'] != STRICT[1:]: raise RuntimeError('complete claims/context changed')
    variants = dict(historical_void=baseline(), return_only=once(s, NEW_PACK, OLD_PACK),
        cast_only=once(once(s, 'Descriptor72 *arb_rate_set(', 'void arb_rate_set('), '    return &D_8017A510[index];\n', ''),
        wrong_frontclip=once(s, '.zoom=2.5f;', '.zoom=0.01f;'),
        wrong_backclip=s.replace('2000.0f', '4000.0f'),
        wrong_alpha=once(s, NEW_PACK, NEW_PACK[:-1]+'0'),
        wrong_return=once(s, 'return &D_8017A510[index];', 'return &D_8017A510[0];'))
    local = once(s, '{\n    D_8017A510[index].first', '{\n    Descriptor72 *winfo = &D_8017A510[index];\n    winfo->first')
    variants['donor_local_pointer'] = local.replace('D_8017A510[index].', 'winfo->').replace('return &D_8017A510[index];', 'return winfo;')
    report = dict(inputs=inputs(), positive={}, negative_controls={}, preserved_helpers=STRICT[1:],
        new_individually_strict_bytes=312, accepted_image_bytes=0, whole_unit_gate='not run', image_gate='not run', rom_gate='not run',
        semantic_scope='Finite viewport values representable after native signed-short conversion; valid indices and objects. Historical state/callback behavior is compared, and the restored return is checked independently. No arbitrary float/invalid pointer claim.')
    with tempfile.TemporaryDirectory(prefix='rush-window-init-') as tmp:
        d = Path(tmp); materialize(d)
        for label in ['first', 'repeat']:
            obj = d / (label+'.o'); score.compile_group(d, obj); report['positive'][label] = group_result(obj)
        for label, variant in variants.items():
            (d / 'candidate.c').write_text(variant); obj = d / (label+'.o'); score.compile_group(d, obj)
            report['negative_controls'][label] = group_result(obj)
        report['host_semantics'] = semantic_check(d)
        report['host_semantic_controls'] = {label: semantic_check(d, variants[label]) for label in ['wrong_frontclip', 'wrong_alpha', 'wrong_return']}
    pos, neg = report['positive'], report['negative_controls']
    if pos['first'] != pos['repeat']: raise RuntimeError('repeated executable sections differ')
    for label, group in pos.items():
        if any(not r['strict_match'] for r in group['scores'].values()): raise RuntimeError('strict closure failed')
        if group['layout']['emitted_functions'] != sorted(STRICT): raise RuntimeError('unexpected emitted functions')
        if group['layout']['root_frame_bytes'] != 56: raise RuntimeError('native frame changed')
        for n in STRICT:
            if group['layout']['function_extents'][n]['bytes'] != report['inputs']['targets'][n]['bytes']: raise RuntimeError('function extent changed: '+n)
    for label, group in neg.items():
        if group['scores'][NAME]['strict_match']: raise RuntimeError('negative incorrectly accepted: '+label)
        if any(not group['scores'][n]['strict_match'] for n in STRICT[1:]): raise RuntimeError('control damaged a preserved helper')
    if neg['historical_void']['scores'][NAME]['differing_words'] != 25 or neg['return_only']['scores'][NAME]['differing_words'] != 6: raise RuntimeError('causal controls changed')
    return report

def main():
    ap = argparse.ArgumentParser(description=__doc__); ap.add_argument('--check', action='store_true'); ap.add_argument('--materialize', type=Path); args = ap.parse_args()
    if args.materialize:
        args.materialize.mkdir(parents=True, exist_ok=True); materialize(args.materialize); return
    report = verify()
    if args.check and report != json.loads((HERE / 'proof.json').read_text()): raise RuntimeError('fresh receipt differs')
    print(json.dumps(report, indent=2, sort_keys=True))

if __name__ == '__main__': main()
