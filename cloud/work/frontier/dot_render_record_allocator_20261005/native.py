"""Fail-closed integer MIPS replay for the native and independently linked allocator."""
import struct

BASE = 0x800A79F4
SIZE = 240
POOL = 0x80140BF0
COUNT = 0x801613AC
HIGH = 0x8013C234
STACK = 0x200100
RETURN = 0x70000000
MASK = 0xffffffff
CAPACITY = 200


def signed(value, bits=32):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def reference(pool, count, high, args):
    """Independent record-level state transition; inputs are bounded below."""
    assert len(pool) == CAPACITY * 32 and -3 <= count <= CAPACITY
    out = bytearray(pool)
    chosen = next((i for i in range(max(count, 0)) if signed(pool[32*i+22], 8) == 2), max(count, 0))
    if chosen >= CAPACITY:
        return -1, bytes(out), count, high
    if chosen >= count:
        count += 1
    high = max(high, count)
    tex, info, image, x, y, width, height = args
    base = chosen * 32
    for offset, value in ((0,image), (4,info)):
        struct.pack_into('>I', out, base+offset, value & MASK)
    for offset, value in ((8,tex), (10,x), (12,y), (14,0), (16,width),
                          (18,height), (24,0), (26,0), (28,height-1), (30,width-1)):
        struct.pack_into('>H', out, base+offset, value & 65535)
    out[base+20] = 255
    out[base+21] = out[base+22] = 0
    return chosen, bytes(out), count, high


class Machine:
    def __init__(self, words, pool, count, high, args):
        assert len(words) == SIZE // 4
        self.words = words
        self.mem = {POOL+i:b for i,b in enumerate(pool)}
        self.mem.update({STACK+i:0x5a for i in range(-32,64)})
        self.mem.update({COUNT+i:0 for i in range(4)})
        self.mem.update({HIGH+i:0 for i in range(4)})
        self.put(COUNT,4,count); self.put(HIGH,4,high)
        for i,a in enumerate(args[4:]): self.put(STACK+16+4*i,4,a)
        self.r = [0xa5000000+i for i in range(32)]
        self.r[0] = 0
        self.r[4:8] = [a & MASK for a in args[:4]]
        self.r[29],self.r[31] = STACK,RETURN
        self.initial = self.r[:]
        self.before = dict(self.mem)
        self.visited = set()
        self.steps = 0

    def get(self,address,width):
        assert address % width == 0 and all(address+i in self.mem for i in range(width)), hex(address)
        return int.from_bytes(bytes(self.mem[address+i] for i in range(width)), 'big')

    def put(self,address,width,value):
        assert address % width == 0 and all(address+i in self.mem for i in range(width)), hex(address)
        for i,b in enumerate((value & ((1 << (8*width))-1)).to_bytes(width,'big')):
            self.mem[address+i] = b

    def wr(self,index,value):
        if index: self.r[index] = value & MASK

    def step(self,pc,delay=False):
        assert BASE <= pc < BASE+SIZE and pc % 4 == 0, hex(pc)
        self.visited.add(pc-BASE); self.steps += 1
        assert self.steps < 2200
        w = self.words[(pc-BASE)//4]
        op,rs,rt,rd,sa,fn = w>>26,w>>21&31,w>>16&31,w>>11&31,w>>6&31,w&63
        imm=w&65535; si=signed(imm,16); a,b=self.r[rs],self.r[rt]
        dest=None
        if op == 0:
            if fn == 0: self.wr(rd,b<<sa)
            elif fn == 8: dest=a
            elif fn == 33: self.wr(rd,a+b)
            elif fn == 37: self.wr(rd,a|b)
            elif fn == 42: self.wr(rd,int(signed(a)<signed(b)))
            else: raise AssertionError(('unknown SPECIAL',fn))
        elif op == 9: self.wr(rt,a+si)
        elif op == 10: self.wr(rt,int(signed(a)<si))
        elif op == 15: self.wr(rt,imm<<16)
        elif op in (4,5,6,20,21):
            yes = {4:a==b,5:a!=b,6:signed(a)<=0,20:a==b,21:a!=b}[op]
            if op in (20,21) and not yes:
                assert not delay
                return pc+8
            dest=pc+4+4*si if yes else pc+8
        elif op in (32,35,40,41,43):
            width={32:1,35:4,40:1,41:2,43:4}[op]
            address=(a+si)&MASK
            if op<40: self.wr(rt,signed(self.get(address,width),width*8))
            else: self.put(address,width,b)
        else: raise AssertionError(('unknown opcode',op))
        if dest is not None:
            assert not delay, 'branch in delay slot'
            assert self.step(pc+4,True)==pc+8
            return dest
        return pc+4

    def run(self):
        pc=BASE
        while pc != RETURN: pc=self.step(pc)
        for i in [*range(16,24),28,29,30,31]: assert self.r[i]==self.initial[i]
        assert all(self.mem[a]==b for a,b in self.before.items() if not (POOL<=a<POOL+6400 or COUNT<=a<COUNT+4 or HIGH<=a<HIGH+4))
        return signed(self.r[2]),bytes(self.mem[POOL+i] for i in range(6400)),signed(self.get(COUNT,4)),signed(self.get(HIGH,4))
