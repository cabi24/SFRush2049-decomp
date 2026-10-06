"""Fail-closed bounded MIPS scalar execution, exact normal/zero RNE products.

No target words are embedded. This is not a VR4300 FCSR/trap emulator.
"""
import random

ENTRY = 0x8039D300
RETURN = 0x81234560
SELECTOR = 0x803B9FD0
OUTPUT = 0x803BA190
INDICES = (0x80110E85, 0x80111049, 0x8011108D, 0x801111A9, 0x8011123D)
VECTORS = (0x803B28C8, 0x803B2948, 0x803B2978, 0x803B2A08, 0x803B2A58)

def signed(n, bits=32):
    n &= (1 << bits) - 1
    return n - (1 << bits) if n & (1 << (bits - 1)) else n

def mul(a, b):
    """Integer-significand binary32 product: normals/zeros, RNE only.

    All nonzero subnormal/NaN/infinity inputs and any under/overflow result
    fail closed. Signed zeros are bit-exact, including negative-zero output.
    """
    sign = (a ^ b) & 0x80000000
    ea, eb = (a >> 23) & 255, (b >> 23) & 255
    fa, fb = a & 0x7fffff, b & 0x7fffff
    if ea == 255 or eb == 255 or (ea == 0 and fa) or (eb == 0 and fb):
        raise ValueError('outside normal/zero arithmetic domain')
    if (ea == 0 and fa == 0) or (eb == 0 and fb == 0):
        return sign
    p = ((1 << 23) | fa) * ((1 << 23) | fb)
    shift = p.bit_length() - 24
    q, remainder = divmod(p, 1 << shift)
    half = 1 << (shift - 1)
    q += remainder > half or (remainder == half and q & 1)
    exponent = ea + eb - 127 + shift - 23
    if q == 1 << 24:
        q >>= 1
        exponent += 1
    if not 1 <= exponent <= 254:
        raise ValueError('outside normal-result arithmetic domain')
    return sign | (exponent << 23) | (q & 0x7fffff)

def load(m, a, n):
    assert a % n == 0 and all(a+j in m for j in range(n)), ('unbacked read', hex(a), n)
    return int.from_bytes(bytes(m[a+j] for j in range(n)), 'big')

def store(m, a, n, v):
    assert a % n == 0 and all(a+j in m for j in range(n)), ('unbacked write', hex(a), n)
    for j, b in enumerate((v & ((1 << (8*n))-1)).to_bytes(n, 'big')):
        m[a+j] = b

def state(player, selector, rows, values):
    assert player in range(4) and selector in range(13)
    assert len(rows) == 5 and all(i in range(3) for i in rows)
    spans = [(SELECTOR-8, SELECTOR+12), (OUTPUT-8, OUTPUT+72)]
    spans += [(a-8, a+60) for a in INDICES]
    # Vector tables are tightly packed; no artificial canaries overlap a peer.
    spans += [(a, a+48) for a in VECTORS]
    m = {a: 0xA5 for lo, hi in spans for a in range(lo, hi)}
    m[SELECTOR+player] = selector
    for i, row in enumerate(rows):
        m[INDICES[i]+13*player+selector] = row
        for component in range(4):
            store(m, VECTORS[i]+16*row+4*component, 4, values[4*i+component])
    return m

def oracle(initial, player, values):
    m = dict(initial)
    writes = []
    for c in range(4):
        v = mul(mul(values[c], values[4+c]), values[8+c])
        address = OUTPUT+16*player+4*c
        for v in (v, mul(v, values[12+c]), mul(mul(v, values[12+c]), values[16+c])):
            store(m, address, 4, v)
            writes.append((address, v))
    return m, writes

def run(words, initial, player, seed=0):
    rng = random.Random(seed)
    r = [rng.getrandbits(32) for _ in range(32)]
    f = [rng.getrandbits(32) for _ in range(32)]
    r[0], r[4], r[31] = 0, player, RETURN
    before, fbefore = list(r), list(f)
    m, writes, reads, multiplies = dict(initial), [], [], []
    pc, pending, coverage, branches = ENTRY, None, set(), set()
    greads, gwrites, freads, fwrites = set(), set(), set(), set()
    for step in range(512):
        if pc == RETURN:
            assert pending is None
            return dict(memory=m, writes=writes, reads=reads, multiplies=multiplies,
                        gpr=r, fpr=f, before=before, fbefore=fbefore,
                        coverage=coverage, branches=branches,
                        greads=greads, gwrites=gwrites, freads=freads, fwrites=fwrites)
        assert ENTRY <= pc < ENTRY+len(words)*4 and pc % 4 == 0, ('bad PC', hex(pc))
        off = pc-ENTRY
        coverage.add(off)
        w = words[off//4]
        op, rs, rt, rd, fn = w >> 26, (w >> 21)&31, (w >> 16)&31, (w >> 11)&31, w&63
        imm = signed(w, 16)
        delayed, pending, transfer = pending, None, None
        def gr(i):
            if i and i not in gwrites: greads.add(i)
            return r[i]
        def gw(i, value):
            if i: gwrites.add(i); r[i] = value & 0xffffffff
        if w == 0: pass
        elif op == 0:
            if fn == 0: gw(rd, gr(rt) << ((w >> 6)&31))
            elif fn == 3: gw(rd, signed(gr(rt)) >> ((w >> 6)&31))
            elif fn == 33: gw(rd, gr(rs)+gr(rt))
            elif fn == 35: gw(rd, gr(rs)-gr(rt))
            elif fn == 37: gw(rd, gr(rs)|gr(rt))
            elif fn == 8:
                assert rs == 31 and (w & 0x1fffff) == 8
                transfer = gr(rs)
            else: raise AssertionError(('unknown SPECIAL', off, fn))
        elif op == 15:
            assert rs == 0
            gw(rt, (w & 65535) << 16)
        elif op == 9: gw(rt, gr(rs)+imm)
        elif op == 10: gw(rt, int(signed(gr(rs)) < imm))
        elif op == 32:
            a = (gr(rs)+imm)&0xffffffff
            gw(rt, signed(load(m, a, 1), 8)); reads.append((a, 1))
        elif op == 49:
            a = (gr(rs)+imm)&0xffffffff
            f[rt] = load(m, a, 4); fwrites.add(rt); reads.append((a, 4))
        elif op == 57:
            a = (gr(rs)+imm)&0xffffffff
            assert OUTPUT+16*player <= a < OUTPUT+16*player+16
            if rt not in fwrites: freads.add(rt)
            store(m, a, 4, f[rt]); writes.append((a, f[rt]))
        elif op == 17:
            assert rs == 16 and fn == 2, ('unsupported COP1', off)
            fd = (w >> 6)&31
            for q in (rd, rt):
                if q not in fwrites: freads.add(q)
            multiplies.append((f[rd], f[rt]))
            f[fd] = mul(f[rd], f[rt]); fwrites.add(fd)
        elif op == 5:
            taken = gr(rs) != gr(rt)
            branches.add((off, taken))
            transfer = pc+4+4*imm if taken else pc+8
        else: raise AssertionError(('unknown opcode', off, op))
        if delayed is not None:
            assert transfer is None, 'transfer in delay slot'
            pc = delayed
        else:
            pc, pending = pc+4, transfer
    raise AssertionError('step limit')
