"""Fail-closed integer MIPS harness; native instruction words are never embedded."""
import random
ENTRY = 0x8039D494
CALLER = 0x8039E3BC
RETURN = 0x81234560
SP = 0x81010000
FLAGS = (0x801407D0, 0x80156994, 0x8014978C, 0x803AF980, 0x8014A110, 0x801164BE)
SELECTORS = 0x803B9FD0
ATTRIBUTES = 0x803B3020
COUNTS = 0x803BA050
OUTPUT = 0x803BA060

def signed(x, bits=32):
    x &= (1 << bits) - 1
    return x - (1 << bits) if x & (1 << (bits-1)) else x

def read(m, a, n):
    assert a % n == 0 and all(a+i in m for i in range(n)), ('unmapped read', hex(a), n)
    return int.from_bytes(bytes(m[a+i] for i in range(n)), 'big')

def write(m, a, n, v):
    assert a % n == 0 and all(a+i in m for i in range(n)), ('unmapped write', hex(a), n)
    for i, b in enumerate((v & ((1 << (8*n))-1)).to_bytes(n, 'big')):
        m[a+i] = b

def state(c):
    item, player, unlock, network, track, active, mode, setting, selector, bits = c
    m = {a: 0xA5 for lo, hi in ((SELECTORS-8,SELECTORS+12), (ATTRIBUTES-136,ATTRIBUTES+136),
          (COUNTS-8,COUNTS+16), (OUTPUT,OUTPUT+4*19*4+8), (SP-32,SP+16)) for a in range(lo,hi)}
    for a, n, v in zip(FLAGS,(1,1,1,4,4,2),(unlock,network,track,active,mode,setting)):
        m.update({a+i:0 for i in range(n)});write(m,a,n,v)
    for p in range(4):m[SELECTORS+p] = (selector+37*(p-player)) % 128
    m[SELECTORS+player] = selector & 255
    m[ATTRIBUTES+selector] = bits & 255
    return m

def oracle(c):
    item, player, unlock, network, track, active, mode, setting, selector, bits = c
    denied = set((12,15))
    if not unlock:denied.update((13,14))
    if not network:
        denied.update((17,18))
        if 0 <= track < 6:denied.add(11)
    if not active or mode != 2:denied.update((16,17,18))
    if mode in (2,6):denied.add(6)
    if mode == 6 and not setting:denied.update((7,8,9))
    for opt,mask in ((7,1),(8,2),(9,4)):
        if not (bits & mask):denied.add(opt)
    if mode != 6:denied.add(10)
    return int(item not in denied)

def run(functions, initial, item=0, player=0, caller=False, salt=0):
    rng=random.Random(salt ^ 0x912391);r=[rng.getrandbits(32) for _ in range(32)]
    r[0]=0;r[4]=item & 0xFFFFFFFF;r[5]=player & 0xFFFFFFFF;r[12]=player;r[29]=SP;r[31]=RETURN
    before=list(r);m=dict(initial);pc=CALLER if caller else ENTRY;pending=None;coverage=set();branches=set();writes=[];reads=[]
    code={base+4*i:w for base,words in functions.items() for i,w in enumerate(words)}
    for step in range(4096):
        if pc==RETURN:
            assert pending is None
            preserve=(*range(17 if caller else 16,24),28,29,30,31)
            assert all(r[i]==before[i] for i in preserve),'O32 saved register corruption'
            return {'return':signed(r[2]),'registers':r,'initial_registers':before,'memory':m,'coverage':coverage,'branches':branches,'writes':writes,'reads':reads}
        assert pc in code and pc % 4 == 0, ('escaped code',hex(pc))
        coverage.add(pc);w=code[pc];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;fn=w&63;imm=signed(w,16)
        delayed=pending;pending=None;transfer=None;annul=False
        if w==0:pass
        elif op==0:
            if fn==0:r[rd]=(r[rt]<<((w>>6)&31)) & 0xFFFFFFFF
            elif fn==33:r[rd]=(r[rs]+r[rt]) & 0xFFFFFFFF
            elif fn==35:r[rd]=(r[rs]-r[rt]) & 0xFFFFFFFF
            elif fn==37:r[rd]=r[rs]|r[rt]
            elif fn==8:assert rs==31 and (w&0x1FFFFF)==8;transfer=r[rs]
            else:raise AssertionError(('unknown special',hex(pc),fn))
        elif op==15:assert rs==0;r[rt]=(w&65535)<<16
        elif op==9:r[rt]=(r[rs]+imm)&0xFFFFFFFF
        elif op==10:r[rt]=int(signed(r[rs])<imm)
        elif op==12:r[rt]=r[rs]&(w&65535)
        elif op in (32,33,35):
            n={32:1,33:2,35:4}[op];a=(r[rs]+imm)&0xFFFFFFFF;v=read(m,a,n);reads.append((a,n))
            r[rt]=(signed(v,n*8) if n<4 else v)&0xFFFFFFFF
        elif op==43:
            a=(r[rs]+imm)&0xFFFFFFFF;write(m,a,4,r[rt]);writes.append((a,r[rt]))
        elif op in (1,4,5,20,21):
            if op==1:assert rt==0;taken=signed(r[rs])<0
            else:taken=(r[rs]==r[rt]) if op in (4,20) else r[rs]!=r[rt]
            branches.add((pc,taken));annul=op in (20,21) and not taken
            transfer=pc+4+imm*4 if taken else pc+8
        elif op==3:
            assert caller;transfer=((pc+4)&0xF0000000)|((w&0x3FFFFFF)<<2);assert transfer==ENTRY;r[31]=pc+8
        else:raise AssertionError(('unknown opcode',hex(pc),op))
        r[0]=0
        if delayed is not None:assert transfer is None;pc=delayed
        elif annul:pc+=8
        else:pc,pending=pc+4,transfer
    raise AssertionError('execution limit')

def cases():
    # Exhaust the boolean gate lattice, item IDs, and eight low-bit attribute states.
    for item in range(19):
        for mask in range(32):
            for bits in range(8):
                yield (item,(item+mask)%4,(mask&1),((mask>>1)&1),3 if mask&4 else -1,
                       (mask>>3)&1,2 if mask&16 else 6,mask&4,(item+bits)%128,bits)
    # Directed boundaries and all signed-byte values, with deterministic combinations.
    for track in (6,127):yield (11,0,1,0,track,1,2,1,0,7)
    rng=random.Random(0xD494)
    for i in range(4096):
        yield (rng.choice((-2147483648,-1,*range(21),2147483647)),i%4,
               signed(i,8),signed(i*37,8),signed(i*73,8),rng.choice((0,1,-1,2147483647,-2147483648)),
               rng.choice((-2147483648,-1,0,1,2,3,5,6,7,2147483647)),
               rng.choice((-32768,-1,0,1,32767)),i%128,signed(i*17,8))
