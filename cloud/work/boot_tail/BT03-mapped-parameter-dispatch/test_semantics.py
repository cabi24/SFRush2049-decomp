#!/usr/bin/env python3
"""Bounded integer-MIPS replay and native-source host differential tests.

This purpose-limited interpreter fails on unknown opcodes. It runs both the
protected full body and the independently relocated IDO candidate, including
their own dispatch tables and the complete protected unit-conversion callee.
This is not a cartridge, audio-output, or production-integration test.
"""
import argparse
import ctypes
import hashlib
import json
from pathlib import Path
import random
import struct
import subprocess
import tempfile
import verify

HERE = Path(__file__).resolve().parent
MASK = 0xFFFFFFFF
STACK = 0x10010000
RETURN = 0x70000000
CAPACITY = 250


def signed(v):
    return v - 0x100000000 if v & 0x80000000 else v


def divide(n, d):
    assert d > 0
    return -(abs(n) // d) if n < 0 else n // d


def expected(before, volume, duration, selector):
    result = bytearray(before)
    volume &= 255
    duration = (duration & 65535) or 1
    selector &= 255
    wanted = {255: (0, 1), 252: (2, 3), 250: (2,), 251: (3,), 253: (0,), 254: (1,)}
    indices = [i for i in range(32) if before[i * 40 + 20] in wanted[selector]] if selector in wanted else [selector]
    for i in indices:
        old = struct.unpack_from('>I', before, i * 40 + 24)[0]
        target = volume << 16
        delta = divide(signed((target - old) & MASK), duration) & MASK
        struct.pack_into('>III', result, i * 40 + 28, target, delta, duration * 256)
    return bytes(result), indices


class Machine:
    def __init__(self, words, table, callee):
        self.code = {verify.BASE + i * 4: w for i, w in enumerate(words)}
        self.code.update({verify.CALLEE + i * 4: w for i, w in enumerate(callee)})
        self.table = table

    def get(self, address, width):
        assert address % width == 0
        if verify.TABLE <= address and address + width <= verify.TABLE + len(self.table):
            self.table_reads.append(address)
            return int.from_bytes(self.table[address - verify.TABLE:address - verify.TABLE + width], 'big')
        if verify.DATA <= address and address + width <= verify.DATA + len(self.data):
            off = address - verify.DATA
            return int.from_bytes(self.data[off:off + width], 'big')
        assert STACK - 256 <= address and address + width <= STACK + 32, hex(address)
        assert all(address + i in self.stack for i in range(width)), 'uninitialized stack read'
        return int.from_bytes(bytes(self.stack[address + i] for i in range(width)), 'big')

    def put(self, address, width, value):
        assert address % width == 0
        buf = (value & ((1 << (width * 8)) - 1)).to_bytes(width, 'big')
        if verify.DATA <= address and address + width <= verify.DATA + len(self.data):
            off = address - verify.DATA
            self.data[off:off + width] = buf
            self.writes.append((address, width))
        else:
            assert STACK - 256 <= address and address + width <= STACK + 32, hex(address)
            for i, v in enumerate(buf):
                self.stack[address + i] = v

    def wr(self, reg, value):
        if reg:
            self.r[reg] = value & MASK

    def step(self, pc, delay=False):
        self.steps += 1
        self.visited.add(pc)
        assert self.steps < 6000
        w = self.code[pc]
        op, rs, rt, rd, sh, fn = w >> 26, w >> 21 & 31, w >> 16 & 31, w >> 11 & 31, w >> 6 & 31, w & 63
        imm = w & 65535
        simm = imm - 65536 if imm & 32768 else imm
        a, b = self.r[rs], self.r[rt]
        branch = None
        likely_skip = False
        if op == 0:
            if fn == 0:
                self.wr(rd, b << sh)
            elif fn == 2:
                self.wr(rd, b >> sh)
            elif fn in (33, 35, 37):
                self.wr(rd, a + b if fn == 33 else a - b if fn == 35 else a | b)
            elif fn == 18:
                self.wr(rd, self.lo)
            elif fn == 26:
                assert signed(b) > 0, ('unexpected divisor', hex(b))
                self.lo = divide(signed(a), signed(b)) & MASK
            elif fn == 8:
                branch = a
            elif fn == 13:
                raise AssertionError('native trap reached')
            else:
                raise AssertionError(('unknown SPECIAL', fn))
        elif op in (4, 5, 20, 21):
            take = (a == b) if op in (4, 20) else (a != b)
            if take:
                branch = pc + 4 + simm * 4
            elif op in (20, 21):
                likely_skip = True
            else:
                branch = pc + 8
        elif op == 3:
            branch = ((pc + 4) & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
            assert branch == verify.CALLEE
            self.wr(31, pc + 8)
        elif op == 9:
            self.wr(rt, a + simm)
        elif op == 11:
            self.wr(rt, int(a < (simm & MASK)))
        elif op == 12:
            self.wr(rt, a & imm)
        elif op == 13:
            self.wr(rt, a | imm)
        elif op == 15:
            self.wr(rt, imm << 16)
        elif op in (35, 36, 37):
            self.wr(rt, self.get((a + simm) & MASK, {35: 4, 36: 1, 37: 2}[op]))
        elif op in (40, 41, 43):
            self.put((a + simm) & MASK, {40: 1, 41: 2, 43: 4}[op], b)
        else:
            raise AssertionError(('unknown opcode', op))
        if branch is not None:
            assert not delay
            self.step(pc + 4, delay=True)
            if op == 3:
                self.calls.append((branch, self.get(self.r[4], 4)))
            return branch
        assert not (delay and likely_skip)
        return pc + (8 if likely_skip else 4)

    def run(self, before, args):
        self.data, self.stack = bytearray(before), {}
        self.r = [(0xA6000000 + i * 257) & MASK for i in range(32)]
        self.r[0] = 0
        self.r[4:7] = args
        self.r[29], self.r[31] = STACK, RETURN
        preserved = {i: self.r[i] for i in list(range(16, 24)) + [28, 29, 30]}
        self.steps, self.lo = 0, 0
        self.visited = set()
        self.calls, self.table_reads, self.writes = [], [], []
        pc = verify.BASE
        while pc != RETURN:
            pc = self.step(pc)
        assert all(self.r[i] == value for i, value in preserved.items())
        assert self.calls == [(verify.CALLEE, (args[1] & 65535) or 1)]
        selector = args[2] & 255
        assert self.table_reads == ([verify.TABLE + 4 * (selector - 250)] if selector >= 250 else [])
        return bytes(self.data)


def fixture(seed):
    rng = random.Random(seed)
    data = bytearray(rng.getrandbits(8) for _ in range(CAPACITY * 40))
    edges = [0, 0x7F0000, 0xFF0000, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF, 1, 0x10000]
    for i in range(CAPACITY):
        data[i * 40 + 20] = [0, 1, 2, 3, 4, 5, 127, 255][(i + seed) % 8]
        struct.pack_into('>I', data, i * 40 + 24, edges[(i * 3 + seed) % len(edges)])
    return bytes(data)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    proof = json.loads((HERE / 'dispatch_proof.json').read_text())
    native_table = verify.raw([int(r['target'], 16) for r in proof['selector_to_target']])
    targets = verify.score.targets()
    native = Machine(targets[verify.NAME], native_table, targets['func_8001E930'])
    count = host_count = 0
    native_visited = set()
    candidate_visited = set()
    rng = random.Random(2049)
    with tempfile.TemporaryDirectory(prefix='bt03-parameter-semantics-') as temporary:
        folder = Path(temporary)
        obj, _, resolved, _ = verify.build(folder)
        candidate_words = list(struct.unpack('>226I', resolved['.text'][:904]))
        candidate = Machine(candidate_words, resolved['.rodata'], targets['func_8001E930'])
        host = folder / 'host.c'
        host.write_text('#include "' + str(verify.SOURCE) + '"\n'
                        'MasterFader D_8004F300[32];\n'
                        'unsigned int host_calls, host_argument;\n'
                        'void func_8001E930(u32 *p) { ++host_calls; host_argument=*p; *p *= 256U; }\n')
        subprocess.run(['gcc', '-std=c89', '-Wall', '-Wextra', '-Werror', '-O2', '-shared', '-fPIC',
                        str(host), '-o', str(folder / 'host.so')], check=True, capture_output=True)
        lib = ctypes.CDLL(str(folder / 'host.so'))
        buf = (ctypes.c_ubyte * 1280).in_dll(lib, 'D_8004F300')
        calls = ctypes.c_uint.in_dll(lib, 'host_calls')
        arg = ctypes.c_uint.in_dll(lib, 'host_argument')
        fn = lib.func_8001BE14
        fn.argtypes = [ctypes.c_uint8, ctypes.c_uint16, ctypes.c_uint8]
        fn.restype = None
        valid = list(range(32)) + list(range(250, 256))
        cases = [(v, t, g, (g + v + t) % 8) for g in range(256)
                 for t in [0, 1, 2, 257, 65535] for v in [0, 1, 127, 255]]
        cases += [(rng.randrange(256), rng.randrange(65536), rng.choice(valid), rng.randrange(8))
                  for _ in range(512)]
        fixtures = [fixture(i) for i in range(8)]
        for volume, time, group, seed in cases:
            before = fixtures[seed]
            expected_bytes, selected = expected(before, volume, time, group)
            # Exercise native O32 low-width normalization with dirty upper bits.
            native_args = [volume | 0x937EAC00, time | 0xBACD0000, group | 0xDEEFAB00]
            assert native.run(before, native_args) == expected_bytes, ('native', volume, time, group)
            assert candidate.run(before, native_args) == expected_bytes, ('candidate', volume, time, group)
            native_visited.update(native.visited)
            candidate_visited.update(candidate.visited)
            allowed = {verify.DATA + i * 40 + off for i in selected for off in (28, 32, 36)}
            for machine in (native, candidate):
                assert {a for a, width in machine.writes} == allowed
                assert all(width == 4 for _, width in machine.writes)
            if group in valid:
                # Convert every aligned word, preserving bytes 20..23 (type/padding).
                host_bytes = bytearray(before[:1280])
                for i in range(32):
                    for off in list(range(0, 20, 4)) + [24, 28, 32, 36]:
                        value = struct.unpack_from('>I', before, i * 40 + off)[0]
                        struct.pack_into('=I', host_bytes, i * 40 + off, value)
                ctypes.memmove(buf, bytes(host_bytes), 1280)
                calls.value = 0
                fn(volume, time, group)
                assert calls.value == 1 and arg.value == (time or 1)
                actual = bytearray(buf)
                for i in range(32):
                    for off in list(range(0, 20, 4)) + [24, 28, 32, 36]:
                        value = struct.unpack_from('=I', actual, i * 40 + off)[0]
                        struct.pack_into('>I', actual, i * 40 + off, value)
                assert actual == expected_bytes[:1280], ('host', volume, time, group)
                host_count += 1
            count += 1
        report = dict(result='PASS', source_sha256=verify.sha(verify.SOURCE),
                      target_sha256=proof['canonical_function_sha256'],
                      object_sha256=verify.sha(obj), full_native_cases=count,
                      full_compiler_cases=count, host_cases=host_count,
                      native_instruction_pcs_covered=sum(verify.BASE <= pc < verify.BASE + 904 for pc in native_visited),
                      candidate_instruction_pcs_covered=sum(verify.BASE <= pc < verify.BASE + 904 for pc in candidate_visited),
                      native_unvisited_offsets=[i * 4 for i in range(226) if verify.BASE + i * 4 not in native_visited],
                      initialized_stack_reads_only=True,
                      checked='entire initialized memory, exact write set, table access, callee input, callee-saved registers, restored stack',
                      valid_C_domain='selectors 0..31 and 250..255; any u8 volume and u16 duration',
                      invalid_selectors='32..249 native/candidate replay only with mapped synthetic backing; not asserted valid production storage',
                      exclusions='no real audio output, original runtime execution, production placement, ROM, or timing claim')
    text = json.dumps(report, indent=2) + '\n'
    if args.output:
        args.output.write_text(text)
    print(text)


if __name__ == '__main__':
    main()
