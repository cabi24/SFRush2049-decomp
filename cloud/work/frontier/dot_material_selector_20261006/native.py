"""Fail-closed integer MIPS model and independent fixed-offset state oracle."""
import random

MASK = 0xffffffff
START = 0x8008b000
TABLE = 0x801161f4
RECORDS = 0x80400000
STACK = 0x80700040
RETURN = 0x807ff000


def signed(x, bits=32):
    x &= (1 << bits) - 1
    return x - (1 << bits) if x >> (bits - 1) else x


def get(mem, address, size):
    assert address % size == 0
    return int.from_bytes(bytes(mem[address+i] for i in range(size)), 'big')


def put(mem, address, size, value):
    assert address % size == 0
    for i in range(size):
        assert address+i in mem, ('unmapped write', hex(address+i))
        mem[address+i] = (value >> (8*(size-1-i))) & 255


def execute(words, mem, args, salt=0):
    r = [((0xa5310000 + 0x137*i) ^ salt) & MASK for i in range(32)]
    r[0], r[29], r[31] = 0, STACK, RETURN
    r[4:7] = args
    saved = r[:]
    code = {START+4*i:w for i,w in enumerate(words)}
    pc, pending = START, None
    visited, reads, writes, branches = set(), [], [], set()
    for _ in range(2048):
        if pc == RETURN:
            assert pending is None
            assert r[16:24] == saved[16:24]
            assert all(r[x] == saved[x] for x in (28,29,30,31))
            return r[2], visited, reads, writes, branches
        assert pc in code, ('unmapped instruction', hex(pc))
        w = code[pc]
        visited.add(pc-START)
        op, rs, rt, rd = w>>26, (w>>21)&31, (w>>16)&31, (w>>11)&31
        sa, fn, imm = (w>>6)&31, w&63, w&65535
        off = signed(imm,16)
        address = (r[rs]+off)&MASK
        delayed, pending, following = pending, None, pc+4
        control = op in (1,4,5,6,7,20,21,22,23) or (op==0 and fn==8)
        assert delayed is None or not control, 'control transfer in delay slot'
        if w == 0: pass
        elif op==0 and fn in (0,3):
            assert rs == 0, 'reserved shift source'
            r[rd] = ((r[rt]<<sa) if fn==0 else signed(r[rt])>>sa)&MASK
        elif op==0 and fn in (33,35,37,42):
            assert sa == 0, 'reserved arithmetic shift'
            if fn==33: value=r[rs]+r[rt]
            elif fn==35: value=r[rs]-r[rt]
            elif fn==37: value=r[rs]|r[rt]
            else: value=int(signed(r[rs])<signed(r[rt]))
            r[rd]=value&MASK
        elif op==0 and fn==8:
            assert (w&0x1fffc0)==0
            pending=r[rs]
        elif op==9: r[rt]=address
        elif op==12: r[rt]=r[rs]&imm
        elif op==13: r[rt]=r[rs]|imm
        elif op==15:
            assert rs==0
            r[rt]=imm<<16
        elif op in (33,35,37):
            size=4 if op==35 else 2
            value=get(mem,address,size)
            r[rt]=(signed(value,16)&MASK) if op==33 else value
            reads.append((address,size))
        elif op in (41,43):
            size=2 if op==41 else 4
            put(mem,address,size,r[rt])
            writes.append((address,size,r[rt]&((1<<(size*8))-1)))
        elif op in (1,4,5,6,7,20,21,22,23):
            if op==1:
                assert rt in (0,1), 'unsupported regimm'
                take=(signed(r[rs])<0) if rt==0 else (signed(r[rs])>=0)
            elif op in (4,20): take=r[rs]==r[rt]
            elif op in (5,21): take=r[rs]!=r[rt]
            elif op in (6,22):
                assert rt==0
                take=signed(r[rs])<=0
            else:
                assert rt==0
                take=signed(r[rs])>0
            branches.add((pc-START,take))
            if take: pending=pc+4+4*off
            elif op in (20,21,22,23): following=pc+8
        else: raise AssertionError(('unsupported instruction',hex(pc),hex(w)))
        r[0]=0
        pc=delayed if delayed is not None else following
    raise AssertionError('step limit')


def fixture(handle, selector, value, count, salt):
    rng=random.Random(salt)
    address=RECORDS+(handle&1023)*88
    ranges=[(TABLE-16,TABLE+512+16),(address-16,address+88+16),(STACK-32,STACK+64)]
    mem={a:rng.randrange(256) for lo,hi in ranges for a in range(lo,hi)}
    put(mem,TABLE+(handle>>10)*8,4,RECORDS)
    put(mem,address+22,2,count)
    args=[((salt*0x137)&0xffff0000)|handle,
          ((salt*0x73)&0xffff0000)|(selector&65535),
          ((salt*0x197)&0xffff0000)|value]
    return mem,args,address


def oracle(mem,args):
    handle=args[0]&65535
    selector=signed(args[1],16)
    value=args[2]&65535
    address=get(mem,TABLE+(handle>>10)*8,4)+(handle&1023)*88
    count=signed(get(mem,address+22,2),16)
    # Native writes all incoming register bits to the three O32 home slots.
    for offset,arg in zip((0,4,8),args): put(mem,STACK+offset,4,arg)
    if selector>=0:
        if selector>=count: return 0
        put(mem,address+24+16*selector,2,value)
        flags=address+26+16*selector
        put(mem,flags,2,get(mem,flags,2)|0x8000)
    else:
        for i in range(max(count,0)):
            put(mem,address+24+16*i,2,value)
            flags=address+26+16*selector
            put(mem,flags,2,get(mem,flags,2)|0x8000)
    return 1
