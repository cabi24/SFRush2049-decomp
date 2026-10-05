"""Bounded native/candidate/host-C differential check with a modeled callee.

The callback model checks all seven real arguments and writes only the accepted
callee's record fields. This tests the caller, not the projection mathematics.
Only finite floats whose doubled viewport values fit signed 16-bit are sampled.
"""
import ctypes
import random
import struct
import subprocess
from pathlib import Path
import score

HERE = Path(__file__).resolve().parent
START, CALLEE = 0x800A5908, 0x800A5744
VIEW, VP, FOG, COLOR = 0x8017A510, 0x8011EA30, 0x80151AA0, 0x80124FC8
SP, RETURN = 0x10008000, 0xFFFFFFFC


def fbits(value):
    return struct.unpack('>I', struct.pack('>f', value))[0]


def flt(value):
    return struct.unpack('>f', struct.pack('>I', value))[0]


def signed(value, bits=32):
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def execute(words, values):
    memory, captures = {}, []
    regions = [(VIEW, 288), (VP, 16), (FOG, 4), (COLOR, 4), (SP - 256, 512)]

    def access(address, size, value=None):
        assert any(base <= address and address + size <= base + length
                   for base, length in regions), hex(address)
        assert size == 1 or address % size == 0
        if value is None:
            return int.from_bytes(bytes(memory.get(address + i, 0xA5) for i in range(size)), 'big')
        value &= (1 << (8 * size)) - 1
        for i, byte in enumerate(value.to_bytes(size, 'big')):
            memory[address + i] = byte

    registers = [0xCA000000 + i * 0x10101 for i in range(32)]
    registers[0], registers[29], registers[31] = 0, SP, RETURN
    registers[4:8] = values[:4]
    for i, value in enumerate(values[4:]):
        access(SP + 16 + i * 4, 4, value)
    saved = registers[16:24] + [registers[28], registers[30]]
    floats, pc, pending, steps = [0] * 32, START, None, 0
    while pc != RETURN:
        assert START <= pc < START + len(words) * 4 and steps < 120, (hex(pc), steps)
        ins = words[(pc - START) // 4]
        op, rs, rt, rd = ins >> 26, (ins >> 21) & 31, (ins >> 16) & 31, (ins >> 11) & 31
        shift, fn, imm = (ins >> 6) & 31, ins & 63, ins & 65535
        address = (registers[rs] + signed(imm, 16)) & 0xFFFFFFFF
        old, pending = pending, None
        if ins == 0:
            pass
        elif op == 0 and fn == 0:
            registers[rd] = registers[rt] << shift
        elif op == 0 and fn == 3:
            registers[rd] = signed(registers[rt]) >> shift
        elif op == 0 and fn == 8:
            pending = registers[rs]
        elif op == 0 and fn == 33:
            registers[rd] = registers[rs] + registers[rt]
        elif op == 0 and fn == 37:
            registers[rd] = registers[rs] | registers[rt]
        elif op == 9:
            registers[rt] = address
        elif op == 12:
            registers[rt] = registers[rs] & imm
        elif op == 13:
            registers[rt] = registers[rs] | imm
        elif op == 15:
            registers[rt] = imm << 16
        elif op == 3:
            destination = ((pc + 4) & 0xF0000000) | ((ins & 0x3FFFFFF) << 2)
            assert destination == CALLEE
            registers[31], pending = pc + 8, ('call', pc + 8)
        elif op in (35, 36, 37):
            registers[rt] = access(address, {35: 4, 36: 1, 37: 2}[op])
        elif op in (40, 41, 43):
            access(address, {40: 1, 41: 2, 43: 4}[op], registers[rt])
        elif op == 49:
            floats[rt] = access(address, 4)
        elif op == 57:
            access(address, 4, floats[rt])
        elif op == 17 and rs == 0:
            registers[rt] = floats[rd]
        elif op == 17 and rs == 4:
            floats[rd] = registers[rt]
        elif op == 17 and rs == 16 and fn == 2:
            floats[shift] = fbits(flt(floats[rd]) * flt(floats[rt]))
        elif op == 17 and rs == 16 and fn == 13:
            floats[shift] = int(flt(floats[rd])) & 0xFFFFFFFF
        else:
            raise AssertionError('Unsupported instruction at ' + hex(pc))
        registers = [x & 0xFFFFFFFF for x in registers]
        registers[0] = 0
        steps += 1
        if isinstance(old, tuple):
            args = registers[4:8] + [access(registers[29] + i, 4) for i in (16, 20, 24)]
            assert args == [values[0], *values[3:]]
            captures.append(args)
            entry = VIEW + values[0] * 72
            for i in range(2, 14):
                access(entry + i * 4, 4, 0xA0000000 + values[0] + i)
            for i in [*range(1, 16), 24, 25]:
                registers[i] = 0xBD000000 + i
            registers[2] = entry
            for i in range(20):
                floats[i] = 0x7FC12345
            pc = old[1]
        else:
            pc = old if old is not None else pc + 4
    assert registers[29] == SP and saved == registers[16:24] + [registers[28], registers[30]]
    assert len(captures) == 1
    return [access(VIEW + 4 * i, 4) for i in range(72)] + [
            access(VP + 4 * i, 4) for i in range(4)] + [
            access(FOG, 4), access(COLOR, 4)] + captures[0]


HOST_SUFFIX = r'''
#include <string.h>
Descriptor72 D_8017A510[4];
Viewport16 D_8011EA30;
f32 D_80151AA0;
u32 D_80124FC8;
static u32 captured[7];
static u32 bits(f32 value) { u32 result; memcpy(&result,&value,4); return result; }
static f32 number(u32 value) { f32 result; memcpy(&result,&value,4); return result; }
Descriptor72 *exhaust_smoke_effect(int index,f32 x,f32 y,f32 w,f32 h,f32 n,f32 f)
{
    int i; u32 value;
    captured[0]=index; captured[1]=bits(x); captured[2]=bits(y); captured[3]=bits(w);
    captured[4]=bits(h); captured[5]=bits(n); captured[6]=bits(f);
    for(i=2;i<14;i++) {
        value=0xA0000000u+index+i;
        memcpy((u8 *)&D_8017A510[index]+i*4,&value,4);
    }
    return &D_8017A510[index];
}
void host_run(u32 *in,u32 *out)
{
    int i,j,k=0; u32 value;
    memset(D_8017A510,0xA5,sizeof(D_8017A510));
    memset(&D_8011EA30,0xA5,sizeof(D_8011EA30));
    arb_rate_set(in[0],in[1],in[2],number(in[3]),number(in[4]),number(in[5]),
                 number(in[6]),number(in[7]),number(in[8]));
    for(i=0;i<4;i++) {
        for(j=0;j<16;j++) { memcpy(&value,(u8 *)&D_8017A510[i]+j*4,4); out[k++]=value; }
        out[k++]=((u32)(u16)D_8017A510[i].range_start<<16)|(u16)D_8017A510[i].range_end;
        out[k++]=((u32)D_8017A510[i].red<<24)|((u32)D_8017A510[i].green<<16)|
                 ((u32)D_8017A510[i].blue<<8)|D_8017A510[i].alpha;
    }
    for(i=0;i<4;i+=2) out[k++]=((u32)(u16)D_8011EA30.scale[i]<<16)|(u16)D_8011EA30.scale[i+1];
    for(i=0;i<4;i+=2) out[k++]=((u32)(u16)D_8011EA30.translate[i]<<16)|(u16)D_8011EA30.translate[i+1];
    out[k++]=bits(D_80151AA0); out[k++]=D_80124FC8;
    for(i=0;i<7;i++) out[k++]=captured[i];
}
typedef char check_descriptor_size[sizeof(Descriptor72)==72?1:-1];
typedef char check_viewport_size[sizeof(Viewport16)==16?1:-1];
'''


def run_cases(candidate, directory):
    source = (HERE / 'candidate.c').read_text()
    # Host adapters preserve the 32-bit target pointer slots; values are never dereferenced.
    source = source.replace('void *first,*bounds;', 'u32 first,bounds;').replace(
                            'void *first,void *bounds', 'u32 first,u32 bounds')
    path, library = directory / 'host.c', directory / 'host.so'
    path.write_text(source + HOST_SUFFIX)
    subprocess.run(['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror',
                    '-O2', '-shared', '-fPIC', str(path), '-o', str(library)],
                   check=True, capture_output=True)
    lib = ctypes.CDLL(str(library))
    lib.host_run.argtypes = [ctypes.POINTER(ctypes.c_uint32), ctypes.POINTER(ctypes.c_uint32)]
    native = score.targets()['arb_rate_set']
    rng = random.Random(0xA5908)
    edges = [-16384.0, -16383.75, -1000.5, -1.75, -0.5, -0.0, 0.0,
             0.25, 0.5, 1.75, 119.5, 160.0, 240.0, 320.0, 16383.0, 16383.25]
    cases = []
    for index in range(4):
        for value in edges:
            cases.append([index, 0xA1000000 + index * 16, 0xB2000000 + index * 32,
                          fbits(60.0), fbits(45.0), *[fbits(value)] * 4])
    for _ in range(4096):
        cases.append([rng.randrange(4), rng.getrandbits(32), rng.getrandbits(32),
                     fbits(rng.uniform(-100.0, 180.0)), fbits(rng.uniform(-100.0, 180.0)),
                     *[fbits(rng.uniform(-16383.0, 16383.0)) for _ in range(4)]])
    for values in cases:
        actual = (ctypes.c_uint32 * 85)()
        lib.host_run((ctypes.c_uint32 * 9)(*values), actual)
        target, got = execute(native, values), execute(candidate, values)
        assert target == got == list(actual), values
    return {'cases': len(cases), 'native_and_candidate_executions': 2 * len(cases),
            'host_c_comparisons': len(cases), 'captured_arguments_per_call': 7,
            'output_words_per_case': 85,
            'scope': 'finite viewport inputs fitting signed 16-bit; modeled accepted callee writes',
            'invalid_nan_infinite_or_out_of_range_inputs_tested': False}
