"""Reproduce bounded emitter controls with canonical full relocations and ELF sizes."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score

FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
BASE = ROOT / 'cloud/work/boot_tail/BT03-high-runtime-lists/nonmatch'
NAMES = {'func_8001D944': 304, 'func_8001DA74': 404}


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    pins = json.loads((ROOT / 'cloud/work/boot_tail/BT03-voice-allocation-retry/input_pins.json').read_text())
    compiler = {p.name: digest(p) for p in score.IDO.iterdir() if p.is_file()}
    assert compiler == pins['compiler_files_sha256']
    targets = score.targets()
    assert len(targets) == 439 and sum(map(len, targets.values())) * 4 == 99120
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())
    extents = json.loads((score.ASM_DIR / 'extents.json').read_text())
    assert {r['address']: r['size'] for r in inventory['functions']} == {r['address']: r['size'] for r in extents['functions']}
    subprocess.run(['sha256sum', '-c', 'SHA256SUMS'], cwd=score.ASM_DIR,
                   capture_output=True, check=True)
    sources = [BASE / (name + '.c') for name in NAMES]
    sources += sorted((WORK / 'controls').glob('*.c'))
    assert len(sources) == 7
    rows = []
    with tempfile.TemporaryDirectory(prefix='emitter-control-') as temp:
        temp = Path(temp)
        getter = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
        score.compile_single(getter, FLAGS, temp / 'getter.o')
        assert score.compare(temp / 'getter.o', getter.stem, show=0).accepted()
        for source in sources:
            name = '_'.join(source.stem.split('_')[:2])
            assert len(targets[name]) * 4 == NAMES[name]
            for level in (2, 1):
                flags = FLAGS.replace('-O2', '-O%d' % level)
                obj = temp / (source.stem + '-O%d.o' % level)
                score.compile_single(source, flags, obj)
                result = score.compare(obj, name, show=0)
                data, sections = score._elf(obj)
                syms = [s for i, section in enumerate(sections) if section['type'] == 2
                        for s in score._symbol_table(data, sections, i)
                        if s['name'] == name and s['type'] == 2]
                assert len(syms) == 1
                rows.append(dict(name=name, source_path=str(source.relative_to(ROOT)),
                                 source_sha256=digest(source), flags=flags,
                                 effective_flags=flags + ' ' + score.R4300_CC,
                                 differing_words=result.differing, total_words=result.total,
                                 extra_words=result.extra_words, unresolved=result.unresolved,
                                 unverified=result.unverified, errors=result.errors,
                                 strict_match=result.accepted(), elf_function_bytes=syms[0]['size']))
    return dict(schema_version=1, base_commit='cf4b9c619c72e83aa5da2e8b5c110765f9b03693',
                new_matches=0, new_matching_bytes=0, new_unique_attempts=0,
                compiler_files_sha256=compiler, census_functions=439, census_bytes=99120,
                getter_match=True, manifest_sha256=digest(score.ASM_DIR / 'SHA256SUMS'), results=rows)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    result = run()
    if args.check:
        assert result == json.loads((WORK / 'verification.json').read_text())
        print('PASS: all 14 hash-bound O2/O1 rows, ELF extents, compiler and canonical-input checks')
    else:
        print(json.dumps(result, indent=2))
