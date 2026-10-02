#!/usr/bin/env python3
"""Differentially execute all native instructions and the standalone host C seed.
No ROM required. This verifies bounded input behavior, not compiler identity.
"""
import argparse, ctypes, hashlib, json, os, random, struct, subprocess, sys, tempfile
from pathlib import Path

FIELDS = [(250,2),(252,2),(254,2),(788,4),(792,4),(820,4),(824,2),(826,2),(828,2),(830,2)]
A, B = 0x100000, 0x200000
COUNTER, MODE, CONSTANT = 0x801161D0, 0x80151CEE, 0x801543CC
START = 0x800EC270

def signed(x, bits=32):
    return x - (1 << bits) if x & (1 << (bits-1)) else x

def execute(words, vehicle, state, counter, mode, constant):
    regions = [(A, bytearray(vehicle)), (B, bytearray(state)),
               (COUNTER, bytearray(struct.pack('>I', counter))),
               (MODE, bytearray(struct.pack('>H', mode))),
               (CONSTANT, bytearray(struct.pack('>I', constant)))]
    def memory(address, width, value=None):
        for base, data in regions:
            offset = address-base
            if 0 <= offset and offset+width <= len(data):
                if value is None: return int.from_bytes(data[offset:offset+width], 'big')
                data[offset:offset+width] = (value & ((1 << (width*8))-1)).to_bytes(width,'big')
                return
        raise AssertionError('out-of-bounds native access: %08x' % address)
    registers = [0]*32; floats = [0]*32
    registers[4], registers[5], registers[31] = A, B, 0xFFFFFFFC
    pc, pending, steps = START, None, 0
    while pc != 0xFFFFFFFC:
        assert START <= pc < START+4*len(words) and steps < 80
        word = words[(pc-START)//4]; op = word >> 26
        rs, rt, rd = (word>>21)&31, (word>>16)&31, (word>>11)&31
        imm = word&65535; simm = signed(imm,16); next_pc=pc+4
        old_pending, pending = pending, None
        address = (registers[rs]+simm)&0xFFFFFFFF
        if word == 0: pass
        elif op == 0 and word&63 == 8: pending = registers[rs]
        elif op == 15: registers[rt] = imm<<16
        elif op == 9: registers[rt] = address
        elif op == 10: registers[rt] = int(signed(registers[rs]) < simm)
        elif op in (4,5,21):
            take = (registers[rs] == registers[rt]) if op==4 else (registers[rs] != registers[rt])
            if take: pending = pc+4+4*simm
            elif op==21: next_pc += 4
        elif op in (32,33,35):
            width={32:1,33:2,35:4}[op]; value=memory(address,width)
            registers[rt] = signed(value,width*8)&0xFFFFFFFF
        elif op in (40,41,43): memory(address,{40:1,41:2,43:4}[op],registers[rt])
        elif op==49: floats[rt]=memory(address,4)
        elif op==57: memory(address,4,floats[rt])
        elif op==17 and rs==4: floats[rd]=registers[rt]
        else: raise AssertionError('unsupported native word %08x at %08x' % (word,pc))
        registers[0]=0; pc = old_pending if old_pending is not None else next_pc; steps+=1
    return bytes(regions[0][1]), bytes(regions[1][1]), memory(COUNTER,4)

HARNESS = r'''
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include "candidate.c"
float D_801543CC;
s32 D_801161D0;
s16 D_80151CEE;
typedef char check_vehicle_byte[(offsetof(A0, f7DC)==2012)?1:-1];
typedef char check_state_float[(offsetof(A1, f334)==820)?1:-1];
typedef char check_state_end[(offsetof(A1, f33E)==830)?1:-1];
uint32_t run(unsigned char *a, unsigned char *b, uint32_t count, uint16_t mode, uint32_t bits) {
    A0 vehicle; A1 state;
    memcpy(&vehicle,a,sizeof(vehicle)); memcpy(&state,b,sizeof(state));
    memcpy(&D_801161D0,&count,4); memcpy(&D_80151CEE,&mode,2); memcpy(&D_801543CC,&bits,4);
    func_800EC270(&vehicle,&state);
    memcpy(a,&vehicle,sizeof(vehicle)); memcpy(b,&state,sizeof(state));
    memcpy(&count,&D_801161D0,4); return count;
}
#ifdef HOST_MAIN
static uint32_t seed=0x12345678;
static uint32_t random_word(void) { seed^=seed<<13;seed^=seed>>17;seed^=seed<<5;return seed; }
int main(void) {
    unsigned n; unsigned char a[2048], b[832], ea[2048], eb[832];
    for(n=0;n<500000;n++) {
        uint32_t c=random_word(), bits=random_word(), out, want=c;
        uint16_t mode=(uint16_t)random_word(), h; uint32_t z=0; unsigned i;
        for(i=0;i<sizeof(a);i++) a[i]=(unsigned char)(i+n);
        for(i=0;i<sizeof(b);i++) b[i]=(unsigned char)(i+n);
        a[1996]=(unsigned char)n; if(n%3==0) mode=0;
        memcpy(ea,a,sizeof(a)); memcpy(eb,b,sizeof(b));
        h=0;memcpy(eb+824,&h,2);memcpy(eb+788,&z,4);memcpy(eb+792,&z,4);
        memcpy(eb+820,&bits,4);h=65535;memcpy(eb+826,&h,2);
        if(a[1996]==1) {
            h=(uint16_t)c;memcpy(eb+828,&h,2);memcpy(eb+830,&h,2);
            want=c+1; if((int32_t)want>=4)want=0;
        } else { h=0;memcpy(eb+828,&h,2); }
        if(mode==0)ea[2012]=1;
        h=65535;memcpy(eb+250,&h,2);h=0;memcpy(eb+252,&h,2);memcpy(eb+254,&h,2);
        out=run(a,b,c,mode,bits);
        assert(out==want && !memcmp(a,ea,sizeof(a)) && !memcmp(b,eb,sizeof(b)));
    }
    return 0;
}
#endif
'''

def host_endian(data):
    data=bytearray(data)
    if sys.byteorder=='little':
        for offset,width in FIELDS: data[offset:offset+width]=data[offset:offset+width][::-1]
    return data

def main():
    ap=argparse.ArgumentParser(); ap.add_argument('--repo',type=Path,default=Path(__file__).resolve().parents[3]);ap.add_argument('--output',type=Path,required=True);args=ap.parse_args()
    root=args.repo.resolve(); source=Path(__file__).resolve().with_name('candidate.c')
    sys.path.insert(0,str(root/'tools/cloud'));import score
    words=score.targets()['func_800EC270']; assert len(words)==34
    cases=0; rng=random.Random(0xEC270)
    with tempfile.TemporaryDirectory() as td:
        d=Path(td);(d/'candidate.c').write_bytes(source.read_bytes());(d/'host.c').write_text(HARNESS)
        # Harness uses C99 fixed-width names, while the candidate separately stays C89.
        subprocess.run(['cc','-std=c89','-Wall','-Wextra','-Werror','-pedantic','-fsyntax-only',str(source)],check=True)
        subprocess.run(['cc','-std=c99','-O2','-shared','-fPIC',str(d/'host.c'),'-o',str(d/'host.so')],check=True)
        subprocess.run(['cc','-std=c99','-O1','-g','-fsanitize=address,undefined','-fno-sanitize-recover=all','-DHOST_MAIN',str(d/'host.c'),'-o',str(d/'host')],check=True)
        subprocess.run([str(d/'host')],check=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0'))
        lib=ctypes.CDLL(str(d/'host.so'));lib.run.argtypes=[ctypes.c_void_p,ctypes.c_void_p,ctypes.c_uint32,ctypes.c_uint16,ctypes.c_uint32];lib.run.restype=ctypes.c_uint32
        def check(flag,mode,count,bits):
            nonlocal cases
            a=bytearray((i+cases)&255 for i in range(2048));b=bytearray((i+cases*3)&255 for i in range(832));a[1996]=flag
            want=execute(words,a,b,count,mode,bits)
            ca=(ctypes.c_ubyte*2048).from_buffer_copy(a);cb=(ctypes.c_ubyte*832).from_buffer_copy(host_endian(b))
            out=lib.run(ca,cb,count,mode,bits)
            assert (bytes(ca),bytes(host_endian(cb)),out)==want,(flag,mode,count,bits)
            cases+=1
        counts=[0,1,2,3,4,65535,65536,0x7ffffffe,0x7fffffff,0x80000000,0xfffffffe,0xffffffff]
        floats=[0,0x80000000,0x3f800000,0x7f800000,0xff800000,0x7fc01234,0x7f800001,1,0x7fffff,0x7f7fffff]
        for flag in range(256):
            for count in counts:
                for mode in [0,1,0x7fff,0x8000,0xffff]:
                    check(flag,mode,count,floats[cases%len(floats)])
        for mode in range(65536): check(1,mode,counts[mode%len(counts)],floats[mode%len(floats)])
        for _ in range(10000): check(rng.randrange(256),rng.randrange(65536),rng.getrandbits(32),rng.getrandbits(32))
    report={'verdict':'PASS','native_differential_cases':cases,'asan_ubsan_host_cases':500000,'native_instructions':len(words),'candidate_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'scope':'Native interpreter vs host C, bounded disjoint memory tests; not native execution or compiler identity','limits':['No concurrent mutation or overlapping vehicle/state/global regions tested','IEEE float words are copied; FCSR effects not modeled','Signed uint32-to-int32 conversion assumes two-complement implementation, as IDO/GCC use']}
    args.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
if __name__=='__main__':main()
