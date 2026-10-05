"""Bounded native interpreter and independent heap-size oracle; no target data stored."""
import hashlib
import random
import struct

START = 0x800E79F8
QUEUE = 0x80152770
DEFAULT = 0x801527C8
RECV = 0x80007270
JAM = 0x800075E0
HEAP = (0x410000, 0x420000)
NODES = 0x430000
STACK = 0x710000


def signed(value, bits=32):
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def cases():
    # mode: explicit heap 0/1 or null default. recv may swap the default and
    # replace the first block's status/size; jam mutates after the snapshot.
    out = []
    values = [0, 1, 31, 32, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFE, 0xFFFFFFFF]
    statuses = [-128, -1, 0, 1, 127]
    for value in values:
        for status in statuses:
            for mode in range(3):
                for flags in range(4):
                    out.append(([value, value ^ 0xFFFFFFFF], [status, 0], [0, 1], mode, flags))
    out.extend([([], [], [], mode, flags) for mode in range(3) for flags in range(4)])
    rng = random.Random(0xE79F8)
    for i in range(4096):
        n = rng.randrange(1, 49)
        sizes = [rng.getrandbits(32) if j % 3 else rng.choice(values) for j in range(n)]
        used = [rng.choice(statuses) for _ in range(n)]
        order = list(range(n)); rng.shuffle(order)
        out.append((sizes, used, order, i % 3, i % 4))
    return out


def state(case):
    sizes, used, order, mode, flags = case
    regions = {h: bytearray([0xA6] * 12) for h in HEAP}
    regions[DEFAULT] = bytearray(struct.pack('>I', HEAP[0]))
    regions[QUEUE] = bytearray([0x97] * 24)
    regions[STACK] = bytearray([0xC5] * 512)
    for i, (size, status) in enumerate(zip(sizes, used)):
        b = bytearray([0xB7] * 32)
        b[12:16] = struct.pack('>I', size)
        b[20] = status & 255
        regions[NODES + 64 * i] = b
    heads = [order, list(reversed(order))]
    # Each heap has its own block records, so its ordering is independent.
    for h, indices in enumerate(heads):
        off = h * 0x10000
        regions[HEAP[h]][8:12] = struct.pack('>I', NODES + off + indices[0] * 64 if indices else 0)
        for j, index in enumerate(indices):
            addr = NODES + off + index * 64
            b = bytearray(regions[NODES + index * 64])
            b[4:8] = struct.pack('>I', NODES + off + indices[j + 1] * 64 if j + 1 < len(indices) else 0)
            regions[addr] = b
    return regions


def access(regions, addr, size, value=None):
    assert addr % size == 0, ('alignment', hex(addr), size)
    for base, data in regions.items():
        off = addr - base
        if 0 <= off and off + size <= len(data):
            if value is None:
                return int.from_bytes(data[off:off + size], 'big')
            data[off:off + size] = (value & ((1 << (8 * size)) - 1)).to_bytes(size, 'big')
            return
    raise AssertionError(('unmapped', hex(addr), size))


def callback(regions, case, is_recv):
    sizes, used, order, mode, flags = case
    if is_recv:
        if flags & 1:
            access(regions, DEFAULT, 4, HEAP[1])
        if flags & 2 and order:
            selected = HEAP[mode] if mode < 2 else access(regions, DEFAULT, 4)
            node = access(regions, selected + 8, 4)
            access(regions, node + 12, 4, 0xFFFFFFFF)
            access(regions, node + 20, 1, 0)
    else:
        access(regions, DEFAULT, 4, HEAP[0])
        if order:
            selected = HEAP[mode] if mode < 2 else (HEAP[1] if flags & 1 else HEAP[0])
            node = access(regions, selected + 8, 4)
            access(regions, node + 12, 4, 0)
            access(regions, node + 20, 1, 1)


def oracle(case):
    regions = state(case)
    callback(regions, case, True)
    mode = case[3]
    heap = HEAP[mode] if mode < 2 else access(regions, DEFAULT, 4)
    p = access(regions, heap + 8, 4)
    free = []
    while p:
        if access(regions, p + 20, 1) == 0:
            free.append(access(regions, p + 12, 4))
        p = access(regions, p + 4, 4)
    result = max(free, default=0)
    callback(regions, case, False)
    return result, {a: bytes(b) for a, b in regions.items() if a != STACK}


def execute(words, case):
    regions = state(case)
    mode = case[3]
    r = [0xD4000000 + 0x101 * i for i in range(32)]
    r[0] = 0; r[4] = HEAP[mode] if mode < 2 else 0
    r[29] = STACK + 256; r[31] = 0xFFFFFFFC
    initial = r[:]; pc = START; delayed = None; seen = set(); events = []
    writes = set(); stack_init = set(); steps = 0
    while pc != 0xFFFFFFFC:
        steps += 1
        assert steps < 3000, 'execution bound exceeded'
        if pc in (RECV, JAM):
            assert delayed is None
            is_recv = pc == RECV
            assert (r[4], r[5], r[6]) == (QUEUE, 0, int(is_recv)), 'queue ABI'
            events.append(('recv' if is_recv else 'jam', r[4], r[5], r[6]))
            callback(regions, case, is_recv)
            for k in (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25):
                r[k] = 0xAB000000 + k
            r[2] = 0
            pc = r[31]
            continue
        assert START <= pc < START + len(words) * 4, ('pc', hex(pc))
        off = pc - START; seen.add(off); w = words[off // 4]
        op, rs, rt, rd = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        sa, imm = (w >> 6) & 31, w & 65535
        disp = signed(imm, 16); addr = (r[rs] + disp) & 0xFFFFFFFF
        old = delayed; delayed = None; next_pc = pc + 4
        if op == 0:
            fn = w & 63
            if fn == 0: r[rd] = (r[rt] << sa) & 0xFFFFFFFF
            elif fn == 8: delayed = r[rs]
            elif fn == 0x21: r[rd] = (r[rs] + r[rt]) & 0xFFFFFFFF
            elif fn == 0x25: r[rd] = r[rs] | r[rt]
            elif fn == 0x2B: r[rd] = int(r[rs] < r[rt])
            else: raise AssertionError(('special', hex(w)))
        elif op == 3:
            r[31] = pc + 8; delayed = ((pc + 4) & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
        elif op == 15: r[rt] = imm << 16
        elif op == 9: r[rt] = addr
        elif op in (4, 5, 20, 21):
            taken = (r[rs] == r[rt]) if op in (4, 20) else (r[rs] != r[rt])
            if taken: delayed = pc + 4 + disp * 4
            elif op in (20, 21): next_pc += 4
        elif op in (32, 35):
            size = 1 if op == 32 else 4
            if STACK <= addr < STACK + 512:
                assert all(addr + i in stack_init for i in range(size)), 'uninitialized stack read'
            v = access(regions, addr, size)
            r[rt] = (signed(v, 8) if size == 1 else v) & 0xFFFFFFFF
        elif op == 43:
            assert STACK + 224 <= addr <= STACK + 256, 'unexpected native data write'
            access(regions, addr, 4, r[rt]); writes.update(range(addr, addr + 4)); stack_init.update(range(addr, addr + 4))
        else: raise AssertionError(('opcode', hex(w), hex(pc)))
        r[0] = 0; pc = old if old is not None else next_pc
    assert events == [('recv', QUEUE, 0, 1), ('jam', QUEUE, 0, 0)]
    assert r[16:24] == initial[16:24] and r[28:31] == initial[28:31], 'callee-saved ABI'
    for i, value in enumerate(regions[STACK]):
        if STACK + i not in writes: assert value == 0xC5, 'stack canary'
    result = r[2], {a: bytes(b) for a, b in regions.items() if a != STACK}
    assert result == oracle(case), 'native/oracle disagreement'
    return r[2], seen


def corpus_text(corpus):
    lines = []
    for sizes, used, order, mode, flags in corpus:
        lines.append(' '.join(map(str, [len(sizes), mode, flags] + sizes + used + order)))
    return '\n'.join(lines) + '\n'


def native_checks(words, linked_words):
    corpus = cases(); seen = set(); returns = []
    for case in corpus:
        result, visited = execute(words, case); seen.update(visited)
        linked, linked_visited = execute(linked_words, case)
        assert linked == result and linked_visited == visited
        returns.append(result)
    assert seen == set(range(0, 160, 4)), sorted(set(range(0, 160, 4)) - seen)
    return {'cases': len(corpus), 'native_executions': 2 * len(corpus),
            'executed_instruction_offsets': len(seen), 'instruction_count': 40,
            'result_sha256': hashlib.sha256(struct.pack('>%dI' % len(returns), *returns)).hexdigest()}, corpus, returns
