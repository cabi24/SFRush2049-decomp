"""Fail-closed MIPS-II caller execution with explicit O32 dependency hooks.

No game-engine or floating-point helper internals are simulated. Every caller
instruction, including delay slots and its sole FP load/store, executes.
"""
import struct

BASE = 0x8010E72C
SIZE = 252
ACTOR, NODE, OLD_HEAD, NEW_HEAD = 0x100000, 0x110000, 0x120000, 0x130000
STACK, RETURN = 0x200200, 0x70000000
ROWS, MODELS, TIME, HEAD = 0x8011753C, 0x80152818, 0x801249D0, 0x801391F0
ALLOC, BASIS, COPY, SOUND = 0x80090284, 0x8008B4C4, 0x8008D6B0, 0x800FEA00
MASK = 0xffffffff


def signed(x, bits=32):
    x &= (1 << bits) - 1
    return x - (1 << bits) if x >> (bits - 1) else x


class State:
    def __init__(self, case):
        self.case = case
        empty, index, player, flags, time, mutations, seed = case
        self.mem, self.events = {}, []
        for address, size in [(ACTOR, 96), (NODE, 24), (ROWS, 192),
                              (MODELS, 3808), (TIME, 4), (HEAD, 4),
                              (STACK - 160, 224)]:
            self.mem.update((address + i, 0xa5) for i in range(size))
        self.put(ACTOR + 16, 2, index)
        self.put(ACTOR + 92, 1, player)
        self.put(ACTOR + 4, 1, flags)
        self.put(NODE, 4, OLD_HEAD)
        self.put(NODE + 12, 4, 0)
        self.put(HEAD, 4, OLD_HEAD)
        self.put(TIME, 4, time)
        for i in range(4):
            self.put(ROWS + 48 * i, 4, (seed ^ (0x12340000 + i)))
            self.put(ROWS + 48 * i + 16, 4, (seed ^ (0x56780000 + i)))
            for j in range(3):
                self.put(MODELS + 952 * i + 20 + j * 4, 4,
                         (0x3f000000 + i * 0x100000 + j * 0x10000))
        self.before = dict(self.mem)
        self.matrix = None

    def get(self, address, width):
        assert address % width == 0
        assert all(address + i in self.mem for i in range(width)), hex(address)
        return int.from_bytes(bytes(self.mem[address + i] for i in range(width)), 'big')

    def put(self, address, width, value):
        assert address % width == 0
        assert all(address + i in self.mem for i in range(width)), hex(address)
        for i, byte in enumerate((value & ((1 << (8 * width)) - 1)).to_bytes(width, 'big')):
            self.mem[address + i] = byte

    def tag(self, pointer):
        return {0: 0, OLD_HEAD: 1, NEW_HEAD: 2, NODE: 3, ACTOR: 4}[pointer]

    def trace(self, call, args):
        self.events += [call, *args, self.get(NODE + 20, 4), self.get(NODE + 16, 4),
                        self.get(ACTOR + 4, 1), self.get(ACTOR + 90, 2),
                        self.get(ACTOR + 16, 2), self.get(ACTOR + 92, 1),
                        self.tag(self.get(HEAD, 4))]

    def mutation(self, phase):
        if self.case[5] & (1 << phase):
            self.put(ACTOR + 16, 2, (self.get(ACTOR + 16, 2) + 1) % 4)
            self.put(ACTOR + 92, 1, (self.get(ACTOR + 92, 1) + 1) % 4)
            self.put(ACTOR + 4, 1, self.get(ACTOR + 4, 1) ^ (0x80 >> phase))
            self.put(HEAD, 4, NEW_HEAD)
            if phase == 0:
                self.put(TIME, 4, self.get(TIME, 4) ^ 0x00800000)

    def external(self, address, args):
        if address == ALLOC:
            self.trace(1, [0, 0, 0, 0])
            self.mutation(0)
            return 0 if self.case[0] else NODE
        if address == BASIS:
            direction, destination = args[:2]
            player = (direction - MODELS - 20) // 952
            assert 0 <= player < 4 and direction == MODELS + 952 * player + 20
            assert STACK - 120 <= destination <= STACK - 36
            self.matrix = destination
            self.trace(2, [player, 1, 0, 0])
            for i in range(9):
                self.put(destination + 4 * i, 4, self.get(direction + 4 * (i % 3), 4) ^ (i << 12))
            self.mutation(1)
            return 0x11223344
        if address == COPY:
            assert args[:2] == [self.matrix, ACTOR + 20]
            self.trace(3, [1, 1, 0, 0])
            for i in range(9): self.put(ACTOR + 20 + i * 4, 4, self.get(self.matrix + i * 4, 4))
            self.mutation(2)
            return 0x22334455
        if address == SOUND:
            assert args[2] == ACTOR + 56 and args[3] == 2
            self.trace(4, [args[0], args[1], 1, args[3]])
            self.mutation(3)
            return self.case[6] ^ 0xdeadbeef
        raise AssertionError(('unknown external target', hex(address)))

    def output(self):
        out = [self.get(NODE + 4, 2), self.get(NODE + 20, 4), self.get(NODE + 16, 4),
               self.tag(self.get(NODE + 12, 4)), self.tag(self.get(NODE, 4)),
               self.tag(self.get(HEAD, 4)), self.get(ACTOR + 4, 1), self.get(ACTOR + 90, 2),
               self.get(ACTOR + 16, 2), self.get(ACTOR + 92, 1)]
        out += [self.get(ACTOR + 20 + 4 * i, 4) for i in range(12)]
        out += [len(self.events) // 12] + self.events
        out += [0] * (71 - len(out))
        allowed = set(range(ACTOR + 16, ACTOR + 18)) | {ACTOR + 4, ACTOR + 92}
        allowed |= set(range(ACTOR + 20, ACTOR + 56)) | set(range(ACTOR + 90, ACTOR + 92))
        allowed |= set(range(NODE, NODE + 6)) | set(range(NODE + 12, NODE + 24))
        allowed |= set(range(HEAD, HEAD + 4)) | set(range(TIME, TIME + 4))
        allowed |= set(range(STACK - 96, STACK))
        assert all(self.mem[a] == value for a, value in self.before.items() if a not in allowed)
        return out


class Machine(State):
    def __init__(self, words, case):
        super().__init__(case)
        assert len(words) == SIZE // 4
        self.words, self.visited, self.steps = words, set(), 0
        self.r = [0xa5000000 + i for i in range(32)]
        self.f = [0xdead0000 + i for i in range(32)]
        self.r[0], self.r[4], self.r[29], self.r[31] = 0, ACTOR, STACK, RETURN
        self.original = self.r[:]

    def reg(self, index, value):
        if index: self.r[index] = value & MASK

    def step(self, pc, delay=False):
        assert BASE <= pc < BASE + SIZE and pc % 4 == 0, hex(pc)
        self.steps += 1
        assert self.steps <= 100
        self.visited.add(pc - BASE)
        w = self.words[(pc - BASE) // 4]
        op, rs, rt, rd, shift, fn = w >> 26, w >> 21 & 31, w >> 16 & 31, w >> 11 & 31, w >> 6 & 31, w & 63
        imm, simm = w & 65535, signed(w, 16)
        a, b = self.r[rs], self.r[rt]
        branch, call = None, False
        if op == 0:
            if fn == 0: self.reg(rd, b << shift)
            elif fn == 33: self.reg(rd, a + b)
            elif fn == 35: self.reg(rd, a - b)
            elif fn == 37: self.reg(rd, a | b)
            elif fn == 8: branch = a
            else: raise AssertionError(('unknown SPECIAL', fn))
        elif op == 9: self.reg(rt, a + simm)
        elif op == 12: self.reg(rt, a & imm)
        elif op == 15: self.reg(rt, imm << 16)
        elif op == 4: branch = pc + 4 + 4 * simm if a == b else pc + 8
        elif op == 3:
            branch = ((pc + 4) & 0xf0000000) | ((w & 0x3ffffff) << 2)
            self.r[31], call = pc + 8, True
        elif op in (32, 33, 35, 36, 40, 41, 43, 49, 57):
            address = (a + simm) & MASK
            width = 1 if op in (32, 36, 40) else 2 if op in (33, 41) else 4
            if op in (32, 33, 35): self.reg(rt, signed(self.get(address, width), width * 8))
            elif op == 36: self.reg(rt, self.get(address, width))
            elif op == 49: self.f[rt] = self.get(address, width)
            else: self.put(address, width, self.f[rt] if op == 57 else b)
        else: raise AssertionError(('unknown opcode', op))
        if branch is not None:
            assert not delay, 'branch in delay slot'
            assert self.step(pc + 4, True) == pc + 8
            if call:
                result = self.external(branch, self.r[4:8])
                for i in [1, *range(2, 16), 24, 25]: self.r[i] = 0xbad00000 + i
                for i in range(20): self.f[i] = 0xbad10000 + i
                self.r[2] = result & MASK
                return self.r[31]
            return branch
        return pc + 4

    def run(self):
        pc = BASE
        while pc != RETURN: pc = self.step(pc)
        assert all(self.r[i] == self.original[i] for i in [*range(16, 24), 28, 29, 30])
        return self.output()


def oracle(case):
    state = State(case)
    node = state.external(ALLOC, [])
    if node:
        state.put(node + 4, 2, 0)
        row = ROWS + 48 * signed(state.get(ACTOR + 16, 2), 16)
        state.put(node + 20, 4, state.get(row, 4))
        state.put(node + 12, 4, ACTOR)
        state.put(node + 16, 4, state.get(TIME, 4))
        state.put(ACTOR + 90, 2, 4)
        state.put(ACTOR + 4, 1, state.get(ACTOR + 4, 1) & ~6)
        car = MODELS + 952 * signed(state.get(ACTOR + 92, 1), 8)
        state.external(BASIS, [car + 20, STACK - 60])
        state.external(COPY, [STACK - 60, ACTOR + 20])
        state.put(node, 4, state.get(HEAD, 4))
        state.put(HEAD, 4, node)
        row = ROWS + 48 * signed(state.get(ACTOR + 16, 2), 16)
        state.external(SOUND, [state.get(row + 16, 4), signed(state.get(ACTOR + 92, 1), 8), ACTOR + 56, 2])
    return state.output()
