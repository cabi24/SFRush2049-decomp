"""Fail-closed delayed-PC MIPS-II model for the actual three-body trig unit.

Binary32 operations round after each operation. FCSR exceptions, signaling NaNs,
concurrency, and hardware timing are outside this bounded instruction model.
"""
import math
import struct


def bits(x):
    try: return struct.unpack('>I', struct.pack('>f', x))[0]
    except OverflowError: return 0xff800000 if x < 0 else 0x7f800000


def value(x): return struct.unpack('>f', struct.pack('>I', x))[0]
def f(x): return value(bits(x))
def signed(x): return x - 0x100000000 if x & 0x80000000 else x


def execute(code, entry, data, x, mode, kernel=False):
    stop, sp = 0xfffffffc, 0x700080
    r = [0xa5000000 + i for i in range(32)]
    fp = [0x3f800000 + 17*i for i in range(32)]
    r[0], r[29], r[31] = 0, sp, stop
    r[4], fp[16 if kernel else 12] = mode, x
    before_r, before_f = list(r), list(fp)
    memory = dict(data)
    for offset in range(-64, 64, 4): memory[sp+offset] = 0xdad00000 + offset
    before_memory = dict(memory)
    pc, branch, condition = entry, None, False
    visited, outcomes, reads, writes = set(), set(), [], []

    def load(address):
        assert address % 4 == 0 and address in memory, ('unmapped read', hex(address))
        reads.append(address)
        return memory[address]

    def store(address, word):
        assert not kernel and address == sp-4, ('unexpected write', hex(address))
        assert address in memory
        writes.append(address)
        memory[address] = word

    for _ in range(300):
        if pc == stop: break
        assert pc in code, ('unmapped PC', hex(pc))
        visited.add(pc)
        word = code[pc]
        op, rs, rt, rd, sa = word>>26, (word>>21)&31, (word>>16)&31, (word>>11)&31, (word>>6)&31
        imm = word & 65535
        offset = imm-65536 if imm&32768 else imm
        delayed, branch, following = branch, None, pc+4
        if word == 0: pass
        elif op == 0:
            fn = word & 63
            if fn == 0: r[rd] = (r[rt] << sa) & 0xffffffff
            elif fn == 0x21: r[rd] = (r[rs]+r[rt]) & 0xffffffff
            elif fn == 0x23: r[rd] = (r[rs]-r[rt]) & 0xffffffff
            elif fn == 0x25: r[rd] = r[rs] | r[rt]
            elif fn == 8: branch = r[rs]
            else: raise AssertionError(('unknown SPECIAL', hex(word)))
        elif op == 3:
            assert delayed is None, 'branch in delay slot'
            r[31] = pc+8
            branch = ((pc+4)&0xf0000000) | ((word&0x3ffffff)<<2)
        elif op == 9: r[rt] = (r[rs]+offset)&0xffffffff
        elif op == 15: r[rt] = imm<<16
        elif op == 35: r[rt] = load((r[rs]+offset)&0xffffffff)
        elif op == 43: store((r[rs]+offset)&0xffffffff,r[rt])
        elif op in (4,5,20,21):
            take = (r[rs] == r[rt]) == (op in (4,20))
            outcomes.add((pc,take))
            if take: branch = pc+4+4*offset
            elif op in (20,21): following += 4
        elif op == 49: fp[rt] = load((r[rs]+offset)&0xffffffff)
        elif op == 17:
            if rs == 4: fp[rd] = r[rt]
            elif rs == 8:
                assert rt in (0,1,2,3)
                take = condition == bool(rt&1)
                outcomes.add((pc,take))
                if take: branch = pc+4+4*offset
                elif rt&2: following += 4
            elif rs == 16:
                fn = word & 63
                a,b = value(fp[rd]),value(fp[rt])
                if fn == 0: fp[sa] = bits(a+b)
                elif fn == 1: fp[sa] = bits(a-b)
                elif fn == 2: fp[sa] = bits(a*b)
                elif fn == 3:
                    if b == 0: fp[sa] = bits(float('nan') if a == 0 else math.copysign(float('inf'),a*b))
                    else: fp[sa] = bits(a/b)
                elif fn == 4: fp[sa] = bits(math.sqrt(a) if a >= 0 else float('nan'))
                elif fn == 5: fp[sa] = fp[rd] & 0x7fffffff
                elif fn == 6: fp[sa] = fp[rd]
                elif fn == 7: fp[sa] = fp[rd] ^ 0x80000000
                elif fn == 0x3c: condition = a < b
                elif fn == 0x3e: condition = a <= b
                else: raise AssertionError(('unknown COP1 op', hex(word)))
            else: raise AssertionError(('unknown COP1 format', hex(word)))
        else: raise AssertionError(('unknown opcode', hex(word)))
        r[0] = 0
        assert delayed is None or branch is None, 'branch in delay slot'
        pc = delayed if delayed is not None else following
    else: raise AssertionError('instruction limit exceeded')
    assert pc == stop and r[29] == sp and r[31] == stop
    assert all(r[i] == before_r[i] for i in [*range(16,24),28,30])
    assert all(fp[i] == before_f[i] for i in range(20,32))
    assert all(memory[a] == w for a,w in before_memory.items() if a not in writes)
    assert not kernel or not writes
    assert kernel or writes == [sp-4]
    return fp[0], visited, outcomes, reads, writes
