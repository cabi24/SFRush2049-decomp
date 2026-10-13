"""Bounded integer O32 replay for the protected 10DD8 words; no embedded ROM.

Unknown instructions, unmapped or unaligned accesses, unexpected callbacks,
stack writes, and zero divisors fail closed. This models architectural integer
results and delay slots, not R4300 timing or a complete emulator.
"""

def require(condition, message):
    if not condition:
        raise AssertionError(message)
MASK = 4294967295
BASE = 2147552728
CALLBACK = 2164264960
GLOBALS = (2147713780, 2147713768, 2147713776)
SP = 1879048320
STOP = 4294967292

def signed(value, bits=32):
    return value - (1 << bits) if value & 1 << bits - 1 else value

def execute(words, rate, duration, result, count, state, buffer, coverage=None):
    regs = [3405643776 + i for i in range(32)]
    regs[0], regs[4], regs[5], regs[29], regs[31] = (0, rate, duration, SP, STOP)
    original = list(regs)
    memory = {2147713048: CALLBACK, **dict(zip(GLOBALS, (count, state, buffer)))}
    stack = {SP + i: 3452816845 for i in range(-128, 128, 4)}
    stack_writes, global_writes, observed = ([], [], [0] * 6)
    allowed_stack = {SP - 4, SP, SP + 4}
    pc, pending, hi, lo = (BASE, None, 0, 0)
    for step in range(128):
        if pc == STOP:
            break
        require(BASE <= pc < BASE + len(words) * 4 and pc % 4 == 0, 'invalid PC')
        if coverage is not None:
            coverage.add(pc - BASE)
        word = words[(pc - BASE) // 4]
        op, rs, rt, rd, shift = (word >> 26, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31, word >> 6 & 31)
        immediate = word & 65535
        address = regs[rs] + signed(immediate, 16) & MASK
        previous, pending, following = (pending, None, pc + 4)
        if op == 0:
            fn = word & 63
            if fn == 0:
                regs[rd] = regs[rt] << shift & MASK
            elif fn == 8:
                pending = regs[rs]
            elif fn == 9:
                require(regs[rs] == CALLBACK and rd == 31, 'unexpected callback')
                regs[rd] = pc + 8
                pending = ('call', pc + 8)
            elif fn == 16:
                regs[rd] = hi
            elif fn == 18:
                regs[rd] = lo
            elif fn == 25:
                product = regs[rs] * regs[rt]
                hi, lo = (product >> 32, product & MASK)
            elif fn == 27:
                require(regs[rt] != 0, 'zero divisor')
                lo, hi = divmod(regs[rs], regs[rt])
            elif fn == 33:
                regs[rd] = regs[rs] + regs[rt] & MASK
            elif fn == 35:
                regs[rd] = regs[rs] - regs[rt] & MASK
            elif fn == 37:
                regs[rd] = regs[rs] | regs[rt]
            else:
                raise AssertionError('unsupported SPECIAL instruction')
        elif op in (4, 5):
            take = regs[rs] == regs[rt] if op == 4 else regs[rs] != regs[rt]
            if take:
                pending = pc + 4 + 4 * signed(immediate, 16)
        elif op == 9:
            regs[rt] = address
        elif op == 12:
            regs[rt] = regs[rs] & immediate
        elif op == 15:
            regs[rt] = immediate << 16
        elif op in (35, 43):
            require(address % 4 == 0, 'unaligned memory')
            if op == 35:
                require(address in memory or address in stack, 'unmapped load')
                regs[rt] = (memory if address in memory else stack)[address]
            elif address in GLOBALS:
                memory[address] = regs[rt]
                global_writes.append(address)
            else:
                require(address in allowed_stack, 'unexpected store')
                stack[address] = regs[rt]
                stack_writes.append(address)
        else:
            raise AssertionError('unsupported instruction')
        regs[0] = 0
        if isinstance(previous, tuple):
            require(pending is None and regs[29] == SP - 24 and (regs[29] % 8 == 0), 'verification requirement failed')
            observed = [observed[0] + 1, regs[4], regs[5], *[memory[a] for a in GLOBALS]]
            for index in [*range(1, 16), 24, 25]:
                regs[index] = 3134193664 + index
            regs[2] = result
            hi, lo = (3134193665, 3134193666)
            pc = previous[1]
        else:
            pc = previous if previous is not None else following
    else:
        raise AssertionError('instruction budget exceeded')
    require(regs[16:24] == original[16:24], 'callee-saved registers changed')
    require(all((regs[i] == original[i] for i in (28, 29, 30, 31))), 'O32 state not restored')
    require(set(stack_writes) <= allowed_stack, 'verification requirement failed')
    require(all((value == 3452816845 for address, value in stack.items() if address not in allowed_stack)), 'verification requirement failed')
    expected_writes = [GLOBALS[0], GLOBALS[1], GLOBALS[2]] if duration & 65535 else [GLOBALS[2]]
    require(global_writes == expected_writes, 'global store order changed')
    return [memory[a] for a in GLOBALS] + observed
