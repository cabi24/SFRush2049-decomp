"""Fail-closed bounded MIPS-II replay using repository canonical words only.
All memory is synthetic. The setter's one external callee records its genuine
four-byte argument contract and clobbers every ordinary O32 volatile register.
"""
MASK = 0xFFFFFFFF
STACK, RETURN = 0x700000, 0xFFFFFFFC
MIDI, EFFECTS = 0x80050D00, 0x80055000


def signed(value, bits=32):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value & (1 << (bits-1)) else value


def execute(words, start, arguments, midi=b'', effects=b'', max_steps=500):
    regions = {STACK: bytearray(512), MIDI: bytes(midi), EFFECTS: bytes(effects)}
    calls, reads, initialized = [], [], set()

    def memory(address, size, value=None):
        if address % size:
            raise ValueError('unaligned memory access')
        for base, data in regions.items():
            offset = address-base
            if 0 <= offset and offset+size <= len(data):
                if value is None:
                    if base == STACK and any(address+i not in initialized for i in range(size)):
                        raise ValueError('uninitialized stack read')
                    if base != STACK:
                        reads.append(address)
                    return int.from_bytes(data[offset:offset+size], 'big')
                if base != STACK:
                    raise ValueError('unexpected non-stack write')
                data[offset:offset+size] = (value & ((1 << (8*size))-1)).to_bytes(size, 'big')
                initialized.update(range(address,address+size))
                return
        raise ValueError('unmapped memory access: %08X' % address)

    regs = [0xA5000000+i for i in range(32)]
    regs[0], regs[29], regs[31] = 0, STACK+256, RETURN
    for i, value in enumerate(arguments): regs[4+i] = value & MASK
    saved = list(regs)
    pc, pending, steps = start, None, 0
    while pc != RETURN:
        if not start <= pc < start+4*len(words) or pc % 4 or steps >= max_steps:
            raise ValueError('control/step bound')
        steps += 1
        word = words[(pc-start)//4]
        op = word >> 26
        rs,rt,rd,shift = (word>>21)&31,(word>>16)&31,(word>>11)&31,(word>>6)&31
        imm,simm = word&65535,signed(word&65535,16)
        address = (regs[rs]+simm)&MASK
        old,pending,next_pc = pending,None,pc+4
        if op == 0:
            fn = word&63
            if fn == 0: regs[rd] = (regs[rt]<<shift)&MASK
            elif fn == 2: regs[rd] = regs[rt]>>shift
            elif fn == 3: regs[rd] = (signed(regs[rt])>>shift)&MASK
            elif fn == 8: pending = regs[rs]
            elif fn == 0x21: regs[rd] = (regs[rs]+regs[rt])&MASK
            elif fn == 0x23: regs[rd] = (regs[rs]-regs[rt])&MASK
            elif fn == 0x25: regs[rd] = regs[rs]|regs[rt]
            else: raise ValueError('unsupported SPECIAL: %02X'%fn)
        elif op == 3:
            regs[31] = pc+8
            pending = ('call',((pc+4)&0xF0000000)|((word&0x3FFFFFF)<<2),pc+8)
        elif op == 9: regs[rt] = address
        elif op == 10: regs[rt] = int(signed(regs[rs]) < simm)
        elif op == 12: regs[rt] = regs[rs]&imm
        elif op == 15: regs[rt] = imm<<16
        elif op in (4,5,20,21):
            take = regs[rs] == regs[rt] if op in (4,20) else regs[rs] != regs[rt]
            if take: pending = pc+4+4*simm
            elif op in (20,21): next_pc += 4
        elif op in (35,36,37): regs[rt] = memory(address,{35:4,36:1,37:2}[op])
        elif op == 43: memory(address,4,regs[rt])
        else: raise ValueError('unsupported opcode: %02X'%op)
        regs[0] = 0
        if isinstance(old,tuple):
            _,destination,resume = old
            if destination != 0x80020610:
                raise ValueError('unexpected callee')
            if any(v > 255 for v in regs[4:8]):
                raise ValueError('un-normalized byte argument')
            calls.append(tuple(regs[4:8]))
            for reg in list(range(1,16))+[24,25]: regs[reg] = 0xBAD00000+reg
            pc = resume
        else:
            pc = old if old is not None else next_pc
    if (regs[29] != saved[29] or regs[16:24] != saved[16:24]
            or regs[28] != saved[28] or regs[30] != saved[30]):
        raise ValueError('callee-save/stack violation')
    return {'result':regs[2], 'calls':calls, 'reads':reads, 'steps':steps}
