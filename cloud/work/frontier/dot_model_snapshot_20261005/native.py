"""Small fail-closed MIPS-II executor for the complete snapshot and real matrix copy.

No native words are embedded. The verifier supplies manifest-checked target
words or independently GNU-linked words. Floating operations round to binary32.
This models bounded finite arithmetic, not FCSR flags or hardware exceptions.
"""
import math
import struct

BASE = 0x800D4DFC
CALLEE = 0x8008D6B0
LITERAL = 0x801241A4
OBJECT = 0x100000
SIZE = 2056
STACK = 0x200100
RETURN = 0x300000


def f32(value):
    return struct.unpack('>f', struct.pack('>f', value))[0]


def signed(value):
    return value - 0x100000000 if value & 0x80000000 else value


class Machine:
    def __init__(self, words, callee, literal, initial):
        self.code = {BASE + 4*i: w for i, w in enumerate(words)}
        self.code.update({CALLEE + 4*i: w for i, w in enumerate(callee)})
        self.memory = {OBJECT+i: v for i, v in enumerate(initial)}
        self.memory.update({STACK+i: (i*37)&255 for i in range(-64, 64)})
        self.memory.update({LITERAL+i: v for i, v in enumerate(literal)})
        self.original = dict(self.memory)
        self.r = [(0x43210000+i*131)&0xffffffff for i in range(32)]
        self.r[0] = 0; self.r[4] = OBJECT; self.r[29] = STACK; self.r[31] = RETURN
        self.fp = [0x3f800000+i*64 for i in range(32)]
        self.entry_registers = list(self.r); self.entry_fp = list(self.fp)
        self.pc = BASE; self.pending = None; self.visited = set(); self.calls = []
        self.reads = []; self.writes = []

    def read(self, address, count):
        assert address % count == 0
        assert all(address+i in self.memory for i in range(count)), hex(address)
        self.reads.append((self.pc, address, count))
        return int.from_bytes(bytes(self.memory[address+i] for i in range(count)), 'big')

    def write(self, address, count, value):
        assert address % count == 0
        assert all(address+i in self.memory for i in range(count)), hex(address)
        assert (OBJECT <= address and address+count <= OBJECT+SIZE) or STACK-24 <= address < STACK+4
        self.writes.append((self.pc, address, count))
        for i, byte in enumerate((value & ((1 << (count*8))-1)).to_bytes(count, 'big')):
            self.memory[address+i] = byte

    def float(self, reg):
        return struct.unpack('>f', struct.pack('>I', self.fp[reg]))[0]

    def run(self):
        for _ in range(256):
            if self.pc == RETURN:
                break
            assert self.pc in self.code, hex(self.pc)
            if self.pc == CALLEE:
                self.calls.append([self.r[4], self.r[5]])
            self.visited.add(self.pc)
            w = self.code[self.pc]; op=w>>26; rs=(w>>21)&31; rt=(w>>16)&31
            rd=(w>>11)&31; shift=(w>>6)&31; fn=w&63
            imm=w&65535; imm=imm-65536 if imm&32768 else imm
            branch = None
            if w == 0:
                pass
            elif op == 0:
                if fn == 0x21:
                    self.r[rd]=(self.r[rs]+self.r[rt])&0xffffffff
                elif fn == 0x25:
                    self.r[rd]=self.r[rs]|self.r[rt]
                elif fn == 8:
                    branch=self.r[rs]
                else:
                    raise AssertionError(('special', fn, hex(self.pc)))
            elif op == 3:
                branch=((self.pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
                assert branch == CALLEE
                self.r[31]=self.pc+8
            elif op == 9:
                self.r[rt]=(self.r[rs]+imm)&0xffffffff
            elif op == 15:
                self.r[rt]=(w&65535)<<16
            elif op == 35:
                self.r[rt]=self.read((self.r[rs]+imm)&0xffffffff,4)
            elif op in (43, 41):
                self.write((self.r[rs]+imm)&0xffffffff,4 if op==43 else 2,self.r[rt])
            elif op == 49:
                self.fp[rt]=self.read((self.r[rs]+imm)&0xffffffff,4)
            elif op == 57:
                self.write((self.r[rs]+imm)&0xffffffff,4,self.fp[rt])
            elif op == 17:
                if rs == 4:
                    self.fp[rd]=self.r[rt]
                elif rs == 0:
                    self.r[rt]=self.fp[rd]
                elif rs == 16:
                    a,b=self.float(rd),self.float(rt)
                    if fn in (0,2):
                        value=f32(a+b if fn==0 else a*b)
                        assert math.isfinite(value)
                        self.fp[shift]=struct.unpack('>I',struct.pack('>f',value))[0]
                    elif fn == 13:
                        assert math.isfinite(a) and -2147483648 <= math.trunc(a) <= 2147483647
                        self.fp[shift]=math.trunc(a)&0xffffffff
                    else:
                        raise AssertionError(('float', fn, hex(self.pc)))
                else:
                    raise AssertionError(('cop1', rs, hex(self.pc)))
            else:
                raise AssertionError(('opcode',op,hex(self.pc)))
            self.r[0]=0
            old=self.pending
            assert old is None or branch is None, 'branch in delay slot'
            self.pending=branch
            self.pc=old if old is not None else self.pc+4
        else:
            raise AssertionError('step limit')
        assert self.pending is None and self.r[29]==STACK
        assert all(self.r[i]==self.entry_registers[i] for i in range(16,24))
        assert self.r[28]==self.entry_registers[28] and self.r[30]==self.entry_registers[30]
        assert self.fp[20:]==self.entry_fp[20:]
        assert self.calls==[[OBJECT+748,OBJECT+1952]]
        allowed=set(range(OBJECT+1880,OBJECT+1882))|set(range(OBJECT+1884,OBJECT+1988))
        for _, address, count in self.writes:
            if address >= OBJECT and address < OBJECT+SIZE:
                assert set(range(address,address+count)) <= allowed
        assert all(v==self.memory[k] for k,v in self.original.items()
                   if not OBJECT <= k < OBJECT+SIZE and not STACK-24 <= k < STACK+4)
        return bytes(self.memory[OBJECT+i] for i in range(SIZE))
