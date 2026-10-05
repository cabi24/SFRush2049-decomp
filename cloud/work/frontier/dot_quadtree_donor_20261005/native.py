"""Small fail-closed integer MIPS interpreter for protected downleaf words.

Instruction bytes are read from the repository manifest or fresh linked objects,
never embedded. Fixtures require finite accessible tree paths and stable data.
"""
import struct
BASE = 0x800AC9BC
GLOBAL = 0x80124EEC
TABLE = 0x80200000
OUTPUT = 0x80280000
SP = 0x7FFF8000
RETURN = 0x70000000


def signed(value, bits=32):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value >> (bits - 1) else value


class Machine:
    def __init__(self, words, records, start, x, y, initial):
        self.words, self.memory = words, {}
        self.coverage, self.branches, self.writes = set(), set(), []
        self.r = [(0xA7159300 + i * 0x01020307) & 0xFFFFFFFF for i in range(32)]
        self.r[0] = 0
        self.r[4:8] = [TABLE + start * 20, x & 0xFFFFFFFF, y & 0xFFFFFFFF, OUTPUT]
        self.r[29], self.r[31] = SP, RETURN
        self.saved = list(self.r)
        self.lo = self.hi = 0
        self.put(TABLE, records)
        self.put(GLOBAL, struct.pack('>I', TABLE))
        self.put(OUTPUT, struct.pack('>h', initial))
        self.put(SP - 64, bytes((i * 29 + 71) & 255 for i in range(128)))
        self.original = dict(self.memory)

    def put(self, address, data):
        for i, byte in enumerate(data):
            assert address + i not in self.memory, 'fixture overlap'
            self.memory[address + i] = byte

    def read(self, address, width):
        assert address % width == 0, 'unaligned read'
        assert all(address + i in self.memory for i in range(width)), 'unmapped read'
        return int.from_bytes(bytes(self.memory[address + i] for i in range(width)), 'big')

    def write(self, address, width, value):
        assert (address, width) in ((SP + 4, 4), (SP + 8, 4), (OUTPUT, 2)), 'write outside contract'
        assert all(address + i in self.memory for i in range(width)), 'unmapped write'
        value &= (1 << (8 * width)) - 1
        self.writes.append((address, width, value))
        for i, byte in enumerate(value.to_bytes(width, 'big')):
            self.memory[address + i] = byte

    def word(self, pc):
        offset = pc - BASE
        assert offset % 4 == 0 and 0 <= offset < 4 * len(self.words), 'instruction outside body'
        self.coverage.add(offset)
        return self.words[offset // 4]

    def plain(self, word):
        r = self.r
        op, rs, rt, rd = word >> 26, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31
        imm, shift, fn = signed(word, 16), word >> 6 & 31, word & 63
        if op == 0:
            if fn == 0: r[rd] = r[rt] << shift
            elif fn == 3: r[rd] = signed(r[rt]) >> shift
            elif fn == 4: r[rd] = r[rt] << (r[rs] & 31)
            elif fn == 0x10: r[rd] = self.hi
            elif fn == 0x12: r[rd] = self.lo
            elif fn == 0x19:
                product = r[rs] * r[rt]
                self.lo, self.hi = product & 0xFFFFFFFF, product >> 32
            elif fn == 0x21: r[rd] = r[rs] + r[rt]
            elif fn == 0x23: r[rd] = r[rs] - r[rt]
            elif fn == 0x24: r[rd] = r[rs] & r[rt]
            elif fn == 0x25: r[rd] = r[rs] | r[rt]
            elif fn == 0x2A: r[rd] = int(signed(r[rs]) < signed(r[rt]))
            else: raise AssertionError('unsupported special opcode %x' % fn)
        elif op == 9: r[rt] = r[rs] + imm
        elif op == 15: r[rt] = (word & 0xFFFF) << 16
        elif op == 0x21: r[rt] = signed(self.read((r[rs] + imm) & 0xFFFFFFFF, 2), 16)
        elif op == 0x23: r[rt] = self.read((r[rs] + imm) & 0xFFFFFFFF, 4)
        elif op == 0x24: r[rt] = self.read((r[rs] + imm) & 0xFFFFFFFF, 1)
        elif op == 0x25: r[rt] = self.read((r[rs] + imm) & 0xFFFFFFFF, 2)
        elif op == 0x29: self.write((r[rs] + imm) & 0xFFFFFFFF, 2, r[rt])
        elif op == 0x2B: self.write((r[rs] + imm) & 0xFFFFFFFF, 4, r[rt])
        else: raise AssertionError('unsupported opcode %x' % op)
        for i in range(1, 32): r[i] &= 0xFFFFFFFF
        r[0] = 0

    def run(self):
        pc = BASE
        for _ in range(100000):
            if pc == RETURN: break
            word = self.word(pc)
            op, rs, rt = word >> 26, word >> 21 & 31, word >> 16 & 31
            if op in (4, 5, 0x14, 0x15) or op == 1:
                if op == 1:
                    assert rt in (0, 1, 2, 3), 'unknown regimm'
                    take = signed(self.r[rs]) >= 0 if rt & 1 else signed(self.r[rs]) < 0
                    likely = rt >= 2
                else:
                    take = self.r[rs] == self.r[rt] if op in (4, 0x14) else self.r[rs] != self.r[rt]
                    likely = op in (0x14, 0x15)
                self.branches.add((pc - BASE, take))
                destination = pc + 4 + 4 * signed(word, 16) if take else pc + 8
                if take or not likely: self.plain(self.word(pc + 4))
                pc = destination
            elif op == 0 and word & 63 == 8:
                destination = self.r[rs]
                self.plain(self.word(pc + 4))
                pc = destination
            else:
                self.plain(word)
                pc += 4
        else: raise AssertionError('execution step bound')
        expected = dict(self.original)
        for address, width, value in self.writes:
            for i, byte in enumerate(value.to_bytes(width, 'big')): expected[address + i] = byte
        assert self.memory == expected, 'unexpected memory change'
        assert self.writes[:2] == [(SP + 4, 4, self.saved[5]), (SP + 8, 4, self.saved[6])], 'formal homes'
        assert all(self.r[i] == self.saved[i] for i in list(range(16, 24)) + [28, 29, 30, 31]), 'O32 preservation'
        result = self.r[2]
        assert result == 0 or (result - TABLE) % 20 == 0, 'invalid result alignment'
        return (-1 if result == 0 else (result - TABLE) // 20, signed(self.read(OUTPUT, 2), 16))
