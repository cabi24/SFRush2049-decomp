#!/usr/bin/env python3
"""Bounded actual-helper checks. Native words are read, never emitted or stored.

No compiler, complete AA8C execution, original-C claim, or FCSR claim is made.
All production context is read from BASE; only native target identities bind the
live canonical target reader. Run with --reference-root REPOSITORY.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import subprocess
import sys

BASE = '6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
SCENE, COUNT, PEAK, HEAD = 0x8012E700, 0x80156990, 0x801569A8, 0x8015B254
STACK, RETURN, TRANSFORM, SOURCE = 0x600100, 0xFFFFFFFC, 0x700000, 0x700100
MASK = 0xFFFFFFFF
NAMES = ('sign_extend_call', 'func_8008E26C', 'render_mode_select', 'func_8008E144',
         'sound_call_minimal', 'entity_spawn_callback', 'func_8009002C', 'func_8008FFD0',
         'func_8008B3A0', 'func_8008E06C', 'func_80090770', 'func_8008D870',
         'func_8008D6FC', 'math_utility', 'model_data_load', 'model_transform_setup')


def signed(v, bits=32):
    v &= (1 << bits) - 1
    return v - (1 << bits) if v >> (bits - 1) else v


def git(root, path):
    return subprocess.check_output(['git', '-C', str(root), 'show', BASE + ':' + path])


def load(root):
    # Support canonical score.py sibling imports from any working directory.
    sys.path.insert(0, str(root / 'tools/cloud'))
    symbols = json.loads(git(root, 'asm/us/blob/symbols.json'))['symbols']
    sections = {}
    paths = subprocess.check_output(['git', '-C', str(root), 'ls-tree', '-r', '--name-only',
                                     BASE, 'asm/us/blob']).decode().splitlines()
    for path in paths:
        if not path.endswith('.s'):
            continue
        current = None
        for line in git(root, path).decode().splitlines():
            m = re.match(r'\s*\.section \.text\.(\S+?),', line)
            if m:
                current = m[1]
                if current in NAMES:
                    sections[current] = []
            m = re.match(r'\s*\.word (0x[0-9A-Fa-f]+)', line)
            if m and current in NAMES:
                sections[current].append(int(m[1], 16))
    assert set(sections) == set(NAMES)
    spec = importlib.util.spec_from_file_location('effect_score', root / 'tools/cloud/score.py')
    score = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = score
    spec.loader.exec_module(score)
    score.ASM_DIR = root / 'asm/us/blob'
    live = score.targets()
    for name, words in sections.items():
        assert live[name] == words, ('native helper changed', name)
    code, metadata, addresses = {}, {}, {}
    for name, words in sections.items():
        address = addresses[name] = int(symbols[name], 16)
        code.update((address + i * 4, w) for i, w in enumerate(words))
        raw = struct.pack('>' + 'I' * len(words), *words)
        metadata[name] = dict(address=hex(address), size=len(raw), sha256=hashlib.sha256(raw).hexdigest())
    callgraph = {}
    for name, words in sections.items():
        dests = []
        for w in words:
            if w >> 26 == 3:
                dest = 0x80000000 | ((w & 0x3FFFFFF) << 2)
                assert dest in code, ('uninspected helper call', name, hex(dest))
                dests.append(hex(dest))
            if w >> 26 == 0 and w & 63 in (8,9):
                assert w & 63 == 8 and (w >> 21)&31 == 31, ('indirect callback', name)
        callgraph[name] = sorted(set(dests))
    score.ASM_DIR = root / 'asm/us/ovl_b'
    root_words = score.targets()['func_8038AA8C']
    raw = struct.pack('>'+'I'*len(root_words),*root_words)
    assert len(raw) == 7812 and hashlib.sha256(raw).hexdigest() == 'c8842f0b25b7c17e36345611643e4b67a7c0c17929f7be6ba93785b530b67b39'
    root_calls = [(hex(0x8038AA8C+i*4),hex(0x80000000|((w&0x3FFFFFF)<<2))) for i,w in enumerate(root_words) if w>>26 == 3]
    assert len(root_calls) == 61 and len(set(d for a,d in root_calls)) == 14
    return code, addresses, metadata, callgraph, root_calls


class Memory:
    def __init__(self):
        self.regions = [(SCENE, bytearray(68 * 8)), (STACK - 1024, bytearray(2048)),
                        (TRANSFORM, bytearray(512)), (COUNT, bytearray(4)),
                        (PEAK, bytearray(4)), (HEAD, bytearray(2))]
        self.reads, self.writes = [], []

    def locate(self, a, w):
        assert a % w == 0, ('unaligned', hex(a), w)
        for base, data in self.regions:
            if base <= a and a + w <= base + len(data):
                return data, a - base
        raise AssertionError(('unmapped', hex(a), w))

    def get(self, a, w=4):
        data, off = self.locate(a, w)
        self.reads.append((a, w))
        return int.from_bytes(data[off:off+w], 'big')

    def put(self, a, value, w=4):
        data, off = self.locate(a, w)
        value &= (1 << (w * 8)) - 1
        data[off:off+w] = value.to_bytes(w, 'big')
        self.writes.append((a, w, value))

    def scene(self, i, off=0):
        return SCENE + 68 * i + off

    def initialize(self, count=4):
        self.put(COUNT, count)
        self.put(PEAK, count)
        self.put(HEAD, 0 if count else -1, 2)
        for i in range(8):
            a = self.scene(i)
            for off in range(0, 68, 4):
                self.put(a+off, 0xA0000000 | i*256 | off)
            self.put(a+0x14, i if i < count else -1, 2)
            self.put(a+0x16, -1, 2)
            self.put(a+0x18, i+1 if i+1 < count else -1, 2)
            self.put(a+8, TRANSFORM)
        for i in range(128):
            self.put(TRANSFORM+i*4, 0x3F000000 + i*0x10000)
        self.reads.clear(); self.writes.clear()


class Machine:
    def __init__(self, code, memory):
        self.code, self.m = code, memory
        self.coverage = set()

    def call(self, address, args):
        r = [(0xA9000000 + i) for i in range(32)]
        r[0], r[29], r[31] = 0, STACK, RETURN
        r[4:4+len(args)] = [a & MASK for a in args]
        original = r[:]
        f = [None] * 32
        pc, pending, lo = address, None, None
        m = self.m
        for _ in range(10000):
            if pc == RETURN:
                assert r[29] == STACK and r[16:24] == original[16:24]
                return r[2]
            assert pc in self.code, ('unknown pc', hex(pc))
            self.coverage.add(pc)
            w = self.code[pc]
            op, rs, rt, rd, sh, fn = w >> 26, (w >> 21)&31, (w >> 16)&31, (w >> 11)&31, (w >> 6)&31, w&63
            si, imm = signed(w, 16), w&65535
            old, pending, nxt = pending, None, pc+4
            a = (r[rs] + si) & MASK
            if op == 0:
                if fn == 0: r[rd] = (r[rt] << sh)&MASK
                elif fn == 2: r[rd] = r[rt] >> sh
                elif fn == 3: r[rd] = (signed(r[rt]) >> sh)&MASK
                elif fn == 4: r[rd] = (r[rt] << (r[rs]&31))&MASK
                elif fn == 8: pending = r[rs]
                elif fn == 0x12:
                    assert lo is not None
                    r[rd] = lo
                elif fn in (0x18, 0x19): lo = (r[rs]*r[rt])&MASK
                elif fn == 0x21: r[rd] = (r[rs]+r[rt])&MASK
                elif fn == 0x23: r[rd] = (r[rs]-r[rt])&MASK
                elif fn == 0x24: r[rd] = r[rs]&r[rt]
                elif fn == 0x25: r[rd] = r[rs]|r[rt]
                elif fn == 0x27: r[rd] = ~(r[rs]|r[rt])&MASK
                elif fn == 0x2A: r[rd] = int(signed(r[rs]) < signed(r[rt]))
                else: raise AssertionError(('unknown SPECIAL', hex(pc), fn))
            elif op in (2, 3):
                if op == 3: r[31] = pc+8
                pending = ((pc+4)&0xF0000000)|((w&0x3FFFFFF)<<2)
            elif op in (1, 4, 5, 6, 7, 20, 21, 22, 23):
                likely = op in (20,21,22,23) or (op == 1 and rt in (2,3))
                if op == 1:
                    assert rt in (0,1,2,3)
                    take = signed(r[rs]) >= 0 if rt&1 else signed(r[rs]) < 0
                elif op in (4,20): take = r[rs] == r[rt]
                elif op in (5,21): take = r[rs] != r[rt]
                elif op in (6,22): take = signed(r[rs]) <= 0
                else: take = signed(r[rs]) > 0
                if take: pending = pc+4+si*4
                elif likely: nxt += 4
            elif op == 9: r[rt] = a
            elif op == 10: r[rt] = int(signed(r[rs]) < si)
            elif op == 11: r[rt] = int(r[rs] < (si&MASK))
            elif op == 12: r[rt] = r[rs]&imm
            elif op == 13: r[rt] = r[rs]|imm
            elif op == 15: r[rt] = imm << 16
            elif op == 17:
                assert rs == 4, ('only mtc1 supported', hex(pc), rs)
                f[rd] = r[rt]
            elif op in (32,33,35,36,37):
                width = {32:1,33:2,35:4,36:1,37:2}[op]
                value = m.get(a,width)
                r[rt] = (signed(value,width*8)&MASK) if op in (32,33) else value
            elif op in (40,41,43): m.put(a,r[rt],{40:1,41:2,43:4}[op])
            elif op == 49: f[rt] = m.get(a)
            elif op == 57:
                assert f[rt] is not None
                m.put(a,f[rt])
            else: raise AssertionError(('unknown opcode', hex(pc), op))
            r[0] = 0
            assert old is None or pending is None, 'control transfer in delay slot'
            pc = old if old is not None else nxt
        raise AssertionError('step limit')


def verify(root):
    code, addresses, metadata, callgraph, root_calls = load(root)
    cases, coverage = 0, set()
    def fixture(count=4):
        m = Memory(); m.initialize(count)
        return m, Machine(code,m)
    def call(vm,name,args):
        nonlocal cases
        result = vm.call(addresses[name],args)
        cases += 1; coverage.update(vm.coverage)
        return result

    for raw in (0,2,0x12340002):
        i = signed(raw,16)
        m,vm = fixture()
        assert call(vm,'func_8008B3A0',[raw]) == TRANSFORM
        call(vm,'func_8008E06C',[raw,SOURCE])
        assert m.get(m.scene(i,0x3C)) == m.get(SOURCE)
        call(vm,'func_80090770',[raw,0x12345678])
        assert m.get(m.scene(i,0x14),2) == 0x5678
        for slot in (-1,0,2,3):
            before = [m.get(m.scene(i,0x20+4*j)) for j in range(4)]
            value = 0xDEADBEE0+slot
            call(vm,'func_8008D870',[raw,value,slot])
            assert [m.get(m.scene(i,0x20+4*j)) for j in range(4)] == [value if slot<0 or slot==j else before[j] for j in range(4)]

    for position,matrix in ((0,0),(SOURCE,0),(0,SOURCE),(SOURCE,SOURCE+32),(TRANSFORM+36,TRANSFORM),(TRANSFORM,TRANSFORM+28)):
        m,vm = fixture()
        expected = bytearray(m.regions[2][1])
        def get(a): return bytes(expected[a-TRANSFORM:a-TRANSFORM+4])
        def put(a,b): expected[a-TRANSFORM:a-TRANSFORM+4] = b
        if position:
            for off in (0,4,8): put(TRANSFORM+36+off,get(position+off))
        if matrix:
            for off in range(0,36,4): put(TRANSFORM+off,get(matrix+off))
        call(vm,'func_8008D6FC',[2,position,matrix])
        assert m.regions[2][1] == expected

    # Forward overlap distinguishes native sequential copy from memcpy snapshots.
    for delta in (0,4,-4,64):
        m,vm = fixture()
        src,dst = TRANSFORM+32,TRANSFORM+32+delta
        expected = bytearray(m.regions[2][1])
        for off in range(0,36,4):
            expected[dst-TRANSFORM+off:dst-TRANSFORM+off+4] = expected[src-TRANSFORM+off:src-TRANSFORM+off+4]
        call(vm,'math_utility',[src,dst])
        assert m.regions[2][1] == expected

    for count,hole,parent in ((0,None,-1),(3,None,-1),(3,1,-1),(3,None,0),(3,1,0)):
        m,vm = fixture(count)
        if hole is not None:
            # Remove the hole from its original sibling chain before allocating it.
            m.put(m.scene(hole,0x14),-1,2)
            m.put(m.scene(hole-1,0x18),hole+1 if hole+1<count else -1,2)
        index = count if hole is None else hole
        result = call(vm,'sign_extend_call',[0x12348001,TRANSFORM,-1 if parent<0 else parent,0x42000])
        assert result == index
        assert m.get(m.scene(index,8)) == TRANSFORM
        assert m.get(m.scene(index,0x14),2) == 0x8001
        assert m.get(m.scene(index)) == 0x42000
        assert m.get(m.scene(index,0xC)) == m.get(m.scene(index,0x10)) == 0x3F800000
        assert all(m.get(m.scene(index,off)) == 0 for off in range(0x1C,0x40,4))
        assert m.get(COUNT) == count+(hole is None)
        if parent >= 0: assert m.get(m.scene(parent,0x16),2) == index
        assert call(vm,'func_8008B3A0',[result]) == TRANSFORM
        # D6FC after allocation must mutate that exact retained transform.
        call(vm,'func_8008D6FC',[result,SOURCE,0])
        assert [m.get(TRANSFORM+36+off) for off in (0,4,8)] == [m.get(SOURCE+off) for off in (0,4,8)]

    for index in (0,1,3):
        m,vm = fixture(4)
        original_transform = bytes(m.regions[2][1])
        call(vm,'sound_call_minimal',[index])
        assert m.get(m.scene(index,0x14),2) == 0xFFFF
        assert m.get(m.scene(index,0x16),2) == m.get(m.scene(index,0x18),2) == 0xFFFF
        assert m.get(COUNT) == (3 if index==3 else 4)
        assert m.get(HEAD,2) == (1 if index==0 else 0)
        if index: assert m.get(m.scene(index-1,0x18),2) == (index+1 if index<3 else 0xFFFF)
        assert bytes(m.regions[2][1]) == original_transform

    for name in ('model_data_load','model_transform_setup'):
        for mask in (0,5,15):
            for mode in (0,1):
                m,vm = fixture()
                before = m.get(m.scene(2))
                call(vm,name,[2,mode,mask])
                expected = (before | (0x80000000 if mode==0 else mask<<8)) if name=='model_data_load' else before & ~(0x80000000 | mask<<8)
                assert m.get(m.scene(2)) == expected

    m,vm = fixture()
    try: call(vm,'func_8008B3A0',[123])
    except AssertionError as exc: assert 'unmapped' in str(exc)
    else: raise AssertionError('unmapped scene index accepted')
    bad = dict(code); bad[addresses['func_8008B3A0']] = MASK
    m,vm = fixture(); vm.code = bad
    try: call(vm,'func_8008B3A0',[0])
    except AssertionError as exc: assert 'unknown opcode' in str(exc)
    else: raise AssertionError('unknown opcode accepted')

    return dict(status='BOUNDED_ACTUAL_NATIVE_SERVICE_CHECKS',base_commit=BASE,
                cases=cases,native_instruction_coverage=len(coverage),native_targets=metadata,
                inspected_service_callgraph=callgraph,aa8c_direct_calls=root_calls,
                negative_controls=['unmapped scene read rejected','unknown opcode rejected'],
                verifier_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
                scope_limits=['not full AA8C execution','not source matching','not arbitrary invalid handles',
                              'no FCSR or FP arithmetic tests','no general caller or original-source-object proof'])


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--reference-root',required=True,type=Path)
    parser.add_argument('--output',type=Path)
    args = parser.parse_args()
    result = json.dumps(verify(args.reference_root.resolve()),indent=2)+'\n'
    if args.output: args.output.write_text(result)
    else: print(result,end='')
