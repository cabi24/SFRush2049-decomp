#!/usr/bin/env python3
"""Bounded source-boundary proof. No native words or objects are published."""
import contextlib
import dataclasses
import hashlib
import io
import json
import os
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
NAMES = ['func_8008E06C', 'func_80092BC8', 'func_80092BF4']
SOURCE_PATHS = ['src/blob/func_8008E06C.c', 'src/blob/func_80092BC8.c',
                'cloud/work/tiny_A44/func_80092BF4.c', 'src/blob/sfx_stop.c']
MASK = 0xffffffff

# Only this packet's selected entries and consumed relocation symbols are pinned.
# Unrelated manifest updates are intentionally allowed.
ADDRESS_ANCHORS = {
    'D_8011B438': 0x8011b438,
    'D_8012E700': 0x8012e700,
    'D_8012E73C': 0x8012e73c,
    'D_80139320': 0x80139320,
    'D_80139334': 0x80139334,
    'D_80140BDC': 0x80140bdc,
    'func_8008D870': 0x8008d870,
    'func_8008E06C': 0x8008e06c,
    'func_80092BC8': 0x80092bc8,
    'func_80092BF4': 0x80092bf4,
    'func_800B24EC': 0x800b24ec,
    'model_data_load': 0x8008ae8c,
    'model_transform_setup': 0x8008b0d8,
    'sfx_stop': 0x800b2658,
}


def verify_address_anchors():
    table = score.image_symbols()
    for name, expected in ADDRESS_ANCHORS.items():
        assert table.get(name) == expected, 'native address drift: '+name
    return {name: hex(table[name]) for name in sorted(ADDRESS_ANCHORS)}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def functions(obj):
    data, secs = score._elf(obj)
    ti = score._text_index(secs)
    return {s['name']: s for i, sec in enumerate(secs) if sec['type'] == 2
            for s in score._symbol_table(data, secs, i)
            if s['type'] == 2 and s['section'] == ti}


def linked(obj, work):
    data, secs = score._elf(obj)
    table = score.image_symbols()
    external = {s['name'] for i, sec in enumerate(secs) if sec['type'] == 2
                for s in score._symbol_table(data, secs, i)
                if s['section'] == 0 and s['name']}
    assert external <= set(ADDRESS_ANCHORS), 'unbound relocation symbol'
    verify_address_anchors()
    ld = work / 'proof.ld'
    ld.write_text('SECTIONS { .text 0x80000000 : { *(.text) } }\n' +
                  ''.join('%s = 0x%x;\n' % (n, table.get(n, score.address_named(n)))
                          for n in sorted(external)))
    out = work / 'linked.elf'
    subprocess.run(['mips-linux-gnu-ld', '-EB', '-T', str(ld), '-o', str(out), str(obj)],
                   capture_output=True, check=True)
    return score.text_words(out)


def inspect(obj, names, work):
    fs, raw = functions(obj), score.text_words(obj)
    gnu = linked(obj, work)
    records, bodies = {}, {}
    _, secs = score._elf(obj)
    assert all(s['size'] == 0 for s in secs if s['name'] in ('.data', '.bss', '.rodata', '.sdata', '.sbss'))
    last = 0
    for n in names:
        f = fs[n]
        start, size = f['value'], f['size']
        end = start + size
        last = max(last, end)
        got, masks, unresolved, unverified, errors = score.relocate(obj, raw, start, end, score.image_symbols())
        assert not masks and not unresolved and not unverified and not errors
        body = got[start//4:end//4]
        assert gnu[start//4:end//4] == body
        want = score.targets()[n]
        diff = [4*i for i in range(max(len(body), len(want)))
                if i >= len(body) or i >= len(want) or body[i] != want[i]]
        with contextlib.redirect_stdout(io.StringIO()):
            strict = dataclasses.asdict(score.compare(obj, n, show=0))
        assert strict['differing'] == len(diff)
        assert strict['extra_words'] == 0
        assert not strict['errors'] and not strict['unresolved'] and not strict['unverified']
        records[n] = {'status': 'MATCH' if not diff else 'NONMATCH',
                      'target_bytes': 4*len(want), 'elf_bytes': size,
                      'native_start':hex(ADDRESS_ANCHORS[n]),
                      'native_end_exclusive':hex(ADDRESS_ANCHORS[n]+4*len(want)),
                      'strict': strict, 'differing_byte_offsets': diff,
                      'target_sha256': sha(struct.pack('>%dI' % len(want), *want)),
                      'linked_body_sha256': sha(struct.pack('>%dI' % len(body), *body)),
                      'all_relocations_independently_gnu_linked': True,
                      'owned_data_bytes': 0}
        bodies[n] = body
    padding = raw[last//4:]
    assert not any(padding)
    return {'functions': records, 'zero_alignment_bytes': 4*len(padding)}, bodies


def s32(x):
    return x - 0x100000000 if x & 0x80000000 else x


def run(words, args, memory):
    r = [((0x9e3779b9 * (i+1)) ^ 0x53127bcd) & MASK for i in range(32)]
    r[0], r[29], r[31] = 0, 0x70001000, 0x7fffffff
    for i, v in enumerate(args):
        r[4+i] = v & MASK
    saved = r.copy()
    mem = dict(memory)
    lo, stop, visited = 0, False, set()
    for pc, w in enumerate(words):
        visited.add(pc)
        op, rs, rt, rd, sh, fn = w >> 26, (w >> 21)&31, (w >> 16)&31, (w >> 11)&31, (w >> 6)&31, w&63
        imm = w & 65535
        simm = imm-65536 if imm&32768 else imm
        was_stop = stop
        if op == 0:
            if fn == 0: r[rd] = (r[rt] << sh)&MASK
            elif fn == 3: r[rd] = (s32(r[rt]) >> sh)&MASK
            elif fn == 33: r[rd] = (r[rs]+r[rt])&MASK
            elif fn == 35: r[rd] = (r[rs]-r[rt])&MASK
            elif fn == 25: lo = (r[rs]*r[rt])&MASK
            elif fn == 18: r[rd] = lo
            elif fn == 8:
                assert rs == 31 and r[31] == saved[31]
                stop = True
            else: raise AssertionError('unsupported native operation')
        elif op == 9: r[rt] = (r[rs]+simm)&MASK
        elif op == 15: r[rt] = imm << 16
        elif op in (35, 43):
            addr = (r[rs]+simm)&MASK
            assert addr % 4 == 0 and addr in mem, 'unmapped word'
            if op == 35: r[rt] = mem[addr]
            else: mem[addr] = r[rt]
        else: raise AssertionError('unsupported native opcode')
        r[0] = 0
        if was_stop:
            assert pc == len(words)-1, 'trailing executable words'
            assert r[16:24] == saved[16:24] and r[28:32] == saved[28:32]
            return mem, visited
    raise AssertionError('truncated native return')


def native_checks(bodies):
    targets = score.targets()
    table = score.image_symbols()
    base, keys, stack = table['D_8012E700'], table['D_80139334'], 0x70001000
    count, visits = 0, {n: set() for n in NAMES}
    # Exhaust the halfword index, including signed negatives, for both helper bodies.
    for half in range(65536):
        signed = half-65536 if half&32768 else half
        for n, offset in zip(NAMES[:2], [60, 64]):
            dst = (base + signed*68 + offset)&MASK
            mem = {dst: 0xabadcafe, stack: 0x91827364, 0x60000000: half ^ 0x12345678}
            raw = 0xa5760000 | half
            want = dict(mem); want[dst] = mem[0x60000000]; want[stack] = raw
            for code in [targets[n], bodies[n]]:
                result, hit = run(code, [raw, 0x60000000], mem)
                assert result == want
                visits[n].update(hit)
                count += 1
    # Bounded real array domain; high input bits demonstrate both signed-half conversions.
    fixtures = 0
    for key in range(6):
        for slot in range(16):
            for high in [0, 0x12340000, 0x7fff0000, 0x80000000, 0xffff0000]:
                for mode in range(6):
                    mem = {base+i*4: (0x87654321+i*193)&MASK for i in range(16*17)}
                    mem.update({keys-20+i*4: (0x76543210+i*157)&MASK for i in range(6*16)})
                    mem.update({stack: 0xabcdef01, 0x60000000: 0x10203040, 0x60000004: 0x50607080})
                    mem[keys+key*64] = high | slot
                    dst = base + slot*68
                    choices = [(0x60000000,0x60000004),(dst+64,dst+60),(dst+60,dst+60),
                               (keys+key*64,dst+60),(dst+64,keys+key*64),(0x60000000,0x60000000)]
                    first, second = choices[mode]
                    rawkey = high | key
                    want = dict(mem)
                    want[stack] = rawkey
                    want[dst+60] = want[first]
                    want[dst+64] = want[second]
                    for code in [targets[NAMES[2]], bodies[NAMES[2]]]:
                        result, hit = run(code, [rawkey, first, second], mem)
                        assert result == want
                        visits[NAMES[2]].update(hit)
                        count += 1
                    fixtures += 1
    # Invalid decoding, truncation, and unmapped accesses must fail closed.
    controls = 0
    for code, mem in [([0xffffffff], {}), (targets[NAMES[0]][:-1], {stack:0,0x60000000:1,base+60:0}),
                      (targets[NAMES[0]], {stack:0})]:
        try: run(code, [0,0x60000000], mem)
        except AssertionError: controls += 1
        else: raise AssertionError('negative native control accepted')
    assert controls == 3
    return {'executions': count, 'helper_signed_halfword_values': 65536,
            'caller_alias_fixtures': fixtures, 'decoder_negative_controls': controls,
            'native_instruction_coverage': {n:[len(visits[n]),len(targets[n])] for n in NAMES}}



def host_checks(work):
    base = ['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror', '-O2',
            '-fstrict-aliasing', '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all']
    exe = work/'host'
    subprocess.run(base + [str(HERE/'host.c'), '-o', str(exe)], check=True, capture_output=True)
    run = subprocess.run([str(exe)], check=True, capture_output=True, text=True)
    assert run.stdout.strip() == 'typed_alias_cases=2880'
    src = (HERE/'group.c').read_text()
    mutations = {
        'wrong_field': src.replace('D_8012E700[slot].first = *value;', 'D_8012E700[slot].second = *value;'),
        'early_second_read': src.replace('    func_8008E06C(slot, first);',
            '    s32 captured = *second;\n    func_8008E06C(slot, first);').replace(
            'func_80092BC8(slot, second);', 'func_80092BC8(slot, &captured);')}
    for name, mutant in mutations.items():
        file = work/(name+'.c'); file.write_text(mutant)
        subprocess.run(base + ['-DCANDIDATE_SOURCE="'+str(file)+'"',str(HERE/'host.c'),'-o',str(exe)],
                       check=True,capture_output=True)
        rejected = subprocess.run([str(exe)],capture_output=True)
        assert rejected.returncode != 0, name+' was accepted'
    return {'c89_strict_aliasing_ubsan_bounds_cases':2880, 'layout_assertions':7,
            'compiled_semantic_mutants_rejected':sorted(mutations)}


def main():
    anchors = verify_address_anchors()
    records, bodies = {}, None
    with tempfile.TemporaryDirectory(prefix='slot-proof-') as td:
        work = Path(td)
        group = json.loads((HERE/'group.json').read_text())
        assert group == {'files':['group.c'], 'flags':FLAGS, 'members':NAMES, 'keep':NAMES, 'claims':[]}
        source = (HERE/'group.c').read_text()
        for label in ['original_helpers', 'normalized_helpers', 'kept_inline_control']:
            d = work/label; d.mkdir()
            if label == 'original_helpers':
                group['files'] = ['first.c','second.c','caller.c']
                (d/'first.c').write_bytes((ROOT/SOURCE_PATHS[0]).read_bytes())
                (d/'second.c').write_bytes((ROOT/SOURCE_PATHS[1]).read_bytes())
                start = source.index('void func_8008E06C')
                end = source.index('void func_80092BF4')
                caller = source[:start] + 'void func_8008E06C(s16, s32 *);\nvoid func_80092BC8(s16, s32 *);\n' + source[end:]
                (d/'caller.c').write_text(caller)
            else:
                group['files']=['group.c']
                candidate = source
                if label == 'kept_inline_control':
                    for name in NAMES[:2]: candidate = candidate.replace('void '+name,'__inline void '+name)
                (d/'group.c').write_text(candidate)
            (d/'group.json').write_text(json.dumps(group))
            obj = d/'proof.o'
            score.compile_group(d,obj)
            record, built = inspect(obj,NAMES,d)
            assert all(record['functions'][n]['status'] == 'MATCH' for n in NAMES[:2])
            assert record['functions'][NAMES[2]]['strict']['differing'] == 22
            records[label]=record
            if label == 'normalized_helpers': bodies=built
        for level in ['O2','O3']:
            d=work/level;d.mkdir();obj=d/'baseline.o'
            score.compile_single(ROOT/SOURCE_PATHS[2],FLAGS.replace('O3',level),obj)
            records['direct_'+level]=inspect(obj,[NAMES[2]],d)[0]
        d=work/'record_witness';d.mkdir();obj=d/'witness.o'
        score.compile_single(ROOT/SOURCE_PATHS[3],FLAGS,obj)
        records['unchanged_record_consumer']=inspect(obj,['sfx_stop'],d)[0]
        assert records['unchanged_record_consumer']['functions']['sfx_stop']['status'] == 'MATCH'
        native = native_checks(bodies)
        host = host_checks(work)
    addresses = score.image_symbols()
    callers = {n:[] for n in NAMES}
    for caller, words in score.targets().items():
        base = addresses[caller]
        for w in words:
            if w >> 26 not in (2,3): continue
            dst = (base & 0xf0000000) | ((w & 0x3ffffff) << 2)
            for n in NAMES:
                if dst == addresses[n] and caller != n: callers[n].append(caller)
    assert not any(callers.values())
    report={'status':'NONMATCH; no new matching claims', 'flags':FLAGS,
            'source_sha256':sha((HERE/'group.c').read_bytes()),
            'input_hashes':{p:sha((ROOT/p).read_bytes()) for p in SOURCE_PATHS},
            'experiments':records,'native':native,'host':host,
            'native_address_anchors':anchors,'direct_native_callers':callers,'claims':[], 'accepted_byte_gain':0,
            'toolchain_sha256':{n:sha((score.IDO/n).read_bytes()) for n in ['cc','cfe','uld','umerge','uopt','ugen','as1']},
            'group_spec_sha256':sha((HERE/'group.json').read_bytes()),'verifier_sha256':sha(Path(__file__).read_bytes()),'host_source_sha256':sha((HERE/'host.c').read_bytes())}
    print(json.dumps(report,indent=2,sort_keys=True))

if __name__ == '__main__':
    main()
