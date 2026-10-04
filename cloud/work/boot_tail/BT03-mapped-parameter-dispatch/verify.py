#!/usr/bin/env python3
"""Source-bound research verification. No protected input is modified.

The temporary GNU ld placement is a test fixture, not a production ownership
assignment. Every relocated instruction bit and all six compiler table entries
are independently checked; stock score.py remains unchanged and unverified.
"""
import argparse
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
score.ASM_DIR = ROOT / 'asm/us/boot_tail'
NAME = 'func_8001BE14'
SOURCE = HERE / (NAME + '_NONMATCH.c')
BASE = 0x8001BE14
TABLE = 0x8002D8E8
DATA = 0x8004F300
CALLEE = 0x8001E930
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def raw(words):
    return struct.pack('>%dI' % len(words), *words)


def sections(obj):
    data, secs = score._elf(obj)
    shoff = struct.unpack_from('>I', data, 0x20)[0]
    shentsize = struct.unpack_from('>H', data, 0x2E)[0]
    for i, sec in enumerate(secs):
        sec['address'] = struct.unpack_from('>I', data, shoff + i * shentsize + 12)[0]
    return data, secs


def symbol(obj):
    data, secs = sections(obj)
    found = [s for i, sec in enumerate(secs) if sec['type'] == 2
             for s in score._symbol_table(data, secs, i) if s['name'] == NAME]
    assert len(found) == 1 and found[0]['type'] == 2
    return found[0]


def independent_relocate(obj):
    """Resolve all emitted REL records, including table R_MIPS_32 records."""
    data, secs = sections(obj)
    bases = {'.text': BASE, '.rodata': TABLE}
    external = {'D_8004F300': DATA, 'func_8001E930': CALLEE}
    output = {name: bytearray(data[s['off']:s['off'] + s['size']])
              for s in secs for name in [s['name']] if name in bases}
    counts = {}
    for sec in secs:
        if sec['type'] != 9:
            continue
        dest = secs[sec['info']]['name']
        assert dest in output, dest
        symbols = score._symbol_table(data, secs, sec['link'])
        buf = output[dest]
        pending = []
        count = 0
        for k in range(sec['size'] // 8):
            off, info = struct.unpack_from('>II', data, sec['off'] + k * 8)
            idx, kind = info >> 8, info & 255
            assert off % 4 == 0 and off + 4 <= len(buf)
            sym = symbols[idx]
            if sym['section'] == 0:
                address = external[sym['name']]
            else:
                address = bases[secs[sym['section']]['name']] + sym['value']
            word = struct.unpack_from('>I', buf, off)[0]
            if kind == 2:
                assert dest == '.rodata' and sym['type'] == 3
                value = (word + address) & 0xFFFFFFFF
            elif kind == 4:
                target = ((word & 0x3FFFFFF) << 2) + address
                assert target & 3 == 0 and target >> 28 == (bases[dest] + off + 4) >> 28
                value = (word & 0xFC000000) | ((target >> 2) & 0x3FFFFFF)
            elif kind == 5:
                pending.append((off, idx, word))
                count += 1
                continue
            elif kind == 6:
                low = word & 65535
                low = low - 65536 if low & 32768 else low
                highs = [x for x in pending if x[1] == idx]
                assert highs
                for highoff, _, highword in highs:
                    addend = ((highword & 65535) << 16) + low
                    highvalue = (highword & 0xFFFF0000) | (((address + addend + 32768) >> 16) & 65535)
                    struct.pack_into('>I', buf, highoff, highvalue)
                value = (word & 0xFFFF0000) | ((address + low) & 65535)
                pending = [x for x in pending if x[1] != idx]
            else:
                raise AssertionError(('unsupported relocation', kind))
            struct.pack_into('>I', buf, off, value)
            count += 1
        assert not pending
        counts[dest] = count
    return {k: bytes(v) for k, v in output.items()}, counts


def build(folder, source=SOURCE, flags=FLAGS):
    obj = folder / 'candidate.o'
    score.compile_single(source, flags, obj)
    resolved, counts = independent_relocate(obj)
    script = folder / 'proof.ld'
    script.write_text('SECTIONS { .text 0x8001BE14 : SUBALIGN(4) { *(.text) } '
                      '.rodata 0x8002D8E8 : SUBALIGN(4) { *(.rodata) } }\n')
    linked = folder / 'candidate.elf'
    before = sha(obj)
    command = ['mips-linux-gnu-ld', '-EB', '-T', str(script),
               '--defsym=D_8004F300=0x8004F300', '--defsym=func_8001E930=0x8001E930',
               '-e', NAME, '-o', str(linked), str(obj)]
    subprocess.run(command, check=True, capture_output=True)
    assert sha(obj) == before, 'GNU ld must not change compiler output'
    data, secs = sections(linked)
    for sec in secs:
        if sec['name'] in resolved:
            assert data[sec['off']:sec['off'] + sec['size']] == resolved[sec['name']]
            assert sec['address'] == {'.text': BASE, '.rodata': TABLE}[sec['name']]
    assert symbol(linked)['value'] == BASE
    assert all(s['type'] != 9 for s in secs), 'linked ELF retains relocations'
    return obj, linked, resolved, counts


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    pins = json.loads((HERE / 'input_pins.json').read_text())
    for path, digest in pins['source_hashes'].items():
        assert sha(ROOT / path) == digest, path
    compiler = {p.name: sha(p) for p in score.IDO.iterdir() if p.is_file()}
    assert compiler == pins['compiler_files_sha256']
    target = score.targets()[NAME]
    proof = json.loads((HERE / 'dispatch_proof.json').read_text())
    assert len(target) * 4 == 904
    assert hashlib.sha256(raw(target)).hexdigest() == proof['canonical_function_sha256']
    incoming = [(name, i * 4) for name, words in score.targets().items()
                for i, w in enumerate(words) if w >> 26 == 3
                and (0x80000000 | ((w & 0x3FFFFFF) << 2)) == BASE]
    assert incoming == [('func_800203EC', 48)]
    table_expected = raw([int(r['target'], 16) for r in proof['selector_to_target']])
    assert hashlib.sha256(table_expected).hexdigest() == proof['table_sha256']
    rows = []
    with tempfile.TemporaryDirectory(prefix='bt03-parameter-proof-') as temp:
        folder = Path(temp)
        score.compile_single(ROOT / 'cloud/matches/boot_tail/func_80010A00.c', FLAGS, folder / 'getter.o')
        assert score.compare(folder / 'getter.o', 'func_80010A00', show=0).accepted()
        for opt in ['O2', 'O1']:
            flags = FLAGS.replace('-O2', '-' + opt)
            obj, linked, resolved, counts = build(folder, flags=flags)
            result = score.compare(obj, NAME, show=0)
            sym = symbol(obj)
            body = resolved['.text'][:sym['size']]
            words = list(struct.unpack('>%dI' % (len(body) // 4), body))
            differing = sum(i >= len(words) or w != words[i] for i, w in enumerate(target))
            table = resolved['.rodata'][:24]
            exact_table = table == table_expected
            assert not result.accepted() and not result.unresolved
            if opt == 'O2':
                assert not result.errors
                assert sym['size'] == 904 and differing == 76
                assert exact_table and resolved['.rodata'][24:] == bytes(8)
                assert resolved['.text'][904:] == bytes(8)
                assert counts == {'.text': 17, '.rodata': 6}
                checks = ['sizeof(void *) == 4', 'sizeof(u32) == 4', 'sizeof(s32) == 4',
                          'sizeof(u16) == 2', 'sizeof(u8) == 1', 'sizeof(MasterFader) == 40']
                checks += ['(unsigned int)&((MasterFader *)0)->%s == %d' % (name, off)
                           for name, off in [('type', 20), ('pauseVolume', 24), ('pauseTarget', 28),
                                             ('pauseDelta', 32), ('pauseTime', 36)]]
                layout_source = folder / 'layout.c'
                layout_source.write_text(SOURCE.read_text() + '\n' + ''.join(
                    'typedef char layout_%d[(%s) ? 1 : -1];\n' % (i, check)
                    for i, check in enumerate(checks)))
                score.compile_single(layout_source, FLAGS, folder / 'layout.o')
                layout_resolved, layout_counts = independent_relocate(folder / 'layout.o')
                assert layout_resolved == resolved and layout_counts == counts
            rows.append(dict(optimization=opt, source_sha256=sha(SOURCE), flags=flags,
                             effective_flags=flags + ' -Wab,-r4300_mul',
                             object_sha256=sha(obj), elf_function_bytes=sym['size'],
                             text_section_bytes=len(resolved['.text']),
                             stock_differing_words=result.differing,
                             stock_extra_nonzero_words=result.extra_words,
                             stock_unverified=result.unverified,
                             stock_errors=result.errors,
                             full_word_differences_without_masks=differing,
                             compiler_table_exact=exact_table,
                             compiler_table_sha256=hashlib.sha256(table).hexdigest(),
                             relocated_body_sha256=hashlib.sha256(body).hexdigest(),
                             independent_relocation_equals_gnu_ld=True,
                             relocation_counts=counts, strict_match=False))
        controls = []
        for source in sorted((HERE / 'controls').glob('*.c')):
            for opt in ['O2', 'O1']:
                flags = FLAGS.replace('-O2', '-' + opt)
                score.compile_single(source, flags, folder / 'control.o')
                result = score.compare(folder / 'control.o', NAME, show=0)
                controls.append(dict(source=source.name, source_sha256=sha(source), flags=flags,
                                     elf_function_bytes=symbol(folder / 'control.o')['size'],
                                     stock_differing_words=result.differing,
                                     stock_extra_nonzero_words=result.extra_words,
                                     unresolved=result.unresolved, unverified=result.unverified,
                                     errors=result.errors, strict_match=result.accepted()))
    report = dict(result='PASS: complete NONMATCH research; no strict match claim',
                  source_sha256=sha(SOURCE), target_bytes=904, incoming_calls=incoming,
                  target_sha256=proof['canonical_function_sha256'],
                  native_layout_assertions_leave_candidate_unchanged=checks,
                  rows=rows, controls=controls,
                  linker_sha256=sha(shutil.which('mips-linux-gnu-ld')),
                  source_free_table_receipt_sha256=pins['table_receipt_sha256'],
                  production_data_ownership='unassigned')
    content = json.dumps(report, indent=2) + '\n'
    if args.output:
        args.output.write_text(content)
    print(content)


if __name__ == '__main__':
    main()
