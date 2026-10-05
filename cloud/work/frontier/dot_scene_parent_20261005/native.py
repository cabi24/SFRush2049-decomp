"""Bounded integer MIPS execution for the actual protected parent-search body.

No instruction bytes are embedded. Unknown instructions and unmapped accesses
fail closed. Tests supply either manifest-verified native or GNU-linked words.
"""
import struct

BASE = 0x800A7BF8
TABLE = 0x8012E700
COUNT = 0x80156990
SP = 0x7FFF8000
RETURN = 0x70000000
STRIDE = 68


def signed(value, bits=32):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value >> (bits - 1) else value


class Machine:
    def __init__(self, words, records, count, argument):
        self.words = words
        self.memory = {}
        self.coverage = set()
        self.branches = set()
        self.registers = [(0xA7159300 + n * 0x01020307) & 0xFFFFFFFF for n in range(32)]
        self.registers[0] = 0
        self.registers[4] = argument & 0xFFFFFFFF
        self.registers[29] = SP
        self.registers[31] = RETURN
        self.original_registers = list(self.registers)
        self.lo = self.hi = 0
        self.put(TABLE, records)
        self.put(COUNT, struct.pack('>i', count))
        self.put(SP - 64, bytes((n * 29 + 71) & 255 for n in range(128)))
        self.original_memory = dict(self.memory)
        self.argument = argument & 0xFFFFFFFF
        self.writes = []

    def put(self, address, data):
        for i, value in enumerate(data):
            assert address + i not in self.memory, 'fixture overlap'
            self.memory[address + i] = value

    def read(self, address, width):
        assert address % width == 0, 'unaligned read'
        assert all(address + i in self.memory for i in range(width)), 'unmapped read'
        return int.from_bytes(bytes(self.memory[address + i] for i in range(width)), 'big')

    def write(self, address, width, value):
        assert address % width == 0, 'unaligned write'
        assert all(address + i in self.memory for i in range(width)), 'unmapped write'
        assert address == SP and width == 4, 'write outside argument home'
        self.writes.append((address, width, value & 0xFFFFFFFF))
        for i, byte in enumerate((value & ((1 << (width * 8)) - 1)).to_bytes(width, 'big')):
            self.memory[address + i] = byte

    def word(self, pc):
        offset = pc - BASE
        assert offset % 4 == 0 and 0 <= offset < len(self.words) * 4, 'instruction outside body'
        self.coverage.add(offset)
        return self.words[offset // 4]

    def plain(self, word):
        r = self.registers
        op, rs, rt, rd = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31
        imm, shift, fn = signed(word, 16), (word >> 6) & 31, word & 63
        if op == 0:
            if fn == 0: r[rd] = r[rt] << shift
            elif fn == 3: r[rd] = signed(r[rt]) >> shift
            elif fn == 0x10: r[rd] = self.hi
            elif fn == 0x12: r[rd] = self.lo
            elif fn == 0x19:
                product = r[rs] * r[rt]
                self.lo, self.hi = product & 0xFFFFFFFF, product >> 32
            elif fn == 0x21: r[rd] = r[rs] + r[rt]
            elif fn == 0x25: r[rd] = r[rs] | r[rt]
            elif fn == 0x2A: r[rd] = int(signed(r[rs]) < signed(r[rt]))
            else: raise AssertionError('unsupported special opcode %x' % fn)
        elif op == 9: r[rt] = r[rs] + imm
        elif op == 15: r[rt] = (word & 0xFFFF) << 16
        elif op == 0x21: r[rt] = signed(self.read((r[rs] + imm) & 0xFFFFFFFF, 2), 16)
        elif op == 0x23: r[rt] = self.read((r[rs] + imm) & 0xFFFFFFFF, 4)
        elif op == 0x2B: self.write((r[rs] + imm) & 0xFFFFFFFF, 4, r[rt])
        else: raise AssertionError('unsupported opcode %x' % op)
        for i in range(1, 32): r[i] &= 0xFFFFFFFF
        r[0] = 0

    def run(self):
        pc = BASE
        for _ in range(2000000):
            if pc == RETURN: break
            word = self.word(pc)
            op, rs, rt = word >> 26, (word >> 21) & 31, (word >> 16) & 31
            control = False
            if op in (4, 5, 6, 7, 0x14, 0x15):
                control = True
                a, b = self.registers[rs], self.registers[rt]
                take = (a == b if op in (4, 0x14) else a != b if op in (5, 0x15)
                        else signed(a) <= 0 if op == 6 else signed(a) > 0)
                self.branches.add((pc - BASE, take))
                destination = pc + 4 + 4 * signed(word, 16) if take else pc + 8
                if take or op not in (0x14, 0x15): self.plain(self.word(pc + 4))
                pc = destination
            elif op == 0 and word & 63 == 8:
                control = True
                destination = self.registers[rs]
                self.plain(self.word(pc + 4))
                pc = destination
            if not control:
                self.plain(word)
                pc += 4
        else: raise AssertionError('execution step bound')
        expected = dict(self.original_memory)
        for i, byte in enumerate(struct.pack('>I', self.argument)): expected[SP + i] = byte
        assert self.memory == expected, 'memory preservation'
        assert self.writes == [(SP, 4, self.argument)], 'argument-home write contract'
        assert all(self.registers[i] == self.original_registers[i] for i in list(range(16, 24)) + [28, 29, 30, 31]), 'O32 preservation'
        return signed(self.registers[2])
