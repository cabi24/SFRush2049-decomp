"""Bounded integer MIPS-II replay of the canonical variable-arithmetic body.
Only synthetic mapped memory and two explicit external-helper contracts exist.
No ROM image or native words are stored here. Unknown instructions fail closed.
"""
MASK = 0xFFFFFFFF


def signed(value, bits=32):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def execute(words, memory_regions, helper, arguments, stack_fill=0x5A, max_steps=50000):
    STACK, RETURN = 0x700000, 0xFFFFFFFC
    regions = {base: bytearray(data) for base, data in memory_regions.items()}
    regions[STACK] = bytearray([stack_fill])*512
    initialized = set()
    uninitialized = []
    calls = []
    visited, reads = [], []

    def memory(address, size, value=None):
        if address % size:
            raise ValueError('unaligned access')
        for base, data in regions.items():
            offset = address - base
            if 0 <= offset and offset + size <= len(data):
                if value is None:
                    reads.append((address, size))
                    if base == STACK and any(address+i not in initialized for i in range(size)):
                        uninitialized.append((address, size))
                    return int.from_bytes(data[offset:offset+size], 'big')
                data[offset:offset+size] = (value & ((1 << (8*size))-1)).to_bytes(size, 'big')
                initialized.update(range(address, address+size))
                return
        raise ValueError('unmapped access')

    registers = [0xA5000000+i for i in range(32)]
    registers[0] = 0
    registers[29], registers[31] = STACK+256, RETURN
    registers[4:7] = arguments
    saved = list(registers)
    start = 0x80023BDC
    pc, pending, low, steps = start, None, 0, 0
    while pc != RETURN:
        if not start <= pc < start+4*len(words) or pc % 4 or steps >= max_steps:
            raise ValueError('control/step bound')
        steps += 1
        visited.append(pc)
        word = words[(pc-start)//4]
        op = word >> 26
        rs, rt, rd, shift = (word>>21)&31, (word>>16)&31, (word>>11)&31, (word>>6)&31
        imm, simm = word&65535, signed(word&65535, 16)
        address = (registers[rs]+simm)&MASK
        old, pending, next_pc = pending, None, pc+4
        if op == 0:
            fn = word&63
            if fn == 0: registers[rd] = (registers[rt] << shift)&MASK
            elif fn == 2: registers[rd] = registers[rt] >> shift
            elif fn == 3: registers[rd] = (signed(registers[rt]) >> shift)&MASK
            elif fn == 8: pending = registers[rs]
            elif fn == 0x12: registers[rd] = low
            elif fn == 0x19: low = (registers[rs]*registers[rt])&MASK
            elif fn == 0x1A:
                numerator, denominator = signed(registers[rs]), signed(registers[rt])
                if denominator == 0 or (numerator == -2147483648 and denominator == -1):
                    raise ValueError('invalid signed division')
                low = ((abs(numerator)//abs(denominator)) * (-1 if (numerator<0)!=(denominator<0) else 1))&MASK
            elif fn == 0x21: registers[rd] = (registers[rs]+registers[rt])&MASK
            elif fn == 0x23: registers[rd] = (registers[rs]-registers[rt])&MASK
            elif fn == 0x25: registers[rd] = registers[rs] | registers[rt]
            elif fn == 0x2A: registers[rd] = int(signed(registers[rs]) < signed(registers[rt]))
            elif fn == 0x2B: registers[rd] = int(registers[rs] < registers[rt])
            else: raise ValueError('unsupported SPECIAL')
        elif op == 3:
            registers[31] = pc+8
            pending = ('call', ((pc+4)&0xF0000000)|((word&0x3FFFFFF)<<2), pc+8)
        elif op == 9: registers[rt] = address
        elif op == 10: registers[rt] = int(signed(registers[rs]) < simm)
        elif op == 11: registers[rt] = int(registers[rs] < (simm&MASK))
        elif op == 12: registers[rt] = registers[rs] & imm
        elif op == 13: registers[rt] = registers[rs] | imm
        elif op == 15: registers[rt] = imm << 16
        elif op in (4,5,20,21):
            take = registers[rs] == registers[rt] if op in (4,20) else registers[rs] != registers[rt]
            if take: pending = pc+4+4*simm
            elif op in (20,21): next_pc += 4
        elif op in (32,33,35,36,37):
            size = 4 if op == 35 else 2 if op in (33,37) else 1
            value = memory(address,size)
            registers[rt] = (signed(value,8 if op == 32 else 16)&MASK) if op in (32,33) else value
        elif op in (40,41,43): memory(address,4 if op == 43 else 2 if op == 41 else 1,registers[rt])
        else: raise ValueError('unsupported opcode')
        registers[0] = 0
        if old is not None and pending is not None:
            raise ValueError('control transfer in delay slot')
        if isinstance(old,tuple):
            _, destination, resume = old
            arities = {0x80023AD4:3, 0x80023B50:4}
            if destination not in arities:
                raise ValueError('unexpected external callee')
            argc = arities[destination]
            arguments = tuple(registers[4:4+min(argc,4)])
            if argc == 5: arguments += (memory(registers[29]+16,4),)
            returned, event = helper(destination, arguments, memory, len(calls))
            calls.append(event)
            for reg in list(range(1,16))+[24,25]: registers[reg] = 0xBAD00000+reg
            registers[2] = returned & MASK
            low = 0xDEADBEEF
            pc = resume
        else:
            pc = old if old is not None else next_pc
    if (registers[29] != saved[29] or registers[16:24] != saved[16:24]
            or registers[28] != saved[28] or registers[30] != saved[30]):
        raise ValueError('callee-save/stack violation')
    return {'result': registers[2], 'regions': {base: bytes(data) for base, data in regions.items() if base != STACK},
            'calls': calls, 'steps': steps, 'uninitialized_stack_reads': uninitialized,
            'visited': visited, 'reads': reads}
