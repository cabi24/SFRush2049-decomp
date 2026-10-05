#!/usr/bin/env python3
"""Replay bounded donor controls, complete ELF/GNU proof and native/host checks."""
import argparse
import ctypes
from dataclasses import asdict
import hashlib
import importlib.util
import itertools
import json
from pathlib import Path
import random
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
_spec = importlib.util.spec_from_file_location('quadtree_native', HERE / 'native.py')
native = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(native)
FN = 'func_800AC9BC'
WRAPPER = 'handbrake_apply'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
SOURCE = HERE / 'candidate.c'
ACCEPTED = ROOT / 'src/blob/groups/func_800AD4C8/group.c'
DONOR_COMMIT = '845329d7b36f5a384c5625ed9a0aef584ab46139'


def sha(data): return hashlib.sha256(data).hexdigest()


def symbol(obj, name):
    data, secs = score._elf(obj)
    ti = score._text_index(secs)
    entries = [s for i, sec in enumerate(secs) if sec['type'] == 2
               for s in score._symbol_table(data, secs, i)
               if s['name'] == name and s['type'] == 2 and s['section'] == ti]
    assert len(entries) == 1
    return entries[0]


def proof(obj, work, name=FN):
    comparison = score.compare(obj, name, show=0)
    data, secs = score._elf(obj)
    ti, fn = score._text_index(secs), symbol(obj, name)
    start, end = fn['value'], fn['value'] + fn['size']
    assert fn['size'] > 0 and fn['size'] % 4 == 0
    allwords = score.text_words(obj)
    resolved, masks, unknown, unchecked, errors = score.relocate(obj, allwords, start, end, score.image_symbols())
    assert not (masks or unknown or unchecked or errors)
    own_bytes = sum(s['size'] for s in secs if s['name'] in ('.rodata', '.data', '.bss', '.sdata', '.sbss', '.lit4', '.lit8'))
    assert own_bytes == 0
    relocations = []
    for sec in secs:
        if sec['type'] == 9 and sec['info'] == ti:
            syms = score._symbol_table(data, secs, sec['link'])
            for at in range(sec['off'], sec['off'] + sec['size'], 8):
                offset, info = struct.unpack_from('>II', data, at)
                if start <= offset < end:
                    relocations.append(dict(offset=offset-start, type=info & 255, symbol=syms[info >> 8]['name']))
    directory = work / (obj.parent.name + '_' + obj.stem + '_' + name)
    directory.mkdir()
    link = directory / 'link.ld'
    # The target is first in all fixed controls; the linker also resolves the real wrapper's call.
    link.write_text('SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } }\nD_80124EEC = 0x80124EEC;\n' % (native.BASE - symbol(obj, FN)['value']))
    linked = directory / 'linked.elf'
    subprocess.run(['mips-linux-gnu-ld', '-EB', '-T', str(link), '-o', str(linked), str(obj)], check=True, capture_output=True)
    ldata, lsecs = score._elf(linked)
    ltext, lfn = lsecs[score._text_index(lsecs)], symbol(linked, name)
    offset = fn['value']
    body = ldata[ltext['off'] + offset:ltext['off'] + offset + lfn['size']]
    nm = subprocess.check_output(['mips-linux-gnu-nm', '-S', '--defined-only', str(linked)], text=True)
    nmrow = next(line.split() for line in nm.splitlines() if line.split()[-1] == name)
    assert int(nmrow[0],16) == lfn['value'] and int(nmrow[1],16) == lfn['size'] == fn['size']
    readelf = subprocess.check_output(['mips-linux-gnu-readelf','-sW',str(obj)],text=True)
    row = next(line.split() for line in readelf.splitlines() if line.split() and line.split()[-1] == name)
    assert row[3] == 'FUNC' and int(row[2]) == fn['size'] and int(row[1],16) == start
    dumped = directory / 'text.bin'
    subprocess.run(['mips-linux-gnu-objcopy','--dump-section','.text='+str(dumped),str(linked)],check=True,capture_output=True)
    assert dumped.read_bytes() == ldata[ltext['off']:ltext['off']+ltext['size']]
    assert dumped.read_bytes()[offset:offset+lfn['size']] == body
    words = list(struct.unpack('>%dI' % (len(body)//4), body))
    assert words == resolved[start//4:end//4]
    target = score.targets()[name]
    true_bad = sum(a != b for a, b in itertools.zip_longest(target, words))
    result = dict(asdict(comparison), verdict=comparison.summary(), symbol_bytes=fn['size'],
                  native_bytes=len(target)*4, full_extent_differing=true_bad,
                  full_extent_extra_words=max(0, len(words)-len(target)),
                  full_extent_missing_words=max(0, len(target)-len(words)),
                  relocations=relocations, gnu_full_body_equals_project_relocation=True,
                  linked_body_sha256=sha(body), own_data_bytes=own_bytes,
                  executable_prefix_bytes=start)
    provenance = dict(object_sha256=sha(data), linked_elf_sha256=sha(ldata))
    return result, words, provenance


def wrapper_parts():
    text = ACCEPTED.read_text()
    header = ('typedef signed short s16;\ntypedef unsigned short u16;\n'
              'typedef int s32;\ntypedef unsigned char u8;\n' +
              text[text.index('typedef struct QNode {'):text.index('void *func_800AC9BC(QNode')])
    archive = text[text.index('void *func_800AC9BC(QNode'):text.index('void *handbrake_apply(QNode')]
    wrapper = text[text.index('void *handbrake_apply(QNode'):text.index('\nfloat fabsf(float);')]
    return header, archive, wrapper


def context_source(source):
    fn = 'void *func_800AC9BC' + source.split('QNode *func_800AC9BC', 1)[1]
    for a, b in [('min_x', 'x0'), ('max_x', 'x1'), ('min_y', 'y0'), ('max_y', 'y1'), ('child_mask', 'mask')]:
        fn = fn.replace(a, b)
    return fn


def compiler_proofs(work):
    rows, provenance = {}, {}
    obj = work / 'candidate.o'
    score.compile_single(SOURCE, FLAGS, obj)
    rows['candidate'], words, provenance['candidate'] = proof(obj, work)
    assert rows['candidate']['differing'] == 19
    assert rows['candidate']['symbol_bytes'] == rows['candidate']['native_bytes'] == 224
    assert rows['candidate']['extra_words'] == 0
    for source in sorted((HERE / 'controls').glob('*.c')):
        obj = work / (source.stem + '.o')
        score.compile_single(source, FLAGS, obj)
        rows[source.stem], _, provenance[source.stem] = proof(obj, work)
    for name in ['donor_recursive', 'donor_iterative']:
        obj = work / (name + '_o2.o')
        score.compile_single(HERE / 'controls' / (name + '.c'), FLAGS.replace('-O3', '-O2'), obj)
        rows[name+'_o2'], _, provenance[name+'_o2'] = proof(obj, work)
    header, archive, wrapper = wrapper_parts()
    groups = {}
    for name, source in [('archive', archive), ('donor_recursive', context_source((HERE/'controls/donor_recursive.c').read_text())),
                         ('candidate', context_source(SOURCE.read_text()))]:
        directory = work / name
        directory.mkdir()
        (directory/'group.c').write_text(header+source+wrapper)
        (directory/'group.json').write_text(json.dumps({'files':['group.c'], 'flags':FLAGS,
            'keep':[FN, WRAPPER], 'members':[FN], 'context':[WRAPPER]}))
        obj = directory/'group.o'
        score.compile_group(directory, obj)
        groups[name] = {}
        for fn in [FN, WRAPPER]:
            groups[name][fn], _, provenance[name+'_'+fn] = proof(obj, work, fn)
        assert groups[name][WRAPPER]['verdict'] == 'MATCH'
        assert groups[name][WRAPPER]['symbol_bytes'] == 216
    assert groups['archive'][FN]['differing'] == 52
    assert groups['candidate'][FN]['differing'] == 19
    return rows, groups, words, provenance


def make_case(rows, start, x, y, initial=-12345):
    data = b''.join(struct.pack('>hBBhhhh4H', -1, (i*31+17)&255, mask, *bounds, *child)
                    for i, (bounds, mask, child) in enumerate(rows))
    return dict(records=data, start=start, x=x, y=y, initial=initial)


def cases():
    bounds = [(-8, 6, -4, 2), (-32768, 32767, -32768, 32767), (-7, -2, -5, -2), (1, 6, 1, 4)]
    coords = [-32768, -9, -5, -3, -1, 0, 1, 2, 4, 7, 32767]
    for b in bounds:
        for mask in range(16):
            for x, y in itertools.product(coords, repeat=2):
                yield make_case([(b, mask, (0,0,0,0))], 0, x, y)
    # High mask bits are irrelevant. All low halfwords plus noisy upper bits are sampled at edges.
    for x in [-2147483648, -65537, -32769, 32768, 65535, 65536, 2147483647]:
        for y in [-2147483648, -32769, 32768, 65536, 2147483647]:
            yield make_case([(bounds[1], 0xF0, (0,0,0,0))], 0, x, y)
    rng = random.Random(0xAC9BC)
    for _ in range(1600):
        n = rng.choice([2,3,4,8,16,32])
        rows = []
        for i in range(n):
            b = tuple(rng.randint(-32768,32767) for _ in range(4))
            children = tuple(rng.choice([0] + list(range(i+1,n))) for _ in range(4))
            rows.append((b, rng.randrange(256), children))
        yield make_case(rows, rng.randrange(n), rng.randrange(-2147483648,2147483648), rng.randrange(-2147483648,2147483648), rng.randrange(-32768,32768))
    for n in [64,128,512]:
        rows = [((-10,10,-10,10), 15 if i<n-1 else 0, (i+1,)*4 if i<n-1 else (0,)*4) for i in range(n)]
        for x,y in [(-1,-1),(1,1),(-1,1),(1,-1)]: yield make_case(rows,0,x,y)


def oracle(case):
    x, y = native.signed(case['x'],16), native.signed(case['y'],16)
    index, output = case['start'], case['initial']
    def half(v): return v//2 if v>=0 else -((-v)//2)
    for _ in range(513):
        _, _, mask, x0, x1, y0, y1, *children = struct.unpack_from('>hBBhhhh4H',case['records'],index*20)
        q = int(x >= half(x0+x1)) + 2*int(y < half(y0+y1))
        if not (mask & (1<<q)): return index,q
        index = children[q]
        if index == 0: return -1,output
    raise AssertionError('nonfinite fixture')


def host(work, source=SOURCE, tag='host'):
    directory=work/tag;directory.mkdir()
    shutil.copyfile(source,directory/'candidate.c');shutil.copyfile(HERE/'host.c',directory/'host.c')
    lib=directory/'host.so'
    subprocess.run(['cc','-std=c89','-Wall','-Wextra','-Werror','-O2','-shared','-fPIC','-fsanitize=undefined',
                    '-fno-sanitize-recover=all',str(directory/'host.c'),'-o',str(lib)],check=True,capture_output=True)
    dll=ctypes.CDLL(str(lib));assert dll.host_layout()==1
    fn=dll.host_run
    fn.argtypes=[ctypes.c_void_p,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_short,ctypes.POINTER(ctypes.c_short)]
    def run(case):
        buf=ctypes.create_string_buffer(case['records']);out=ctypes.c_short()
        result=fn(buf,len(case['records'])//20,case['start'],case['x'],case['y'],case['initial'],ctypes.byref(out))
        return result,out.value
    return run


def behavior(work, linked):
    host_run=host(work)
    words=score.targets()[FN]
    covered,branches=set(),set()
    linked_covered,linked_branches=set(),set()
    case_list=list(cases())
    for case in case_list:
        wanted=oracle(case)
        for program, cov, br in [(words,covered,branches),(linked,linked_covered,linked_branches)]:
            machine=native.Machine(program,**case)
            assert machine.run()==wanted, 'native-oracle discrepancy'
            expected_writes=2+int(wanted[0]!=-1)
            assert len(machine.writes)==expected_writes
            cov.update(machine.coverage);br.update(machine.branches)
        assert host_run(case)==wanted, 'host-oracle discrepancy'
    assert covered==set(range(0,224,4))
    assert linked_covered==set(range(0,224,4))
    for program, outcomes in [(words,branches),(linked,linked_branches)]:
        expected=set()
        for i,w in enumerate(program):
            op,rs,rt=w>>26,w>>21&31,w>>16&31
            if op in (1,4,5,0x14,0x15):
                states = [True] if op in (4,0x14) and rs==rt else [False] if op in (5,0x15) and rs==rt else [False,True]
                expected.update((4*i,state) for state in states)
        assert outcomes==expected, 'incomplete branch outcomes'
    mutants={
        'floor_negative_midpoint':('mid_x / 2','mid_x >> 1'),
        'wrong_y_quadrant':('y < mid_y / 2','y >= mid_y / 2'),
        'wrong_child_mask':('node->child_mask & (1 << q)','node->child_mask & (1 << (q ^ 1))'),
        'wrong_null_output':('if (node == D_80124EEC)\n                return 0;','if (node == D_80124EEC) {\n                *quadrant = q;\n                return 0;\n            }'),
    }
    rejection={}
    for name,(old,new) in mutants.items():
        source=SOURCE.read_text();assert old in source
        path=work/(name+'.c');path.write_text(source.replace(old,new))
        fn=host(work,path,name)
        witness=next((i for i,c in enumerate(case_list) if fn(c)!=oracle(c)),None)
        assert witness is not None, name
        rejection[name]=witness
    adverse={}
    bad=list(words);bad[0]=0xFFFFFFFF
    try: native.Machine(bad,**case_list[0]).run()
    except AssertionError: adverse['unknown_instruction']=True
    else: raise AssertionError('unknown instruction accepted')
    bad=list(words);site=next(i for i,w in enumerate(bad) if w>>26==0x2B);bad[site]=(bad[site]&0xFFFF0000)|12
    try: native.Machine(bad,**case_list[0]).run()
    except AssertionError: adverse['redirected_argument_home']=True
    else: raise AssertionError('wrong write accepted')
    return dict(cases=len(case_list),native_executions=2*len(case_list),host_executions=len(case_list),
        native_covered_instructions=len(covered),native_branch_outcomes=sorted([list(x) for x in branches]),
        linked_covered_instructions=len(linked_covered),linked_branch_outcomes=sorted([list(x) for x in linked_branches]),
        compiled_host_mutant_witnesses=rejection,interpreter_adverse_controls=adverse,
        domain='Finite accessible stable trees; all records have full initialized 20-byte storage; depth <=512. Not cyclic/corrupt or concurrent trees, whole callers, game, image or ROM proof.')


def main():
    parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path);args=parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='quadtree-proof-') as temporary:
        work=Path(temporary)
        rows,groups,words,provenance=compiler_proofs(work)
        behavioral=behavior(work,words)
        result=dict(schema=1,target=FN,address='0x800AC9BC',native_bytes=224,status='NONMATCH',claims=[],
            source_sha256=sha(SOURCE.read_bytes()),accepted_context_sha256=sha(ACCEPTED.read_bytes()),
            native_target_sha256=sha(b''.join(struct.pack('>I',w) for w in score.targets()[FN])),
            symbols_sha256=sha((ROOT/'asm/us/blob/symbols.json').read_bytes()),
            flags=FLAGS,controls=rows,genuine_wrapper_context=groups,behavior=behavioral,
            provenance=provenance,donor_commit=DONOR_COMMIT,
            source_files={str(p.relative_to(HERE)):sha(p.read_bytes()) for p in sorted(HERE.rglob('*.c'))},
            verifier_sha256={p.name:sha(p.read_bytes()) for p in [HERE/'native.py',HERE/'verify.py']})
        output=json.dumps(result,indent=2,sort_keys=True)+'\n'
        if args.output: args.output.write_text(output)
        print(json.dumps({k:result[k] for k in ['target','status','source_sha256','behavior']},indent=2))


if __name__=='__main__':main()
