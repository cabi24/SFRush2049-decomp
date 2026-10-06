#!/usr/bin/env python3
"""Source-bound full ELF/GNU nonmatch proof and bounded behavior replay."""
import argparse
import ctypes
import hashlib
import importlib.util
import itertools
import json
import os
from pathlib import Path
import random
import struct
import subprocess
import sys
import native

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
NAME = 'func_8039D300'
SOURCE = HERE/'candidate.c'
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
NATIVE_HASH = 'e0bf3d0731f618da18552bced27d621bec55be5f5cc274d1554c3a9656b046bc'
ANCHORS = {('D_%08X' % a): a for a in (native.SELECTOR, native.OUTPUT, *native.INDICES, *native.VECTORS)}
BINDINGS = {
 'func_803A0FD8': (0x803A0FD8, 3796, '661a6c6d552d31d34f6284c659cc8565e599f8b2fa734317a6817d9ba58281a1'),
 'func_803A1EAC': (0x803A1EAC, 7104, '9160fdb9d9c7e58ec66c11d7b33053193a894bb142df01995e5a39ca25f6eaf6'),
 'func_803A4134': (0x803A4134, 524, 'c79fb81fa3af695c6fa11fb403e7ddba62771bdd022442c8cc4410715c21f9eb')}

def sha(b): return hashlib.sha256(b).hexdigest()
def pack(words): return struct.pack('>%dI' % len(words), *words)
def shell(*args):
    p = subprocess.run([str(a) for a in args], capture_output=True, text=True)
    assert p.returncode == 0, (args, p.stdout, p.stderr)
    return p.stdout

def elf(path):
    """Independent complete ELF32 section/symbol/REL reader."""
    b = path.read_bytes()
    assert b[:6] == b'\x7fELF\x01\x02' and struct.unpack_from('>H', b, 18)[0] == 8
    at = struct.unpack_from('>I', b, 32)[0]
    stride, n, ni = struct.unpack_from('>HHH', b, 46)
    assert stride == 40
    rows = [struct.unpack_from('>10I', b, at+i*stride) for i in range(n)]
    nr = rows[ni]; names = b[nr[4]:nr[4]+nr[5]]
    sections, symbols, tables, relocs = {}, {}, {}, []
    for i, row in enumerate(rows):
        name = names[row[0]:].split(b'\0')[0].decode()
        raw = b[row[4]:row[4]+row[5]] if row[1] != 8 else b''
        sections[name] = (i, row, raw)
        if row[1] == 2:
            st = rows[row[6]]; strings = b[st[4]:st[4]+st[5]]; table = []
            for off in range(0, len(raw), 16):
                no, value, size, info, other, index = struct.unpack_from('>IIIBBH', raw, off)
                name = strings[no:].split(b'\0')[0].decode()
                table.append(name)
                if name: symbols[name] = (value, size, info & 15, index)
            tables[i] = table
    for row in rows:
        if row[1] in (4, 9):
            assert row[1] == 9, 'unsupported RELA'
            for off in range(row[4], row[4]+row[5], 8):
                loc, info = struct.unpack_from('>II', b, off)
                relocs.append((loc, info & 255, tables[row[6]][info >> 8], row[7]))
    return sections, symbols, relocs

def inspect(path, size, linked=False):
    sections, symbols, relocs = elf(path)
    ti, row, raw = sections['.text']
    functions = {n:s for n,s in symbols.items() if s[2] == 2 and s[3] not in (0, 0xfff1)}
    assert functions == {NAME:(native.ENTRY if linked else 0, size, 2, ti)}
    assert row[3] == (native.ENTRY if linked else 0)
    assert len(raw) == (size+15)&~15 and raw[size:] == bytes(len(raw)-size)
    for name, (_, r, body) in sections.items():
        if r[2]&2 and name not in ('.text', '.options', '.reginfo'):
            assert r[5] == 0, ('unexpected owned data', name)
    if linked:
        assert not relocs
        assert not any(s[3] == 0 for n,s in symbols.items() if s[2] != 4)
        for name, addr in ANCHORS.items(): assert symbols[name][0] == addr and symbols[name][3] == 0xfff1
    else:
        assert {n for n,s in symbols.items() if s[3] == 0} == set(ANCHORS)
        assert len(relocs) == 24
        assert {r[2] for r in relocs} == set(ANCHORS)
        assert all(o < size and o%4 == 0 and kind in (5, 6) and sec == ti for o,kind,sym,sec in relocs)
        assert sorted(s for o,k,s,t in relocs if k == 5) == sorted(ANCHORS)
        assert sorted(s for o,k,s,t in relocs if k == 6) == sorted(ANCHORS)
    return raw, relocs

def cases():
    rng = random.Random(0xD300)
    # All player/selector cells; all 3^5 row combinations independently;
    # ±zero in every one of five factors, all sign combinations, all four bars.
    ordinary = (0x3E800000, 0x3F000001, 0x3F800001, 0x40000001, 0xBF7FFFFF)
    for player in range(4):
        for selector in range(13):
            for row in range(3):
                yield player, selector, (row,)*5, [ordinary[i] for i in range(5) for _ in range(4)]
    for rows in itertools.product(range(3), repeat=5):
        yield sum(rows)%4, sum(rows)%13, rows, [ordinary[i] for i in range(5) for _ in range(4)]
    for at in range(5):
        for negative in range(2):
            for signs in range(32):
                v = [0x3F800000 | (((signs >> i)&1) << 31) for i in range(5) for _ in range(4)]
                v[at*4:at*4+4] = [negative << 31]*4
                yield signs%4, (signs+at)%13, (0,1,2,1,0), v
    for _ in range(512):
        v = [(rng.randrange(2)<<31) | (rng.randrange(119,136)<<23) | rng.getrandbits(23) for _ in range(20)]
        yield rng.randrange(4), rng.randrange(13), tuple(rng.randrange(3) for _ in range(5)), v

def check_host(lib, player, selector, rows, values, expected):
    crows = (ctypes.c_int*5)(*rows); cin = (ctypes.c_uint32*20)(*values); out = (ctypes.c_uint32*16)()
    lib.host_run(player, selector, crows, cin, out)
    assert list(out) == [native.load(expected, native.OUTPUT+i*4, 4) for i in range(16)]
    for i, a in enumerate(native.VECTORS):
        seen = list((ctypes.c_uint32*12).in_dll(lib, 'D_%08X'%a)); want = [0xA5A5A5A5]*12
        want[rows[i]*4:rows[i]*4+4] = values[i*4:i*4+4]
        assert seen == want, 'host changed vector input'
    for i, a in enumerate(native.INDICES):
        want = [0xA5]*52; want[13*player+selector] = rows[i]
        assert list((ctypes.c_ubyte*52).in_dll(lib, 'D_%08X'%a)) == want
    want = [0]*4; want[player] = selector
    assert list((ctypes.c_ubyte*4).in_dll(lib, 'D_803B9FD0')) == want

def behavioral(words, compiled, lib):
    digest = hashlib.sha256(); count = 0; coverage = set(); branch = set(); contract_failures = 0
    for player, selector, rows, values in cases():
        initial = native.state(player, selector, rows, values)
        expected, writes = native.oracle(initial, player, values)
        for label, program in [('native', words), ('compiled', compiled)]:
            run = native.run(program, initial, player, count)
            assert run['memory'] == expected and run['writes'] == writes, (label, count)
            assert run['greads'] == {4, 31} and not run['freads']
            if label == 'native':
                assert run['gwrites'] == {1,2,3,4,5,6,7,8,9,10,14,15,24,25}
                assert run['fwrites'] == {4,6,8,10}
                assert all(run['gpr'][i] == run['before'][i] for i in (11,12,13,*range(16,24),28,29,30,31))
                assert all(run['fpr'][i] == run['fbefore'][i] for i in range(32) if i not in (4,6,8,10))
                assert len([a for a,n in run['reads'] if a == native.SELECTOR+player]) == 5
                assert len(run['writes']) == 12 and len(run['multiplies']) == 16
                coverage |= run['coverage']; branch |= run['branches']
            else:
                assert len([a for a,n in run['reads'] if a == native.SELECTOR+player]) == 1
                assert all(run['gpr'][i] != run['before'][i] for i in (11,12,13))
                assert all(run['fpr'][i] != run['fbefore'][i] for i in (16,18))
                contract_failures += 1
        check_host(lib, player, selector, rows, values, expected)
        digest.update(pack([v for a,v in writes])); count += 1
    assert coverage == set(range(0,404,4)) and branch == {(388,True),(388,False)}
    return dict(cases=count, compiled_caller_contract_failures=contract_failures, trace_sha256=digest.hexdigest(), native_instructions_covered=len(coverage), branch_outcomes=len(branch))

def main():
    if not __debug__:
        raise RuntimeError('Verification assertions must remain enabled')
    ap = argparse.ArgumentParser(); ap.add_argument('--repo', type=Path, default=ROOT); ap.add_argument('--out', type=Path, default=HERE/'verification.json'); ap.add_argument('--check', action='store_true'); a = ap.parse_args()
    repo = a.repo.resolve(); build = ROOT/'build/runtime_a_float_product'; build.mkdir(parents=True, exist_ok=True)
    sys.path.insert(0, str(repo/'tools/cloud'))
    spec = importlib.util.spec_from_file_location('score', repo/'tools/cloud/score.py'); score = importlib.util.module_from_spec(spec); sys.modules['score'] = score; spec.loader.exec_module(score)
    score.ASM_DIR = repo/'asm/us/ovl_a'; manifest = score.target_manifest(); targets = score.targets(); words = targets[NAME]
    assert len(words) == 101 and sha(pack(words)) == NATIVE_HASH
    ext = json.loads(score.verified_bytes(score.ASM_DIR/'extents.json', manifest))
    assert ext['image'] == 'A' and ext['base'] == '0x8038A400'
    assert next(x for x in ext['functions'] if x['name'] == NAME) == dict(name=NAME,address='0x8039D300',size=404,evidence=['jal'])
    addresses = score.image_symbols()
    assert addresses[NAME] == native.ENTRY
    for name, addr in ANCHORS.items(): assert addresses.get(name, score.address_named(name)) == addr
    for name,(addr,size,digest) in BINDINGS.items():
        assert addresses[name] == addr and len(targets[name])*4 == size and sha(pack(targets[name])) == digest
    sites = []
    for name, w in targets.items():
        for i, x in enumerate(w):
            if x >> 26 == 3 and ((x&0x3ffffff)<<2 | ((addresses[name]+4)&0xf0000000)) == native.ENTRY:
                sites.append((name, hex(addresses[name]+4*i)))
    assert sorted(sites) == [('func_803A0FD8','0x803a10ec'),('func_803A1EAC','0x803a3880')]
    script = build/'whole.ld'
    script.write_text('SECTIONS { .text 0x8039D300 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'+''.join('%s = 0x%X;\n'%(n,v) for n,v in ANCHORS.items()))
    proofs = {}; code = None
    for opt, size, differ, extra in [('O1',516,100,28),('O2',384,99,0),('O3',384,99,0)]:
        obj = build/'candidate.o'; linked = build/'candidate.elf'
        score.compile_single(SOURCE, FLAGS.replace('O2',opt), obj)
        cmp = score.compare(obj, NAME, show=0)
        assert (cmp.differing,cmp.total,cmp.extra_words) == (differ,101,extra)
        assert not cmp.accepted() and not any((cmp.unresolved,cmp.unverified,cmp.errors))
        raw, relocs = inspect(obj, size)
        resolved, masks, unknown, unverified, errors = score.relocate(obj, score.text_words(obj), 0, len(raw), addresses)
        assert not any((masks,unknown,unverified,errors))
        shell('mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj)
        linkedraw, _ = inspect(linked,size,True)
        assert linkedraw == pack(resolved)
        proofs[opt] = dict(comparison=cmp.__dict__, function_bytes=size, alignment_bytes=len(raw)-size, owned_data_bytes=0, relocations=relocs, relocated_text_sha256=sha(linkedraw), full_gnu_agreement=True)
        if opt == 'O2': code = resolved[:size//4]
    host = build/'host.so'
    shell('gcc','-std=c89','-O2','-fno-fast-math','-ffp-contract=off','-shared','-fPIC','-fsanitize=undefined,bounds','-fno-sanitize-recover=all',HERE/'host.c','-o',host)
    lib = ctypes.CDLL(str(host)); lib.host_run.argtypes = [ctypes.c_int,ctypes.c_int,ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)]
    behavior = behavioral(words, code, lib)
    refusal = []
    for bad in (0x7f800000,0xff800000,0x7fc00000,0x7f800001,0x7fbfffff,1,0x80000001):
        try: native.mul(bad,0x3f800000)
        except ValueError: refusal.append(hex(bad))
        else: raise AssertionError('exceptional arithmetic accepted')
    # Source mutants are independently compiled and must fail behavior or exact store trace.
    mutations = {'reassociate':'(first[component] * second[component]) * third[component]|first[component] * (second[component] * third[component])',
                 'drop_fifth':'dest[component] *= fifth[component];|dest[component] *= fourth[component];',
                 'coalesce_stores':'dest[component] = (first[component] * second[component]) * third[component];\n        dest[component] *= fourth[component];\n        dest[component] *= fifth[component];|dest[component] = (((first[component] * second[component]) * third[component]) * fourth[component]) * fifth[component];'}
    controls = {}
    for name, change in mutations.items():
        old,new = change.split('|'); assert old in SOURCE.read_text()
        source = build/'variant.c'; source.write_text(SOURCE.read_text().replace(old,new)); obj = build/'candidate.o'
        score.compile_single(source,FLAGS,obj)
        cmp = score.compare(obj,NAME,show=0); assert not cmp.accepted()
        w = score.text_words(obj); rel, mask, un, uv, er = score.relocate(obj,w,0,len(w)*4,addresses); assert not any((mask,un,uv,er))
        rejected = False
        for player,selector,rows,values in cases():
            initial = native.state(player,selector,rows,values); expected,writes = native.oracle(initial,player,values)
            result = native.run(rel,initial,player)
            if result['memory'] != expected or result['writes'] != writes:
                rejected = True; break
        assert rejected, ('undetected source mutant',name)
        controls[name] = dict(strict_match=False, bounded_behavior_or_store_trace_rejected=True)
    receipt = dict(status='NONMATCH; bounded research only',image='A',address=hex(native.ENTRY),end=hex(native.ENTRY+404),native_bytes=404,native_sha256=NATIVE_HASH,
        source_sha256=sha(SOURCE.read_bytes()),harness_sha256=sha((HERE/'host.c').read_bytes()),model_sha256=sha((HERE/'native.py').read_bytes()), verifier_sha256=sha(Path(__file__).read_bytes()),
        proofs=proofs,behavior=behavior,exceptional_inputs_refused=refusal,negative_controls=controls,
        anchors={n:hex(v) for n,v in ANCHORS.items()},native_bindings=BINDINGS,direct_call_sites=sites,
        tools={p:sha((score.IDO/p).read_bytes()) for p in ('cc','cfe','uopt','ugen','as1')},scorer_sha256=sha((repo/'tools/cloud/score.py').read_bytes()),
        protected_manifest_sha256=sha((score.ASM_DIR/'SHA256SUMS').read_bytes()),
        limits=['Stable, disjoint, synthetic valid backing: player0..3, selector0..12, five rows0..2, four floats each; not a proof of every game reachable index',
        'Binary32 normal/zero inputs and normal/zero intermediate products under round-to-nearest ties-even only; no hardware FCSR, trap, subnormal or NaN payload equivalence',
        'NaN/infinity/subnormal probes are explicit domain refusals, not arithmetic compatibility claims',
        'No genuine caller group compiled; native preserves t3/t4/t5 and f16/f18, candidate clobbers them',
        'No image/compression/ROM gates; no coverage credit; no broad suite'])
    receipt = json.loads(json.dumps(receipt))
    if a.check: assert receipt == json.loads(a.out.read_text()), 'portable receipt differs'
    else: a.out.write_text(json.dumps(receipt,indent=2)+'\n')
    print(json.dumps({k:receipt[k] for k in ('status','behavior','negative_controls')},indent=2))
if __name__ == '__main__': main()
