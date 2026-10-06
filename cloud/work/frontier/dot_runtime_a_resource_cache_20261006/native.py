"""Bounded fail-closed integer MIPS execution; no embedded target words."""
import random
import struct
ENTRY=0x80390BC0
SIZE=364
RECORDS=0x803BA230
TABLE=0x80142B08
OUT=0x81001000
SP=0x81002000
RETURN=0x81234560
LOAD=0x80097798
SETUP=0x800BB02C

def signed(x,bits=32):
    x &= (1<<bits)-1
    return x-(1<<bits) if x&(1<<(bits-1)) else x

def read(m,a,n):
    assert a%n==0 and all(a+i in m for i in range(n)),('bad read',hex(a),n)
    return int.from_bytes(bytes(m[a+i] for i in range(n)),'big')

def write(m,a,n,v):
    assert a%n==0 and all(a+i in m for i in range(n)),('bad write',hex(a),n)
    for i,b in enumerate((v&((1<<(n*8))-1)).to_bytes(n,'big')):m[a+i]=b

def state(records,salt):
    rng=random.Random(salt)
    m={a:rng.randrange(256) for lo,hi in ((RECORDS-8,RECORDS+200),(TABLE-8,TABLE+34),(OUT-8,OUT+12)) for a in range(lo,hi)}
    for i,(handle,loaded,ident) in enumerate(records):
        p=RECORDS+i*12;write(m,p,4,handle);m[p+4]=loaded&255;m[p+5]=ident&255
    return m

def helper(memory,which,mode,index):
    """Explicit side-effecting O32 hook, separate from real helper bodies."""
    if mode:
        p=RECORDS+((index+7)%16)*12
        if which==LOAD:
            write(memory,p,4,0x12345678);memory[p+4]=0x80
        else:
            memory[p+5]=0xfd;write(memory,OUT,4,0x76543210)

def oracle(initial,ident,kind,handle,mode):
    m=dict(initial);trace=[]
    for i in range(16):
        p=RECORDS+i*12
        if signed(m[p+5],8)==ident and m[p+4]!=0:
            write(m,OUT,4,read(m,p,4));return m,1,trace,'hit'
    matches=[i for i in range(16) if signed(m[RECORDS+i*12+5],8)==ident]
    available=[i for i in range(16) if signed(m[RECORDS+i*12+5],8)<0]
    if not matches and not available:return m,0,trace,'full'
    i=matches[0] if matches else available[0];p=RECORDS+i*12
    if not matches:m[p+5]=ident&255
    trace.append((LOAD,(kind+88,1,1,0,0),dict(m)))
    helper(m,LOAD,mode,i)
    write(m,p,4,handle);write(m,OUT,4,handle);m[p+4]=1
    write(m,TABLE+ident*2,2,read(m,p,4))
    trace.append((SETUP,(ident,kind,0),dict(m)))
    helper(m,SETUP,mode,i)
    return m,1,trace,'reload' if matches else 'allocate'

def run(words,initial,ident,kind,handle,mode,salt=0):
    assert len(words)==SIZE//4,'wrong full-function extent'
    rng=random.Random(salt^0xab9812);r=[rng.getrandbits(32) for _ in range(32)]
    r[0]=0;r[4]=ident&0xffffffff;r[5]=kind&0xffffffff;r[6]=OUT;r[29]=SP;r[31]=RETURN
    before=list(r);m=dict(initial)
    for a in range(SP-64,SP+32):m[a]=rng.randrange(256)
    stack=dict(m);pc=ENTRY;pending=None;trace=[];coverage=set();branches=set();writes=[]
    allowed=set(range(OUT,OUT+4))|set(range(TABLE,TABLE+26))
    for i in range(16):allowed.update(range(RECORDS+i*12,RECORDS+i*12+6))
    allowed.update(range(SP-48,SP+12))
    for step in range(2048):
        if pc==RETURN:
            assert pending is None
            assert all(r[i]==before[i] for i in (*range(16,24),28,29,30,31)), 'callee-save or return corruption'
            assert all(m[a]==stack[a] for a in range(SP-64,SP-48))
            assert all(m[a]==stack[a] for a in range(SP+12,SP+32))
            return {'memory':{a:m[a] for a in initial},'return':signed(r[2]),'calls':trace,'coverage':coverage,'branches':branches,'writes':writes}
        if pc in (LOAD,SETUP):
            args=tuple(signed(r[i]) for i in range(4,8))+(read(m,r[29]+16,4),) if pc==LOAD else tuple(signed(r[i]) for i in range(4,7))
            index=next((i for i in range(16) if signed(m[RECORDS+i*12+5],8)==ident),-1)
            assert index>=0,'missing selected record at helper boundary'
            trace.append((pc,args,{a:m[a] for a in initial}));helper(m,pc,mode,index)
            ret=r[31]
            for i in (1,2,3,*range(4,16),24,25):r[i]=rng.getrandbits(32)
            r[2]=handle&0xffffffff if pc==LOAD else 0x981237ab
            pc=ret;continue
        assert ENTRY<=pc<ENTRY+SIZE and pc%4==0,('escaped function',hex(pc))
        off=pc-ENTRY;coverage.add(off);w=words[off//4]
        op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;fn=w&63;imm=signed(w,16)
        delayed=pending;pending=None;transfer=None;annul=False
        if w==0:pass
        elif op==0:
            if fn==0:r[rd]=(r[rt]<<((w>>6)&31))&0xffffffff
            elif fn==3:r[rd]=(signed(r[rt])>>((w>>6)&31))&0xffffffff
            elif fn==33:r[rd]=(r[rs]+r[rt])&0xffffffff
            elif fn==35:r[rd]=(r[rs]-r[rt])&0xffffffff
            elif fn==37:r[rd]=r[rs]|r[rt]
            elif fn==42:r[rd]=int(signed(r[rs])<signed(r[rt]))
            elif fn==8:
                assert rs==31 and (w&0x1fffff)==8;transfer=r[rs]
            else:raise AssertionError(('unknown special',off,fn))
        elif op==15:assert rs==0;r[rt]=(w&65535)<<16
        elif op==9:r[rt]=(r[rs]+imm)&0xffffffff
        elif op==10:r[rt]=int(signed(r[rs])<imm)
        elif op in (32,33,35):
            n={32:1,33:2,35:4}[op];a=(r[rs]+imm)&0xffffffff;v=read(m,a,n)
            r[rt]=(signed(v,n*8) if n<4 else v)&0xffffffff
        elif op in (40,41,43):
            n={40:1,41:2,43:4}[op];a=(r[rs]+imm)&0xffffffff
            assert all(a+j in allowed for j in range(n)),('unexpected store',hex(a),n)
            write(m,a,n,r[rt]);writes.append((a,n,r[rt]&((1<<(8*n))-1)))
        elif op in (1,4,5,20,21):
            if op==1:assert rt==0;taken=signed(r[rs])<0
            else:taken=(r[rs]==r[rt]) if op in (4,20) else (r[rs]!=r[rt])
            branches.add((off,taken));annul=op in (20,21) and not taken
            transfer=pc+4+imm*4 if taken else pc+8
        elif op==3:
            transfer=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
            assert transfer in (LOAD,SETUP),'unknown external call';r[31]=pc+8
        else:raise AssertionError(('unknown opcode',off,op))
        r[0]=0
        if delayed is not None:assert transfer is None;pc=delayed
        elif annul:pc=pc+8
        else:pc,pending=pc+4,transfer
    raise AssertionError('execution step limit')
