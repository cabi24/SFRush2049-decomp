#!/usr/bin/env python3
"""Reproduce the initial controls and nine bounded, ABI-preserving variants."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import sys
import tempfile

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parents[3]))
from tools.cloud import score
score.ASM_DIR = HERE.parents[3] / 'asm/us/boot_tail'
EXPRESSIONS = ['(em->fade40 * vol) * 127.0f', 'vol * 127.0f', '(1.0f + xPan) * 64.0f', '(1.0f - zPan) * 64.0f']
SENDERS = ['func_8001B7C0', 'func_8001B7C0', 'func_8001B29C', 'func_8001B3A0']


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--ido-dir', type=Path, default=Path(os.environ.get('IDO_DIR', 'tools/cloud/ido')))
    ap.add_argument('--output', type=Path)
    args = ap.parse_args()
    score.IDO = args.ido_dir.resolve()
    source = (HERE / 'func_8001CCDC_NONMATCH.c').read_text()
    initial = source
    for expr in EXPRESSIONS:
        initial = initial.replace('func_8001CC9C((u32)(' + expr + '))', 'func_8001CC9C(' + expr + ')')
    variants = [('initial_O2', initial, 'O2'), ('initial_O1', initial, 'O1')]
    for label, cast in [('explicit_u8', '(u8)'), ('word_then_u8', '(u8)(u32)'), ('explicit_word', '(u32)')]:
        body = initial
        for expr in EXPRESSIONS:
            body = body.replace('func_8001CC9C(' + expr + ')', 'func_8001CC9C(' + cast + '(' + expr + '))')
        variants.append((label, body, 'O2'))
    for label, typ in [('byte_local', 'u8'), ('word_local', 'u32')]:
        body = initial.replace('    u32 identifier;', '    u32 identifier;\n    ' + typ + ' value;')
        for expr, sender in zip(EXPRESSIONS, SENDERS):
            body = body.replace(sender + '(identifier, func_8001CC9C(' + expr + '));',
                                'value = ' + expr + ';\n        ' + sender + '(identifier, func_8001CC9C(value));')
        variants.append((label, body, 'O2'))
    for label, typ in [('clipped_byte', 'u8'), ('clipped_word', 'u32')]:
        body = initial.replace('    u32 identifier;', '    u32 identifier;\n    ' + typ + ' controller;')
        body = re.sub(r'( *)func_(8001B(?:7C0|29C|3A0))\(identifier, (func_8001CC9C\(.*\))\);',
                      r'\1controller = \3;\n\1func_\2(identifier, controller);', body)
        variants.append((label, body, 'O2'))
    for label, cast in [('word_mask', '((u32)(%s) & 255)'), ('ulong_cast', '(unsigned long)(%s)')]:
        body = initial
        for expr in EXPRESSIONS:
            body = body.replace('func_8001CC9C(' + expr + ')', 'func_8001CC9C(' + (cast % expr) + ')')
        variants.append((label, body, 'O2'))
    results = []
    with tempfile.TemporaryDirectory(prefix='bt03-chain-controls-') as folder:
        folder = Path(folder)
        for label, body, opt in variants:
            src, obj = folder / (label + '.c'), folder / (label + '.o')
            src.write_text(body)
            flags = score.DEFAULT_FLAGS.replace('O2', opt)
            score.compile_single(src, flags, obj)
            r = score.compare(obj, 'func_8001CCDC', show=0)
            data, secs = score._elf(obj)
            size = next(s['size'] for i, sec in enumerate(secs) if sec['type'] == 2
                        for s in score._symbol_table(data, secs, i) if s['name'] == 'func_8001CCDC')
            expected = (225, 234, 14, 1004) if opt == 'O1' else (155, 234, 2, 948) if label == 'word_local' else (181, 234, 3, 952)
            assert (r.differing, r.total, r.extra_words, size) == expected
            assert not r.accepted() and not (r.unresolved or r.unverified or r.errors)
            results.append(dict(label=label, source_sha256=hashlib.sha256(body.encode()).hexdigest(), flags=flags,
                                differing_words=r.differing, target_words=r.total, extra_nonzero_words=r.extra_words,
                                elf_function_bytes=size, strict_match=False))
    rendered = json.dumps(dict(result='PASS', initial_controls=2, directed_variants=9, results=results), indent=2) + '\n'
    if args.output:
        args.output.write_text(rendered)
    else:
        print(rendered, end='')


if __name__ == '__main__':
    main()
