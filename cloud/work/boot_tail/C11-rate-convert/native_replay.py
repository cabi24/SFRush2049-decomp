"""Fail-closed native replay for finite binary32-to-u32 conversion fixtures.

Loads canonical words at runtime. Models only the opcode/FCSR subset in this
body, round-to-nearest binary32 arithmetic and truncating representable casts.
"""
import math
import struct
MASK = 0xFFFFFFFF
BASE = 0x8001E50C
RETURN = 0x00F00000


def signed(value):
    return value - 0x100000000 if value & 0x80000000 else value


def bits(value):
    return struct.unpack('>I', struct.pack('>f', value))[0]


def floating(value):
    return struct.unpack('>f', struct.pack('>I', value))[0]


def f32(value):
    return floating(bits(value))


def reference(note, encoded, pool, rate):
    if encoded == MASK:
        encoded = 0x40005622
    key, value = encoded >> 24, float(encoded & 0xFFFFFF)
    if key < note:
        value = f32(pool[note - key] * value)
    elif key > note:
        value = f32(pool[128 + key - note] * value)
    value = f32(f32(value * 4096.) / f32(float(rate)))
    if not math.isfinite(value) or not 0 <= value < 4294967296.:
        raise ValueError('outside proved unsigned-cast domain')
    return math.trunc(value)


def execute(words, note, encoded, pool_bits, rate):
    reg, fp = [0] * 32, [0] * 32
    reg[4], reg[5], reg[29], reg[31] = note, encoded, 0x700100, RETURN
    stack = {}
    fcsr, pc, steps = 0, BASE, 0
    reads = set()
    def memory(address, size, value=None):
        if value is not None:
            assert 0x700000 <= address <= 0x700200 - size
            for i in range(size):
                stack[address+i] = value >> (8*(size-1-i)) & 255
            return
        if address == 0x8004F800 and size == 4:
            return rate & MASK
        if 0x8002C640 <= address < 0x8002CC40 and size == 4:
            assert address % 4 == 0
            reads.add(address)
            return pool_bits[(address - 0x8002C640) // 4]
        return int.from_bytes(bytes(stack[address+i] for i in range(size)), 'big')
    def operation(pc, delay=False):
        nonlocal fcsr
        assert BASE <= pc < BASE+len(words)*4 and pc % 4 == 0
        w=words[(pc-BASE)//4];op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31;shift=(w>>6)&31;fn=w&63;imm=w&65535;si=imm if imm<32768 else imm-65536
        branch=None
        if op==0:
            if fn==0:reg[rd]=reg[rt]<<shift&MASK
            elif fn==2:reg[rd]=reg[rt]>>shift
            elif fn==3:reg[rd]=signed(reg[rt])>>shift&MASK
            elif fn==8:branch=(True,reg[rs],False)
            elif fn==33:reg[rd]=(reg[rs]+reg[rt])&MASK
            elif fn==35:reg[rd]=(reg[rs]-reg[rt])&MASK
            elif fn==36:reg[rd]=reg[rs]&reg[rt]
            elif fn==37:reg[rd]=reg[rs]|reg[rt]
            elif fn==42:reg[rd]=int(signed(reg[rs])<signed(reg[rt]))
            else:raise AssertionError(('SPECIAL',fn))
        elif op==1:
            assert rt in (0,1,2,3)
            branch=((signed(reg[rs])<0) if rt in (0,2) else (signed(reg[rs])>=0),pc+4+4*si,rt in (2,3))
        elif op in (4,5,20,21):branch=((reg[rs]==reg[rt]) if op in (4,20) else (reg[rs]!=reg[rt]),pc+4+4*si,op>=20)
        elif op==9:reg[rt]=(reg[rs]+si)&MASK
        elif op==12:reg[rt]=reg[rs]&imm
        elif op==13:reg[rt]=reg[rs]|imm
        elif op==15:reg[rt]=imm<<16
        elif op in (35,36,37,40,41,43,49):
            address=(reg[rs]+si)&MASK
            if op in (35,36,37):reg[rt]=memory(address,{35:4,36:1,37:2}[op])
            elif op==49:fp[rt]=memory(address,4)
            else:memory(address,{40:1,41:2,43:4}[op],reg[rt])
        elif op==17:
            if rs==0:reg[rt]=fp[rd]
            elif rs==4:fp[rd]=reg[rt]
            elif rs==2:assert rd==31;reg[rt]=fcsr
            elif rs==6:assert rd==31;fcsr=reg[rt]
            elif rs==20:
                assert fn==32
                fp[shift]=bits(float(signed(fp[rd])))
            elif rs==16:
                a,b=floating(fp[rd]),floating(fp[rt])
                if fn==0:fp[shift]=bits(a+b)
                elif fn==1:fp[shift]=bits(a-b)
                elif fn==2:fp[shift]=bits(a*b)
                elif fn==3:assert b!=0;fp[shift]=bits(a/b)
                elif fn==36:
                    assert fcsr&3==1,'expected truncate-toward-zero rounding mode'
                    value=math.trunc(a)
                    if not -2147483648<=value<=2147483647:
                        fcsr|=0x40;fp[shift]=0x80000000
                    else:fp[shift]=value&MASK
                else:raise AssertionError(('single operation',fn))
            else:raise AssertionError(('COP1',rs))
        else:raise AssertionError(('opcode',op))
        reg[0]=0
        if delay:assert branch is None
        return branch
    while pc!=RETURN:
        steps+=1;assert steps<300
        branch=operation(pc)
        if branch is None:pc+=4
        else:
            taken,target,likely=branch
            if taken or not likely:operation(pc+4,True)
            pc=target if taken else pc+8
    assert reg[29]==0x700100 and fcsr==0 and not any(fp[20:]), 'ABI/FCSR restoration'
    return reg[2],reads
