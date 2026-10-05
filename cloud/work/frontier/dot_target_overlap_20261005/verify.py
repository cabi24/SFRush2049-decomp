#!/usr/bin/env python3
"""Source-bound whole-function, GNU-link and bounded behavioral proof. No splice."""
import argparse
import ctypes
import dataclasses
import hashlib
import importlib.util
import itertools
import json
import os
from pathlib import Path
import random
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
spec = importlib.util.spec_from_file_location('target_overlap_native', HERE / 'native.py')
native = importlib.util.module_from_spec(spec)
spec.loader.exec_module(native)
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
NAMES = {'func_8010C448': 18.0, 'func_8010C588': 4.0}
CONTEXT = ['func_8010C02C', 'func_8010C2E4', 'func_8010C7CC']
DONOR_REVISION = '845329d7b36f5a384c5625ed9a0aef584ab46139'
BASELINES = {
    'func_8010C448': 'cloud/work/dot_cylinder_predicate/func_8010C448.c',
    'func_8010C588': 'cloud/work/dot_low_cylinder/func_8010C588.c',
}

def digest(data):
    return hashlib.sha256(data).hexdigest()

def sha(path):
    return digest(Path(path).read_bytes())

def run(args, **kwargs):
    return subprocess.run(args, check=True, capture_output=True, **kwargs)

def pack(values):
    return struct.pack('>%dI' % len(values), *values)

def equivalent(a, b):
    return a == b or ((a & 0x7fffffff) > 0x7f800000 and (b & 0x7fffffff) > 0x7f800000)

def symbols(obj):
    data, sections = score._elf(obj)
    entries = [sym for i, sec in enumerate(sections) if sec['type'] == 2
               for sym in score._symbol_table(data, sections, i)]
    return data, sections, entries

def extent(obj, name):
    rows = [s for s in symbols(obj)[2] if s['name'] == name and s['type'] == 2]
    assert len(rows) == 1
    return rows[0]['size']

def comparison(obj, name):
    value = dataclasses.asdict(score.compare(obj, name, show=0))
    value['elf_function_bytes'] = extent(obj, name)
    return value

def full_body(obj, name, work):
    addresses = score.image_symbols()
    want = score.targets()[name]
    data, sections, syms = symbols(obj)
    fn = next(s for s in syms if s['name'] == name and s['type'] == 2)
    assert fn['value'] == 0 and fn['size'] == 320 and len(want) == 80
    text = next(s for s in sections if s['name'] == '.text')
    assert text['size'] == 320
    assert not any(s['size'] for s in sections if s['name'] in ('.rodata', '.data', '.bss'))
    undefined = sorted(s['name'] for s in syms if s['name'] and s['section'] == 0)
    assert undefined == ['D_8014AA3A', 'player_array']
    resolved, masks, unresolved, unverified, errors = score.relocate(
        obj, score.text_words(obj), 0, 320, addresses)
    assert not (masks or unresolved or unverified or errors)
    assert resolved == want and score.compare(obj, name, show=0).accepted()
    linker = work / 'link.ld'
    linker.write_text('SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } }\n' % addresses[name]
                      + ''.join('%s = 0x%x;\n' % (s, addresses[s]) for s in undefined))
    linked, binary = work / 'linked.elf', work / 'text.bin'
    run(['mips-linux-gnu-ld', '-EB', '-T', str(linker), '-e', name, str(obj), '-o', str(linked)])
    run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.text', str(linked), str(binary)])
    blob = binary.read_bytes()
    assert len(blob) == 320 and blob == pack(want) and extent(linked, name) == 320
    relocs = [row.strip() for row in run(['mips-linux-gnu-readelf', '-r', str(obj)], text=True).stdout.splitlines()
              if 'R_MIPS_' in row and 'R_MIPS_32' not in row]
    assert len(relocs) == 4
    # Independent type-size/offset assertions use the final C source unchanged.
    layout = work / 'layout.c'
    layout.write_text('#define offsetof(t,m) ((unsigned long)&((t *)0)->m)\n#include "candidate.c"\n'
                      'typedef char a[(sizeof(ProximityEnabled)==2056)?1:-1];\n'
                      'typedef char b[(sizeof(ProximityPlayer)==952)?1:-1];\n'
                      'typedef char c[(offsetof(ProximityPlayer,position)==8)?1:-1];\n')
    score.compile_single(layout, FLAGS, work / 'layout.o')
    assert score.compare(work / 'layout.o', name, show=0).accepted()
    return {
        'comparison': comparison(obj, name), 'function_bytes': 320, 'text_bytes': 320,
        'alignment_bytes': 0, 'owned_data_bytes': 0, 'GNU_linked_equal': True,
        'body_sha256': digest(blob), 'object_sha256': sha(obj),
        'relocations': relocs, 'O32_layout_assertions': 'passed',
    }, resolved

def corpus(upper, random_count=1500):
    values = [native.to_bits(x) for x in [-float('inf'), -upper, -3.5, -2, -0.0, 0.0, 1, 3.5, upper, float('inf')]]
    values += [0xbfffffff, 0xc0000001, native.to_bits(upper)-1, native.to_bits(upper)+1,
               1, 0x80000001, 0x7f7fffff, 0x7fc12345]
    rows = []
    for y, x, radius in itertools.product(values, [0, 0x40600000, 0x40600001, 0x405fffff],
                                        [0, 0xc0600000, 0x3f800000, 0x7fc00000]):
        rows.append([0, 0, 0, x, y, 0, radius, 0x41200000])
    rng = random.Random(0xc448 if upper == 18 else 0xc588)
    for n in range(random_count):
        data = [rng.getrandbits(32) for _ in range(8)]
        if n % 2:
            data[:7] = [native.to_bits(rng.randrange(-320, 320)/4.) for _ in range(7)]
        rows.append(data)
    return [(n % 8, [0, 1, 127, 128, 255][n % 5], values, alias)
            for n, values in enumerate(rows) for alias in range(-1, 8)]

def oracle(data, enabled, alias, upper):
    expected = list(data)
    if not enabled:
        return 0, expected
    f32 = lambda x: native.to_float(native.to_bits(x))
    p = list(map(native.to_float, data))
    dx, dy, dz = (f32(p[i+3]-p[i]) for i in range(3))
    radius = f32(p[6]+3.5)
    # Native computes the Z product, X product, sum, then radial gap.
    square = f32(f32(dz*dz)+f32(dx*dx))
    gap = f32(square-f32(radius*radius))
    if alias >= 0:
        expected[alias] = native.to_bits(gap)
    return int(not (gap > 0.0) and dy > -2.0 and dy < upper), expected

def native_case(words, start, case):
    index, enabled, data, alias = case
    flag, player = 0x8014aa3a+2056*index, 0x80152818+952*index
    regions = [(0x100000, struct.pack('>h', index)), (0x200000, pack(data[:3])),
               (0x300000, pack(data[6:7])), (0x400000, pack(data[7:])),
               (flag, bytes([enabled])),
               (player, bytes([0xA5])*8+pack(data[3:6])+bytes([0xA5])*(952-20))]
    dest = (0 if alias < 0 else 0x200000+4*alias if alias < 3 else
            player+8+4*(alias-3) if alias < 6 else 0x300000 if alias == 6 else 0x400000)
    result, memory, reads, writes, seen = native.execute(words, start, regions,
                                                       [0x100000, 0x200000, 0x300000, dest])
    memory = dict(memory)
    after = list(struct.unpack('>3I', memory[0x200000]))
    after += list(struct.unpack('>3I', memory[player][8:20]))
    after += [int.from_bytes(memory[0x300000], 'big'), int.from_bytes(memory[0x400000], 'big')]
    assert memory[0x100000] == struct.pack('>h', index) and memory[flag] == bytes([enabled])
    assert memory[player][:8] == bytes([0xA5])*8 and memory[player][20:] == bytes([0xA5])*(952-20)
    assert (0x300000, 4) in reads
    if not enabled:
        assert all(addr not in [0x200000, 0x200004, 0x200008, player+8, player+12, player+16]
                   for addr, width in reads)
    assert [w for w in writes if not 0x700000 <= w[0] < 0x700100] == ([(dest, 4)] if enabled and dest else [])
    return result, after, seen

def host_library(work, name, source=None):
    work.mkdir(exist_ok=True)
    (work / 'candidate.c').write_text(source or (ROOT / 'cloud/matches' / (name+'.c')).read_text())
    (work / 'host.c').write_bytes((HERE / 'host.c').read_bytes())
    shared = work / 'host.so'
    run(['cc', '-shared', '-fPIC', '-std=c89', '-pedantic', '-O2', '-Wall', '-Wextra', '-Werror',
         '-ffp-contract=off', '-fsanitize=undefined', '-fno-sanitize-recover=all', '-DTARGET='+name,
         str(work / 'host.c'), '-o', str(shared)])
    lib = ctypes.CDLL(str(shared))
    lib.run_case.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_uint32), ctypes.c_int]
    return lib

def behavior(name, words, linked, work, random_count=1500, mutants=True):
    upper = NAMES[name]
    cases = corpus(upper, random_count)
    host = host_library(work / 'host', name)
    seen, expectations = set(), []
    start = score.image_symbols()[name]
    for case in cases:
        index, enabled, data, alias = case
        want, expected = oracle(data, enabled, alias, upper)
        for stream in [words, linked]:
            result, after, executed = native_case(stream, start, case)
            assert result == want and all(equivalent(a, b) for a, b in zip(after, expected)), case
            seen |= executed
        array = (ctypes.c_uint32*8)(*data)
        result = host.run_case(index, enabled, array, alias)
        assert result == want and all(equivalent(a, b) for a, b in zip(array, expected)), case
        expectations.append((want, expected))
    # The duplicate mtc1 site is unreachable after branch/likely copying.
    unreachable = sorted(set(range(0, 320, 4)) - seen)
    assert unreachable == [0xfc], unreachable
    negatives = {}
    if mutants:
        source = (ROOT / 'cloud/matches' / (name+'.c')).read_text()
        controls = {
            'radial_boundary_excluded': source.replace('gap > 0.0f', 'gap >= 0.0f'),
            'lower_boundary_included': source.replace('delta[1] > -2.0f', 'delta[1] >= -2.0f'),
            'upper_boundary_included': source.replace('delta[1] < ', 'delta[1] <= '),
            'wrong_radius': source.replace('radius += 3.5f', 'radius += 3.0f'),
            'wrong_plane': source.replace('delta[2] * delta[2]', 'delta[1] * delta[1]'),
        }
        for label, altered in controls.items():
            assert altered != source
            lib = host_library(work / label, name, altered)
            failures = 0
            for case, (want, expected) in zip(cases, expectations):
                index, enabled, data, alias = case
                array = (ctypes.c_uint32*8)(*data)
                result = lib.run_case(index, enabled, array, alias)
                failures += result != want or not all(equivalent(a, b) for a, b in zip(array, expected))
            assert failures > 0, label
            negatives[label] = {'rejected': True, 'differing_cases': failures}
    return {'cases': len(cases), 'native_and_GNU_runs': 2*len(cases),
            'host_C89_UBSan': 'passed', 'oracle': 'binary32_round_after_each_operation',
            'executed_instruction_offsets': len(seen), 'unreachable_offsets': unreachable,
            'memory_canaries_access_order_O32_preservation': 'passed', 'output_alias_modes': 9,
            'mutants': negatives}

def control_sources(name, work):
    source = (ROOT / 'cloud/matches' / (name+'.c')).read_text()
    controls = {'historical': (ROOT / BASELINES[name]).read_text(),
                'radius_before_distance': source.replace(
                    '    distance_squared = delta[0] * delta[0] + delta[2] * delta[2];\n    radius += 3.5f;',
                    '    radius += 3.5f;\n    distance_squared = delta[0] * delta[0] + delta[2] * delta[2];'),
                'reverse_source_sum': source.replace('delta[0] * delta[0] + delta[2] * delta[2]',
                                                     'delta[2] * delta[2] + delta[0] * delta[0]'),
                'vectors_before_scalars': source.replace('    float distance_squared, gap;\n    float delta[3], position[3];',
                                                         '    float delta[3], position[3];\n    float distance_squared, gap;')}
    result = {}
    for label, contents in controls.items():
        assert contents != source
        path = work / (label+'.c')
        path.write_text(contents)
        obj = work / (label+'.o')
        score.compile_single(path, FLAGS, obj)
        result[label] = comparison(obj, name)
        assert result[label]['differing'] > 0
    assert result['historical']['differing'] == (45 if name.endswith('448') else 43)
    return result

def context(work):
    work.mkdir()
    names = list(NAMES)+CONTEXT
    sources = {n: ROOT / ('cloud/matches/'+n+'.c' if n in NAMES else 'src/blob/'+n+'.c') for n in names}
    for name, path in sources.items():
        (work / (name+'.c')).write_bytes(path.read_bytes())
    (work / 'group.json').write_text(json.dumps({'files': [n+'.c' for n in names],
        'keep': names, 'members': names, 'claims': names, 'flags': FLAGS}))
    obj = work / 'context.o'
    score.compile_group(work, obj)
    rows = {}
    for name in names:
        rows[name] = comparison(obj, name)
        assert score.compare(obj, name, show=0).accepted(), (name, rows[name])
        assert extent(obj, name) == len(score.targets()[name])*4
    return {'sources': {n: sha(p) for n, p in sources.items()}, 'comparisons': rows,
            'scope': 'genuine adjacent callback bodies, unchanged separate type views; not full shadow unit'}

def donor_proof(path):
    assert run(['git', '-C', str(path), 'rev-parse', 'HEAD'], text=True).stdout.strip() == DONOR_REVISION
    paths = ['game/targets.c', 'game/vecmath.h', 'game/vecmath.c', 'LIB/stdtypes.h']
    for name in paths:
        assert run(['git', '-C', str(path), 'show', DONOR_REVISION+':'+name]).stdout == (path/name).read_bytes()
    return {'revision': DONOR_REVISION, 'files': {p: sha(path / p) for p in paths},
            'ancestor': 'game/targets.c:673-693 OverlapTarget; N64 cylinder specialization',
            'vector_macros': 'game/vecmath.h:20-21'}

def verify(donor=None):
    result = {'schema': 1, 'status': 'strict_matches_unspliced', 'claims': list(NAMES),
              'accepted_byte_gain': 0, 'flags': FLAGS, 'targets': {},
              'protected_manifest_sha256': sha(ROOT / 'asm/us/blob/SHA256SUMS'),
              'scorer_sha256': sha(ROOT / 'tools/cloud/score.py'),
              'compiler_sha256': sha(score.IDO / 'cc'),
              'proof_sources': {p.name: sha(p) for p in [HERE/'verify.py', HERE/'native.py', HERE/'host.c']}}
    with tempfile.TemporaryDirectory(prefix='target-overlap-') as tmp:
        base = Path(tmp)
        for name in NAMES:
            work = base / name
            work.mkdir()
            source = ROOT / 'cloud/matches' / (name+'.c')
            (work/'candidate.c').write_bytes(source.read_bytes())
            obj = work / 'candidate.o'
            # Use a stable relative source name: ECOFF paths otherwise vary with scratch location.
            old = Path.cwd()
            try:
                os.chdir(work)
                score.compile_single(Path('candidate.c'), FLAGS, obj)
            finally:
                os.chdir(old)
            proof, linked = full_body(obj, name, work)
            proof['source_sha256'] = sha(source)
            proof['start'] = hex(score.image_symbols()[name])
            proof['end_exclusive'] = hex(score.image_symbols()[name]+320)
            proof['historical_source_sha256'] = sha(ROOT / BASELINES[name])
            proof['source_controls'] = control_sources(name, work)
            score.compile_single(source, FLAGS.replace('-O3', '-O2'), work/'o2.o')
            proof['O2_comparison'] = comparison(work/'o2.o', name)
            assert score.compare(work/'o2.o', name, show=0).accepted()
            proof['behavior'] = behavior(name, score.targets()[name], linked, work)
            result['targets'][name] = proof
        result['genuine_context'] = context(base/'context')
    if donor:
        result['donor'] = donor_proof(Path(donor))
    return result

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--donor')
    parser.add_argument('--output')
    args = parser.parse_args()
    result = verify(args.donor)
    text = json.dumps(result, indent=2, sort_keys=True)+'\n'
    if args.output:
        Path(args.output).write_text(text)
    else:
        print(text)
