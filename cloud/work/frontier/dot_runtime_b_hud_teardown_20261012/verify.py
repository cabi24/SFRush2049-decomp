#!/usr/bin/env python3
"""Protocol-v2 complete-extent proof for the four-word HUD teardown research."""
import argparse
from contextlib import contextmanager, redirect_stdout
from dataclasses import asdict
import hashlib
import importlib.util
import io
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
NAME, ADDRESS, SIZE = 'func_8039244C', 0x8039244C, 380
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
BASELINE = ROOT / 'cloud/work/runtime_b_teardown_20261006/candidate.c'
TEST = ROOT / 'tests/cloud/test_runtime_b_hud_teardown_research.py'
SYMBOLS = {'D_80395ED4': 0x80395ED4, 'D_80151AD0': 0x80151AD0,
           'D_803950C0': 0x803950C0, 'D_80395C00': 0x80395C00,
           'D_80395CD0': 0x80395CD0, 'D_80395DA0': 0x80395DA0,
           'sound_stop': 0x800B358C, 'sound_call_minimal': 0x80090254}
DIFFS = [0xA4, 0xC0, 0x134, 0x138]


def require(ok, reason):
    if not ok:
        raise ValueError(reason)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def packed(words):
    return struct.pack('>%dI' % len(words), *words)


def run(args, **kw):
    result = subprocess.run([str(a) for a in args], capture_output=True, text=True, **kw)
    require(result.returncode == 0, ('command failed', args, result.returncode, result.stdout, result.stderr))
    return result


@contextmanager
def targets(population):
    old = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us' / population
    try:
        yield
    finally:
        score.ASM_DIR = old


def inputs():
    paths = [HERE/n for n in ('candidate.c','group.json','verify.py','machine.py','host.c','trace.py','layout_check.c')]
    paths += [TEST, BASELINE, ROOT/'src/blob/sound_call_minimal.c']
    result = {'files': {str(p.relative_to(ROOT)): sha(p.read_bytes()) for p in paths}}
    with targets('ovl_b'):
        table = score.targets()
        ext = json.loads(score.verified_bytes(score.ASM_DIR/'extents.json', score.target_manifest()))
        row = next(f for f in ext['functions'] if f['name'] == NAME)
        require(row['size'] == SIZE and int(row['address'],16) == ADDRESS, 'native extent drift')
        following = next(f for f in ext['functions'] if int(f['address'],16) == ADDRESS+SIZE)
        require(following['name'] == 'func_803925C8', 'native next boundary')
        require(len(table[NAME])*4 == SIZE, 'complete native word extent')
        syms = score.image_symbols()
        require(all(syms.get(n,score.address_named(n)) == a for n,a in SYMBOLS.items()), 'binding drift')
        result['native'] = {'image':'B','rom_offset':ext['rom_offset'],'image_sha256':ext['image_sha256'],
                            'extent':row,'next_boundary':following,'word_sha256':sha(packed(table[NAME])),
                            'symbol_bindings':SYMBOLS}
    census, census_inputs = [], {}
    for population in ('blob','ovl_a','ovl_b'):
        with targets(population):
            table=score.targets()
            # Bind the complete parsed word inventory, not mutable asm text/provenance lines.
            census_inputs[population]=sha(json.dumps({n:sha(packed(w)) for n,w in sorted(table.items())},sort_keys=True).encode())
            for name,words in table.items():
                for i,w in enumerate(words):
                    if w>>26 == 3 and ((w&0x3ffffff)<<2|0x80000000)==ADDRESS:
                        census.append([population,name,i*4])
    require(census == [['blob','records_screen',0x110]], 'caller census drift')
    result['callers']={'direct_calls':census,'parsed_inventory_sha256':census_inputs}
    with targets('blob'):
        table=score.targets()
        result['native_helpers_and_caller']={n:{'bytes':len(table[n])*4,'word_sha256':sha(packed(table[n]))}
                                             for n in ('records_screen','sound_stop','sound_call_minimal')}
    return result


def tools_receipt():
    return {name:sha(Path(score.ido(name)).read_bytes())
            for name in ('cc','cfe','uopt','ugen','as1','uld','usplit','umerge')}


def inspect_object(obj, expected_differences, expected_size=SIZE):
    data,secs=score._elf(obj);ti=score._text_index(secs)
    syms=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i)]
    funcs=[s for s in syms if s['type']==2 and s['section']]
    require(funcs==[dict(name=NAME,value=0,size=expected_size,type=2,section=ti)], 'full STT_FUNC extent/set')
    shoff=struct.unpack_from('>I',data,32)[0];stride=struct.unpack_from('>H',data,46)[0]
    for i,sec in enumerate(secs):
        flags=struct.unpack_from('>I',data,shoff+stride*i+8)[0]
        require(not(flags&2 and sec['size'] and sec['name'] not in ('.text','.reginfo')), 'unexpected owned storage')
    rels=[]
    for sec in secs:
        if sec['type']!=9:continue
        require(sec['info']==ti,'nontext relocation')
        ss=score._symbol_table(data,secs,sec['link'])
        for off in range(sec['off'],sec['off']+sec['size'],8):
            site,info=struct.unpack_from('>II',data,off);name=ss[info>>8]['name'];kind=info&255
            require(site%4==0 and site<SIZE and kind in (4,5,6) and name in SYMBOLS,'unexpected REL')
            rels.append([site,kind,name])
    require(len(rels)==21,'complete original REL census')
    raw=score.text_words(obj)
    require(len(raw)==96 and raw[95:]==[0],'full text size / zero suffix')
    with targets('ovl_b'):
        got,masks,unresolved,unverified,errors=score.relocate(obj,raw,0,SIZE,score.image_symbols())
        require(not any((masks,unresolved,unverified,errors)),'incomplete binding')
        native=score.targets()[NAME]
        diffs=[i*4 for i in range(95) if native[i]!=got[i]]
        require(diffs==expected_differences if isinstance(expected_differences,list) else len(diffs)==expected_differences,'full-word residual drift')
        with redirect_stdout(io.StringIO()):comp=score.compare(obj,NAME)
        require(comp.differing==len(diffs) and not comp.accepted() and not any((comp.extra_words,comp.unresolved,comp.unverified,comp.errors)),'canonical residual drift')
    return got[:95],dict(function_bytes=SIZE,text_bytes=384,separate_zero_suffix_bytes=4,owned_data_bytes=0,
                         difference_offsets=diffs,comparison=asdict(comp),original_relocations=rels,
                         relocated_sha256=sha(packed(got[:95])))


def compile_final(work):
    obj=work/'candidate.o';score.compile_single(HERE/'candidate.c',FLAGS,obj)
    body,meta=inspect_object(obj,DIFFS)
    return obj,body,meta


def gnu_link(obj,work,expected):
    script=work/'link.ld';linked=work/'linked.elf';binary=work/'linked.bin'
    script.write_text('SECTIONS { .text 0x8039244C : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n'+'\n'.join('%s = 0x%x;'%x for x in SYMBOLS.items()))
    run(['mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj])
    run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',linked,binary])
    raw=binary.read_bytes();require(raw==packed(expected)+bytes(4),'GNU disagrees with complete relocated object')
    row=[l.split() for l in run(['mips-linux-gnu-nm','-S',linked]).stdout.splitlines() if l.split()[-1]==NAME]
    require(len(row)==1 and row[0][:3]==['8039244c','0000017c','T'],'GNU native placement/size')
    data,secs=score._elf(linked)
    for i,sec in enumerate(secs):
        require(not(sec['type']==9 and sec['size']),'GNU remaining RELs')
        if sec['type']==2:
            require(not any(s['section']==0 and s['name'] for s in score._symbol_table(data,secs,i)),'GNU undefined symbol')
    return {'complete_original_relocations':21,'function_bytes':380,'text_bytes':384,'owned_data_bytes':0,
            'all_original_rels_consumed':True,'native_entry_verified':True,'body_sha256':sha(raw[:380])}


def installer_gate():
    from tools.conveyor.pipeline import ovl_splice
    recipe=json.loads((HERE/'group.json').read_text())
    require(recipe['members']==[] and recipe['claims']==[] and recipe['context']==[NAME], 'research recipe widened')
    try:ovl_splice.packet_spec(HERE)
    except ovl_splice.SpliceError as exc:
        require('no claims (research only), nothing to splice' in str(exc),'unexpected installer refusal')
    else:raise ValueError('empty claims became installable')
    return {'empty_claims_refused':True,'selected_bodies':[],'installed_bytes':0,'production_mutation':False}


def machine_proof(bodies):
    spec=importlib.util.spec_from_file_location('hud_machine',HERE/'machine.py')
    m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
    coverage=[set() for _ in bodies];branches=[set() for _ in bodies];total=0
    for seed in range(16):
        for count in m.COUNTS:
            for resource in (False,True):
                original=m.fixture(seed,count,resource)
                for scenario in range(6):
                    expected=m.oracle(original,scenario)
                    for i,body in enumerate(bodies):
                        got=m.execute(body,original,scenario)
                        require(got[:2]==expected,('field-oracle difference',i,seed,count,resource,scenario))
                        coverage[i]|=got[2];branches[i]|=got[3]
                    total+=1
    for body,sites,outcomes in zip(bodies,coverage,branches):
        require(sites==set(range(0,SIZE,4)),'incomplete instruction coverage')
        expected={(i*4,taken) for i,w in enumerate(body) if w>>26 in (4,5,6,0x14) for taken in (False,True)}
        require(outcomes==expected,'incomplete conditional outcomes')
    negatives={}
    wrong=bodies[1][:];wrong[0x9c//4]=(wrong[0x9c//4]&0xffff0000)|9
    for name,body in [('wrong_state',wrong),('unsupported_opcode',[0xffffffff])]:
        try:require(m.execute(body,m.fixture(1,1,False),0)[:2]==m.oracle(m.fixture(1,1,False),0),'wrong machine result')
        except ValueError:negatives[name]=True
        else:raise ValueError('machine negative escaped')
    return {'fixtures':total,'machine_executions':total*len(bodies),'instruction_offsets_covered':[len(s) for s in coverage],
            'all_branch_outcomes':True,'complete_nonstack_snapshots_and_helper_events':True,
            'o32_integer_clobbers_and_saved_registers':True,'negative_controls':negatives}


def host_proof(work,sanitize=False):
    exe=work/'host';flags=['gcc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O2','-fstrict-aliasing']
    if sanitize:flags+=['-fsanitize=address,undefined,bounds','-fno-sanitize-recover=all']
    run(flags+[HERE/'host.c','-o',exe])
    env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0') if sanitize else None
    result=run([exe],env=env)
    require(result.stdout.strip()=='1344 complete-state host fixtures passed' and not result.stderr,'host output')
    source=(HERE/'candidate.c').read_text();require(source.count('first->state = 8;')==1,'host mutant anchor')
    mutant=work/'wrong.c';mutant.write_text(source.replace('first->state = 8;','first->state = 9;'))
    run(flags+['-DCANDIDATE_PATH="'+str(mutant)+'"',HERE/'host.c','-o',exe])
    bad=subprocess.run([str(exe)],capture_output=True,text=True,env=env)
    require(bad.returncode==1 and 'mismatch' in bad.stderr,'compiled mutation escaped')
    return {'actual_source_fixtures':1344,'wrong_state_mutation_rejected':True,'sanitizers':sanitize}


def deep_proof(work,obj,body):
    gnu=gnu_link(obj,work,body)
    score.compile_single(HERE/'layout_check.c',FLAGS,work/'layout.o')
    layout_body,_=inspect_object(work/'layout.o',DIFFS)
    require(layout_body==body,'native layout checks altered candidate')
    controls={};baseline=work/'baseline.o';score.compile_single(BASELINE,FLAGS,baseline)
    old,meta=inspect_object(baseline,34);controls['baseline']=meta
    src=(HERE/'candidate.c').read_text()
    for name,changed,expected in [
        ('unguarded_first',src.replace('    if (D_80151AD0 > 0) {\n        first = D_80395C00;','    first = D_80395C00;\n    if (D_80151AD0 > 0) {'),17),
        ('separate_initializers',src.replace('second = D_80395CD0; third = D_80395DA0; group = D_803950C0;','second = D_80395CD0;\n        third = D_80395DA0;\n        group = D_803950C0;'),6)]:
        path=work/(name+'.c');path.write_text(changed);control=work/(name+'.o');score.compile_single(path,FLAGS,control)
        _,controls[name]=inspect_object(control,expected)
    group=work/'group.o';score.compile_group(HERE,group);group_body,group_meta=inspect_object(group,DIFFS)
    require(group_body==body,'O3 research recipe differs from O2')
    with targets('ovl_b'):native=score.targets()[NAME]
    machine=machine_proof([native,body,old])
    helper=work/'helper.o';source=ROOT/'src/blob/sound_call_minimal.c'
    score.compile_single(source,FLAGS,helper)
    with targets('blob'):
        with redirect_stdout(io.StringIO()):result=score.compare(helper,'sound_call_minimal')
        require(result.accepted(),'existing O32 wrapper drift')
        hw=score.targets()['sound_call_minimal'];require(len(hw)==12,'wrapper extent')
    # Change STT_FUNC extent in a disposable object; all proof gates remain fail-closed.
    corrupted=bytearray(obj.read_bytes());_,secs=score._elf(obj)
    for i,sec in enumerate(secs):
        if sec['type']==2:
            for j,sym in enumerate(score._symbol_table(corrupted,secs,i)):
                if sym['name']==NAME:struct.pack_into('>I',corrupted,sec['off']+16*j+8,SIZE+4)
    bad=work/'bad-extent.o';bad.write_bytes(corrupted)
    try:inspect_object(bad,DIFFS)
    except ValueError as exc:require('STT_FUNC' in str(exc),'wrong extent rejection')
    else:raise ValueError('overlong extent escaped')
    return {'gnu':gnu,'controls':controls,'group_o3_identical':group_meta,'machine':machine,
            'host':host_proof(work),'sanitized_host':host_proof(work,True),
            'helper_48_bytes_preserved_no_new_credit':True,'overlong_extent_rejected':True,'native_C89_shape_checks':12,
            'installer':installer_gate()}


def verify(deep=False,write=False,show_score=False):
    current=inputs();saved=None
    receipt=HERE/'verification.json'
    if not write:
        saved=json.loads(receipt.read_text());require(saved['inputs']==current,'input receipt changed')
    compiler=tools_receipt()
    if saved:require(saved['stock_compilers']==compiler,'stock compiler identity changed')
    with tempfile.TemporaryDirectory(prefix='hud-teardown-') as directory:
        work=Path(directory);obj,body,fast=compile_final(work)
        fast['installer']=installer_gate()
        if saved:require(saved['fast']==fast,'fast receipt changed')
        detail=deep_proof(work,obj,body) if deep or write else None
        if deep and saved:require(saved['deep']==detail,'deep receipt changed')
        if show_score:
            with targets('ovl_b'):
                print(NAME+':');print('  '+score.compare(obj,NAME,show=0).summary())
        result={'protocol':2,'status':'NONMATCH','new_strict_bytes':0,'accepted_image_bytes':0,'rom_verified_bytes':0,
                'inputs':current,'stock_compilers':compiler,'fast':fast,'deep':detail}
        if write:receipt.write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    return result


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--deep',action='store_true');parser.add_argument('--write-receipt',action='store_true');parser.add_argument('--score',action='store_true')
    args=parser.parse_args();verify(args.deep,args.write_receipt,args.score)
    print('PASS input-bound complete HUD teardown proof (NONMATCH 4/95; zero coverage)')
