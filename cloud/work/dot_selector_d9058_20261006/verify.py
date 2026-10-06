#!/usr/bin/env python3
"""Replay the bounded D9058 source/ABI negative; never promote or mask it."""
import argparse
import ctypes
from dataclasses import asdict
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
spec = importlib.util.spec_from_file_location('selector_native', HERE / 'native.py')
native = importlib.util.module_from_spec(spec)
spec.loader.exec_module(native)
FN = 'func_800D9058'
SOURCE = HERE / 'candidate.c'


def sha(data):
    return hashlib.sha256(data).hexdigest()


def run(args):
    p = subprocess.run(args, capture_output=True, text=True)
    assert p.returncode == 0, (args, p.stdout, p.stderr)
    return p.stdout


def functions(obj):
    data, secs = score._elf(obj)
    ti = score._text_index(secs)
    return {s['name']: s for i, sec in enumerate(secs) if sec['type'] == 2
            for s in score._symbol_table(data, secs, i)
            if s['section'] == ti and s['type'] == 2}


def link_proof(obj, work):
    """Link the unmodified object at a real contiguous GNU .text placement.

    All source-group functions retain their compiler offsets in that witness.
    GNU bytes must equal the project resolver with those same actual addresses.
    A separate explicit native-entry map supplies the nonmatching comparison;
    a contiguous source group cannot physically occupy noncontiguous native
    slots without the project's real splice/link procedure.
    """
    data, secs = score._elf(obj)
    ti = score._text_index(secs)
    text = secs[ti]
    raw = data[text['off']:text['off'] + text['size']]
    words = list(struct.unpack('>%dI' % (len(raw) // 4), raw))
    fns = functions(obj)
    addresses = score.image_symbols()
    rels, externs = [], {}
    for sec in secs:
        if sec['type'] != 9 or sec['info'] != ti:
            continue
        syms = score._symbol_table(data, secs, sec['link'])
        symtab = secs[sec['link']]
        fn_ids = {s['value']: i for i, s in enumerate(syms)
                  if s['type'] == 2 and s['section'] == ti}
        for at in range(sec['off'], sec['off'] + sec['size'], 8):
            off, info = struct.unpack_from('>II', data, at)
            sym, kind = syms[info >> 8], info & 255
            assert kind in (4, 5, 6), kind
            n = sym['name']
            if sym['section'] == ti:
                assert kind == 4 and sym['type'] == 3, (n, kind)
                addend = (words[off // 4] & 0x3FFFFFF) << 2
                assert addend in fn_ids, addend
                idx = fn_ids[addend]
                n = syms[idx]['name']
                assert n in addresses
            else:
                assert sym['section'] == 0 and sym['type'] != 3, n
                value = addresses.get(n, score.address_named(n))
                assert value is not None, n
                externs[n] = value
            rels.append({'offset': off, 'type': kind, 'symbol': n})
    own_names = ('.data', '.sdata', '.rodata', '.rdata', '.lit4', '.lit8', '.bss', '.sbss')
    assert not any(s['size'] for s in secs if s['name'] in own_names)
    ld = work / (obj.stem + '.ld')
    ld.write_text('SECTIONS { .text 0x80000000 : { *(.text) } }\n' +
                  ''.join('%s = 0x%X;\n' % x for x in sorted(externs.items())))
    elf = work / (obj.stem + '.elf')
    run(['mips-linux-gnu-ld', '-EB', '-T', str(ld), '-o', str(elf), str(obj)])
    linked, lsecs = score._elf(elf)
    lt = lsecs[score._text_index(lsecs)]
    reports, bodies, covered = {}, {}, set()
    placed_addresses = dict(addresses, **{n: 0x80000000 + f['value'] for n, f in fns.items()})
    for n, f in fns.items():
        start, size = f['value'], f['size']
        end = start + size
        assert size > 0 and size % 4 == 0 and end <= len(raw)
        assert not covered.intersection(range(start, end))
        body = linked[lt['off'] + start:lt['off'] + end]
        got = list(struct.unpack('>%dI' % (len(body) // 4), body))
        placed, masks, unknown, unverified, errors = score.relocate(obj, words, start, end, placed_addresses)
        assert not any([masks, unknown, unverified, errors])
        assert got == placed[start // 4:end // 4], n
        resolved, masks, unknown, unverified, errors = score.relocate(obj, words, start, end, addresses)
        assert not any([masks, unknown, unverified, errors])
        got = resolved[start // 4:end // 4]
        body = struct.pack('>%dI' % len(got), *got)
        want = score.targets()[n]
        differences = [i * 4 for i in range(max(len(got), len(want)))
                       if i >= len(got) or i >= len(want) or got[i] != want[i]]
        canonical = asdict(score.compare(obj, n, show=0))
        assert not any(canonical[k] for k in ['unresolved', 'unverified', 'errors'])
        reports[n] = {'symbol_bytes': size, 'native_bytes': len(want) * 4,
                      'complete_differing_words': len(differences),
                      'differing_offsets': differences, 'body_sha256': sha(body),
                      'gnu_equals_project_relocation': True, 'canonical': canonical,
                      'relocations': [dict(r, offset=r['offset'] - start) for r in rels
                                      if start <= r['offset'] < end]}
        bodies[n] = got
        covered.update(range(start, end))
    outside = bytes(raw[i] for i in range(len(raw)) if i not in covered)
    assert not any(outside), 'Nonzero instructions outside complete ELF function extents'
    return {'functions': reports, 'owned_data_bytes': 0,
            'excluded_zero_text_padding_bytes': len(outside),
            'gnu_unmodified_object_text_address': '0x80000000',
            'native_comparison_uses_explicit_original_entry_map': True}, bodies


def negative_elf_checks(obj, work):
    data, secs = score._elf(obj)
    ti = score._text_index(secs)
    bads = []
    for si, sec in enumerate(secs):
        if sec['type'] == 2:
            for i, s in enumerate(score._symbol_table(data, secs, si)):
                if s['name'] == FN:
                    bad = bytearray(data)
                    # Truncate beyond the final nonzero instruction; a trailing
                    # zero word alone cannot distinguish code from alignment.
                    struct.pack_into('>I', bad, sec['off'] + i * 16 + 8, s['size'] - 8)
                    bads.append(('bad_extent', bad))
        elif sec['type'] == 9 and sec['info'] == ti:
            bad = bytearray(data)
            info = struct.unpack_from('>I', bad, sec['off'] + 4)[0]
            struct.pack_into('>I', bad, sec['off'] + 4, (info & ~255) | 255)
            bads.append(('bad_relocation', bad))
    assert len(bads) == 2
    for name, bad in bads:
        p = work / (name + '.o')
        p.write_bytes(bad)
        try:
            link_proof(p, work)
        except (AssertionError, SystemExit):
            pass
        else:
            raise AssertionError('Corrupted ELF accepted: ' + name)
    return [name for name, _ in bads]


def cases():
    heights = list(range(-40, 41))
    for q in [-65537, -32769, -32768, -14, -1, 0, 1, 12, 13, 14, 32767, 32768, 65535]:
        heights += [24 + q * 16 + r for r in [-15, -1, 0, 1, 15]]
    heights += [-0x80000000, -0x80000000 + 23, -0x80000000 + 24, 0x7FFFFFFF]
    for i, h in enumerate(sorted(set(heights))):
        for flags in [0, 1, 2, 0xFFFFFFFF, 0x80000000]:
            for mode in range(4):
                yield h, i % 7, flags, mode
    for bit in range(32):
        for h in [-1, 23, 24, 25, 231, 232, 247, 248, 0x7FFFFFFF]:
            for mode in range(4):
                yield h, bit % 7, 1 << bit, mode
    v = 1
    for i in range(1024):
        v = (v * 1664525 + 1013904223) & 0xFFFFFFFF
        yield native.signed(v), i % 7, v ^ 0x19325147, i % 4


def behavior(bodies, work):
    shared = work / 'host.so'
    flags = ['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror', '-O2',
             '-fsanitize=undefined', '-fno-sanitize-recover=all']
    run(flags + ['-fPIC', '-shared', str(SOURCE), str(HERE / 'host.c'), '-o', str(shared)])
    lib = ctypes.CDLL(str(shared))
    f = lib.run_case
    f.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_uint, ctypes.c_int]
    f.restype = ctypes.c_int
    codes = {'native': (score.targets()[FN], 18),
             'standalone': (bodies['standalone'][FN], 4),
             'clean_context': (bodies['clean_context'][FN], 17)}
    cov = {n: set() for n in codes}
    branches = {n: {} for n in codes}
    count = 0
    for h, index, flags_value, mode in cases():
        assert f(h, index, flags_value, mode) == native.oracle(h, flags_value, mode)
        for name, (code, reg) in codes.items():
            seen, outcomes = native.Machine(code, h, index, flags_value, mode, reg).run()
            cov[name].update(seen)
            for at, values in outcomes.items():
                branches[name].setdefault(at, set()).update(values)
        count += 1
    # Negative-index behavior is tested only against the native word-view
    # model; the C fixture does not invent enclosing storage before its array.
    for index in [-32768, -2, -1, 7, 32767]:
        native.Machine(score.targets()[FN], 24, index, 0, 0).run()
    binary = work / 'sanitized-host'
    run(['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror', '-O1',
         '-DHOST_MAIN', '-fsanitize=address,undefined', '-fno-sanitize-recover=all',
         str(SOURCE), str(HERE / 'host.c'), '-o', str(binary)])
    sanitized = run([str(binary)]).strip()
    assert sanitized == 'sanitized source cases: 40000'
    return {'source_native_and_compiled_cases': count, 'sanitized_source_cases': 40000,
            'additional_native_signed_index_cases': 5,
            'selector_argument_registers': {'native': 's2', 'standalone': 'a0', 'clean_context': 's1'},
            'hooks_are_abi_specific_not_full_callee_execution': True,
            'coverage': {n: {'visited_words': len(cov[n]), 'symbol_words': len(codes[n][0]),
                             'unvisited_offsets': sorted(set(range(0, len(codes[n][0]) * 4, 4)) - cov[n]),
                             'branch_outcomes': {str(k): sorted(v) for k, v in sorted(branches[n].items())}}
                         for n in codes}}


def verify():
    origin = ROOT / 'cloud/work/game_C104/group_complete_callers.c'
    original_context = origin.read_text()
    expected_context = original_context.replace(
        'typedef struct {u32 opaque[6];} Queue24;\nextern Queue24 D_801461D0;',
        'extern u8 D_801461D0[];').replace(
        's32 osRecvMesg(Queue24*,void**,s32); s32 osJamMesg(Queue24*,void*,s32);',
        's32 osRecvMesg(void*,void*,s32); s32 osJamMesg(void*,void*,s32);').replace(
        '&D_801461D0', 'D_801461D0')
    expected_context = ('/* Complete C104 semantic context; declarations of the shared queue are\n'
                        ' * harmonized with candidate.c. The empty hook has no synthetic blocker.\n'
                        ' * This context remains nonmatching and is not proposed as accepted source.\n'
                        ' */\n' + expected_context)
    assert (HERE / 'context.c').read_text() == expected_context
    build = ROOT / 'build' / 'selector_caller_contract'
    build.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(dir=build, prefix='replay-') as tmp:
        work = Path(tmp)
        # Keep IDO's intermediate files out of the nearly full system /tmp.
        old_tempdir = tempfile.tempdir
        tempfile.tempdir = str(work)
        try:
            reports, bodies = {}, {}
            single = work / 'standalone.o'
            score.compile_single(SOURCE, score.DEFAULT_FLAGS, single)
            group = work / 'clean_context.o'
            score.compile_group(HERE, group)
            for n, obj in [('standalone', single), ('clean_context', group)]:
                reports[n], bodies[n] = link_proof(obj, work)
            negatives = negative_elf_checks(single, work)
            behavioral = behavior(bodies, work)
        finally:
            tempfile.tempdir = old_tempdir
    targets, symbols = score.targets(), score.image_symbols()
    incoming = []
    for n, words in targets.items():
        for i, w in enumerate(words):
            if w >> 26 == 3 and 0x80000000 | ((w & 0x3FFFFFF) << 2) == symbols[FN]:
                incoming.append({'caller': n, 'site': '0x%08X' % (symbols[n] + i * 4)})
    assert incoming == [{'caller': 'func_800D91A0', 'site': '0x800D9310'}]
    assert reports['standalone']['functions'][FN]['symbol_bytes'] == 336
    assert reports['clean_context']['functions'][FN]['symbol_bytes'] == 364
    assert reports['standalone']['functions'][FN]['canonical']['differing'] == 72
    assert reports['clean_context']['functions'][FN]['canonical']['differing'] == 77
    return {'status': 'NONMATCH', 'claims': [], 'new_verified_matching_bytes': 0,
            'base': 'cd22879d40b3de443cfde047b86e75e159b6cec6',
            'function': FN, 'start': '0x800D9058', 'end': '0x800D91A0', 'native_bytes': 328,
            'native_sha256': sha(struct.pack('>82I', *targets[FN])),
            'source_hashes': {p.name: sha(p.read_bytes()) for p in sorted(HERE.iterdir())
                              if p.suffix in ['.py', '.c'] or p.name == 'group.json'},
            'real_parent': incoming, 'parent_reconstructed': False,
            'context_origin': str(origin.relative_to(ROOT)),
            'context_origin_sha256': sha(origin.read_bytes()),
            'protected_target_manifest_sha256': sha((score.ASM_DIR / 'SHA256SUMS').read_bytes()),
            'compile': reports, 'behavior': behavioral, 'negative_elf_drills': negatives,
            'literal_ownership': 'No candidate/group owned data sections or literal references',
            'source_originality': 'Behavior reconstruction; original declarations and sufficient private-ABI closure unresolved',
            'runtime_image_or_rom_verified': False}


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--write', action='store_true')
    args = parser.parse_args()
    result = verify()
    text = json.dumps(result, indent=2) + '\n'
    if args.write:
        (HERE / 'verification.json').write_text(text)
    else:
        assert result == json.loads((HERE / 'verification.json').read_text()), 'Receipt changed'
    print(json.dumps({'status': result['status'], 'cases': result['behavior']['source_native_and_compiled_cases'],
                      'new_verified_matching_bytes': 0}))
