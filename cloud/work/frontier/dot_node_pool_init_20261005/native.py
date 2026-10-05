"""Fail-closed MIPS subset and independent free-list state oracle."""
import random
import struct

MASK = 0xffffffff
POOL = 0x80138880
HEAD = 0x801392c8
EXTRA = 0x801391f0
COUNT = 0x8012e66c
HIGH = 0x8012e678
INIT = 0x800b2bdc
POP = 0x80090284
RETURN = 0x807ff000


def signed(x, bits=32):
    x &= (1 << bits) - 1
    return x - (1 << bits) if x >> (bits - 1) else x


def put(mem, addr, size, value):
    assert addr % size == 0
    for i in range(size):
        assert addr + i in mem, ('unmapped store', hex(addr + i))
        mem[addr + i] = (value >> (8 * (size - 1 - i))) & 255


def get(mem, addr, size):
    assert addr % size == 0
    return int.from_bytes(bytes(mem[addr + i] for i in range(size)), 'big')


def execute(words, start, mem, salt=0):
    regs = [((0xa5310000 + 0x137 * i) ^ salt) & MASK for i in range(32)]
    regs[0], regs[29], regs[31] = 0, 0x80700080, RETURN
    saved = regs[:]
    floats = [0x7f812345] * 32
    code = {start + 4 * i: w for i, w in enumerate(words)}
    pc, pending = start, None
    visited, writes, reads, branches = set(), [], [], set()
    for _ in range(5000):
        if pc == RETURN:
            assert pending is None
            assert regs[16:24] == saved[16:24]
            assert all(regs[r] == saved[r] for r in [28, 29, 30, 31])
            return regs[2], visited, writes, reads, branches
        assert pc in code, ('unmapped instruction', hex(pc))
        word = code[pc]
        visited.add(pc - start)
        op, rs, rt, rd = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31
        imm, fn = word & 65535, word & 63
        off = signed(imm, 16)
        addr = (regs[rs] + off) & MASK
        delayed, pending, following = pending, None, pc + 4
        if delayed is not None:
            assert not (op in (4, 5) or (op == 0 and fn == 8)), 'control transfer in delay slot'
        if word == 0:
            pass
        elif op == 0 and fn in (33, 37, 42):
            assert ((word >> 6) & 31) == 0, 'reserved shift field'
            if fn == 33: value = regs[rs] + regs[rt]
            elif fn == 37: value = regs[rs] | regs[rt]
            else: value = int(signed(regs[rs]) < signed(regs[rt]))
            regs[rd] = value & MASK
        elif op == 0 and fn == 8:
            assert (word & 0x1fffc0) == 0, 'reserved jr fields'
            pending = regs[rs]
        elif op == 9:
            regs[rt] = addr
        elif op == 15:
            assert rs == 0, 'reserved lui source'
            regs[rt] = imm << 16
        elif op in (33, 35):
            size = 2 if op == 33 else 4
            value = get(mem, addr, size)
            regs[rt] = signed(value, 16) & MASK if size == 2 else value
            reads.append((addr, size))
        elif op in (41, 43, 57):
            size = 2 if op == 41 else 4
            value = floats[rt] if op == 57 else regs[rt]
            put(mem, addr, size, value)
            writes.append((addr, size, value & ((1 << (size * 8)) - 1)))
        elif op == 17 and rs == 4 and (word & 0x7ff) == 0:
            floats[rd] = regs[rt]
        elif op in (4, 5):
            take = (regs[rs] == regs[rt]) == (op == 4)
            branches.add((pc - start, take))
            if take: pending = pc + 4 + 4 * off
        else:
            raise AssertionError(('unsupported instruction', hex(pc), hex(word)))
        regs[0] = 0
        pc = delayed if delayed is not None else following
    raise AssertionError('step limit')


def case(seed, high):
    rng = random.Random(seed)
    mem = {a: rng.randrange(256) for lo, hi in [
        (POOL - 32, HEAD + 32), (COUNT - 12, HIGH + 12),
        (0x80700000, 0x80700100)] for a in range(lo, hi)}
    for i in range(100):
        put(mem, POOL + i * 24, 4, POOL + (i + 7) % 100 * 24)
        put(mem, POOL + i * 24 + 12, 4, 0)
    put(mem, HIGH, 2, high)
    return mem


def oracle_init(mem):
    put(mem, HEAD, 4, POOL)
    for i in range(100):
        put(mem, POOL + i * 24, 4, POOL + (i + 1) * 24 if i < 99 else 0)
        put(mem, POOL + i * 24 + 6, 2, -1)
    put(mem, EXTRA, 4, 0)
    put(mem, COUNT, 2, 0)


def oracle_pop(mem):
    node = get(mem, HEAD, 4)
    if not node: return 0
    put(mem, HEAD, 4, get(mem, node, 4))
    count = signed(get(mem, COUNT, 2) + 1, 16)
    put(mem, COUNT, 2, count)
    if count > signed(get(mem, HIGH, 2), 16): put(mem, HIGH, 2, count)
    for offset, size, value in [(0, 4, 0), (4, 2, 0), (6, 2, -1),
                                (8, 2, 0), (12, 4, 0), (16, 4, 0), (20, 4, 0)]:
        put(mem, node + offset, size, value)
    return node


def compact(mem):
    return bytes(mem[POOL + i] for i in range(2400)) + struct.pack(
        '>IIHH', get(mem, HEAD, 4), get(mem, EXTRA, 4),
        get(mem, COUNT, 2), get(mem, HIGH, 2))
