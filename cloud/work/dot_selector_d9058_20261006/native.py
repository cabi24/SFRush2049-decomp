"""Fail-closed D9058 caller interpreter; no native instructions are embedded.

External calls are explicit contract hooks. This does not execute the queues,
selector, bank loader, or scheduler. ABI-specific hook inputs are kept separate.
"""
BASE = 0x800D9058
STACK = 0x200100
STOP = 0x300000
INDEX = 0x80151AD0
HEIGHT = 0x80113E8C
LINES = 0x8014A10A
FLAGS = 0x801174B4
QUEUE = 0x801461D0
RECV = 0x80007270
SLOT = 0x800B4200
JAM = 0x800075E0
MASK = 0xFFFFFFFF


def signed(v, bits=32):
    v &= (1 << bits) - 1
    return v - (1 << bits) if v & (1 << (bits - 1)) else v


def oracle(height, flags, mode):
    numerator = signed((height & MASK) - 24)
    quotient = abs(numerator) // 16 * (-1 if numerator < 0 else 1)
    lines = signed(quotient, 16)
    if mode == 2:
        flags |= 2
    elif mode == 3:
        flags = 0
    return 13 if lines > 13 or flags & 0x7C03FFFE else lines


class Machine:
    def __init__(self, words, height, index, flags, mode, selector_reg=18):
        self.words = words
        self.case = height, index, flags, mode
        self.selector_reg = selector_reg
        self.pc = BASE
        self.pending = None
        self.mem = {}
        for a, n in [(STACK - 64, 128), (INDEX, 2),
                     (HEIGHT + index * 96, 4), (LINES, 2), (FLAGS, 4)]:
            self.mem.update({a + i: 0 for i in range(n)})
        self.put(INDEX, 2, index)
        self.put(HEIGHT + index * 96, 4, height)
        self.put(FLAGS, 4, flags)
        self.put(LINES, 2, -1777)
        self.r = [(0x5ABC0000 + i * 977) & MASK for i in range(32)]
        self.r[0] = 0
        self.r[29] = STACK
        self.r[31] = STOP
        self.entry = list(self.r)
        self.events = []
        self.visited = set()
        self.branches = {}
        self.writes = []

    def get(self, a, n):
        assert a % n == 0 and all(a + i in self.mem for i in range(n)), hex(a)
        return int.from_bytes(bytes(self.mem[a + i] for i in range(n)), 'big')

    def put(self, a, n, v):
        assert a % n == 0 and all(a + i in self.mem for i in range(n)), hex(a)
        for i, b in enumerate((v & ((1 << (8 * n)) - 1)).to_bytes(n, 'big')):
            self.mem[a + i] = b

    def hook(self):
        height, index, flags, mode = self.case
        if self.pc == RECV:
            assert not self.events and self.r[4:7] == [QUEUE, 0, 1]
            self.events.append('lock')
            if mode:
                self.put(INDEX, 2, index + 1)
                self.put(HEIGHT + index * 96, 4, height ^ 0x12345678)
        elif self.pc == SLOT:
            assert self.events == ['lock'] and self.r[self.selector_reg] == 11
            self.events.append('select_11')
            if mode == 2:
                self.put(FLAGS, 4, flags | 2)
        elif self.pc == JAM:
            assert self.events == ['lock', 'select_11']
            assert self.r[4:7] == [QUEUE, 0, 0]
            self.events.append('unlock')
            if mode == 3:
                self.put(FLAGS, 4, 0)
        else:
            return False
        ret = self.r[31]
        scratch = [1, 2, 3, *range(4, 16), 24, 25]
        if self.pc == SLOT and self.selector_reg != 4:
            scratch += [r for r in range(16, 20) if r != self.selector_reg]
        for r in scratch:
            self.r[r] = (0xABC00000 + r * 91 + len(self.events)) & MASK
        for i in range(16):
            self.put(self.r[29] + i, 1, 0xB0 + i)
        self.r[2] = (height ^ flags ^ (mode * 0x12345)) & MASK
        self.pc = ret
        return True

    def run(self):
        for _ in range(1000):
            if self.pc == STOP:
                break
            if self.hook():
                continue
            at = self.pc
            assert BASE <= at < BASE + len(self.words) * 4 and (at - BASE) % 4 == 0
            self.visited.add(at - BASE)
            w = self.words[(at - BASE) // 4]
            op, rs, rt, rd, sh, fn = w >> 26, w >> 21 & 31, w >> 16 & 31, w >> 11 & 31, w >> 6 & 31, w & 63
            imm = w & 65535
            off = signed(imm, 16)
            branch, skip = None, False
            if op == 0:
                if fn == 0:
                    self.r[rd] = self.r[rt] << sh & MASK
                elif fn == 3:
                    self.r[rd] = signed(self.r[rt]) >> sh & MASK
                elif fn == 0x21:
                    self.r[rd] = (self.r[rs] + self.r[rt]) & MASK
                elif fn == 0x23:
                    self.r[rd] = (self.r[rs] - self.r[rt]) & MASK
                elif fn == 0x24:
                    self.r[rd] = self.r[rs] & self.r[rt]
                elif fn == 0x25:
                    self.r[rd] = self.r[rs] | self.r[rt]
                elif fn == 0x2A:
                    self.r[rd] = int(signed(self.r[rs]) < signed(self.r[rt]))
                elif fn == 0x2B:
                    self.r[rd] = int(self.r[rs] < self.r[rt])
                elif fn == 8:
                    branch = self.r[rs]
                else:
                    raise AssertionError(('special', at, fn))
            elif op == 3:
                branch = ((at + 4) & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
                assert branch in (RECV, SLOT, JAM)
                self.r[31] = at + 8
            elif op == 9:
                self.r[rt] = (self.r[rs] + off) & MASK
            elif op == 13:
                self.r[rt] = self.r[rs] | imm
            elif op == 15:
                self.r[rt] = imm << 16
            elif op in (1, 4, 5, 20, 21):
                if op == 1:
                    assert rt == 1
                    cond = signed(self.r[rs]) >= 0
                elif op in (4, 20):
                    cond = self.r[rs] == self.r[rt]
                else:
                    cond = self.r[rs] != self.r[rt]
                self.branches.setdefault(at - BASE, set()).add(cond)
                if cond:
                    branch = at + 4 + 4 * off
                elif op in (20, 21):
                    skip = True
            elif op in (33, 35):
                v = self.get((self.r[rs] + off) & MASK, 2 if op == 33 else 4)
                self.r[rt] = signed(v, 16) & MASK if op == 33 else v
            elif op in (41, 43):
                a = (self.r[rs] + off) & MASK
                n = 2 if op == 41 else 4
                assert a == LINES and n == 2 or STACK - 64 <= a and a + n <= STACK + 16
                self.writes.append((at - BASE, a, n))
                self.put(a, n, self.r[rt])
            else:
                raise AssertionError(('opcode', at, op))
            old = self.pending
            self.pending = branch
            self.pc = old if old is not None else at + (8 if skip else 4)
            self.r[0] = 0
        else:
            raise AssertionError('step bound exceeded')
        height, index, flags, mode = self.case
        assert signed(self.get(LINES, 2), 16) == oracle(height, flags, mode)
        assert self.events == ['lock', 'select_11', 'unlock']
        # Native private ABI may overwrite s0-s5; ordinary/group entry must
        # preserve the saved-register set after its ABI-specific hook.
        preserve = [22, 23, 28, 29, 30, 31]
        if self.selector_reg != 18:
            preserve += list(range(16, 22))
        assert all(self.r[r] == self.entry[r] for r in preserve)
        return self.visited, self.branches
