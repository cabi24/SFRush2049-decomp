"""Bounded MIPS-II interpreter for the speed command writer; no instruction data."""
import struct

MASK = 0xFFFFFFFF
START = 0x800C9210
STACK = 0x70000100
STOP = 0xFFFFFFFC
RECORD = 0x20000010
LOCK, QUEUE = 0x80142728, 0x801427A8
RECV, ALLOC, JAM = 0x80007270, 0x80091B00, 0x800075E0

def number(bits):
    return struct.unpack('>f', struct.pack('>I', bits))[0]

def initial_record():
    return bytearray((i * 37 + 11) & 255 for i in range(24))

def mutate(record, mode):
    if mode == 1:
        for i in range(24): record[i] ^= 0xA5
    elif mode == 2:
        for i in [2, 4, 7, 8, 11, 12, 13]: record[i] ^= 0x5A

def oracle(case):
    blend, amount, first, second, mode = case
    record = initial_record()
    record[2] = 10
    record[4:8] = struct.pack('>I', 0 if number(blend) < 0 else 0x3F800000 if number(blend) > 1 else blend)
    record[8:12] = struct.pack('>I', 0 if number(amount) < 0 else amount)
    record[12], record[13] = first & 255, second & 255
    release = list(record)
    mutate(record, mode)
    return release + list(record)

def execute(words, case):
    assert len(words) == 51, 'complete 204-byte body required'
    blend, amount, first, second, mode = case
    mem = {RECORD + i: b for i, b in enumerate(initial_record())}
    for i in range(-40, 32): mem[STACK+i] = (i * 13 + 7) & 255
    before = dict(mem)
    regs = [(0xA5000000 + i * 0x10001) & MASK for i in range(32)]
    regs[0], regs[17], regs[18], regs[29], regs[31] = 0, first, second, STACK, STOP
    fp = [0x3F000000 + i * 7 for i in range(32)]
    fp[20], fp[22] = blend, amount
    initial_regs, initial_fp = regs[:], fp[:]
    condition, pc, pending, steps = False, START, None, 0
    visited, branches, stores, events, snapshots = set(), set(), [], [], []
    def read(addr, n):
        assert all(addr+i in mem for i in range(n)), ('unmapped read', hex(addr), n)
        return int.from_bytes(bytes(mem[addr+i] for i in range(n)), 'big')
    def write(addr, value, n):
        assert all(addr+i in mem for i in range(n)), ('unmapped write', hex(addr), n)
        assert (RECORD <= addr and addr+n <= RECORD+24) or STACK-24 <= addr < STACK, ('unexpected write', hex(addr))
        stores.append((addr,n))
        for i,b in enumerate((value & ((1<<(n*8))-1)).to_bytes(n,'big')): mem[addr+i]=b
    while pc != STOP:
        assert steps < 150
        if pc in (RECV, ALLOC, JAM):
            args = tuple(regs[4:7]); events.append((pc,args))
            if pc == RECV:
                assert len(events)==1 and args==(LOCK,0,1)
            elif pc == ALLOC:
                assert len(events)==2
            elif len(events)==3:
                assert args==(LOCK,0,0)
                snapshots += [mem[RECORD+i] for i in range(24)]
                r=bytearray(mem[RECORD+i] for i in range(24));mutate(r,mode)
                for i,b in enumerate(r): mem[RECORD+i]=b
            else:
                assert len(events)==4 and args==(QUEUE,RECORD,0)
                snapshots += [mem[RECORD+i] for i in range(24)]
            for r in list(range(2,16))+[24,25]: regs[r]=(0xCCCC0000+r*17+len(events))&MASK
            for f in range(20): fp[f]=0x3F800000+f+len(events)
            for off in range(0,16,4): write(regs[29]+off, 0xFEED0000+off+len(events),4)
            if pc == ALLOC: regs[2]=RECORD
            pc=regs[31]
            continue
        assert START <= pc < START+204 and pc%4==0, ('outside body',hex(pc))
        visited.add(pc-START)
        w=words[(pc-START)//4];op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31
        imm=w&65535;off=imm-65536 if imm&32768 else imm;addr=(regs[rs]+off)&MASK
        old_pending,pending,following=pending,None,pc+4
        if w==0: pass
        elif op==0 and w&63==37: regs[rd]=regs[rs]|regs[rt]
        elif op==0 and w&63==8: pending=regs[rs]
        elif op==9: regs[rt]=addr
        elif op==15: regs[rt]=imm<<16
        elif op==35: assert addr%4==0;regs[rt]=read(addr,4)
        elif op==43: assert addr%4==0;write(addr,regs[rt],4)
        elif op==40: write(addr,regs[rt],1)
        elif op==57: assert addr%4==0;write(addr,fp[rt],4)
        elif op==4:
            take=regs[rs]==regs[rt];branches.add((pc-START,take))
            if take: pending=pc+4+off*4
        elif op==3:
            regs[31]=pc+8;pending=((pc+4)&0xF0000000)|((w&0x3FFFFFF)<<2)
            assert pending in (RECV,ALLOC,JAM), 'unexpected call'
        elif op==17:
            if rs==4: fp[rd]=regs[rt]
            elif rs==8:
                assert rt in (0,1,2,3)
                take=condition==bool(rt&1);branches.add((pc-START,take))
                if take: pending=pc+4+off*4
                elif rt&2: following+=4
            elif rs==16 and w&63==6: fp[(w>>6)&31]=fp[rd]
            elif rs==16 and w&63==60: condition=number(fp[rd])<number(fp[rt])
            else: raise AssertionError(('unsupported FP',pc-START))
        else: raise AssertionError(('unsupported instruction',pc-START,op))
        regs[0]=0;pc=old_pending if old_pending is not None else following;steps+=1
    assert len(events)==4 and len(snapshots)==48
    assert regs[29]==STACK and regs[31]==STOP
    assert all(regs[i]==initial_regs[i] for i in [17,18,20,21,22,23,28,30])
    assert fp[20:]==initial_fp[20:]
    # s0 and s3 are documented IPA clobbers, not callee-save preservation claims.
    assert regs[16]==RECORD and regs[19]==LOCK
    for addr in mem:
        if not RECORD <= addr < RECORD+24 and not STACK-24 <= addr < STACK-8 and not STACK-4 <= addr < STACK:
            assert mem[addr]==before[addr], ('canary changed',hex(addr))
    assert read(STACK-4,4)==STOP
    actual_writes={addr for addr,n in stores for addr in range(addr,addr+n)}
    expected={RECORD+2}|set(range(RECORD+4,RECORD+14))|set(range(STACK-24,STACK-8))|set(range(STACK-4,STACK))
    assert actual_writes==expected
    return snapshots,visited,branches
