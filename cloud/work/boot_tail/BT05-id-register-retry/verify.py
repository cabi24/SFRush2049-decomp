#!/usr/bin/env python3
"""Strict fresh replay of one evidence-backed negative ID-composition probe."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score

NAME = 'func_80022324'
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
BASE = 'f02638ed8388bd3e5256e0c31b2ae36b6be9524a'
BASELINE = ROOT / 'cloud/work/boot_tail/BT05-macro-five/nonmatch/func_80022324.c'
BASELINE_SHA = '49fbe784f384d7273c953911401a99b281f39d2d9e6a134ad882ece76bc287b7'


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
    extents = json.loads((score.ASM_DIR / 'extents.json').read_text())['functions']
    assert {r['address']: r['size'] for r in inventory} == {r['address']: r['size'] for r in extents}
    assert len(extents) == 439 and sum(r['size'] for r in extents) == 99120
    row = next(r for r in inventory if r['name'] == NAME)
    assert row['scope'] == 'in_scope' and row['size'] == 172
    target = score.targets()[NAME]
    assert len(target) * 4 == 172 and digest(BASELINE) == BASELINE_SHA
    results = []
    with tempfile.TemporaryDirectory(prefix='bt05-id-register-') as directory:
        for source in [BASELINE, WORK / 'candidates' / (NAME + '.c')]:
            data = source.read_bytes()
            assert data.splitlines()[0] == ('/* flags: ' + FLAGS + ' */').encode()
            for flags in [FLAGS, FLAGS.replace('-O2', '-O1')]:
                obj = Path(directory) / 'candidate.o'
                score.compile_single(source, flags, obj)
                result = score.compare(obj, NAME, show=0)
                elf, sections = score._elf(obj)
                symbols = [sym for idx, sec in enumerate(sections) if sec['type'] == 2
                           for sym in score._symbol_table(elf, sections, idx)
                           if sym['type'] == 2 and sym['section'] == score._text_index(sections)]
                assert len(symbols) == 1 and symbols[0]['name'] == NAME and symbols[0]['value'] == 0
                words = score.text_words(obj)
                relocated, masks, unresolved, unverified, errors = score.relocate(
                    obj, words, 0, len(words) * 4, score.image_symbols())
                assert not (masks or unresolved or unverified or errors)
                exact = (symbols[0]['size'] == 172 and relocated[:len(target)] == target
                         and not any(relocated[len(target):]))
                assert not result.accepted() and not exact
                assert source.read_bytes() == data
                results.append({'name': NAME, 'native_bytes': 172,
                    'source_path': str(source.relative_to(ROOT)), 'source_sha256': digest(source),
                    'flags': flags, 'effective_flags': flags + ' -Wab,-r4300_mul',
                    'differing_words': result.differing, 'total_words': result.total,
                    'extra_words': result.extra_words, 'elf_function_size': symbols[0]['size'],
                    'unresolved': unresolved, 'unverified': unverified, 'errors': errors,
                    'masks': masks, 'strict_match': result.accepted(),
                    'full_relocated_equality_with_exact_extent': exact})
    assert [(r['differing_words'], r['extra_words'], r['elf_function_size']) for r in results] == [
        (2, 0, 172), (43, 10, 220), (7, 0, 172), (43, 10, 220)]
    return {'schema_version': 1, 'result': 'PASS_BOUNDED_NEGATIVE_RESEARCH',
            'base_commit': BASE, 'reopened_functions': 1, 'native_bytes': 172,
            'new_attempted_targets': 0, 'matching_functions': 0, 'verified_body_bytes': 0,
            'candidate_forms': 1, 'flags_per_form': 2,
            'target_manifest_sha256': digest(score.ASM_DIR / 'SHA256SUMS'),
            'score_sha256': digest(ROOT / 'tools/cloud/score.py'),
            'compiler_sha256': {name: digest(score.IDO / name) for name in ['cc', 'cfe', 'uopt', 'ugen', 'as1']},
            'frozen_baseline_sha256': BASELINE_SHA, 'results': results}


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
