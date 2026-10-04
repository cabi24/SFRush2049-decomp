"""Fail-closed MIPS-II reset replay; canonical words and synthetic memory only."""
MASK = 0xFFFFFFFF
STACK, RETURN = 0x700000, 0xFFFFFFFC
REGULAR, EXTERNAL = 0x80050D00, 0x80055000
COMMANDS = (7, 10, 128, 129, 64, 65, 91, 131, 132, 133)
VALUES = (127, 64, 64, 0, 0, 0, 0, 0, 64, 0)


def signed(value, bits=32):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def execute(words, channel, set_number, pattern, upper=0):
    regular = bytearray([pattern] * (8 * 16 * 134))
    external = bytearray([pattern] * (32 * 134))
    regions = {STACK: bytearray(512), REGULAR: regular, EXTERNAL: external}
    initialized, writes, calls = set(), [], []
    row_address = (EXTERNAL + channel * 134 if set_number == 255
                   else REGULAR + (set_number * 16 + channel) * 134)

    def memory(address, size, value=None):
        if address % size:
            raise ValueError('unaligned memory access')
        for base, data in regions.items():
            offset = address - base
            if 0 <= offset and offset + size <= len(data):
                if value is None:
                    if base == STACK and any(address + i not in initialized for i in range(size)):
                        raise ValueError('uninitialized stack read')
                    return int.from_bytes(data[offset:offset + size], 'big')
                if base != STACK:
                    if size != 1 or not row_address <= address < row_address + 134:
                        raise ValueError('unexpected row write')
                    writes.append(address)
                data[offset:offset + size] = (value & ((1 << (8 * size)) - 1)).to_bytes(size, 'big')
                initialized.update(range(address, address + size))
                return
        raise ValueError('unmapped memory access')

    regs = [0xA5000000 + i for i in range(32)]
    regs[0], regs[29], regs[31] = 0, STACK + 256, RETURN
    regs[4], regs[5] = channel | upper, set_number | upper
    saved = list(regs)
    start = 0x80020820
    pc, pending, steps = start, None, 0
    while pc != RETURN:
        if not start <= pc < start + 4 * len(words) or pc % 4 or steps >= 1000:
            raise ValueError('control/step bound')
        steps += 1
        word = words[(pc - start) // 4]
        op = word >> 26
        rs, rt, rd, shift = (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31, (word >> 6) & 31
        imm, simm = word & 65535, signed(word & 65535, 16)
        address = (regs[rs] + simm) & MASK
        old, pending, next_pc = pending, None, pc + 4
        if op == 0:
            fn = word & 63
            if fn == 0: regs[rd] = (regs[rt] << shift) & MASK
            elif fn == 8: pending = regs[rs]
            elif fn == 0x21: regs[rd] = (regs[rs] + regs[rt]) & MASK
            elif fn == 0x23: regs[rd] = (regs[rs] - regs[rt]) & MASK
            elif fn == 0x25: regs[rd] = regs[rs] | regs[rt]
            else: raise ValueError('unsupported SPECIAL')
        elif op == 3:
            regs[31] = pc + 8
            pending = ('call', ((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2), pc + 8)
        elif op == 9: regs[rt] = address
        elif op == 12: regs[rt] = regs[rs] & imm
        elif op == 15: regs[rt] = imm << 16
        elif op in (4, 5):
            take = regs[rs] == regs[rt] if op == 4 else regs[rs] != regs[rt]
            if take: pending = pc + 4 + 4 * simm
        elif op == 35: regs[rt] = memory(address, 4)
        elif op == 40: memory(address, 1, regs[rt])
        elif op == 43: memory(address, 4, regs[rt])
        else: raise ValueError('unsupported opcode')
        regs[0] = 0
        if old is not None and pending is not None:
            raise ValueError('control transfer in delay slot')
        if isinstance(old, tuple):
            _, destination, resume = old
            index = len(calls)
            if index < 10:
                expected = (COMMANDS[index], channel, set_number, VALUES[index])
                if destination != 0x80020610 or tuple(regs[4:8]) != expected:
                    raise ValueError('controller call contract')
                if index == 0:
                    if len(writes) != 134 or set(writes) != set(range(row_address, row_address + 134)):
                        raise ValueError('clear write coverage')
                    if any(memory(row_address + i, 1) for i in range(134)):
                        raise ValueError('helper called before full clear')
                    memory(row_address, 1, 0xC5)
                elif memory(row_address, 1) != 0xC5:
                    raise ValueError('lost helper mutation')
                memory(row_address + COMMANDS[index], 1, VALUES[index])
                calls.append((destination,) + expected)
            else:
                expected = (channel, set_number, 255 if index == 10 else 0)
                if index not in (10, 11) or destination != (0x80020F4C if index == 10 else 0x80020FDC):
                    raise ValueError('unexpected trailing call')
                if tuple(regs[4:7]) != expected:
                    raise ValueError('trailing call contract')
                calls.append((destination,) + expected)
            for reg in list(range(1, 16)) + [24, 25]:
                regs[reg] = 0xBAD00000 + reg
            pc = resume
        else:
            pc = old if old is not None else next_pc
    if len(calls) != 12:
        raise ValueError('missing calls')
    if (regs[29] != saved[29] or regs[16:24] != saved[16:24]
            or regs[28] != saved[28] or regs[30] != saved[30]):
        raise ValueError('callee-save/stack violation')
    return bytes(regular), bytes(external), calls
