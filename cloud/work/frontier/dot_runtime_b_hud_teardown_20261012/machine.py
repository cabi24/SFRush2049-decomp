"""Bounded field oracle and MIPS II interpreter, reused from the prior teardown packet.

No native words are embedded. Helpers are synchronous state-mutating hooks,
not implementations. Nonstack snapshots include every mapped record byte.
"""
SYMBOLS = {'D_80395ED4': 0x80395ED4}
ADDRESS = 0x8039244C
BASE, END, COUNT, SP, RETURN = 0x803950C0, 0x80395ED8, 0x80151AD0, 0x70001000, 0x60000000
U32 = 0xffffffff
COUNTS = [-32768, -1, 0, 1, 2, 3, 4]

def check(ok, message='machine proof failed'):
    if not ok:
        raise ValueError(message)

def signed(x, n=32):
    x &= (1 << n) - 1
    return x - (1 << n) if x & (1 << (n - 1)) else x


class Memory:
    def __init__(self, other=None):
        self.banks = {BASE: bytearray(END - BASE), COUNT: bytearray(2), SP - 64: bytearray(64)} if other is None else {a: bytearray(b) for a, b in other.banks.items()}

    def locate(self, address, width):
        check(address % width == 0, ('unaligned access', address, width))
        for base, data in self.banks.items():
            if base <= address and address + width <= base + len(data):
                return data, address - base
        raise ValueError(('unmapped access', hex(address), width))

    def get(self, address, width):
        data, offset = self.locate(address, width)
        return int.from_bytes(data[offset:offset + width], 'big')

    def put(self, address, value, width):
        data, offset = self.locate(address, width)
        data[offset:offset + width] = (value & ((1 << (width * 8)) - 1)).to_bytes(width, 'big')

    def snapshot(self):
        return bytes(self.banks[BASE]), bytes(self.banks[COUNT])


def fixture(seed, count, resource):
    m = Memory()
    state = seed + 1
    for data in m.banks.values():
        for i in range(len(data)):
            state = (state * 1664525 + 1013904223) & U32
            data[i] = state >> 24
    m.put(COUNT, count, 2)
    m.put(SYMBOLS['D_80395ED4'], 0x50001000 if resource else 0, 4)
    for player in range(4):
        addresses = [0x80395C02 + player * 52, 0x80395CD2 + player * 52, 0x80395DA0 + player * 52]
        addresses += [BASE + player * 720 + i * 72 + 2 for i in range(10)]
        for i, address in enumerate(addresses):
            values = [-1, 0, 1, -32768, 32767, 0x1234]
            m.put(address, values[(seed + player * 3 + i) % len(values)], 2)
    return m


def hook(memory, address, argument, scenario, events):
    check(address in [0x800B358C, 0x80090254], 'unknown helper')
    if address == 0x80090254:
        check(argument == (signed(argument, 16) & U32), 'handle must be sign extended')
    events.append((address, argument, memory.snapshot()))
    ordinal = len(events)
    if scenario == 1 and ordinal == 1:
        memory.put(COUNT, 4, 2)
    elif scenario == 2 and ordinal == 1:
        memory.put(COUNT, 0, 2)
    elif scenario == 3:
        memory.put(0x80395ED4, 0x50002000, 4)
        memory.put(0x80395CD2 + (ordinal % 4) * 52, 32000 - ordinal, 2)
        memory.put(BASE + (ordinal % 40) * 72 + 8, ordinal * 31, 4)
    elif scenario == 4:
        memory.put(COUNT, [4, 1, 3, 2, 0][ordinal % 5], 2)
        memory.put(0x80395C00 + (ordinal % 4) * 52, -ordinal, 2)
    elif scenario == 5 and ordinal == 1:
        memory.put(COUNT, 2, 2)


def oracle(original, scenario):
    memory, events = Memory(original), []
    def release(address):
        value = signed(memory.get(address, 2), 16)
        if value != -1:
            hook(memory, 0x80090254, value & U32, scenario, events)
            memory.put(address, -1, 2)
    pointer = memory.get(0x80395ED4, 4)
    if pointer:
        hook(memory, 0x800B358C, pointer, scenario, events)
        memory.put(0x80395ED4, 0, 4)
    if signed(memory.get(COUNT, 2), 16) > 0:
        player = 0
        while True:
            check(player < 4, 'fixture domain escaped')
            release(0x80395C02 + player * 52)
            memory.put(0x80395C00 + player * 52, 8, 2)
            release(0x80395CD2 + player * 52)
            memory.put(0x80395CD0 + player * 52, 0, 2)
            release(0x80395DA0 + player * 52)
            for i in range(10):
                p = BASE + player * 720 + i * 72
                release(p + 2)
                memory.put(p, 11, 2)
            player += 1
            if player >= signed(memory.get(COUNT, 2), 16):
                break
    return memory.snapshot(), events


def execute(words, original, scenario):
    """Bounded MIPS II execution with explicit O32 helper hooks and delay slots."""
    memory, events, covered, branches = Memory(original), [], set(), set()
    regs = [(0x13579bdf * (i + 1)) & U32 for i in range(32)]
    regs[0], regs[29], regs[31] = 0, SP, RETURN
    initial = regs[:]
    pc, pending = 0, None
    for step in range(20000):
        if pc == RETURN:
            check(pending is None, 'return pending')
            for r in list(range(16, 24)) + [28, 29, 30, 31]:
                check(regs[r] == initial[r], ('preserved register', r))
            return memory.snapshot(), events, covered, branches
        check(pc % 4 == 0 and 0 <= pc < len(words) * 4, ('bad PC', pc))
        covered.add(pc)
        w = words[pc // 4]
        op, rs, rt, rd = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        imm, shift, fn = signed(w, 16), (w >> 6) & 31, w & 63
        address = (regs[rs] + imm) & U32
        current, pending = pending, None
        nextpc = pc + 4
        if op == 0:
            if fn == 0: regs[rd] = (regs[rt] << shift) & U32
            elif fn == 0x21: regs[rd] = (regs[rs] + regs[rt]) & U32
            elif fn == 0x23: regs[rd] = (regs[rs] - regs[rt]) & U32
            elif fn == 0x25: regs[rd] = regs[rs] | regs[rt]
            elif fn == 0x2b: regs[rd] = int(regs[rs] < regs[rt])
            elif fn == 8:
                check(rs == 31 and regs[rs] == RETURN, 'bad return')
                pending = ('jump', RETURN)
            else: raise ValueError(('unsupported SPECIAL', fn))
        elif op == 9: regs[rt] = (regs[rs] + imm) & U32
        elif op == 15: regs[rt] = (w & 0xffff) << 16
        elif op == 0x21: regs[rt] = signed(memory.get(address, 2), 16) & U32
        elif op == 0x23: regs[rt] = memory.get(address, 4)
        elif op == 0x29: memory.put(address, regs[rt], 2)
        elif op == 0x2b: memory.put(address, regs[rt], 4)
        elif op in (4, 5, 6, 0x14):
            check(current is None, 'branch in delay slot')
            taken = regs[rs] == regs[rt] if op in (4, 0x14) else regs[rs] != regs[rt] if op == 5 else signed(regs[rs]) <= 0
            branches.add((pc, taken))
            if op == 0x14 and not taken:
                nextpc += 4
            else:
                pending = ('jump', pc + 4 + imm * 4 if taken else pc + 8)
        elif op == 3:
            check(current is None, 'call in delay slot')
            regs[31] = ADDRESS + pc + 8
            pending = ('call', ((w & 0x3ffffff) << 2) | 0x80000000)
        else: raise ValueError(('unsupported opcode', op))
        regs[0] = 0
        if current is not None:
            check(pending is None, 'control transfer in delay slot')
            kind, destination = current
            if kind == 'call':
                hook(memory, destination, regs[4], scenario, events)
                for r in [1, 2, 3] + list(range(4, 16)) + [24, 25]:
                    regs[r] = (0xabcdef00 ^ r * 0x1020304 ^ len(events)) & U32
            else:
                nextpc = destination
        pc = nextpc
    raise ValueError('execution limit')

