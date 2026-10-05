#!/usr/bin/env python3
"""Protected native setter vs host C on synthetic records; never emits retail bytes."""
import ctypes
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata

MASK = 0xffffffff
OWNER, OBJECT, RESOURCE, DATA, LOOKUP, STACK, STOP = 0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0xfffffff0
N = 160

def signed(x, n=32):
    return x - (1 << n) if x & (1 << (n - 1)) else x

def checksum(data):
    result = 0
    for v in data:
        op = v & 7
        if op == 0: result -= v
        elif op == 1: result |= v
        elif op == 2: result &= v
        elif op == 3: result ^= v
        elif op == 4: result *= v
        elif op == 5: result //= v
        else: result += v
        result &= MASK
    return result

class Native:
    def __init__(self):
        self.addresses = score.image_symbols()
        self.code = {}
        for name in ('audio_bus_mix', 'voice_stop_2'):
            a = self.addresses[name]
            self.code.update((a + i * 4, w) for i, w in enumerate(score.targets()[name]))
        self.image = owndata.ImageData.from_artifact(ROOT / 'asm/us/blob_data')
        assert self.image is not None

    def run(self, mode, selector, value, initial, resource_present=True):
        records = {OWNER: bytearray(4), OBJECT: bytearray(48), RESOURCE: bytearray(4),
                   DATA: bytearray(initial), STACK: bytearray(512), 0x8014978c: bytearray([mode & 255])}
        def mem(a, n, v=None):
            assert n == 1 or a % n == 0, 'unaligned native memory access'
            for base, data in records.items():
                off = a - base
                if 0 <= off and off + n <= len(data):
                    if v is None: return int.from_bytes(data[off:off+n], 'big')
                    data[off:off+n] = (v & ((1 << (8*n))-1)).to_bytes(n, 'big')
                    return
            assert v is None, 'write outside synthetic regions'
            data = self.image.read(a, n)
            assert data is not None, 'read outside synthetic or authenticated table regions'
            return int.from_bytes(data, 'big')
        mem(OWNER, 4, OBJECT); mem(OBJECT+8, 4, LOOKUP)
        mem(OBJECT+44, 4, RESOURCE if resource_present else 0); mem(RESOURCE, 4, DATA)
        r = [0] * 32
        r[4:7] = [OWNER, selector & MASK, value & MASK]
        r[29], r[31] = STACK+256, STOP
        pc = self.addresses['audio_bus_mix']; pending = None; calls = []; steps = 0
        while pc != STOP:
            if pc == self.addresses['format_string_parse']:
                a, n = r[4], r[5]
                assert DATA <= a <= DATA+N and 0 <= n <= N and a+n <= DATA+N
                r[2] = checksum(records[DATA][a-DATA:a-DATA+n])
                pc = r[31]
                continue
            if pc == self.addresses['slot_state_lookup']:
                assert r[4] == LOOKUP
                calls.append((r[5]-DATA, r[6], bytes(records[DATA][r[5]-DATA:r[5]-DATA+r[6]])))
                pc = r[31]
                continue
            assert pc in self.code and steps < 300, 'unsupported native control flow'
            w = self.code[pc]; op = w >> 26; rs = (w >> 21) & 31; rt = (w >> 16) & 31
            rd = (w >> 11) & 31; sh = (w >> 6) & 31; fn = w & 63; imm = w & 65535
            si = signed(imm, 16); addr = (r[rs]+si) & MASK
            old, pending = pending, None; next_pc = pc+4
            if w == 0: pass
            elif op == 0:
                if fn == 0: r[rd] = (r[rt] << sh) & MASK
                elif fn == 3: r[rd] = (signed(r[rt]) >> sh) & MASK
                elif fn == 8: pending = r[rs]
                elif fn == 33: r[rd] = (r[rs]+r[rt]) & MASK
                elif fn == 37: r[rd] = r[rs] | r[rt]
                else: raise AssertionError('unsupported native special operation')
            elif op in (2, 3):
                if op == 3: r[31] = pc+8
                pending = ((pc+4) & 0xf0000000) | ((w & 0x3ffffff) << 2)
            elif op == 1 and rt == 0:
                if signed(r[rs]) < 0: pending = pc+4+4*si
            elif op in (4, 5, 20, 21):
                take = (r[rs] == r[rt]) if op in (4, 20) else (r[rs] != r[rt])
                if take: pending = pc+4+4*si
                elif op in (20, 21): next_pc += 4
            elif op == 9: r[rt] = addr
            elif op == 10: r[rt] = int(signed(r[rs]) < si)
            elif op == 11: r[rt] = int(r[rs] < (si & MASK))
            elif op == 12: r[rt] = r[rs] & imm
            elif op == 15: r[rt] = imm << 16
            elif op == 32: r[rt] = signed(mem(addr, 1), 8) & MASK
            elif op == 35: r[rt] = mem(addr, 4)
            elif op == 40: mem(addr, 1, r[rt])
            elif op == 43: mem(addr, 4, r[rt])
            else: raise AssertionError('unsupported native opcode')
            r[0] = 0; pc = old if old is not None else next_pc; steps += 1
        assert r[29] == STACK+256 and r[31] == STOP
        return bytes(records[DATA]), calls

HARNESS = r'''
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "group.c"
s8 D_8014978C;
static unsigned char *live;
static void *lookup;
static int calls, offset, length;
static unsigned char snapshot[32];
void slot_state_lookup(void *p, u32 *record, int size) {
    assert(p == lookup && calls == 0 && size <= 32);
    calls++; offset = (unsigned char *)record-live; length = size;
    memcpy(snapshot, record, size);
}
int run(int mode, unsigned int selector, int value, const unsigned char *input,
        unsigned char *output, int *call_data, unsigned char *call_bytes, int present) {
    union { uint32_t align; unsigned char bytes[160]; } data;
    Resource resource; Object object; Handle owner;
    memset(&object,0,sizeof(object)); memcpy(data.bytes,input,160);
    resource.data=data.bytes; object.resource=present ? &resource : 0;
    object.lookup=&object; owner.object=&object; lookup=&object; live=data.bytes;
    D_8014978C=(s8)mode; calls=0; offset=-1; length=0;
    audio_bus_mix(&owner,(u8)selector,(s8)value);
    /* Normalize the host-endian checksum and persisted snapshot to big endian. */
    if(calls) {
        uint32_t v; memcpy(&v,data.bytes+offset,4);
        data.bytes[offset]=(unsigned char)(v>>24); data.bytes[offset+1]=(unsigned char)(v>>16);
        data.bytes[offset+2]=(unsigned char)(v>>8); data.bytes[offset+3]=(unsigned char)v;
        memcpy(snapshot,data.bytes+offset,length);
    }
    memcpy(output,data.bytes,160); call_data[0]=offset; call_data[1]=length;
    memcpy(call_bytes,snapshot,32); return calls;
}
'''

def run():
    native = Native(); total = 0; transitions = 0; paths = set()
    with tempfile.TemporaryDirectory(prefix='audio-setter-semantics-') as tmp:
        p = Path(tmp); (p/'host.c').write_text(HARNESS); (p/'group.c').write_text((HERE/'group.c').read_text())
        subprocess.run(['gcc','-std=c99','-O1','-shared','-fPIC',str(p/'host.c'),'-o',str(p/'host.so')],check=True)
        lib = ctypes.CDLL(str(p/'host.so')); U = ctypes.c_ubyte; I = ctypes.c_int
        lib.run.argtypes = [I,ctypes.c_uint,I,ctypes.POINTER(U),ctypes.POINTER(U),ctypes.POINTER(I),ctypes.POINTER(U),I]
        lib.run.restype = I
        def check(mode, selector, value, initial, present=True):
            nonlocal total
            expected, calls = native.run(mode, selector, value, initial, present)
            inp=(U*N).from_buffer_copy(initial); out=(U*N)(); info=(I*2)(); snap=(U*32)()
            count=lib.run(mode,selector,value,inp,out,info,snap,present)
            got_calls=[] if count == 0 else [(info[0],info[1],bytes(snap[:info[1]]))]
            assert bytes(out) == expected and got_calls == calls, (mode,selector,value,present)
            paths.add((count, info[0], info[1])); total += 1
            return expected, bool(calls)
        initial = bytes((i*73+19)&255 for i in range(N))
        for mode in range(-128,128):
            for selector in range(256):
                for value in (-128,0,127):
                    after, changed = check(mode,selector,value,initial)
                    if changed and value == 127:
                        again, repeated = check(mode,selector,value,after)
                        assert not repeated and again == after
                        transitions += 1
        for mode in (-128,-1,0,5,6,13,14,17,18,19,24,25,127):
            for selector in (0,20,21,23,25,31,36,40,255,256,511):
                for value in (-129,-1,128,255,256):
                    check(mode,selector,value,initial)
                    check(mode,selector,value,initial,False)
    return {'status':'PASS','native_vs_host_cases':total,'change_then_same_value_sequences':transitions,
            'persisted_record_shapes':[list(x) for x in sorted(paths)],
            'source_sha256':hashlib.sha256((HERE/'group.c').read_bytes()).hexdigest(),
            'limits':'Synthetic 160-byte profiles; native MIPS caller and internal helper executed, authenticated native jump tables read. The accepted checksum and external persistence routine use explicit functional contracts. Host layout populated through fields; host checksum endianness normalized. This is semantic evidence, not a match or ROM claim.'}

if __name__ == '__main__':
    result=run(); print(json.dumps(result,indent=2))
    if len(sys.argv)>1: Path(sys.argv[1]).write_text(json.dumps(result,indent=2)+'\n')
