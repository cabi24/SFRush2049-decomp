#!/usr/bin/env python3
"""Compile/score with authenticated image B data held in memory; no binary output."""
import argparse
import hashlib
from pathlib import Path
import subprocess
import sys
import tempfile
import zlib

BASE = 'f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2'
ASSET_HASH = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
IMAGE_HASH = 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'


def main():
    here = Path(__file__).resolve().parent
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', type=Path, default=here.parents[3])
    parser.add_argument('--reference-root', type=Path)
    args = parser.parse_args()
    repo = args.repo.resolve()
    reference = (args.reference_root or repo).resolve()
    sys.path.insert(0, str(repo / 'tools/cloud'))
    import score
    import owndata
    asset = subprocess.check_output(['git', '-C', str(reference), 'show', BASE + ':assets/us/data.bin'])
    if len(asset) != 12418096 or hashlib.sha256(asset).hexdigest() != ASSET_HASH:
        raise SystemExit('asset identity mismatch')
    if int.from_bytes(asset[0x3854:0x3858], 'big') != 0xB6FEC4:
        raise SystemExit('image B loader pointer mismatch')
    image = zlib.decompress(asset[0xB6FEC4 - 0x283D0:], -15)
    if len(image) != 43888 or hashlib.sha256(image).hexdigest() != IMAGE_HASH:
        raise SystemExit('image B identity mismatch')
    score.ASM_DIR = repo / 'asm/us/ovl_b'
    score._own_data[owndata.artifact_dir(score.ASM_DIR)] = owndata.ImageData.from_image(image, 0x8038A400)
    with tempfile.TemporaryDirectory() as temp:
        obj = Path(temp) / 'candidate.o'
        score.compile_single(here / 'candidate.c', FLAGS, obj)
        result = score.compare(obj, 'func_80392FE4', show=0)
        print(result.summary())
        print('unresolved:', result.unresolved)
        print('unverified:', result.unverified)
        print('errors:', result.errors)
        print('extra words:', result.extra_words)


if __name__ == '__main__':
    main()
