#!/usr/bin/env python3
"""Reproduce a bounded NONMATCH; never splice or change shared proof inputs.

Requires the existing IDO_DIR plus GNU MIPS binutils on PATH. Builds use a
temporary directory. Receipts contain hashes and offsets, never retail words.
"""
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
ROOT = HERE.parents[2]
sys.path.insert(0, str(ROOT / 'tools/cloud'))
sys.path.insert(0, str(HERE))
import score
import owndata

NAME = 'arb_rate_set'
CALLEE = 'exhaust_smoke_effect'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
BASE = '55ddfc6b'


def sha(data):
    return hashlib.sha256(data).hexdigest()


def pack(words):
    return struct.pack('>%dI' % len(words), *words)


def function_symbol(obj, name):
    data, sections = score._elf(obj)
    found = [s for i, sec in enumerate(sections) if sec['type'] == 2
             for s in score._symbol_table(data, sections, i)
             if s['name'] == name and s['type'] == 2 and s['section'] != 0]
    assert len(found) == 1
    return found[0]


def full_report(obj, name):
    """Use STT_FUNC size, retaining relocations after the target-sized prefix."""
    fn = function_symbol(obj, name)
    start, end = fn['value'], fn['value'] + fn['size']
    raw = score.text_words(obj)
    got, masks, unresolved, unverified, errors = score.relocate(
        obj, raw, start, end, score.image_symbols())
    want = score.targets()[name]
    body = got[start // 4:end // 4]
    bad = [i * 4 for i, word in enumerate(want)
           if i >= len(body) or word != body[i]]
    return {
        'elf_function_bytes': fn['size'], 'target_bytes': len(want) * 4,
        'full_extent_different_offsets': bad,
        'missing_bytes': max(0, len(want) * 4 - fn['size']),
        'excess_bytes': max(0, fn['size'] - len(want) * 4),
        'full_extent_relocations': {'masks': len(masks), 'unresolved': unresolved,
                                    'unverified': unverified, 'errors': errors},
        'canonical_scorer': dataclasses.asdict(score.compare(obj, name, show=0)),
        'object_sha256': sha(obj.read_bytes()),
    }, body


def variants():
    original = (ROOT / 'cloud/work/near_miss_B83/arb_rate_set_colors.c').read_text()
    integer = original.replace('*2.0f', '*2')
    opaque = integer.replace('entry->alpha>>7', '1')
    prefix = '    Descriptor72 *entry = &D_8017A510[index];\n    u16 packed;'
    result = {
        'float': original,
        'integer': integer,
        'double': original.replace('*2.0f', '*2.0'),
        'short_float': original.replace('*2.0f', '*2.f'),
        'opaque': opaque,
        'stored_alpha': integer.replace('entry->alpha>>7', 'entry->alpha'),
        'shift_cast': integer.replace('entry->alpha>>7', '((u32)entry->alpha)>>7'),
        'chain_rgb': opaque.replace('entry->red=0;\n    entry->green=0;\n    entry->blue=0;',
                                    'entry->red=entry->green=entry->blue=0;'),
        'viewport_local': opaque.replace('    u16 packed;',
                         '    u16 packed;\n    Viewport16 *viewport = &D_8011EA30;'),
        'viewport_first': opaque.replace(prefix,
                         '    Viewport16 *viewport = &D_8011EA30;\n' + prefix),
        'pointer_return': opaque.replace('    exhaust_smoke_effect(',
                                         '    entry = exhaust_smoke_effect('),
        'pack_u32': opaque.replace('    u16 packed;', '    u32 packed;').replace(
                                   'packed=GPACK', 'packed=(u16)GPACK'),
        'alpha_first': opaque.replace('    entry->red=0;\n    entry->green=0;\n'
            '    entry->blue=0;\n    entry->alpha=255;',
            '    entry->alpha=255;\n    entry->red=0;\n    entry->green=0;\n    entry->blue=0;'),
        'packed_before_alpha': opaque.replace('    entry->alpha=255;\n', '').replace(
                             '    D_80124FC8=', '    entry->alpha=255;\n    D_80124FC8='),
    }
    for key in ('viewport_local', 'viewport_first'):
        result[key] = result[key].replace('D_8011EA30.scale', 'viewport->scale').replace(
                                         'D_8011EA30.translate', 'viewport->translate')
    return result


def group(directory, source):
    directory.mkdir()
    (directory / 'candidate.c').write_text(source)
    callee = ROOT / ('src/blob/' + CALLEE + '.c')
    (directory / 'callee.c').write_bytes(callee.read_bytes())
    assert (directory / 'callee.c').read_bytes() == callee.read_bytes()
    (directory / 'group.json').write_text(json.dumps({
        'files': ['candidate.c', 'callee.c'], 'keep': [NAME, CALLEE],
        'flags': FLAGS, 'claims': []}))
    obj = directory / 'group.o'
    score.compile_group(directory, obj)
    return obj


def independent_link(obj, directory):
    """Link the actual contiguous 452-byte callee + 312-byte caller unit."""
    addr = score.image_symbols()
    fn, callee = function_symbol(obj, NAME), function_symbol(obj, CALLEE)
    assert fn['value'] == 452 and fn['size'] == 312
    assert callee['value'] == 0 and callee['size'] == 452
    image = owndata.ImageData.from_artifact(ROOT / 'asm/us/blob_data')
    assert image is not None
    own = owndata.verify(obj, CALLEE, score.targets()[CALLEE],
                         address=addr[CALLEE], image=image)
    assert own.ok and own.references == 3 and len(own.sites) == 6
    assert own.bases() == {'.rodata': 0x80123BC8}
    data, sections = score._elf(obj)
    undefined = sorted({s['name'] for i, sec in enumerate(sections) if sec['type'] == 2
                        for s in score._symbol_table(data, sections, i)
                        if s['section'] == 0 and s['name']})
    assert set(undefined) == {'D_8017A510', 'D_8011EA30', 'D_80151AA0', 'D_80124FC8',
                              'D_8002AFC4', 'func_8008C720', 'func_800A557C'}
    script = directory / 'link.ld'
    script.write_text('SECTIONS { .text 0x800A5744 : SUBALIGN(4) { *(.text) } '
                      '.rodata 0x80123BC8 : SUBALIGN(4) { *(.rodata) } }')
    ld, copy = shutil.which('mips-linux-gnu-ld'), shutil.which('mips-linux-gnu-objcopy')
    assert ld and copy
    elf, binary = directory / 'linked.elf', directory / 'linked.bin'
    subprocess.run([ld, '-T', str(script), '-e', NAME,
        *['--defsym=' + name + '=' + hex(addr[name]) for name in undefined],
        str(obj), '-o', str(elf)], check=True, capture_output=True)
    subprocess.run([copy, '-O', 'binary', '--only-section=.text', str(elf), str(binary)],
                   check=True, capture_output=True)
    linked = binary.read_bytes()
    assert linked[:452] == pack(score.targets()[CALLEE])
    caller = list(struct.unpack('>78I', linked[452:764]))
    record, canonical = full_report(obj, NAME)
    assert caller == canonical
    assert linked[764:] == b'\0' * 4
    assert len(record['full_extent_different_offsets']) == 31
    assert not any(record['full_extent_relocations'].values())
    return caller, {
        'callee_unchanged_source_sha256': sha((ROOT / ('src/blob/' + CALLEE + '.c')).read_bytes()),
        'callee_bytes': 452, 'callee_strict_words_equal': 113,
        'callee_owned_literal_references': own.references,
        'callee_owned_literal_relocation_sites': len(own.sites),
        'callee_owned_literal_bytes': 12,
        'callee_owned_data_failures': own.failures, 'callee_owned_data_unverified': own.unverified,
        'caller_relocated_sha256': sha(pack(caller)),
        'independent_gnu_link_agrees': True, 'outside_function_zero_alignment_bytes': 4,
        'resolved_symbols': {name: hex(addr[name]) for name in undefined},
    }


def run(output=None, include_variants=True):
    receipt = {'status': 'COMPLETE-NONMATCH', 'claims': [], 'base_commit': subprocess.check_output(
        ['git', 'rev-parse', BASE], cwd=ROOT, text=True).strip(), 'flags': FLAGS,
        'target_address': hex(score.image_symbols()[NAME]),
        'target_sha256': sha(pack(score.targets()[NAME])),
        'source_sha256': sha((HERE / 'candidate.c').read_bytes()),
        'compiler_sha256': sha((score.IDO / 'cc').read_bytes()),
        'toolchain_sha256': {tool: sha((score.IDO / tool).read_bytes())
                            for tool in ('cc', 'cfe', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1')},
        'protected_target_manifest_sha256': sha((ROOT / 'asm/us/blob/SHA256SUMS').read_bytes()),
        'owned_data_manifest_sha256': sha((ROOT / 'asm/us/blob_data/SHA256SUMS').read_bytes()),
        'experiments': []}
    with tempfile.TemporaryDirectory(prefix='viewport-rate-') as temporary:
        work = Path(temporary)
        if include_variants:
            for key, source in variants().items():
                path, obj = work / (key + '.c'), work / (key + '.o')
                path.write_text(source)
                score.compile_single(path, FLAGS, obj)
                result, _ = full_report(obj, NAME)
                result.update(variant=key, mode='single', source_sha256=sha(source.encode()))
                receipt['experiments'].append(result)
                if key in ('float', 'integer', 'double', 'short_float'):
                    obj = group(work / (key + '_group'), source)
                    result, _ = full_report(obj, NAME)
                    result.update(variant=key, mode='real_callee_group')
                    receipt['experiments'].append(result)
        source = (HERE / 'candidate.c').read_text()
        obj = group(work / 'final_group', source)
        receipt['candidate'], _ = full_report(obj, NAME)
        words, receipt['group_verification'] = independent_link(obj, work)
        from verify_semantics import run_cases
        receipt['semantics'] = run_cases(words, work)
    if output:
        Path(output).write_text(json.dumps(receipt, indent=2) + '\n')
    return receipt


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--output', type=Path)
    parser.add_argument('--skip-variants', action='store_true')
    args = parser.parse_args()
    result = run(args.output, not args.skip_variants)
    print(json.dumps({'status': result['status'],
                      'different_words': len(result['candidate']['full_extent_different_offsets']),
                      'candidate_bytes': result['candidate']['elf_function_bytes'],
                      'group_verification': result['group_verification'],
                      'semantics': result['semantics']}, indent=2))
