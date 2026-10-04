#!/usr/bin/env python3
"""Read-only, hash-bound strict replay of the seven BT07 small bodies."""
from contextlib import redirect_stdout
from hashlib import sha256
from io import StringIO
from pathlib import Path
import json
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

ADDRESSES = ['80024FB0', '800250F0', '80025120', '80025150',
             '80025D84', '80026328', '80026348']
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
score.ASM_DIR = ROOT / 'asm/us/boot_tail'


def digest(path):
    return sha256(path.read_bytes()).hexdigest()


def trial(source, flags, obj):
    score.compile_single(source, flags, obj)
    with redirect_stdout(StringIO()):
        result = score.compare(obj, source.stem, show=0)
    return {'flags': flags, 'effective_flags': flags + ' -Wab,-r4300_mul',
            'differing_words': result.differing, 'total_words': result.total,
            'extra_words': result.extra_words, 'unresolved': result.unresolved,
            'unverified': result.unverified, 'errors': result.errors,
            'strict_match': result.accepted()}


def run():
    manifest = subprocess.run(['sha256sum', '-c', 'SHA256SUMS'],
                              cwd=score.ASM_DIR, capture_output=True,
                              text=True, check=True).stdout.splitlines()
    inventory_path = ROOT / 'specs/015-boot-tail-runtime/inventory.json'
    extent_path = score.ASM_DIR / 'extents.json'
    inventory = json.loads(inventory_path.read_text())['functions']
    extents = json.loads(extent_path.read_text())['functions']
    left = {row['address']: row['size'] for row in inventory}
    right = {row['address']: row['size'] for row in extents}
    assert len(left) == len(inventory) == len(right) == len(extents) == 439
    assert left == right and sum(left.values()) == 99120
    selected = [row for row in inventory if row['address'][2:] in ADDRESSES]
    assert len(selected) == 7 and sum(row['size'] for row in selected) == 280
    assert all(row['scope'] == 'in_scope' and row['size'] < 64 for row in selected)
    files = [inventory_path, extent_path, score.ASM_DIR / 'SHA256SUMS',
             score.ASM_DIR / 'symbols.json', ROOT / 'tools/cloud/score.py',
             ROOT / 'tools/cloud/setup.sh', Path(__file__).resolve()]
    receipt = {'schema_version': 1,
               'base_commit': '301d9e7552ad4fd7f54a38796db84671e1000d35',
               'central_claim_commit': '5b44ebd3', 'packet': 'BT07-small',
               'manifest': manifest,
               'extents': {'functions': 439, 'bytes': 99120, 'equal': True},
               'toolchain_archive_sha256': 'ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506',
               'compiler_files_sha256': {p.name: digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()},
               'source_hashes': {str(p.relative_to(ROOT)): digest(p) for p in files},
               'results': []}
    with tempfile.TemporaryDirectory(prefix='bt07-replay-') as directory:
        getter = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
        receipt['getter'] = {'source_sha256': digest(getter),
                             **trial(getter, FLAGS, Path(directory) / 'getter.o')}
        assert receipt['getter']['strict_match']
        for address in ADDRESSES:
            source = ROOT / 'cloud/matches/boot_tail' / ('func_' + address + '.c')
            assert source.read_text().splitlines()[0] == '/* flags: ' + FLAGS + ' */'
            row = {'name': source.stem, 'bytes': left['0x' + address],
                   'source_path': str(source.relative_to(ROOT)),
                   'source_sha256': digest(source), 'trials': []}
            for level in (2, 1):
                flags = '-g0 -O%d -mips2 -G 0 -non_shared' % level
                row['trials'].append(trial(source, flags, Path(directory) / (address + '-O%d.o' % level)))
            assert row['trials'][0]['strict_match'], row['name']
            receipt['results'].append(row)
    receipt['result'] = 'PASS'
    receipt['new_matching_bodies'] = 7
    receipt['new_matching_bytes'] = 280
    return receipt


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
