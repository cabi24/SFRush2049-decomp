#!/usr/bin/env python3
"""Portable, compiler-free selector-address evidence; no native bytes emitted."""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import subprocess
import sys
import zlib

BASE = '6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
TARGET_SHA256 = 'c8842f0b25b7c17e36345611643e4b67a7c0c17929f7be6ba93785b530b67b39'
IMAGE_SHA256 = 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
IMAGE_BASE = 0x8038A400
OFFSETS, PLAYERS, PHYSICS, CACHE = 0x803943A4, 0x80152818, 0x8014A250, 0x80399118
DESCRIPTOR, STACK = 0x100000, 0x200000
MASK = 0xffffffff


def signed(value, width=32):
    value &= (1 << width) - 1
    return value - (1 << width) if value >> (width - 1) else value


def context(root, path):
    return subprocess.check_output(['git', '-C', str(root), 'show', BASE + ':' + path])


def load(root):
    spec = importlib.util.spec_from_file_location('selector_score', root / 'tools/cloud/score.py')
    score = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = score
    spec.loader.exec_module(score)
    score.ASM_DIR = root / 'asm/us/ovl_b'
    words = score.targets()['func_8038AA8C']
    raw = struct.pack('>' + 'I' * len(words), *words)
    assert len(raw) == 7812 and hashlib.sha256(raw).hexdigest() == TARGET_SHA256
    asset = context(root, 'assets/us/data.bin')
    inflater = zlib.decompressobj(-15)
    image = inflater.decompress(asset[0xB6FEC4 - 0x283D0:])
    assert inflater.eof and len(image) == 43888
    assert hashlib.sha256(image).hexdigest() == IMAGE_SHA256
    offset = 0x8038AA8C - IMAGE_BASE
    assert image[offset:offset + len(raw)] == raw
    return {0x8038AA8C + i*4: word for i, word in enumerate(words)}, image


class Slice:
    """Execute only the unchanged AA8C selector prefix, with unmapped reads fatal.

    Callee effects and the creation/animation bodies are intentionally excluded.
    The changed-mode fixture uses the genuinely initialized cache sentinel 9,
    so no private cleanup call is reached. Same-mode fixtures exercise the
    alternate offset loads. Integer and floating loads preserve bits exactly.
    """
    def __init__(self, code, image, owner, mode, model, changed):
        self.code, self.memory, self.reads, self.table_reads = code, {}, [], []
        self.r = [None] * 32
        self.f = [None] * 32
        self.r[0], self.r[16], self.r[22], self.r[23], self.r[29] = 0, owner, DESCRIPTOR, PLAYERS + 0x3B8*owner, STACK
        self.put(DESCRIPTOR + 8, owner, 2)
        self.put(self.r[23] + 0x384, mode, 1)
        self.put(PHYSICS + 0x808*owner + 8, model, 1)
        self.put(CACHE + owner, 9 if changed else mode, 1)
        for i, byte in enumerate(image):
            self.memory[IMAGE_BASE + i] = byte
        self.changed = changed
        self.covered = set()

    def reg(self, i):
        assert self.r[i] is not None, ('uninitialized register', i)
        return self.r[i]

    def put(self, address, value, width=4):
        assert address % width == 0
        for i, byte in enumerate((value & ((1 << (8*width))-1)).to_bytes(width, 'big')):
            self.memory[address + i] = byte

    def get(self, address, width=4):
        assert address % width == 0
        assert all(address + i in self.memory for i in range(width)), ('unmapped read', hex(address))
        self.reads.append((self.pc, address, width))
        if IMAGE_BASE <= address < IMAGE_BASE + 43888:
            self.table_reads.append((self.pc, address, width))
        return int.from_bytes(bytes(self.memory[address + i] for i in range(width)), 'big')

    def run(self):
        self.pc, pending = 0x8038AC80, None
        terminals = {0x8038AE30, 0x8038B3E4, 0x8038B754, 0x8038B760, 0x8038B76C,
                     0x8038B778, 0x8038B784, 0x8038B790, 0x8038C8E4, 0x8038B798,
                     0x8038B964}
        for _ in range(250):
            if self.pc in terminals:
                break
            assert self.pc in self.code, ('unknown pc', hex(self.pc))
            self.covered.add(self.pc)
            w = self.code[self.pc]
            op, rs, rt, rd, sh = w >> 26, (w >> 21)&31, (w >> 16)&31, (w >> 11)&31, (w >> 6)&31
            si = signed(w, 16)
            old, pending, next_pc = pending, None, self.pc + 4
            if op == 0:
                fn = w & 63
                if fn == 0: self.r[rd] = (self.reg(rt) << sh)&MASK
                elif fn == 3: self.r[rd] = (signed(self.reg(rt)) >> sh)&MASK
                elif fn == 0x21: self.r[rd] = (self.reg(rs) + self.reg(rt))&MASK
                elif fn == 0x23: self.r[rd] = (self.reg(rs) - self.reg(rt))&MASK
                elif fn == 8: pending = self.reg(rs)
                else: raise AssertionError(('unexpected SPECIAL', hex(self.pc), fn))
            elif op == 9: self.r[rt] = (self.reg(rs) + si)&MASK
            elif op == 11: self.r[rt] = int(self.reg(rs) < (si&MASK))
            elif op == 15: self.r[rt] = (w&65535) << 16
            elif op in (4, 5):
                take = (self.reg(rs) == self.reg(rt)) if op == 4 else (self.reg(rs) != self.reg(rt))
                if take: pending = self.pc + 4 + si*4
            elif op in (32, 33, 35, 36):
                width = 4 if op == 35 else 2 if op == 33 else 1
                v = self.get((self.reg(rs) + si)&MASK, width)
                self.r[rt] = (signed(v, width*8)&MASK) if op in (32, 33) else v
            elif op in (40, 43): self.put((self.reg(rs) + si)&MASK, self.reg(rt), 1 if op == 40 else 4)
            elif op == 49: self.f[rt] = self.get((self.reg(rs) + si)&MASK)
            elif op == 57:
                assert self.f[rt] is not None
                self.put((self.reg(rs) + si)&MASK, self.f[rt])
            else: raise AssertionError(('unexpected opcode', hex(self.pc), op))
            self.r[0] = 0
            assert old is None or pending is None, 'branch in delay slot'
            self.pc = old if old is not None else next_pc
        else: raise AssertionError('step limit')
        return self.pc


def verify(root):
    code, image = load(root)
    cases, coverage = 0, set()
    targets = [0x8038AE30, 0x8038B3E4, 0x8038B754, 0x8038B760, 0x8038B76C,
               0x8038B778, 0x8038B784, 0x8038B790, 0x8038C8E4]
    mode8_reads = set()
    for owner in range(4):
        for mode in range(9):
            for model in range(13):
                for changed in (False, True):
                    machine = Slice(code, image, owner, mode, model, changed)
                    assert machine.run() == (targets[mode] if changed else 0x8038B964)
                    offset = OFFSETS + 0x9C*mode + 12*model
                    loads = [(a,w) for pc,a,w in machine.table_reads if pc != 0x8038AE24]
                    assert loads == [(offset + i*4, 4) for i in range(3)]
                    expected_sites = [0x8038ACE8, 0x8038AD48, 0x8038AD9C] if changed else [0x8038B880, 0x8038B8F8, 0x8038B94C]
                    assert [pc for pc,a,w in machine.table_reads if pc != 0x8038AE24] == expected_sites
                    for i in range(12):
                        assert machine.memory[STACK + 92 + i] == image[offset - IMAGE_BASE + i]
                    assert machine.memory[CACHE + owner] == mode
                    if mode == 8: mode8_reads.update(a for a,w in loads)
                    coverage.update(machine.covered)
                    cases += 1
    # Out-of-domain mode 9 does not get a resource index assigned by a switch case.
    invalid = Slice(code, image, 0, 9, 0, True)
    invalid.put(CACHE, 8, 1)
    assert invalid.run() == 0x8038B798
    assert not any(STACK + 128 + i in invalid.memory for i in range(4))
    # Fail closed rather than treat missing model memory or unknown opcode as zero.
    missing = Slice(code, image, 0, 0, 0, True)
    del missing.memory[PHYSICS + 8]
    try: missing.run()
    except AssertionError as exc: assert 'unmapped read' in str(exc)
    else: raise AssertionError('unmapped model accepted')
    bad = Slice(dict(code), image, 0, 0, 0, True)
    bad.code[0x8038AC80] = MASK
    try: bad.run()
    except AssertionError as exc: assert 'unexpected opcode' in str(exc)
    else: raise AssertionError('unknown opcode accepted')
    return {
        'status': 'BOUNDED_NATIVE_ADDRESS_RESEARCH_ONLY', 'base_commit': BASE,
        'verifier_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        'native_target': {'name': 'func_8038AA8C', 'size': 7812, 'sha256': TARGET_SHA256},
        'selector_slice_fixtures': cases, 'selector_slice_instruction_coverage': len(coverage),
        'fixture_domain': {'owner': [0,3], 'mode': [0,8], 'model': [0,12],
                           'changed_cache': 9, 'same_cache': 'equal to mode'},
        'mode8_read_envelope': [hex(min(mode8_reads)), hex(max(mode8_reads)+4)],
        'mode8_source_color_alias': hex(OFFSETS + 8*0x9C),
        'negative_controls': ['mode 9 reaches unset resource default', 'unmapped model rejected', 'unknown opcode rejected'],
        'scope_limits': ['not a complete-function execution', 'no source-object extent or general caller-bound proof',
                         'no cleanup/create/animation helper effects', 'no compiled candidate or matching claim'],
    }


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--reference-root', type=Path, required=True)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    result = verify(args.reference_root.resolve())
    text = json.dumps(result, indent=2) + '\n'
    if args.output: args.output.write_text(text)
    else: print(text, end='')
