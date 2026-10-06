"""Bounded fail-closed MIPS-I/II integer replay for the 1F954 body only."""
import struct
if not __debug__:
    raise SystemExit("Native proof requires Python assertions.")
BASE=0x8001F954
VOICE=0x8004BEB8
STRIDE=416
SP=0x807FF000
STOP=0x80700000
MASK=0xffffffff

def signed(x):return x-0x100000000 if x&0x80000000 else x

def initial(seed):return bytearray(((i*13+seed*17)&255) for i in range(STRIDE*32))

def hook_image(mem,slot,seed,stage):
    """Independent callback model; mutations preserve mapped valid records."""
    p=slot*STRIDE
    if stage==0:mem[p+36:p+40]=struct.pack('>I',0xa5000000|seed)
    elif stage==1:mem[((slot+1)%32)*STRIDE+189]=(seed*7)&255
    else:
        mem[p+96:p+100]=struct.pack('>I',0xbeef0000|seed)
        mem[p+189]=255

def expected(slot,active,seed):
    m=initial(seed);events=[]
    if slot!=MASK:
        events.append((0x8001467C,slot));hook_image(m,slot,seed,0)
        if active:events.append((0x80014AF0,slot));hook_image(m,slot,seed,1)
        p=slot*STRIDE;m[p+96:p+100]=struct.pack('>I',slot)
        events.append((0x8001F6EC,VOICE+p));hook_image(m,slot,seed,2)
        m[p+189]=0
    return m,events

def run(words,slot,active,seed):
    m=initial(seed);stack=bytearray([0xa5]*512);r=[(0x11220000+i*0x101)&MASK for i in range(32)];r[0]=0;r[4]=slot;r[29]=SP;r[31]=STOP
    original=list(r);events=[];visited=set();pc=BASE;pending=None;steps=0;stores=[]
    def read(addr,n):
        if VOICE<=addr and addr+n<=VOICE+len(m):return bytes(m[addr-VOICE:addr-VOICE+n])
        if SP-256<=addr and addr+n<=SP+256:return bytes(stack[addr-SP+256:addr-SP+256+n])
        raise AssertionError(('unmapped read',hex(addr),n))
    def write(addr,b):
        n=len(b);stores.append((addr,n))
        if VOICE<=addr and addr+n<=VOICE+len(m):m[addr-VOICE:addr-VOICE+n]=b;return
        if SP-256<=addr and addr+n<=SP+256:stack[addr-SP+256:addr-SP+256+n]=b;return
        raise AssertionError(('unmapped write',hex(addr),n))
    while pc!=STOP:
        steps+=1;assert steps<200
        if pc in (0x8001467C,0x80014AF0,0x8001F6EC):
            assert pending is None
            arg=r[4];events.append((pc,arg));stage={0x8001467C:0,0x80014AF0:1,0x8001F6EC:2}[pc]
            assert slot!=MASK and arg==(VOICE+slot*STRIDE if stage==2 else slot)
            if stage==2:assert int.from_bytes(m[slot*STRIDE+96:slot*STRIDE+100],'big')==slot
            hook_image(m,slot,seed,stage)
            ret=r[31]
            for i in [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25]:r[i]=(0xde000000+i*257+seed)&MASK
            if stage==0:r[2]=active
            pc=ret;continue
        assert BASE<=pc<BASE+len(words)*4 and pc%4==0
        visited.add(pc-BASE);w=words[(pc-BASE)//4];op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31;sa=(w>>6)&31;fn=w&63;imm=w&65535;si=imm-65536 if imm&32768 else imm
        nextpc=pc+4;newpending=None
        if op==0:
            if fn==0:r[rd]=(r[rt]<<sa)&MASK
            elif fn==0x21:r[rd]=(r[rs]+r[rt])&MASK
            elif fn==0x23:r[rd]=(r[rs]-r[rt])&MASK
            elif fn==0x25:r[rd]=r[rs]|r[rt]
            elif fn==8:newpending=r[rs]
            else:raise AssertionError(('unknown SPECIAL',hex(pc),fn))
        elif op==9:r[rt]=(r[rs]+si)&MASK
        elif op==15:r[rt]=imm<<16
        elif op in (4,5):
            take=(r[rs]==r[rt]);take=take if op==4 else not take
            newpending=pc+4+si*4 if take else pc+8
        elif op==3:r[31]=pc+8;newpending=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
        elif op==35:
            addr=(r[rs]+si)&MASK;assert addr%4==0;r[rt]=int.from_bytes(read(addr,4),'big')
        elif op==43:
            addr=(r[rs]+si)&MASK;assert addr%4==0;write(addr,r[rt].to_bytes(4,'big'))
        elif op==40:write((r[rs]+si)&MASK,bytes([r[rt]&255]))
        elif op in (42,46):
            addr=(r[rs]+si)&MASK;aligned=addr&~3;k=addr&3;b=r[rt].to_bytes(4,'big')
            if op==42:write(addr,b[:4-k])
            else:write(aligned,b[3-k:])
        else:raise AssertionError(('unknown opcode',hex(pc),op))
        r[0]=0
        if pending is not None:
            assert newpending is None,'branch in delay slot';nextpc=pending
        pending=newpending;pc=nextpc
    assert pending is None and r[29]==SP and r[31]==STOP
    assert all(r[i]==original[i] for i in range(16,24)) and r[30]==original[30]
    want,trace=expected(slot,active,seed);assert m==want and events==trace
    allowed_stack={SP-12,SP,SP-4}
    assert all(VOICE<=a<VOICE+len(m) or a in allowed_stack for a,n in stores)
    return {'visited':visited,'memory':bytes(m),'events':events,'stack':bytes(stack)}

def verify(native,linked):
    seen=set();count=0
    for seed in range(32):
        for active in (0,1):
            for slot in list(range(32))+[MASK]:
                a=run(native,slot,active,seed);b=run(linked,slot,active,seed)
                assert a==b;seen|=a['visited'];count+=1
    assert len(seen)==31
    rejected=[]
    # Real instruction mutations: preserve decoder legality while changing behavior.
    for name,offset,new in [('wrong_identifier_field',0x54,lambda w:(w&0xffff0000)|0x64),('wrong_clear_field',0x68,lambda w:(w&0xffff0000)|0xbc),('wrong_sentinel',0x04,lambda w:(w&0xffff0000)|0xfffe)]:
        ws=list(native);ws[offset//4]=new(ws[offset//4])
        try:
            for args in [(0,1,5),(31,0,3),(MASK,1,7)]:run(ws,*args)
        except AssertionError:rejected.append(name)
        else:raise AssertionError('mutant survived '+name)
    return dict(cases=count,native_and_linked_executions=count*2,all_instruction_offsets_covered=sorted(seen),negative_controls_rejected=rejected,scope='1F954 body with explicit side-effecting helper hooks; valid slots 0..31 and FFFFFFFF only; no helper/native whole-runtime claim')
