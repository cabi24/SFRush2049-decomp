#!/usr/bin/env python3
"""Fresh, read-only NONMATCH proof. Never writes native words into the packet."""
import argparse
import dataclasses
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
SOURCES = {'func_800DC628': 'cloud/work/heads_B14/func_800DC628_wordsize.c',
           'func_800D4DFC': 'cloud/work/tiny_A62/func_800D4DFC.c'}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def extent(obj, name):
    data, sections = score._elf(obj)
    text = score._text_index(sections)
    symbols = [symbol for i, sec in enumerate(sections) if sec['type'] == 2
               for symbol in score._symbol_table(data, sections, i)
               if symbol['name'] == name and symbol['section'] == text and symbol['type'] == 2]
    assert len(symbols) == 1
    return symbols[0]['size']


def comparison(obj, name):
    result = dataclasses.asdict(score.compare(obj, name, show=0))
    result['elf_function_bytes'] = extent(obj, name)
    return result


def proof(name, work):
    source = HERE / (name + '.c')
    obj = work / (name + '.o')
    score.compile_single(source, FLAGS, obj)
    want = score.targets()[name]
    expected = len(want) * 4
    assert extent(obj, name) == expected == 248
    baseline = work / (name + '.baseline.o')
    score.compile_single(ROOT / SOURCES[name], FLAGS, baseline)
    addresses = score.image_symbols()
    start = addresses[name]
    script = work / (name + '.ld')
    own = '.rodata 0x801241A4 : SUBALIGN(4) { *(.rodata) }' if name == 'func_800D4DFC' else ''
    script.write_text('SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } %s }\n' % (start, own)
                      + ''.join('%s = 0x%x;\n' % (key, value) for key, value in addresses.items()
                                if key != name))
    linked, binary = work / (name + '.elf'), work / (name + '.bin')
    subprocess.run(['mips-linux-gnu-ld', '-EB', '-T', str(script), '-o', str(linked), str(obj)],
                   check=True, capture_output=True)
    subprocess.run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.text', str(linked), str(binary)], check=True)
    data = binary.read_bytes()
    assert extent(linked, name) == expected
    assert not any(data[expected:])
    words = struct.unpack('>%dI' % len(want), data[:expected])
    residual = [i * 4 for i, (actual, target) in enumerate(zip(words, want)) if actual != target]
    strict = comparison(obj, name)
    assert len(residual) == strict['differing']
    assert not strict['unresolved'] and not strict['unverified'] and not strict['errors']
    assert strict['extra_words'] == 0
    literal = None
    if name == 'func_800D4DFC':
        blob = work / 'literal.bin'
        subprocess.run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.rodata', str(linked), str(blob)], check=True)
        natural = struct.pack('>f', 2.72727275)
        assert blob.read_bytes()[:4] == natural == score.own_data().read(0x801241A4, 4)
        literal = {'address': '0x801241A4', 'value': 2.72727275, 'bytes_verified': 4}
    return {'status': 'NONMATCH', 'source_sha256': sha(source.read_bytes()), 'flags': FLAGS,
            'start': hex(start), 'end_exclusive': hex(start + expected),
            'target_sha256': sha(struct.pack('>%dI' % len(want), *want)),
            'baseline_source': SOURCES[name], 'baseline': comparison(baseline, name),
            'comparison': strict, 'independent_gnu_link_verified': True,
            'linked_code_sha256': sha(data[:expected]), 'alignment_bytes': len(data) - expected,
            'differing_word_offsets': residual, 'own_literal': literal}


def host_checks(work):
    command = ['cc', '-std=c99', '-O1', '-g', '-Wall', '-Wextra', '-Werror',
               '-Wno-pointer-to-int-cast', '-fno-strict-aliasing', '-ffp-contract=off',
               '-fsanitize=address,undefined', '-fno-sanitize-recover=all']
    exe = work / 'host'
    subprocess.run(command + [str(HERE / 'host.c'), '-o', str(exe)], check=True, capture_output=True)
    env = dict(os.environ, ASAN_OPTIONS='detect_leaks=0')
    run = subprocess.run([str(exe)], check=True, capture_output=True, text=True, env=env)
    assert run.stdout.strip() == 'packing_cases=4608 snapshot_cases=4096'
    controls = {}
    for label, filename, before, after in [
        ('wrong_coefficient', 'func_800D4DFC.c', '*2.72727275f', '*2.0f'),
        ('wrong_position_reset', 'func_800DC628.c', 'D_801170F4=0;', 'D_801170F4=1;')]:
        directory = work / label
        directory.mkdir()
        for source in ['host.c', 'func_800DC628.c', 'func_800D4DFC.c']:
            shutil.copyfile(HERE / source, directory / source)
        candidate = directory / filename
        text = candidate.read_text()
        assert before in text
        candidate.write_text(text.replace(before, after))
        binary = directory / 'host'
        subprocess.run(command + [str(directory / 'host.c'), '-o', str(binary)], check=True, capture_output=True)
        failed = subprocess.run([str(binary)], capture_output=True, text=True, env=env)
        assert failed.returncode != 0 and 'Assertion' in failed.stderr
        controls[label] = 'rejected_by_semantic_assertion'
    return {'host_source_sha256': sha((HERE / 'host.c').read_bytes()),
            'packing_cases': 4608, 'snapshot_cases': 4096,
            'asan_ubsan': 'passed', 'negative_controls': controls,
            'limit': 'Bounded host/reference checks, not native execution or full-game verification'}


def verify(work):
    result = {'status': 'NONMATCH', 'claims': [], 'accepted_byte_gain': 0,
              'base_commit': 'f88dc3cb3807b8be3246b719a9b1e008210b477c',
              'compiler_sha256': sha((score.IDO / 'cc').read_bytes()),
              'target_manifest_sha256': sha((score.ASM_DIR / 'SHA256SUMS').read_bytes()),
              'targets': {name: proof(name, work) for name in SOURCES}}
    # Real accepted callee, copied verbatim to a temporary two-function unit.
    group = work / 'real_callee_group'
    group.mkdir()
    shutil.copyfile(HERE / 'func_800D4DFC.c', group / 'candidate.c')
    shutil.copyfile(ROOT / 'src/blob/math_utility.c', group / 'math_utility.c')
    (group / 'group.json').write_text(json.dumps({'files': ['candidate.c', 'math_utility.c'],
        'flags': FLAGS, 'members': ['func_800D4DFC'], 'context': ['math_utility'],
        'keep': ['func_800D4DFC', 'math_utility'], 'claims': []}))
    obj = work / 'group.o'
    score.compile_group(group, obj)
    result['accepted_context'] = {'source_sha256': sha((group / 'math_utility.c').read_bytes()),
        'caller': comparison(obj, 'func_800D4DFC'), 'math_utility': comparison(obj, 'math_utility')}
    assert score.compare(obj, 'math_utility', show=0).accepted()
    for name, path in [('func_800A43FC', 'cloud/work/game_C33/func_800A43FC.c'),
                       ('battle_mode_setup', 'cloud/work/near_miss_B64/battle_mode_setup_pair.c')]:
        obj = work / (name + '.o')
        score.compile_single(ROOT / path, FLAGS, obj)
        result.setdefault('stopped_controls', {})[name] = comparison(obj, name)
    result['host_semantics'] = host_checks(work)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='fresh-small-proof-') as tmp:
        result = verify(Path(tmp))
    text = json.dumps(result, indent=2) + '\n'
    if args.output:
        args.output.write_text(text)
    print(text, end='')


if __name__ == '__main__':
    main()
