"""Small fail-closed MIPS-II interpreter for the complete callback body.

Instructions are read only from manifest-verified targets or GNU-linked output.
The external UpdateBlit call is an adversarial audited O32 mock, not gameplay.
"""
MASK = 0xffffffff
BASE, SIZE = 0x8010b560, 112
BLIT, STACK, RETURN = 0x100000, 0x200100, 0x70000000
GLOBAL, IMAGE, UPDATE = 0x80149d98, 0x80117358, 0x80094ec8
INITIAL_IMAGE, CHANGED_IMAGE = 0x12345600, 0x12345604
INITIAL_CALLBACK, CHANGED_CALLBACK = 0x400000, 0x400004


def signed(value, bits=32):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


class Machine:
    def __init__(self, words, args):
        assert len(words) == SIZE // 4
        self.words, self.args = words, args
        self.mem, self.events, self.visited, self.reads = {}, [], set(), []
        self.steps, self.calls = 0, 0
        self.r = [0xa5000000 + i for i in range(32)]
        self.r[0], self.r[4], self.r[29], self.r[31] = 0, BLIT, STACK, RETURN
        self.original = list(self.r)
        for base, size in [(BLIT, 56), (STACK - 48, 80), (GLOBAL, 4)]:
            for offset in range(size): self.mem[base + offset] = 0x5a
        self.put(BLIT + 26, 1, args[1])
        self.put(BLIT + 4, 4, INITIAL_IMAGE)
        self.put(BLIT + 40, 4, INITIAL_CALLBACK)
        self.put(GLOBAL, 4, args[0])
        self.before = dict(self.mem)

    def get(self, address, width):
        assert address % width == 0 and all(address + i in self.mem for i in range(width)), hex(address)
        return int.from_bytes(bytes(self.mem[address + i] for i in range(width)), 'big')

    def put(self, address, width, value):
        assert address % width == 0 and all(address + i in self.mem for i in range(width)), hex(address)
        for i, byte in enumerate((value & ((1 << (width * 8)) - 1)).to_bytes(width, 'big')):
            self.mem[address + i] = byte

    def hide(self): return signed(self.get(BLIT + 26, 1), 8)
    def image(self): return {INITIAL_IMAGE: 0, IMAGE: 1, CHANGED_IMAGE: 2}[self.get(BLIT + 4, 4)]
    def callback(self): return {0: 0, INITIAL_CALLBACK: 1, CHANGED_CALLBACK: 2}[self.get(BLIT + 40, 4)]

    def external(self):
        assert self.r[4] == BLIT and self.calls < 2
        self.events += [self.hide(), self.image(), self.callback(), signed(self.get(GLOBAL, 4))]
        hide = self.args[2 + self.calls]
        self.calls += 1
        if hide != 256: self.put(BLIT + 26, 1, hide)
        if self.args[4] & 1:
            self.put(BLIT + 4, 4, CHANGED_IMAGE)
            self.put(BLIT + 40, 4, CHANGED_CALLBACK)
        if self.args[4] & 2: self.put(GLOBAL, 4, int(self.get(GLOBAL, 4) == 0))
        for i in [1, *range(2, 16), 24, 25]: self.r[i] = 0xbad00000 + i

    def write_reg(self, register, value):
        if register: self.r[register] = value & MASK

    def step(self, pc, delay=False):
        assert BASE <= pc < BASE + SIZE and pc % 4 == 0, hex(pc)
        self.visited.add(pc - BASE); self.steps += 1; assert self.steps < 60
        word = self.words[(pc - BASE) // 4]
        op, rs, rt, rd, shift, fn = word >> 26, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31, word >> 6 & 31, word & 63
        imm = word & 65535; simm = signed(imm, 16); a, b = self.r[rs], self.r[rt]
        branch = None
        if op == 0:
            if fn == 0: self.write_reg(rd, b << shift)
            elif fn == 43: self.write_reg(rd, int(a < b))
            elif fn == 8: branch = a
            else: raise AssertionError(('unknown SPECIAL', fn))
        elif op == 9: self.write_reg(rt, a + simm)
        elif op == 15: self.write_reg(rt, imm << 16)
        elif op == 4: branch = pc + 4 + 4 * simm if a == b else pc + 8
        elif op == 3:
            branch = ((pc + 4) & 0xf0000000) | ((word & 0x3ffffff) << 2)
            self.r[31] = pc + 8
        elif op in (32, 35, 40, 43):
            address = (a + simm) & MASK; width = 1 if op in (32, 40) else 4
            if op < 40:
                self.reads.append((address, width))
                self.write_reg(rt, signed(self.get(address, width), width * 8))
            else: self.put(address, width, b)
        else: raise AssertionError(('unknown opcode', op))
        if branch is not None:
            assert not delay, 'branch in delay slot'
            assert self.step(pc + 4, True) == pc + 8
            if branch == UPDATE: self.external(); return self.r[31]
            return branch
        return pc + 4

    def run(self):
        pc = BASE
        while pc != RETURN: pc = self.step(pc)
        assert self.r[29] == self.original[29]
        assert self.r[16:24] == self.original[16:24] and self.r[28] == self.original[28] and self.r[30] == self.original[30]
        assert self.reads.count((GLOBAL, 4)) == 1
        changed = {*range(BLIT + 4, BLIT + 8), BLIT + 26, *range(BLIT + 40, BLIT + 44),
                   *range(GLOBAL, GLOBAL + 4), *range(STACK - 4, STACK + 4)}
        assert all(self.mem[a] == value for a, value in self.before.items() if a not in changed)
        out = [signed(self.r[2]), self.hide(), self.image(), self.callback(), signed(self.get(GLOBAL, 4)), self.calls] + self.events
        return out + [0] * (14 - len(out))


def reference(args):
    global_value, hide, first, second, mutation = args
    image, callback, calls, events = 0, 1, 0, []
    def update():
        nonlocal global_value, hide, image, callback, calls
        events.extend([hide, image, callback, global_value])
        replacement = (first, second)[calls]; calls += 1
        if replacement != 256: hide = replacement
        if mutation & 1: image, callback = 2, 2
        if mutation & 2: global_value = int(not global_value)
    desired = int(global_value != 0)
    if hide != desired:
        hide = desired
        update()
    if hide == 0:
        image = 1
        update()
        callback = 0
    out = [1, hide, image, callback, global_value, calls] + events
    return out + [0] * (14 - len(out))
