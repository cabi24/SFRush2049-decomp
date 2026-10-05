#!/usr/bin/env python3
"""Rebuild the genuine F207C caller group; save metadata, never native bytes.

The expanded-context experiment uses only current accepted source files. Its
policy condition calls the unchanged shadow gate's transform with only the two
already-existing inline blockers. The untransformed condition is also recorded.
No new blocker, flag, target, shared context, or scorer change is introduced.
"""
import argparse
import contextlib
import dataclasses
import hashlib
import io
import json
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
from tools.conveyor.pipeline import blob_unit

FN = 'func_800F207C'
CALLEES = ['audio_distance_atten', 'audio_doppler', 'resource_type_select',
           'func_800F084C', 'func_800F1D04']
EXISTING_BLOCKERS = {'audio_distance_atten', 'audio_doppler'}
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'


def sha(data):
    return hashlib.sha256(data).hexdigest()


def function_symbol(obj, name):
    data, sections = score._elf(obj)
    matches = [symbol for i, section in enumerate(sections) if section['type'] == 2
               for symbol in score._symbol_table(data, sections, i)
               if symbol['name'] == name and symbol['type'] == 2]
    assert len(matches) == 1, (name, matches)
    return matches[0]


def inspect(obj, name):
    symbol = function_symbol(obj, name)
    start, size = symbol['value'], symbol['size']
    end = start + size
    words = score.text_words(obj)
    resolved, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, end, score.image_symbols())
    body = resolved[start // 4:end // 4]
    native = score.targets()[name]
    with contextlib.redirect_stdout(io.StringIO()):
        comparison = score.compare(obj, name, show=0)
    receipt = dataclasses.asdict(comparison)
    receipt.update(elf_function_bytes=size, native_bytes=4 * len(native),
                   full_extent_relocated_equal=(body == native and not masks and not unresolved
                                                 and not unverified and not errors),
                   resolved_body_sha256=sha(struct.pack('>%dI' % len(body), *body)),
                   native_sha256=sha(struct.pack('>%dI' % len(native), *native)),
                   frame_bytes=next((65536 - (w & 65535) for w in body[:30]
                                     if w >> 16 == 0x27BD and w & 32768), 0))
    return receipt


def gnu_link_equal(obj, directory):
    # F207C is the first complete symbol; the caller is deliberately not claimed.
    assert function_symbol(obj, FN)['value'] == 0
    addresses = score.image_symbols()
    data, sections = score._elf(obj)
    undefined = [s for i, section in enumerate(sections) if section['type'] == 2
                 for s in score._symbol_table(data, sections, i)
                 if s['section'] == 0 and s['name']]
    definitions = []
    for symbol in undefined:
        name = symbol['name']
        address = addresses.get(name, score.address_named(name))
        assert address is not None, name
        definitions.append('%s = 0x%08X;' % (name, address))
    script = directory / 'proof.ld'
    script.write_text('SECTIONS { .text 0x800F207C : SUBALIGN(4) { *(.text) } }\n' + '\n'.join(definitions))
    elf, binary = directory / 'proof.elf', directory / 'proof.text'
    subprocess.run(['mips-linux-gnu-ld', '-T', str(script), '-o', str(elf), str(obj)],
                   capture_output=True, text=True, check=True)
    subprocess.run(['mips-linux-gnu-objcopy', '-O', 'binary', '--only-section=.text',
                    str(elf), str(binary)], capture_output=True, text=True, check=True)
    symbol = function_symbol(elf, FN)
    assert symbol['value'] == 0x800F207C and symbol['size'] == 1684
    native = struct.pack('>421I', *score.targets()[FN])
    return binary.read_bytes()[:symbol['size']] == native


def verify(directory):
    directory.mkdir(parents=True, exist_ok=True)
    spec = json.loads((HERE / 'group/group.json').read_text())
    source_paths = ['group/' + name for name in spec['files']] + ['group/group.json', 'semantic_test.c']
    result = {'base_commit': 'f88dc3cb3807b8be3246b719a9b1e008210b477c',
              'status': 'MATCH_IN_GENUINE_CALLER_GROUP', 'claims': [FN],
              'accepted_byte_gain': 0, 'flags': FLAGS,
              'source_sha256': {p: sha((HERE / p).read_bytes()) for p in source_paths},
              'tools_sha256': {p: sha((ROOT / p).read_bytes()) for p in
                               ['tools/cloud/score.py', 'tools/cloud/owndata.py',
                                'tools/conveyor/pipeline/blob_unit.py', 'src/blob/unit_overrides.json']},
              'compiler_sha256': {n: sha(Path(score.ido(n)).read_bytes()) for n in
                                  ['cc', 'cfe', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1']},
              'controls': {}}
    obj = directory / 'genuine_pair.o'
    score.compile_group(HERE / 'group', obj)
    result['comparison'] = inspect(obj, FN)
    result['caller_nonmatch'] = inspect(obj, 'billboard_render')
    result['independent_gnu_link_equal'] = gnu_link_equal(obj, directory)
    for opt in ['O2', 'O3']:
        obj = directory / ('single_' + opt + '.o')
        score.compile_single(HERE / 'group/func_800F207C.c', FLAGS.replace('O3', opt), obj)
        result['controls']['single_' + opt] = inspect(obj, FN)
    existing = {entry['name'] for entry in blob_unit.load_overrides()['inline_blockers']}
    assert EXISTING_BLOCKERS <= existing
    result['direct_callee_sha256'] = {n: sha((ROOT / 'src/blob' / (n + '.c')).read_bytes())
                                      for n in CALLEES}
    for policy in [False, True]:
        group = directory / ('expanded_existing_policy' if policy else 'expanded_unchanged')
        group.mkdir()
        expanded = dict(spec)
        expanded['files'] = list(spec['files'])
        expanded['keep'] = list(spec['keep'])
        for name in spec['files']:
            shutil.copy2(HERE / 'group' / name, group / name)
        for name in CALLEES:
            filename = name + '.c'
            source = (ROOT / 'src/blob' / filename).read_text()
            if policy and name in EXISTING_BLOCKERS:
                source, _ = blob_unit.transform(source, {name}, block=[name])
            (group / filename).write_text(source)
            expanded['files'].append(filename)
            expanded['keep'].append(name)
        (group / 'group.json').write_text(json.dumps(expanded))
        obj = group / 'group.o'
        score.compile_group(group, obj)
        result['controls'][group.name] = {n: inspect(obj, n) for n in [FN] + CALLEES}
    executable = directory / 'host'
    subprocess.run(['cc', '-std=c89', '-O1', '-Wall', '-Wextra', '-Werror',
                    '-Wno-pointer-to-int-cast', '-fsanitize=undefined', '-fno-sanitize-recover=all',
                    str(HERE / 'semantic_test.c'), '-o', str(executable)], check=True)
    result['host_test'] = subprocess.run([str(executable)], check=True, capture_output=True,
                                         text=True).stdout.strip()
    assert result['comparison']['full_extent_relocated_equal']
    assert result['independent_gnu_link_equal']
    assert all(v['full_extent_relocated_equal'] for v in
               result['controls']['expanded_existing_policy'].values())
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--write', action='store_true')
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='billboard-proof-') as tmp:
        result = verify(Path(tmp))
    if args.write:
        (HERE / 'verification.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps({'status': result['status'], 'comparison': result['comparison'],
                      'independent_gnu_link_equal': result['independent_gnu_link_equal'],
                      'host_test': result['host_test']}, indent=2))
