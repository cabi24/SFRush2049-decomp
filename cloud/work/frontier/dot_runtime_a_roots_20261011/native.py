"""Fail-closed integer MIPS-II root runner with explicit call-boundary fixtures.

FP registers are preserved as opaque 64-bit pairs; this root performs no FP
arithmetic. The renderer and initializer are not executed by this runner.
"""
import random

SP = 0x81010000
RETURN = 0x81234560
ENTRY = 0x80394058
PRIVATE = {0x80393B3C, 0x803930B8, 0x80393004, 0x803931C0,
           0x803932E4, 0x8039338C, 0x803923B0, 0x80392F28}
EXTERNAL = {0x800B5570, 0x800B5F88, 0x800B5F4C, 0x800B5FC4,
            0x800A3424, 0x800C7308}


def require(ok, detail):
    if not ok:
        raise ValueError(detail)


def signed(value, bits=32):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def read(memory, address, size):
    require(address % size == 0 and all(address+i in memory for i in range(size)),
            ('unmapped/unaligned read', hex(address), size))
    return int.from_bytes(bytes(memory[address+i] for i in range(size)), 'big')


def write(memory, address, size, value):
    require(address % size == 0 and all(address+i in memory for i in range(size)),
            ('unmapped/unaligned write', hex(address), size))
    for i, byte in enumerate((value & ((1 << (8*size))-1)).to_bytes(size, 'big')):
        memory[address+i] = byte


def run(words, initial, seed):
    rng = random.Random(seed)
    regs = [rng.getrandbits(32) for _ in range(32)]
    fp = [rng.getrandbits(64) for _ in range(16)]
    regs[0], regs[29], regs[31] = 0, SP, RETURN
    before, before_fp = list(regs), list(fp)
    memory = dict(initial)
    pc, pending = ENTRY, None
    coverage, branches, calls = set(), set(), []
    for step in range(10000):
        if pc == RETURN:
            require(pending is None, 'branch pending at return')
            require(regs[29] == SP, 'stack not restored')
            require(all(regs[i] == before[i] for i in range(16,24)) and regs[30] == before[30], 'integer ABI')
            require(fp[10:] == before_fp[10:], 'floating ABI')
            return dict(memory=memory, calls=calls, coverage=coverage, branches=branches,
                        callee_saved=regs[16:24]+[regs[30]], fp_saved=fp[10:], steps=step)
        if pc in PRIVATE | EXTERNAL:
            require(pending is None, 'helper in delay slot')
            args = tuple(regs[4:8])
            argc = 2 if pc == 0x800A3424 else (1 if pc in EXTERNAL | {0x803931C0} else 0)
            calls.append((pc,args[:argc]))
            # Fixtures model only documented selected effects. They deliberately
            # do not assert complete child behavior, and preserve valid domains.
            if pc == 0x80393B3C:
                write(memory,0x803B6528,1,1)
                write(memory,0x803BA804,4,0)
            elif pc == 0x803931C0:
                write(memory,0x803BA804,4,args[0])
            elif pc == 0x80392F28:
                level = signed(read(memory,0x803BA858,1),8)
                category = 0 if level < 12 else 1 if level < 20 else 2 if level < 24 else 3 if level < 25 else 4 if level < 29 else 5
                write(memory,0x803BA880,4,category)
                if category == 4: write(memory,0x803BA860,1,level-25)
                for address in range(0x803BA7FC,0x803BA800):write(memory,address,1,1)
            elif pc == 0x800A3424 and seed % 3 == 0:
                # Native reloads the handle after this external callback.
                write(memory,0x803BA800,4,0x80700100)
            resume = regs[31]
            # Leaf selector has its actual narrow scratch contract. All other
            # fixtures may clobber standard O32 caller-saved registers.
            clobbers = (1,2,3,4,5,6,7,14,15,24,25) if pc == 0x80392F28 else (*range(1,16),24,25)
            for i in clobbers:regs[i]=rng.getrandbits(32)
            if pc in PRIVATE and pc != 0x80392F28:
                for i in (*range(16,24),30):regs[i]=rng.getrandbits(32)
            if pc in (0x80393B3C,0x803923B0):
                for i in range(10,16):fp[i]=rng.getrandbits(64)
            pc=resume
            continue
        require(ENTRY <= pc < ENTRY+4*len(words) and pc%4 == 0, ('escaped code',hex(pc)))
        coverage.add(pc-ENTRY)
        w=words[(pc-ENTRY)//4];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;fn=w&63
        imm=signed(w,16);delayed,pending,transfer,annul=pending,None,None,False
        if w == 0:pass
        elif op == 0:
            if fn==0:regs[rd]=regs[rt]<<(w>>6&31)
            elif fn==3:regs[rd]=signed(regs[rt])>>(w>>6&31)
            elif fn==33:regs[rd]=regs[rs]+regs[rt]
            elif fn==35:regs[rd]=regs[rs]-regs[rt]
            elif fn==36:regs[rd]=regs[rs]&regs[rt]
            elif fn==37:regs[rd]=regs[rs]|regs[rt]
            elif fn==42:regs[rd]=int(signed(regs[rs])<signed(regs[rt]))
            elif fn==43:regs[rd]=int(regs[rs]<regs[rt])
            elif fn==8:
                require(rs==31 and w&0x1FFFFF==8,'unexpected jump');transfer=regs[rs]
            else:raise ValueError(('unknown special',hex(pc),fn))
        elif op==15:require(rs==0,'lui rs');regs[rt]=(w&65535)<<16
        elif op==9:regs[rt]=regs[rs]+imm
        elif op==10:regs[rt]=int(signed(regs[rs])<imm)
        elif op==11:regs[rt]=int(regs[rs]<(imm&0xFFFFFFFF))
        elif op==12:regs[rt]=regs[rs]&(w&65535)
        elif op==13:regs[rt]=regs[rs]|(w&65535)
        elif op in (32,33,35,36):
            n={32:1,33:2,35:4,36:1}[op];v=read(memory,(regs[rs]+imm)&0xFFFFFFFF,n)
            regs[rt]=signed(v,n*8) if op in (32,33) else v
        elif op in (40,43):write(memory,(regs[rs]+imm)&0xFFFFFFFF,1 if op==40 else 4,regs[rt])
        elif op in (53,61):
            require(rt%2==0,'odd FP pair');address=(regs[rs]+imm)&0xFFFFFFFF
            if op==53:fp[rt//2]=read(memory,address,8)
            else:write(memory,address,8,fp[rt//2])
        elif op in (1,4,5,6,7,20,21):
            if op==1:
                require(rt in (0,1),'unexpected regimm');taken=signed(regs[rs])<0 if rt==0 else signed(regs[rs])>=0
            elif op==6:require(rt==0,'blez rt');taken=signed(regs[rs])<=0
            elif op==7:require(rt==0,'bgtz rt');taken=signed(regs[rs])>0
            else:taken=(regs[rs]==regs[rt]) if op in (4,20) else (regs[rs]!=regs[rt])
            branches.add((pc-ENTRY,taken));annul=op in (20,21) and not taken
            transfer=pc+4+imm*4 if taken else pc+8
        elif op==3:
            transfer=((pc+4)&0xF0000000)|((w&0x3FFFFFF)<<2)
            require(transfer in PRIVATE|EXTERNAL,('unknown call',hex(transfer)));regs[31]=pc+8
        else:raise ValueError(('unknown opcode',hex(pc),op))
        regs=[v&0xFFFFFFFF for v in regs];regs[0]=0
        if delayed is not None:
            require(transfer is None,'nested delay transfer');pc=delayed
        elif annul:pc+=8
        else:pc,pending=pc+4,transfer
    raise ValueError('execution limit')
