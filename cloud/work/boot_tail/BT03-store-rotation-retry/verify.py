"""Reproduce the frozen initializer and its single stock-IDO scheduling repair."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

NAME = 'func_8001C1D8'
BASE = 'cloud/work/boot_tail/BT03-high-runtime-lists/nonmatch/' + NAME + '.c'
FINAL = 'cloud/matches/boot_tail/' + NAME + '.c'
BASE_HASH = 'e78eaf80607f870959b674da6718e94de3f03d8f67250ed937b2b028092bc9dd'
FINAL_HASH = '82d2de520663eeaedaec17624b865cc7ea3e2a4da20e7ca3b28220f8016774a5'
OLD_LOOP = '    for (i=0;i<16;i++) D_8004BE98[i] = 0;'
NEW_LOOP = '    for (i=0;i<16;i++) {\n        D_8004BE98[i] = 0;\n    }'


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    targets = score.targets()
    assert len(targets) == 439 and sum(map(len, targets.values())) * 4 == 99120
    assert len(targets[NAME]) * 4 == 432
    baseline = (ROOT / BASE).read_bytes()
    final = (ROOT / FINAL).read_bytes()
    assert hashlib.sha256(baseline).hexdigest() == BASE_HASH
    assert hashlib.sha256(final).hexdigest() == FINAL_HASH
    assert baseline.decode().count(OLD_LOOP) == 1
    assert final.decode() == baseline.decode().replace(OLD_LOOP, NEW_LOOP)
    pins = json.loads(Path(__file__).with_name('input_pins.json').read_text())
    compiler = {p.name: hashlib.sha256(p.read_bytes()).hexdigest()
                for p in sorted(score.IDO.iterdir()) if p.is_file()}
    assert compiler == pins['compiler_files_sha256']
    for path, value in pins['files_sha256'].items():
        assert hashlib.sha256((ROOT / path).read_bytes()).hexdigest() == value, path
    rows = []
    with tempfile.TemporaryDirectory(prefix='store-rotation-') as temp:
        for path, expected in ((BASE, 4), (FINAL, 0)):
            for level in (2, 1):
                flags = '-g0 -O%d -mips2 -G 0 -non_shared' % level
                obj = Path(temp) / 'candidate.o'
                score.compile_single(ROOT / path, flags, obj)
                result = score.compare(obj, NAME, show=0)
                data, sections = score._elf(obj)
                symbols = [s for i, section in enumerate(sections)
                           if section['type'] == 2
                           for s in score._symbol_table(data, sections, i)
                           if s['name'] == NAME and s['type'] == 2]
                assert len(symbols) == 1
                words = score.text_words(obj)
                relocated, masks, unresolved, unverified, errors = score.relocate(
                    obj, words, 0, symbols[0]['size'], score.image_symbols())
                assert not masks and not unresolved and not unverified and not errors
                assert not any(words[symbols[0]['size'] // 4:])
                if level == 2:
                    assert result.differing == expected and not result.extra_words
                    assert not result.unresolved and not result.unverified and not result.errors
                    assert symbols[0]['size'] == 432
                    assert result.accepted() == (expected == 0)
                    assert (relocated[:len(targets[NAME])] == targets[NAME]) == (expected == 0)
                else:
                    assert not result.accepted()
                    assert result.differing == 107 and result.extra_words == 246
                rows.append(dict(source_path=path,
                    source_sha256=hashlib.sha256((ROOT / path).read_bytes()).hexdigest(),
                    flags=flags, effective_flags=flags + ' -Wab,-r4300_mul',
                    differing_words=result.differing, total_words=result.total,
                    extra_words=result.extra_words, unresolved=result.unresolved,
                    unverified=result.unverified, errors=result.errors,
                    strict_match=result.accepted(), elf_function_symbols=symbols,
                    full_function_relocation_errors=errors, relocation_mask_count=len(masks),
                    section_padding_is_zero=True,
                    full_native_window_equal=relocated[:len(targets[NAME])] == targets[NAME]))
        controls = json.loads(Path(__file__).with_name('line_provenance_controls.json').read_text())
        for control in controls['results']:
            src = Path(temp) / (control['label'] + '.c')
            src.write_text(baseline.decode().replace(OLD_LOOP, control['exact_replacement_loop']))
            assert hashlib.sha256(src.read_bytes()).hexdigest() == control['source_sha256']
            obj = Path(temp) / (control['label'] + '.o')
            score.compile_single(src, control['flags'], obj)
            result = score.compare(obj, NAME, show=0)
            assert result.differing == control['differing_words']
            assert result.accepted() == control['strict_match']
            assert not result.extra_words and not result.unresolved and not result.unverified and not result.errors
    return dict(result='PASS', base_commit='8cccffb88f99656d3948a71e15517be63fb2405a',
                target_manifest_sha256=hashlib.sha256((score.ASM_DIR / 'SHA256SUMS').read_bytes()).hexdigest(),
                natural_source_delta='Only the existing scalar for-loop gains an ordinary braced multiline body.',
                compiler_files_sha256=compiler, input_pins_verified=True,
                orthogonal_line_controls_reproduced=True,
                new_verified_functions=1, new_verified_bytes=432, results=rows)


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
