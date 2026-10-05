#!/usr/bin/env python3
"""Replay strict BE744 proof and host encoding cases; write sanitized JSON only."""
import argparse
import ctypes
import hashlib
import json
import random
import struct
import subprocess
import sys
import tempfile
from dataclasses import asdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

NAME = 'func_800BE744'
SOURCE = ROOT / 'cloud/matches/func_800BE744.c'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'


def digest(data):
    return hashlib.sha256(data).hexdigest()


def symbol_extent(obj):
    data, sections = score._elf(obj)
    symbols = [s for i, section in enumerate(sections) if section['type'] == 2
               for s in score._symbol_table(data, sections, i)
               if s['name'] == NAME and s['type'] == 2]
    assert len(symbols) == 1, symbols
    return symbols[0]['size']


def proof(obj, target):
    result = score.compare(obj, NAME, show=0)
    size = symbol_extent(obj)
    assert result.differing == 0 and result.total == 30
    assert not (result.unresolved or result.unverified or result.errors or result.extra_words)
    assert size == len(target) * 4 == 120
    start = score.symbols(obj)[NAME]
    words = score.text_words(obj)
    resolved, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, start + size, score.image_symbols())
    assert not (masks or unresolved or unverified or errors)
    assert resolved[start // 4:(start + size) // 4] == target
    return dict(comparison=asdict(result), elf_function_bytes=size,
                full_unmasked_words_equal=True, object_sha256=digest(obj.read_bytes()))


def semantics(work):
    shared = work / 'host.so'
    subprocess.run(['cc', '-std=c89', '-Wall', '-Wextra', '-Werror', '-shared', '-fPIC',
                    '-O2', str(SOURCE), '-o', str(shared)], check=True, capture_output=True)
    fn = getattr(ctypes.CDLL(str(shared)), NAME)
    fn.argtypes = [ctypes.POINTER(ctypes.c_ubyte)]
    fn.restype = ctypes.c_int
    cases = 0

    def check(data, expected):
        nonlocal cases
        buf = (ctypes.c_ubyte * len(data))(*data)
        got = fn(buf)
        assert got == expected, (data, expected, got)
        cases += 1

    check([0], 0)
    check([255, 0, 0], 0)
    for first in range(1, 255):
        check([first, 255, 0], 2)  # marker only applies at the beginning
    for high in range(256):
        for low in range(256):
            if high or low:
                check([255, high, low, 0, 0], 1)
    rng = random.Random(0xBE744)
    for length in range(65):
        for _ in range(4):
            plain = [rng.randrange(1, 255)] if length else []
            plain += [rng.randrange(1, 256) for _ in range(max(0, length - 1))]
            check(plain + [0], length)
            wide = [255]
            for _ in range(length):
                value = rng.randrange(1, 65536)
                wide.extend([value >> 8, value & 255])
            check(wide + [0, 0], length)
    return dict(passed_cases=cases, exhaustive_nonzero_two_byte_characters=65535,
                ordinary_lengths=[0, 64], wide_lengths=[0, 64],
                compiler='cc -std=c89 -Wall -Wextra -Werror -shared -fPIC -O2')


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    target = score.targets()[NAME]
    records = {}
    with tempfile.TemporaryDirectory(prefix='be744-proof-') as directory:
        work = Path(directory)
        for level in ('O3', 'O2'):
            obj = work / (level + '.o')
            flags = FLAGS.replace('O3', level)
            score.compile_single(SOURCE, flags, obj)
            records[level] = dict(flags=flags + ' -Wab,-r4300_mul', **proof(obj, target))
        group = work / 'group'
        group.mkdir()
        (group / 'source.c').write_bytes(SOURCE.read_bytes())
        (group / 'group.json').write_text(json.dumps(dict(
            files=['source.c'], keep=[NAME], members=[NAME], claims=[NAME], flags=FLAGS)))
        score.compile_group(group, work / 'group.o')
        records['O3_kept_single_source_group'] = dict(flags=FLAGS, **proof(work / 'group.o', target))
        negative = work / 'negative.c'
        negative.write_text(SOURCE.read_text().replace('return count;', 'return count + 1;'))
        score.compile_single(negative, FLAGS, work / 'negative.o')
        rejection = score.compare(work / 'negative.o', NAME, show=0)
        assert rejection.differing > 0
        behavior = semantics(work)
    receipt = dict(
        function=NAME, start='0x800BE744', end_exclusive='0x800BE7BC', target_bytes=120,
        baseline='cf10b3392d7f00ae42d75c008b79fdc2541aab6b',
        source=str(SOURCE.relative_to(ROOT)), source_sha256=digest(SOURCE.read_bytes()),
        target_sha256=digest(struct.pack('>30I', *target)),
        manifest_sha256=digest((score.ASM_DIR / 'SHA256SUMS').read_bytes()),
        toolchain={tool: digest(Path(score.ido(tool)).read_bytes())
                   for tool in ('cc', 'uopt', 'ugen', 'as1')},
        builds=records, semantic_tests=behavior, negative_return_control=asdict(rejection),
        gates=dict(whole_program_shadow='not run: extracted image/layout unavailable',
                   image='not run', compressed_stream='not run', full_rom='not run'),
        cartridge_coverage_claim=False)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(receipt, indent=2, sort_keys=True) + '\n')
    print(f'{NAME}: strict 30/30 + ELF 120 B at O3/O2 and O3 kept group; '
          f'{behavior["passed_cases"]} host cases; negative control rejected')


if __name__ == '__main__':
    main()
