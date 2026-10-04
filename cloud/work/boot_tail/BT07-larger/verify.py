#!/usr/bin/env python3
"""Read-only strict replay for the two-function BT07 larger packet."""
from hashlib import sha256
from pathlib import Path
import json
import struct
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

ADDRESSES = ['800259A8', '80025C68']
FLAGS = '-g0 -O%d -mips2 -G 0 -non_shared'
score.ASM_DIR = ROOT / 'asm/us/boot_tail'


def digest(path):
    return sha256(path.read_bytes()).hexdigest()


def trial(source, name, level, obj):
    flags = FLAGS % level
    score.compile_single(source, flags, obj)
    result = score.compare(obj, name, show=0)
    want = score.targets()[name]
    words = score.text_words(obj)
    relocated, masks, unresolved, unverified, errors = score.relocate(
        obj, words, 0, len(want) * 4, score.image_symbols())
    assert score.symbols(obj) == {name: 0}
    if level == 2:
        assert not masks and not unresolved and not unverified and not errors
    return {'flags': flags, 'effective_flags': flags + ' -Wab,-r4300_mul',
            'differing_words': result.differing, 'total_words': result.total,
            'extra_words': result.extra_words, 'unresolved': result.unresolved,
            'unverified': result.unverified, 'errors': result.errors,
            'strict_match': result.accepted(), 'relocation_mask_count': len(masks), 'object_text_bytes': len(words) * 4,
            'relocated_full_word_equality': relocated[:len(want)] == want,
            'zero_trailing_alignment': not any(words[len(want):])}


def run():
    manifest = subprocess.run(['sha256sum', '-c', 'SHA256SUMS'], cwd=score.ASM_DIR,
                             text=True, capture_output=True, check=True).stdout.splitlines()
    ip = ROOT / 'specs/015-boot-tail-runtime/inventory.json'
    ep = score.ASM_DIR / 'extents.json'
    inv = json.loads(ip.read_text())['functions']
    ext = json.loads(ep.read_text())['functions']
    left = {r['address']: r['size'] for r in inv}
    right = {r['address']: r['size'] for r in ext}
    assert len(inv) == len(ext) == len(left) == len(right) == 439
    assert left == right and sum(left.values()) == 99120
    selected = [r for r in inv if r['address'][2:] in ADDRESSES]
    assert len(selected) == 2 and sum(r['size'] for r in selected) == 552
    assert all(r['scope'] == 'in_scope' and 256 <= r['size'] < 1024 for r in selected)
    paths = [ip, ep, score.ASM_DIR/'SHA256SUMS', score.ASM_DIR/'symbols.json',
             ROOT/'tools/cloud/score.py', ROOT/'tools/cloud/setup.sh', Path(__file__).resolve(),
             Path(__file__).with_name('test_semantics.py'), Path(__file__).with_name('README.md'),
             Path(__file__).with_name('input_pins.json'), Path(__file__).with_name('status_delta.csv'),
             Path(__file__).with_name('experiments.json'), Path(__file__).with_name('reconstruction.md')]
    pins = json.loads(Path(__file__).with_name('input_pins.json').read_text())
    assert {p.name: digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()} == pins['compiler_files_sha256']
    for path, expected in pins['source_hashes'].items():
        assert digest(ROOT/path) == expected, path
    receipt = {'schema_version': 1, 'packet': 'BT07-larger',
        'base_commit': '301d9e7552ad4fd7f54a38796db84671e1000d35',
        'central_claim_commit': 'b34484c9', 'manifest': manifest,
        'extents': {'functions': 439, 'bytes': 99120, 'equal': True},
        'toolchain_archive_sha256': 'ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506',
        'compiler_files_sha256': {p.name: digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()},
        'source_hashes': {str(p.relative_to(ROOT)): digest(p) for p in paths}, 'results': []}
    with tempfile.TemporaryDirectory(prefix='bt07-larger-') as tmp:
        getter = ROOT/'cloud/matches/boot_tail/func_80010A00.c'
        receipt['getter'] = trial(getter, getter.stem, 2, Path(tmp)/'getter.o')
        assert receipt['getter']['strict_match']
        for address in ADDRESSES:
            name = 'func_' + address
            source = ROOT/('cloud/matches/boot_tail/' + name + '.c')
            assert source.read_text().splitlines()[0] == '/* flags: ' + FLAGS % 2 + ' */'
            row = {'name': name, 'bytes': left['0x'+address],
                   'source_path': str(source.relative_to(ROOT)), 'source_sha256': digest(source),
                   'target_body_sha256': sha256(struct.pack('>%dI' % len(score.targets()[name]), *score.targets()[name])).hexdigest(),
                   'trials': [trial(source, name, level, Path(tmp)/(address+'-%d.o'%level)) for level in (2, 1)]}
            assert row['trials'][0]['strict_match'] and row['trials'][0]['relocated_full_word_equality']
            assert row['trials'][0]['zero_trailing_alignment']
            receipt['results'].append(row)
    receipt.update(result='PASS', new_matching_bodies=2, new_matching_bytes=552,
                   complete_nonmatch_bodies=0, complete_nonmatch_bytes=0)
    return receipt


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
