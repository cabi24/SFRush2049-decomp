"""Bounded integer MIPS-II interpreter for this routine, with fail-closed memory.
External calls model only verified strcmp/strcpy contracts. Stack reads track
initialization; the native speculative load of best is recorded, not fabricated.
"""
def signed(x,bits=32):
    return x-(1<<bits) if x&(1<<(bits-1)) else x

def execute(words,start,names,ages,refs,name,stack_fill=0x5a):
    regions={0x80151968:bytearray(b''.join(names)),0x80151a78:bytearray().join(a.to_bytes(2,'big') for a in ages),0x80151690:bytearray().join((r&0xffffffff).to_bytes(4,'big') for r in refs),0x600000:bytearray(name),0x700000:bytearray([stack_fill])*512}
    initialized=set();uninitialized=[];trace=[]
    def mem(addr,n,value=None):
        assert addr%n==0
        for base,data in regions.items():
            k=addr-base
            if 0<=k and k+n<=len(data):
                if value is None:
                    if base==0x700000 and any(addr+j not in initialized for j in range(n)): uninitialized.append((addr,n))
                    return int.from_bytes(data[k:k+n],'big')
                data[k:k+n]=(value&((1<<(8*n))-1)).to_bytes(n,'big');initialized.update(range(addr,addr+n));return
        raise AssertionError(('unmapped',hex(addr),n))
    def cstr(addr):
        result=[]
        for j in range(13):
            b=mem(addr+j,1)
            if b==0:return bytes(result)
            result.append(b)
        raise AssertionError('unterminated string outside bounded domain')
    r=[0xa5000000+i for i in range(32)];r[0]=0;r[4]=0x600000;r[29]=0x700100;r[31]=0xfffffffc
    saved=r[:];pc=start;pending=None;lo=0;steps=0
    while pc!=0xfffffffc:
        assert steps<20000;steps+=1
        if pc in (0x8008ad04,0x800a473c):
            a,b=r[4],r[5];left,right=cstr(a),cstr(b)
            if pc==0x8008ad04:
                result=0
                for x,y in zip(left+b'\0',right+b'\0'):
                    if x!=y:result=x-y;break
                trace.append(('compare',a,left,right,result))
            else:
                for j,x in enumerate(right+b'\0'):mem(a+j,1,x)
                result=a;trace.append(('copy',a,right))
            # Actual helpers are leaves. Clobber caller-save temporaries to
            # ensure the reconstructed routine never relies on their contents.
            for k in (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25):r[k]=0xb6000000+k
            r[2]=result&0xffffffff;pc=r[31];continue
        assert start<=pc<start+len(words)*4,hex(pc)
        w=words[(pc-start)//4];op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31;sa=(w>>6)&31;im=w&65535;si=signed(im,16)
        old=pending;pending=None;nextpc=pc+4;addr=(r[rs]+si)&0xffffffff
        if op==0:
            fn=w&63
            if fn==0:r[rd]=(r[rt]<<sa)&0xffffffff
            elif fn==3:r[rd]=(signed(r[rt])>>sa)&0xffffffff
            elif fn==8:pending=r[rs]
            elif fn==0x12:r[rd]=lo
            elif fn==0x19:lo=(r[rs]*r[rt])&0xffffffff
            elif fn==0x21:r[rd]=(r[rs]+r[rt])&0xffffffff
            elif fn==0x23:r[rd]=(r[rs]-r[rt])&0xffffffff
            elif fn==0x25:r[rd]=r[rs]|r[rt]
            elif fn==0x2a:r[rd]=int(signed(r[rs])<signed(r[rt]))
            else:raise AssertionError(('SPECIAL',hex(w)))
        elif op==3:r[31]=pc+8;pending=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
        elif op==15:r[rt]=im<<16
        elif op==9:r[rt]=addr
        elif op==10:r[rt]=int(signed(r[rs])<si)
        elif op in (4,5,20,21):
            take=(r[rs]==r[rt]) if op in (4,20) else (r[rs]!=r[rt])
            if take:pending=pc+4+4*si
            elif op in (20,21):nextpc+=4
        elif op==1:
            assert rt==0
            if signed(r[rs])<0:pending=pc+4+4*si
        elif op in (32,33,35,36,37):
            n={32:1,33:2,35:4,36:1,37:2}[op];v=mem(addr,n);r[rt]=(signed(v,n*8) if op in (32,33) else v)&0xffffffff
        elif op in (40,41,43):mem(addr,{40:1,41:2,43:4}[op],r[rt])
        else:raise AssertionError(('opcode',hex(w),hex(pc)))
        r[0]=0;pc=old if old is not None else nextpc
    assert r[16:24]==saved[16:24] and r[30]==saved[30] and r[29]==saved[29], 'ABI preservation'
    return signed(r[2]),bytes(regions[0x80151968]),bytes(regions[0x80151a78]),bytes(regions[0x80151690]),trace,uninitialized
