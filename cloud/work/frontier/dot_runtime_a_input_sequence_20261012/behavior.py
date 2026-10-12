"""Bounded MIPS-II/IEEE single execution and independent input-state model.

This is a fail-closed fixture interpreter, not a console/FCSR emulator. Cases
use finite single-precision clocks, valid step storage and a stable clock.
"""
import itertools
import random
import struct

ENTRY = 0x8038F648
SEQUENCE = 0x80700000
POINTER, DEADLINE, STEP = 0x803B4278, 0x803B427C, 0x803B4280
CLOCK, HELD, PRESSED = 0x8002EB90, 0x80156944, 0x8015694C
RETURN, SP = 0x81234560, 0x81010000


def require(value, detail):
    if not value:
        raise ValueError(detail)


def bits(value):
    return struct.unpack('>I', struct.pack('>f', value))[0]


def floating(value):
    return struct.unpack('>f', struct.pack('>I', value))[0]


def signed(value, width=32):
    value &= (1 << width) - 1
    return value - (1 << width) if value >> (width - 1) else value


def load(memory, address, size):
    require(address % size == 0 and all(address + i in memory for i in range(size)), 'invalid memory read')
    return int.from_bytes(bytes(memory[address + i] for i in range(size)), 'big')


def store(memory, address, size, value):
    require(address % size == 0 and all(address + i in memory for i in range(size)), 'invalid memory write')
    for i, byte in enumerate((value & ((1 << (8 * size)) - 1)).to_bytes(size, 'big')):
        memory[address + i] = byte


def cases():
    rng = random.Random(0x8038F648)
    for count, clock, deadline, mode in itertools.product((1, 2, 20), (0., 1., 17.5), (0., 1., 18.5), range(6)):
        for step in sorted({0, count - 1}):
            held = [rng.randrange(0x10000) | 1 for _ in range(20)]
            pressed = [rng.randrange(0x10000) | 2 for _ in range(20)]
            active = 0 if step and deadline <= clock else step
            h, p = held[active], pressed[active]
            buttons = [h, p]
            if mode == 1:
                buttons[0] &= ~1
            elif mode == 2:
                buttons[1] &= ~2
            elif mode == 3:
                buttons[0] |= 0x80000000
            elif mode == 4:
                buttons[1] |= 0x80000000
            yield dict(present=mode != 5, count=count, step=step, clock=bits(clock),
                       deadline=bits(deadline), held=held, pressed=pressed, buttons=buttons)
    for _ in range(128):
        count = rng.randrange(1, 21)
        yield dict(present=True, count=count, step=rng.randrange(count),
                   clock=bits(rng.uniform(-100, 100)), deadline=bits(rng.uniform(-100, 100)),
                   held=[rng.getrandbits(32) for _ in range(20)],
                   pressed=[rng.getrandbits(32) for _ in range(20)],
                   buttons=[rng.getrandbits(32), rng.getrandbits(32)])


def state(case):
    ranges = [(SEQUENCE, 164), (POINTER, 12), (CLOCK, 4), (HELD, 4), (PRESSED, 4)]
    memory = {start + i: 0xA5 for start, size in ranges for i in range(size)}
    for address, value in ((POINTER, SEQUENCE if case['present'] else 0), (DEADLINE, case['deadline']),
                           (STEP, case['step']), (CLOCK, case['clock']),
                           (HELD, case['buttons'][0]), (PRESSED, case['buttons'][1])):
        store(memory, address, 4, value)
    store(memory, SEQUENCE, 1, case['count'])
    for offset, values in ((4, case['held']), (84, case['pressed'])):
        for i, value in enumerate(values):
            store(memory, SEQUENCE + offset + 4 * i, 4, value)
    return memory


def model(case):
    step, deadline, result = case['step'], case['deadline'], 0
    if case['present']:
        if step and floating(deadline) <= floating(case['clock']):
            step = 0
        held, pressed = case['held'][step], case['pressed'][step]
        other = ~(held | pressed) & 0xFFFFFFFF
        if case['buttons'][1] & other:
            step = 0
        elif (case['buttons'][0] & held == held and case['buttons'][1] & pressed == pressed
              and not case['buttons'][0] & other):
            step += 1
            deadline = bits(floating(case['clock']) + 1.)
            result = int(step == case['count'])
    return result, step, deadline


def run(words, case, salt):
    rng = random.Random(salt)
    registers = [rng.getrandbits(32) for _ in range(32)]
    fp = [rng.getrandbits(32) for _ in range(32)]
    registers[0], registers[29], registers[31] = 0, SP, RETURN
    before, fp_before = registers[:], fp[:]
    initial = state(case)
    memory = dict(initial)
    pc, pending, condition = ENTRY, None, False
    coverage, branches, reads, writes = set(), set(), [], []
    for _ in range(256):
        if pc == RETURN:
            require(pending is None, 'pending branch on return')
            require(all(registers[i] == before[i] for i in (*range(16, 24), 28, 29, 30, 31)), 'saved-register/stack ABI')
            require(fp[20:] == fp_before[20:], 'saved FP ABI')
            require(all(memory[a] == value for a, value in initial.items() if not DEADLINE <= a < STEP + 4), 'unexpected memory mutation')
            return dict(output=(registers[2], load(memory, STEP, 4), load(memory, DEADLINE, 4)),
                        coverage=coverage, branches=branches, reads=reads, writes=writes)
        require(ENTRY <= pc < ENTRY + len(words) * 4 and pc % 4 == 0, 'escaped function')
        coverage.add(pc - ENTRY)
        word = words[(pc - ENTRY) // 4]
        op, rs, rt, rd, fn = word >> 26, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31, word & 63
        immediate = signed(word, 16)
        delayed, pending, transfer, annul = pending, None, None, False
        if op == 0:
            if fn == 0:
                registers[rd] = registers[rt] << (word >> 6 & 31)
            elif fn == 33:
                registers[rd] = registers[rs] + registers[rt]
            elif fn == 36:
                registers[rd] = registers[rs] & registers[rt]
            elif fn == 37:
                registers[rd] = registers[rs] | registers[rt]
            elif fn == 39:
                registers[rd] = ~(registers[rs] | registers[rt])
            elif fn == 8:
                require(rs == 31, 'unexpected indirect branch')
                transfer = registers[rs]
            else:
                raise ValueError('unsupported special')
        elif op == 15:
            registers[rt] = (word & 65535) << 16
        elif op == 9:
            registers[rt] = registers[rs] + immediate
        elif op in (35, 36, 49):
            size = 1 if op == 36 else 4
            address = (registers[rs] + immediate) & 0xFFFFFFFF
            value = load(memory, address, size)
            reads.append((address, size))
            if op == 49:
                fp[rt] = value
            else:
                registers[rt] = value
        elif op in (43, 57):
            address = (registers[rs] + immediate) & 0xFFFFFFFF
            value = registers[rt] if op == 43 else fp[rt]
            store(memory, address, 4, value)
            writes.append((address, 4, value))
        elif op in (4, 5, 20, 21):
            taken = (registers[rs] == registers[rt]) if op in (4, 20) else (registers[rs] != registers[rt])
            branches.add((pc - ENTRY, taken))
            transfer = pc + 4 + immediate * 4 if taken else pc + 8
            annul = op in (20, 21) and not taken
        elif op == 17:
            if rs == 4:
                fp[rd] = registers[rt]
            elif rs == 16 and fn == 62:
                condition = floating(fp[rd]) <= floating(fp[rt])
            elif rs == 16 and fn == 0:
                fp[word >> 6 & 31] = bits(floating(fp[rd]) + floating(fp[rt]))
            elif rs == 8 and rt == 2:
                taken = not condition
                branches.add((pc - ENTRY, taken))
                transfer = pc + 4 + immediate * 4 if taken else pc + 8
                annul = not taken
            else:
                raise ValueError('unsupported cop1')
        else:
            raise ValueError('unsupported opcode')
        registers = [value & 0xFFFFFFFF for value in registers]
        registers[0] = 0
        if delayed is not None:
            require(transfer is None, 'branch in delay slot')
            pc = delayed
        elif annul:
            pc += 8
        else:
            pc, pending = pc + 4, transfer
    raise ValueError('execution budget exceeded')
