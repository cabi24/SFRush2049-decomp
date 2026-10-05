"""Bounded binary32 MIPS-II model for the E0B8 leaf normalizer.

Reads complete instruction streams from existing protected repository targets;
no target instructions are embedded here. Unknown instructions, unmapped or
unaligned accesses, escaped control flow and O32 callee-save damage fail closed.
The model covers round-to-nearest arithmetic, delay slots and branch-likely
annulment. FCSR exception behavior and NaN payload identity are not modeled.
"""
import math
import struct


def bits(value):
    try:
        return struct.unpack('>I', struct.pack('>f', value))[0]
    except OverflowError:
        return 0xff800000 if value < 0 else 0x7f800000


def value(word):
    return struct.unpack('>f', struct.pack('>I', word))[0]


def signed(word, width):
    return word - (1 << width) if word & (1 << (width - 1)) else word


def canonical(word):
    return 0x7fc00000 if math.isnan(value(word)) else word


def divide(a, b):
    if b == 0.0:
        if a == 0.0 or math.isnan(a):
            return float('nan')
        return math.copysign(float('inf'), a * math.copysign(1.0, b))
    return a / b


def execute(words, start, vector_address, threshold_address, xyz, threshold):
    """Return f0 bits, xyz bits, and ordered external memory read/write events.

    A threshold address inside xyz aliases that component. The explicit
    threshold argument is ignored in that case. The stack is separately mapped.
    """
    memory = {vector_address + 4 * i: word for i, word in enumerate(xyz)}
    if threshold_address not in memory:
        memory[threshold_address] = threshold
    external = set(memory)
    stack_base = 0x700000
    memory.update({stack_base + i: 0xD00D0000 + i for i in range(0, 256, 4)})
    registers = [0xA5000000 + i for i in range(32)]
    registers[0], registers[4] = 0, vector_address
    registers[29], registers[31] = stack_base + 128, 0xFFFFFFFC
    fp = [0x7fc00000 + i for i in range(32)]
    original, original_fp = list(registers), list(fp)
    events = []
    condition, pc, pending, steps = False, start, None, 0

    def access(address, word=None):
        assert address % 4 == 0, 'unaligned memory access'
        assert address in memory, 'unmapped memory access'
        if address in external:
            events.append(('read' if word is None else 'write', address))
        if word is None:
            return memory[address]
        memory[address] = word & 0xffffffff

    while pc != 0xFFFFFFFC:
        assert start <= pc < start + len(words) * 4 and steps < 256, 'escaped control flow'
        assert not (pc - start) % 4, 'unaligned instruction'
        instruction = words[(pc - start) // 4]
        opcode = instruction >> 26
        rs, rt, rd, sa = [(instruction >> shift) & 31 for shift in (21, 16, 11, 6)]
        immediate = instruction & 0xffff
        displacement = signed(immediate, 16)
        address = (registers[rs] + displacement) & 0xffffffff
        delayed, pending, following = pending, None, pc + 4
        if instruction == 0:
            pass
        elif opcode == 0 and instruction & 63 == 8:
            pending = registers[rs]
        elif opcode == 9:
            registers[rt] = address
        elif opcode == 15:
            registers[rt] = immediate << 16
        elif opcode in (4, 5, 20, 21):
            take = (registers[rs] == registers[rt]) == (opcode in (4, 20))
            if take:
                pending = pc + 4 + 4 * displacement
            elif opcode in (20, 21):
                following += 4
        elif opcode == 49:
            fp[rt] = access(address)
        elif opcode == 57:
            access(address, fp[rt])
        elif opcode == 17:
            if rs == 4:
                fp[rd] = registers[rt]
            elif rs == 8:
                assert rt in (0, 1, 2, 3), 'unsupported COP1 branch'
                take = condition == bool(rt & 1)
                if take:
                    pending = pc + 4 + 4 * displacement
                elif rt & 2:
                    following += 4
            elif rs == 16:
                operation = instruction & 63
                a, b = value(fp[rd]), value(fp[rt])
                if operation == 0:
                    fp[sa] = bits(a + b)
                elif operation == 1:
                    fp[sa] = bits(a - b)
                elif operation == 2:
                    fp[sa] = bits(a * b)
                elif operation == 3:
                    fp[sa] = bits(divide(a, b))
                elif operation == 4:
                    fp[sa] = bits(math.sqrt(a) if a >= 0 else float('nan'))
                elif operation == 6:
                    fp[sa] = fp[rd]
                elif operation == 0x3c:
                    condition = a < b
                elif operation == 0x3e:
                    condition = a <= b
                else:
                    raise AssertionError('unsupported COP1 operation')
            else:
                raise AssertionError('unsupported COP1 format')
        else:
            raise AssertionError('unsupported instruction')
        registers[0] = 0
        pc = delayed if delayed is not None else following
        steps += 1
    assert registers[29] == original[29], 'stack not restored'
    assert registers[16:24] == original[16:24], 'callee-save register damage'
    assert registers[28] == original[28] and registers[30] == original[30], 'callee-save register damage'
    assert fp[20:] == original_fp[20:], 'callee-save FP damage'
    return fp[0], [memory[vector_address + i * 4] for i in range(3)], events
