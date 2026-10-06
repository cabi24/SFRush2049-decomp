"""Small fail-closed MIPS executor for the entire image-A availability leaf.

Instructions come from authenticated target files or a GNU-linked ELF at run
time. No target instruction array is embedded in this module.
"""
import random
import struct

ENTRY = 0x80393004
RETURN = 0x81234560
ROW0 = 0x803BA7E0
ROW1 = 0x803BA7F0
TABLE = 0x803BA830
SELECTOR = 0x803B65A4


def signed(value, bits=32):
    return (value & ((1 << bits) - 1)) - ((value >> (bits - 1) & 1) << bits)


def fixture(index, counts, salt):
    """Four real table slots; distinct canaries surround both output rows."""
    rng = random.Random(salt)
    memory = {p: rng.randrange(256) for p in range(ROW0 - 16, ROW1 + 32)}
    memory[SELECTOR] = index & 255
    for i, value in enumerate(counts):
        for j, byte in enumerate(struct.pack('>i', value)):
            memory[TABLE + i * 4 + j] = byte
    return memory


def run(words, initial, salt=0):
    assert len(words) == 45, 'incomplete or excess native function extent'
    rng = random.Random(salt ^ 0xABC123)
    regs = [rng.getrandbits(32) for _ in range(32)]
    regs[0] = 0
    regs[31] = RETURN
    before = list(regs)
    memory = dict(initial)
    accesses, coverage, branches = [], set(), set()
    allowed_writes = set(range(ROW0, ROW0 + 11)) | set(range(ROW1, ROW1 + 11))
    pc, pending = ENTRY, None
    for steps in range(128):
        if pc == RETURN:
            assert pending is None, 'return with pending transfer'
            assert all(regs[i] == before[i] for i in (*range(16, 24), 28, 29, 30, 31)), 'callee-saved register changed'
            assert len([x for x in accesses if x[0] == 'write']) == 22
            return {'memory': memory, 'accesses': accesses,
                    'coverage': coverage, 'branches': branches, 'steps': steps}
        assert ENTRY <= pc < ENTRY + 180 and pc % 4 == 0, 'escaped function'
        off = pc - ENTRY
        coverage.add(off)
        word = words[off // 4]
        op, rs, rt, rd = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31
        imm = signed(word, 16)
        delayed, transfer = pending, None
        pending = None
        if word == 0:
            pass
        elif op == 15:
            assert rs == 0
            regs[rt] = (word & 65535) << 16
        elif op == 9:
            regs[rt] = (regs[rs] + imm) & 0xFFFFFFFF
        elif op == 10:
            regs[rt] = int(signed(regs[rs]) < imm)
        elif op == 32:
            address = (regs[rs] + imm) & 0xFFFFFFFF
            assert address == SELECTOR and address in memory, 'unexpected byte read'
            regs[rt] = signed(memory[address], 8) & 0xFFFFFFFF
            accesses.append(('read8', address, memory[address]))
        elif op == 35:
            address = (regs[rs] + imm) & 0xFFFFFFFF
            assert address in range(TABLE, TABLE + 16, 4), 'invalid count slot'
            regs[rt] = int.from_bytes(bytes(memory[address + j] for j in range(4)), 'big')
            accesses.append(('read32', address, regs[rt]))
        elif op == 40:
            address = (regs[rs] + imm) & 0xFFFFFFFF
            assert address in allowed_writes, 'unexpected output store'
            memory[address] = regs[rt] & 255
            accesses.append(('write', address, memory[address]))
        elif op == 5:
            taken = regs[rs] != regs[rt]
            branches.add((off, taken))
            transfer = pc + 4 + imm * 4 if taken else pc + 8
        elif op == 0 and word & 63 == 0:
            assert rs == 0
            regs[rd] = (regs[rt] << ((word >> 6) & 31)) & 0xFFFFFFFF
        elif op == 0 and word & 63 == 33:
            regs[rd] = (regs[rs] + regs[rt]) & 0xFFFFFFFF
        elif op == 0 and word & 63 == 37:
            regs[rd] = regs[rs] | regs[rt]
        elif op == 0 and word & 63 == 42:
            regs[rd] = int(signed(regs[rs]) < signed(regs[rt]))
        elif op == 0 and word & 63 == 8:
            assert rs == 31 and word & 0x1FFFFF == 8, 'unexpected indirect transfer'
            transfer = regs[rs]
        else:
            raise AssertionError('unsupported native instruction at +0x%x' % off)
        regs[0] = 0
        if delayed is not None:
            assert transfer is None, 'branch in delay slot'
            pc = delayed
        else:
            pc, pending = pc + 4, transfer
    raise AssertionError('native execution did not return')


def oracle(initial, index, count):
    memory = dict(initial)
    for row in (ROW0, ROW1):
        memory[row] = memory[row + 1] = 1
        for i in range(8):
            memory[row + 3 + i] = int(i < count)
    memory[ROW0 + 2] = 0
    memory[ROW1 + 2] = int(count < 1)
    return memory
