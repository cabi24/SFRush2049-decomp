#!/usr/bin/env python3
"""Reproduce whole-symbol equality, GNU linking, context and bounded behavior."""
import argparse
import ctypes
from dataclasses import asdict
import hashlib
import importlib.util
import itertools
import json
from pathlib import Path
import random
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
spec = importlib.util.spec_from_file_location('scene_parent_native', HERE / 'native.py')
native = importlib.util.module_from_spec(spec)
spec.loader.exec_module(native)
FN = 'func_800A7BF8'
SOURCE = ROOT / 'cloud/matches/func_800A7BF8.c'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
CONTEXT = ['func_8008AE2C', 'func_8008AE10']
ARCHIVE = ROOT / 'cloud/work/near-miss/func_800A7BF8/base.c'
MUTANTS = {
    'wrong_owner': ('return i;', 'return i + 1;'),
    'skip_first_child': ('if (search_key == first_child)', 'if (0)'),
    'skip_siblings': ('sibling = D_8012E700[next].sibling;', 'sibling = -1;'),
    'sentinel_before_comparison': ('if (search_key == first_child)', 'if (first_child != -1 && search_key == first_child)'),
    'wrong_failure': ('return -1;', 'return 0;'),
}


def sha(data): return hashlib.sha256(data).hexdigest()


def symbol(obj, name):
    data, secs = score._elf(obj)
    ti = score._text_index(secs)
    matches = [s for i, sec in enumerate(secs) if sec['type'] == 2
               for s in score._symbol_table(data, secs, i)
               if s['name'] == name and s['type'] == 2 and s['section'] == ti]
    assert len(matches) == 1
    return matches[0]


def inspect(obj, name=FN):
    cmp = score.compare(obj, name, show=0)
    return dict(asdict(cmp), verdict=cmp.summary(), symbol_bytes=symbol(obj, name)['size'],
                native_bytes=len(score.targets()[name]) * 4)


def complete(result):
    return result['verdict'] == 'MATCH' and result['symbol_bytes'] == result['native_bytes'] and not any(
        result[k] for k in ('differing', 'unresolved', 'unverified', 'errors', 'extra_words'))


def object_proof(work):
    obj = work / 'candidate.o'
    score.compile_single(SOURCE, FLAGS, obj)
    result = inspect(obj)
    assert complete(result)
    data, secs = score._elf(obj)
    ti = score._text_index(secs)
    text = secs[ti]
    fn = symbol(obj, FN)
    assert fn['value'] == 0 and fn['size'] == 164 and text['size'] == 176
    assert not any(data[text['off'] + 164:text['off'] + text['size']])
    assert not any(s['size'] for s in secs if s['name'] in ('.rodata', '.data', '.bss', '.sdata', '.sbss', '.lit4', '.lit8'))
    relocs = []
    for sec in secs:
        if sec['type'] == 9 and sec['info'] == ti:
            syms = score._symbol_table(data, secs, sec['link'])
            for off in range(sec['off'], sec['off'] + sec['size'], 8):
                at, info = struct.unpack_from('>II', data, off)
                relocs.append(dict(offset=at, type=info & 255, symbol=syms[info >> 8]['name']))
    assert len(relocs) == 6 and {r['type'] for r in relocs} == {5, 6}
    assert {r['symbol'] for r in relocs} == {'D_8012E700', 'D_80156990'}
    script = work / 'link.ld'
    script.write_text('SECTIONS { .text 0x800A7BF8 : SUBALIGN(4) { *(.text) } }\n'
                      'D_8012E700 = 0x8012E700;\nD_80156990 = 0x80156990;\n')
    linked = work / 'candidate.elf'
    subprocess.run(['mips-linux-gnu-ld', '-EB', '-T', str(script), '-o', str(linked), str(obj)], check=True, capture_output=True)
    binary, sections = score._elf(linked)
    txt = sections[score._text_index(sections)]
    body = binary[txt['off']:txt['off'] + 164]
    words = list(struct.unpack('>41I', body))
    assert words == score.targets()[FN]
    assert symbol(linked, FN)['value'] == native.BASE and symbol(linked, FN)['size'] == 164
    result.update(relocations=relocs, gnu_linked_full_body_equal=True,
                  gnu_body_sha256=sha(body), own_data_bytes=0, zero_alignment_bytes=12)
    provenance = {'object_sha256': sha(data), 'linked_elf_sha256': sha(binary),
                  'note': 'Full ELF hashes are build-path provenance; .mdebug can differ across worktrees.'}
    return result, words, provenance


def context_proof(work):
    group = work / 'context'
    group.mkdir()
    names = [FN] + CONTEXT
    paths = [SOURCE] + [ROOT / 'src/blob' / (n + '.c') for n in CONTEXT]
    for i, path in enumerate(paths): shutil.copyfile(path, group / ('c%d.c' % i))
    (group / 'group.json').write_text(json.dumps({'files': ['c%d.c' % i for i in range(3)],
        'members': [FN], 'context': CONTEXT, 'keep': names, 'flags': FLAGS}))
    obj = group / 'context.o'
    score.compile_group(group, obj)
    results = {n: inspect(obj, n) for n in names}
    assert all(complete(r) for r in results.values())
    return {'bodies': results, 'source_sha256': {str(p.relative_to(ROOT)): sha(p.read_bytes()) for p in paths[1:]},
            'limit': 'Three unchanged genuine bodies; shared record accessor regression, not actual caller closure or full-game shadow unit.'}


def controls(work):
    source = SOURCE.read_text()
    unnamed = source.replace('    SceneRecord *record;\n', '').replace(
        '            record = &D_8012E700[i];\n            first_child = record->child;',
        '            first_child = D_8012E700[i].child;')
    variants = {'o2': (source, FLAGS.replace('-O3', '-O2')),
                'archived_m2c': (ARCHIVE.read_text(), FLAGS.replace('-O3', '-O2')),
                'archived_full_width_counter': (ARCHIVE.read_text().replace('s16 var_v1;', 's32 var_v1;'), FLAGS),
                'unnamed_record': (unnamed, FLAGS)}
    result = {}
    for tag, (text, flags) in variants.items():
        path, obj = work / (tag + '.c'), work / (tag + '.o')
        path.write_text(text)
        score.compile_single(path, flags, obj)
        result[tag] = inspect(obj)
    assert complete(result['o2'])
    assert result['archived_m2c']['differing'] == 35
    assert result['archived_full_width_counter']['differing'] == 34
    assert result['unnamed_record']['differing'] == 11
    # A real, unmodified accepted-accessor composition is a fixed negative control.
    group = work / 'accessor_calls'
    group.mkdir()
    text = source.replace('extern s32 D_80156990;', 'extern s32 D_80156990;\nextern s16 func_8008AE2C(s32);\nextern s16 func_8008AE10(s32);')
    text = text.replace('record = &D_8012E700[i];\n            first_child = record->child;', 'first_child = func_8008AE2C(i);')
    text = text.replace('    SceneRecord *record;\n', '').replace('D_8012E700[next].sibling', 'func_8008AE10(next)')
    (group / 'candidate.c').write_text(text)
    for name in CONTEXT: shutil.copyfile(ROOT / 'src/blob' / (name + '.c'), group / (name + '.c'))
    names = [FN] + CONTEXT
    (group / 'group.json').write_text(json.dumps({'files': ['candidate.c'] + [n + '.c' for n in CONTEXT],
        'members': [FN], 'context': CONTEXT, 'keep': names, 'flags': FLAGS}))
    obj = group / 'control.o'
    score.compile_group(group, obj)
    result['accepted_accessor_calls'] = {name: inspect(obj, name) for name in names}
    assert not complete(result['accepted_accessor_calls'][FN])
    assert all(complete(result['accepted_accessor_calls'][n]) for n in CONTEXT)
    return result


def make_case(children, siblings, argument, count=None):
    assert len(children) == len(siblings) and 1 <= len(children) <= 512
    records = bytearray((i * 37 + 101) & 255 for i in range(len(children) * 68))
    for i, (child, sibling) in enumerate(zip(children, siblings)):
        struct.pack_into('>hh', records, 68 * i + 22, child, sibling)
    return dict(records=bytes(records), count=len(children) if count is None else count,
                argument=native.signed(argument))


def terminating(siblings):
    for start in range(len(siblings)):
        seen = set()
        while start != -1:
            if start in seen: return False
            seen.add(start)
            start = siblings[start]
    return True


def cases(random_count=768):
    for count in [-2147483648, -32768, -1, 0]:
        for key in [-2147483648, -65537, -32768, -1, 0, 1, 65535, 2147483647]:
            yield make_case([-1], [-1], key, count)
    # Exhaustive all finite sibling graphs and all first-child choices through 3 records.
    for n in range(1, 4):
        options = list(range(-1, n))
        for siblings in itertools.product(options, repeat=n):
            if not terminating(siblings): continue
            for children in itertools.product(options, repeat=n):
                for key in [-32768, -2, -1] + list(range(n + 1)) + [32767]:
                    yield make_case(children, siblings, key)
    rng = random.Random(0xA7BF8)
    for k in range(random_count):
        n = rng.choice([4, 5, 8, 16, 32])
        order = list(range(n)); rng.shuffle(order)
        siblings = [-1] * n
        for j, index in enumerate(order[:-1]):
            if rng.randrange(4): siblings[index] = rng.choice(order[j + 1:])
        children = [rng.choice([-1] + list(range(n))) for _ in range(n)]
        key = rng.choice([-32768, -2, -1, n, 32767] + list(range(n)))
        key = native.signed(((rng.randrange(65536) << 16) | (key & 65535)))
        yield make_case(children, siblings, key)
    for n in [64, 256, 512]:
        siblings = list(range(1, n)) + [-1]
        children = [-1] * n
        children[0] = 0
        for key in [-1, 0, n // 2, n - 1, n, 0x12340000 + n - 1]:
            yield make_case(children, siblings, key)
        children = [-1] * n; children[-1] = n - 1
        yield make_case(children, [-1] * n, n - 1)
    # Invalid negative links can be observed safely when equality returns before dereference.
    for key in [-32768, -30000, -2]:
        yield make_case([key], [-1], key)
        yield make_case([0], [key], key)


def oracle(case):
    values = [struct.unpack_from('>hh', case['records'], i + 22)
              for i in range(0, len(case['records']), 68)]
    key = native.signed(case['argument'], 16)
    for parent in range(max(0, case['count'])):
        link = values[parent][0]
        seen = set()
        while True:
            if link == key: return native.signed(parent, 16)
            if link == -1: break
            assert 0 <= link < len(values) and link not in seen
            seen.add(link)
            link = values[link][1]
    return -1


def host_bytes(records):
    data = bytearray(records)
    if sys.byteorder == 'little':
        for i in range(0, len(data), 68):
            data[i + 22:i + 24] = data[i + 22:i + 24][::-1]
            data[i + 24:i + 26] = data[i + 24:i + 26][::-1]
    return bytes(data)


def host_library(work, tag, source):
    path, lib = work / (tag + '.c'), work / (tag + '.so')
    path.write_text(source + '\n' + (HERE / 'host.c').read_text())
    subprocess.run(['cc', '-std=c89', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra',
                    '-fsanitize=undefined', '-fno-sanitize-recover=all', str(path), '-o', str(lib)],
                   check=True, capture_output=True)
    dll = ctypes.CDLL(str(lib))
    dll.scene_parent_host.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_void_p]
    dll.scene_parent_host.restype = ctypes.c_int
    return dll


def host_result(dll, case):
    incoming = host_bytes(case['records'])
    input_buffer = ctypes.create_string_buffer(incoming)
    output = ctypes.create_string_buffer(512 * 68)
    result = dll.scene_parent_host(input_buffer, len(incoming), case['count'], case['argument'], output)
    assert output.raw == incoming + b'\x6B' * (512 * 68 - len(incoming)), 'host record preservation'
    assert ctypes.c_int.in_dll(dll, 'D_80156990').value == case['count'], 'host count preservation'
    return result


def behavior(work, linked_words, random_count):
    corpus = list(cases(random_count))
    target = score.targets()[FN]
    host = host_library(work, 'host', SOURCE.read_text())
    coverage, branches, fingerprint = set(), set(), hashlib.sha256()
    for case in corpus:
        expected = oracle(case)
        for words in (target, linked_words):
            machine = native.Machine(words, case['records'], case['count'], case['argument'])
            assert machine.run() == expected
            coverage.update(machine.coverage); branches.update(machine.branches)
        assert host_result(host, case) == expected
        fingerprint.update(struct.pack('>iii', case['count'], case['argument'], expected) + case['records'])
    assert coverage == set(range(0, 164, 4))
    branch_sites = {4 * i for i, word in enumerate(target) if word >> 26 in (4, 5, 6, 7, 0x14, 0x15)}
    assert branches == {(pc, direction) for pc in branch_sites for direction in (False, True)}
    mutants = {}
    for tag, (before, after) in MUTANTS.items():
        changed = SOURCE.read_text().replace(before, after)
        assert changed != SOURCE.read_text()
        mutant = host_library(work, tag, changed)
        for index, case in enumerate(corpus):
            # These deliberately wrong controls never introduce nonterminating traversal.
            if tag == 'skip_first_child' and oracle(case) >= 0 and struct.unpack_from('>h', case['records'], 22)[0] < -1:
                continue
            got = host_result(mutant, case)
            if got != oracle(case):
                mutants[tag] = {'rejected': True, 'first_counterexample': index, 'expected': oracle(case), 'actual': got}
                break
        else: raise AssertionError('mutant survived: ' + tag)
    # Fail-closed decoder control runs on the actual native entry.
    bad = list(target); bad[0] = 0xFFFFFFFF
    try: native.Machine(bad, corpus[0]['records'], corpus[0]['count'], corpus[0]['argument']).run()
    except AssertionError as exc: assert 'opcode' in str(exc)
    else: raise AssertionError('unknown instruction accepted')
    return dict(cases=len(corpus), native_executions=2 * len(corpus), source_host_executions=len(corpus),
                corpus_sha256=fingerprint.hexdigest(), instruction_offsets_covered=len(coverage),
                all_conditional_outcomes_covered=True, branch_sites=len(branch_sites),
                host_c89_ubsan='passed', full_record_count_stack_and_saved_register_preservation=True,
                negative_controls=mutants, unknown_opcode_rejected=True)


def verify(work, random_count=768):
    work.mkdir(parents=True, exist_ok=True)
    obj, words, provenance = object_proof(work)
    context = context_proof(work)
    control = controls(work)
    runtime = behavior(work, words, random_count)
    syms = score.image_symbols()
    callers = [[n, hex(syms[n] + 4 * i)] for n, body in score.targets().items() for i, word in enumerate(body)
        if word >> 26 == 3 and (((syms[n] + i * 4 + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)) == native.BASE]
    assert callers == [['transmission_shift', '0x800abc34'], ['transmission_shift', '0x800abc98']]
    return dict(status='STRICT_MATCH_CANDIDATE', base_revision='e24b47d89a0c8ffade1e4c75ad76b9d390a1c232',
        function=FN, address=hex(native.BASE), end_exclusive=hex(native.BASE + 164), native_bytes=164,
        accepted_byte_gain=0, flags=FLAGS, source_sha256=sha(SOURCE.read_bytes()),
        target_sha256=sha(struct.pack('>41I', *score.targets()[FN])),
        target_manifest_sha256=sha((ROOT / 'asm/us/blob/SHA256SUMS').read_bytes()),
        scorer_sha256=sha(Path(score.__file__).read_bytes()),
        compiler_sha256={n: sha((Path(score.IDO) / n).read_bytes()) for n in ['cc', 'cfe', 'uopt', 'ugen', 'as1']},
        packet_sha256={n: sha((HERE / n).read_bytes()) for n in ['verify.py', 'native.py', 'host.c']},
        archive_sha256=sha(ARCHIVE.read_bytes()), object=obj, object_provenance=provenance,
        context=context, controls=control, behavior=runtime, direct_callers=callers,
        limits=['Finite accessible sibling chains; no cyclic/corrupt/concurrent graph proof.',
                'Record fixtures use 1..512 entries; arbitrary positive out-of-range counts and runtime gameplay are untested.',
                'Negative links below -1 are tested only on equality-short-circuit paths before dereference.',
                'Exact code includes signed-half input/return narrowing; parent indices above 32767 are not behaviorally exercised.',
                'No direct arcade donor, original C declaration recovery, actual transmission_shift execution, full-game shadow, splice, image, compression or ROM claim.'])


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--out', type=Path, default=HERE / 'verification.json')
    parser.add_argument('--random-count', type=int, default=768)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='scene-parent-') as directory:
        result = verify(Path(directory), args.random_count)
    args.out.write_text(json.dumps(result, indent=2, sort_keys=True) + '\n')
    print(json.dumps({'status': result['status'], 'native_bytes': result['native_bytes'], 'behavior': result['behavior']}, indent=2))
