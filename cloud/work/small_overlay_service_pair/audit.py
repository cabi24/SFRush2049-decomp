"""Authenticate private input and local control-flow closure; emits metadata only."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import zlib

ROOT = Path(__file__).resolve().parents[3]
BASE = 0x8038A400
IMAGE_SHA256 = 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
# assets/us/data.bin = ROM 0x283D0-0xC00000 (boot-tail static extension; was ROM 0x10000-, c348ea04...)
ASSET_SHA256 = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
BODIES = [(0x8038D3A4, 0x8038D498, {0x800C55E4}),
          (0x8038D798, 0x8038DA78, {0x800A61B0, 0x8008C768, 0x8038D3A4})]


def sha(data):
    return hashlib.sha256(data).hexdigest()


def audit(path):
    asset = path.read_bytes()
    assert len(asset) == 12418096 and sha(asset) == ASSET_SHA256
    compressed = asset[0xB6FEC4 - 0x283D0:]
    inflater = zlib.decompressobj(-15)
    image = inflater.decompress(compressed)
    assert inflater.eof
    consumed = len(compressed) - len(inflater.unused_data)
    assert consumed == 23639
    assert len(image) == 43888 and sha(image) == IMAGE_SHA256
    constants = {}
    for address, expected in [(0x80394DC4, 0x3E4CCCCD),
                              (0x80394DD4, 0x3EB33333),
                              (0x80394DD8, 0x3FACCCCD)]:
        bits = struct.unpack_from('>I', image, address - BASE)[0]
        assert bits == expected
        constants[f'0x{address:08X}'] = f'0x{bits:08X}'
    bodies = []
    for start, end, expected_calls in BODIES:
        body = image[start - BASE:end - BASE]
        calls, branches, returns = [], 0, []
        for offset in range(0, len(body), 4):
            pc = start + offset
            word = struct.unpack_from('>I', body, offset)[0]
            op = word >> 26
            if op in (2, 3):
                dest = ((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
                assert op == 3, 'unreviewed tail/jump'
                calls.append(dest)
            if op in (1, 4, 5, 6, 7, 20, 21, 22, 23) or (
                    op == 17 and ((word >> 21) & 31) == 8):
                immediate = (word & 0xFFFF) - (0x10000 if word & 0x8000 else 0)
                dest = pc + 4 + immediate * 4
                assert start <= dest < end, 'branch escapes body'
                branches += 1
            if op == 0 and (word & 63) in (8, 9):
                assert (word & 63) == 8 and ((word >> 21) & 31) == 31
                returns.append(pc)
        assert set(calls) == expected_calls
        assert returns == [end - 8]
        bodies.append(dict(start=f'0x{start:08X}', end_exclusive=f'0x{end:08X}',
                           bytes=len(body), sha256=sha(body), local_branches=branches,
                           direct_calls=[f'0x{x:08X}' for x in calls],
                           return_address=f'0x{returns[0]:08X}'))
    return dict(classification='BOUNDED_BEHAVIORAL_SOURCE_NONMATCH',
                image_sha256=sha(image), rom_start='0x00B6FEC4',
                runtime_base=f'0x{BASE:08X}', compressed_bytes=consumed,
                image_bytes=len(image), asset_sha256=sha(asset),
                bodies=bodies, pool_words=constants, accepted_bytes=0,
                full_image_function_registry=False,
                caller_residency_proved=False)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--asset', type=Path, default=ROOT / 'assets/us/data.bin')
    args = parser.parse_args()
    print(json.dumps(audit(args.asset), indent=2))
