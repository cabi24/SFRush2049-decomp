#!/usr/bin/env python3
"""Freshly replay only the recovered retained source; old controls are unavailable."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score
NAME = 'func_80017D38'
SOURCE_SHA256 = 'ad5137579da173b15c903c8aa35c69cd26416d51def3b65267045062543bff65'
TARGET_SHA256 = '4fbd944f2a61b52f46402c29fbcf4dd910a4619268cb7c7ac2c6b2cebaa86c38'
BASE = '496a72b0edd683bd7d46e8c21f8323ab43629494'
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
EXPECTED = [(173, 218, 2, 880), (217, 218, 96, 1276)]


def word_hash(words):
    return hashlib.sha256(struct.pack('>' + str(len(words)) + 'I', *words)).hexdigest()


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    source = WORK / 'nonmatch' / (NAME + '.c')
    data = source.read_bytes()
    assert hashlib.sha256(data).hexdigest() == SOURCE_SHA256
    assert data.splitlines()[0] == ('/* flags: ' + FLAGS + ' */').encode()
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
    row = next(row for row in inventory if row['name'] == NAME)
    target = score.targets()[NAME]
    assert row['scope'] == 'in_scope' and row['size'] == 872 and len(target) == 218
    assert word_hash(target) == TARGET_SHA256
    results = []
    with tempfile.TemporaryDirectory(prefix='sequence-events-replay-') as temp:
        for flags, expected in zip([FLAGS, FLAGS.replace('-O2', '-O1')], EXPECTED):
            obj = Path(temp) / 'candidate.o'
            score.compile_single(source, flags, obj)
            result = score.compare(obj, NAME, show=0)
            elf, sections = score._elf(obj)
            symbols = [sym for idx, sec in enumerate(sections) if sec['type'] == 2
                       for sym in score._symbol_table(elf, sections, idx)
                       if sym['type'] == 2 and sym['section'] == score._text_index(sections)]
            assert len(symbols) == 1 and symbols[0]['name'] == NAME and symbols[0]['value'] == 0
            size = symbols[0]['size']
            words = score.text_words(obj)
            relocated, masks, unresolved, unverified, errors = score.relocate(
                obj, words, 0, len(words) * 4, score.image_symbols())
            assert not (masks or unresolved or unverified or errors)
            assert not (result.unresolved or result.unverified or result.errors)
            assert (result.differing, result.total, result.extra_words, size) == expected
            assert not result.accepted()
            assert source.read_bytes() == data
            results.append(dict(name=NAME, source_path=str(source.relative_to(ROOT)),
                                source_sha256=SOURCE_SHA256, flags=flags,
                                effective_flags=flags + ' -Wab,-r4300_mul',
                                differing_words=result.differing, total_words=result.total,
                                extra_words=result.extra_words, elf_function_size=size,
                                text_section_bytes=len(words) * 4,
                                fully_relocated_text_sha256=word_hash(relocated),
                                masked_relocations=len(masks), unresolved=unresolved,
                                unverified=unverified, errors=errors, strict_match=False))
    return dict(schema_version=2, result='PASS', classification='COMPLETE-NONMATCH',
                base_commit=BASE, recovered_source_sha256=SOURCE_SHA256,
                target_sha256=TARGET_SHA256, target_bytes=872,
                match_functions=0, verified_body_bytes=0, fresh_retained_flag_rows=2,
                fresh_archived_control_rows=0,
                historical_controls='Eight pre-reset rows are historical evidence only; missing sources and receipts are not recreated or checked here.',
                target_manifest_sha256=hashlib.sha256((score.ASM_DIR / 'SHA256SUMS').read_bytes()).hexdigest(),
                results=results)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    result = run()
    if args.check:
        assert result == json.loads((WORK / 'verification.json').read_text()), 'verification receipt drift'
        print('PASS: both recovered retained-source rows and complete text relocation reproduced; zero matching credit.')
    else:
        print(json.dumps(result, indent=2))
