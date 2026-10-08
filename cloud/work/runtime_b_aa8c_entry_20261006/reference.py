"""Independent semantic oracle for the documented contract, not candidate code.

This direct algorithm has no MIPS/register/frame knowledge. The partial root C
is not compiled. Mutation switches are deliberate wrong-contract test controls.
"""
def short(value):
    return ((value & 65535) ^ 32768) - 32768


def cleanup(fixture, group, player, mutant=None):
    memory = fixture.memory
    base = (0x80399550 + player * 0x148) if group else (0x80399120 + player * 0x10C)
    for index in range(5):
        address = base + index * 4
        value = memory.get(address)
        absent = short(value) == -1 if mutant == 'narrow sentinel' else value == 0xFFFFFFFF
        if not absent:
            if mutant == 'early store': memory.put(address, -1)
            argument = value if mutant == 'wide removal argument' else short(value)
            fixture.service(0x80090254, [argument, 0, 0])
            memory.put(address, -1)
    if group:
        value = memory.get(base + 0x104)
        if value != 0xFFFFFFFF:
            fixture.service(0x80090254, [short(value), 0, 0])
            memory.put(base + 0x104, -1)
            memory.put(base + (0x138 if mutant == 'wrong trailing byte' else 0x139), -1, 1)
        elif mutant == 'unconditional extra clear':
            memory.put(base + 0x139, -1, 1)


def entry(fixture, mutant=None):
    memory, descriptor = fixture.memory, fixture.descriptor
    player = short(memory.get(descriptor + 8, 2))
    state = 0x80152818 + player * 0x3B8
    # The early shared color read is real, even on cleanup-only invocations.
    memory.get(0x80394884)
    update = fixture.update if mutant == 'wide update test' else short(fixture.update)
    cleanup_needed = update == 0
    if not cleanup_needed:
        inhibit = memory.get(0x8014A250 + player * 0x808 + 0x640, 1)
        cleanup_needed = (inhibit == 1) if mutant == 'inhibit exactly one' else (inhibit != 0)
    if not cleanup_needed:
        # The guard's taken branch-likely delay slot reads the next region's color.
        memory.get(state + 0x34C)
        return 'continue'
    player = short(memory.get(descriptor + 8, 2))
    kind = memory.get(state + 0x384, 1) if mutant == 'mode instead of cache' else memory.get(0x80399118 + player, 1)
    handle = short(memory.get(descriptor + 6, 2)) if mutant == 'early handle read' else None
    if kind in (0, 1): cleanup(fixture, kind == 0, player, mutant)
    if mutant == 'reload owner': state = 0x80152818 + short(memory.get(descriptor + 8, 2)) * 0x3B8
    memory.put(state + 0x384, 8, 1)
    memory.put(state + 0x385, -1, 1)
    if handle is None: handle = short(memory.get(descriptor + 6, 2))
    fixture.service(0x8008AE8C, [handle, 0, 15])
    return 'return'
