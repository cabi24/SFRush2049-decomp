"""Bounded MIPS unlink/call-contract execution; external routines are hooks."""
import hashlib
import random
import struct

START = 0x800E7A98
DEFAULT = 0x801527C8
QUEUE = 0x80152770
RECV, RELEASE, JAM = 0x80007270, 0x80095FD8, 0x800075E0
BASE, STACK = 0x410000, 0x710000

def addr(index): return BASE + index * 32 if index >= 0 else 0

def cases():
    result = []
    for n in range(1, 9):
        for selected in [-1, 0, n - 1, n]:
            for shifted in [0, 1]:
                for rewrite in [0, 1]:
                    result.append((n, selected, shifted, rewrite, list(range(n))))
    # An empty default chain is valid for the wrapper when an explicit heap is supplied.
    result.append((0, 0, 0, 0, []))
    rng = random.Random(0xE7A98)
    for i in range(1024):
        n = rng.randrange(1, 33)
        order = list(range(n)); rng.shuffle(order)
        result.append((n, rng.choice([-1] + list(range(n + 1))), i % 2, (i // 2) % 2, order))
    return result

def initial(case):
    n, selected, shifted, rewrite, order = case
    memory = {BASE: bytearray([0xA6] * (32 * (n + 1))),
              DEFAULT: bytearray(struct.pack('>I', addr(order[0]) if n else 0)),
              QUEUE: bytearray([0x97] * 24), STACK: bytearray([0xC5] * 512)}
    for i in range(n + 1): put(memory, addr(i) + 4, 0)
    for left, right in zip(order, order[1:]): put(memory, addr(left) + 4, addr(right))
    return memory

def access(memory, address, value=None):
    assert address % 4 == 0, 'unaligned memory'
    for base, data in memory.items():
        i = address - base
        if 0 <= i <= len(data) - 4:
            if value is None: return int.from_bytes(data[i:i + 4], 'big')
            data[i:i + 4] = (value & 0xFFFFFFFF).to_bytes(4, 'big'); return
    raise AssertionError(('unmapped', hex(address)))

def put(memory, address, value): access(memory, address, value)

def receive(memory, case):
    n, selected, shifted, rewrite, order = case
    if n:
        put(memory, DEFAULT, addr(order[min(1, n - 1)]) if shifted else addr(order[0]))
        if rewrite and n >= 3: put(memory, addr(order[0]) + 4, addr(order[2]))

def finish(memory, chosen):
    # Deliberate opaque callee effects: caller must neither reselect nor unlink again.
    put(memory, chosen, 0xABCDEF01)
    put(memory, QUEUE, 0x11223344)

def external_state(memory): return {a: bytes(b) for a, b in memory.items() if a != STACK}

def oracle(case):
    memory = initial(case); receive(memory, case)
    selected = case[1]; chosen = addr(selected) if selected >= 0 else access(memory, DEFAULT)
    current = access(memory, DEFAULT)
    while current:
        successor = access(memory, current + 4)
        if successor == chosen:
            put(memory, current + 4, access(memory, chosen + 4)); break
        current = successor
    before_release = external_state(memory)
    finish(memory, chosen)
    return chosen, before_release, external_state(memory)

def execute(words, case):
    memory = initial(case)
    r = [0xD4000000 + 0x101 * i for i in range(32)]
    r[0] = 0; r[4] = addr(case[1]); r[29] = STACK + 256; r[31] = 0xFFFFFFFC
    original = r[:]; pc = START; delayed = None; seen = set(); events = []
    writes = set(); initialized = set(); expected = oracle(case)
    for steps in range(4000):
        if pc == 0xFFFFFFFC: break
        if pc in (RECV, RELEASE, JAM):
            assert delayed is None
            if pc == RECV:
                assert events == [] and (r[4], r[5], r[6]) == (QUEUE, 0, 1)
                receive(memory, case); events.append('receive')
            elif pc == RELEASE:
                assert events == ['receive'] and (r[5], r[6]) == (expected[0], 1), 'release ABI'
                assert external_state(memory) == expected[1], 'unlink must precede release'
                put(memory, expected[0], 0xABCDEF01); events.append('release')
                r[16] = 0xBAD016; r[17] = 0xBAD017
            else:
                assert events == ['receive', 'release'] and (r[4], r[5], r[6]) == (QUEUE, 0, 0)
                put(memory, QUEUE, 0x11223344); events.append('jam')
            for k in (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25): r[k] = 0xAB000000 + k
            r[2] = 0; pc = r[31]; continue
        assert START <= pc < START + len(words) * 4, ('bad PC', hex(pc))
        seen.add(pc - START); word = words[(pc - START) // 4]
        op, rs, rt, rd = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31
        imm = word & 65535; disp = imm - 65536 if imm & 32768 else imm
        effective = (r[rs] + disp) & 0xFFFFFFFF
        pending = delayed; delayed = None; following = pc + 4
        if op == 0:
            fn = word & 63
            if fn == 0: r[rd] = (r[rt] << ((word >> 6) & 31)) & 0xFFFFFFFF
            elif fn == 8: delayed = r[rs]
            elif fn == 0x25: r[rd] = r[rs] | r[rt]
            else: raise AssertionError(('unsupported SPECIAL', hex(word)))
        elif op == 3:
            r[31] = pc + 8; delayed = ((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
        elif op == 15: r[rt] = imm << 16
        elif op == 9: r[rt] = effective
        elif op in (4, 5):
            if (r[rs] == r[rt]) == (op == 4): delayed = pc + 4 + disp * 4
        elif op == 35:
            if STACK <= effective < STACK + 512:
                assert all(effective + i in initialized for i in range(4)), 'uninitialized stack'
            r[rt] = access(memory, effective)
        elif op == 43:
            if STACK <= effective < STACK + 512:
                assert STACK + 224 <= effective <= STACK + 256, 'stack frame write'
                writes.update(range(effective, effective + 4)); initialized.update(range(effective, effective + 4))
            else:
                assert BASE <= effective < BASE + 32 * (case[0] + 1) and (effective - BASE) % 32 == 4, 'unexpected write'
            put(memory, effective, r[rt])
        else: raise AssertionError(('unsupported opcode', hex(word)))
        r[0] = 0; pc = pending if pending is not None else following
    else: raise AssertionError('execution limit')
    assert events == ['receive', 'release', 'jam']
    assert r[16:24] == original[16:24] and r[28:31] == original[28:31], 'callee-saved registers'
    for i, value in enumerate(memory[STACK]):
        if STACK + i not in writes: assert value == 0xC5, 'stack canary'
    assert external_state(memory) == expected[2], 'final state'
    return expected[0], seen

def corpus_text(corpus):
    return ''.join(' '.join(map(str, [n, chosen, shift, rewrite] + order)) + '\n'
                   for n, chosen, shift, rewrite, order in corpus)

def expected_text(corpus): return ''.join(str((oracle(c)[0] - BASE) // 32) + '\n' for c in corpus)

def verify(native, candidate, linked):
    coverage = [set(), set(), set()]; corpus = cases()
    for case in corpus:
        for i, words in enumerate((native, candidate, linked)):
            _, seen = execute(words, case); coverage[i].update(seen)
    assert all(seen == set(range(0, 172, 4)) for seen in coverage)
    text = expected_text(corpus)
    return {'cases': len(corpus), 'native_candidate_gnu_executions': 3 * len(corpus),
            'covered_words_each': [len(x) for x in coverage], 'total_words': 43,
            'oracle_sha256': hashlib.sha256(text.encode()).hexdigest(),
            'external_callees': 'contract hooks; actual release body is code-checked but not executed',
            'delay_slots_stack_canaries_and_saved_registers': True}, corpus
