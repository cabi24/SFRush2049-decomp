"""Fail-closed MIPS interpreter for the two complete authenticated functions.

Services are explicit native ABI boundaries. No instruction or image bytes are
embedded. Single-precision arithmetic rounds after every native operation.
"""
import math
import struct

MASK = 0xffffffff
ROOT, PRIVATE = 0x800AF06C, 0x80090308
STACK, RETURN = 0x700100, 0xfffffffc


def signed(v, bits=32):
    v &= (1 << bits)-1
    return v - (1 << bits) if v & (1 << (bits-1)) else v


def fbits(value):
    try: return struct.unpack('>I', struct.pack('>f', value))[0]
    except OverflowError: return 0xff800000 if value < 0 else 0x7f800000


def fvalue(bits): return struct.unpack('>f', struct.pack('>I', bits & MASK))[0]
def f32(value): return fvalue(fbits(value))


class Machine:
    def __init__(self, code, fixture):
        self.code, self.fixture, self.memory = code, fixture, fixture.memory
        self.r = [(0xA5000000+i*397) & MASK for i in range(32)]
        self.r[0], self.r[29], self.r[31] = 0, STACK, RETURN
        self.r[4:8] = [fixture.input, fixture.mode & MASK, fbits(fixture.scale), fixture.sound & MASK]
        self.f = [(0x3F000000+i*701) & MASK for i in range(32)]
        self.original_f = self.f[:]
        self.original = self.r[:]
        self.coverage, self.branches = set(), set()
        self.lo, self.condition = 0, False

    def run(self):
        pc, pending = ROOT, None
        for _ in range(15000):
            if pc == RETURN:
                assert self.r[29] == STACK
                assert self.r[16:24] == self.original[16:24]
                assert self.r[30] == self.original[30]
                assert self.f[20:32] == self.original_f[20:32]
                return
            assert pc in self.code, ('out-of-scope pc', hex(pc))
            self.coverage.add(pc)
            self.memory.pc = pc
            word, old = self.code[pc], pending
            pending, next_pc = None, pc + 4
            op, rs, rt, rd = word>>26, (word>>21)&31, (word>>16)&31, (word>>11)&31
            shift, fn, imm = (word>>6)&31, word&63, word&65535
            off = signed(imm,16)
            r, f, m = self.r,self.f,self.memory
            address = (r[rs]+off)&MASK
            branch = None
            if op == 0:
                if fn == 0: r[rd] = (r[rt]<<shift)&MASK
                elif fn == 3: r[rd] = (signed(r[rt])>>shift)&MASK
                elif fn == 4: r[rd] = (r[rt]<<(r[rs]&31))&MASK
                elif fn == 8: pending = r[rs]
                elif fn == 18: r[rd] = self.lo
                elif fn == 25: self.lo = (r[rs]*r[rt])&MASK
                elif fn == 33: r[rd] = (r[rs]+r[rt])&MASK
                elif fn == 35: r[rd] = (r[rs]-r[rt])&MASK
                elif fn == 37: r[rd] = r[rs]|r[rt]
                else: raise AssertionError(('unsupported SPECIAL',hex(pc),fn))
            elif op == 1:
                assert rt in (0,2)
                branch = (signed(r[rs])<0, rt == 2)
            elif op == 3:
                destination = ((pc+4)&0xf0000000)|((word&0x3ffffff)<<2)
                r[31] = pc+8
                pending = destination if destination == PRIVATE else ('call',destination,pc+8)
            elif op in (4,5,20,21):
                branch = (r[rs] == r[rt] if op in (4,20) else r[rs] != r[rt], op >= 20)
            elif op == 9: r[rt] = address
            elif op == 10: r[rt] = int(signed(r[rs])<off)
            elif op == 11: r[rt] = int(r[rs]<(off&MASK))
            elif op == 12: r[rt] = r[rs]&imm
            elif op == 13: r[rt] = r[rs]|imm
            elif op == 15: r[rt] = imm<<16
            elif op == 17:
                if rs == 0: r[rt] = f[rd]
                elif rs == 4: f[rd] = r[rt]
                elif rs == 8:
                    assert rt in (0,1,2,3)
                    branch = (self.condition if rt&1 else not self.condition, bool(rt&2))
                elif rs == 16:
                    a,b = fvalue(f[rd]),fvalue(f[rt])
                    if fn == 0: f[shift] = fbits(a+b)
                    elif fn == 1: f[shift] = fbits(a-b)
                    elif fn == 2: f[shift] = fbits(a*b)
                    elif fn == 3:
                        assert b != 0
                        f[shift] = fbits(a/b)
                    elif fn == 6: f[shift] = f[rd]
                    elif fn == 13:
                        assert math.isfinite(a) and -2147483648 <= a < 2147483648, ('invalid trunc',a)
                        f[shift] = int(a)&MASK
                    elif fn == 62: self.condition = a <= b
                    else: raise AssertionError(('unsupported COP1 function',hex(pc),fn))
                elif rs == 20 and fn == 32: f[shift] = fbits(float(signed(f[rd])))
                else: raise AssertionError(('unsupported COP1 format',hex(pc),rs))
            elif op in (32,33,35,36,37):
                width = {32:1,33:2,35:4,36:1,37:2}[op]
                value = m.get(address,width)
                r[rt] = (signed(value,width*8)&MASK) if op in (32,33) else value
            elif op in (40,41,43): m.put(address,r[rt],{40:1,41:2,43:4}[op])
            elif op == 49: f[rt] = m.get(address)
            elif op == 57: m.put(address,f[rt])
            elif op == 53:
                assert address % 8 == 0
                f[rt],f[rt+1] = m.get(address),m.get(address+4)
            elif op == 61:
                assert address % 8 == 0
                m.put(address,f[rt]);m.put(address+4,f[rt+1])
            else: raise AssertionError(('unsupported opcode',hex(pc),op))
            if branch is not None:
                take,likely = branch
                self.branches.add((pc,take))
                if take: pending = pc+4+off*4
                elif likely: next_pc += 4
            r[0] = 0
            assert old is None or pending is None, ('control transfer in delay slot',hex(pc))
            if isinstance(old,tuple):
                destination = old[1]
                count = {0x80090284:0,0x8008D6B0:2,0x80090088:3,0x8008E26C:4,0x8008EA10:4,0x800AED64:10}[destination]
                args = r[4:4+min(count,4)]
                if count > 4: args += [m.get(r[29]+4*i) for i in range(4,count)]
                result = self.fixture.service(destination,*args)
                for index in list(range(1,16))+[24,25,31]: r[index] = (0xBAD00000+index*197)&MASK
                for index in range(20): f[index] = 0x7FC00000
                r[2] = result & MASK
                self.lo, self.condition = 0xBAD01234, not self.condition
                pc = old[2]
            else: pc = old if old is not None else next_pc
        raise AssertionError('instruction bound exceeded')
