"""Bounded MIPS-II execution for the stored-name counterexample.

Only authenticated current-tree instruction words are executed. This is a
single-threaded logical-RDRAM fixture, not an emulator/gameplay verdict.
Unknown instructions, unmapped memory and calls outside the declared hooks
fail closed. Branch-likely annulment and delay slots are modeled explicitly.
"""
import struct

MASK = 0xffffffff
RETURN = 0x70000000
STACK = 0x803f2000


def require(value, message):
    if not value:
        raise ValueError(message)


def signed(value, bits=32):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value >> (bits - 1) else value


class Memory:
    def __init__(self, game):
        self.banks = {0x80086a50: bytearray(game), 0x80124a00: bytearray(0x5b600),
                      0x80200000: bytearray(0x4000), 0x803b0000: bytearray(0x10000),
                      0x803f0000: bytearray(0x4000)}
        self.writes = []
        self.protected = [(0x80123c20, 0x80123c38)]

    def locate(self, address, size, alignment=1):
        require(address % alignment == 0, 'unaligned memory access')
        for start, bank in self.banks.items():
            if start <= address and address + size <= start + len(bank):
                return bank, address - start
        raise ValueError('unmapped memory: ' + hex(address))

    def read(self, address, size):
        bank, offset = self.locate(address, size)
        return bytes(bank[offset:offset + size])

    def write(self, address, value):
        bank, offset = self.locate(address, len(value))
        require(not any(address < end and start < address + len(value) for start, end in self.protected), 'write to selected native code or owned table')
        bank[offset:offset + len(value)] = value
        self.writes.append((address, len(value)))

    def get(self, address, size):
        self.locate(address, size, size)
        return int.from_bytes(self.read(address, size), 'big')

    def put(self, address, value, size):
        self.locate(address, size, size)
        self.write(address, (value & ((1 << (8 * size)) - 1)).to_bytes(size, 'big'))

    def string(self, address, limit=128):
        for length in range(limit):
            if self.get(address + length, 1) == 0:
                return self.read(address, length)
        raise ValueError('unterminated fixture string')


class Machine:
    def __init__(self, bodies, memory, hooks):
        self.code = {address + i * 4: word for address, words in bodies.items()
                     for i, word in enumerate(words)}
        require(len(self.code) == sum(map(len, bodies.values())), 'overlapping native code extents')
        self.memory = memory
        memory.protected += [(a, a + len(words)*4) for a, words in bodies.items()]
        self.hooks = hooks
        self.coverage = set()
        self.calls = []
        self.steps = 0

    def run(self, address, registers=None, budget=250000):
        r = [0] * 32
        f = [0] * 32
        r[29], r[31] = STACK, RETURN
        for register, value in (registers or {}).items():
            r[register] = value & MASK
        pc, pending, lo, hi, condition = address, None, 0, 0, False
        local_steps = 0
        while pc != RETURN:
            require(local_steps < budget, 'native execution budget exceeded')
            require(pc in self.code, 'PC outside selected authenticated functions: ' + hex(pc))
            self.coverage.add(pc)
            local_steps += 1
            w = self.code[pc]
            op, rs, rt, rd, shift, fn = w >> 26, w >> 21 & 31, w >> 16 & 31, w >> 11 & 31, w >> 6 & 31, w & 63
            imm = signed(w, 16)
            a = (r[rs] + imm) & MASK
            prior, pending, next_pc = pending, None, pc + 4
            if op == 0:
                if fn == 0: r[rd] = r[rt] << shift & MASK
                elif fn == 2: r[rd] = r[rt] >> shift
                elif fn == 3: r[rd] = signed(r[rt]) >> shift & MASK
                elif fn == 0x21: r[rd] = (r[rs] + r[rt]) & MASK
                elif fn == 0x23: r[rd] = (r[rs] - r[rt]) & MASK
                elif fn == 0x24: r[rd] = r[rs] & r[rt]
                elif fn == 0x25: r[rd] = r[rs] | r[rt]
                elif fn == 0x26: r[rd] = r[rs] ^ r[rt]
                elif fn == 0x2a: r[rd] = int(signed(r[rs]) < signed(r[rt]))
                elif fn == 0x2b: r[rd] = int(r[rs] < r[rt])
                elif fn == 0x19:
                    product = r[rs] * r[rt]
                    lo, hi = product & MASK, product >> 32
                elif fn == 0x1b:
                    require(r[rt] != 0, 'native zero division')
                    lo, hi = r[rs] // r[rt], r[rs] % r[rt]
                elif fn == 0x12: r[rd] = lo
                elif fn == 0x10: r[rd] = hi
                elif fn == 8:
                    require(prior is None, 'jump in delay slot')
                    pending = r[rs]
                else: raise ValueError('unsupported SPECIAL: ' + str(fn))
            elif op == 9: r[rt] = a
            elif op == 0xa: r[rt] = int(signed(r[rs]) < imm)
            elif op == 0xb: r[rt] = int(r[rs] < (imm & MASK))
            elif op == 0xc: r[rt] = r[rs] & (w & 65535)
            elif op == 0xd: r[rt] = r[rs] | (w & 65535)
            elif op == 0xf: r[rt] = (w & 65535) << 16
            elif op in (0x20, 0x21, 0x23, 0x24, 0x25):
                n = {0x20:1, 0x21:2, 0x23:4, 0x24:1, 0x25:2}[op]
                value = self.memory.get(a, n)
                r[rt] = (signed(value, n * 8) if op in (0x20, 0x21) else value) & MASK
            elif op in (0x28, 0x29, 0x2b):
                self.memory.put(a, r[rt], {0x28:1, 0x29:2, 0x2b:4}[op])
            elif op in (0x31, 0x39):
                if op == 0x31: f[rt] = self.memory.get(a, 4)
                else: self.memory.put(a, f[rt], 4)
            elif op == 0x11:
                if rs == 4: f[rd] = r[rt]
                elif rs == 16 and fn in (0x32, 0x3c):
                    left = struct.unpack('>f', f[rd].to_bytes(4, 'big'))[0]
                    right = struct.unpack('>f', f[rt].to_bytes(4, 'big'))[0]
                    condition = left == right if fn == 0x32 else left < right
                elif rs == 8:
                    require(prior is None, 'FP branch in delay slot')
                    require(rt in (0, 1, 2, 3), 'unsupported FP branch')
                    take = condition if rt & 1 else not condition
                    if not take and rt & 2: next_pc = pc + 8
                    else: pending = pc + 4 + imm * 4 if take else pc + 8
                else: raise ValueError('unsupported COP1')
            elif op in (4, 5, 6, 7, 0x14, 0x15, 0x16, 0x17, 1):
                require(prior is None, 'branch in delay slot')
                if op in (4, 0x14): take = r[rs] == r[rt]
                elif op in (5, 0x15): take = r[rs] != r[rt]
                elif op in (6, 0x16): take = signed(r[rs]) <= 0
                elif op in (7, 0x17): take = signed(r[rs]) > 0
                else:
                    require(rt in (0, 1), 'unsupported REGIMM')
                    take = signed(r[rs]) < 0 if rt == 0 else signed(r[rs]) >= 0
                if not take and op in (0x14, 0x15, 0x16, 0x17): next_pc = pc + 8
                else: pending = pc + 4 + imm * 4 if take else pc + 8
            elif op in (2, 3):
                require(prior is None, 'call or jump in delay slot')
                target = (pc + 4) & 0xf0000000 | (w & 0x3ffffff) << 2
                if op == 3:
                    r[31] = pc + 8
                    self.calls.append((pc, target))
                pending = target
            else: raise ValueError('unsupported opcode: ' + str(op))
            r[0] = 0
            if prior is not None:
                require(pending is None, 'control transfer inside delay slot')
                if prior in self.hooks:
                    self.hooks[prior](self.memory, r)
                    next_pc = r[31]
                else:
                    next_pc = prior
            pc = next_pc
        require(pending is None and r[29] == STACK, 'unbalanced native return')
        self.steps += local_steps
        return r[2]
