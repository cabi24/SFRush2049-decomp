"""Complete raw-word replay and bounded native/candidate/host differential proof.

Constants are injected test fixtures, not claims about absent ROM data.
IEEE single operations and modff are modeled; N64 FCSR is not emulated.
"""
import ctypes, dataclasses, hashlib, json, math, os, random, struct, subprocess, sys, tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]; HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'tools/cloud'))
import score
NAME='func_800A557C'; START=0x800a557c; STACK=0x80700000; STOP=0x81234560
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
GLOBALS=['D_%08X'%a for a in range(0x80123b98,0x80123bc8,4)]
def bits(x):
    try:return int.from_bytes(struct.pack('>f',x),'big')
    except OverflowError:return 0xff800000 if x<0 else 0x7f800000
def val(x):return struct.unpack('>f',struct.pack('>I',x))[0]
def signed(x,n=32):return x-(1<<n) if x&(1<<(n-1)) else x
def divide(a,b):
    if b:return a/b
    if a==0 or math.isnan(a):return math.nan
    return math.copysign(math.inf,math.copysign(1,a)*math.copysign(1,b))
def execute(words,inputs,seed=1):
    mem={};calls=[];branches=set();writes=[]
    for i,w in enumerate(inputs[1:]):mem[0x80123b98+4*i]=w
    r=[(seed*69069+i*1234567)&0xffffffff for i in range(32)]
    f=[0x7fc00001]*32;r[0]=0;r[29]=STACK;r[31]=STOP;f[12]=inputs[0]
    preserved={i:r[i] for i in [*range(16,24),28,30]};pc=START;pending=None;condition=False;steps=0
    def read(a):
        assert a%4==0 and a in mem, 'uninitialized/out-of-bounds read %x'%a
        return mem[a]
    def write(a,v):
        assert a%4==0 and STACK-128<=a<STACK+16, 'unexpected write %x'%a
        mem[a]=v;writes.append(a)
    while pc!=STOP:
        assert START<=pc<START+len(words)*4 and steps<200,(hex(pc),steps)
        w=words[(pc-START)//4];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;fd=w>>6&31;imm=w&65535;si=signed(imm,16)
        old,pending=pending,None;nextpc=pc+4;addr=(r[rs]+si)&0xffffffff
        if w==0:pass
        elif op==0:
            fn=w&63
            if fn==8:pending=r[rs]
            elif fn==37:r[rd]=r[rs]|r[rt]
            else:raise AssertionError('unsupported %08x'%w)
        elif op==15:r[rt]=imm<<16
        elif op==9:r[rt]=addr
        elif op==12:r[rt]=r[rs]&imm
        elif op==3:r[31]=pc+8;pending=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
        elif op in (4,5,20,21):
            take=(r[rs]==r[rt])==(op in (4,20));branches.add((pc-START,take))
            if take:pending=pc+4+si*4
            elif op in (20,21):nextpc+=4
        elif op==35:r[rt]=read(addr)
        elif op==43:write(addr,r[rt])
        elif op==49:f[rt]=read(addr)
        elif op==57:write(addr,f[rt])
        elif op==17:
            if rs==4:f[rd]=r[rt]
            elif rs==0:r[rt]=f[rd]
            elif rs==8:
                take=condition if rt&1 else not condition;branches.add((pc-START,take))
                if take:pending=pc+4+si*4
                elif rt&2:nextpc+=4
            elif rs==16:
                fn=w&63;a,b=val(f[rd]),val(f[rt])
                if fn==0:f[fd]=bits(a+b)
                elif fn==1:f[fd]=bits(a-b)
                elif fn==2:f[fd]=bits(a*b)
                elif fn==3:f[fd]=bits(divide(a,b))
                elif fn==5:f[fd]=f[rd]&0x7fffffff
                elif fn==6:f[fd]=f[rd]
                elif fn==7:f[fd]=f[rd]^0x80000000
                elif fn==13:
                    assert math.isfinite(a) and -2147483648<=a<2147483648, 'outside defined C conversion domain'
                    f[fd]=math.trunc(a)&0xffffffff
                elif fn==60:condition=a<b
                elif fn==62:condition=a<=b
                else:raise AssertionError('unsupported FP %08x'%w)
            else:raise AssertionError('unsupported COP1 %08x'%w)
        else:raise AssertionError('unsupported %08x'%w)
        r=[x&0xffffffff for x in r];r[0]=0
        if old==0x80002a64:
            x=val(f[12]);frac,whole=math.modf(x);calls.append((f[12],bits(whole),bits(frac)))
            write(r[5],bits(whole));ret=r[31]
            for i in [1,2,3,*range(4,16),24,25]:r[i]=(seed*1664525+i*1013904223)&0xffffffff
            for i in range(20):f[i]=0x7fc0abcd
            f[0]=bits(frac);pc=ret
        else:pc=old if old is not None else nextpc
        steps+=1
    assert r[29]==STACK and all(r[i]==v for i,v in preserved.items()),'ABI violation'
    return f[0],calls,branches

def compile_link(d):
    obj=d/'candidate.o';subprocess.run([str(score.ido('cc')),*FLAGS.split(),'-c',str(HERE/'candidate.c'),'-o',str(obj)],check=True)
    script='SECTIONS { . = 0x800A557C; .text : { *(.text) } /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) } }\nmodff = 0x80002A64;\n'+''.join(n+' = 0x'+n[2:]+';\n' for n in GLOBALS)
    (d/'link.ld').write_text(script);subprocess.run(['mips-linux-gnu-ld','-T',str(d/'link.ld'),'-o',str(d/'linked.elf'),str(obj)],check=True,capture_output=True)
    subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(d/'linked.elf'),str(d/'text.bin')],check=True)
    raw=(d/'text.bin').read_bytes();words=list(struct.unpack('>%dI'%(len(raw)//4),raw));data,secs=score._elf(obj)
    sym=[s for i,v in enumerate(secs) if v['type']==2 for s in score._symbol_table(data,secs,i) if s['name']==NAME][0]
    return words,sym['size'],dataclasses.asdict(score.compare(obj,NAME,show=0))

HARNESS='''#include <stdint.h>
#include <string.h>
#include <math.h>
#include "candidate.c"
'''+''.join('float '+n+';\n' for n in GLOBALS)+'''
uint32_t run(const uint32_t *v) { float x;uint32_t out;float result;
memcpy(&x,v,4);
'''+''.join('memcpy(&'+n+',v+'+str(i+1)+',4);\n' for i,n in enumerate(GLOBALS))+'''
result=func_800A557C(x);memcpy(&out,&result,4);return out;}
#ifdef HOST_MAIN
int main(void) {uint32_t x[13];float values[13];unsigned i,j;uint32_t rng=557;
float c[12]={100.0f,0.63661975f,1.5703125f,0.00048382679f,0.0001f,-0.1f,0.02f,0.01f,0.003f,-0.02f,0.05f,-0.2f};
for(i=0;i<100000;i++){rng=rng*1664525u+1013904223u;values[0]=(int32_t)rng/20000000.0f;for(j=0;j<12;j++)values[j+1]=c[j];memcpy(x,values,sizeof(x));run(x);}return 0;}
#endif
'''
def main(output=None):
    native=score.targets()[NAME];assert len(native)==114
    with tempfile.TemporaryDirectory() as tmp:
        d=Path(tmp);cand,size,canonical=compile_link(d)
        diff=[{'offset':i*4,'target':'%08x'%w,'candidate':'%08x'%cand[i] if i<len(cand) else None} for i,w in enumerate(native) if i>=len(cand) or cand[i]!=w]
        assert len(diff)==canonical['differing'] and not canonical['unresolved'] and not canonical['unverified'] and not canonical['errors']
        (d/'candidate.c').write_text((HERE/'candidate.c').read_text());(d/'host.c').write_text(HARNESS)
        common=['cc','-std=c99','-O1','-ffp-contract=off','-fno-fast-math','-Wall','-Wextra','-Werror']
        subprocess.run(common+['-shared','-fPIC',str(d/'host.c'),'-lm','-o',str(d/'host.so')],check=True)
        lib=ctypes.CDLL(str(d/'host.so'));lib.run.argtypes=[ctypes.POINTER(ctypes.c_uint32)];lib.run.restype=ctypes.c_uint32
        rng=random.Random(557);cases=0;paths=set();call_counts=set()
        constants=[100.0,0.63661975,1.5703125,0.00048382679,0.0001,-0.1,0.02,0.01,0.003,-0.02,0.05,-0.2]
        def check(x,c):
            nonlocal cases
            inputs=[bits(x)]+[bits(v) for v in c];a,calls,branches=execute(native,inputs,cases+1);b,bcalls,_=execute(cand,inputs,cases+1);h=lib.run((ctypes.c_uint32*13)(*inputs))
            same=lambda u,v:u==v or (math.isnan(val(u)) and math.isnan(val(v)))
            assert same(a,b) and same(a,h) and calls==bcalls,(cases,x,c,hex(a),hex(b),hex(h))
            cases+=1;paths.update(branches);call_counts.add(len(calls))
        for x in [0.0,-0.0,1e-8,-1e-8,0.5,-0.5,1,-1,2,-2,100,-100,101,-101,math.inf,-math.inf]:check(x,constants)
        for i in range(10000):
            c=constants[:] if i%2 else [100.0,rng.uniform(-2,2),rng.uniform(-2,2),rng.uniform(-.01,.01),rng.choice([0,0.0001,1,1000])]+[rng.uniform(-.2,.2) for _ in range(7)]
            check(rng.uniform(-110,110),c)
        # Exact rounding ties and neighbors, both signs; unit multiplier makes ties explicit.
        for n in range(-20,21):
            c=constants[:];c[1]=1.0
            for x in [n+0.5,n+0.5-0.00001,n+0.5+0.00001]:check(x,c)
        subprocess.run(common+['-g','-fsanitize=address,undefined','-fno-sanitize-recover=all','-DHOST_MAIN',str(d/'host.c'),'-lm','-o',str(d/'host')],check=True)
        subprocess.run([str(d/'host')],check=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0'))
        result={'status':'NONMATCH','function':NAME,'claims':[],'source_sha256':hashlib.sha256((HERE/'candidate.c').read_bytes()).hexdigest(),'target_sha256':hashlib.sha256(struct.pack('>114I',*native)).hexdigest(),'compiler_sha256':hashlib.sha256(Path(score.ido('cc')).read_bytes()).hexdigest(),'flags':FLAGS,'target_bytes':456,'candidate_function_bytes':size,'candidate_text_bytes':len(cand)*4,'canonical':canonical,'differences':diff,'differential_cases':cases,'sanitizer_cases':100000,'native_branch_outcomes':sorted(paths),'call_counts':sorted(call_counts),'limitations':['Test coefficients are injected fixtures, not recovered ROM values.','Only paths with defined rounded-to-s32 conversion are tested; NaN input and overflowing range reduction excluded.','FCSR flags, traps, target NaN payloads/subnormals not modeled; NaNs compared by class.','No ROM gate or accepted coverage claim.']}
        print(json.dumps(result,indent=2))
        if output is not None:Path(output).write_text(json.dumps(result,indent=2)+'\n')
if __name__=='__main__':main(sys.argv[1] if len(sys.argv)>1 else None)
