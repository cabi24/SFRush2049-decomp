#!/usr/bin/env python3
"""Strict whole-object, GNU relocation, authentic-context and bounded runtime proof."""
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

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=importlib.util.spec_from_file_location('model_snapshot_native',HERE/'native.py')
native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
FN='func_800D4DFC'
SOURCE=ROOT/'cloud/matches/func_800D4DFC.c'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
CONTEXT=['math_utility','players_race_update','func_800D4D84','battle_mode_setup']
FLOAT_OFFSETS=sorted(set(range(748,784,4))|{1256,1328,1348,1420}|set(range(1484,1500,4))|
                     set(range(1516,1532,4))|set(range(1844,1880,4))|set(range(1884,1988,4)))
READ_OFFSETS=[x for x in FLOAT_OFFSETS if x < 1880]
MUTANTS={
 'wrong_coefficient':('2.72727275f);','2.0f);'),
 'wrong_second_radius':('object->rear2_angvel*object->rear2_radius','object->rear2_angvel*object->rear3_radius'),
 'skip_shadow_component':('object->reckon_airdist[i]=object->airdist[i];','object->reckon_airdist[i]=object->suscomp[i];'),
 'wrong_position_vector':('mveccopy((&object->base_vectors[6]),(&object->reckon_vectors[6]));',
                          'mveccopy((&object->base_vectors[3]),(&object->reckon_vectors[6]));'),
 'reversed_basis_copy':('math_utility(object->uv,object->reckon_uv);','math_utility(object->reckon_uv,object->uv);'),
}


def sha(data): return hashlib.sha256(data).hexdigest()


def symbol(obj,name):
    data,secs=score._elf(obj); ti=score._text_index(secs)
    syms=[s for i,x in enumerate(secs) if x['type']==2 for s in score._symbol_table(data,secs,i)
          if s['name']==name and s['type']==2 and s['section']==ti]
    assert len(syms)==1
    return syms[0]


def inspect(obj,name):
    cmp=score.compare(obj,name,show=0)
    return dict(asdict(cmp),verdict=cmp.summary(),symbol_bytes=symbol(obj,name)['size'],
                native_bytes=4*len(score.targets()[name]))


def complete(result):
    return result['verdict']=='MATCH' and result['symbol_bytes']==result['native_bytes'] and not any(
        result[k] for k in ('differing','unresolved','unverified','errors','extra_words'))


def code_proof(work):
    obj=work/'candidate.o';score.compile_single(SOURCE,FLAGS,obj)
    result=inspect(obj,FN);assert complete(result)
    data,secs=score._elf(obj);ti=score._text_index(secs);text=secs[ti]
    fn=symbol(obj,FN);assert fn['value']==0 and fn['size']==248
    assert text['size']==256 and not any(data[text['off']+248:text['off']+256])
    ro,=[s for s in secs if s['name']=='.rodata']
    own=data[ro['off']:ro['off']+ro['size']]
    assert own[:4]==struct.pack('>f',2.72727275)==score.own_data().read(native.LITERAL,4)
    assert len(own)==16 and not any(own[4:])
    assert not any(s['size'] for s in secs if s['name'] in ('.data','.bss'))
    relocs=[]
    for sec in secs:
        if sec['type']==9 and sec['info']==ti:
            syms=score._symbol_table(data,secs,sec['link'])
            for off in range(sec['off'],sec['off']+sec['size'],8):
                at,info=struct.unpack_from('>II',data,off)
                relocs.append({'offset':at,'type':info&255,'symbol':syms[info>>8]['name']})
    assert len(relocs)==3 and sum(r['type']==4 for r in relocs)==1
    assert {r['symbol'] for r in relocs}=={'math_utility','.rodata'}
    script=work/'link.ld'
    script.write_text('SECTIONS { .text 0x800D4DFC : SUBALIGN(4) { *(.text) }\n'
                      '.rodata 0x801241A4 : SUBALIGN(4) { *(.rodata) } }\nmath_utility = 0x8008D6B0;\n')
    elf=work/'candidate.elf'
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True)
    linked,sections=score._elf(elf);txt=sections[score._text_index(sections)]
    body=linked[txt['off']:txt['off']+248]
    words=list(struct.unpack('>62I',body))
    assert words==score.targets()[FN]
    assert symbol(elf,FN)['size']==248 and symbol(elf,FN)['value']==native.BASE
    linked_ro,=[s for s in sections if s['name']=='.rodata']
    assert linked[linked_ro['off']:linked_ro['off']+4]==own[:4]
    shoff=struct.unpack_from('>I',linked,0x20)[0]; shsize=struct.unpack_from('>H',linked,0x2e)[0]
    assert struct.unpack_from('>I',linked,shoff+sections.index(linked_ro)*shsize+12)[0]==native.LITERAL
    result.update(body_sha256=sha(body),all_gnu_linked_words_equal=True,relocations=relocs,
                  own_literal_address=hex(native.LITERAL),own_literal_bytes=4,
                  excluded_zero_text_alignment_bytes=8,excluded_zero_rodata_alignment_bytes=12)
    return result,words,own[:4]


def context_proof(work):
    group=work/'context';group.mkdir()
    names=[FN]+CONTEXT; paths=[SOURCE]+[ROOT/'src/blob'/(n+'.c') for n in CONTEXT]
    for i,path in enumerate(paths): shutil.copyfile(path,group/('c%d.c'%i))
    (group/'group.json').write_text(json.dumps({'files':['c%d.c'%i for i in range(len(names))],
        'members':[FN],'context':CONTEXT,'keep':names,'flags':FLAGS}))
    obj=group/'context.o';score.compile_group(group,obj)
    results={n:inspect(obj,n) for n in names}
    assert all(complete(v) for v in results.values())
    return {'bodies':results,'source_sha256':{str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in paths[1:]},
            'existing_exported_roots_preserved':True,
            'limit':'Five genuine bodies; separate accepted type views, not a unified full-game shadow unit.'}


def controls(work):
    original=SOURCE.read_text(); result={}
    variants={'o2':(original,FLAGS.replace('-O3','-O2'))}
    archived=(ROOT/'cloud/work/tiny_A98/func_800D4DFC.c').read_text()
    variants['a98_correct_owned_literal']=(archived.replace('extern f32 D_801241A4;','').replace('*D_801241A4','*2.72727275f'),FLAGS)
    # Exact donor macro is restored component-wise as separate ordinary statements.
    separate=original
    for i in (0,3,6):
        call='    mveccopy((&object->base_vectors[%d]),(&object->reckon_vectors[%d]));'%(i,i)
        expansion='\n'.join('    object->reckon_vectors[%d]=object->base_vectors[%d];'%(j,j) for j in range(i,i+3))
        assert call in separate;separate=separate.replace(call,expansion)
    variants['separate_vector_statements']=(separate,FLAGS)
    tail=original
    for i in (0,3):
        call='    mveccopy((&object->base_vectors[%d]),(&object->reckon_vectors[%d]));'%(i,i)
        expansion='\n'.join('    object->reckon_vectors[%d]=object->base_vectors[%d];'%(j,j) for j in range(i,i+3))
        tail=tail.replace(call,expansion)
    variants['authentic_final_vector_macro_only']=(tail,FLAGS)
    for name,(src,flags) in variants.items():
        path=work/(name+'.c');path.write_text(src);obj=work/(name+'.o')
        score.compile_single(path,flags,obj);result[name]=inspect(obj,FN)
        result[name]['source_sha256']=sha(src.encode())
    old=work/'archived_a98.o'
    score.compile_single(ROOT/'cloud/work/tiny_A98/func_800D4DFC.c',FLAGS,old)
    result['archived_a98']=inspect(old,FN)
    result['archived_a98']['source_sha256']=sha(archived.encode())
    assert result['archived_a98']['differing']==4
    assert result['a98_correct_owned_literal']['differing']==4
    assert complete(result['o2']) and complete(result['authentic_final_vector_macro_only'])
    assert result['separate_vector_statements']['differing']>0
    return result


def oracle(initial):
    out=bytearray(initial)
    def get(offset): return struct.unpack_from('>f',initial,offset)[0]
    # Independently group the two rear-wheel linear velocities, then average/scale.
    wheel2=native.f32(get(1328)*get(1256));wheel3=native.f32(get(1420)*get(1348))
    average=native.f32(native.f32(wheel2+wheel3)*0.5)
    speed=int(native.f32(average*native.f32(2.72727275)))
    assert -32768<=speed<=32767
    struct.pack_into('>h',out,1880,speed)
    for a,b,count in [(748,1952,36),(1484,1884,16),(1516,1900,16),(1844,1916,36)]:
        out[b:b+count]=initial[a:a+count]
    return bytes(out)


def cases(count=4096):
    rng=random.Random(0xD4DFC)
    edges=[0.0,-0.0,1.0,-1.0,0.5,-0.5,11999.0,-11999.0,0.125,-0.125]
    for k in range(count):
        data=bytearray(rng.randbytes(native.SIZE))
        for off in FLOAT_OFFSETS:
            struct.pack_into('>f',data,off,rng.randint(-65535,65535)/128.0)
        if k<100:
            a,b=edges[k//10],edges[k%10]
            vals=[1.0,a,1.0,b]
        else:
            vals=[rng.randint(1,256)/128.0,rng.randint(-8192,8192)/16.0,
                  rng.randint(1,256)/128.0,rng.randint(-8192,8192)/16.0]
        for off,value in zip([1256,1328,1348,1420],vals):struct.pack_into('>f',data,off,value)
        yield bytes(data)


def host_order(data):
    out=bytearray(data)
    if sys.byteorder=='little':
        for p in FLOAT_OFFSETS:out[p:p+4]=out[p:p+4][::-1]
        out[1880:1882]=out[1880:1882][::-1]
    return bytes(out)


def host_library(work,mutant=None):
    source=SOURCE
    if mutant:
        before,after=MUTANTS[mutant];text=SOURCE.read_text();assert text.count(before)==1
        source=work/(mutant+'.c');source.write_text(text.replace(before,after))
    library=work/((mutant or 'ordinary')+'.so')
    cmd=['cc','-std=c89','-pedantic-errors','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
         '-ffp-contract=off','-fsanitize=undefined','-fno-sanitize-recover=all',
         '-DCANDIDATE_SOURCE="%s"'%source,str(HERE/'host.c'),'-o',str(library)]
    subprocess.run(cmd,check=True,capture_output=True)
    dll=ctypes.CDLL(str(library));dll.run_case.argtypes=[ctypes.c_void_p,ctypes.c_void_p]
    dll.run_case.restype=ctypes.c_uint
    return dll


def host_result(dll,initial):
    src=ctypes.create_string_buffer(host_order(initial),native.SIZE)
    out=ctypes.create_string_buffer(native.SIZE)
    assert dll.run_case(src,out)==1
    return host_order(out.raw)


def behavior(work,words,literal,count=4096):
    dll=host_library(work); visited=set();digest=hashlib.sha256();selected=list(cases(count))
    callee=score.targets()['math_utility'];native_words=score.targets()[FN]
    for initial in selected:
        expected=oracle(initial)
        assert host_result(dll,initial)==expected
        for stream in (native_words,words):
            machine=native.Machine(stream,callee,literal,initial)
            assert machine.run()==expected
            visited.update(machine.visited)
        digest.update(expected)
    assert visited==set(range(native.BASE,native.BASE+248,4))|set(range(native.CALLEE,native.CALLEE+76,4))
    negative={}
    for name in MUTANTS:
        bad=host_library(work,name)
        for i,initial in enumerate(selected):
            if host_result(bad,initial)!=oracle(initial):
                negative[name]={'rejected':True,'case_index':i};break
        assert name in negative
    return {'cases':len(selected),'native_and_gnu_runs':2*len(selected),'native_target_instructions_executed':62,
            'real_native_callee_instructions_executed':19,'host_c89_ubsan':'passed',
            'complete_2056_byte_snapshots_equal':True,'callee_arguments_and_saved_registers_checked':True,
            'stack_and_untouched_memory_checked':True,'expected_output_sha256':digest.hexdigest(),
            'negative_controls':negative}


def verify(work,count=4096):
    proof,words,literal=code_proof(work)
    result={'status':'STRICT_MATCH_CANDIDATE','function':FN,'range':['0x800D4DFC','0x800D4EF4'],
            'base_revision':'cc4d5fdd','candidate_bytes':248,'accepted_byte_gain':0,
            'source_sha256':sha(SOURCE.read_bytes()),'flags':FLAGS,'object':proof,
            'controls':controls(work),'context':context_proof(work),
            'behavior':behavior(work,words,literal,count),
            'target_manifest_sha256':sha((score.ASM_DIR/'SHA256SUMS').read_bytes()),
            'tool_sha256':{n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']},
            'packet_sha256':{n:sha((HERE/n).read_bytes()) for n in ['verify.py','native.py','host.c','provenance.json']},
            'limits':['No full-game shadow unit, splice, linked game image, compression or ROM proof.',
                      'Finite binary32 round-to-nearest arithmetic with signed-halfword-range conversions only.',
                      'No signaling NaN, FCSR/exception, arbitrary pointer, gameplay or exact original-source claim.']}
    return result


if __name__=='__main__':
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--write',action='store_true')
    args=ap.parse_args()
    with tempfile.TemporaryDirectory(prefix='model-snapshot-proof-') as d:result=verify(Path(d))
    if args.write:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    else:
        expected=json.loads((HERE/'verification.json').read_text())
        compared=dict(result);saved=dict(expected)
        for key in ('target_manifest_sha256','tool_sha256'):
            compared.pop(key);saved.pop(key)
        assert compared==saved,'source or proof receipt drift'
    print(json.dumps({k:result[k] for k in ('status','object','controls','behavior')},indent=2))
