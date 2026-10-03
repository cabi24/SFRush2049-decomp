"""Independent non-match proof and small bounded MIPS semantic interpreter.
No project scorer parsing or relocation implementation is reused.
"""
from pathlib import Path
import hashlib, json, os, re, struct, subprocess, random, tempfile
ROOT = Path(__file__).resolve().parents[3]
NAME = 'audio_output_setup'
HERE = Path(__file__).resolve().parent
FLAGS = ['-g0', '-O2', '-mips2', '-G', '0', '-non_shared', '-Wab,-r4300_mul']

def targets():
    a = ROOT / 'asm/us/blob'
    entries = (a / 'SHA256SUMS').read_text().splitlines()
    assert {p.name for p in a.glob('*.s')} == {line.split('  ')[1] for line in entries if line.endswith('.s')}
    for line in entries:
        h, p = line.split('  ')
        assert hashlib.sha256((a / p).read_bytes()).hexdigest() == h
    words = []
    active = False
    for p in a.glob('*.s'):
        for line in p.read_text().splitlines():
            if line.startswith('.section'):
                active = line.startswith('.section .text.' + NAME + ',')
            if active and '.word' in line:
                words.append(int(line.split('.word')[1], 16))
    symbols = {k: int(v, 16) for k,v in json.loads((a/'symbols.json').read_text())['symbols'].items()}
    assert len(words) == 37
    return words, symbols

def compile_link(out, symbols, source=NAME+'.c'):
    ido = Path(os.environ.get('IDO_DIR', ROOT/'tools/cloud/ido')).resolve()
    subprocess.run([str(ido/'cc'), '-c', *FLAGS, str(HERE/source), '-o', str(out/'candidate.o')], check=True)
    names = ['D_80152770', 'D_801527C8', 'osRecvMesg', 'osJamMesg']
    script = 'OUTPUT_ARCH(mips)\nENTRY('+NAME+')\nSECTIONS { . = '+hex(symbols[NAME])+'; .text : SUBALIGN(4) { *(.text) } }\n'
    script += '\n'.join(k+' = '+hex(symbols[k])+';' for k in names)
    (out/'link.ld').write_text(script)
    subprocess.run(['mips-linux-gnu-ld','-T',str(out/'link.ld'),'-o',str(out/'candidate.elf'),str(out/'candidate.o')],check=True,capture_output=True)
    subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(out/'candidate.elf'),str(out/'candidate.bin')],check=True)
    for tool, args, name in [('objdump',['-dr'],'object.txt'),('readelf',['-a'],'elf.txt')]:
        (out/name).write_text(subprocess.check_output(['mips-linux-gnu-'+tool,*args,str(out/'candidate.o')],text=True))
    body=(out/'candidate.bin').read_bytes()
    words=list(struct.unpack('>%dI'%(len(body)//4),body))
    if source == NAME+'.c':
        assert len(words)==36
        return words
    assert len(words)==40 and words[37:]==[0,0,0]
    return words[:37]

def emulate(words, symbols, values, excluded, use_default, poison_seed=0):
    """Execute only the documented integer subset used by both whole bodies.
    Includes branch-likely annul, branch/jump delay slots and ABI-clobbering OS stubs.
    Fail closed on every unknown opcode, unmapped read or non-stack write.
    """
    mask=0xffffffff; mem={}; stack=0x80700000; base=symbols[NAME]
    def put(a,v,size=4):
        for i,b in enumerate((v & ((1<<(size*8))-1)).to_bytes(size,'big')): mem[a+i]=b
    def get(a,size=4,signed=False):
        return int.from_bytes(bytes(mem[a+i] for i in range(size)), 'big', signed=signed)
    for a in range(stack-64,stack+32): mem[a]=0
    header=0x80500000; nodes=0x80501000
    put(symbols['D_801527C8'],0xDEAD0000) # Changed only by the acquire stub.
    put(header+8,nodes if values else 0)
    for i,(v,e) in enumerate(zip(values,excluded)):
        a=nodes+32*i;put(a+4,a+32 if i+1<len(values) else 0);put(a+12,v);put(a+20,e,1)
    regs=[0]*32;regs[4]=0 if use_default else header;regs[29]=stack;regs[31]=0x81234560
    preserved={i:0x10000000+i for i in range(16,24)};preserved[28]=0x12345678;preserved[30]=0x34567890
    for i,v in preserved.items():regs[i]=v
    calls=[];steps=0;pc=base;pending=None;data_reads=[]
    def signed(x):return x if x<0x80000000 else x-0x100000000
    while pc!=0x81234560:
        steps+=1;assert steps < 2000+20*len(values)
        i=(pc-base)//4;assert 0<=i<len(words)
        w=words[i];op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31;imm=w&65535;off=imm if imm<32768 else imm-65536
        old_pending=pending;pending=None;nextpc=pc+4
        if w==0:pass
        elif op==0:
            if w&63==33:regs[rd]=(regs[rs]+regs[rt])&mask
            elif w&63==37:regs[rd]=regs[rs]|regs[rt]
            elif w&63==8:pending=regs[rs]
            else:raise AssertionError(hex(w))
        elif op==9:regs[rt]=(regs[rs]+off)&mask
        elif op==15:regs[rt]=imm<<16
        elif op in (35,32):
            a=(regs[rs]+off)&mask
            if not stack-64<=a<stack+32: data_reads.append(a);assert len(calls)==1,'data read outside acquired interval'
            regs[rt]=get(a,4 if op==35 else 1,op==32)&mask
        elif op==43:
            a=(regs[rs]+off)&mask;assert stack-64<=a<stack+32;put(a,regs[rt])
        elif op in (4,5,20,21):
            take=(regs[rs]==regs[rt]) if op in (4,20) else (regs[rs]!=regs[rt])
            if take:pending=pc+4+off*4
            elif op in (20,21):nextpc=pc+8
        elif op==3:
            regs[31]=pc+8;pending=('call',((pc+4)&0xf0000000)|((w&0x3ffffff)<<2))
        else:raise AssertionError(hex(w))
        regs[0]=0
        if old_pending is not None:
            assert pending is None,'branch inside delay slot'
            if isinstance(old_pending,tuple):
                address=old_pending[1]
                expected=symbols['osRecvMesg'] if not calls else symbols['osJamMesg']
                assert address==expected and len(calls)<2
                assert regs[4]==symbols['D_80152770'] and regs[5]==0 and regs[6]==(1 if not calls else 0)
                calls.append(address)
                if len(calls)==1:put(symbols['D_801527C8'],header)
                returnpc=regs[31]
                for r in [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25]:regs[r]=(0x90000000+poison_seed*37+r)&mask
                nextpc=returnpc
            else:nextpc=old_pending
        pc=nextpc
    assert len(calls)==2 and regs[29]==stack and all(regs[i]==v for i,v in preserved.items())
    assert regs[2]==sum(v for v,e in zip(values,excluded) if e==0)&mask
    for i,e in enumerate(excluded):assert ((nodes+i*32+12) in data_reads)==(e==0)
    return regs[2]

def verify(out):
    out.mkdir(parents=True,exist_ok=True)
    target,symbols=targets();candidate=compile_link(out,symbols)
    differences=[i for i in range(max(len(target),len(candidate))) if (target[i] if i<len(target) else None)!=(candidate[i] if i<len(candidate) else None)]
    assert len(differences)==24
    cases=[([],[]),([0],[0]),([0xffffffff,2],[0,0]),([9,8,7],[1,128,255]),([1,2,3],[0,1,0])]
    rng=random.Random(2049)
    for _ in range(2000):
        n=rng.randrange(50);cases.append(([rng.getrandbits(32) for _ in range(n)],[rng.choice([0,0,0,1,127,128,255]) for _ in range(n)]))
    for case,(values,excluded) in enumerate(cases):
        for default in [False,True]:
            assert emulate(target,symbols,values,excluded,default,case)==emulate(candidate,symbols,values,excluded,default,case)
    report={'status':'NONMATCH','base_commit':subprocess.check_output(['git','rev-parse','origin/master'],cwd=ROOT,text=True).strip(),'target_sha256':hashlib.sha256(struct.pack('>37I',*target)).hexdigest(),'flags':FLAGS,'source_sha256':hashlib.sha256((HERE/(NAME+'.c')).read_bytes()).hexdigest(),'target_bytes':148,'candidate_body_bytes':144,'zero_alignment_bytes':0,'full_word_differences':len(differences),'difference_word_indices':differences,'relocation_method':'GNU ld with verified symbol addresses, no masks','semantic_cases':len(cases)*2,'executions':len(cases)*4,'scope':'Bounded whole-body MIPS interpreter, target and candidate; not hardware/ROM validation','abi':'o32 pointer argument, unsigned 32-bit result; callee-saved registers/sp preserved, OS calls poison all integer caller-saved registers','protected_manifest':'all files valid'}
    (out/'verification.json').write_text(json.dumps(report,indent=2)+'\n');return report

if __name__=='__main__':
    import sys
    print(json.dumps(verify(Path(sys.argv[1]) if len(sys.argv)>1 else ROOT/'build/natural_audio_output_setup'),indent=2))
