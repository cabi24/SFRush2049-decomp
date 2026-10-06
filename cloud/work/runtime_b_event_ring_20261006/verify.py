#!/usr/bin/env python3
"""Complete image-B body, ELF, GNU link and bounded native/C behavior proof."""
import argparse
import contextlib
import ctypes
import hashlib
import io
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile

if not __debug__:
    raise RuntimeError('Verification requires assertions enabled')
PACKET = Path(__file__).resolve().parent
PROJECT = PACKET.parents[2]
ROOT = Path(os.environ.get('RUSH_REPO', str(PROJECT))).resolve()
sys.path.insert(0, str(ROOT))
from tools.cloud import score
SOURCE = PROJECT / 'cloud/matches/ovl_b/func_803914B4.c'
BASE = 'cd22879d40b3de443cfde047b86e75e159b6cec6'
NAME = 'func_803914B4'
ADDRESS = 0x803914B4
SIZE = 396
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
IMAGE = 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
TARGET = '6ed117381897934108773febf083f53af71131a0242e8ba4a3c9b4ad73560a38'
U32 = 0xffffffff
STACK = 0x70000000
RETURN = 0x60000000
HELPER = 0x800F7E30
REGIONS = [('D_80146130', 0x80146130, 1), ('D_80152818', 0x80152818, 3808),
           ('D_80395E70', 0x80395E70, 97), ('D_80151AD0', 0x80151AD0, 2),
           ('D_80115F28', 0x80115F28, 128), ('D_803940D0', 0x803940D0, 128),
           ('D_80149428', 0x80149428, 16)]


def sha_bytes(b):
    return hashlib.sha256(b).hexdigest()


def sha(p):
    return sha_bytes(Path(p).read_bytes())


def signed(n, bits):
    n &= (1 << bits) - 1
    return n - (1 << bits) if n >> (bits - 1) else n


def run(args):
    p = subprocess.run([str(a) for a in args], capture_output=True, text=True)
    assert p.returncode == 0, (args, p.returncode, p.stdout, p.stderr)
    return p


def pinned(path):
    return subprocess.check_output(['git', '-C', str(ROOT), 'show', BASE + ':' + path])


class Memory:
    def __init__(self, seed):
        self.parts = [(a, bytearray(((i * 71 + seed * 19) ^ 0xa5) & 255
                                   for i in range(n))) for _, a, n in REGIONS]
        self.parts.append((STACK - 64, bytearray([0xcd] * 128)))

    def region(self, a, n):
        for base, data in self.parts:
            if base <= a and a + n <= base + len(data):
                return data, a - base
        raise AssertionError(('unmapped', hex(a), n))

    def get(self, a, n):
        assert a % n == 0
        d, i = self.region(a, n)
        return int.from_bytes(d[i:i+n], 'big')

    def put(self, a, value, n):
        assert a % n == 0
        d, i = self.region(a, n)
        d[i:i+n] = (value & ((1 << (n*8)) - 1)).to_bytes(n, 'big')

    def state(self):
        return b''.join(bytes(d) for a, d in self.parts[:-1])


def fixture(case):
    seed, row, column, delta, guard, remaining, ring, selector = case
    m = Memory(seed)
    m.put(0x80146130, guard, 1)
    m.put(0x80152818 + row * 952 + 931, remaining, 1)
    m.put(0x80395ED0, ring, 1)
    m.put(0x80151AD0, selector, 2)
    values = [-2147483648, -16777217, -16777216, -1, 0, 1,
              16777215, 16777216, 16777217, 2147483647]
    for base in [0x80115F28, 0x803940D0]:
        for i in range(32):
            v = values[(seed+i) % len(values)] if seed & 1 else signed(seed*7919+i*65537, 32)
            m.put(base + i*4, v, 4)
    return m


def expected(case):
    _, row, column, delta, guard, remaining, ring, selector = case
    m = fixture(case)
    if delta < 0:
        p = 0x80149428 + row*4 + column
        m.put(p, signed(m.get(p, 1), 8) + delta, 1)
        if guard == 0:
            return m
        if remaining <= 0:
            m.put(0x80152818 + row*952 + 931, 0, 1)
            return m
    m.put(0x80395ED0, (ring + 1) % 4, 1)
    p = 0x80395E70 + ring*24
    for offset, value in [(0, 1), (1, 0), (2, column), (3, row), (20, delta)]:
        m.put(p + offset, value, 1)
    for out, base, index in [(4, 0x80115F28, column), (12, 0x803940D0, row)]:
        for j in range(2):
            v = signed(m.get(base + (selector-1)*32 + index*8 + j*4, 4), 32)
            bits = int.from_bytes(struct.pack('>f', float(v)), 'big')
            m.put(p + out + j*4, bits, 4)
    return m


def execute(body, helper, case, poison=False):
    m = fixture(case)
    initial_state = m.state()
    row, column, delta = case[1:4]
    regs = [((i+1)*0x1020304 ^ case[0]) & U32 for i in range(32)]
    # Arbitrary high bits exercise the callee's explicit signed-byte narrowing.
    regs[4:7] = [0x7abcde00 | (x & 255) for x in (row, column, delta)]
    regs[0] = 0
    regs[29] = STACK
    regs[31] = RETURN
    original = regs[:]
    fregs = [0x11223344] * 32
    pc = ADDRESS
    pending = None
    coverage, branches = set(), set()
    calls = 0
    helper_return = None
    stack_before = bytes(m.parts[-1][1])
    for _ in range(1000):
        if pc == helper_return:
            helper_return = None
            if poison:
                for r in [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25]:
                    regs[r] = (0xf0000000+r*0x12345) & U32
                for r in range(20):
                    fregs[r] = 0x7fc00000
        if ADDRESS <= pc < ADDRESS+SIZE:
            w = body[(pc-ADDRESS)//4]
            coverage.add(pc-ADDRESS)
        elif HELPER <= pc < HELPER+len(helper)*4:
            w = helper[(pc-HELPER)//4]
        else:
            raise AssertionError(('bad pc', hex(pc)))
        old = pending
        pending = None
        op = w >> 26
        rs, rt, rd, shift, fn = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31, (w >> 6) & 31, w & 63
        imm = signed(w, 16)
        addr = (regs[rs] + imm) & U32
        if op == 0:
            if fn == 0:
                regs[rd] = (regs[rt] << shift) & U32
            elif fn == 3:
                regs[rd] = (signed(regs[rt], 32) >> shift) & U32
            elif fn == 0x21:
                regs[rd] = (regs[rs] + regs[rt]) & U32
            elif fn == 0x23:
                regs[rd] = (regs[rs] - regs[rt]) & U32
            elif fn == 0x25:
                regs[rd] = regs[rs] | regs[rt]
            elif fn == 8:
                assert old is None
                pending = regs[rs]
            else:
                raise AssertionError(('SPECIAL', fn))
        elif op == 9:
            regs[rt] = addr
        elif op == 15:
            regs[rt] = (w & 65535) << 16
        elif op == 10:
            regs[rt] = int(signed(regs[rs], 32) < imm)
        elif op in [0x20, 0x21, 0x23]:
            n = {0x20: 1, 0x21: 2, 0x23: 4}[op]
            regs[rt] = signed(m.get(addr, n), n*8) & U32
        elif op in [0x28, 0x2b]:
            m.put(addr, regs[rt], 1 if op == 0x28 else 4)
        elif op in [1, 4, 5, 7]:
            assert old is None
            if op == 1:
                assert rt == 1
                taken = signed(regs[rs], 32) >= 0
            elif op == 4:
                taken = regs[rs] == regs[rt]
            elif op == 5:
                taken = regs[rs] != regs[rt]
            else:
                assert rt == 0
                taken = signed(regs[rs], 32) > 0
            branches.add((pc-ADDRESS, taken))
            pending = pc + 4 + imm*4 if taken else pc + 8
        elif op == 3:
            assert old is None
            target = ((pc+4) & 0xf0000000) | ((w & 0x3ffffff) << 2)
            assert target == HELPER
            regs[31] = pc+8
            pending = target
            helper_return = pc+8
        elif op == 17:
            if rs == 4:
                fregs[rd] = regs[rt]
            elif rs == 20 and fn == 32:
                fregs[shift] = int.from_bytes(struct.pack('>f', float(signed(fregs[rd], 32))), 'big')
            else:
                raise AssertionError(('COP1', rs, fn))
        elif op == 0x39:
            m.put(addr, fregs[rt], 4)
        else:
            raise AssertionError(('opcode', op))
        regs[0] = 0
        pc += 4
        if old is not None:
            assert pending is None, 'control transfer in delay slot'
            pc = old
            if pc == HELPER:
                calls += 1
                assert regs[4:7] == [x & U32 for x in (row, column, delta)]
                assert m.state() == initial_state, 'global write before helper'
            if pc == RETURN:
                break
    else:
        raise AssertionError('did not return')
    assert calls == int(delta < 0)
    assert m.state() == expected(case).state(), ('complete native state', case)
    assert all(regs[r] == original[r] for r in list(range(16, 24)) + [28, 29, 30, 31])
    now = bytes(m.parts[-1][1])
    assert now[:40] == stack_before[:40] and now[76:] == stack_before[76:]
    return coverage, branches


def host_cases(library, cases):
    lib = ctypes.CDLL(str(library))
    lib.func_803914B4.argtypes = [ctypes.c_byte] * 3
    lib.func_803914B4.restype = None
    def carray(name, n):
        return (ctypes.c_ubyte*n).in_dll(lib, name)
    def events(data):
        result = bytearray(data)
        if sys.byteorder == 'little':
            for i in range(4):
                for k in [4, 8, 12, 16]:
                    off = i*24+k
                    result[off:off+4] = result[off:off+4][::-1]
        return bytes(result)
    for case in cases:
        m = fixture(case)
        for (name, address, n), (_, data) in zip(REGIONS, m.parts):
            if name == 'D_80395E70':
                carray(name, 96)[:] = events(data[:96])
                carray('D_80395ED0', 1)[:] = data[96:]
            elif name in ['D_80115F28', 'D_803940D0']:
                (ctypes.c_int32*32).in_dll(lib, name)[:] = struct.unpack('>32i', data)
            elif name == 'D_80151AD0':
                ctypes.c_int16.in_dll(lib, name).value = signed(int.from_bytes(data, 'big'), 16)
            else:
                carray(name, n)[:] = data
        ctypes.c_int.in_dll(lib, 'call_count').value = 0
        lib.func_803914B4(*case[1:4])
        actual = []
        for name, _, n in REGIONS:
            if name == 'D_80395E70':
                actual.append(events(carray(name, 96)) + bytes(carray('D_80395ED0', 1)))
            elif name in ['D_80115F28', 'D_803940D0']:
                actual.append(struct.pack('>32i', *(ctypes.c_int32*32).in_dll(lib, name)))
            elif name == 'D_80151AD0':
                actual.append(struct.pack('>h', ctypes.c_int16.in_dll(lib, name).value))
            else:
                actual.append(bytes(carray(name, n)))
        assert b''.join(actual) == expected(case).state(), ('host mismatch', case)
        assert ctypes.c_int.in_dll(lib, 'call_count').value == int(case[3] < 0)
        if case[3] < 0:
            assert list((ctypes.c_int*3).in_dll(lib, 'call_args')) == list(case[1:4])
            assert events(carray('call_ring', 96)) == bytes(m.parts[2][1][:96])
            for name, value in [('call_index', case[6]), ('call_guard', case[4]),
                                ('call_remaining', case[5]),
                                ('call_score', signed(m.get(0x80149428+case[1]*4+case[2], 1), 8))]:
                assert ctypes.c_byte.in_dll(lib, name).value == value
            assert ctypes.c_short.in_dll(lib, 'call_selector').value == case[7]


def fixtures():
    cases = []
    # Every delta and remaining byte, crossed with each branch mode and all rings.
    for byte in range(256):
        for mode in range(4):
            cases.append((len(cases), byte % 4, (byte//4) % 4, signed(byte, 8),
                          [0, 1, -1, -128][mode], signed(byte*73+mode*47, 8),
                          mode, (byte//16) % 4 + 1))
    # Deterministic complete row/column/selector/ring cross with all gate edges.
    for row in range(4):
        for column in range(4):
            for selector in range(1, 5):
                for ring in range(4):
                    for delta, guard, remaining in [(-128, 0, -128), (-1, 1, -1),
                                                     (-1, -128, 0), (-1, 127, 1),
                                                     (0, 0, -128), (127, 1, 127)]:
                        cases.append((len(cases), row, column, delta, guard, remaining, ring, selector))
    return cases


def inspect(obj, linked=False):
    data, sections = score._elf(obj)
    symbols = [s for i, t in enumerate(sections) if t['type'] == 2
               for s in score._symbol_table(data, sections, i)]
    fns = [s for s in symbols if s['type'] == 2 and s['section'] == score._text_index(sections)]
    assert len(fns) == 1 and fns[0]['name'] == NAME and fns[0]['size'] == SIZE
    assert fns[0]['value'] == (ADDRESS if linked else 0)
    shoff = struct.unpack_from('>I', data, 0x20)[0]
    entsize = struct.unpack_from('>H', data, 0x2e)[0]
    for i, sec in enumerate(sections):
        flags = struct.unpack_from('>I', data, shoff+i*entsize+8)[0]
        assert not (flags & 2 and sec['size'] and sec['name'] not in
                    ['.text', '.reginfo', '.MIPS.abiflags'])
    ti = score._text_index(sections)
    text = sections[ti]
    raw = data[text['off']:text['off']+text['size']]
    assert len(raw) == 400 and raw[SIZE:] == bytes(4)
    address = struct.unpack_from('>I', data, shoff+ti*entsize+12)[0]
    assert address == (ADDRESS if linked else 0)
    return data, sections, raw


def prove():
    score.ASM_DIR = ROOT / 'asm/us/ovl_b'
    native = score.targets()[NAME]
    assert len(native)*4 == SIZE and sha_bytes(struct.pack('>99I', *native)) == TARGET
    ext = json.loads(score.verified_bytes(score.ASM_DIR/'extents.json', score.target_manifest()))
    entry = next(x for x in ext['functions'] if x['name'] == NAME)
    assert (ext['image'], ext['image_sha256'], entry['address'], entry['size']) == ('B', IMAGE, hex(ADDRESS).upper().replace('0X', '0x'), SIZE)
    addresses = score.image_symbols()
    bdir = score.ASM_DIR
    score.ASM_DIR = ROOT/'asm/us/blob'
    blob = score.targets()
    score.ASM_DIR = bdir
    helper = blob['func_800F7E30']
    lock = json.loads(pinned('blob_matched.lock.json'))
    helpers = {}
    for name in ['func_800F7E30', 'effect_cleanup']:
        source = pinned('src/blob/'+name+'.c')
        assert lock[name]['source_sha256'] == sha_bytes(source) and lock[name]['verified'] == 'image_gate'
        helpers[name] = {'address': hex(addresses[name]), 'source_sha256': sha_bytes(source),
                         'native_bytes': len(blob[name])*4,
                         'native_sha256': sha_bytes(struct.pack('>%dI'%len(blob[name]), *blob[name]))}
    assert addresses['func_800F7E30'] == HELPER and addresses['effect_cleanup'] == 0x800C55E4
    calls = []
    for name, body in blob.items():
        for i, w in enumerate(body):
            if w >> 26 == 3 and ((w & 0x3ffffff) << 2 | 0x80000000) == ADDRESS:
                calls.append((name, hex(addresses[name]+i*4)))
    assert sorted(calls) == sorted([('effect_cleanup', '0x800c562c'), ('camera_play_script', '0x800c60b8'),
        ('entity_update', '0x800c6fac'), ('func_800E0B20', '0x800e0ff8'),
        ('func_800E0B20', '0x800e1084'), ('func_800E0B20', '0x800e1120')])
    consumer = score.targets()['func_803936A8']
    # Consumer memory operations independently witness the complete field view.
    for off, op, displacement in [(0x140, 0x20, 0), (0x14c, 0x20, 1), (0x15c, 0x20, 20),
                                  (0x174, 0x20, 3), (0x17c, 0x20, 2),
                                  (0x1b8, 0x31, 4), (0x1bc, 0x31, 12),
                                  (0x1c4, 0x31, 8), (0x1cc, 0x31, 16)]:
        assert consumer[off//4] >> 26 == op and consumer[off//4] & 65535 == displacement
    assert consumer[0x434//4] & 65535 == 24 and consumer[0x430//4] & 65535 == 0x5ed0
    cases = fixtures()
    with tempfile.TemporaryDirectory(prefix='b-event-ring-') as td:
        tmp = Path(td)
        obj = tmp/'candidate.o'
        score.compile_single(SOURCE, FLAGS, obj)
        with contextlib.redirect_stdout(io.StringIO()):
            comparison = score.compare(obj, NAME)
        assert comparison.accepted() and not comparison.differing
        data, sections, raw = inspect(obj)
        relocated, masks, unresolved, unverified, errors = score.relocate(obj, score.text_words(obj), 0, SIZE, addresses)
        assert not any((masks, unresolved, unverified, errors)) and relocated[:99] == native and relocated[99:] == [0]
        relocations = []
        for sec in sections:
            if sec['type'] != 9:
                continue
            assert sec['info'] == score._text_index(sections)
            syms = score._symbol_table(data, sections, sec['link'])
            for at in range(sec['off'], sec['off']+sec['size'], 8):
                offset, info = struct.unpack_from('>II', data, at)
                assert offset % 4 == 0 and 0 <= offset < SIZE and info & 255 in [4, 5, 6]
                relocations.append({'offset': offset, 'type': info & 255, 'symbol': syms[info >> 8]['name']})
        bindings = {r['symbol']: addresses.get(r['symbol'], score.address_named(r['symbol'])) for r in relocations}
        assert bindings == {'func_800F7E30': 0x800F7E30, 'D_80146130': 0x80146130,
                            'D_80152818': 0x80152818, 'D_80395ED0': 0x80395ED0,
                            'D_80395E70': 0x80395E70, 'D_80151AD0': 0x80151AD0,
                            'D_80115F28': 0x80115F28, 'D_803940D0': 0x803940D0}
        assert len(relocations) == 15
        script = tmp/'native.ld'
        script.write_text('SECTIONS { . = 0x803914B4; .text : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n'+
                          '\n'.join('%s = 0x%08x;' % (k, v) for k, v in bindings.items()))
        linked = tmp/'linked.elf'
        run(['mips-linux-gnu-ld', '-EB', '-T', script, '-o', linked, obj])
        _, _, linked_raw = inspect(linked, True)
        assert list(struct.unpack('>100I', linked_raw)) == relocated
        # Native IDO layout assertions compile the same unchanged target body.
        layout = tmp/'layout.c'
        layout.write_text('#include "'+str(SOURCE)+'"\n#define OFF(t,m) ((unsigned int)&((t*)0)->m)\n'+
                          'typedef char p[(sizeof(void*)==4 && sizeof(Player)==952 && OFF(Player,remaining)==931)?1:-1];\n'+
                          'typedef char e[(sizeof(Event)==24 && OFF(Event,active)==0 && OFF(Event,age)==1 && OFF(Event,column)==2 && OFF(Event,row)==3 && OFF(Event,from_x)==4 && OFF(Event,from_y)==8 && OFF(Event,to_x)==12 && OFF(Event,to_y)==16 && OFF(Event,delta)==20)?1:-1];\n')
        score.compile_single(layout, FLAGS, tmp/'layout.o')
        assert inspect(tmp/'layout.o')[2] == raw
        coverage, branches = set(), set()
        for case in cases:
            for body in [native, relocated[:99], list(struct.unpack('>99I', linked_raw[:SIZE]))]:
                for poison in [False, True]:
                    c, b = execute(body, helper, case, poison)
                    coverage |= c
                    branches |= b
        assert coverage == set(range(0, SIZE, 4))
        assert branches == {(o, v) for o in [0x34, 0x74, 0xa0, 0xdc] for v in [False, True]} | {(0xa8, True)}
        library = tmp/'host.so'
        common = ['gcc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror', '-O2', '-shared', '-fPIC',
                  '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all', '-fno-fast-math', '-ffp-contract=off']
        run(common + ['-DCANDIDATE_PATH="'+str(SOURCE)+'"', PACKET/'host_test.c', '-o', library])
        host_cases(library, cases)
        mutants = {'wrong_delta_guard': ('if (delta < 0)', 'if (delta <= 0)'),
                   'wrong_remaining_boundary': ('remaining <= 0', 'remaining < 0'),
                   'wrong_ring_wrap': ('D_80395ED0 >= 4', 'D_80395ED0 >= 3'),
                   'swapped_row_column': ('event->row = row;', 'event->row = column;'),
                   'wrong_delta_store': ('event->delta = delta;', 'event->delta = 0;'),
                   'wrong_table_column': ('[column][0]', '[row][0]')}
        rejected = []
        for name, (old, new) in mutants.items():
            assert old in SOURCE.read_text()
            source = tmp/(name+'.c')
            source.write_text(SOURCE.read_text().replace(old, new))
            mutant = tmp/(name+'.so')
            run(common + ['-DCANDIDATE_PATH="'+str(source)+'"', PACKET/'host_test.c', '-o', mutant])
            try:
                host_cases(mutant, cases)
            except AssertionError:
                rejected.append(name)
            else:
                raise AssertionError(('mutant accepted', name))
        try:
            execute([U32]+native[1:], helper, cases[0])
        except AssertionError:
            pass
        else:
            raise AssertionError('unknown opcode accepted')
        # Initial source is the same natural C, only its documented compiler level changes.
        score.compile_single(SOURCE, FLAGS.replace('-O3', '-O2'), obj)
        with contextlib.redirect_stdout(io.StringIO()):
            o2 = score.compare(obj, NAME)
        assert not o2.accepted()
        controls = {'O2': {'differing_words': o2.differing, 'target_words': o2.total,
                           'text_bytes': len(score.text_words(obj))*4, 'extra_words': o2.extra_words}}
        return {'status': 'MATCH', 'base': BASE, 'image': 'B', 'rom_stream': '0xB6FEC4',
                'image_sha256': IMAGE, 'address': hex(ADDRESS), 'bytes': SIZE, 'native_sha256': TARGET,
                'source_sha256': sha(SOURCE), 'flags': FLAGS+' -Wab,-r4300_mul', 'accepted_or_coverage_bytes': 0,
                'elf': {'function_bytes': SIZE, 'text_bytes': 400, 'zero_alignment_bytes': 4,
                        'owned_data_bytes': 0, 'differing_words': 0, 'relocations': relocations,
                        'bindings': {k: hex(v) for k, v in sorted(bindings.items())},
                        'whole_object_gnu_agrees': True, 'linked_text_sha256': sha_bytes(linked_raw)},
                'helpers': helpers, 'game_direct_calls': sorted([list(x) for x in calls]),
                'consumer': {'address': '0x803936a8', 'bytes': len(consumer)*4,
                             'sha256': sha_bytes(struct.pack('>%dI'%len(consumer), *consumer)),
                             'record_count': 4, 'record_stride': 24},
                'behavior': {'fixtures': len(cases), 'native_project_gnu_executions': len(cases)*6,
                             'native_helper_executed': True, 'host_c89_ubsan_bounds_cases': len(cases),
                             'instruction_offsets': len(coverage), 'branch_outcomes': len(branches),
                             'full_state_and_helper_entry_checks': True, 'caller_save_poisoning': True,
                             'callee_saves_and_stack_canaries': True, 'compiled_mutants_rejected': rejected,
                             'domain': 'row/column/ring 0..3; selector 1..4 backed coordinate rows; all signed-byte deltas and counter values; round-to-nearest binary32 signed32 conversions; nonaliasing valid storage; stable image B residency; no concurrency or FCSR equality claim'},
                'controls': controls,
                'tool_sha256': {k: sha(v) for k, v in [(n, score.ido(n)) for n in ['cc','cfe','uopt','ugen','as1']]+[(n, shutil.which(n)) for n in ['mips-linux-gnu-ld','gcc']]},
                'inputs_sha256': {str(p.relative_to(ROOT)): sha(p) for p in [ROOT/'tools/cloud/score.py', ROOT/'tools/cloud/owndata.py',
                    bdir/'SHA256SUMS', bdir/'ovl_b_8038a400.s', bdir/'symbols.json', bdir/'extents.json', ROOT/'asm/us/blob/SHA256SUMS']},
                'packet_sha256': {p.name: sha(p) for p in [PACKET/'verify.py', PACKET/'host_test.c', PACKET/'test_packet.py', PACKET/'README.md']}}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    result = prove()
    path = PACKET/'verification.json'
    if args.check:
        assert result == json.loads(path.read_text()), 'frozen receipt differs'
        print('396-byte event-ring MATCH: frozen receipt, complete ELF/GNU and 2560 native/C fixtures passed')
    else:
        path.write_text(json.dumps(result, indent=2)+'\n')
        print(json.dumps(result, indent=2))
