#!/usr/bin/env python3
"""Strict research replay. Prints hashes/counts only; no raw target bytes."""
import contextlib
import dataclasses
import hashlib
import io
import json
import os
import re
from pathlib import Path
import struct
import subprocess
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor.pipeline import blob_group

CLAIM = 'func_8010A7A4'
CONTEXT = 'func_800BEA3C'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
sha = lambda data: hashlib.sha256(data).hexdigest()
pack = lambda words: struct.pack('>%dI' % len(words), *words)


def comparison(obj, name):
    with contextlib.redirect_stdout(io.StringIO()):
        return dataclasses.asdict(score.compare(obj, name, show=0))


def verify():
    build = ROOT / 'build/menu_row_caller'
    build.mkdir(parents=True, exist_ok=True)
    obj = build / 'verified.o'
    spec = score.compile_group(HERE, obj)
    targets, addresses = score.targets(), score.image_symbols()
    names = spec['members'] + spec['context']
    offsets = score.symbols(obj)
    text = score.text_words(obj)
    data, sections = score._elf(obj)
    functions = {
        sym['name']: sym for i, sec in enumerate(sections) if sec['type'] == 2
        for sym in score._symbol_table(data, sections, i) if sym['type'] == 2 and sym['name'] in offsets
    }
    image = owndata.ImageData.from_artifact(ROOT / 'asm/us/blob_data')
    own = owndata.verify(obj, CLAIM, targets[CLAIM], address=addresses[CLAIM], image=image)
    assert own.ok and own.references == 1
    assert own.placements == {'.rodata': [(0, 4, 0x801248D4, 'rodata')]}
    # Production relocation consumes a sparse research image made solely from
    # integrity-checked target extents and own-data artifact. This is not a ROM
    # build/image gate, and unknown holes are not used as evidence.
    extents = {name: {'vaddr': addresses[name], 'size': len(targets[name])*4} for name in names}
    slices, ndx = blob_group.member_slices(obj, names, extents)
    runs = [(addresses[name], pack(targets[name])) for name in names] + image.runs
    base = min(a for a, _ in runs)
    end = max(a+len(b) for a, b in runs)
    sparse = bytearray(end-base)
    for a, b in runs:
        sparse[a-base:a-base+len(b)] = b
    bodies = blob_group.relocate(obj, slices, ndx, addresses,
        members=[CLAIM, CONTEXT], image=(bytes(sparse), base))
    # Independently resolve the complete ELF with GNU ld. The proof function
    # calls no other local function: its endpoint setter has been inlined.
    text_base = addresses[CLAIM] - offsets[CLAIM]
    linker_script = build / 'verify.ld'
    link_addresses = dict(addresses)
    for i, section in enumerate(sections):
        if section['type'] == 2:
            for symbol in score._symbol_table(data, sections, i):
                name = symbol['name']
                if symbol['section'] == 0 and re.fullmatch(r'D_[0-9A-Fa-f]{8}', name):
                    link_addresses.setdefault(name, int(name[2:], 16))
    linker_script.write_text(
        'SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } .rodata 0x801248D4 : SUBALIGN(4) { *(.rodata) } }\n' % text_base
        + ''.join('%s = 0x%x;\n' % (n, a) for n, a in sorted(link_addresses.items()) if n not in functions))
    linked = build / 'linked.elf'
    subprocess.run(['mips-linux-gnu-ld', '-EB', '-T', str(linker_script), '-o', str(linked), str(obj)], check=True, capture_output=True)
    linked_bin = build / 'linked-text.bin'
    subprocess.run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.text', str(linked), str(linked_bin)], check=True)
    independent = linked_bin.read_bytes()
    rows = []
    for name in names:
        start = offsets[name]
        end = min((v for v in offsets.values() if v > start), default=len(text)*4)
        native = pack(targets[name])
        row = {
            'function': name, 'claimed': name == CLAIM,
            'target_address': '0x%08X' % addresses[name],
            'target_bytes': len(native), 'target_sha256': sha(native),
            'elf_st_size': functions[name]['size'],
            'extent_to_next_symbol_or_text_end': end-start,
            'canonical_comparison': comparison(obj, name),
        }
        if name in [CLAIM, CONTEXT]:
            assert bodies[name] == native
            assert independent[start:start+len(native)] == native
            assert functions[name]['size'] == len(native) and end-start == len(native)
            assert row['canonical_comparison']['differing'] == 0
            assert row['canonical_comparison']['extra_words'] == 0
            assert not row['canonical_comparison']['errors']
            assert not row['canonical_comparison']['unresolved']
            row.update({'production_relocation_full_extent_equal': True,
                'independent_gnu_link_full_extent_equal': True,
                'fully_resolved_sha256': sha(bodies[name]),
                'unresolved_after_proof': [], 'unverified_after_proof': [], 'errors_after_proof': []})
        else:
            row['claimed'] = False
            row['status'] = 'complete genuine caller; NONMATCH; no context coverage credit'
        rows.append(row)
    # The copied endpoint logic and packed view must preserve the accepted
    # standalone setter, in addition to the grouped context replay above.
    accepted_obj = build / 'accepted.o'
    score.compile_single(ROOT/'src/blob/func_800BEA3C.c', score.DEFAULT_FLAGS, accepted_obj)
    accepted = comparison(accepted_obj, CONTEXT)
    assert accepted == {'differing': 0, 'total': 7, 'unresolved': [], 'unverified': [], 'errors': [], 'extra_words': 0}
    score.compile_single(HERE/'layout_probe.c', FLAGS, build/'layout.o')
    host = build/'host_test'
    subprocess.run(['cc', '-std=c89', '-Wall', '-Wextra', '-Werror', '-O1', '-fsanitize=address,undefined',
        '-fno-sanitize-recover=all', str(HERE/'host_test.c'), '-o', str(host)], check=True)
    subprocess.run([str(host)], check=True, env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0'))
    source = (HERE/'group.c').read_text()
    controls = []
    cases = {
        'aggregate_byte_copy': source.replace('union Color4 { struct { u8 r,g,b,a; } channels; u32 rgba; }',
            'struct Color4 { u8 r,g,b,a; }').replace('D_80118E28.rgba = first.rgba;', 'D_80118E28 = first;')
            .replace('D_80118E2C.rgba = second.rgba;', 'D_80118E2C = second;'),
        'aggregate_word_copy': source.replace('D_80118E28.rgba = first.rgba;', 'D_80118E28 = first;')
            .replace('D_80118E2C.rgba = second.rgba;', 'D_80118E2C = second;'),
        'separate_dispatch_calls': source.replace('dispatch_handler(index == D_80116D9C ? 22 : 1);',
            'if (index == D_80116D9C) dispatch_handler(22); else dispatch_handler(1);'),
    }
    for label, body in cases.items():
        assert body != source
        directory = build/'controls'/label
        directory.mkdir(parents=True, exist_ok=True)
        (directory/'group.c').write_text(body)
        (directory/'group.json').write_text(json.dumps(spec))
        control_obj = directory/'out.o'
        score.compile_group(directory, control_obj)
        result = comparison(control_obj, CLAIM)
        assert result['differing'] > 0
        controls.append({'name': label, 'source_sha256': sha(body.encode()),
            'helper_comparison': result, 'accepted_context_comparison': comparison(control_obj, CONTEXT)})
    return {
        'base_commit': 'cf10b3392d7f00ae42d75c008b79fdc2541aab6b',
        'claims': [CLAIM], 'new_verified_function_bytes': 300,
        'promotion_status': 'research proof only; no splice, lock or ROM claim',
        'flags': FLAGS, 'source_sha256': sha((HERE/'group.c').read_bytes()),
        'manifest_sha256': sha((HERE/'group.json').read_bytes()),
        'accepted_context_source_sha256': sha((ROOT/'src/blob/func_800BEA3C.c').read_bytes()),
        'accepted_context_standalone': accepted,
        'own_literal': {'address': '0x801248D4', 'bytes': 4, 'references': own.references,
            'sha256': sha(image.read(0x801248D4, 4)), 'verified': own.ok},
        'compiler_sha256': {n: sha((score.IDO/n).read_bytes()) for n in ['cc','uld','umerge','uopt','ugen','as1']},
        'native_layout_checks': 9, 'host_behavior_cases': 1769, 'asan_ubsan': 'passed',
        'results': rows, 'rejected_controls': controls,
    }


if __name__ == '__main__':
    result = verify()
    text = json.dumps(result, indent=2, sort_keys=True)+'\n'
    if len(sys.argv) > 1:
        Path(sys.argv[1]).write_text(text)
    print(text)
