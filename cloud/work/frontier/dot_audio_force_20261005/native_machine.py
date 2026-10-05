"""Fail-closed MIPS-II leaf replay for the observed get_force_and_peak ABI.

Only data supplied by the harness is mapped. Float operations round to binary32.
NaN payloads and FCSR exception flags are outside this bounded model.
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


def signed(word, width=32):
    return word - (1 << width) if word & (1 << (width - 1)) else word


def execute(words, start, memory, player, index, vector_address, threshold,
            clock_address, clock_values):
    memory = dict(memory)
    stack = 0x700080
    memory.update({stack + i: 0xD00D0000 + i for i in range(0, 32, 4)})
    r = [0xA5000000 + i for i in range(32)]
    f = [0x7FC00000 + i for i in range(32)]
    r[0], r[4], r[5], r[6] = 0, player, index & 0xffffffff, vector_address
    r[29], r[31], f[18] = stack, 0xfffffffc, threshold
    original, original_f = list(r), list(f)
    pc, pending, condition = start, None, False
    visited, events, timer_reads = set(), [], 0

    def access(address, word=None):
        nonlocal timer_reads
        assert address % 4 == 0 and address in memory, ('unmapped/unaligned', hex(address))
        if word is None:
            if address == clock_address:
                memory[address] = clock_values[min(timer_reads, len(clock_values)-1)]
                timer_reads += 1
            events.append(('read', address))
            return memory[address]
        events.append(('write', address))
        memory[address] = word & 0xffffffff

    for _ in range(200):
        if pc == 0xfffffffc:
            break
        assert start <= pc < start + 4*len(words) and (pc-start)%4 == 0, 'escaped control flow'
        visited.add(pc-start)
        word = words[(pc-start)//4]
        op, rs, rt, rd, sa = word>>26, (word>>21)&31, (word>>16)&31, (word>>11)&31, (word>>6)&31
        imm = word & 0xffff
        offset = signed(imm, 16)
        address = (r[rs] + offset) & 0xffffffff
        delayed, pending, following = pending, None, pc+4
        if word == 0:
            pass
        elif op == 0:
            fn = word & 63
            if fn == 0: r[rd] = (r[rt] << sa) & 0xffffffff
            elif fn == 3: r[rd] = (signed(r[rt]) >> sa) & 0xffffffff
            elif fn == 0x21: r[rd] = (r[rs]+r[rt]) & 0xffffffff
            elif fn == 0x23: r[rd] = (r[rs]-r[rt]) & 0xffffffff
            elif fn == 8: pending = r[rs]
            else: raise AssertionError(('unsupported SPECIAL', hex(word)))
        elif op == 9: r[rt] = address
        elif op == 15: r[rt] = imm << 16
        elif op == 35: r[rt] = access(address)
        elif op == 43: access(address, r[rt])
        elif op in (4, 5, 20, 21):
            take = (r[rs] == r[rt]) == (op in (4,20))
            if take: pending = pc+4+4*offset
            elif op in (20,21): following += 4
        elif op == 49: f[rt] = access(address)
        elif op == 57: access(address, f[rt])
        elif op == 17:
            if rs == 4: f[rd] = r[rt]
            elif rs == 8:
                assert rt in (0,1,2,3), 'unsupported COP1 branch'
                take = condition == bool(rt&1)
                if take: pending = pc+4+4*offset
                elif rt&2: following += 4
            elif rs == 16:
                fn = word & 63
                a,b = value(f[rd]),value(f[rt])
                if fn == 0: f[sa] = bits(a+b)
                elif fn == 1: f[sa] = bits(a-b)
                elif fn == 2: f[sa] = bits(a*b)
                elif fn == 4: f[sa] = bits(math.sqrt(a) if a >= 0 else float('nan'))
                elif fn == 0x32: condition = a == b
                elif fn == 0x3c: condition = a < b
                else: raise AssertionError(('unsupported COP1 operation', hex(word)))
            else: raise AssertionError(('unsupported COP1 format', hex(word)))
        else: raise AssertionError(('unsupported opcode', hex(word)))
        r[0] = 0
        pc = delayed if delayed is not None else following
    else:
        raise AssertionError('step limit')
    assert r[16:24] == original[16:24] and r[28:] == original[28:], 'integer callee-save damage'
    assert f[22:] == original_f[22:], 'unexpected FP callee-save damage'
    # Native IPA explicitly clobbers f20; this is not an ordinary O32 export.
    assert memory[stack] == (index & 0xffffffff), 'wrong index parameter home'
    assert all(memory[stack+i] == 0xD00D0000+i for i in range(4,32,4)), 'stack overwrite'
    return memory, visited, timer_reads, events
