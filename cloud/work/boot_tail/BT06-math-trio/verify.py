#!/usr/bin/env python3
"""Replay pinned inputs, strict relocated equality, exact function sizes and layouts."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
SIZES = {'80024BF0': 172, '80024C9C': 104, '80024D04': 112}


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', type=Path, default=HERE.parents[3])
    parser.add_argument('--ido-dir', type=Path, default=Path(os.environ.get('IDO_DIR', 'tools/cloud/ido')))
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
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
    results, layouts = [], []
    with tempfile.TemporaryDirectory(prefix='bt06-math-verify-') as folder:
        folder = Path(folder)
        getter = root / 'cloud/matches/boot_tail/func_80010A00.c'
        obj = folder / 'getter.o'
        score.compile_single(getter, score.DEFAULT_FLAGS, obj)
        assert score.compare(obj, getter.stem, show=0).accepted()
        for address, size in SIZES.items():
            name = 'func_' + address
            source = root / 'cloud/matches/boot_tail' / (name + '.c')
            for opt in ['O2', 'O1']:
                flags = '-g0 -' + opt + ' -mips2 -G 0 -non_shared'
                obj = folder / (address + opt + '.o')
                score.compile_single(source, flags, obj)
                result = score.compare(obj, name, show=0)
                data, sections = score._elf(obj)
                symbols = [s for i, sec in enumerate(sections) if sec['type'] == 2
                           for s in score._symbol_table(data, sections, i) if s['name'] == name]
                assert len(symbols) == 1 and symbols[0]['type'] == 2
                symbol = symbols[0]
                assert symbol['value'] == 0 and symbol['section'] == score._text_index(sections)
                words = score.text_words(obj)
                resolved, masks, unresolved, unverified, errors = score.relocate(
                    obj, words, 0, symbol['size'], score.image_symbols())
                assert not (unresolved or unverified or errors)
                full = symbol['size'] == size and resolved[:size // 4] == score.targets()[name]
                full = full and all(mask == 0xFFFFFFFF for mask in masks.values())
                expected_match = not (address == '80024C9C' and opt == 'O1')
                assert result.accepted() == full == expected_match
                expected = (26, 26, 8, 140) if not expected_match else (0, size // 4, 0, size)
                assert (result.differing, result.total, result.extra_words, symbol['size']) == expected
                results.append(dict(function=name, source_path=str(source.relative_to(root)), source_sha256=sha(source),
                    target_bytes=size, flags=flags, effective_flags=flags + ' -Wab,-r4300_mul',
                    differing_words=result.differing, total_words=result.total, extra_nonzero_words=result.extra_words,
                    unresolved=result.unresolved, unverified=result.unverified, errors=result.errors,
                    elf_function_bytes=symbol['size'], text_section_bytes=len(words) * 4,
                    full_relocated_equal_and_exact_function_size=full, strict_match=result.accepted()))
            checks = [('sizeof(Vector)', 12), ('(unsigned int)&((Vector *)0)->x', 0),
                      ('(unsigned int)&((Vector *)0)->y', 4), ('(unsigned int)&((Vector *)0)->z', 8)]
            if address == '80024BF0':
                checks += [('sizeof(Matrix)', 48), ('(unsigned int)&((Matrix *)0)->m', 0),
                           ('(unsigned int)&((Matrix *)0)->t', 36)]
            layout = source.read_text() + '\n'
            for i, (expression, value) in enumerate(checks):
                layout += 'typedef char native_layout_%d[(%s == %d) ? 1 : -1];\n' % (i, expression, value)
            layout_path = folder / (address + '-layout.c')
            layout_path.write_text(layout)
            score.compile_single(layout_path, score.DEFAULT_FLAGS, folder / (address + '-layout.o'))
            layouts.append(dict(function=name, result='PASS', checks=[dict(expression=e, value=v) for e, v in checks]))
    report = dict(schema_version=1, result='PASS', manifest=manifest, extent_functions=439, extent_bytes=99120,
                  pinned_compiler_files=len(compiler), getter_strict_match=True,
                  local_match_functions=3, local_match_bytes=388, results=results, native_layout=layouts)
    rendered = json.dumps(report, indent=2) + '\n'
    if args.output:
        args.output.write_text(rendered)
    else:
        print(rendered, end='')


if __name__ == '__main__':
    main()
