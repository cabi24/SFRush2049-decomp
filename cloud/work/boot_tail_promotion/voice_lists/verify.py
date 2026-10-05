#!/usr/bin/env python3
"""Source-only replay of voice/list promotion contracts. No promotion or relock.

Compiles actual baseline/current ROM TUs, then overlays remaining candidates in
scratch copies using the promotion context/deduplication procedure. Every slot
gets full-body preservation comparison; every C body gets strict relocated
comparison and exact ELF extent. Unchanged assembly remains a baseline check.
"""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import shutil
import subprocess
import struct
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor.pipeline.lock import load_lock, normalize_body
from tools.conveyor.seeds.extract_candidates import extract_functions, extract_named_function

BASE = 'cf10b3392d7f00ae42d75c008b79fdc2541aab6b'
SOURCES = Path('cloud/work/boot_tail_promotion/voice_lists/sources')
LEGACY_SOURCES = Path('cloud/work/boot_tail_promotion/sources')
SCOPE = {
    'lib_1a660': {
        'candidates': ['8001B29C', '8001B3A0', '8001B7C0', '8001B8C4', '8001B968'],
        'locked': ['80019AA8', '80019BE4', '80019C8C', '8001B154', '8001B744']},
    'lib_1f5b0': {
        'candidates': ['8001EB10', '8001ECE0', '8001F9D0'],
        'locked': ['8001EAA0', '8001EAEC', '8001EDF4', '8001EE9C', '8001F7EC',
                   '8001F864', '8001F898', '8001FA18', '8001FAE4']}}
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
TU_FLAGS = ['-G', '0', '-mips2', '-O2', '-non_shared', '-Iinclude', '-Iinclude/PR',
            '-D_LANGUAGE_C', '-Wab,-r4300_mul', '-Xcpluscomm', '-Isrc/rom']
DATA = {'.data', '.rodata', '.bss', '.sdata', '.sbss', '.rdata', '.lit4', '.lit8'}


def sha(value):
    return hashlib.sha256(value).hexdigest()


def old(path):
    return subprocess.check_output(['git', 'show', BASE + ':' + str(path)],
                                   cwd=ROOT, text=True)


def bodies(text):
    return {fn: text[start:end] for fn, start, end in extract_functions(text)}


def body_hash(text):
    return sha(normalize_body(text).encode())


def slots(text):
    return set(bodies(text)) | set(re.findall(r'GLOBAL_ASM\("[^\"]+/(func_[0-9A-F]+)\.s"\)', text))


def pragma(tu, fn):
    return '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/' + tu + '/' + fn + '.s")'


def current_state(tu, text, locks):
    """Allow later genuine promotions, while failing missing/changed locked C."""
    path = Path('src/rom') / (tu + '.c')
    baseline = old(path)
    base_locks = json.loads(old('matched.lock.json'))
    current_bodies = bodies(text)
    for address in SCOPE[tu]['locked']:
        fn = 'func_' + address
        key = str(path) + ':' + fn
        if fn not in current_bodies or key not in locks:
            raise AssertionError(fn + ': original locked body/lock missing')
        if body_hash(current_bodies[fn]) != base_locks[key]['body_sha256']:
            raise AssertionError(fn + ': original locked body changed')
    for fn, body in current_bodies.items():
        key = str(path) + ':' + fn
        if key not in locks or body_hash(body) != locks[key]['body_sha256']:
            raise AssertionError(fn + ': current locked body mismatch')
    if slots(text) != slots(baseline):
        raise AssertionError(tu + ': slot population changed')
    pending = []
    for address in SCOPE[tu]['candidates']:
        fn = 'func_' + address
        count = text.count(pragma(tu, fn))
        if count == 1 and fn not in current_bodies:
            pending.append(fn)
        elif count != 0 or fn not in current_bodies:
            raise AssertionError(fn + ': missing/duplicate candidate slot')
    return pending, sorted(current_bodies), sorted(slots(text))


def context():
    spec = importlib.util.spec_from_file_location('voice_list_context',
        ROOT / 'cloud/work/boot_tail_promotion/context_check.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.CC = score.IDO / 'cc'
    return module


def fit(paths, work):
    checker = context()
    rows = {fn: checker.check(fn, work, str(path)) for fn, path in paths.items()}
    for fn, row in rows.items():
        if row['status'] != 'ok':
            raise AssertionError(fn + ': ' + str(row))
    return rows


def splice(tu, text, paths, rows):
    for fn, path in paths.items():
        marker = pragma(tu, fn)
        if text.count(marker) != 1:
            raise AssertionError(fn + ': expected one passthrough')
        have = {' '.join(line.split()) for line in text.splitlines() if line.strip()}
        declarations = [s for s in rows[fn]['preamble']
                        if s.startswith('#') or ' '.join(s.split()) not in have]
        block = ''.join(s + '\n' for s in declarations) + extract_named_function(path, fn)
        text = text.replace(marker, block, 1)
    return text


def build(text, work, label, expected_error=None):
    source, obj = work / (label + '.c'), work / (label + '.o')
    source.write_text(text)
    command = [sys.executable, 'tools/asm-processor/build.py', str(score.IDO / 'cc'),
               '--', 'mips-linux-gnu-as', '-march=vr4300', '-mabi=32', '-Iinclude',
               '--', '-c', *TU_FLAGS, '-o', str(obj), str(source)]
    proc = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if expected_error:
        if proc.returncode == 0 or expected_error not in proc.stderr:
            raise AssertionError(label + ': historical refusal not reproduced\n' + proc.stderr)
        return expected_error
    if proc.returncode or not obj.is_file():
        raise AssertionError(label + ': compile failed\n' + proc.stdout + proc.stderr)
    return obj


def compare_all(obj, names, extents, c_names):
    symbols = score.symbols(obj)
    if set(symbols) != set(names):
        raise AssertionError('compiled TU membership differs from actual source')
    data, sections = score._elf(obj)
    text_index = score._text_index(sections)
    sizes = {s['name']: s['size'] for i, section in enumerate(sections)
             if section['type'] == 2 for s in score._symbol_table(data, sections, i)
             if s['section'] == text_index and s['type'] == 2}
    if any(s['size'] for s in sections if s['name'] in DATA):
        raise AssertionError('unexpected allocated data')
    results = {}
    for fn in sorted(names):
        result = score.compare(obj, fn, show=0)
        if fn in c_names and not result.accepted(False):
            raise AssertionError(fn + ': ' + result.summary())
        if result.total * 4 != extents[fn]['size']:
            raise AssertionError(fn + ': target extent mismatch')
        if fn in c_names and sizes[fn] != extents[fn]['size']:
            raise AssertionError(fn + ': ELF function extent mismatch')
        hashes = {}
        if result.accepted(False):
            start = symbols[fn]
            want = score.targets()[fn]
            got, masks, unresolved, unverified, errors = score.relocate(
                obj, score.text_words(obj), start, start + len(want) * 4,
                score.image_symbols())
            got = got[start // 4:start // 4 + len(want)]
            if masks or unresolved or unverified or errors or got != want:
                raise AssertionError(fn + ': exact unmasked full-body proof failed')
            hashes = {'target_sha256': sha(struct.pack('>' + str(len(want)) + 'I', *want)),
                      'relocated_body_sha256': sha(struct.pack('>' + str(len(got)) + 'I', *got))}
        results[fn] = dict(asdict(result), function_symbol_bytes=sizes[fn],
                           target_bytes=extents[fn]['size'], strict_match=result.accepted(False),
                           **hashes)
    return results


def passthrough_signature(obj, fn, extent):
    """Compare untouched assembly words and symbolic relocation identities.

    A historical jump-table alias is absent from the protected scorer symbol
    manifest. Do not manufacture its address, alter the scorer, or certify an
    unresolved body as a match. Its unchanged bytes/references are instead
    checked against the pinned baseline assembly under the same recipe.
    """
    start = score.symbols(obj)[fn]
    data, sections = score._elf(obj)
    text_index = score._text_index(sections)
    relocations = []
    for section in sections:
        if section['type'] != 9 or section['info'] != text_index:
            continue
        symbols = score._symbol_table(data, sections, section['link'])
        for i in range(section['size'] // 8):
            offset, info = struct.unpack_from('>II', data, section['off'] + 8 * i)
            if start <= offset < start + extent:
                relocations.append((offset - start, info & 255, symbols[info >> 8]['name']))
    return (score.text_words(obj)[start // 4:(start + extent) // 4],
            sorted(relocations))


def layout_proofs(paths, work):
    a = 'VoiceState_80019C8C'
    definitions = {
        'func_8001B968': {a: (416, {'child': 16, 'parent': 20, 'flags': 36,
            'channel': 74, 'set': 75, 'base_key': 78, 'key': 80, 'channel55': 85,
            'identifier': 96, 'glide': 140, 'pitch': 148, 'cents': 192,
            'original_key': 193, 'valueC2': 194})},
        'func_8001EB10': {
            'SequenceNode': (16, {'next': 0, 'previous': 4, 'key': 8, 'value': 12}),
            'VoiceState': (416, {'command00': 0, 'next_identifier': 16,
                'parent_identifier': 20, 'entry18': 24, 'flags24': 36,
                'value28': 40, 'external4C': 76, 'identifier60': 96, 'activeBD': 189})}}
    for fn, layouts in definitions.items():
        checks = ['sizeof(void *) == 4', 'sizeof(int) == 4', 'sizeof(u16) == 2']
        for name, (size, fields) in layouts.items():
            checks.append('sizeof(' + name + ') == ' + str(size))
            checks.extend('OFF(' + name + ', ' + field + ') == ' + str(offset)
                          for field, offset in fields.items())
        src = work / (fn + '.layout.c')
        src.write_text('#include "' + str(paths[fn]) + '"\n'
            '#define OFF(type, field) ((unsigned int)&((type *)0)->field)\n'
            'typedef char native_layout[(' + ' && '.join(checks) + ')?1:-1];\n')
        score.compile_single(src, FLAGS, src.with_suffix('.o'))
    return definitions


def verify(work, source_overrides=None, entries=None):
    if Path.cwd().resolve() != ROOT:
        raise RuntimeError('run from repository root')
    work.mkdir(parents=True, exist_ok=True)
    before = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    try:
        manifest = score.target_manifest()
        score.targets()
        extents = {r['name']: r for r in json.loads(score.verified_bytes(
            score.ASM_DIR / 'extents.json', manifest))['functions']}
        paths = {'func_' + a: ROOT / SOURCES / ('func_' + a + '.c')
                 for group in SCOPE.values() for a in group['candidates']}
        standalone = {}
        for fn, path in paths.items():
            obj = work / (fn + '.standalone.o')
            score.compile_single(path, FLAGS, obj)
            standalone[fn] = compare_all(obj, [fn], extents, [fn])[fn]
        rows = fit(paths, work)
        report = {}
        locks = load_lock() if entries is None else entries
        for tu, group in SCOPE.items():
            path = Path('src/rom') / (tu + '.c')
            source = (source_overrides or {}).get(tu, (ROOT / path).read_text())
            baseline = old(path)
            pending, current_c, names = current_state(tu, source, locks)
            base_c = sorted(bodies(baseline))
            residual = {fn: paths[fn] for fn in pending}
            combined_text = splice(tu, source, residual, rows)
            objects = {label: build(text, work, tu + '.' + label)
                for label, text in [('baseline', baseline), ('current', source),
                                    ('combined', combined_text)]}
            results = {label: compare_all(obj, names, extents,
                base_c if label == 'baseline' else current_c if label == 'current'
                else set(current_c) | set(residual)) for label, obj in objects.items()}
            if len({len(score.text_words(obj)) for obj in objects.values()}) != 1:
                raise AssertionError(tu + ': whole-TU extent changed')
            if any(score.symbols(obj) != score.symbols(objects['baseline'])
                   for obj in objects.values()):
                raise AssertionError(tu + ': function offsets changed')
            unchanged_asm = sorted(set(names) - set(current_c) - set(residual))
            for fn in unchanged_asm:
                reference = passthrough_signature(objects['baseline'], fn, extents[fn]['size'])
                for label in ('current', 'combined'):
                    if passthrough_signature(objects[label], fn, extents[fn]['size']) != reference:
                        raise AssertionError(fn + ': unchanged assembly bytes/relocations differ')
            old_paths = {}
            for address in group['candidates']:
                fn = 'func_' + address
                legacy = work / (fn + '.legacy.c')
                legacy.write_text(old(LEGACY_SOURCES / (fn + '.c')))
                old_paths[fn] = legacy
            old_rows = fit(old_paths, work)
            error = "redeclaration of 'D_8004BEB8'" if tu == 'lib_1a660' else "redeclaration of 'D_80050C50'"
            old_control = build(splice(tu, baseline, old_paths, old_rows), work,
                                tu + '.old_conflicts', error)
            if tu == 'lib_1a660':
                mutations = {
                    'wrong_stride': ('unknownC4[220]', 'unknownC4[224]', 'func_8001B29C'),
                    'wrong_relocation': ('D_8004BEB8', 'D_8004FA50', 'func_8001B29C')}
            else:
                mutations = {'wrong_previous_offset': (
                    'struct SequenceNode *previous; u32 key;',
                    'u32 key; struct SequenceNode *previous;', 'func_8001EB10')}
            controls = {}
            for label, (search, replacement, fn) in mutations.items():
                wrong_text = combined_text.replace(search, replacement)
                if wrong_text == combined_text:
                    raise AssertionError(label + ': mutation did not apply')
                obj = build(wrong_text, work, tu + '.' + label)
                bad = score.compare(obj, fn, show=0)
                if bad.accepted(False) or not bad.differing:
                    raise AssertionError(label + ': negative escaped full-body comparison')
                controls[label] = asdict(bad)
            report[tu] = dict(results, original_locked_c=base_c,
                object_sha256={label: sha(obj.read_bytes()) for label, obj in objects.items()},
                baseline_source_sha256=sha(baseline.encode()),
                current_source_sha256=sha(source.encode()),
                combined_source_sha256=sha(combined_text.encode()),
                pending_candidates=pending,
                promoted_candidates=['func_' + a for a in group['candidates']
                                     if 'func_' + a not in pending],
                current_c=current_c, all_tu_functions=len(names),
                all_tu_function_bytes=sum(extents[f]['size'] for f in names),
                all_tu_text_bytes=len(score.text_words(objects['combined'])) * 4,
                all_function_offsets_unchanged=True, allocated_data_bytes=0,
                old_declaration_refusal=old_control, negative_controls=controls,
                unchanged_assembly_words_and_relocations=unchanged_asm,
                opaque_assembly_without_full_relocation_proof=[fn for fn, row in results['combined'].items() if not row['strict_match']])
        files = [Path('src/rom') / (tu + '.c') for tu in SCOPE]
        files += [p.relative_to(ROOT) for p in paths.values()]
        files += [LEGACY_SOURCES / p.name for p in paths.values()]
        files += [Path(p) for p in ['matched.lock.json', 'Makefile', 'src/rom/rom_tu.h',
            'include/rom_auto.h', 'include/types.h', 'include/m2c_types.h',
            'tools/cloud/score.py', 'tools/asm-processor/build.py',
            'tools/asm-processor/asm_processor.py',
            'cloud/work/boot_tail_promotion/context_check.py']]
        files += [Path(__file__).relative_to(ROOT),
            Path('cloud/work/boot_tail_promotion/voice_lists/semantics.py'),
            Path('tests/conveyor/test_voice_list_promotion.py')]
        files += [Path('cloud/work/boot_tail') / packet / 'test_semantics.py'
            for packet in ['BT03-high-larger', 'BT03-high-runtime',
                           'BT03-high-routing', 'BT03-high-runtime-followon',
                           'BT03-high-chains', 'BT03-high-init']]
        files += [Path('asm/us/boot_tail') / name for name in manifest]
        files += [Path('asm/us/boot_tail/SHA256SUMS')]
        for tu in SCOPE:
            files += sorted(Path('asm/us/nonmatchings/rom', tu).glob('*.s'))
        return {'result': 'PASS', 'baseline_commit': BASE,
            'scope': list(paths), 'candidate_bytes': sum(extents[f]['size'] for f in paths),
            'standalone_flags': FLAGS + ' ' + score.R4300_CC, 'full_tu_flags': TU_FLAGS,
            'standalone': standalone, 'header_context': rows, 'translation_units': report,
            'native_layouts': layout_proofs(paths, work),
            'source_sha256': {str(p): sha((ROOT / p).read_bytes()) for p in files},
            'compiler_sha256': {p.name: sha(p.read_bytes()) for p in sorted(score.IDO.iterdir()) if p.is_file()},
            'assembler_sha256': sha(Path(shutil.which('mips-linux-gnu-as')).read_bytes()),
            'full_rom_gate': 'NOT RUN; maintainer-controlled',
            'promotion': 'NOT PERFORMED; all candidate passthroughs/locks unchanged',
            'relocking': 'New adapted source paths require maintainer locks. Historical candidate files/locks and all fourteen existing ROM-TU body hashes remain unchanged.'}
    finally:
        score.ASM_DIR = before


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='voice-list-proof-') as tmp:
        report = verify(Path(tmp))
    text = json.dumps(report, indent=2, sort_keys=True) + '\n'
    if args.output:
        args.output.write_text(text)
    else:
        print(text, end='')


if __name__ == '__main__':
    main()
