"""Bounded integer MIPS model for the clipped texture-rectangle leaf.

No original instructions or ROM bytes are embedded. The caller supplies a
protected, integrity-checked stream. Unknown instructions, misaligned/unmapped
accesses, control-flow escapes, and callee-save damage fail closed. Arithmetic
uses the native modulo-2**32 operations; this is not an ISO-C overflow promise.
"""
MASK = 0xffffffff
GLOBALS = ['D_8012E608', 'D_8012E60C', 'D_8012E610', 'D_8012E668',
           'D_8012E674', 'D_8014A248']


def signed(value, width=32):
    value &= (1 << width) - 1
    return value - (1 << width) if value & (1 << (width - 1)) else value


def execute(words, start, symbols, arguments, state):
    """Return the command words, pointer advance, events and executed offsets.

    Valid readable globals and a writable, aligned, disjoint command buffer are
    mapped explicitly. Outgoing O32 stack arguments five and six are word-sized.
    Both ends of the command buffer are guarded and no extra writes are allowed.
    """
    stack, display = 0x700080, 0x100000
    memory = {stack + i: (0xD00D0000 + i) & MASK for i in range(-128, 128, 4)}
    memory[stack + 16], memory[stack + 20] = [a & MASK for a in arguments[4:]]
    initial_globals = {symbols[name]: value & MASK for name, value in zip(GLOBALS, state)}
    memory.update(initial_globals)
    holder = symbols['D_80149438']
    memory[holder] = display
    sentinel = [0xa55a0000 + i for i in range(8)]
    memory.update({display - 4 + i * 4: value for i, value in enumerate(sentinel)})
    external = set(initial_globals) | {holder} | set(range(display - 4, display + 28, 4))
    registers = [0xa5000000 + i for i in range(32)]
    registers[0], registers[29], registers[31] = 0, stack, 0xfffffffc
    registers[4:8] = [a & MASK for a in arguments[:4]]
    original = list(registers)
    events, visited = [], set()
    pc, pending, steps = start, None, 0

    def access(address, value=None):
        assert address % 4 == 0, 'misaligned memory access'
        assert address in memory, 'unmapped memory access'
        if address in external:
            events.append(('read' if value is None else 'write', address))
        if value is None:
            return memory[address]
        memory[address] = value & MASK

    while pc != 0xfffffffc:
        assert start <= pc < start + 4 * len(words) and steps < 1000, 'escaped control flow'
        assert (pc - start) % 4 == 0, 'misaligned instruction'
        visited.add(pc - start)
        instruction = words[(pc - start) // 4]
        opcode, function = instruction >> 26, instruction & 63
        rs, rt, rd, sa = [(instruction >> shift) & 31 for shift in (21, 16, 11, 6)]
        immediate = instruction & 0xffff
        displacement = signed(immediate, 16)
        address = (registers[rs] + displacement) & MASK
        delayed, pending, following = pending, None, pc + 4
        if opcode == 0:
            if function == 0:
                registers[rd] = registers[rt] << sa
            elif function == 8:
                pending = registers[rs]
            elif function == 33:
                registers[rd] = registers[rs] + registers[rt]
            elif function == 35:
                registers[rd] = registers[rs] - registers[rt]
            elif function == 36:
                registers[rd] = registers[rs] & registers[rt]
            elif function == 37:
                registers[rd] = registers[rs] | registers[rt]
            elif function == 42:
                registers[rd] = int(signed(registers[rs]) < signed(registers[rt]))
            else:
                raise AssertionError('unsupported SPECIAL operation')
        elif opcode == 9:
            registers[rt] = address
        elif opcode == 12:
            registers[rt] = registers[rs] & immediate
        elif opcode == 13:
            registers[rt] = registers[rs] | immediate
        elif opcode == 15:
            registers[rt] = immediate << 16
        elif opcode in (4, 5, 20, 21):
            take = (registers[rs] == registers[rt]) == (opcode in (4, 20))
            if take:
                pending = pc + 4 + 4 * displacement
            elif opcode in (20, 21):
                following += 4
        elif opcode == 35:
            registers[rt] = access(address)
        elif opcode == 43:
            access(address, registers[rt])
        else:
            raise AssertionError('unsupported opcode')
        registers = [r & MASK for r in registers]
        registers[0] = 0
        pc = delayed if delayed is not None else following
        steps += 1
    assert registers[29] == original[29], 'stack not restored'
    assert registers[16:24] == original[16:24], 'callee-save register damage'
    assert registers[28] == original[28] and registers[30] == original[30], 'callee-save register damage'
    assert all(memory[address] == value for address, value in initial_globals.items()), 'input global changed'
    assert memory[display - 4] == sentinel[0] and memory[display + 24] == sentinel[7], 'command buffer guard changed'
    advance = memory[holder] - display
    assert advance in (0, 24), 'unexpected display-list advance'
    expected = list(range(display, display + advance, 4))
    writes = [address for kind, address in events if kind == 'write' and address != holder]
    assert sorted(writes) == expected, 'missing, duplicate or unexpected external write'
    holder_writes = [address for kind, address in events if kind == 'write' and address == holder]
    assert len(holder_writes) == advance // 8, 'unexpected pointer publication count'
    if not advance:
        assert [memory[display - 4 + i * 4] for i in range(8)] == sentinel, 'rejected rectangle touched output'
    return [memory[address] for address in expected], advance, events, visited
