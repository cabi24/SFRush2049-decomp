#!/usr/bin/env python3
"""Replay conditional NONMATCH evidence without inventing native table bytes."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
NAME = 'func_80022CD4'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--repo', type=Path, default=HERE.parents[3])
    ap.add_argument('--ido-dir', type=Path, default=Path(os.environ.get('IDO_DIR', 'tools/cloud/ido')))
    ap.add_argument('--output', type=Path)
    args = ap.parse_args()
    root = args.repo.resolve()
    sys.path.insert(0, str(root))
    from tools.cloud import score
    score.IDO = args.ido_dir.resolve()
    score.ASM_DIR = root / 'asm/us/boot_tail'
    pin = json.loads((HERE / 'preflight.json').read_text())
    for name, want in pin['source_hashes'].items():
        assert sha(root / name) == want, name
    compiler = {p.name: sha(p) for p in score.IDO.iterdir() if p.is_file()}
    assert compiler == pin['compiler_files_sha256']
    manifest = subprocess.check_output(['sha256sum', '-c', 'SHA256SUMS'], cwd=score.ASM_DIR, text=True).splitlines()
    def extents(path):
        return {r['address']: r['size'] for r in json.loads(path.read_text())['functions']}
    inventory = extents(root / 'specs/015-boot-tail-runtime/inventory.json')
    assert inventory == extents(score.ASM_DIR / 'extents.json')
    assert len(inventory) == 439 and sum(inventory.values()) == 99120
    target = score.targets()[NAME]
    assert len(target) == 148
    # Prove both native LUI/ADDIU pairs produce the token address.
    for upper_at, lower_at in [(0x80022D70, 0x80022D80), (0x80022E64, 0x80022E74)]:
        upper = target[(upper_at - 0x80022CD4) // 4]
        lower = target[(lower_at - 0x80022CD4) // 4]
        assert upper >> 26 == 15 and lower >> 26 == 9
        assert (upper >> 16 & 31) == 7 and (lower >> 21 & 31) == 7 and (lower >> 16 & 31) == 7
        signed_low = (lower & 65535) - (65536 if lower & 32768 else 0)
        assert (((upper & 65535) << 16) + signed_low) & 0xFFFFFFFF == 0x8002D476
    target_hash = hashlib.sha256(struct.pack('>148I', *target)).hexdigest()
    assert target_hash == pin['target_body_sha256']
    assert not any(word >> 26 == 3 or (word >> 26 == 0 and word & 63 == 9) for word in target)
    # Only the known call dependency is inspected, not the dispatcher's other bodies.
    caller = score.targets()['func_80023E9C']
    at = (0x800244D8 - 0x80023E9C) // 4
    assert caller[at] >> 26 == 3
    assert 0x80000000 | ((caller[at] & 0x3FFFFFF) << 2) == 0x80022CD4
    source = HERE / (NAME + '_CONDITIONAL.c')
    results = []
    with tempfile.TemporaryDirectory(prefix='bt05-pitch-verify-') as folder:
        folder = Path(folder)
        getter = root / 'cloud/matches/boot_tail/func_80010A00.c'
        score.compile_single(getter, score.DEFAULT_FLAGS, folder / 'getter.o')
        assert score.compare(folder / 'getter.o', getter.stem, show=0).accepted()
        for opt in ['O2', 'O1']:
            flags = '-g0 -' + opt + ' -mips2 -G 0 -non_shared'
            obj = folder / (opt + '.o')
            score.compile_single(source, flags, obj)
            result = score.compare(obj, NAME, show=0)
            data, sections = score._elf(obj)
            symbols = [s for i, sec in enumerate(sections) if sec['type'] == 2
                       for s in score._symbol_table(data, sections, i) if s['name'] == NAME]
            assert len(symbols) == 1 and symbols[0]['type'] == 2
            symbol = symbols[0]
            assert symbol['value'] == 0 and symbol['section'] == score._text_index(sections)
            words = score.text_words(obj)
            resolved, masks, unresolved, unverified, errors = score.relocate(
                obj, words, 0, symbol['size'], score.image_symbols())
            assert not (unresolved or unverified or errors)
            assert not masks
            full = symbol['size'] == 592 and resolved[:148] == target
            assert not result.accepted() and not full
            expected = (125, 148, 0, 592) if opt == 'O2' else (148, 148, 55, 820)
            assert (result.differing, result.total, result.extra_words, symbol['size']) == expected
            # All symbol-table references to the address token must be undefined:
            # source supplies no local object or initializer pretending to be the table.
            token = [s for i, sec in enumerate(sections) if sec['type'] == 2
                     for s in score._symbol_table(data, sections, i) if s['name'] == 'D_8002D476']
            assert len(token) == 1 and token[0]['section'] == 0
            assert score.address_named('D_8002D476') == 0x8002D476
            results.append(dict(function=NAME, source_sha256=sha(source), flags=flags,
                effective_flags=flags + ' -Wab,-r4300_mul', differing_words=result.differing,
                total_words=result.total, extra_nonzero_words=result.extra_words,
                elf_function_bytes=symbol['size'], text_section_bytes=len(words) * 4,
                unresolved=result.unresolved, unverified=result.unverified, errors=result.errors,
                full_relocated_equal_and_exact_function_size=full, strict_match=False,
                address_token='0x8002D476', address_token_locally_defined=False))
        cmd = ['gcc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Wno-sign-compare',
               '-Wno-unknown-pragmas', '-Wno-pointer-to-int-cast', '-Wno-int-to-pointer-cast',
               '-fno-pie', '-no-pie', '-mcmodel=large', '-fsanitize=undefined',
               '-fno-sanitize-recover=all', '-g', '-O2', '-Wl,--defsym,D_8002D476=0x8002D476',
               str(HERE / 'test_conditional.c'), '-o', str(folder / 'test')]
        subprocess.run(cmd, check=True)
        tests = subprocess.check_output([str(folder / 'test')], text=True).strip()
        assert tests == 'PASS: 98816 validated actual-source calls; 8 unsafe fixtures rejected before execution'
    output = dict(classification='CONDITIONAL-COMPLETE-NONMATCH', target_bytes=592,
        target_body_sha256=target_hash, manifest_checks=manifest, extent_population=439,
        extent_bytes=99120, binary_results=results, tests=tests,
        sanitizer='UBSan; no ASan claim (fixed native address conflicts with shadow mapping)',
        native_table_object_proven=False, native_input_domain_proven=False,
        iso_c_portable=False, universal_runtime_safety=False)
    serialized = json.dumps(output, indent=2) + '\n'
    if args.output:
        args.output.write_text(serialized)
    print(serialized, end='')


if __name__ == '__main__':
    main()
