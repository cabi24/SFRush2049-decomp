"""Strict full-extent replay of the two naturally formatted byte-clear loops."""
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score
NAME = 'func_80020820'
BASE = 'cloud/work/boot_tail/C13-controller-pair/nonmatch/' + NAME + '.c'
FINAL = 'cloud/matches/boot_tail/' + NAME + '.c'
BASE_HASH = '89123b46d8496a55a0a250a19c9bb23612154011203f141b243a6b3e4a4c6d05'
FINAL_HASH = '93577fdccd962174556eb10f0ac4d70ac85926831c661ebc7ccc234d1b0955ca'
OLD = [
    '        for (i = 0; i < 134; i++) D_80050D00[set][channel][i] = 0;',
    '        for (i = 0; i < 134; i++) D_80055000[channel][i] = 0;',
]
NEW = [
    '        for (i = 0; i < 134; i++) {\n            D_80050D00[set][channel][i] = 0;\n        }',
    '        for (i = 0; i < 134; i++) {\n            D_80055000[channel][i] = 0;\n        }',
]


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def source_line_stores(source, directory):
    directory.mkdir()
    subprocess.run([str(score.IDO / 'cc'), *score.DEFAULT_FLAGS.split(), '-Wab,-r4300_mul', '-S', str(source)], cwd=directory, check=True, capture_output=True)
    listing = (directory / (source.stem + '.s')).read_text()
    line_number, stores = None, []
    for line in listing.splitlines():
        match = re.fullmatch(r'\s*\.loc\s+\d+\s+(\d+)\s*', line)
        if match:
            line_number = int(match[1])
        match = re.fullmatch(r'\s*sb\s+\$0,\s*([0-3])\(\$3\)\s*', line)
        if match:
            stores.append({'unrolled_store_ordinal': int(match[1]), 'source_line': line_number})
    return stores


def compile_row(path, flags, directory):
    source = ROOT / path
    before = source.read_bytes()
    obj = directory / 'candidate.o'
    score.compile_single(source, flags, obj)
    result = score.compare(obj, NAME, show=0)
    data, sections = score._elf(obj)
    symbols = [symbol for i, section in enumerate(sections) if section['type'] == 2
               for symbol in score._symbol_table(data, sections, i)
               if symbol['name'] == NAME and symbol['type'] == 2]
    assert len(symbols) == 1
    size = symbols[0]['size']
    words = score.text_words(obj)
    relocated, masks, unresolved, unverified, errors = score.relocate(obj, words, 0, size, score.image_symbols())
    assert not masks and not unresolved and not unverified and not errors
    assert not any(words[size // 4:])
    assert source.read_bytes() == before
    return dict(source_path=path, source_sha256=digest(source), flags=flags,
                effective_flags=flags + ' -Wab,-r4300_mul', differing_words=result.differing,
                total_words=result.total, extra_words=result.extra_words,
                unresolved=result.unresolved, unverified=result.unverified, errors=result.errors,
                strict_match=result.accepted(), elf_function_size=size,
                full_function_relocation_errors=errors, relocation_mask_count=len(masks),
                section_padding_is_zero=True,
                full_native_window_equal=relocated[:121] == score.targets()[NAME])


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    targets = score.targets()
    assert len(targets) == 439 and sum(map(len, targets.values())) * 4 == 99120
    assert len(targets[NAME]) == 121
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())
    entry = next(row for row in inventory['functions'] if row['name'] == NAME)
    assert entry['scope'] == 'in_scope' and entry['size'] == 484
    pins = json.loads((WORK / 'input_pins.json').read_text())
    compiler = {p.name: digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()}
    assert compiler == pins['compiler_files_sha256']
    for path, expected in pins['files_sha256'].items():
        assert digest(ROOT / path) == expected, path
    assert digest(ROOT / BASE) == BASE_HASH
    assert digest(ROOT / FINAL) == FINAL_HASH
    baseline = (ROOT / BASE).read_text()
    expected = baseline
    for old, new in zip(OLD, NEW):
        assert expected.count(old) == 1
        expected = expected.replace(old, new)
    assert (ROOT / FINAL).read_text() == expected
    assert (WORK / 'controls/multiline_braces.c').read_text() == expected
    results = []
    with tempfile.TemporaryDirectory(prefix='byte-clear-verify-') as tmp:
        temp = Path(tmp)
        for path, differing in ((BASE, 10), (FINAL, 0)):
            for level in (2, 1):
                row = compile_row(path, score.DEFAULT_FLAGS.replace('-O2', '-O%d' % level), temp)
                expected_diff, expected_size = (differing, 484) if level == 2 else (120, 448)
                assert row['differing_words'] == expected_diff and row['elf_function_size'] == expected_size
                assert not row['extra_words'] and not row['unresolved'] and not row['unverified'] and not row['errors']
                assert row['strict_match'] == (expected_diff == 0)
                assert row['full_native_window_equal'] == (expected_diff == 0)
                results.append(row)
        for control in json.loads((WORK / 'line_provenance_controls.json').read_text())['results']:
            row = compile_row(control['source_path'], control['flags'], temp)
            for key in ('source_sha256', 'differing_words', 'total_words', 'extra_words', 'unresolved', 'unverified', 'errors', 'strict_match', 'elf_function_size'):
                assert row[key] == control[key], (control['label'], key)
            results.append(row)
        baseline_lines = source_line_stores(ROOT / BASE, temp / 'baseline')
        final_lines = source_line_stores(ROOT / FINAL, temp / 'final')
        assert [row['source_line'] for row in baseline_lines] == [13] * 4 + [15] * 4
        assert [row['source_line'] for row in final_lines] == [14, 13, 13, 13, 18, 17, 17, 17]
        for records in (baseline_lines, final_lines):
            assert [row['unrolled_store_ordinal'] for row in records] == [0, 1, 2, 3] * 2
    return dict(result='PASS', base_commit=pins['base_commit'], base_tree=pins['base_tree'],
                target_manifest_sha256=digest(score.ASM_DIR / 'SHA256SUMS'),
                new_verified_functions=1, new_verified_bytes=484,
                natural_delta='Only both existing scalar for-loops gain ordinary braced multiline bodies.',
                protected_and_prior_packet_pins_verified=True, compiler_pins_verified=True,
                stock_preassembler_line_ownership={'baseline': baseline_lines, 'final': final_lines},
                results=results)


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
