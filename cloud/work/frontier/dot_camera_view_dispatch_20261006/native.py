"""Fail-closed finite-domain MIPS-II interpreter for the EB028 outer body.
No native payload is embedded. External calls have explicit O32 contract hooks.
Binary32 round-to-nearest is modeled; FCSR/exceptions and NaNs are excluded.
"""
import struct
MASK=0xffffffff
STACK=0x700100
RETURN=0xfffffffc

def signed(x,n=32):
    x &= (1<<n)-1
    return x-(1<<n) if x&(1<<(n-1)) else x

def bits(x): return struct.unpack('>I',struct.pack('>f',x))[0]
def value(x): return struct.unpack('>f',struct.pack('>I',x&MASK))[0]
def f32(x): return value(bits(x))

def execute(words,start,regions,argument,hook,coverage=None,branches=None,position_home=48):
    regions=[(a,bytearray(b)) for a,b in regions]
    regions.append((STACK-256,bytearray([0xcd]*320)))
    reads=[];writes=[]
    def memory(a,n=4,v=None):
        assert a%n==0,('unaligned',hex(a),n)
        for base,data in regions:
            o=a-base
            if 0<=o and o+n<=len(data):
                if v is None:
                    reads.append((a,n));return int.from_bytes(data[o:o+n],'big')
                writes.append((a,n));data[o:o+n]=(v&((1<<(8*n))-1)).to_bytes(n,'big');return
        raise AssertionError(('unmapped',hex(a),n))
    r=[0xa5000000+i for i in range(32)];r[0]=0;r[4]=argument;r[29]=STACK;r[31]=RETURN
    fp=[bits(i+0.125) for i in range(32)];initial_r=r[:];initial_fp=fp[:]
    condition=False;pc=start;pending=None;steps=0
    while pc!=RETURN:
        assert pc%4==0 and start<=pc<start+len(words)*4 and steps<2000,('control flow',hex(pc),steps)
        if coverage is not None:coverage.add(pc-start)
        w=words[(pc-start)//4];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63
        imm=w&65535;si=signed(imm,16);a,b=r[rs],r[rt]
        old=pending;pending=None;nxt=pc+4
        control=(op in (1,3,4,5,20,21) or (op==0 and fn==8) or (op==17 and rs==8))
        assert old is None or not control, ('transfer in delay slot',hex(pc))
        def put(reg,v):
            if reg:r[reg]=v&MASK
        def branch(take,likely=False):
            nonlocal pending,nxt
            if branches is not None:branches.add((pc-start,bool(take)))
            if take:pending=pc+4+4*si
            elif likely:nxt+=4
            else:pending=pc+8
        if w==0:pass
        elif op==0:
            if fn==0:put(rd,b<<sh)
            elif fn==3:put(rd,signed(b)>>sh)
            elif fn==0x21:put(rd,a+b)
            elif fn==0x23:put(rd,a-b)
            elif fn==0x25:put(rd,a|b)
            elif fn==8:pending=a
            else:raise AssertionError(('SPECIAL',hex(w),hex(pc)))
        elif op==1:
            assert rt in (0,1,2,3),('REGIMM',rt)
            branch((signed(a)>=0) if rt&1 else (signed(a)<0),bool(rt&2))
        elif op==3:r[31]=pc+8;pending=('call',((pc+4)&0xf0000000)|((w&0x3ffffff)<<2),pc+8)
        elif op in (4,5,20,21):branch((a==b)==(op in (4,20)),op in (20,21))
        elif op==9:put(rt,a+si)
        elif op==10:put(rt,signed(a)<si)
        elif op==11:put(rt,a<(si&MASK))
        elif op==15:put(rt,imm<<16)
        elif op in (32,33,35,36,37):
            n={32:1,33:2,35:4,36:1,37:2}[op];v=memory((a+si)&MASK,n)
            put(rt,signed(v,n*8) if op in (32,33) else v)
        elif op==43:memory((a+si)&MASK,4,b)
        elif op==49:fp[rt]=memory((a+si)&MASK)
        elif op==57:memory((a+si)&MASK,4,fp[rt])
        elif op==17:
            if rs==4:fp[rd]=r[rt]
            elif rs==8:
                assert rt in (0,1,2,3);branch(condition==bool(rt&1),bool(rt&2))
            elif rs==16:
                x,y=value(fp[rd]),value(fp[rt])
                if fn==0:fp[sh]=bits(x+y)
                elif fn==1:fp[sh]=bits(x-y)
                elif fn==2:fp[sh]=bits(x*y)
                elif fn==0x3c:condition=x<y
                elif fn==0x3e:condition=x<=y
                else:raise AssertionError(('COP1',hex(w),hex(pc)))
            else:raise AssertionError(('COP1 format',rs))
        else:raise AssertionError(('opcode',hex(w),hex(pc)))
        r[0]=0
        assert old is None or pending is None, ('transfer in delay slot',hex(pc))
        if isinstance(old,tuple):
            _,target,resume=old
            answer=hook(target,r[4:8],memory)
            for i in [*range(1,16),24,25]:r[i]=0xbad00000+i
            for i in range(20):fp[i]=bits(100.0+i)
            if answer is not None:fp[0]=answer
            
            if target==0x800c69c0:r[2]=0
            pc=resume
        else:pc=old if old is not None else nxt
        steps+=1
    assert r[29]==STACK and r[31]==RETURN,'stack/return restoration'
    assert r[16:24]==initial_r[16:24] and r[28]==initial_r[28] and r[30]==initial_r[30],'integer saved registers'
    assert fp[20:]==initial_fp[20:],'floating saved registers'
    frame=-signed(words[0]&65535,16);assert frame==96,frame
    allowed={STACK-frame+i+j for i in (24,28,position_home,56,60,64,68,72,76,80,84,96) for j in range(4)}
    for a,n in writes:
        if STACK-256<=a<STACK+64:assert set(range(a,a+n))<=allowed,('stack write',hex(a),n,[(hex(x),y) for x,y in writes if STACK-256<=x<STACK+64])
    for i,b in enumerate(regions[-1][1]):
        if STACK-256+i not in allowed:assert b==0xcd,'stack canary'
    return regions[:-1],reads,writes
