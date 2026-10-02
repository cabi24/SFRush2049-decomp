"""Research proof: GNU-linked complete words plus bounded MIPS differential tests."""
import hashlib, json, os, random, struct, subprocess, sys, tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
HERE=Path(__file__).resolve().parent
NAME='func_800B3704'
sys.path.insert(0,str(ROOT/'tools/cloud'))
import score
FLAGS=['-g0','-O2','-mips2','-G','0','-non_shared','-Wab,-r4300_mul']
def signed(n): return n-0x100000000 if n&0x80000000 else n

def execute(words,symbols,count,owner,x,y,initial,resource,handle,seed):
    """Fail-closed integer interpreter. Calls use explicit observable ABI stubs."""
    base=symbols[NAME]; stack=0x80700000; obj=0x80500000; stop=0x81234560
    mem={}; calls=[]; writes=[]
    def put(a,v,n=4):
        for i,b in enumerate((v&((1<<(8*n))-1)).to_bytes(n,'big')): mem[a+i]=b
    def get(a,n=4): return int.from_bytes(bytes(mem[a+i] for i in range(n)),'big')
    for a in range(stack-64,stack+32): mem[a]=0
    for i,b in enumerate(initial): mem[obj+i]=b
    put(symbols['D_80149788'],count)
    if count<200: put(symbols['D_80149450']+4*count,obj)
    regs=[(seed*1664525+i*1013904223)&0xffffffff for i in range(32)]
    regs[4:7]=[owner,x&0xffffffff,y&0xffffffff];regs[29]=stack;regs[31]=stop;regs[0]=0
    preserved={i:regs[i] for i in [*range(16,24),28,30]}
    pc=base;pending=None;steps=0
    while pc!=stop:
        assert steps<100 and base<=pc<base+4*len(words),(hex(pc),steps)
        w=words[(pc-base)//4];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;imm=w&65535;si=imm if imm<32768 else imm-65536
        old=pending;pending=None;nextpc=pc+4;a=(regs[rs]+si)&0xffffffff
        if w==0: pass
        elif op==0:
            fn=w&63
            if fn==0: regs[rd]=regs[rt]<<(w>>6&31)
            elif fn==33: regs[rd]=regs[rs]+regs[rt]
            elif fn==37: regs[rd]=regs[rs]|regs[rt]
            elif fn==8: pending=regs[rs]
            else: raise AssertionError(hex(w))
        elif op==15:regs[rt]=imm<<16
        elif op==9:regs[rt]=a
        elif op==10:regs[rt]=int(signed(regs[rs])<si)
        elif op in (4,5):
            if (regs[rs]==regs[rt])==(op==4):pending=pc+4+4*si
        elif op==3:regs[31]=pc+8;pending=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
        elif op in (35,37):regs[rt]=get(a,4 if op==35 else 2)
        elif op in (40,41,43):
            n={40:1,41:2,43:4}[op]
            assert (stack-64<=a and a+n<=stack+32) or (obj<=a and a+n<=obj+64) or a==symbols['D_80149788']
            put(a,regs[rt],n)
            if not stack-64<=a<stack+32:writes.append((a,n,regs[rt]&((1<<(n*8))-1)))
        else:raise AssertionError(hex(w))
        regs=[v&0xffffffff for v in regs];regs[0]=0
        if old is not None:
            if old in [symbols[n] for n in ['func_800B362C','func_800A79F4','func_80094EC8']]:
                ret=regs[31]
                if old==symbols['func_800B362C']:
                    assert regs[4]==obj
                    calls.append(('initialize',bytes(mem[obj+i] for i in range(64))))
                    put(obj+12,resource,2) # must be read after this callback
                    put(obj+14,0x6abc,2);put(obj+16,0x789a,2) # later call must use original arguments
                    value=0xabcddcba
                elif old==symbols['func_800A79F4']:
                    calls.append(('create',*regs[4:8],get(regs[29]+16),get(regs[29]+20),get(regs[29]+24)))
                    value=handle
                else:
                    assert regs[4]==obj
                    calls.append(('finish',get(obj+52,2)))
                    value=0xdefaced
                for i in [1,2,3,*range(4,16),24,25]:regs[i]=(seed*69069+i*1234567)&0xffffffff
                regs[2]=value&0xffffffff;pc=ret
            else:pc=old
        else:pc=nextpc
        steps+=1
    assert regs[29]==stack and all(regs[i]==v for i,v in preserved.items())
    assert regs[2]==(obj if count<200 else 0)
    return get(symbols['D_80149788']),bytes(mem[obj+i] for i in range(64)),calls,writes

def run():
    symbols=score.image_symbols();symbols.update({n:score.address_named(n) for n in ['D_80149788','D_80149450','func_800B362C','func_800A79F4','func_80094EC8']});target=score.targets()[NAME]
    assert len(target)==57
    with tempfile.TemporaryDirectory() as td:
        td=Path(td);source=HERE/(NAME+'.c')
        subprocess.run([str(score.IDO.resolve()/'cc'),'-c',*FLAGS,str(source),'-o',str(td/'candidate.o')],check=True)
        names=['D_80149788','D_80149450','func_800B362C','func_800A79F4','func_80094EC8']
        (td/'link.ld').write_text('OUTPUT_ARCH(mips)\nSECTIONS { . = '+hex(symbols[NAME])+'; .text : SUBALIGN(4) { *(.text) } }\n'+'\n'.join(n+' = '+hex(symbols[n])+';' for n in names))
        subprocess.run(['mips-linux-gnu-ld','-T',str(td/'link.ld'),'-o',str(td/'candidate.elf'),str(td/'candidate.o')],check=True,capture_output=True)
        subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(td/'candidate.elf'),str(td/'candidate.bin')],check=True)
        data=(td/'candidate.bin').read_bytes();candidate=list(struct.unpack('>%dI'%(len(data)//4),data))
        assert len(candidate)==56
        differences=[{'offset':i*4,'target':'%08x'%w,'candidate':'%08x'%candidate[i] if i<len(candidate) else None} for i,w in enumerate(target) if i>=len(candidate) or candidate[i]!=w]
        result=score.compare(td/'candidate.o',NAME,show=0)
        assert len(differences)==41 and result.differing==41 and not result.accepted(False)
        assert not result.errors and not result.unresolved and not result.unverified and result.extra_words==0
        rng=random.Random(0xb3704);cases=0
        for i in range(10000):
            count=[0,1,198,199,200,201,0x7fffffff][i%7] if i<100 else rng.randrange(205)
            initial=bytes(rng.randrange(256) for _ in range(64));owner=rng.getrandbits(32)
            x=signed(rng.getrandbits(32));y=signed(rng.getrandbits(32));resource=rng.randrange(65536);handle=rng.getrandbits(32)
            args=(symbols,count,owner,x,y,initial,resource,handle,i)
            a=execute(target,*args);b=execute(candidate,*args)
            assert a==b,(i,a,b)
            if count>=200: assert a==(count,initial,[],[])
            else:
                assert a[0]==count+1
                assert a[2][1]==('create',resource,0,0,x&0xffffffff,y&0xffffffff,0xffffffff,0xffffffff)
                assert a[2][2]==('finish',handle&65535)
            cases+=1
        evidence={'verdict':'NONMATCH','claims':[],'target_words':57,'target_bytes':228,'candidate_words':56,'candidate_bytes':224,'differing_words':41,'differences':differences,'target_words_hex':['%08x'%x for x in target],'resolved_candidate_words_hex':['%08x'%x for x in candidate],'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'target_sha256':hashlib.sha256(struct.pack('>57I',*target)).hexdigest(),'compiler_sha256':hashlib.sha256((score.IDO/'cc').read_bytes()).hexdigest(),'differential_cases':cases,'unresolved':[],'unverified':[],'flags':FLAGS,'scope':'Research only; no match or coverage claim'}
        (HERE/'verification.json').write_text(json.dumps(evidence,indent=2)+'\n')
        print('NONMATCH 41/57 words, 224/228 bytes; 10000 complete target/candidate differential cases passed')
if __name__=='__main__':run()
