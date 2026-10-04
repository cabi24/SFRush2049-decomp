#!/usr/bin/env python3
"""Recompile all retained complete nonmatches and compare every relocated word."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile

PACKET = Path(__file__).resolve().parent

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', type=Path, default=PACKET.parents[3])
    parser.add_argument('--ido-dir', type=Path)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    root = args.repo.resolve()
    sys.path.insert(0, str(root))
    from tools.cloud import score
    if args.ido_dir:
        score.IDO = args.ido_dir.resolve()
    score.ASM_DIR = root / 'asm/us/boot_tail'
    manifest = subprocess.check_output(['sha256sum', '-c', 'SHA256SUMS'],
                                       cwd=score.ASM_DIR, text=True).splitlines()
    inventory = json.loads((root / 'specs/015-boot-tail-runtime/inventory.json').read_text())
    extents = json.loads((score.ASM_DIR / 'extents.json').read_text())
    keyed = lambda data: {row['address']: row['size'] for row in data['functions']}
    assert keyed(inventory) == keyed(extents)
    assert len(keyed(inventory)) == 439
    assert sum(keyed(inventory).values()) == 99120
    pinned = json.loads((PACKET / 'preflight.json').read_text())
    for relative, expected in pinned['source_hashes'].items():
        assert digest(root / relative) == expected, relative
    compiler_hashes = {p.name: digest(p) for p in score.IDO.iterdir() if p.is_file()}
    assert compiler_hashes == pinned['compiler_files_sha256']
    expected_scores = {'800158D8': {'O2': (8, 77, 0), 'O1': (74, 77, 26)},
                       '80015F28': {'O2': (8, 77, 0), 'O1': (74, 77, 26)},
                       '8001661C': {'O2': (31, 64, 0), 'O1': (63, 64, 16)}}
    rows = []
    with tempfile.TemporaryDirectory(prefix='bt03-low-removal-verify-') as tmp:
        tmp = Path(tmp)
        getter = root / 'cloud/matches/boot_tail/func_80010A00.c'
        score.compile_single(getter, score.DEFAULT_FLAGS, tmp / 'getter.o')
        assert score.compare(tmp / 'getter.o', getter.stem, show=0).accepted()
        for address in ['800158D8', '80015F28', '8001661C']:
            name = 'func_' + address
            source = PACKET / (name + '_NONMATCH.c')
            for opt in ['O2', 'O1']:
                flags = '-g0 -' + opt + ' -mips2 -G 0 -non_shared'
                obj = tmp / (name + '-' + opt + '.o')
                score.compile_single(source, flags, obj)
                result = score.compare(obj, name, show=0)
                assert not result.accepted()
                assert not (result.unresolved or result.unverified or result.errors)
                assert (result.differing, result.total, result.extra_words) == expected_scores[address][opt]
                rows.append({'function': name, 'source_sha256': digest(source),
                             'flags': flags, 'effective_flags': flags + ' -Wab,-r4300_mul',
                             'differing_words': result.differing, 'total_words': result.total,
                             'extra_words': result.extra_words,
                             'unresolved': result.unresolved, 'unverified': result.unverified,
                             'errors': result.errors, 'strict_match': result.accepted()})
            # Compiler-native geometry, independent of host pointer width.
            offset_id = 0 if address == '8001661C' else 4
            offset_second = offset_id + 2
            layout = tmp / (name + '-layout.c')
            layout.write_text(source.read_text() + '\n' +
                'typedef char check_record_size[(sizeof(RecordStorage) == 8) ? 1 : -1];\n' +
                'typedef char check_fields_size[(sizeof(RecordFields) == 8) ? 1 : -1];\n' +
                'typedef char check_id_offset[(__offsetof(RecordFields,id) == %d) ? 1 : -1];\n' % offset_id)
            # IDO does not provide a portable __offsetof builtin; standard C89 offsetof.
            text = layout.read_text().replace('__offsetof(RecordFields,id)',
                                             '((unsigned int)&((RecordFields *)0)->id)')
            layout.write_text(text)
            score.compile_single(layout, score.DEFAULT_FLAGS, tmp / (name + '-layout.o'))
    report = {'schema_version': 1, 'packet': 'BT03-low-removal', 'manifest': manifest,
              'extents': {'functions': 439, 'bytes': 99120, 'equal': True},
              'pinned_compiler_files': len(compiler_hashes), 'getter_strict_match': True,
              'native_layout': 'PASS: RecordFields and RecordStorage are eight bytes; id offsets 4/4/0',
              'results': rows, 'verified_match_functions': sum(r['strict_match'] for r in rows)}
    rendered = json.dumps(report, indent=2) + '\n'
    if args.output:
        args.output.write_text(rendered)
    else:
        print(rendered, end='')

if __name__ == '__main__':
    main()
