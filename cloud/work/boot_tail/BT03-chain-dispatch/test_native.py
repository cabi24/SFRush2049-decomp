#!/usr/bin/env python3
"""Execute protected target instructions with bounded MIPS/FPU and call models.

This is a purpose-limited interpreter, not an emulator or cartridge test. It
supports only the audited function's instructions. Unknown instructions fail.
External helpers are modeled at their audited integer ABI boundaries. IEEE
single arithmetic and conversion status paths are modeled explicitly, including
native out-of-C-domain conversion fallback. Raw words are read only at runtime.
"""
import argparse
import json
import math
from pathlib import Path
import random
import struct
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
score.ASM_DIR = ROOT / 'asm/us/boot_tail'
MASK = 0xFFFFFFFF
BASE = 0x8001CCDC
EMITTER = 0x100000
STACK = 0x200000
RETURN = 0x700000


def bits(value):
    try:
        return struct.unpack('>I', struct.pack('>f', value))[0]
    except OverflowError:
        return 0xFF800000 if value < 0 else 0x7F800000


def fval(word):
    return struct.unpack('>f', struct.pack('>I', word & MASK))[0]


def f32(value):
    return fval(bits(value))


def signed(value):
    return value - (1 << 32) if value & 0x80000000 else value


def quantize(value):
    """Independent oracle for this IDO unsigned conversion sequence."""
    if not math.isfinite(value) or value >= 4294967296.0 or value <= -1.0:
        return MASK
    return math.trunc(value)


class Machine:
    def __init__(self, words, flags, identifier, fade, inputs):
        self.words = words
        self.gpr = [0] * 32
        self.fpr = [0] * 32
        self.fcsr = 0x1234
        self.memory = {EMITTER + i: 0xA5A5A5A5 for i in range(0, 68, 4)}
        self.memory.update({EMITTER + 8: flags, EMITTER + 52: identifier, EMITTER + 64: bits(fade),
                            STACK + 16: bits(inputs[3]), STACK + 20: bits(inputs[4])})
        self.gpr[4:8] = [EMITTER] + [bits(v) for v in inputs[:3]]
        self.gpr[29] = STACK
        self.gpr[31] = RETURN
        self.events = []
        self.steps = 0
        self.initial = dict(self.memory)

    def write(self, reg, value):
        if reg:
            self.gpr[reg] = value & MASK

    def external(self, address):
        a0, a1 = self.gpr[4:6]
        if address == 0x8001CC9C:
            assert a0 <= 255
            self.events.append(('clip7', a0))
            result = min(a0, 127)
        elif address == 0x8001CCC0:
            self.events.append(('clip14', a0))
            result = min(a0, 16383)
        else:
            channel = {0x8001B7C0: 7, 0x8001B29C: 10, 0x8001B3A0: 131, 0x8001B4A4: 132}[address]
            self.events.append(('send', channel, a0, a1))
            result = MASK if channel & 1 else 0
        # Strong adversarial caller-clobber and globally reachable emitter writes.
        for reg in list(range(2, 16)) + [24, 25]:
            self.gpr[reg] = 0xBAD00000 | reg
        for reg in range(20):
            self.fpr[reg] = 0x7FC00001
        self.gpr[2] = result
        self.memory[EMITTER + 8] ^= 0x100000
        self.memory[EMITTER + 52] ^= MASK
        self.memory[EMITTER + 64] = bits(0.375)

    def execute(self, pc):
        assert BASE <= pc < BASE + 936 and (pc - BASE) % 4 == 0
        self.steps += 1
        assert self.steps < 500
        w = self.words[(pc - BASE) // 4]
        op, rs, rt, rd, sa, fn = w >> 26, w >> 21 & 31, w >> 16 & 31, w >> 11 & 31, w >> 6 & 31, w & 63
        imm = w & 65535
        simm = imm - 65536 if imm & 32768 else imm
        r, t = self.gpr[rs], self.gpr[rt]
        target = pc + 4 + simm * 4
        if op == 0:
            if fn == 0:
                self.write(rd, t << sa)
            elif fn == 37:
                self.write(rd, r | t)
            elif fn == 8:
                return ('jump', r, True)
            else:
                raise AssertionError(('R opcode', fn))
        elif op == 9:
            self.write(rt, r + simm)
        elif op == 12:
            self.write(rt, r & imm)
        elif op == 15:
            self.write(rt, imm << 16)
        elif op in (35, 43, 49):
            address = (r + simm) & MASK
            if op == 35:
                self.write(rt, self.memory[address])
            elif op == 43:
                self.memory[address] = t
            else:
                self.fpr[rt] = self.memory[address]
        elif op in (4, 5, 20):
            take = r != t if op == 5 else r == t
            return ('jump', target if take else pc + 8, take or op != 20)
        elif op == 1:
            assert rt in (0, 1)
            take = signed(r) < 0 if rt == 0 else signed(r) >= 0
            return ('jump', target if take else pc + 8, True)
        elif op == 3:
            self.write(31, pc + 8)
            return ('call', (pc + 4) & 0xF0000000 | (w & 0x3FFFFFF) * 4, True)
        elif op == 17:
            if rs == 0:
                self.write(rt, self.fpr[rd])
            elif rs == 4:
                self.fpr[rd] = t
            elif rs == 2:
                assert rd == 31
                self.write(rt, self.fcsr)
            elif rs == 6:
                assert rd == 31
                self.fcsr = t
            elif rs == 16:
                a, b = fval(self.fpr[rd]), fval(self.fpr[rt])
                if fn in (0, 1, 2):
                    value = a + b if fn == 0 else a - b if fn == 1 else a * b
                    self.fpr[sa] = bits(value)
                elif fn == 36:
                    assert self.fcsr & 3 == 1  # round toward zero, established by native CTC1
                    if not math.isfinite(a) or a >= 2147483648.0 or a < -2147483648.0:
                        self.fcsr |= 0x40
                        self.fpr[sa] = 0x80000000
                    else:
                        integer = math.trunc(a)
                        self.fpr[sa] = integer & MASK
                        if integer != a:
                            self.fcsr |= 4
                else:
                    raise AssertionError(('COP1 arithmetic', fn))
            else:
                raise AssertionError(('COP1', rs))
        else:
            raise AssertionError(('opcode', op))
        return None

    def run(self):
        pc = BASE
        while pc != RETURN:
            action = self.execute(pc)
            if action is None:
                pc += 4
                continue
            kind, destination, delay = action
            if delay:
                assert self.execute(pc + 4) is None
            if kind == 'call':
                self.external(destination)
                pc += 8
            else:
                pc = destination
        assert self.gpr[29] == STACK and self.gpr[31] == RETURN and self.gpr[16] == 0
        assert self.fcsr == 0x1234
        for offset in range(0, 68, 4):
            assert self.memory[EMITTER + offset] == (bits(0.375) if offset == 64 else self.initial[EMITTER + offset])
        return self.events


def trial(words, flags, identifier, fade, values):
    volume, xpan, ypan, zpan, doppler = map(f32, values)
    fade = f32(fade)
    gain = f32(fade * volume) if flags & 0x100000 else volume
    raw = [quantize(f32(gain * 127.0)), quantize(f32(f32(1.0 + xpan) * 64.0)),
           quantize(f32(f32(1.0 - zpan) * 64.0)), quantize(f32(doppler * 8192.0))]
    expected = []
    for channel, value in zip([7, 10, 131, 132], raw):
        if channel != 132:
            expected += [('clip7', value & 255), ('send', channel, identifier, min(value & 255, 127))]
        else:
            expected += [('clip14', value), ('send', channel, identifier, min(value, 16383))]
    m = Machine(words, flags, identifier, fade, [volume, xpan, ypan, zpan, doppler])
    assert m.run() == expected, (flags, fade, values, m.events, expected)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--output', type=Path)
    args = ap.parse_args()
    words = score.targets()['func_8001CCDC']
    rng = random.Random(0x1CCDC)
    count = 0
    for i in range(3000):
        trial(words, rng.getrandbits(32), rng.getrandbits(32), rng.randrange(512) / 256,
              [rng.randrange(1024) / 256, rng.randrange(65536) / 64 - 1,
               math.nan, 1 - rng.randrange(65536) / 64, rng.randrange(65536) / 8192])
        count += 1
    # Include values around the unsigned high-bit fallback, as well as native
    # defined fallback outcomes that C does not promise for nonfinite/out-of-range.
    edges = [0.0, -0.0, 0.5, 126.0, 127.0, 128.0, 255.0, 256.0, 257.0,
             16383.0, 16384.0, 2147483520.0, 2147483648.0, 4294967040.0,
             4294967296.0, -0.5, -1.0, -256.0, math.inf, -math.inf, math.nan]
    for edge in edges:
        for flag in [0, 0x100000, 0xFFEFFFFF, MASK]:
            trial(words, flag, 0xABCDEF12, 0.5,
                  [edge / 127.0, edge / 64.0 - 1.0, math.inf, 1.0 - edge / 64.0, edge / 8192.0])
            count += 1
    report = dict(result='PASS', native_calls=count, modeled_external_calls=count * 8,
                  target='func_8001CCDC', target_bytes=936, target_words=234,
                  coverage=['complete native body both fade paths', 'all four ordered controller sends',
                            'all genuine O32 input slots including unused a3', 'caller-saved clobbers and emitter mutation',
                            'preserved stack/s0/ra/FCSR', 'unsigned conversion high-bit fallback',
                            'native invalid conversion saturation before byte truncation'],
                  limits='Purpose-limited software instruction model, no hardware or ROM execution; C tests have a narrower defined conversion domain.')
    rendered = json.dumps(report, indent=2) + '\n'
    if args.output:
        args.output.write_text(rendered)
    else:
        print(rendered, end='')


if __name__ == '__main__':
    main()
