"""Bounded big-endian o32 instruction/ABI replay. Unknown operations fail closed.

No retail instructions are stored here; the caller supplies manifest-verified words.
Callbacks are synchronous contract hooks, not a replacement for gameplay testing.
"""
U32 = 0xffffffff
ADDRESS = 0x8001558c
SP = 0x70001000
RETURN = 0x60000000


def need(value, message):
    if not value:
        raise ValueError(message)


def signed(v, bits=32):
    v &= (1 << bits) - 1
    return v - (1 << bits) if v >> (bits - 1) else v


class Memory:
    def __init__(self, other=None):
        sizes = {0x8002c630: 1, 0x80038390: 2, 0x80038398: 32,
                 0x10000000: 8192, SP - 128: 256}
        self.banks = ({a: bytearray(n) for a, n in sizes.items()} if other is None
                      else {a: bytearray(b) for a, b in other.banks.items()})

    def locate(self, a, n):
        need(a % n == 0, ('alignment', hex(a), n))
        for base, data in self.banks.items():
            if base <= a and a + n <= base + len(data):
                return data, a - base
        raise ValueError(('unmapped', hex(a), n))

    def get(self, a, n):
        data, at = self.locate(a, n)
        return int.from_bytes(data[at:at+n], 'big')

    def put(self, a, v, n):
        data, at = self.locate(a, n)
        data[at:at+n] = (v & ((1 << (8*n))-1)).to_bytes(n, 'big')

    def half(self, a):
        return self.get(a, 1) * 256 + self.get(a + 1, 1)

    def snapshot(self):
        return tuple((a, bytes(b)) for a, b in self.banks.items() if a != SP - 128)


def fixture(seed, mode, unlocked):
    m = Memory()
    group = (0x8000 if seed & 1 else 0) + 7
    key = (0x8000 if seed & 2 else 0) + 11
    count = (-32768, -1, 0, 1, 3, 5)[mode % 6]
    m.put(0x8002c630, 0 if mode == 0 else 255, 1)
    m.put(0x80038390, count, 2)
    for i in range(5):
        base = 0x10000000 + 1024 * i
        m.put(0x80038398 + 4*i, base, 4)
        match = (i == (seed % 5)) or (mode >= 6 and i in (0, 1))
        m.put(base + 4, group if match else group + 1, 2)
        m.put(base + 6, 1 if mode == 7 and i == 0 else 0, 2)
        m.put(base + 28, 40, 4)
        m.put(base + 32, 64, 4)
        m.put(base + 36, 96 + (seed & 1), 4)
        p = base + 104 + (seed & 1)
        for j in range(5):
            value = key if j == (seed % 4) and mode != 8 else key + 1
            if j == 4 or mode == 9:
                value = 65535
            m.put(p + j*132, value >> 8, 1)
            m.put(p + j*132 + 1, value, 1)
    # Modes 6..11 add successful and deliberately unsuitable first matches.
    if mode >= 6:
        m.put(0x80038390, 5, 2)
    return m, dict(group=group, key=key, unlocked=unlocked,
                   stream=0x12345678, options=0 if seed & 4 else 0x23456780,
                   result=(0, 0xffffffff, 0x12345678, 0x80000000, 0xdeadbeef)[seed % 5],
                   mutate=bool(seed & 8))


def helper(m, address, args, scenario, events):
    if address == 0x80014594:
        events.append(('lock',))
        if scenario['mutate']:
            m.put(0x80038390, 0, 2)
        return 0x55555555
    if address == 0x800145dc:
        events.append(('unlock',))
        return 0xaaaaaaaa
    need(address == 0x800178b0, ('unknown callback', hex(address)))
    events.append(('dispatch', tuple(args)))
    return scenario['result']


def oracle(original, s):
    m = Memory(original)
    events = []
    result = U32
    if m.get(0x8002c630, 1):
        for i in range(max(0, signed(m.get(0x80038390, 2), 16))):
            base = m.get(0x80038398 + i*4, 4)
            if m.get(base + 4, 2) != s['group']:
                continue
            if m.get(base + 6, 2) == 0:
                p = base + m.get(base + 36, 4) + 8
                while m.half(p) != 65535:
                    if m.half(p) == s['key']:
                        args = [base + m.get(base + 28, 4) + 8,
                                base + m.get(base + 32, 4) + 8, p,
                                s['stream'], s['options']]
                        if not s['unlocked']:
                            helper(m, 0x80014594, [], s, events)
                        result = helper(m, 0x800178b0, args, s, events)
                        if not s['unlocked']:
                            helper(m, 0x800145dc, [], s, events)
                        break
                    p += 132
            break
    return result, events, m.snapshot()


def execute(words, original, s, upper):
    m = Memory(original)
    regs = [(0x1234567 * (i+1)) & U32 for i in range(32)]
    regs[0] = 0
    regs[4:8] = [(upper << 16) | s['group'], ((upper ^ 65535) << 16) | s['key'],
                 s['stream'], s['options']]
    regs[29], regs[31] = SP, RETURN
    m.put(SP + 16, 0xabcdef00 | s['unlocked'], 4)
    initial = regs[:]
    events, coverage = [], set()
    pc, pending = 0, None
    for _ in range(2500):
        if pc == RETURN:
            need(pending is None, 'pending return')
            need(all(regs[r] == initial[r] for r in list(range(16,24)) + [28,29,30,31]), 'o32 callee-saved corruption')
            return (regs[2], events, m.snapshot()), coverage
        need(pc % 4 == 0 and 0 <= pc < 4*len(words), ('PC', pc))
        coverage.add(pc)
        w = words[pc//4]
        op, rs, rt, rd, sh, fn = w >> 26, w >> 21 & 31, w >> 16 & 31, w >> 11 & 31, w >> 6 & 31, w & 63
        imm = signed(w, 16)
        a = (regs[rs] + imm) & U32
        current, pending, nextpc = pending, None, pc + 4
        if op == 0:
            if fn == 0:
                regs[rd] = regs[rt] << sh & U32
            elif fn == 0x21:
                regs[rd] = (regs[rs] + regs[rt]) & U32
            elif fn == 0x25:
                regs[rd] = regs[rs] | regs[rt]
            elif fn == 0x2a:
                regs[rd] = int(signed(regs[rs]) < signed(regs[rt]))
            elif fn == 8:
                need(rs == 31, 'indirect jump')
                pending = ('jump', regs[rs])
            else:
                raise ValueError(('unknown SPECIAL', fn))
        elif op == 9:
            regs[rt] = (regs[rs] + imm) & U32
        elif op == 12:
            regs[rt] = regs[rs] & (w & 65535)
        elif op == 13:
            regs[rt] = regs[rs] | (w & 65535)
        elif op == 15:
            regs[rt] = (w & 65535) << 16
        elif op in (0x21, 0x24, 0x25, 0x23):
            n = {0x21:2, 0x24:1, 0x25:2, 0x23:4}[op]
            v = m.get(a, n)
            regs[rt] = (signed(v, 16) if op == 0x21 else v) & U32
        elif op == 0x2b:
            m.put(a, regs[rt], 4)
        elif op in (4, 5, 6, 0x15):
            need(current is None, 'branch in delay slot')
            taken = (regs[rs] == regs[rt] if op == 4 else
                     signed(regs[rs]) <= 0 if op == 6 else regs[rs] != regs[rt])
            if op == 0x15 and not taken:
                nextpc = pc + 8
            else:
                pending = ('jump', pc + 4 + imm*4 if taken else pc + 8)
        elif op == 3:
            need(current is None, 'call in delay slot')
            regs[31] = ADDRESS + pc + 8
            pending = ('call', 0x80000000 | ((w & 0x3ffffff) << 2))
        else:
            raise ValueError(('unknown opcode', op))
        regs[0] = 0
        if current:
            need(pending is None, 'control in delay slot')
            kind, dest = current
            if kind == 'jump':
                nextpc = dest
            else:
                args = regs[4:8] + [m.get(regs[29] + 16, 4)]
                result = helper(m, dest, args, s, events)
                for r in [1,2,3] + list(range(4,16)) + [24,25]:
                    regs[r] = (0xdeadbeef ^ (r * 0x1020304) ^ len(events)) & U32
                regs[2] = result
        pc = nextpc
    raise ValueError('instruction budget')


def prove(bodies):
    coverage = {n:set() for n in bodies}
    cases = 0
    event_shapes = set()
    for seed in range(32):
        for mode in range(12):
            for unlocked in (0,1,255):
                m, s = fixture(seed, mode, unlocked)
                expected = oracle(m, s)
                event_shapes.add(tuple(e[0] for e in expected[1]))
                for name, words in bodies.items():
                    actual, seen = execute(words, m, s, (0,0x5555,0xffff)[seed%3])
                    need(actual == expected, ('behavior/ABI', name, seed, mode, unlocked))
                    coverage[name].update(seen)
                cases += 1
    need(event_shapes == {(), ('dispatch',), ('lock','dispatch','unlock')}, 'callback-path coverage')
    return dict(fixtures=cases, executions=cases*len(bodies),
                covered_instructions={n:len(v) for n,v in coverage.items()},
                unexecuted_offsets={n:sorted(set(range(0, len(bodies[n])*4,4))-v) for n,v in coverage.items()},
                domain='bounded mapped tables and sentinel-terminated packed programs; high input-home bits; all o32 volatile GPRs clobbered at callbacks')
