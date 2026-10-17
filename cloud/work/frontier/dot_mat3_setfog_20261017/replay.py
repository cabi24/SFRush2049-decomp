#!/usr/bin/env python3
"""Replay the native fog setter with genuine types, return and narrowing."""
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
NAME = 'func_800A7480'
STRICT = [NAME, 'func_800A5560']
CONTEXT = {'fill.c': 'src/blob/func_800A5560.c'}
HISTORICAL = 'cloud/work/frontier/w9d/func_800A7480/a.c'
OLD_PACK = 'GPACK_RGBA5551(red, green, blue, 1)'
NEW_PACK = '(u16)(((red<<8)&0xf800)|((green<<3)&0x7c0)|((blue>>2)&0x3e))|1'

EXTENTS = {NAME: 136, 'func_800A5560': 28}
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
NEGATIVES = {'cast_only': (136, None, 33, 0, 0, 0), 'natural_void': (136, None, 33, 0, 0, 0), 'return_only': (136, None, 9, 0, 0, 0), 'wrong_alpha': (132, None, 11, 0, 0, 0), 'wrong_green_mask': (136, None, 1, 0, 0, 0), 'wrong_min': (136, None, 26, 0, 0, 0), 'wrong_return': (136, None, 33, 0, 0, 0)}
SEMANTICS = '262144 state/return cases across four windows and every signed-short value; 16777216 exhaustive RGB triples; unsigned native-shift helper model; UBSan clean; checksum=0'
SEMANTIC_CONTROLS = {'wrong_alpha': 'mutation rejected with exact differential exit code and message', 'wrong_green_mask': 'mutation rejected with exact differential exit code and message', 'wrong_min': 'mutation rejected with exact differential exit code and message', 'wrong_return': 'mutation rejected with exact differential exit code and message'}

def require(condition, message):
    if not condition: raise RuntimeError(message)

def digest(b): return hashlib.sha256(b).hexdigest()
def recipe(): return json.loads((HERE / 'group.json').read_text())
def source(): return (HERE / 'candidate.c').read_text()
def tokens(s): return re.sub(r'\s+', '', re.sub(r'/\*.*?\*/|//[^\n]*', '', s, flags=re.S))
def once(s, a, b):
    if s.count(a) != 1: raise RuntimeError('ambiguous source anchor: ' + a)
    return s.replace(a, b, 1)

def baseline():
    s = (ROOT / HISTORICAL).read_text()
    return once(s, 'void func_800A5560(u16 color)\n{\n    D_80124FC8 = (color << 16) | color;\n}', 'void func_800A5560(u16 color);')

def natural_return(s):
    return once(s, 'void func_800A7480(', 'Descriptor72 *func_800A7480(').rstrip()[:-1] + '    return &D_8017A510[index];\n}\n'

def donor_types(s):
    return once(once(s, 's32 index)', 'u32 index)'), 's16 range_start, range_end;', 's16 range_start; u16 range_end;')

def donor_local(s):
    s = once(s, '{\n    D_8017A510[index].range_start', '{\n    Descriptor72 *win = &D_8017A510[index];\n    win->range_start')
    return s.replace('D_8017A510[index].', 'win->').replace('return &D_8017A510[index];', 'return win;')

def verify_source_change():
    s = donor_local(donor_types(once(natural_return(baseline()), OLD_PACK, NEW_PACK)))
    if tokens(s) != tokens(source()): raise RuntimeError('source exceeds genuine return, narrowing, type and local changes')

def materialize(d):
    for dest, p in CONTEXT.items(): shutil.copyfile(ROOT / p, d / dest)
    for n in ['candidate.c', 'group.json']: shutil.copyfile(HERE / n, d / n)

def inputs():
    targets, syms = score.targets(), score.image_symbols()
    return dict(protocol_version=3, population='game_blob', compiler='ido5.3', recipe=recipe(),
        packet_sha256={p: digest((HERE / p).read_bytes()) for p in ['candidate.c', 'group.json', 'replay.py', 'semantics.c', 'README.md']},
        context_sha256={p: digest((ROOT / p).read_bytes()) for p in sorted([HISTORICAL] + list(CONTEXT.values()))},
        targets={n: dict(address=hex(syms[n]), bytes=len(targets[n])*4, sha256=digest(b''.join(struct.pack('>I', w) for w in targets[n]))) for n in STRICT},
        tooling_sha256={p: digest((ROOT / p).read_bytes()) for p in ['tools/cloud/score.py', 'tools/cloud/owndata.py', 'blob_matched.lock.json']},
        compiler_sha256={n: digest(Path(score.ido(n)).read_bytes()) for n in ['cc', 'cfe', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1']})

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

def root_frame(words, start, extent):
    frames = [65536-(w&65535) for w in words[start:start+extent//4] if w>>16 == 0x27bd and w&0x8000]
    return frames[0] if frames else None

def layout(obj):
    words = score.text_words(obj); start = score.symbols(obj)[NAME] // 4
    elf = owndata._Object(obj)
    funcs = [sym for i, sec in enumerate(elf.sections) if sec['type'] == owndata.SHT_SYMTAB
             for sym in elf.symbols(i) if sym['type'] == owndata.STT_FUNC and 0 < sym['section'] < len(elf.sections)]
    root_bytes = next(sym['size'] for sym in funcs if sym['name'] == NAME)
    return dict(emitted_functions=sorted(sym['name'] for sym in funcs),
        function_extents={sym['name']: dict(offset=sym['value'], bytes=sym['size']) for sym in funcs},
        root_frame_bytes=root_frame(words, start, root_bytes))

def group_result(obj):
    return dict(scores={n: result(obj, n) for n in STRICT}, sections=sections(obj), layout=layout(obj))

def semantic_check(d, mutation=None):
    s = baseline(); body = s[s.index('void func_800A7480('):]
    (d / 'historical_body.inc').write_text(body.replace(NAME, 'historical_setfog'))
    for n in ['candidate.c', 'semantics.c']: shutil.copyfile(HERE / n, d / n)
    if mutation is not None: (d / 'candidate.c').write_text(mutation)
    exe = d / 'semantics'
    subprocess.run([shutil.which('cc') or 'cc', '-std=c11', '-O2', '-fno-strict-aliasing', '-fsanitize=undefined', '-fno-sanitize-recover=undefined', str(d / 'semantics.c'), '-lm', '-o', str(exe)], check=True, capture_output=True, text=True)
    run = subprocess.run([str(exe)], capture_output=True, text=True)
    if mutation is not None:
        if run.returncode != 3 or run.stderr != 'setfog differential mismatch\n' or run.stdout:
            raise RuntimeError('mutation did not produce the expected differential failure: ' + repr((run.returncode, run.stdout, run.stderr)))
        return 'mutation rejected with exact differential exit code and message'
    if run.returncode or run.stderr: raise RuntimeError('host semantics failed: ' + repr((run.returncode, run.stdout, run.stderr)))
    return run.stdout.strip()

def verify():
    verify_source_change(); spec = recipe(); s = source(); old = baseline()
    if spec['members'] != [NAME] or spec['claims'] != [NAME] or spec['context'] != STRICT[1:]: raise RuntimeError('complete claims/context changed')
    direct = donor_types(once(natural_return(old), OLD_PACK, NEW_PACK))
    variants = dict(natural_void=old, return_only=natural_return(old), cast_only=once(old, OLD_PACK, NEW_PACK),
        wrong_alpha=once(s, NEW_PACK, NEW_PACK[:-1]+'0'),
        wrong_return=once(s, 'return win;', 'return &D_8017A510[0];'),
        wrong_green_mask=once(s, NEW_PACK, NEW_PACK.replace('0x7c0', '0x3c0')),
        wrong_min=once(s, 'win->range_start = range_start;', 'win->range_start = range_end;'))
    report = dict(inputs=inputs(), positive={}, negative_controls={}, preserved_helpers=STRICT[1:],
        new_individually_strict_bytes=136, accepted_image_bytes=0, whole_unit_gate='not run', image_gate='not run', rom_gate='not run',
        semantic_scope='Valid window indices and storage; all RGB byte triples and all signed-short values are covered. Host background helper uses defined unsigned 32-bit shifts to model native operations; the unchanged canonical helper spelling is not an ISO-C correctness claim.')
    with tempfile.TemporaryDirectory(prefix='rush-setfog-') as tmp:
        d=Path(tmp);materialize(d)
        for label, candidate in [('first',s),('repeat',s),('direct_index',direct)]:
            (d/'candidate.c').write_text(candidate);obj=d/(label+'.o');score.compile_group(d,obj);report['positive'][label]=group_result(obj)
        for label,candidate in variants.items():
            (d/'candidate.c').write_text(candidate);obj=d/(label+'.o');score.compile_group(d,obj);report['negative_controls'][label]=group_result(obj)
        report['host_semantics']=semantic_check(d)
        report['host_semantic_controls']={label:semantic_check(d,variants[label]) for label in ['wrong_alpha','wrong_return','wrong_green_mask','wrong_min']}
    verify_report(report)
    return report

def verify_report(report):
    require(report['inputs'] == inputs(), 'source, target, compiler or tooling binding changed')
    spec = report['inputs']['recipe']
    require(spec['members'] == [NAME] and spec['claims'] == [NAME] and spec['context'] == STRICT[1:]
            and spec['keep'] == STRICT and spec['files'] == ['candidate.c', 'fill.c']
            and spec['flags'] == FLAGS, 'complete recipe or claims changed')
    require({n: report['inputs']['targets'][n]['bytes'] for n in STRICT} == EXTENTS, 'protected target extents changed')
    require(report['new_individually_strict_bytes'] == 136 and report['accepted_image_bytes'] == 0, 'false byte credit')
    require(report['preserved_helpers'] == STRICT[1:], 'preserved helper claims changed')
    require(all(report[k] == 'not run' for k in ['whole_unit_gate', 'image_gate', 'rom_gate']), 'false integration credit')
    pos = report['positive']; neg = report['negative_controls']
    require(set(pos) == {'first', 'repeat', 'direct_index'} and set(neg) == set(NEGATIVES), 'build inventory changed')
    require(pos['first'] == pos['repeat'] == pos['direct_index'], 'positive object sections differ')
    for label, group in list(pos.items()) + list(neg.items()):
        positive = label in pos
        require(set(group['scores']) == set(STRICT), 'score inventory changed: ' + label)
        layout = group['layout']; extents = layout['function_extents']
        require(layout['emitted_functions'] == sorted(STRICT) and set(extents) == set(STRICT), 'emitted function inventory changed: ' + label)
        expected_extents = dict(EXTENTS)
        if not positive: expected_extents[NAME] = NEGATIVES[label][0]
        require({n: extents[n]['bytes'] for n in STRICT} == expected_extents, 'function extent changed: ' + label)
        require(layout['root_frame_bytes'] == (None if positive else NEGATIVES[label][1]), 'root frame changed: ' + label)
        for n, row in group['scores'].items():
            require(row['target_words'] == EXTENTS[n]//4, 'target word count changed: ' + label + ':' + n)
            strict = row['differing_words'] == 0 and row['extra_nonzero_words'] == 0 and not row['unresolved'] and not row['unverified'] and not row['errors']
            require(row['strict_match'] is strict, 'inconsistent strict fields: ' + label + ':' + n)
            if positive or n != NAME:
                require(strict, 'preserved strict closure failed: ' + label + ':' + n)
            else:
                require(not strict, 'negative incorrectly accepted: ' + label + ':' + n)
        if not positive:
            row = group['scores'][NAME]
            require((row['differing_words'], row['extra_nonzero_words'], len(row['unverified']), len(row['errors'])) == NEGATIVES[label][2:]
                    and not row['unresolved'], 'negative outcome changed: ' + label)
    require(report['host_semantics'] == SEMANTICS and report['host_semantic_controls'] == SEMANTIC_CONTROLS, 'semantic scope or outcome changed')

def checked_verify():
    saved = json.loads((HERE / 'proof.json').read_text())
    verify_report(saved)
    report = verify()
    require(report == saved, 'fresh receipt differs')
    return report

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--check',action='store_true');ap.add_argument('--materialize',type=Path);args=ap.parse_args()
    if args.materialize:
        args.materialize.mkdir(parents=True,exist_ok=True);materialize(args.materialize);return
    report=checked_verify() if args.check else verify()
    print(json.dumps(report,indent=2,sort_keys=True))

if __name__=='__main__':main()
