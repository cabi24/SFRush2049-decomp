#!/usr/bin/env python3
"""Replay two complete nonmatches and their bounded controls, never accepting them."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
import tempfile

P = Path(__file__).resolve().parent
parser = argparse.ArgumentParser()
parser.add_argument('--root', type=Path, default=None)
a = parser.parse_args()
ROOT = (a.root if a.root is not None else P.parents[3]).resolve()
sys.path.insert(0, str(ROOT))
from tools.cloud import score
score.ASM_DIR = ROOT / 'asm/us/boot_tail'
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'


def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def record(result):
    return {'strict_match': result.accepted(), 'differing_words': result.differing,
            'total_words': result.total, 'extra_words': result.extra_words,
            'unresolved': result.unresolved, 'unverified': result.unverified,
            'errors': result.errors}

pins = json.loads((P / 'input_pins.json').read_text())
for path, expected in pins['source_hashes'].items():
    assert sha(ROOT / path) == expected, path
assert {p.name: sha(p) for p in sorted(score.IDO.iterdir()) if p.is_file()} == pins['compiler_files_sha256']
manifest = subprocess.check_output(['sha256sum', '-c', 'SHA256SUMS'], cwd=score.ASM_DIR, text=True)
inv = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
ext = json.loads((score.ASM_DIR / 'extents.json').read_text())['functions']
assert {r['address']: r['size'] for r in inv} == {r['address']: r['size'] for r in ext}
assert len(inv) == len(ext) == 439 and sum(r['size'] for r in inv) == 99120
expected = {'80011A64': [(107, 110, 1), (104, 110, 5)],
            '80011D74': [(24, 123, 0), (122, 123, 14)]}
rows, controls = [], []
with tempfile.TemporaryDirectory(prefix='bt02-next-verify-') as temp:
    temp = Path(temp)
    for name, residuals in expected.items():
        source = P / 'nonmatch' / ('func_' + name + '.c')
        assert source.read_text().splitlines()[0] == '/* flags: ' + FLAGS + ' */'
        row = {'name': 'func_' + name, 'source': str(source.relative_to(P)), 'source_sha256': sha(source), 'scores': []}
        for i, opt in enumerate(['-O2', '-O1']):
            flags = FLAGS.replace('-O2', opt)
            obj = temp / 'candidate.o'
            score.compile_single(source, flags, obj)
            result = score.compare(obj, 'func_' + name, show=0)
            assert not result.accepted()
            assert (result.differing, result.total, result.extra_words) == residuals[i]
            assert not (result.unresolved or result.unverified or result.errors)
            row['scores'].append({'flags': flags, **record(result)})
        rows.append(row)
    for name in expected:
        baseline = ((P / 'controls' / 'func_80011D74_baseline.c') if name == '80011D74'
                    else P / 'nonmatch' / 'func_80011A64.c')
        source = baseline.read_text()
        variants = [('baseline', source), ('register_parameter', source.replace('unsigned short samples', 'register unsigned short samples'))]
        if name == '80011A64':
            kr = source.replace('void func_80011A64(AudioState *audio, unsigned short samples, unsigned char *active)',
                'void func_80011A64(audio, samples, active)\nAudioState *audio;\nunsigned short samples;\nunsigned char *active;')
            variants += [('K_and_R_definition', kr), ('split_increment', source.replace('increment = ((samples * 32000U) / D_8003828C) << 11;',
                         'increment = (samples * 32000U) / D_8003828C;\n    increment <<= 11;')),
                         ('signed_multiplier', source.replace('32000U', '32000'))]
            residual = (107, 110, 1)
        else:
            kr = source.replace('void func_80011D74(AudioState *audio, unsigned short samples)',
                'void func_80011D74(audio, samples)\nAudioState *audio;\nunsigned short samples;')
            variants += [('K_and_R_definition', kr), ('multiply_divide', source.replace('(audio->rate * samples) * 0.000244140625',
                                                                                      '(audio->rate * samples) / 4096.0'))]
            residual = (97, 123, 0)
        for label, text in variants:
            path, obj = temp / 'control.c', temp / 'control.o'
            path.write_text(text)
            score.compile_single(path, FLAGS, obj)
            result = score.compare(obj, 'func_' + name, show=0)
            assert (result.differing, result.total, result.extra_words) == residual
            assert not (result.accepted() or result.unresolved or result.unverified or result.errors)
            controls.append({'name': 'func_' + name, 'variant': label, 'source_sha256': sha(path), **record(result)})
    getter = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
    score.compile_single(getter, FLAGS, temp / 'getter.o')
    assert score.compare(temp / 'getter.o', getter.stem, show=0).accepted()
print(json.dumps({'result': 'PASS', 'meaning': 'Expected complete-nonmatch residuals and pinned-input preflight reproduced.',
    'strict_match_functions': 0, 'strict_match_bytes': 0, 'complete_nonmatch_functions': 2,
    'complete_nonmatch_bytes': 932, 'manifest': manifest.splitlines(), 'census_functions': 439,
    'census_bytes': 99120, 'existing_getter_strict_match': True, 'sources': rows, 'controls': controls,
    'harness_hashes': {p.name: sha(p) for p in [P / 'verify.py', P / 'test_host.py', P / 'host_behavior.c', P / 'host_types.h']}}, indent=2))
