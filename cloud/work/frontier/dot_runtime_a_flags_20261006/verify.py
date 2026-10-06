#!/usr/bin/env python3
"""Rebuild and verify the complete image-A availability initializer.

Run from any cwd with IDO_DIR and the MIPS GNU binutils in PATH. Binary
artifacts stay under build/. This receipt contains hashes, never ROM words.
"""
import argparse
import ctypes
import hashlib
import json
import os
from pathlib import Path
import random
import struct
import subprocess
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score
sys.path.insert(0, str(HERE))
import native

NAME = 'func_80393004'
SOURCE = ROOT / 'cloud/matches/ovl_a' / (NAME + '.c')
ARCHIVED = ROOT / 'cloud/work/r16_runtime_images/near_miss' / (NAME + '.c')
TARGETS = ROOT / 'asm/us/ovl_a'
ENTRY, SIZE = native.ENTRY, 180


def sha(data):
    return hashlib.sha256(data).hexdigest()


def sh(args):
    proc = subprocess.run([str(x) for x in args], text=True, capture_output=True)
    assert proc.returncode == 0, '%s\n%s\n%s' % (' '.join(map(str, args)), proc.stdout, proc.stderr)
    return proc.stdout


def elf(path):
    """Separate ELF32-BE parser: symbol sizes and complete allocatable bodies."""
    data = Path(path).read_bytes()
    assert data[:6] == b'\x7fELF\x01\x02', 'not ELF32 big-endian'
    assert struct.unpack_from('>H', data, 18)[0] == 8, 'not MIPS'
    off = struct.unpack_from('>I', data, 32)[0]
    entsize, num, strings = struct.unpack_from('>HHH', data, 46)
    assert entsize == 40 and 0 < num < 100
    sections = [list(struct.unpack_from('>10I', data, off + i * entsize)) for i in range(num)]
    names = sections[strings]
    namebytes = data[names[4]:names[4] + names[5]]
    result, symbols, relocs = {}, {}, []
    for i, row in enumerate(sections):
        name = namebytes[row[0]:].split(b'\0', 1)[0].decode()
        body = data[row[4]:row[4] + row[5]]
        assert row[1] == 8 or len(body) == row[5], 'truncated ELF section'
        result[name] = {'index': i, 'type': row[1], 'flags': row[2], 'address': row[3],
                        'size': row[5], 'body': body, 'alignment': row[8]}
        if row[1] == 2:
            stringsec = sections[row[6]]
            stringsbody = data[stringsec[4]:stringsec[4] + stringsec[5]]
            assert row[9] == 16
            for at in range(0, len(body), 16):
                no, value, size, info, other, index = struct.unpack_from('>IIIBBH', body, at)
                symbol = stringsbody[no:].split(b'\0', 1)[0].decode()
                if symbol:
                    symbols[symbol] = {'value': value, 'size': size, 'type': info & 15,
                                       'bind': info >> 4, 'section': index}
        elif row[1] == 9:
            assert row[9] == 8
            for at in range(0, len(body), 8):
                address, info = struct.unpack_from('>II', body, at)
                relocs.append({'offset': address, 'type': info & 255, 'symbol_index': info >> 8})
    return result, symbols, relocs


def inspect(path, linked=False):
    sections, symbols, relocs = elf(path)
    funcs = {n: s for n, s in symbols.items() if s['type'] == 2 and s['section']}
    assert set(funcs) == {NAME}, 'unexpected defined function'
    sym, text = funcs[NAME], sections['.text']
    assert sym['size'] == SIZE, 'wrong ELF function size'
    assert sym['value'] == (ENTRY if linked else 0), 'wrong function address'
    assert text['address'] == (ENTRY if linked else 0), 'wrong section address'
    assert sym['section'] == text['index']
    assert text['size'] == 192 and text['body'][SIZE:] == bytes(12), 'nonzero or changed alignment extent'
    assert text['alignment'] == (4 if linked else 16)
    for name, section in sections.items():
        if section['flags'] & 2 and name not in ('.text', '.options', '.reginfo'):
            assert section['size'] == 0, 'unverified owned storage: ' + name
    if linked:
        assert not relocs, 'unapplied linked relocations'
        for name, address in {'D_803BA7E0': native.ROW0, 'D_803BA7F0': native.ROW1,
                              'D_803BA830': native.TABLE, 'D_803B65A4': native.SELECTOR}.items():
            assert symbols[name]['value'] == address and symbols[name]['section'] == 0xFFF1
    else:
        assert len(relocs) == 12 and sorted(x['type'] for x in relocs) == [5] * 6 + [6] * 6
        assert all(0 <= x['offset'] < SIZE for x in relocs)
    return text['body'][:SIZE], relocs


def host_lib(source, out):
    sh(['gcc', '-std=c89', '-O2', '-shared', '-fPIC', '-fsanitize=undefined',
        '-fno-sanitize-recover=all', source, HERE / 'host.c', '-o', out])
    lib = ctypes.CDLL(str(out))
    lib.host_run.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_int),
                            ctypes.POINTER(ctypes.c_ubyte), ctypes.POINTER(ctypes.c_ubyte)]
    return lib


def host_run(lib, index, counts, initial):
    inbytes = bytes(initial[x] for x in range(native.ROW0, native.ROW1 + 16))
    inp = (ctypes.c_ubyte * 32).from_buffer_copy(inbytes)
    result = (ctypes.c_ubyte * 49)()
    values = (ctypes.c_int * 4)(*counts)
    lib.host_run(index, values, inp, result)
    assert bytes(result)[32:48] == bytes(values), 'host mutated count table'
    assert result[48] == index, 'host mutated selector'
    return bytes(result)[:32]


def write_link_script(path):
    path.write_text('SECTIONS { .text 0x80393004 : SUBALIGN(4) { *(.text) }\n'
                    '/DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'
                    'D_803BA7E0 = 0x803BA7E0; D_803BA7F0 = 0x803BA7F0;\n'
                    'D_803BA830 = 0x803BA830; D_803B65A4 = 0x803B65A4;\n')


def main(out):
    build = ROOT / 'build/runtime_a_flags_proof'
    build.mkdir(parents=True, exist_ok=True)
    (build / 'tmp').mkdir(exist_ok=True)
    os.environ['TMPDIR'] = str(build / 'tmp')
    import tempfile
    tempfile.tempdir = str(build / 'tmp')
    score.ASM_DIR = TARGETS
    targets = score.targets()
    words = targets[NAME]
    target = struct.pack('>%dI' % len(words), *words)
    manifest = score.target_manifest()
    metadata = json.loads(score.verified_bytes(TARGETS / 'extents.json', manifest))
    assert metadata['image'] == 'A' and metadata['base'] == '0x8038A400'
    assert metadata['rom_offset'] == '0xB5C534' and metadata['size'] == 194128
    assert metadata['image_sha256'] == '0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
    extent = next(x for x in metadata['functions'] if x['name'] == NAME)
    assert extent['address'] == '0x80393004' and extent['size'] == SIZE and len(target) == SIZE
    obj, linked = build / 'candidate.o', build / 'candidate.elf'
    score.compile_single(str(SOURCE), score.DEFAULT_FLAGS, obj)
    raw, relocs = inspect(obj)
    comparison = score.compare(obj, NAME, show=0)
    assert comparison.accepted(), comparison.summary()
    full = score.text_words(obj)
    resolved, masks, unresolved, unverified, errors = score.relocate(obj, full, 0, len(full) * 4, score.image_symbols())
    assert not any((masks, unresolved, unverified, errors))
    assert struct.pack('>%dI' % len(resolved), *resolved) == target + bytes(12)
    script = build / 'whole.ld'
    write_link_script(script)
    sh(['mips-linux-gnu-ld', '-EB', '-T', script, '-o', linked, obj])
    linkedbody, unused = inspect(linked, linked=True)
    assert linkedbody == target
    linkedwords = list(struct.unpack('>45I', linkedbody))
    controls = {}
    baseline = build / 'baseline.o'
    score.compile_single(str(ARCHIVED), score.DEFAULT_FLAGS, baseline)
    c = score.compare(baseline, NAME, show=0)
    assert c.differing == 45 and c.extra_words == 6 and not c.accepted()
    controls['archived_signed_induction'] = c.summary()
    source = SOURCE.read_text()
    wrong = source.replace('(s32)i < count', 'i < count')
    wrongpath, wrongobj = build / 'unsigned_value.c', build / 'unsigned_value.o'
    wrongpath.write_text(wrong)
    score.compile_single(str(wrongpath), score.DEFAULT_FLAGS, wrongobj)
    c = score.compare(wrongobj, NAME, show=0)
    assert c.differing == 24 and not c.accepted()
    controls['unsigned_value_comparison'] = c.summary()
    lib = host_lib(SOURCE, build / 'host.so')
    rng = random.Random(0x80393004)
    counts = sorted(set(list(range(-16, 65)) + [-2147483645, -2147483644, -1000000, -257,
                     -129, -128, -127, 127, 128, 255, 256, 65535, 2147483646, 2147483647]
                     + [rng.randint(-2147483645, 2147483647) for _ in range(1024)]))
    cases, coverage, branches = 0, set(), set()
    tracehash = hashlib.sha256()
    for count in counts:
        for index in range(4):
            for salt in (0, 193):
                table = [rng.randint(-4, 64) for _ in range(4)]
                table[index] = count
                initial = native.fixture(index, table, salt + cases)
                expected = native.oracle(initial, index, count)
                a = native.run(words, initial, salt + cases)
                b = native.run(linkedwords, initial, salt + cases)
                assert a == b, 'GNU/native execution divergence'
                assert a['memory'] == expected, 'native/C contract divergence'
                assert host_run(lib, index, table, initial) == bytes(expected[x] for x in range(native.ROW0, native.ROW1 + 16))
                reads = [x for x in a['accesses'] if x[0].startswith('read')]
                assert reads == [('read8', native.SELECTOR, index), ('read32', native.TABLE + 4 * index, count & 0xFFFFFFFF)]
                assert a['accesses'][5:7] == reads
                coverage |= a['coverage']
                branches |= a['branches']
                tracehash.update(json.dumps(a['accesses'], separators=(',', ':')).encode())
                cases += 1
    assert coverage == set(range(0, 180, 4)) and branches == {(164, True), (164, False)}
    underflow = []
    for count in (-2147483648, -2147483647, -2147483646):
        initial = native.fixture(0, [count, -4, 0, 8], 74)
        a = native.run(words, initial)
        assert a == native.run(linkedwords, initial)
        expected = native.oracle(initial, 0, count)
        host = host_run(lib, 0, [count, -4, 0, 8], initial)
        assert host == bytes(expected[x] for x in range(native.ROW0, native.ROW1 + 16))
        differing = [i for i in range(11) if a['memory'][native.ROW0 + i] != expected[native.ROW0 + i]]
        assert differing == { -2147483648: [4, 5, 6, 8, 9, 10],
                              -2147483647: [5, 6, 9, 10],
                              -2147483646: [6, 10]}[count]
        underflow.append({'count': count, 'differing_indices_in_each_row': differing,
                          'native_value': 1, 'host_oracle_value': 0})
    semantic_mutants = {
        'unsigned_comparison': wrong,
        'omit_last_item': source.replace('i < 8', 'i < 7'),
        'inclusive_comparison': source.replace('(s32)i < count', '(s32)i <= count'),
        'empty_threshold': source.replace('count < 1', 'count < 0'),
        'wrong_first_row': source.replace('D_803BA7E0[2] = 0', 'D_803BA7E0[2] = 1')}
    rejected = []
    for name, body in semantic_mutants.items():
        path = build / (name + '.c')
        path.write_text(body)
        mutantlib = host_lib(path, build / (name + '.so'))
        detected = False
        for value in (-4, -1, 0, 1, 7, 8, 9):
            initial = native.fixture(2, [3, -4, value, 8], 981)
            expected = native.oracle(initial, 2, value)
            if host_run(mutantlib, 2, [3, -4, value, 8], initial) != bytes(expected[x] for x in range(native.ROW0, native.ROW1 + 16)):
                detected = True
                break
        assert detected, 'undetected semantic mutant: ' + name
        rejected.append(name)
    malformed = []
    initial = native.fixture(0, [8, 0, -4, 1], 42)
    badbodies = {'truncated': words[:-1], 'extra_extent': words + [0],
                 'unknown_opcode': [0xFC000001] + words[1:]}
    for name, offset, replacement in [('bad_byte_load', 44, words[11] ^ 4),
                                      ('bad_count_load', 64, words[16] ^ 32),
                                      ('bad_store', 20, words[5] | 0x100),
                                      ('bad_return', 172, (words[43] & ~(31 << 21)) | (4 << 21))]:
        mutant = list(words)
        mutant[offset // 4] = replacement
        badbodies[name] = mutant
    for name, body in badbodies.items():
        try:
            native.run(body, initial)
        except (AssertionError, KeyError):
            malformed.append(name)
        else:
            raise AssertionError('undetected malformed native: ' + name)
    negative_elf = []
    for name in ('function_address', 'function_extent'):
        data = bytearray(obj.read_bytes())
        # The independent parser finds .symtab without trusting fixed offsets.
        sho = struct.unpack_from('>I', data, 32)[0]
        n = struct.unpack_from('>H', data, 48)[0]
        for i in range(n):
            row = struct.unpack_from('>10I', data, sho + i * 40)
            if row[1] != 2:
                continue
            for at in range(row[4], row[4] + row[5], 16):
                if data[at + 12] & 15 == 2:
                    field = at + (4 if name == 'function_address' else 8)
                    struct.pack_into('>I', data, field, (4 if name == 'function_address' else 176))
        path = build / (name + '.o')
        path.write_bytes(data)
        try:
            inspect(path)
        except AssertionError:
            negative_elf.append(name)
        else:
            raise AssertionError('ELF drift accepted')
    caller_sites = []
    for name, body in targets.items():
        for i, word in enumerate(body):
            if word >> 26 == 3 and ((word & 0x3FFFFFF) << 2 | 0x80000000) == ENTRY:
                caller_sites.append({'function': name, 'site': '0x%08X' % (int(name[5:], 16) + 4 * i)})
    assert len(caller_sites) == 3
    writer = struct.pack('>%dI' % len(targets['func_803930B8']), *targets['func_803930B8'])
    inputs = [SOURCE, ARCHIVED, HERE / 'native.py', HERE / 'host.c', HERE / 'verify.py',
              ROOT / 'tools/cloud/score.py', ROOT / 'tools/cloud/owndata.py',
              TARGETS / 'SHA256SUMS', TARGETS / 'extents.json', TARGETS / 'symbols.json',
              TARGETS / 'ovl_a_8038a400.s']
    result = {'schema': 1, 'status': 'MATCH; bounded behavioral proof',
              'image': {'name': 'A', 'base': metadata['base'], 'rom_offset': metadata['rom_offset'],
                        'size': metadata['size'], 'sha256': metadata['image_sha256']},
              'function': {'name': NAME, 'address': '0x%08X' % ENTRY, 'size': SIZE,
                           'target_sha256': sha(target), 'gnu_linked_sha256': sha(linkedbody),
                           'elf_function_size': 180, 'section_size': 192, 'alignment_zero_bytes': 12,
                           'relocations': relocs, 'owned_data_bytes': 0},
              'source_sha256': sha(SOURCE.read_bytes()), 'recipe': score.DEFAULT_FLAGS,
              'assembler_addition': score.R4300_CC, 'score': comparison.summary(), 'controls': controls,
              'behavior': {'cases': cases, 'native_executions': cases * 2 + 6,
                           'host_cases': cases + 3, 'count_values': len(counts),
                           'tested_count_min': min(counts), 'tested_count_max': max(counts),
                           'real_slots': [0, 1, 2, 3], 'covered_instruction_offsets': sorted(coverage),
                           'branch_outcomes': sorted(branches), 'trace_sha256': tracehash.hexdigest(),
                           'underflow_outside_producer_contract': underflow},
              'rejected_semantic_mutants': rejected, 'rejected_native_controls': malformed,
              'rejected_elf_controls': negative_elf, 'direct_callers': caller_sites,
              'producer': {'name': 'func_803930B8', 'address': '0x803930B8', 'size': len(writer),
                           'target_sha256': sha(writer), 'observed_slots': 4,
                           'observed_values': '-4, -3, -2, -1, or nonnegative linked-list count under terminating/nonoverflowing traversal',
                           'method': 'read-only native inspection; producer not executed'},
              'input_sha256': {str(p.relative_to(ROOT)): sha(p.read_bytes()) for p in inputs},
              'tool_sha256': {name: sha(Path(score.ido(name)).read_bytes()) for name in
                              ('cc', 'cfe', 'uopt', 'ugen', 'as1')},
              'gnu_ld': sh(['mips-linux-gnu-ld', '--version']).splitlines()[0],
              'host_cc': sh(['gcc', '--version']).splitlines()[0],
              'limits': ['No runtime-image composition, compression or full-ROM gate.',
                         'No original-source spelling or whole-translation-unit claim.',
                         'Valid initialized selector slots 0..3, disjoint nonvolatile storage; no concurrency.',
                         'Observed producer domain assumes a terminating, nonoverflowing list; no exhaustive writer census.',
                         'The three INT_MIN-near values differ from host C; outside observed producer contract.',
                         'Finite counts and seeds do not prove unrestricted behavior or hardware execution.']}
    out.write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps({'score': result['score'], 'cases': cases, 'receipt': str(out)}, indent=2))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, default=HERE / 'verification.json')
    main(parser.parse_args().output.resolve())
