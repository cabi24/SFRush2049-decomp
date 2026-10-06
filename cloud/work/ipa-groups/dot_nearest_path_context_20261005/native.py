"""Fail-closed MIPS-II leaf interpreter for the private path-search ABI.
No native byte payloads are stored. Binary32 operations round after each step.
"""
import struct

def signed(n, bits=32):
    n &= (1 << bits)-1
    return n-(1 << bits) if n & (1 << (bits-1)) else n

def f32(v):
    return struct.unpack('>f', struct.pack('>f', v))[0]

def bits(v):
    return struct.unpack('>I', struct.pack('>f', v))[0]

def value(v):
    return struct.unpack('>f', struct.pack('>I', v))[0]

def execute(words, start, inputs, arguments):
    regions = [(a, bytearray(b)) for a,b in inputs]
    stack_base, sp, end = 0x700000, 0x700080, 0xfffffffc
    regions.append((stack_base,bytearray([0xa5])*256))
    reads, writes, seen, branches = [],[],set(),{}
    def mem(a, width, write=None):
        assert a % width == 0, ('unaligned',a,width)
        for base, data in regions:
            off=a-base
            if 0 <= off and off+width <= len(data):
                if write is None:
                    reads.append((a,width))
                    return int.from_bytes(data[off:off+width],'big')
                writes.append((a,width))
                data[off:off+width]=(write&((1<<(width*8))-1)).to_bytes(width,'big')
                return
        raise AssertionError(('unmapped',hex(a),width))
    r=[0xa5000000+i for i in range(32)];f=[0x3f000000+i for i in range(32)]
    r[0]=0;r[29]=sp;r[31]=end
    # Exact observed E451C call interface: pos, prev, point, row, current.
    for register,arg in zip([6,10,13,5,8],arguments):r[register]=arg&0xffffffff
    original_r=list(r);original_f=list(f)
    pc,pending,condition,steps=start,None,False,0
    while pc!=end:
        assert start<=pc<start+4*len(words) and (pc-start)%4==0 and steps<1500000,('control',hex(pc),steps)
        seen.add(pc-start);w=words[(pc-start)//4];op=w>>26
        rs,rt,rd,sa=(w>>21)&31,(w>>16)&31,(w>>11)&31,(w>>6)&31
        imm=w&65535;si=signed(imm,16);address=(r[rs]+si)&0xffffffff
        old_pending,pending,next_pc=pending,None,pc+4
        def branch(take,likely=False):
            nonlocal pending,next_pc
            branches.setdefault(pc-start,set()).add(bool(take))
            if take:pending=pc+4+4*si
            elif likely:next_pc+=4
        if w==0:pass
        elif op==0:
            fn=w&63
            if fn==0:r[rd]=(r[rt]<<sa)&0xffffffff
            elif fn==3:r[rd]=(signed(r[rt])>>sa)&0xffffffff
            elif fn==0x21:r[rd]=(r[rs]+r[rt])&0xffffffff
            elif fn==0x23:r[rd]=(r[rs]-r[rt])&0xffffffff
            elif fn==0x25:r[rd]=r[rs]|r[rt]
            elif fn==0x2a:r[rd]=int(signed(r[rs])<signed(r[rt]))
            elif fn==8:pending=r[rs]
            else:raise AssertionError(('unsupported SPECIAL',hex(w)))
        elif op==1:
            assert rt in (0,1,2,3)
            branch(signed(r[rs])>=0 if rt&1 else signed(r[rs])<0,bool(rt&2))
        elif op in (4,5,20,21):branch(r[rs]==r[rt] if op in (4,20) else r[rs]!=r[rt],op>=20)
        elif op==6:branch(signed(r[rs])<=0)
        elif op==9:r[rt]=address
        elif op==10:r[rt]=int(signed(r[rs])<si)
        elif op==15:r[rt]=imm<<16
        elif op in (33,35,37):
            width=4 if op==35 else 2;n=mem(address,width)
            r[rt]=(signed(n,width*8) if op!=37 else n)&0xffffffff
        elif op==43:mem(address,4,r[rt])
        elif op==49:f[rt]=mem(address,4)
        elif op==17:
            if rs==4:f[rd]=r[rt]
            elif rs==8:
                assert rt in (0,1,2,3)
                branch(condition==bool(rt&1),bool(rt&2))
            elif rs==20:
                assert w&63==32
                f[sa]=bits(float(signed(f[rd])))
            elif rs==16:
                fn=w&63;a,b=value(f[rd]),value(f[rt])
                if fn==0:f[sa]=bits(a+b)
                elif fn==2:f[sa]=bits(a*b)
                elif fn==6:f[sa]=f[rd]
                elif fn==0x3c:condition=a<b
                else:raise AssertionError(('unsupported FP',hex(w)))
            else:raise AssertionError(('unsupported COP1',hex(w)))
        else:raise AssertionError(('unsupported word',hex(w)))
        r[0]=0;pc=old_pending if old_pending is not None else next_pc;steps+=1
    assert all(r[i]==original_r[i] for i in list(range(18,24))+list(range(26,32))), 'private ABI preserved register'
    assert f[20:]==original_f[20:],'private ABI preserved FP register'
    expected={(sp+x,4) for x in (4,8,12,16)}
    assert set(writes)==expected and len(writes)==4,'unexpected memory writes'
    stack=regions[-1][1]
    assert stack[:132]==bytes([0xa5])*132 and stack[148:]==bytes([0xa5])*(256-148),'stack canary'
    assert [(a,bytes(b)) for a,b in regions[:-1]]==[(a,bytes(b)) for a,b in inputs],'input mutation'
    assert [int.from_bytes(stack[x:x+4],'big') for x in (132,136,140,144)]==[arguments[i]&0xffffffff for i in (1,2,3,4)],'argument homes'
    return signed(r[2]),seen,branches,reads,writes
