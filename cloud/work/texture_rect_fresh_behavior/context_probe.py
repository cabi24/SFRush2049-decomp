#!/usr/bin/env python3
"""Test the actual SDK context against an unchanged frozen function body.

This is a source-backed context test, not a token/whitespace fitness sweep.
Generated source and binary artifacts remain in ignored build/.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import re
import shutil
import sys

ROOT = Path(__file__).resolve().parents[3]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from verify_behavior import sdk_context, HEADERS, extra_cases


def digest(content):
    return hashlib.sha256(content).hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--baseline', type=Path, required=True)
    p.add_argument('--verifier-dir', type=Path, required=True)
    p.add_argument('--build', type=Path, default=ROOT / 'build/87110_fresh_behavior')
    a = p.parse_args()
    build = a.build.resolve()
    sdk_context(build)
    original = a.baseline.read_text()
    start = original.index('void func_80087110(')
    body = original[start:]
    prefix = original[:start]
    externs = '\n'.join(line for line in prefix.splitlines() if line.startswith('extern ')) + '\n'
    sdk = (build / 'libreultra_gbi.h').read_text()
    mbi = (build / 'mbi.h').read_text()
    def macro(content, name):
        m = re.search(r'^#\s*define\s+' + name + r'\(', content, re.M)
        assert m, name
        lines = content[m.start():].splitlines(True)
        out = []
        for line in lines:
            out.append(line)
            if not line.rstrip().endswith('\\'):
                break
        return ''.join(out)
    # Macro parser deliberately does not change macro spelling or structure.
    macro_text = (macro(mbi, '_SHIFTL') + '\n#define G_TEXRECT 0xe4\n'
        '#define G_RDPHALF_1 0xe1\n#define G_RDPHALF_2 0xf1\n'
        + macro(sdk, 'gImmp1') + '\n' + macro(sdk, 'gSPTextureRectangle') + '\n')
    types = '\n'.join(line for line in prefix.splitlines() if line.startswith('typedef ')) + '\n'
    full_context = '#include "sdk_context.h"\n' + externs
    variants = {'frozen_baseline': original,
        'authentic_macros_minimal_Gfx': types + externs + macro_text + body,
        'full_SDK_Gfx_and_macros': full_context + body}
    spec = importlib.util.spec_from_file_location('rect_verifier', a.verifier_dir / 'replay.py')
    replay = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(replay)
    flags = '-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
    results = {'body_sha256': digest(body.encode()), 'baseline_sha256': digest(original.encode()),
        'flags': flags, 'SDK_git_blob_ids': HEADERS, 'variants': {}}
    for name, text in variants.items():
        directory = build / name
        directory.mkdir(exist_ok=True)
        for header in ['sdk_context.h', 'libreultra_gbi.h']:
            shutil.copyfile(build / header, directory / header)
        source = directory / 'candidate.c'
        source.write_text(text)
        assert source.read_text().endswith(body), 'function body changed'
        obj = directory / 'candidate.o'
        score.compile_single(source, flags, obj)
        proof, native, linked, symbols = replay.inspect(obj, directory)
        semantics = replay.semantics(source, directory, native, linked, symbols)
        for category, arguments, state, unused in extra_cases():
            expected = replay.oracle(arguments, state)
            words, advance, events, offsets = replay.machine.execute(linked, symbols[replay.NAME], symbols, arguments, state)
            assert words == expected and advance == 4 * len(expected)
        results['variants'][name] = {'source_sha256': digest(text.encode()),
            'object_sha256': digest(obj.read_bytes()),
            'elf_function_bytes': proof['elf_function_bytes'],
            'differing_words': proof['project_scorer_differing_words'],
            'residual_offsets': proof['residual_offsets'],
            'linked_body_sha256': proof['linked_body_sha256'],
            'full_link_proof': proof, 'semantics': semantics,
            'new_native_cases': len(list(extra_cases()))}
    (build / 'context_probe.json').write_text(json.dumps(results, indent=2) + '\n')
    print(json.dumps({k:{a:b for a,b in v.items() if a not in ['full_link_proof','semantics']} for k,v in results['variants'].items()},indent=2))


if __name__ == '__main__':
    main()
