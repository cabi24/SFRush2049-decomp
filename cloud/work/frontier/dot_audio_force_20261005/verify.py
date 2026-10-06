#!/usr/bin/env python3
"""Rebuild the real-caller proof; emit metadata only, never native byte payloads."""
import argparse
import ctypes
from dataclasses import asdict
import hashlib
import importlib.util
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

# Context groups superseded after this packet was frozen are kept byte-identical
# under cloud/work/frontier/superseded/; receipts keep the original paths.
SUPERSEDED={'src/blob/groups/frontier_list_alloc_sound':'cloud/work/frontier/superseded/frontier_list_alloc_sound'}
def src(p):
    p=str(p)
    for old,new in SUPERSEDED.items():
        if p.startswith(old) and not (ROOT/old).exists():return ROOT/(new+p[len(old):])
    return ROOT/p
sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(HERE))
from tools.cloud import score
_native_spec = importlib.util.spec_from_file_location('dot_audio_force_native',HERE/'native_machine.py')
_native = importlib.util.module_from_spec(_native_spec)
_native_spec.loader.exec_module(_native)
bits,value,execute = _native.bits,_native.value,_native.execute

NAME = 'func_800DED78'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
OLD = ROOT/'cloud/work/module_campaign_20261002/ai/model_audio'
ACCEPTED_KEY = 'src/blob/groups/frontier_list_alloc_sound'
ACCEPTED = src(ACCEPTED_KEY)
CONTEXT = ['func_80092278','entity_flags_apply','high_scores_display','func_8009211C','func_80091FBC']


def sha(data):
    return hashlib.sha256(data).hexdigest()


def extent(obj, name):
    raw, sections = score._elf(obj)
    text = score._text_index(sections)
    found = [s for i,sec in enumerate(sections) if sec['type'] == 2
             for s in score._symbol_table(raw,sections,i)
             if s['name'] == name and s['section'] == text and s['type'] == 2]
    assert len(found) == 1 and found[0]['size'] > 0
    return found[0]['value'],found[0]['size']


def caller(index_first=True, volatile_clock=True):
    source = (OLD/'ROOT_DEF68/mode_select_handler.c').read_text()
    source = source.replace('#include "impact_sound.h"',(OLD/'ROOT_DEF68/impact_sound.h').read_text())
    if volatile_clock:
        declaration = 'extern f32 D_8002EB90,D_80140BE0[],D_80140B10[],D_80142518[];'
        assert source.count(declaration) == 1
        source = source.replace(declaration,'extern volatile f32 D_8002EB90;\nextern f32 D_80140BE0[],D_80140B10[],D_80142518[];')
    if index_first:
        changes = [('func_800DED78(s32,s16,void*,f32)', 'func_800DED78(s16,s32,f32*,f32)'),
                   ('func_800DED78((s32) m->player, var_a3,','func_800DED78(var_a3, (s32) m->player,'),
                   ('func_800DED78((s32) m->player, 4,','func_800DED78(4, (s32) m->player,')]
        for old,new in changes:
            assert source.count(old) == 1
            source = source.replace(old,new)
    return source


def full_core():
    archived = (OLD/'audio_core.c').read_text()
    first = archived.index('void func_800DED78(s32 arg0')
    last = archived.index('\nvoid func_800E05F0',first)
    archived = (archived[:first]+archived[last:]).replace(
        'func_800DED78(s32,s16,void*,f32)','func_800DED78(s16,s32,f32*,f32)')
    assert archived.count('extern float D_8002EB90;') == 1
    archived = archived.replace('extern float D_8002EB90;','extern volatile float D_8002EB90;')
    return archived


def group(work, candidate=None, context=False, index_first=True):
    work.mkdir(parents=True)
    source = candidate if candidate is not None else (HERE/'candidate.c').read_text()
    (work/'candidate.c').write_text(source)
    (work/'caller.c').write_text(caller(index_first, 'extern volatile f32 D_8002EB90;' in source))
    spec = {'files':['candidate.c','caller.c'],'members':[NAME],
            'context':['mode_select_handler'],'keep':['mode_select_handler'],'claims':[NAME],'flags':FLAGS}
    if context:
        (work/'list.c').write_bytes((ACCEPTED/'group.c').read_bytes())
        existing = json.loads((ACCEPTED/'group.json').read_text())
        spec['files'].append('list.c')
        spec['keep'] += existing['keep']
        spec['context'] += CONTEXT
    (work/'group.json').write_text(json.dumps(spec,indent=2)+'\n')
    obj = work/'candidate.o'
    score.compile_group(work,obj)
    return obj


def strict(obj, name=NAME):
    begin,size = extent(obj,name)
    want = score.targets()[name]
    assert size == len(want)*4, (name,'wrong complete extent',size)
    result = score.compare(obj,name,show=0)
    assert result.accepted(), (name,size,asdict(result))
    return {'elf_bytes':size,'comparison':asdict(result),'object_offset':begin}


def gnu_proof(obj, work):
    start = score.image_symbols()[NAME]
    begin,size = extent(obj,NAME)
    assert begin == 0 and size == 488
    addresses = score.image_symbols()
    script = ('SECTIONS { .text 0x%X : SUBALIGN(4) { *(.text) } '
              '.rodata 0x80124320 : SUBALIGN(4) { *(.rodata) } }\n' % start)
    # The actual caller is unclaimed. Its definition remains at its generated
    # address; only the helper and its own first literal are checked here.
    definitions = set(score.symbols(obj))
    script += ''.join('%s = 0x%X;\n'%(key,val) for key,val in addresses.items() if key not in definitions)
    ld = work/'link.ld'; ld.write_text(script)
    elf, binary, rodata = work/'linked.elf',work/'linked.bin',work/'rodata.bin'
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(ld),'-o',str(elf),str(obj)],check=True,capture_output=True)
    subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(elf),str(binary)],check=True)
    subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.rodata',str(elf),str(rodata)],check=True)
    raw = binary.read_bytes()[:size]
    target = struct.pack('>122I',*score.targets()[NAME])
    assert extent(elf,NAME) == (start,size)
    assert raw == target, 'GNU full-body mismatch'
    assert rodata.read_bytes()[:4] == struct.pack('>f',0.1) == score.own_data().read(0x80124320,4)
    return list(struct.unpack('>122I',raw)), {'full_body_equal':True,'body_sha256':sha(raw),
            'elf_bytes':size,'literal_address':'0x80124320','literal_bytes':4,'literal_value':0.1}


def source_bindings():
    paths = [HERE/'candidate.c',HERE/'native_machine.py',HERE/'verify.py',
             OLD/'audio_core.c',OLD/'ROOT_DEF68/mode_select_handler.c',OLD/'ROOT_DEF68/impact_sound.h',
             ]
    bindings = {str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in paths}
    for name in ('group.c','group.json'):
        bindings[ACCEPTED_KEY+'/'+name] = sha((ACCEPTED/name).read_bytes())
    return bindings


def oracle(record, vector, threshold, clocks):
    out = list(record)
    read = 0
    def clock():
        nonlocal read
        v = value(clocks[min(read,len(clocks)-1)]); read += 1
        return v
    def f(word): return value(word)
    if out[5] == 0:
        if f(out[4]) != 0 and f(out[0]) == 0:
            if all(f(v) == 0 for v in vector):
                if value(bits(clock()-f(out[4]))) > 0.5: out[4] = bits(0)
            else: out[4] = bits(clock())
        else:
            if any(f(v) != 0 for v in vector):
                sq = [value(bits(f(v)*f(v))) for v in vector]
                total = value(bits(value(bits(sq[0]+sq[1]))+sq[2]))
                force = bits(__import__('math').sqrt(total) if total >= 0 else float('nan'))
                if f(threshold) < f(force) and f(out[0]) < f(force):
                    out[0] = force; out[1:4] = vector; out[4] = bits(clock())
            if f(out[4]) != 0 and value(bits(clock()-f(out[4]))) > value(bits(0.1)):
                out[5] = 1; out[4] = bits(clock())
    return out,read


def cases():
    # Exact boundaries, signed zeros, all branch families, all players/slots.
    vectors = [(0,0,0),(4,0,0),(1,0,0),(0,-1,0),(0,0,1),(3,4,0),(-3,-4,12),(5000,0,0),(5001,0,0)]
    for player in range(4):
        for index in range(5):
            for flag in (0,1,-1):
                for peak,timestamp in [(0,0),(0,1),(4,1),(6,1),(0,-1)]:
                    for vector in vectors:
                        for now in (1.0,1.1,1.100000143,1.5,1.500000119,0.5):
                            yield player,index,[bits(peak),bits(91),bits(92),bits(93),bits(timestamp),flag&0xffffffff],list(map(bits,vector)),bits(4),[bits(now)]
    yield 0,0,[bits(5),bits(91),bits(92),bits(93),bits(1),0],[bits(3),bits(4),0],bits(4),[bits(2)]
    yield 0,0,[bits(3),bits(91),bits(92),bits(93),bits(0.025),0],[0,0,0],bits(1),[bits(0.125)]
    for player in range(4):
        for index in range(5):
            yield player,65536+index,[0,0,0,0,0,0],[bits(3),bits(4),0],bits(4),[bits(2)]
    rng = random.Random(2049)
    for _ in range(2500):
        player,index = rng.randrange(4),rng.randrange(5)
        record = [bits(rng.choice([0,1,4,10000])),bits(91),bits(92),bits(93),bits(rng.choice([0,1,2,-1])),rng.choice([0,0,0,1,0xffffffff])]
        vector = [bits(rng.uniform(-10000,10000)) for _ in range(3)]
        yield player,index,record,vector,bits(rng.choice([0,1,5000,15000])),[bits(rng.uniform(-2,5))]
    for vec in [(-0.0,0,0),(float('nan'),0,0),(float('inf'),0,0),(1e30,-1e30,1e30),(1e-30,1e-30,0)]:
        for peak in (0,1,float('nan')):
            yield 0,0,[bits(peak),bits(91),bits(92),bits(93),bits(0),0],list(map(bits,vec)),bits(1),[bits(2)]


def behavior(words, linked, work, source=None):
    source = source if source is not None else (HERE/'candidate.c').read_text()
    c = work/'host.c'; lib = work/'host.so'
    c.write_text(source+'\nImpact120 D_80140808[4];\nvolatile f32 D_8002EB90;\n')
    subprocess.run(['cc','-std=c99','-O1','-shared','-fPIC','-fno-fast-math','-ffp-contract=off',
                    '-fno-strict-aliasing','-fsanitize=undefined','-fno-sanitize-recover=all',
                    '-Wno-unknown-pragmas',str(c),'-lm','-o',str(lib)],check=True,capture_output=True)
    host = ctypes.CDLL(str(lib))
    table = (ctypes.c_uint32*120).in_dll(host,'D_80140808')
    clock = ctypes.c_uint32.in_dll(host,'D_8002EB90')
    fn = host.func_800DED78
    fn.argtypes = [ctypes.c_int16,ctypes.c_int32,ctypes.POINTER(ctypes.c_uint32),ctypes.c_float]
    fn.restype = None
    base,timer,vector_base = 0x80140808,0x8002eb90,0x600000
    literal = bits(0.1)
    all_visited,count = set(),0
    def norm(word):
        return 0x7fc00000 if __import__('math').isnan(value(word)) else word
    for player,index,record,vector,threshold,clocks in cases():
        initial = [bits(-23)]*120
        start = player*30+ctypes.c_int16(index).value*6
        initial[start:start+6] = record
        memory = {base+4*i:v for i,v in enumerate(initial)}
        memory.update({vector_base+4*i:v for i,v in enumerate(vector)})
        memory[timer],memory[0x80124320] = clocks[0],literal
        expected,reads = oracle(record,vector,threshold,clocks)
        want = list(initial); want[start:start+6] = expected
        for code in (words,linked):
            result,visited,nread,events = execute(code,0x800ded78,memory,player,index,vector_base,threshold,timer,clocks)
            actual = [result[base+4*i] for i in range(120)]
            assert list(map(norm,actual)) == list(map(norm,want)), ('native/oracle',count)
            assert nread == reads, ('timer read contract',count,nread,reads)
            all_visited.update(visited)
        for i,v in enumerate(initial): table[i] = v
        clock.value = clocks[0]
        vec = (ctypes.c_uint32*3)(*vector)
        fn(index,player,vec,value(threshold))
        assert list(map(norm,table)) == list(map(norm,want)), ('host/native',count)
        assert list(vec) == vector
        count += 1
    dynamic = 0
    for record,vector,threshold,clocks in [
        ([0,0,0,0,bits(1),0],[bits(0)]*3,bits(1),[bits(2)]),
        ([bits(3),0,0,0,bits(1),0],[bits(0)]*3,bits(1),[bits(2),bits(7)]),
        ([0,0,0,0,0,0],[bits(3),bits(4),0],bits(1),[bits(1),bits(2),bits(9)]),
        ([0,0,0,0,0,0],[bits(3),bits(4),0],bits(1),[bits(2),bits(1)])]:
        memory = {base+4*i:v for i,v in enumerate(record)}
        memory.update({vector_base+4*i:v for i,v in enumerate(vector)})
        memory[timer],memory[0x80124320] = clocks[0],literal
        want,nread = oracle(record,vector,threshold,clocks)
        for code in (words,linked):
            actual,visited,reads,_ = execute(code,0x800ded78,memory,0,0,vector_base,threshold,timer,clocks)
            assert [actual[base+4*i] for i in range(6)] == want and reads == nread
        dynamic += 1
    return {'stable_clock_cases':count,'dynamic_clock_cases':dynamic,
            'native_runs':2*(count+dynamic),'host_ubsan_cases':count,
            'executed_instruction_offsets':sorted(all_visited),
            'unexecuted_instruction_offsets':sorted(set(range(0,488,4))-all_visited)}


def build_report(work):
    obj = group(work/'minimal')
    proof = strict(obj)
    raw,sections = score._elf(obj)
    text_index = score._text_index(sections)
    relocations = []
    for section in sections:
        if section['type'] == 9 and section['info'] == text_index:
            symbols = score._symbol_table(raw,sections,section['link'])
            for offset in range(section['off'],section['off']+section['size'],8):
                site,info = struct.unpack_from('>II',raw,offset)
                if site < 488:
                    relocations.append({'offset':site,'type':info&255,'symbol':symbols[info>>8]['name']})
    assert len(relocations) == 12
    proof['relocations'] = relocations
    baseline_dir = work/'archive'
    baseline_dir.mkdir()
    (baseline_dir/'audio_core.c').write_bytes((OLD/'audio_core.c').read_bytes())
    (baseline_dir/'caller.c').write_text(caller(False,False))
    baseline_spec = json.loads((OLD/'group.json').read_text())
    baseline_spec['files'] = ['audio_core.c','caller.c']
    (baseline_dir/'group.json').write_text(json.dumps(baseline_spec))
    baseline_obj = baseline_dir/'baseline.o'
    score.compile_group(baseline_dir,baseline_obj)
    baseline = {'comparison':asdict(score.compare(baseline_obj,NAME,show=0)),
                'elf_bytes':extent(baseline_obj,NAME)[1]}
    assert baseline['comparison']['differing'] == 35 and baseline['elf_bytes'] == 488
    complete = work/'full-genuine-closure'
    complete.mkdir()
    archived = full_core()
    (complete/'core.c').write_text(archived)
    (complete/'candidate.c').write_bytes((HERE/'candidate.c').read_bytes())
    (complete/'caller.c').write_text(caller())
    complete_spec = dict(baseline_spec, files=['candidate.c','core.c','caller.c'])
    (complete/'group.json').write_text(json.dumps(complete_spec))
    complete_obj = complete/'candidate.o'
    score.compile_group(complete,complete_obj)
    full_closure = strict(complete_obj)
    linked,gnu = gnu_proof(obj,work/'minimal')
    extended = group(work/'context',context=True)
    context = {name:strict(extended,name) for name in CONTEXT}
    strict(extended)
    # Keep all caller uncertainty, but omit compared word payloads from public notes.
    caller_result = score.compare(obj,'mode_select_handler',show=0)
    caller_report = {k:v for k,v in asdict(caller_result).items() if k != 'errors'}
    caller_report['errors'] = ['own switch-table bytes differ from the protected target' for _ in caller_result.errors]
    caller_report['elf_bytes'] = extent(obj,'mode_select_handler')[1]
    original = (HERE/'candidate.c').read_text()
    controls = {}
    for label,source,index_first in [
        ('ordinary_clock',original.replace('extern volatile f32 D_8002EB90;','extern f32 D_8002EB90;'),True),
        ('player_first',original.replace('s16 index, s32 player','s32 player, s16 index'),False),
        ('wrong_literal',original.replace('> 0.1f','> 0.2f'),True)]:
        control = group(work/label,candidate=source,index_first=index_first)
        comparison = score.compare(control,NAME,show=0)
        assert not comparison.accepted(),label
        controls[label] = {'comparison':asdict(comparison),'elf_bytes':extent(control,NAME)[1]}
    controls['wrong_literal']['comparison']['errors'] = ['owned literal differs from protected target']
    behavioral = behavior(score.targets()[NAME],linked,work)
    mutants = {}
    for label,old,new in [
        ('wrong_peak_comparison','force > D_80140808','force >= D_80140808'),
        ('wrong_threshold_comparison','threshold < force','threshold <= force'),
        ('wrong_timeout_boundary','> 0.1f','>= 0.1f'),
        ('wrong_bump_guard','.flag == 0','.flag != 0')]:
        assert original.count(old) == 1
        directory = work/label
        directory.mkdir()
        try:
            behavior(score.targets()[NAME],linked,directory,source=original.replace(old,new))
        except AssertionError as exc:
            assert exc.args and exc.args[0][0] == 'host/native', str(exc)
            mutants[label] = {'rejected':True,'case_index':exc.args[0][1]}
        else:
            raise AssertionError('semantic mutation escaped: '+label)
    # Native redundant clock observations survive without any intervening write.
    target = score.targets()[NAME]
    assert all((target[o//4] >> 26) not in (40,41,42,43,46,56,57,61) for o in range(0x1b0,0x1d0,4))
    symbols = json.loads((ROOT/'asm/us/boot_tail/symbols.json').read_text())['symbols']
    assert symbols['__osScElapsedTime'] == '0x8002EB90'
    for writer in ('viTickStart','viUpdateTime'):
        assert '%lo(__osScElapsedTime)' in (ROOT/'asm/us/nonmatchings/rom/lib_1050'/ (writer+'.s')).read_text()
    return {'status':'MATCHING_RESEARCH','coverage_credit':0,'base':'cc4d5fdd',
            'toolchain_sha256':{name:sha((score.IDO/name).read_bytes()) for name in ('cc','cfe','uopt','ugen','as1','uld','usplit','umerge')},
            'function':NAME,'start':'0x800DED78','end_exclusive':'0x800DEF60','flags':FLAGS,
            'source_bindings':source_bindings(),'proof':proof,'gnu_link':gnu,'archive_baseline':baseline,'full_genuine_closure':full_closure,
            'accepted_context':context,'unclaimed_caller':caller_report,'controls':controls,
            'behavior':behavioral,'semantic_mutants':mutants,
            'clock_observation':{'address':'0x8002EB90','static_alias':'__osScElapsedTime',
                'writers':['viTickStart','viUpdateTime'],'two_load_offsets':[432,464],
                'no_intervening_stores':True}}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--record',action='store_true')
    parser.add_argument('--output',type=Path)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='rush-force-') as tmp:
        result = build_report(Path(tmp))
    if args.record:
        (HERE/'verification.json').write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    else:
        assert result == json.loads((HERE/'verification.json').read_text()),'receipt drift'
    if args.output: args.output.write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    print(json.dumps({'status':result['status'],'bytes':488,'cases':result['behavior']['stable_clock_cases'],
                      'executed_instructions':len(result['behavior']['executed_instruction_offsets'])}))

if __name__ == '__main__': main()
