#!/usr/bin/env python3
"""Replay directed DC08 controls and prove the retained nonmatch's exact extent."""
import hashlib
import json
from pathlib import Path
import re
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
OLD = ROOT / 'cloud/work/boot_tail/BT03-high-spatial-control'
sys.path.insert(0, str(ROOT))
from tools.cloud import score

NAME = 'func_8001DC08'
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
SOURCE = WORK / 'nonmatch/func_8001DC08.c'


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def row_for(source, flags, obj):
    before = source.read_bytes()
    score.compile_single(source, flags, obj)
    result = score.compare(obj, NAME, show=0)
    elf, sections = score._elf(obj)
    symbols = [s for i, sec in enumerate(sections) if sec['type'] == 2
               for s in score._symbol_table(elf, sections, i)
               if s['type'] == 2 and s['section'] == score._text_index(sections)]
    assert len(symbols) == 1 and symbols[0]['name'] == NAME and symbols[0]['value'] == 0
    assert source.read_bytes() == before
    return dict(name=NAME, source_path=str(source.relative_to(ROOT)),
                source_sha256=digest(source), flags=flags,
                differing_words=result.differing, total_words=result.total,
                extra_words=result.extra_words, unresolved=result.unresolved,
                unverified=result.unverified, errors=result.errors,
                strict_match=result.accepted(), elf_function_size=symbols[0]['size'])


def threshold_loads(words, address):
    regs = [None] * 32
    hits = []
    for i, word in enumerate(words):
        op, rs, rt = word >> 26, (word >> 21) & 31, (word >> 16) & 31
        imm = word & 65535
        if imm >= 32768:
            imm -= 65536
        if op == 15:
            regs[rt] = (word & 65535) << 16
        elif op == 49 and regs[rs] is not None and (regs[rs] + imm) & 0xffffffff == address:
            hits.append(i * 4)
    return hits


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    target = score.targets()[NAME]
    assert len(target) == 118
    prior = json.loads((OLD / 'preflight.json').read_text())
    for relative, expected in prior['source_hashes'].items():
        assert digest(ROOT / relative) == expected, relative
    for name, expected in prior['compiler_files_sha256'].items():
        assert digest(score.IDO / name) == expected, name
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())
    extents = json.loads((score.ASM_DIR / 'extents.json').read_text())
    expected = {r['address']: r['size'] for r in inventory['functions']}
    actual = {r['address']: r['size'] for r in extents['functions']}
    assert expected == actual and len(expected) == 439 and sum(expected.values()) == 99120
    baseline = OLD / 'nonmatch/func_8001DC08.c'
    assert re.sub(r'\s+', '', baseline.read_text()) == re.sub(r'\s+', '', SOURCE.read_text())
    controls = json.loads((WORK / 'controls.json').read_text())
    results = []
    with tempfile.TemporaryDirectory(prefix='dc08-retry-verify-') as temp:
        obj = Path(temp) / 'candidate.o'
        baseline_row = row_for(baseline, FLAGS, obj)
        assert baseline_row['differing_words'] == 3 and baseline_row['elf_function_size'] == 472
        for row in controls:
            got = row_for(ROOT / row['source_path'], row['flags'], obj)
            assert row == got, row['source_path']
        for flags in [FLAGS, FLAGS.replace('-O2', '-O1')]:
            results.append(row_for(SOURCE, flags, obj))
        final = row_for(SOURCE, FLAGS, obj)
        assert final['differing_words'] == 2 and final['elf_function_size'] == 472
        assert final['extra_words'] == 0 and not final['strict_match']
        words = score.text_words(obj)
        relocated, masks, unresolved, unverified, errors = score.relocate(
            obj, words, 0, len(words) * 4, score.image_symbols())
        assert not (masks or unresolved or unverified or errors)
        differences = [i * 4 for i, (a, b) in enumerate(zip(target, relocated)) if a != b]
        assert differences == [0x48, 0x64] and not any(relocated[118:])
        thresholds = []
        for symbol, address in [('D_8002D904', 0x8002D904), ('D_8002D908', 0x8002D908)]:
            native = threshold_loads(target, address)
            candidate = threshold_loads(relocated, address)
            assert len(native) == len(candidate) == 1
            thresholds.append(dict(symbol=symbol, address=hex(address),
                                   native_load_offsets=native, candidate_load_offsets=candidate,
                                   contents_known=False, loads_per_nonempty_invocation=1))
        terms = ['sizeof(void*)==4', 'sizeof(float)==4', 'sizeof(u32)==4', 'sizeof(u16)==2',
                 'sizeof(Emitter)==68', 'sizeof(Vector)==12', 'sizeof(SpatialGroup)==12',
                 'sizeof(SpatialEntry)==28']
        offsets = {'Emitter': {'flags':8, 'handle':52, 'identifier':60, 'counter':62, 'fade':64},
                   'SpatialGroup': {'key':0, 'pending':4, 'active':8},
                   'SpatialEntry': {'next':0, 'volume':4, 'pan':8, 'span':12, 'send':16,
                                    'pitch':20, 'emitter':24}}
        terms += ['((unsigned int)&(('+t+'*)0)->'+f+')=='+str(n)
                  for t, fields in offsets.items() for f, n in fields.items()]
        probe = Path(temp) / 'native_abi.c'
        probe.write_text('#include "'+str(SOURCE)+'"\ntypedef char native_layout[('+
                         ' && '.join(terms)+')?1:-1];\n')
        score.compile_single(probe, FLAGS, Path(temp) / 'native_abi.o')
    return dict(result='PASS', status='COMPLETE-NONMATCH', matching_credit=0,
                base_commit='68a48a5e2bb3f88625d3af20cfd5a8e899a43944',
                target_words=118, native_bytes=472, control_rows_replayed=len(controls),
                frozen_source_and_compiler_hashes='PASS', target_manifest='PASS',
                census_functions=439, census_bytes=99120,
                token_identical_to_published_source=True, baseline=baseline_row,
                results=results, differing_byte_offsets=differences,
                masks=0, unresolved=0, unverified=0, relocation_errors=0,
                nonzero_excess=0, abi_layout='PASS', native_offsets=offsets,
                external_thresholds=thresholds)


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
