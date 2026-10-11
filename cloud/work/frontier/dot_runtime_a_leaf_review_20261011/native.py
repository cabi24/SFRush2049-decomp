"""Fail-closed MIPS-II execution for the availability-family review.

Instruction words are supplied from authenticated targets or newly linked C.
External helpers are explicit test fixtures, never matching source context.
"""
import random

RETURN = 0x81234560
SP = 0x81010000
HELPERS = (0x800A1A3C, 0x800A35F8, 0x80094FC4)
CLEANUP_HELPERS = (0x80090254, 0x800DC720, 0x800B358C, 0x800B557C)


def require(ok, detail):
    if not ok:
        raise ValueError(detail)


def signed(value, bits=32):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def read(memory, address, size):
    require(address % size == 0 and all(address+i in memory for i in range(size)),
            ('unmapped/unaligned read', hex(address), size))
    return int.from_bytes(bytes(memory[address+i] for i in range(size)), 'big')


def write(memory, address, size, value):
    require(address % size == 0 and all(address+i in memory for i in range(size)),
            ('unmapped/unaligned write', hex(address), size))
    for i, byte in enumerate((value & ((1 << (8*size))-1)).to_bytes(size, 'big')):
        memory[address+i] = byte


def run(functions, initial, entry, argument, helper_results, salt=0):
    rng = random.Random(salt ^ 0x92F28)
    registers = [rng.getrandbits(32) for _ in range(32)]
    registers[0] = 0
    registers[4] = argument & 0xFFFFFFFF
    registers[29] = SP
    registers[31] = RETURN
    before = list(registers)
    memory = dict(initial)
    pc, pending = entry, None
    code = {base+4*i: word for base, words in functions.items()
            for i, word in enumerate(words)}
    coverage, branches, reads, writes, calls = set(), set(), [], [], []
    for step in range(10000):
        if pc == RETURN:
            require(pending is None, 'pending branch at return')
            return dict(registers=registers, initial_registers=before, memory=memory,
                        coverage=coverage, branches=branches, reads=reads,
                        writes=writes, calls=calls)
        if pc in HELPERS or pc in CLEANUP_HELPERS:
            require(pending is None, 'helper entered in delay slot')
            slot = registers[4]
            if pc in HELPERS:
                require(0 <= slot < 4, ('invalid helper slot', slot))
                calls.append((pc, slot))
                result = helper_results[HELPERS.index(pc)][slot]
            else:
                calls.append((pc, None if pc == CLEANUP_HELPERS[3] else slot))
                result = 0
            # Exercise the real O32 caller-saved contract, not accidental
            # preservation from a trivial C stub. Saved registers survive.
            resume = registers[31]
            for index in (*range(1,16), 24, 25):
                registers[index] = rng.getrandbits(32)
            registers[2] = result & 0xFFFFFFFF
            pc = resume
            continue
        require(pc in code and pc % 4 == 0, ('escaped code', hex(pc)))
        coverage.add(pc)
        word = code[pc]
        op, rs, rt, rd, fn = word >> 26, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31, word & 63
        immediate = signed(word, 16)
        delayed, pending, transfer, annul = pending, None, None, False
        if word == 0:
            pass
        elif op == 0:
            if fn == 0:
                registers[rd] = (registers[rt] << (word >> 6 & 31)) & 0xFFFFFFFF
            elif fn == 3:
                registers[rd] = (signed(registers[rt]) >> (word >> 6 & 31)) & 0xFFFFFFFF
            elif fn == 33:
                registers[rd] = (registers[rs]+registers[rt]) & 0xFFFFFFFF
            elif fn == 35:
                registers[rd] = (registers[rs]-registers[rt]) & 0xFFFFFFFF
            elif fn == 37:
                registers[rd] = registers[rs] | registers[rt]
            elif fn == 42:
                registers[rd] = int(signed(registers[rs]) < signed(registers[rt]))
            elif fn == 8:
                require(rs == 31 and word & 0x1FFFFF == 8, 'unexpected indirect jump')
                transfer = registers[rs]
            else:
                raise ValueError(('unknown special', hex(pc), fn))
        elif op == 15:
            require(rs == 0, 'malformed lui')
            registers[rt] = (word & 65535) << 16
        elif op == 9:
            registers[rt] = (registers[rs]+immediate) & 0xFFFFFFFF
        elif op == 10:
            registers[rt] = int(signed(registers[rs]) < immediate)
        elif op in (32, 33, 35, 36):
            size = {32:1,33:2,35:4,36:1}[op]
            address = (registers[rs]+immediate) & 0xFFFFFFFF
            value = read(memory,address,size)
            reads.append((address,size))
            registers[rt] = (signed(value,8*size) if op in (32,33) else value) & 0xFFFFFFFF
        elif op in (40,43):
            size = 1 if op == 40 else 4
            address = (registers[rs]+immediate) & 0xFFFFFFFF
            write(memory,address,size,registers[rt])
            writes.append((address,size,registers[rt] & ((1 << (8*size))-1)))
        elif op in (4,5,20,21):
            taken = (registers[rs] == registers[rt]) if op in (4,20) else (registers[rs] != registers[rt])
            branches.add((pc,taken))
            annul = op in (20,21) and not taken
            transfer = pc+4+immediate*4 if taken else pc+8
        elif op == 3:
            transfer = ((pc+4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
            require(transfer in functions or transfer in HELPERS or transfer in CLEANUP_HELPERS, ('unknown call',hex(transfer)))
            registers[31] = pc+8
        else:
            raise ValueError(('unknown opcode',hex(pc),op))
        registers[0] = 0
        if delayed is not None:
            require(transfer is None, 'control transfer in delay slot')
            pc = delayed
        elif annul:
            pc += 8
        else:
            pc,pending = pc+4,transfer
    raise ValueError('execution limit')
