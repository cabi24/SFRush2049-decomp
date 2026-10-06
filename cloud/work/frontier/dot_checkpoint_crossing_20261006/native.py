"""Fail-closed bounded MIPS execution, with explicit O32 helper boundaries.

Instructions are supplied from the protected target or a complete linked object.
No native instruction words or retail data are embedded in this file.
"""
import struct

BASE = 0x800D24C8
MODEL = 0x20000000
STACK = 0x70001000
STOP = 0x7FFF0000
MASK = 0xFFFFFFFF
TRACK = 0x80151CE8
PLAYER = 0x80153E88
CARS = 0x80152818
MODE = 0x8014A110
DEBUG = 0x801174B4
LAPS = 0x80152734
REMAIN = 0x80152014
LOOKAHEAD = 0x80152015
FINISHED = 0x80110680
TIMES = 0x80110668
CLOCK = 0x8002EB90
SOUND = 0x8010FFC0
CALLS = {0x800D2458: 'split', 0x800D2054: 'lap',
         0x800D1CE0: 'finish', 0x80092360: 'sound', 0x800D197C: 'event'}


def signed(value, bits=32):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value >> (bits - 1) else value


def get(mem, address, size):
    assert address % size == 0, ('unaligned read', hex(address), size)
    assert all(address + i in mem for i in range(size)), ('unmapped read', hex(address), size)
    return int.from_bytes(bytes(mem[address + i] for i in range(size)), 'big')


def put(mem, address, size, value):
    assert address % size == 0, ('unaligned write', hex(address), size)
    assert all(address + i in mem for i in range(size)), ('unmapped write', hex(address), size)
    for i, b in enumerate((value & ((1 << (size * 8)) - 1)).to_bytes(size, 'big')):
        mem[address + i] = b


def fixture(case):
    mem = {}
    regions = [(MODEL - 16, 0x828), (TRACK, 12), (PLAYER, 32),
               (CARS, 952 * 4), (MODE, 4), (DEBUG, 4), (LAPS, 2),
               (REMAIN, 2), (FINISHED, 4), (TIMES, 16), (CLOCK, 4),
               (SOUND, 1), (STACK - 512, 768)]
    for start, size in regions:
        for i in range(size):
            assert start + i not in mem
            mem[start + i] = (i * 13 + 91) & 255
    # player, next, enabled, laps, total, kind, mode, debug, remaining,
    # place, locked, sound_enabled, finish, before, count, loop, hook_mode
    player, nxt, enabled, lap, total, kind, mode, debug, remaining, place, locked, sound, finish, before, count, loop, _ = case
    for off, width, value in [(0x7C6,2,player),(0x7E4,2,nxt),(0x7DC,1,enabled),
                              (0x7E8,1,lap),(0x7D4,4,0x1234567F)]:
        put(mem, MODEL + off, width, value)
    for off, value in [(2,loop),(4,finish),(6,before),(8,count)]: put(mem,TRACK+off,2,value)
    put(mem, PLAYER + 8 * player + 7, 1, kind)
    put(mem, CARS + 952 * player + 0xEE, 1, place)
    put(mem, CARS + 952 * player + 0xEF, 1, locked)
    for address, width, value in [(MODE,4,mode),(DEBUG,4,debug),(LAPS,2,total),
                                  (REMAIN,1,remaining),(SOUND,1,sound),
                                  (CLOCK,4,0x42F70000)]: put(mem,address,width,value)
    return mem


def hook(mem, name, args, mode, player):
    """Synthetic bounded mutations; real helper implementations do not run."""
    if mode == 1 and name == 'split':
        put(mem, TRACK + 4, 2, signed(get(mem, TRACK + 4, 2),16) + 1)
    elif mode == 2 and name == 'lap':
        put(mem, MODEL + 0x7E8, 1, get(mem, MODEL + 0x7E8, 1) + 1)
    elif mode == 3 and name == 'finish':
        put(mem, CARS + 952 * player + 0xEF, 1, 1)
        put(mem, PLAYER + 8 * player + 7, 1, 0)
    elif mode == 4 and name == 'sound':
        put(mem, CARS + 952 * player + 0x311, 1, 0x42)
        put(mem, MODEL + 0x7D4, 4, 0xABCD003F)
    return 0xD24C0000 | (len(name) << 4)


def observable(mem):
    return {a: b for a, b in mem.items() if not STACK - 512 <= a < STACK + 256}


def execute(words, initial, case, salt=0):
    mem = dict(initial)
    r = [(0xA5370000 + i * 0x1020307 + salt) & MASK for i in range(32)]
    r[0], r[4], r[5], r[29], r[31] = 0, MODEL, 0x42C80000, STACK, STOP
    original = r[:]
    f = [(0x5A000000 + i * 0x1001 + salt) & MASK for i in range(32)]
    original_f = f[:]
    pc, pending, lo, hi = BASE, None, 0, 0
    trace, coverage, branches = [], set(), set()
    for _ in range(5000):
        if pc == STOP:
            assert pending is None
            assert all(r[i] == original[i] for i in list(range(16,24))+[28,29,30,31])
            assert f[20:] == original_f[20:]
            assert all(mem[a] == initial[a] for a in range(STACK-512,STACK-192)), 'lower stack canary'
            assert all(mem[a] == initial[a] for a in range(STACK+8,STACK+256)), 'upper stack canary'
            return observable(mem), trace, coverage, branches
        if pc in CALLS:
            assert pending is None
            name = CALLS[pc]
            args = r[4:6] if name in ('split','lap','finish') else r[4:8]
            if name == 'event': args += [get(mem,r[29]+off,4) for off in (16,20,24)]
            trace.append((name, args[:], observable(mem)))
            result = hook(mem,name,args,case[-1],case[0])
            ret = r[31]
            for i in list(range(2,16))+[24,25]: r[i]=(0xC12E0000+i*0x01010101+salt)&MASK
            for i in range(20): f[i]=(0x7FC00000+i+salt)&MASK
            r[2]=result
            pc=ret
            continue
        offset=pc-BASE
        assert offset%4==0 and 0<=offset<4*len(words), ('outside function',hex(pc))
        word=words[offset//4];coverage.add(offset)
        op,rs,rt,rd=word>>26,(word>>21)&31,(word>>16)&31,(word>>11)&31
        imm,shift,fn=signed(word,16),(word>>6)&31,word&63
        addr=(r[rs]+imm)&MASK
        prior=pending;pending=None;nextpc=pc+4
        control=op in (2,3,4,5,6,7,20,21,22,23) or (op==0 and fn in (8,9))
        assert prior is None or not control, 'control in delay slot'
        if op==0:
            if fn==0:r[rd]=r[rt]<<shift
            elif fn==2:r[rd]=r[rt]>>shift
            elif fn==3:r[rd]=signed(r[rt])>>shift
            elif fn==8:pending=r[rs]
            elif fn==16:r[rd]=hi
            elif fn==18:r[rd]=lo
            elif fn==0x21:r[rd]=r[rs]+r[rt]
            elif fn==0x23:r[rd]=r[rs]-r[rt]
            elif fn==0x24:r[rd]=r[rs]&r[rt]
            elif fn==0x25:r[rd]=r[rs]|r[rt]
            elif fn==0x26:r[rd]=r[rs]^r[rt]
            elif fn==0x2A:r[rd]=int(signed(r[rs])<signed(r[rt]))
            elif fn==0x2B:r[rd]=int(r[rs]<r[rt])
            else:raise AssertionError(('special opcode',fn,hex(pc)))
        elif op in (2,3):
            if op==3:r[31]=pc+8
            pending=((pc+4)&0xF0000000)|((word&0x3FFFFFF)<<2)
        elif op in (4,5,6,7,20,21,22,23):
            kind=op&15
            take=(r[rs]==r[rt] if kind==4 else r[rs]!=r[rt] if kind==5 else signed(r[rs])<=0 if kind==6 else signed(r[rs])>0)
            branches.add((offset,bool(take)))
            if op>=20 and not take:nextpc=pc+8
            else:pending=pc+4+4*imm if take else pc+8
        elif op==9:r[rt]=r[rs]+imm
        elif op==10:r[rt]=int(signed(r[rs])<imm)
        elif op==11:r[rt]=int(r[rs]<(imm&MASK))
        elif op==12:r[rt]=r[rs]&(word&65535)
        elif op==13:r[rt]=r[rs]|(word&65535)
        elif op==14:r[rt]=r[rs]^(word&65535)
        elif op==15:r[rt]=(word&65535)<<16
        elif op in (32,33,35,36,37):
            width=1 if op in (32,36) else 2 if op in (33,37) else 4
            value=get(mem,addr,width)
            r[rt]=signed(value,width*8) if op in (32,33) else value
        elif op in (40,41,43):put(mem,addr,{40:1,41:2,43:4}[op],r[rt])
        elif op==49:f[rt]=get(mem,addr,4)
        elif op==57:put(mem,addr,4,f[rt])
        elif op==17 and rs==4:f[rd]=r[rt]
        elif op==17 and rs==0:r[rt]=f[rd]
        else:raise AssertionError(('unsupported opcode',op,hex(pc)))
        for i in range(1,32):r[i]&=MASK
        r[0]=0
        pc=prior if prior is not None else nextpc
    raise AssertionError('execution budget exhausted')
