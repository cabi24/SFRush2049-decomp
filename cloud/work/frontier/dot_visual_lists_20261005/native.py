"""Bounded integer MIPS-II executor; native words are read only from protected targets."""
LIST = 0x80144D60
NODES = 0x20000000
BODIES = 0x21000000
STACK = 0x70000100
STOP = 0xFFFFFFFC
CALL = 0x800A2378
MASK = 0xFFFFFFFF

def na(i): return 0 if i < 0 else NODES + 4 * i
def ba(i): return BODIES + 76 * i
def ni(p):
    if not p: return -1
    assert NODES <= p < NODES + 64 and (p - NODES) % 4 == 0
    return (p - NODES) // 4

def bi(p):
    assert BODIES <= p < BODIES + 32 * 76 and (p - BODIES) % 76 == 0
    return (p - BODIES) // 76

def initial(seed):
    mem = {LIST + j: 0xDCBA0000 + j for j in range(0, 64, 4)}
    for i in range(32):
        mem.update({ba(i) + j: 0xBCDE0000 + j for j in range(0, 76, 4)})
    for i in range(16):
        mem[na(i)] = ba(i)
        mem[ba(i)] = na(i + 1) if i % 4 < 3 else 0
        mem[ba(i) + 72] = 0x80000001 if (seed >> i) & 1 else 0
        mem[ba(16 + i)] = na(i + 2) if i % 4 < 2 else 0
        mem[ba(16 + i) + 72] = MASK
    for i in range(4):
        length = (seed >> (i * 3)) % 5
        mem[LIST + 16 * i + 8] = na(4 * i) if length else 0
        if length: mem[ba(4 * i + length - 1)] = 0
    return mem

def operation(mem, node, action, mode, events):
    ident, body = ni(node), mem[node]
    nxt = ni(mem[body])
    events.extend([ident, action, bi(body), nxt, mem[body + 72]])
    events.extend(ni(mem[LIST + 16 * k + 8]) for k in range(4))
    if mode == 1 and nxt >= 0: mem[body] = na(nxt + 1) if nxt < 15 else 0
    elif mode == 2: mem[node] = ba(16 + ident)
    elif mode == 3 and ident // 4 < 3: mem[LIST + 16 * (ident // 4 + 1) + 8] = 0
    elif mode == 4 and nxt >= 0: mem[mem[na(nxt)] + 72] = 0
    elif mode == 5: mem[body] = 0
    elif mode == 6 and ident // 4 < 3:
        mem[LIST + 16 * (ident // 4 + 1) + 8] = na((ident // 4 + 1) * 4 + 3)

def snapshot(mem, events):
    out = [len(events)] + events
    out.extend(ni(mem[LIST + 16 * i + 8]) for i in range(4))
    out.extend(bi(mem[na(i)]) for i in range(16))
    for i in range(32): out.extend([ni(mem[ba(i)]), mem[ba(i) + 72]])
    return [v & MASK for v in out]

def oracle(seed, mode, action):
    mem, events = initial(seed), []
    for slot in range(4):
        node = mem[LIST + 16 * slot + 8]
        visits = 0
        while node:
            visits += 1
            assert visits <= 16
            if mem[mem[node] + 72]: operation(mem, node, action, mode, events)
            node = mem[mem[node]]
    return snapshot(mem, events), mem

def execute(words, start, seed, mode, action):
    mem, events, visited = initial(seed), [], set()
    external = set(mem)
    mem.update({STACK + j: 0xDEAD0000 + j for j in range(-128, 32, 4)})
    regs = [(0xA5000000 + i * 0x10001) & MASK for i in range(32)]
    regs[0], regs[4], regs[29], regs[31] = 0, action, STACK, STOP
    original = list(regs)
    pc, pending, steps = start, None, 0
    while pc != STOP:
        if pc == CALL:
            operation(mem, regs[4], regs[5], mode, events)
            for r in list(range(2, 16)) + [24, 25]: regs[r] = (0xABCD0000 + r * 7 + len(events)) & MASK
            for offset in range(0, 16, 4): mem[regs[29] + offset] = 0xCBAD0000 + offset
            pc = regs[31]
            continue
        assert start <= pc < start + len(words) * 4 and (pc - start) % 4 == 0
        assert steps < 3000
        visited.add(pc - start)
        word = words[(pc - start) // 4]
        op, rs, rt, rd = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31
        immediate = word & 65535
        offset = immediate - 65536 if immediate & 32768 else immediate
        addr = (regs[rs] + offset) & MASK
        delayed, pending, following = pending, None, pc + 4
        if word == 0: pass
        elif op == 0 and word & 63 == 33: regs[rd] = (regs[rs] + regs[rt]) & MASK
        elif op == 0 and word & 63 == 37: regs[rd] = regs[rs] | regs[rt]
        elif op == 0 and word & 63 == 8: pending = regs[rs]
        elif op == 9: regs[rt] = addr
        elif op == 15: regs[rt] = immediate << 16
        elif op in (35, 43):
            assert addr % 4 == 0 and addr in mem, ('unmapped access', hex(addr))
            if op == 35: regs[rt] = mem[addr]
            else:
                assert addr not in external, 'candidate unexpectedly writes external state'
                mem[addr] = regs[rt]
        elif op in (4, 5, 20, 21):
            take = (regs[rs] == regs[rt]) == (op in (4, 20))
            if take: pending = pc + 4 + 4 * offset
            elif op in (20, 21): following += 4
        elif op == 3:
            regs[31] = pc + 8
            pending = ((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
            assert pending == CALL
        else: raise AssertionError(('unsupported instruction', hex(pc), op))
        regs[0] = 0
        pc = delayed if delayed is not None else following
        steps += 1
    assert regs[16:24] == original[16:24]
    assert all(regs[r] == original[r] for r in [28, 29, 30, 31])
    assert all(mem[STACK + j] == 0xDEAD0000 + j for j in range(0, 32, 4))
    return snapshot(mem, events), {k: mem[k] for k in external}, visited
