#!/usr/bin/env python3
"""Read-only Packet 2 target/census checks and strict existing-getter replay."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def check_extents(inventory, extents):
    def keyed(rows):
        out = {int(r['address'], 16): r['size'] for r in rows}
        if len(out) != len(rows) or len(rows) != 439:
            raise ValueError('expected 439 unique function starts')
        return out
    left = keyed(inventory['functions'])
    right = keyed(extents['functions'])
    if left != right or sum(left.values()) != 99120:
        raise ValueError('target/inventory starts or sizes drifted')
    return {'functions': len(left), 'bytes': sum(left.values()), 'equal': True}


def run():
    subprocess.run(['bash', 'tools/cloud/setup.sh'], cwd=ROOT, check=True,
                   stdout=sys.stderr)
    target_dir = ROOT / 'asm/us/boot_tail'
    manifest = subprocess.run(['sha256sum', '-c', 'SHA256SUMS'], cwd=target_dir,
                              capture_output=True, text=True, check=True)
    inventory = ROOT / 'specs/015-boot-tail-runtime/inventory.json'
    extents = target_dir / 'extents.json'
    extent_result = check_extents(json.loads(inventory.read_text()),
                                 json.loads(extents.read_text()))
    flags = '-g0 -O2 -mips2 -G 0 -non_shared'
    source = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
    score.ASM_DIR = target_dir
    with tempfile.TemporaryDirectory(prefix='boot-tail-preflight-') as tmp:
        obj = Path(tmp) / 'getter.o'
        score.compile_single(source, flags, obj)
        result = score.compare(obj, source.stem, show=0)
        if not result.accepted():
            raise ValueError('strict getter replay failed: ' + result.summary())
    files = [inventory, extents, target_dir / 'SHA256SUMS',
             ROOT / 'tools/cloud/setup.sh', ROOT / 'tools/cloud/score.py', source]
    compiler = {p.name: digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()}
    return {'schema_version': 1, 'result': 'PASS', 'manifest': manifest.stdout.splitlines(),
            'extents': extent_result, 'getter': {'name': source.stem, 'bytes': 12,
            'flags': flags, 'effective_flags': flags + ' -Wab,-r4300_mul',
            'differing_words': result.differing, 'total_words': result.total,
            'extra_words': result.extra_words, 'unresolved': result.unresolved,
            'unverified': result.unverified, 'errors': result.errors, 'strict_match': True},
            'source_hashes': {str(p.relative_to(ROOT)): digest(p) for p in files},
            'compiler_files_sha256': compiler}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    result = json.dumps(run(), indent=2) + '\n'
    if args.output:
        args.output.write_text(result)
    else:
        print(result, end='')
