#!/usr/bin/env python3
"""Verify pinned inputs, honest NONMATCH geometry, relocations and native ABI."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
NAME = 'func_8001CCDC'


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
    assert len(target) == 234
    calls = [(4 * i, 'func_%08X' % (0x80000000 | ((word & 0x3FFFFFF) << 2)))
             for i, word in enumerate(target) if word >> 26 == 3]
    assert [n for _, n in calls] == [
        'func_8001CC9C', 'func_8001B7C0', 'func_8001CC9C', 'func_8001B7C0',
        'func_8001CC9C', 'func_8001B29C', 'func_8001CC9C', 'func_8001B3A0',
        'func_8001CCC0', 'func_8001B4A4']
    indirect = [i * 4 for i, word in enumerate(target)
                if word >> 26 == 0 and word & 63 in (8, 9)]
    assert indirect == [928] and (target[232] >> 21 & 31) == 31
    incoming = []
    for name, words in score.targets().items():
        for i, word in enumerate(words):
            if word >> 26 == 3 and 0x80000000 | ((word & 0x3FFFFFF) << 2) == 0x8001CCDC:
                incoming.append((name, i * 4))
    assert incoming == [('func_8001D1F4', 408), ('func_8001DC08', 340), ('func_8001DDE0', 548)]
    # Target parameter homes and source-proven input slots: a0,a1,a2,a3,sp+16,sp+20.
    homes = [(word >> 16 & 31, word & 65535) for word in target[:6]
             if word >> 26 == 43 and word >> 21 & 31 == 29]
    assert homes == [(31, 28), (16, 24), (4, 32), (6, 40), (7, 44)]
    source = HERE / (NAME + '_NONMATCH.c')
    results = []
    checks = [('sizeof(Emitter)', 68), ('(unsigned int)&((Emitter *)0)->flags08', 8),
              ('(unsigned int)&((Emitter *)0)->identifier34', 52),
              ('(unsigned int)&((Emitter *)0)->fade40', 64)]
    with tempfile.TemporaryDirectory(prefix='bt03-chain-verify-') as folder:
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
            assert all(mask == 0xFFFFFFFF for mask in masks.values())
            full = symbol['size'] == 936 and resolved[:234] == target
            assert not result.accepted() and not full
            expected = (181, 234, 3, 952) if opt == 'O2' else (225, 234, 14, 1004)
            assert (result.differing, result.total, result.extra_words, symbol['size']) == expected
            results.append(dict(function=NAME, source_sha256=sha(source), flags=flags,
                effective_flags=flags + ' -Wab,-r4300_mul', differing_words=result.differing,
                total_words=result.total, extra_nonzero_words=result.extra_words,
                elf_function_bytes=symbol['size'], text_section_bytes=len(words) * 4,
                unresolved=result.unresolved, unverified=result.unverified, errors=result.errors,
                full_relocated_equal_and_exact_function_size=full, strict_match=False))
        layout = source.read_text() + '\n'
        for i, (expression, value) in enumerate(checks):
            layout += 'typedef char native_layout_%d[(%s == %d) ? 1 : -1];\n' % (i, expression, value)
        layout_path = folder / 'layout.c'
        layout_path.write_text(layout)
        score.compile_single(layout_path, score.DEFAULT_FLAGS, folder / 'layout.o')
    report = dict(schema_version=1, result='PASS', candidate_status='COMPLETE-NONMATCH',
                  match_credit_functions=0, match_credit_bytes=0, manifest=manifest,
                  extent_functions=439, extent_bytes=99120, pinned_compiler_files=len(compiler),
                  getter_strict_match=True, results=results,
                  native_layout=[dict(expression=e, value=v) for e, v in checks],
                  native_call_sites=[dict(offset=hex(o), callee=n) for o, n in calls],
                  callers=[dict(function=n, offset=hex(o)) for n, o in incoming],
                  indirect_control_flow='Only the final jr ra; no callbacks or tables.',
                  abi='void(Emitter *, float volume, float xPan, float yPan, float zPan, float doppler); yPan is genuine but unused.')
    rendered = json.dumps(report, indent=2) + '\n'
    if args.output:
        args.output.write_text(rendered)
    else:
        print(rendered, end='')


if __name__ == '__main__':
    main()
