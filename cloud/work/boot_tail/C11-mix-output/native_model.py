"""Small fail-closed interpreter for this canonical float mixer, not a MIPS emulator.

Words are loaded from the existing hash-verified canonical target, never embedded.
All floating operations round to binary32; fixtures exclude exceptional casts.
"""
import math
import struct
from pathlib import Path
import sys
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
BASE = 0x8001E0E0
MASK = 0xFFFFFFFF


def signed(x):
    return x - 0x100000000 if x & 0x80000000 else x


def bits(x):
    return struct.unpack('>I', struct.pack('>f', x))[0]


def floating(x):
    return struct.unpack('>f', struct.pack('>I', x))[0]


def f32(x):
    return floating(bits(x))


class Mixer:
    def __init__(self, words=None):
        score.ASM_DIR = ROOT / 'asm/us/boot_tail'
        self.words = list(words) if words is not None else score.targets()['func_8001E0E0']
        if words is None:
            assert len(self.words) == 216

    def run(self, volume, pan, span, aux, table, balance, scalars, aliases=(0, 1, 2, 3)):
        memory = {}
        def store(address, value, width):
            for i in range(width):
                memory[address + i] = (value >> (8 * (width - i - 1))) & 255
        def load(address, width):
            return int.from_bytes(bytes(memory[address + i] for i in range(width)), 'big')
        for address, values in [(0x8002CA40, table), (0x8002CC44, balance), (0x8002D910, scalars)]:
            for i, value in enumerate(values):
                store(address + 4 * i, bits(value), 4)
        out = 0x00200000
        for i in range(4):
            store(out + 2 * i, 0xACDC, 2)
        reg = [0] * 32
        fp = [0] * 32
        reg[29] = 0x00700000
        reg[31] = 0x00F00000
        reg[4:8] = [out + 2 * aliases[0], out + 2 * aliases[1], volume, pan]
        for i, value in enumerate([span, out + 2 * aliases[2], aux, out + 2 * aliases[3]]):
            store(reg[29] + 16 + 4 * i, value, 4)
        writes = []
        reads = set()
        def execute(pc, delay=False):
            assert BASE <= pc < BASE + len(self.words) * 4 and pc % 4 == 0
            word = self.words[(pc - BASE) // 4]
            op, rs, rt = word >> 26, (word >> 21) & 31, (word >> 16) & 31
            rd, shift, fn = (word >> 11) & 31, (word >> 6) & 31, word & 63
            imm = word & 65535
            si = imm if imm < 32768 else imm - 65536
            branch = None
            if op == 0:
                if fn == 0: reg[rd] = (reg[rt] << shift) & MASK
                elif fn == 2: reg[rd] = reg[rt] >> shift
                elif fn == 3: reg[rd] = signed(reg[rt]) >> shift & MASK
                elif fn == 8: branch = (True, reg[rs], False)
                elif fn == 33: reg[rd] = (reg[rs] + reg[rt]) & MASK
                elif fn == 35: reg[rd] = (reg[rs] - reg[rt]) & MASK
                elif fn == 36: reg[rd] = reg[rs] & reg[rt]
                elif fn == 37: reg[rd] = reg[rs] | reg[rt]
                elif fn == 43: reg[rd] = int(reg[rs] < reg[rt])
                else: raise AssertionError(('unsupported SPECIAL', fn))
            elif op == 1:
                assert rt in (0, 1)
                branch = ((signed(reg[rs]) < 0) if rt == 0 else (signed(reg[rs]) >= 0), pc + 4 + 4 * si, False)
            elif op in (4, 5, 20, 21):
                equal = reg[rs] == reg[rt]
                branch = (equal if op in (4, 20) else not equal, pc + 4 + 4 * si, op >= 20)
            elif op == 9: reg[rt] = (reg[rs] + si) & MASK
            elif op == 12: reg[rt] = reg[rs] & imm
            elif op == 13: reg[rt] = reg[rs] | imm
            elif op == 15: reg[rt] = imm << 16
            elif op in (35, 43, 41, 49, 53, 61):
                address = (reg[rs] + si) & MASK
                if op == 35: reg[rt] = load(address, 4)
                elif op == 43: store(address, reg[rt], 4)
                elif op == 41:
                    assert out <= address < out + 8 and address % 2 == 0
                    store(address, reg[rt], 2)
                    writes.append((address - out, reg[rt] & 65535))
                elif op == 49:
                    assert address % 4 == 0
                    fp[rt] = load(address, 4)
                    reads.add(address)
                elif op == 53:
                    fp[rt], fp[rt + 1] = load(address, 4), load(address + 4, 4)
                else:
                    store(address, fp[rt], 4)
                    store(address + 4, fp[rt + 1], 4)
            elif op == 17:
                if rs == 0: reg[rt] = fp[rd]
                elif rs == 4: fp[rd] = reg[rt]
                elif rs == 20:
                    assert fn == 32
                    fp[shift] = bits(float(signed(fp[rd])))
                elif rs == 16:
                    a, b = floating(fp[rd]), floating(fp[rt])
                    if fn == 0: fp[shift] = bits(a + b)
                    elif fn == 1: fp[shift] = bits(a - b)
                    elif fn == 2: fp[shift] = bits(a * b)
                    elif fn == 13:
                        assert math.isfinite(a) and -2147483648 <= math.trunc(a) <= 2147483647
                        fp[shift] = math.trunc(a) & MASK
                    else: raise AssertionError(('unsupported single operation', fn))
                else: raise AssertionError(('unsupported COP1 format', rs))
            else: raise AssertionError(('unsupported opcode', op))
            reg[0] = 0
            if delay:
                assert branch is None, 'control transfer in delay slot'
            return branch
        pc, steps = BASE, 0
        while pc != reg[31]:
            steps += 1
            assert steps < 400
            branch = execute(pc)
            if branch is None:
                pc += 4
            else:
                taken, target, likely = branch
                if taken or not likely:
                    execute(pc + 4, delay=True)
                pc = target if taken else pc + 8
        assert [offset for offset, _ in writes] == [2 * aliases[i] for i in (2, 0, 1, 3)]
        assert reg[29] == 0x00700000
        return [load(out + 2 * i, 2) for i in range(4)], writes, reads


def reference(volume, pan, span, aux, table, balance, scalars, aliases=(0, 1, 2, 3)):
    """Independent high-level binary32 model with explicit wrap and write order."""
    def interpolation(value, shift, data):
        i = value >> shift
        fraction = f32(float(value & ((1 << shift) - 1)) / float(1 << shift))
        return f32(f32(data[i + 1] * fraction) + f32(f32(1.0 - fraction) * data[i]))
    def product(*values):
        result = values[0]
        for value in values[1:]:
            result = f32(result * value)
        return math.trunc(result) & 65535
    def reflect(value):
        result = (0x800000 - value) & MASK
        return 0x7F0000 if result >= 0x800000 else result
    gain = interpolation(min(volume, 0x7F0000), 16, table)
    values = [0] * 4
    values[2] = product(gain, interpolation(span, 22, balance), scalars[1], scalars[0])
    gain = f32(gain * interpolation(reflect(span), 22, balance))
    if pan == 0x800000:
        values[0] = product(gain, scalars[2], scalars[0])
        values[1] = product(gain, scalars[3], scalars[0])
    else:
        values[0] = product(gain, interpolation(pan, 22, balance), scalars[0])
        values[1] = product(gain, interpolation(reflect(pan), 22, balance), scalars[0])
    values[3] = product(interpolation(min(aux, 0x7F0000), 16, table), scalars[4], scalars[0])
    output = [0xACDC] * 4
    writes = []
    for index in (2, 0, 1, 3):
        output[aliases[index]] = values[index]
        writes.append((2 * aliases[index], values[index]))
    return output, writes
