#!/usr/bin/env python3
"""Fail-closed standalone replay; no production inputs are modified."""
import argparse
import contextlib
import dataclasses
import hashlib
import io
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile

NAME = 'records_screen'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
BASE = '7e62ed7b3c3e6f1e788bf013419b89e132b4c65d'
ADDRESS = 0x800D58CC
SIZE = 312
BODY_SHA256 = '8bc788ec8d193185d9285d90d41bcf883934f5677ed2dbb59c50a665a4beb397'


def check(condition, message):
    if not condition:
        raise RuntimeError(message)


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def run(args):
    return subprocess.check_output([str(x) for x in args], text=True, stderr=subprocess.STDOUT)


def function_identity(symbols, name):
    found = re.findall(r'^\s*\d+:\s+([0-9a-fA-F]+)\s+(\d+)\s+FUNC\s+GLOBAL\s+DEFAULT\s+\d+\s+' + re.escape(name) + r'$', symbols, re.M)
    check(len(found) == 1, 'expected exactly one defined function: ' + name)
    return int(found[0][0], 16), int(found[0][1])


def elf_identity(data, expected_type):
    check(len(data) >= 52, 'truncated ELF header')
    check(data[:16] == b'\x7fELF\x01\x02\x01' + bytes(9), 'ELF identification mismatch')
    elf_type, machine, version = struct.unpack_from('>HHI', data, 16)
    flags, = struct.unpack_from('>I', data, 36)
    check(elf_type == expected_type, 'ELF type mismatch')
    check(machine == 8 and version == 1, 'ELF machine/version mismatch')
    check(flags == 0x10000000, 'ELF MIPS2 ABI flags mismatch')
    return {'class': 'ELF32', 'byte_order': 'big', 'osabi': 'SYSV', 'abi_version': 0,
            'type': expected_type, 'machine': machine, 'version': version, 'flags': hex(flags)}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--repo', type=Path, default=Path(__file__).resolve().parents[4])
    ap.add_argument('--history-repo', type=Path)
    ap.add_argument('--source', type=Path)
    ap.add_argument('--output', type=Path)
    ap.add_argument('--local-provenance', action='store_true', help='include optional local tool identities')
    args = ap.parse_args()
    repo = args.repo.resolve()
    history = (args.history_repo or repo).resolve()
    packet = Path(__file__).resolve().parent
    source = (args.source or repo / 'cloud/work/frontier/dot_records_cleanup_20261006/records_screen.c').resolve()
    bindings = json.loads((packet / 'bindings.json').read_text())
    packet_files = {'source': source, 'verifier': Path(__file__).resolve(), 'host_contract': packet / 'host_contract.c'}
    for label, path in packet_files.items():
        check(sha(path) == bindings['packet_files'][label], 'packet binding mismatch: ' + label)
    check(bindings['base_commit'] == BASE, 'frozen base mismatch')
    check(bindings['native_body_sha256'] == BODY_SHA256, 'native body binding mismatch')
    contexts = {}
    for path, expected in bindings['frozen_contexts'].items():
        data = subprocess.check_output(['git', '-C', str(history), 'show', BASE + ':' + path])
        actual = hashlib.sha256(data).hexdigest()
        check(actual == expected, 'frozen context mismatch: ' + path)
        contexts[path] = actual
    ido_dir = Path(os.environ.get('IDO_DIR', repo / 'tools/cloud/ido'))
    check((ido_dir / 'cc').is_file(), 'missing IDO compiler: ' + str(ido_dir / 'cc'))
    for tool in ['mips-linux-gnu-readelf', 'mips-linux-gnu-nm', 'mips-linux-gnu-ld', 'mips-linux-gnu-objcopy', os.environ.get('CC', 'cc')]:
        check(shutil.which(tool) is not None, 'missing required tool: ' + tool)
    sys.path.insert(0, str(repo / 'tools/cloud'))
    import score
    receipt = {'base_commit': BASE, 'target': NAME, 'address': hex(ADDRESS),
               'native_bytes': SIZE, 'flags': FLAGS, 'required_as1_flag': score.R4300_CC,
               'packet_files': bindings['packet_files'], 'frozen_contexts': contexts}
    if args.local_provenance:
        receipt['local_provenance'] = {'scorer_sha256': sha(repo / 'tools/cloud/score.py'),
                                       'ido_cc_sha256': sha(score.IDO / 'cc'),
                                       'target_manifest_sha256': sha(repo / 'asm/us/blob/SHA256SUMS')}
    with tempfile.TemporaryDirectory(prefix='records-cleanup-') as temp:
        temp = Path(temp)
        obj = temp / 'candidate.o'
        score.compile_single(source, FLAGS, obj)
        with contextlib.redirect_stdout(io.StringIO()):
            result = score.compare(obj, NAME)
        check(result.accepted() and not result.unverified, 'strict comparison failed: ' + result.summary())
        receipt['strict'] = dataclasses.asdict(result)
        receipt['scorer_output'] = result.summary()
        want = struct.pack('>78I', *score.targets()[NAME])
        check(len(want) == SIZE and hashlib.sha256(want).hexdigest() == BODY_SHA256, 'target body identity mismatch')
        receipt['native_body_sha256'] = BODY_SHA256
        check(function_identity(run(['mips-linux-gnu-readelf', '-Ws', obj]), NAME) == (0, SIZE), 'object function extent mismatch')
        data, sections = score._elf(obj)
        receipt['object_elf_identity'] = elf_identity(data, 1)
        defined_functions = [sym for index, sec in enumerate(sections) if sec['type'] == 2
                             for sym in score._symbol_table(data, sections, index)
                             if sym['type'] == 2 and sym['section'] != 0]
        check(len(defined_functions) == 1 and defined_functions[0]['name'] == NAME
              and defined_functions[0]['value'] == 0 and defined_functions[0]['size'] == SIZE,
              'unexpected defined-function topology')
        shoff, = struct.unpack_from('>I', data, 32)
        shentsize, = struct.unpack_from('>H', data, 46)
        for index, sec in enumerate(sections):
            section_flags, = struct.unpack_from('>I', data, shoff + index * shentsize + 8)
            check(not (section_flags & 2 and sec['size'] and sec['name'] not in ('.text', '.reginfo')),
                  'unexpected allocated section: ' + sec['name'])
        relocation_tuples = []
        names = {4: 'R_MIPS_26', 5: 'R_MIPS_HI16', 6: 'R_MIPS_LO16'}
        for sec in sections:
            if sec['type'] != 9:
                continue
            check(sections[sec['info']]['name'] == '.text', 'unexpected relocation section')
            symbols = score._symbol_table(data, sections, sec['link'])
            for offset in range(0, sec['size'], 8):
                site, info = struct.unpack_from('>II', data, sec['off'] + offset)
                kind = info & 0xFF
                check(kind in names and site < SIZE and site % 4 == 0, 'unexpected relocation topology')
                relocation_tuples.append({'site': hex(site), 'type': names[kind], 'symbol': symbols[info >> 8]['name']})
        check(relocation_tuples == bindings['relocations'], 'relocation tuple mismatch')
        receipt['relocations'] = relocation_tuples
        text = next(sec for sec in sections if sec['name'] == '.text')
        check(text['size'] == 320 and data[text['off'] + SIZE:text['off'] + text['size']] == bytes(8), 'unexpected text padding or extra body bytes')
        addresses = score.image_symbols()
        assignments = []
        for line in run(['mips-linux-gnu-nm', '-u', obj]).splitlines():
            name = line.split()[-1]
            address = addresses.get(name, score.address_named(name))
            check(address is not None, 'unresolved symbol: ' + name)
            assignments.append('{} = 0x{:08X};'.format(name, address))
        script = temp / 'candidate.ld'
        script.write_text('\n'.join(assignments) + '\nSECTIONS { .text 0x800D58CC : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.options) *(.mdebug) } }\n')
        linked = temp / 'candidate.elf'
        run(['mips-linux-gnu-ld', '--hash-style=sysv', '-T', script, '-o', linked, obj])
        receipt['linked_elf_identity'] = elf_identity(linked.read_bytes(), 2)
        linked_address, linked_size = function_identity(run(['mips-linux-gnu-readelf', '-Ws', linked]), NAME)
        check(linked_address == ADDRESS, 'linked function address mismatch')
        check(linked_size == SIZE, 'linked function extent mismatch')
        raw = temp / 'candidate.bin'
        run(['mips-linux-gnu-objcopy', '-O', 'binary', '--only-section=.text', linked, raw])
        linked_bytes = raw.read_bytes()
        check(len(linked_bytes) == 320 and linked_bytes[:SIZE] == want and linked_bytes[SIZE:] == bytes(8), 'GNU-linked complete body or padding mismatch')
        relocation_text = run(['mips-linux-gnu-readelf', '-r', obj])
        relocations = [line.split()[2] for line in relocation_text.splitlines() if 'R_MIPS_' in line]
        check(len(relocations) == 20, 'unexpected relocation count')
        receipt.update({'elf_function_bytes': SIZE, 'gnu_linked_address': hex(linked_address),
                        'gnu_linked_function_bytes': linked_size, 'text_alignment_bytes': 8,
                        'relocation_count': len(relocations),
                        'relocation_kinds': {kind: relocations.count(kind) for kind in sorted(set(relocations))},
                        'owned_data_bytes': 0, 'gnu_linked_body_sha256': hashlib.sha256(linked_bytes[:SIZE]).hexdigest(),
                        'gnu_full_body_equal': True})
        host = temp / 'host'
        cc = os.environ.get('CC', 'cc')
        host_flags = [cc, '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-fsanitize=undefined', '-fno-sanitize-recover=all']
        run(host_flags + ['-DRECORDS_SOURCE="{}"'.format(source), packet / 'host_contract.c', '-o', host])
        receipt['host_contract'] = run([host]).strip()
        negative_changes = {
            'wrong_sentinel': ('const int sentinel = -1;', 'const int sentinel = -2;'),
            'wrong_mode': ('D_8014A110==6 || D_8014A110==4', 'D_8014A110==6 || D_8014A110==5'),
            'wrong_recursive_flag': ('entity_spawn_callback((s16)id,0,0);', 'entity_spawn_callback((s16)id,1,0);'),
        }
        receipt['host_negative_controls'] = {}
        for label, (before, after) in negative_changes.items():
            original = source.read_text()
            check(before in original, 'negative-control anchor missing: ' + label)
            bad_source = temp / (label + '.c')
            bad_source.write_text(original.replace(before, after))
            bad_host = temp / label
            run(host_flags + ['-DRECORDS_SOURCE="{}"'.format(bad_source), packet / 'host_contract.c', '-o', bad_host])
            control = subprocess.run([str(bad_host)], capture_output=True, text=True)
            check(control.returncode != 0, 'wrong-contract control unexpectedly passed: ' + label)
            receipt['host_negative_controls'][label] = 'rejected'
        receipt['coverage_claim'] = 'standalone object match; image/compression/ROM gates not run'
    rendered = json.dumps(receipt, indent=2) + '\n'
    if args.output:
        args.output.write_text(rendered)
    print(rendered, end='')


if __name__ == '__main__':
    main()
