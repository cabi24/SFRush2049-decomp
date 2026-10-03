"""Resolve complete IDO text and compare bounded native/C executions, fail closed."""
import hashlib, json, random, struct, subprocess, sys, tempfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score
NAME = 'func_800B9740'
NAMES = ['D_801407D4', 'D_801407B4', 'D_801407F0', 'D_801409E8', 'D_801527A4']
FLAGS = ['-g0', '-O2', '-mips2', '-G', '0', '-non_shared', '-Wab,-r4300_mul']
def signed(x): return x - 0x100000000 if x & 0x80000000 else x

def execute(words, symbols, points, primary, ranges, seed):
    base = symbols[NAME]; stack = 0x80700000; stop = 0x81234560
    vertices = 0x80500000; descriptors = 0x80600000
    mem = {}; writes = []
    def put(a, v, n=4):
        for i, b in enumerate((v & ((1 << (8*n))-1)).to_bytes(n, 'big')): mem[a+i] = b
    def get(a, n): return int.from_bytes(bytes(mem[a+i] for i in range(n)), 'big')
    for a in range(stack-128, stack+32): mem[a] = 0xa5
    for name,n in [('D_801407D4',6),('D_801407B4',6),('D_801407F0',16)]:
        for a in range(symbols[name], symbols[name]+n): mem[a] = 0xa5
    for i,p in enumerate(points):
        for axis,value in enumerate(p): put(vertices+i*6+axis*2,value,2)
    for i,(start,count,kind) in enumerate(ranges):
        for j in range(16): mem[descriptors+i*16+j] = 0xa5
        put(descriptors+i*16,kind,1);put(descriptors+i*16+10,count,2);put(descriptors+i*16+12,vertices+6*start)
    mesh=symbols['D_801407F0'];put(mesh,primary,2);put(mesh+8,len(ranges),1);put(mesh+12,descriptors)
    put(symbols['D_801409E8'],vertices);put(symbols['D_801527A4'],len(points),2)
    original=dict(mem)
    regs=[(seed*1664525+i*1013904223)&0xffffffff for i in range(32)]
    regs[0]=0;regs[29]=stack;regs[31]=stop
    preserved={i:regs[i] for i in [*range(16,24),28,30]}
    pc=base;pending=None;steps=0;lo=hi=0
    while pc!=stop:
        assert steps<100000 and base<=pc<base+4*len(words),(hex(pc),steps)
        w=words[(pc-base)//4];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;imm=w&65535;si=imm if imm<32768 else imm-65536
        old=pending;pending=None;nextpc=pc+4;a=(regs[rs]+si)&0xffffffff
        if w==0: pass
        elif op==0:
            fn=w&63
            if fn==0:regs[rd]=regs[rt]<<(w>>6&31)
            elif fn==2:regs[rd]=regs[rt]>>(w>>6&31)
            elif fn==3:regs[rd]=signed(regs[rt])>>(w>>6&31)
            elif fn==33:regs[rd]=regs[rs]+regs[rt]
            elif fn==35:regs[rd]=regs[rs]-regs[rt]
            elif fn==37:regs[rd]=regs[rs]|regs[rt]
            elif fn==42:regs[rd]=int(signed(regs[rs])<signed(regs[rt]))
            elif fn==43:regs[rd]=int(regs[rs]<regs[rt])
            elif fn==25:
                value=regs[rs]*regs[rt];lo=value&0xffffffff;hi=value>>32
            elif fn==18:regs[rd]=lo
            elif fn==8:pending=regs[rs]
            else:raise AssertionError(hex(w))
        elif op==15:regs[rt]=imm<<16
        elif op==9:regs[rt]=a
        elif op==10:regs[rt]=int(signed(regs[rs])<si)
        elif op==11:regs[rt]=int(regs[rs]<(si&0xffffffff))
        elif op==12:regs[rt]=regs[rs]&imm
        elif op in (4,5,6,7,20,21,22,23):
            cond={4:regs[rs]==regs[rt],5:regs[rs]!=regs[rt],6:signed(regs[rs])<=0,7:signed(regs[rs])>0}[op-16 if op>=20 else op]
            if cond:pending=pc+4+4*si
            elif op>=20:nextpc+=4
        elif op==1:
            assert rt in (0,1,2,3)
            cond=(signed(regs[rs])<0) if rt in (0,2) else (signed(regs[rs])>=0)
            if cond:pending=pc+4+4*si
            elif rt>=2:nextpc+=4
        elif op in (32,33,35,36,37):
            n={32:1,33:2,35:4,36:1,37:2}[op];v=get(a,n)
            if op in (32,33) and v&(1<<(8*n-1)):v-=1<<(8*n)
            regs[rt]=v
        elif op in (40,41,43):
            n={40:1,41:2,43:4}[op]
            assert stack-128<=a<=stack+32-n or any(symbols[name]<=a<=symbols[name]+6-n for name in NAMES[:2]),hex(a)
            put(a,regs[rt],n)
            if not stack-128<=a<stack+32:writes.append((a,n,regs[rt]&((1<<(n*8))-1)))
        else:raise AssertionError(hex(w))
        regs=[v&0xffffffff for v in regs];regs[0]=0;pc=old if old is not None else nextpc;steps+=1
    assert regs[29]==stack and all(regs[i]==v for i,v in preserved.items())
    for a,v in original.items():
        if not stack-128<=a<stack+32 and not any(symbols[n]<=a<symbols[n]+6 for n in NAMES[:2]):assert mem[a]==v
    result=[]
    for name in NAMES[:2]:
        result.append([int.from_bytes(bytes(mem[symbols[name]+2*i+j] for j in range(2)),'big',signed=True) for i in range(3)])
    return result,writes

def oracle(points, primary, ranges):
    low=[32767]*3;high=[-32767]*3
    for i,p in enumerate(points):
        if i<primary or any(start<=i<start+count and kind==1 for start,count,kind in ranges):
            low=[min(a,b) for a,b in zip(low,p)];high=[max(a,b) for a,b in zip(high,p)]
    return [low,high]

def run():
    symbols=score.image_symbols();symbols.update({n:score.address_named(n) for n in NAMES})
    target=score.targets()[NAME];assert len(target)==102
    source=HERE/(NAME+'.c')
    with tempfile.TemporaryDirectory() as td:
        td=Path(td)
        subprocess.run([str(score.IDO.resolve()/'cc'),'-c',*FLAGS,str(source),'-o',str(td/'candidate.o')],check=True)
        (td/'link.ld').write_text('OUTPUT_ARCH(mips)\nSECTIONS { . = '+hex(symbols[NAME])+'; .text : SUBALIGN(4) { *(.text) } }\n'+'\n'.join(n+' = '+hex(symbols[n])+';' for n in NAMES))
        subprocess.run(['mips-linux-gnu-ld','-T',str(td/'link.ld'),'-o',str(td/'candidate.elf'),str(td/'candidate.o')],check=True,capture_output=True)
        subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(td/'candidate.elf'),str(td/'candidate.bin')],check=True)
        symbol_line=next(line.split() for line in subprocess.check_output(['mips-linux-gnu-nm','-S',str(td/'candidate.o')],text=True).splitlines() if line.split()[-1]==NAME)
        function_bytes=int(symbol_line[1],16)
        assert function_bytes==420
        data=(td/'candidate.bin').read_bytes();assert len(data)==432
        candidate=list(struct.unpack('>%dI'%(len(data)//4),data))
        differences=[i*4 for i,w in enumerate(target) if i>=len(candidate) or candidate[i]!=w]
        result=score.compare(td/'candidate.o',NAME,show=0)
        assert not result.accepted(False) and not result.errors and not result.unresolved and not result.unverified
        assert len(differences)==result.differing==100
        rng=random.Random(0xb9740);cases=0
        for test in range(1000):
            count=test if test<34 else rng.randrange(34)
            primary=[0,1,count,max(0,count-1),65535][test%5]
            points=[[rng.choice([-32768,-32767,-1,0,1,32767]) if test<100 else rng.randrange(-32768,32768) for _ in range(3)] for _ in range(count)]
            ranges=[]
            for _ in range([0,1,2,8,255][test%5] if test<50 else rng.randrange(9)):
                start=rng.randrange(count+1);length=rng.randrange(count-start+1)
                ranges.append((start,length,rng.choice([0,1,1,2,255])))
            args=(symbols,points,primary,ranges,test)
            a=execute(target,*args);b=execute(candidate,*args)
            assert a==b,(test,a,b)
            assert a[0]==oracle(points,primary,ranges),(test,a)
            cases+=1
        evidence={'verdict':'NONMATCH','claims':[],'target_words':len(target),'target_bytes':4*len(target),'candidate_function_bytes':function_bytes,'candidate_text_bytes':len(data),'candidate_words_including_padding':len(candidate),'differing_words':len(differences),'nonzero_excess_words':result.extra_words,'difference_offsets':differences,'target_words_hex':['%08x'%w for w in target],'resolved_candidate_words_hex':['%08x'%w for w in candidate],'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'compiler_sha256':{n:hashlib.sha256((score.IDO/n).read_bytes()).hexdigest() for n in ['cc','cfe','uopt','ugen','as1']},'target_sha256':hashlib.sha256(struct.pack('>102I',*target)).hexdigest(),'differential_cases':cases,'flags':FLAGS,'unresolved':[],'unverified':[]}
        (HERE/'verification.json').write_text(json.dumps(evidence,indent=2)+'\n')
        print('PASS:',cases,'native / linked C / oracle cases; NONMATCH',len(differences),'of',len(target),'words; excess',result.extra_words)
if __name__=='__main__':run()
