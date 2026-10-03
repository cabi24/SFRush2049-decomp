"""Bounded MIPS-II leaf interpreter, fail-closed on unknown opcodes/accesses.
IEEE binary32 arithmetic in host round-to-nearest; FCSR exceptions and signaling
NaN payload propagation are outside this model. Delay slots and likely annulment
are executed explicitly. Inputs/outputs are unsigned 32-bit bit patterns.
"""
import math
import struct


def signed(value, bits=32):
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def to_float(bits):
    return struct.unpack('>f', struct.pack('>I', bits))[0]


def to_bits(value):
    try:
        return struct.unpack('>I', struct.pack('>f', value))[0]
    except OverflowError:
        return 0xff800000 if value < 0 else 0x7f800000


def execute(words, start, input_regions, arguments):
    regions = [(base, bytearray(data)) for base, data in input_regions]
    reads, writes = [], []
    def memory(address, width, value=None):
        assert address % width == 0, ('unaligned', address, width)
        for base, data in regions:
            offset = address - base
            if 0 <= offset and offset + width <= len(data):
                if value is None:
                    reads.append((address, width))
                    return int.from_bytes(data[offset:offset+width], 'big')
                writes.append((address, width))
                data[offset:offset+width] = (value & ((1 << (width*8))-1)).to_bytes(width, 'big')
                return
        raise AssertionError(('unmapped access', hex(address), width))
    r = [0xA5000000+i for i in range(32)]
    f = [0x7fc00000]*32
    r[0], r[29], r[31] = 0, 0x700080, 0xFFFFFFFC
    r[4:8] = arguments
    regions.append((0x700000, bytearray(256)))
    original = list(r)
    pc, pending, condition, steps = start, None, False, 0
    while pc != 0xFFFFFFFC:
        assert start <= pc < start+4*len(words) and steps < 200, ('invalid control flow', hex(pc))
        w = words[(pc-start)//4]; op = w >> 26
        rs, rt, rd, sa = (w>>21)&31, (w>>16)&31, (w>>11)&31, (w>>6)&31
        imm = w & 65535; si = signed(imm, 16)
        address = (r[rs]+si)&0xffffffff
        old_pending, pending, next_pc = pending, None, pc+4
        if w == 0: pass
        elif op == 0:
            fn = w & 63
            if fn == 0: r[rd] = (r[rt] << sa)&0xffffffff
            elif fn == 0x21: r[rd] = (r[rs]+r[rt])&0xffffffff
            elif fn == 0x23: r[rd] = (r[rs]-r[rt])&0xffffffff
            elif fn == 0x25: r[rd] = r[rs] | r[rt]
            elif fn == 8: pending = r[rs]
            else: raise AssertionError(('unsupported SPECIAL', hex(w)))
        elif op == 15: r[rt] = imm << 16
        elif op == 9: r[rt] = address
        elif op in (4, 5, 20, 21):
            take = (r[rs] == r[rt]) if op in (4,20) else (r[rs] != r[rt])
            if take: pending = pc+4+4*si
            elif op in (20,21): next_pc += 4
        elif op in (32,33,35):
            width = {32:1,33:2,35:4}[op]
            r[rt] = signed(memory(address,width),width*8)&0xffffffff
        elif op == 49: f[rt] = memory(address,4)
        elif op == 57: memory(address,4,f[rt])
        elif op == 17:
            if rs == 4: f[rd] = r[rt]
            elif rs == 8:
                assert rt in (0,1,2,3)
                take = condition == bool(rt & 1)
                if take: pending = pc+4+4*si
                elif rt & 2: next_pc += 4
            elif rs == 16:
                fn = w & 63; a, b = to_float(f[rd]), to_float(f[rt])
                if fn == 0: f[sa] = to_bits(a+b)
                elif fn == 1: f[sa] = to_bits(a-b)
                elif fn == 2: f[sa] = to_bits(a*b)
                elif fn == 0x3c: condition = a < b
                else: raise AssertionError(('unsupported COP1',hex(w)))
            else: raise AssertionError(('unsupported COP1 format',hex(w)))
        else: raise AssertionError(('unsupported native word',hex(w),hex(pc)))
        r[0] = 0
        pc = old_pending if old_pending is not None else next_pc
        steps += 1
    assert r[29] == original[29], 'stack not restored'
    assert r[16:24] == original[16:24] and r[30] == original[30], 'callee-save violation'
    return r[2], regions[:-1], reads, writes
