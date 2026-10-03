#!/usr/bin/env python3
"""Bounded protected MIPS-vs-host-C differential test; not N64 FCSR emulation."""
import ctypes, hashlib, json, math, os, random, struct, subprocess, sys, tempfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
HERE = Path(__file__).resolve().parent
START, STATE, CONFIG, CONSTANT = 0x800E1AA0, 0x100000, 0x200000, 0x801243C0
FIELDS = [64,72,80,320,980,1008,1452,1824]

def bits(x):
    try: return int.from_bytes(struct.pack('>f',x),'big')
    except OverflowError: return 0xff800000 if x < 0 else 0x7f800000

def value(x): return struct.unpack('>f',struct.pack('>I',x))[0]
def signed(x,n=32): return x-(1<<n) if x&(1<<(n-1)) else x

def execute(words, inputs):
    data=bytearray([0xa5]*2008); config=bytearray([0x5a]*40)
    for off,v in zip(FIELDS,inputs): data[off:off+4]=v.to_bytes(4,'big')
    data[4:8]=CONFIG.to_bytes(4,'big');config[36:40]=inputs[8].to_bytes(4,'big')
    for off,v in [(1548,inputs[10]),(1552,inputs[11]),(2004,inputs[13])]:data[off:off+4]=v.to_bytes(4,'big')
    data[1994:1996]=(inputs[12]&65535).to_bytes(2,'big')
    regions=[(STATE,data),(CONFIG,config),(CONSTANT,bytearray(inputs[9].to_bytes(4,'big')))]
    original=bytes(data);original_config=bytes(config);writes=[]
    def mem(addr,n,v=None):
        assert addr % n == 0, 'unaligned access'
        for base,buf in regions:
            off=addr-base
            if 0<=off and off+n<=len(buf):
                if v is None:return int.from_bytes(buf[off:off+n],'big')
                assert addr==STATE+320 and n==4, 'unexpected write'
                writes.append(addr);buf[off:off+n]=v.to_bytes(n,'big');return
        raise AssertionError('out-of-bounds access %x'%addr)
    r=[0]*32;f=[0]*32;r[4]=STATE;r[31]=0xfffffffc
    pc=START;pending=None;condition=False;steps=0
    while pc!=0xfffffffc:
        assert START<=pc<START+4*len(words) and steps<200
        w=words[(pc-START)//4];op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31;fd=(w>>6)&31;imm=w&65535;si=signed(imm,16)
        old,pending=pending,None;next_pc=pc+4;addr=(r[rs]+si)&0xffffffff
        if w==0:pass
        elif op==0 and w&63==8:pending=r[rs]
        elif op==15:r[rt]=imm<<16
        elif op==9:r[rt]=addr
        elif op==12:r[rt]=r[rs]&imm
        elif op in (4,5,20,21):
            take=(r[rs]==r[rt]) if op in (4,20) else (r[rs]!=r[rt])
            if take:pending=pc+4+4*si
            elif op in (20,21):next_pc+=4
        elif op==35:r[rt]=mem(addr,4)
        elif op==33:r[rt]=signed(mem(addr,2),16)&0xffffffff
        elif op==49:f[rt]=mem(addr,4)
        elif op==57:mem(addr,4,f[rt])
        elif op==17:
            if rs==4:f[rd]=r[rt]
            elif rs==8:
                take=condition if rt&1 else not condition
                if take:pending=pc+4+4*si
                elif rt&2:next_pc+=4
            elif rs==16:
                fn=w&63;a,b=value(f[rd]),value(f[rt])
                if fn==0:f[fd]=bits(a+b)
                elif fn==1:f[fd]=bits(a-b)
                elif fn==2:f[fd]=bits(a*b)
                elif fn==6:f[fd]=f[rd]
                elif fn==60:condition=a<b
                else:raise AssertionError('unsupported FP word %08x'%w)
            else:raise AssertionError('unsupported cop1 word %08x'%w)
        else:raise AssertionError('unsupported word %08x'%w)
        r[0]=0;pc=old if old is not None else next_pc;steps+=1
    assert data[:320]==original[:320] and data[324:]==original[324:]
    assert bytes(config)==original_config
    return mem(STATE+320,4), len(writes)

HARNESS=r'''
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "candidate.c"
float D_801243C0;
/* Host pointer width differs from O32; field values are populated by name.
   Full native offsets are separately asserted in the IDO layout unit. */
static float cv(uint32_t x){float f; memcpy(&f,&x,4);return f;}
uint32_t run(const uint32_t *x){
 ForceState s,before;ForceConfig c;uint32_t out;
 memset(&s,0xa5,sizeof(s));memset(&c,0x5a,sizeof(c));
 s.config=&c;s.direction=cv(x[0]);s.height=cv(x[1]);s.velocity=cv(x[2]);s.force=cv(x[3]);
 s.blend=cv(x[4]);s.speed=cv(x[5]);s.bias=cv(x[6]);s.magnitude=cv(x[7]);c.scale=cv(x[8]);D_801243C0=cv(x[9]);
 memcpy(&s.mode_a,x+10,4);memcpy(&s.mode_b,x+11,4);s.direct=(short)x[12];s.flags=x[13];
 before=s;func_800E1AA0(&s);memcpy(&out,&s.force,4);s.force=before.force;
 assert(memcmp(&s,&before,sizeof(s))==0);assert(c.scale==cv(x[8]) || (isnan(c.scale)&&isnan(cv(x[8]))));return out;
}
#ifdef HOST_MAIN
static uint32_t rng=0x91fea03U;
static uint32_t next(void){rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;return rng;}
int main(void){uint32_t x[14];int n,i;for(n=0;n<200000;n++){for(i=0;i<14;i++)x[i]=next();x[10]=(n&1)?8:0;x[11]=(n&2)?8:0;x[13]=(n&4)?0:16;run(x);}return 0;}
#endif
'''

def main():
    sys.path.insert(0,str(ROOT/'tools/cloud'));import score
    words=score.targets()['func_800E1AA0']
    with tempfile.TemporaryDirectory() as tmp:
        d=Path(tmp);(d/'host.c').write_text(HARNESS);(d/'candidate.c').write_text((HERE/'candidate.c').read_text())
        common=['gcc','-std=c99','-O1','-ffp-contract=off','-fno-fast-math']
        subprocess.run(common+['-shared','-fPIC',str(d/'host.c'),'-o',str(d/'host.so')],check=True)
        lib=ctypes.CDLL(str(d/'host.so'));lib.run.argtypes=[ctypes.POINTER(ctypes.c_uint32)];lib.run.restype=ctypes.c_uint32
        edges=[0,0x80000000,bits(-1),bits(1),bits(0.5),bits(100),bits(120),0x7f800000,0xff800000,0x7fc00001,1,0x80000001,0x7f7fffff,0xff7fffff]
        rng=random.Random(0xe1aa0);cases=0;paths=set()
        def check(x):
            nonlocal cases
            want,writes=execute(words,x);got=lib.run((ctypes.c_uint32*14)(*x))
            assert got==want or (math.isnan(value(got)) and math.isnan(value(want))), (x,hex(want),hex(got))
            paths.add(writes);cases+=1
        for mode in range(16):
            for field in range(10):
                for e in edges:
                    x=[bits(1),bits(0),bits(1),bits(10),bits(0.25),bits(99),bits(0.1),bits(1),bits(0.5),bits(12000),8 if mode&1 else 0,8 if mode&2 else 0,1 if mode&4 else 0,16 if mode&8 else 0]
                    x[field]=e;check(x)
        for n in range(20000):
            x=[rng.getrandbits(32) for _ in range(10)]+[8 if n&1 else rng.getrandbits(32),8 if n&2 else rng.getrandbits(32),0 if n&4 else rng.randrange(65536),0 if n&8 else rng.getrandbits(32)]
            check(x)
        subprocess.run(common+['-g','-fsanitize=address,undefined','-fno-sanitize-recover=all','-DHOST_MAIN',str(d/'host.c'),'-o',str(d/'host')],check=True)
        env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0');subprocess.run([str(d/'host')],env=env,check=True)
    result={'status':'PASS','differential_cases':cases,'write_counts':sorted(paths),'sanitizer_cases':200000,'source_sha256':hashlib.sha256((HERE/'candidate.c').read_bytes()).hexdigest(),'limits':'Host IEEE single rounding; NaNs compared by class. Not N64 FCSR/exception/subnormal/NaN payload proof.'}
    print(json.dumps(result,indent=2))
    if len(sys.argv)>1:Path(sys.argv[1]).write_text(json.dumps(result,indent=2)+'\n')
if __name__=='__main__':main()
