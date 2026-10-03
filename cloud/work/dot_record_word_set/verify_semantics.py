#!/usr/bin/env python3
"""Bounded native-word vs host-C record setter audit; no ROM coverage claim."""
import ctypes
import hashlib
import json
import os
from pathlib import Path
import random
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[3]
HERE = Path(__file__).resolve().parent
SOURCE = ROOT / 'cloud/matches/func_800A7830.c'
BASE = 0x8012E700
STACK = 0x20000000


def execute(words, index_word, value, unused):
    regs = [0] * 32
    regs[4:7] = [index_word & 0xffffffff, value, unused]
    regs[29], regs[31] = STACK, 0xfffffffc
    writes = []
    pc, pending = 0, None
    steps = 0
    while pc != 0xfffffffc:
        assert 0 <= pc < len(words) * 4 and steps < 20
        word = words[pc // 4]
        op, rs, rt = word >> 26, (word >> 21) & 31, (word >> 16) & 31
        rd, shift, imm = (word >> 11) & 31, (word >> 6) & 31, word & 0xffff
        signed_imm = imm - 65536 if imm & 32768 else imm
        next_pc, old = pc + 4, pending
        pending = None
        if op == 0:
            fn = word & 63
            if fn == 0: regs[rd] = (regs[rt] << shift) & 0xffffffff
            elif fn == 3:
                signed = regs[rt] - 2**32 if regs[rt] & 2**31 else regs[rt]
                regs[rd] = (signed >> shift) & 0xffffffff
            elif fn == 33: regs[rd] = (regs[rs] + regs[rt]) & 0xffffffff
            elif fn == 8: pending = regs[rs]
            else: raise AssertionError('unsupported native operation')
        elif op == 15: regs[rt] = imm << 16
        elif op == 43:
            address = (regs[rs] + signed_imm) & 0xffffffff
            assert address % 4 == 0
            writes.append((address, regs[rt]))
        else: raise AssertionError('unsupported native opcode')
        regs[0] = 0
        pc = old if old is not None else next_pc
        steps += 1
    assert steps == len(words)
    return writes


HARNESS = r'''
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "candidate.c"
RecordSlot D_8012E700[32768];
typedef char layout_size[(sizeof(RecordSlot)==68)?1:-1];
typedef char layout_offset[(offsetof(RecordSlot,value)==52)?1:-1];
void reset(void) { memset(D_8012E700, 0xa5, sizeof(D_8012E700)); }
int run(unsigned index, uint32_t value, uint32_t unused) {
    unsigned char before[68];
    assert(index < 32768);
    memcpy(before, &D_8012E700[index], 68);
    func_800A7830((short)index,value,unused);
    assert(D_8012E700[index].value == value);
    memcpy(before+52, &value, 4);
    assert(memcmp(before, &D_8012E700[index], 68)==0);
    return 1;
}
#ifdef HOST_MAIN
int main(void) {
    unsigned i,j;uint32_t values[]={0,1,0x80000000U,0xffffffffU,0x55555555U,0xaaaaaaaaU};
    reset();
    for(i=0;i<32768;i++)for(j=0;j<6;j++)run(i,values[j],~values[j]);
    return 0;
}
#endif
'''


def main(output=None):
    sys.path.insert(0, str(ROOT / 'tools/cloud'))
    import score
    words = score.targets()['func_800A7830']
    rng = random.Random(0xa7830)
    with tempfile.TemporaryDirectory() as tmp:
        d = Path(tmp)
        (d/'candidate.c').write_bytes(SOURCE.read_bytes())
        (d/'host.c').write_text(HARNESS)
        subprocess.run(['gcc','-std=c99','-O2','-shared','-fPIC',str(d/'host.c'),'-o',str(d/'host.so')],check=True)
        lib=ctypes.CDLL(str(d/'host.so'))
        lib.run.argtypes=[ctypes.c_uint,ctypes.c_uint32,ctypes.c_uint32]
        lib.reset()
        fixture=(ctypes.c_ubyte*(32768*68)).in_dll(lib,'D_8012E700')
        expected=bytearray([0xa5])*len(fixture)
        for index in range(32768):
            value,unused=rng.getrandbits(32),rng.getrandbits(32)
            index_word=(rng.getrandbits(16)<<16)|index
            writes=execute(words,index_word,value,unused)
            assert writes==[(STACK,index_word),(STACK+8,unused),(BASE+index*68+52,value)]
            assert lib.run(index,value,unused)==1
            off=index*68+52
            expected[off:off+4]=value.to_bytes(4,sys.byteorder)
        assert bytes(fixture)==expected, 'unexpected write outside selected values'
        # Signed conversion at the entry is checked without claiming negative
        # indices are in bounds for the original global table.
        for index in range(-32768,0):
            value,unused=rng.getrandbits(32),rng.getrandbits(32)
            index_word=(rng.getrandbits(16)<<16)|(index&65535)
            assert execute(words,index_word,value,unused)[-1]==((BASE+index*68+52)&0xffffffff,value)
        subprocess.run(['gcc','-std=c99','-O1','-g','-fsanitize=address,undefined','-fno-sanitize-recover=all','-DHOST_MAIN',str(d/'host.c'),'-o',str(d/'host')],check=True)
        subprocess.run([str(d/'host')],check=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0'))
    result={'status':'PASS','host_native_cases':32768,'negative_native_address_cases':32768,'sanitizer_cases':196608,'source_sha256':hashlib.sha256(SOURCE.read_bytes()).hexdigest(),'limits':'Synthetic32768-entry host fixture. Original table bounds and value semantic type unknown. Negative native addresses tested without executing out-of-bounds C. No full-ROM proof.'}
    print(json.dumps(result,indent=2))
    if output:Path(output).write_text(json.dumps(result,indent=2)+'\n')
    return result

if __name__=='__main__':main(sys.argv[1] if len(sys.argv)>1 else None)
