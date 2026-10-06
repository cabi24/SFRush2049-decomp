"""Fail-closed bounded native/GNU-linked replay of the complete voiceFree body."""
import struct

if not __debug__:
    raise SystemExit('Native proof requires Python assertions.')

BASE = 0x8001F6EC
HELPER = 0x8001EE9C
STATE = 0x80100000
LINKS = 0x80050440
SP = 0x807FF000
STOP = 0x80700000
MASK = 0xFFFFFFFF


def fixtures():
    for slot in range(32):
        for active in (0, 1, 65535):
            for nonempty in (0, 1):
                for external in (0, 1, 255):
                    for count in (0, 1, 255):
                        for alignment in range(4):
                            for mutation in (0, 1):
                                yield (slot, active, nonempty, external, count,
                                       alignment, mutation)


def initial(case):
    slot, active, nonempty, external, count, alignment, mutation = case
    seed = slot * 3 + external + count + mutation * 17
    state = bytearray((i * 13 + seed) & 255 for i in range(424))
    pointer = STATE + 4 + alignment
    relative = pointer - STATE
    state[relative + 96:relative + 100] = struct.pack('>I', 0xACDEF000 | slot)
    state[relative + 76] = external
    links = bytearray((i * 7 + seed) & 255 for i in range(132))
    after_slot = (slot + 11) % 32 if mutation else slot
    links[after_slot * 4 + 2:after_slot * 4 + 4] = struct.pack('>H', active)
    links[128] = 3 if nonempty else 255
    links[129] = after_slot if mutation else (after_slot + 13) % 32
    links[130] = count
    links[131] = count
    return state, links, pointer


def hook(state, links, pointer, case):
    slot, active, nonempty, external, count, alignment, mutation = case
    relative = pointer - STATE
    after_slot = (slot + 11) % 32 if mutation else slot
    if mutation:
        state[relative + 96:relative + 100] = struct.pack('>I', 0xDAAB0000 | after_slot)
        state[relative + 76] = 0 if external else 255
        state[relative + 0:relative + 4] = struct.pack('>I', 99)
        state[relative + 46] = 77
        state[relative + 17] ^= 0x55
        links[127] ^= 0x37
        # Preserve the selected user field if slot 31 was selected.
        if after_slot == 31:
            links[126:128] = struct.pack('>H', active)


def oracle(case):
    state, links, pointer = initial(case)
    hook(state, links, pointer, case)
    p = pointer - STATE
    index = int.from_bytes(state[p + 96:p + 100], 'big') & 255
    state[p:p + 4] = b'\0' * 4
    state[p + 46] = 0
    if int.from_bytes(links[index * 4 + 2:index * 4 + 4], 'big') == 0:
        links[index * 4 + 2:index * 4 + 4] = b'\0\1'
        if links[128] == 255:
            links[index * 4:index * 4 + 2] = b'\xff\xff'
            links[128] = index
        else:
            tail = links[129]
            links[index * 4] = tail
            links[index * 4 + 1] = 255
            links[tail * 4 + 1] = index
        links[129] = index
        counter = 130 if state[p + 76] else 131
        links[counter] = (links[counter] - 1) & 255
    state[p + 96:p + 100] = b'\xff' * 4
    return state, links


def run(words, case):
    state, links, pointer = initial(case)
    entry_state, entry_links = bytes(state), bytes(links)
    stack = bytearray([0xA5] * 512)
    regs = [(0x11220000 + i * 0x101) & MASK for i in range(32)]
    regs[0], regs[4], regs[29], regs[31] = 0, pointer, SP, STOP
    original = list(regs)
    visited, branches, stores, events = set(), set(), [], []
    pc, pending, steps = BASE, None, 0

    def location(address, size):
        for base, image in ((STATE, state), (LINKS, links), (SP - 256, stack)):
            if base <= address and address + size <= base + len(image):
                return image, address - base
        raise AssertionError(('unmapped access', hex(address), size))

    def read(address, size):
        image, offset = location(address, size)
        return bytes(image[offset:offset + size])

    def write(address, value):
        image, offset = location(address, len(value))
        stores.append((address, len(value)))
        image[offset:offset + len(value)] = value

    while pc != STOP:
        steps += 1
        assert steps < 150
        if pc == HELPER:
            assert pending is None and regs[4] == pointer and not events
            assert bytes(state) == entry_state and bytes(links) == entry_links
            events.append((HELPER, pointer))
            hook(state, links, pointer, case)
            return_pc = regs[31]
            for i in (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25):
                regs[i] = (0xDE000000 + i * 257) & MASK
            pc = return_pc
            continue
        assert BASE <= pc < BASE + len(words) * 4 and pc % 4 == 0
        visited.add(pc - BASE)
        word = words[(pc - BASE) // 4]
        op, rs, rt, rd = word >> 26, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31
        sa, fn, imm = word >> 6 & 31, word & 63, word & 65535
        signed_imm = imm - 65536 if imm & 32768 else imm
        address = (regs[rs] + signed_imm) & MASK
        next_pc, new_pending = pc + 4, None
        if op == 0:
            if fn == 0:
                regs[rd] = (regs[rt] << sa) & MASK
            elif fn == 0x21:
                regs[rd] = (regs[rs] + regs[rt]) & MASK
            elif fn == 0x25:
                regs[rd] = regs[rs] | regs[rt]
            elif fn == 8:
                new_pending = regs[rs]
            else:
                raise AssertionError(('unknown SPECIAL', pc, fn))
        elif op == 9:
            regs[rt] = address
        elif op == 12:
            regs[rt] = regs[rs] & imm
        elif op == 15:
            regs[rt] = imm << 16
        elif op in (4, 5, 20, 21):
            take = (regs[rs] == regs[rt]) == (op in (4, 20))
            branches.add((pc - BASE, take))
            if op in (20, 21) and not take:
                next_pc = pc + 8
            else:
                new_pending = pc + 4 + signed_imm * 4 if take else pc + 8
        elif op == 3:
            regs[31] = pc + 8
            new_pending = ((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
        elif op in (35, 36, 37):
            size = {35: 4, 36: 1, 37: 2}[op]
            assert address % size == 0
            regs[rt] = int.from_bytes(read(address, size), 'big')
        elif op in (34, 38):
            aligned, k = address & ~3, address & 3
            value = bytearray(regs[rt].to_bytes(4, 'big'))
            if op == 34:
                value[:4 - k] = read(address, 4 - k)
            else:
                value[3 - k:] = read(aligned, k + 1)
            regs[rt] = int.from_bytes(value, 'big')
        elif op in (43, 40, 41):
            size = {43: 4, 40: 1, 41: 2}[op]
            assert address % size == 0
            write(address, (regs[rt] & ((1 << (size * 8)) - 1)).to_bytes(size, 'big'))
        elif op in (42, 46):
            aligned, k = address & ~3, address & 3
            value = regs[rt].to_bytes(4, 'big')
            if op == 42:
                write(address, value[:4 - k])
            else:
                write(aligned, value[3 - k:])
        else:
            raise AssertionError(('unknown opcode', pc, op))
        regs[0] = 0
        if pending is not None:
            assert new_pending is None, 'branch in delay slot'
            next_pc = pending
        pending, pc = new_pending, next_pc
    assert pending is None and regs[29] == SP and regs[31] == STOP
    assert all(regs[i] == original[i] for i in tuple(range(16, 24)) + (30,))
    expected_state, expected_links = oracle(case)
    assert state == expected_state and links == expected_links
    assert events == [(HELPER, pointer)]
    assert all(STATE <= a < STATE + len(state) or LINKS <= a < LINKS + len(links)
               or (a, n) in ((SP - 4, 4), (SP, 4)) for a, n in stores)
    return dict(state=bytes(state), links=bytes(links), stack=bytes(stack),
                events=events, visited=visited, branches=branches)


def verify(native, linked):
    seen_native, seen_linked, native_branches, linked_branches = set(), set(), set(), set()
    count = 0
    for case in fixtures():
        a, b = run(native, case), run(linked, case)
        for key in ('state', 'links', 'stack', 'events'):
            assert a[key] == b[key]
        seen_native |= a['visited']
        seen_linked |= b['visited']
        native_branches |= a['branches']
        linked_branches |= b['branches']
        count += 1
    assert len(seen_native) == len(native) == 64
    assert len(seen_linked) == len(linked) == 64
    for words, outcomes in ((native, native_branches), (linked, linked_branches)):
        conditional = [i * 4 for i, w in enumerate(words)
                       if w >> 26 in (4, 5, 20, 21) and (w >> 21 & 31 or w >> 16 & 31)]
        for offset in conditional:
            assert (offset, False) in outcomes and (offset, True) in outcomes
    rejected = []
    for name, offset, transform in (
        ('wrong_clear_field', 0x38, lambda w: (w & 0xFFFF0000) | 0x2F),
        ('wrong_user_value', 0x54, lambda w: (w & 0xFFFF0000) | 2),
        ('wrong_invalidation', 0xE4, lambda w: (w & 0xFFFF0000) | 0x64),
        ('unknown_instruction', 0x2C, lambda w: 0xFC000000)):
        mutated = list(native)
        mutated[offset // 4] = transform(mutated[offset // 4])
        try:
            run(mutated, (7, 0, 1, 255, 0, 3, 1))
        except AssertionError:
            rejected.append(name)
        else:
            raise AssertionError('mutant survived: ' + name)
    return dict(cases=count, native_and_linked_executions=count * 2,
                native_covered_offsets=sorted(seen_native), linked_covered_offsets=sorted(seen_linked),
                native_branch_outcomes=[list(item) for item in sorted(native_branches)],
                linked_branch_outcomes=[list(item) for item in sorted(linked_branches)],
                negative_controls_rejected=rejected,
                scope='Bounded packed-state and queue domain; helper is an explicit mutation hook, not its real body')
