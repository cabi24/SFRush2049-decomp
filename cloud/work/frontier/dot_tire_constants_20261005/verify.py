#!/usr/bin/env python3
"""Source-bound complete ELF, GNU relocation, donor/context and runtime proof."""
import argparse
import ctypes
from dataclasses import asdict
import hashlib
import importlib.util
import json
import math
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
spec=importlib.util.spec_from_file_location('tire_constants_native',HERE/'native.py')
native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
FN='track_preview_handler'
SOURCE=ROOT/'cloud/matches'/ (FN+'.c')
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
CONTEXT=ROOT/'src/blob/groups/codex_car_resistance_b114'
MUTANTS={
 'wrong_load_scale':('* .5f;','* 1.0f;'),
 'wrong_model_field':('m->load * otw','m->wheelbase * otw'),
 'reversed_stiffness_ratio':('tdes->Cstiff/tdes->Zforce','tdes->Zforce/tdes->Cstiff'),
 'wrong_m1':('4.16f/tdes->Cfmax','4.0f/tdes->Cfmax'),
 'wrong_l3':('tdes->l3 = tdes->k3*3;','tdes->l3 = tdes->k3*2;'),
 'wrong_patch_reset':('tdes->patchy = 0;','tdes->patchy = 1;'),
}


def sha(data):return hashlib.sha256(data).hexdigest()

def symbol(obj,name):
    data,secs=score._elf(obj);ti=score._text_index(secs)
    found=[s for i,x in enumerate(secs) if x['type']==2 for s in score._symbol_table(data,secs,i)
           if s['name']==name and s['type']==2 and s['section']==ti]
    assert len(found)==1,(name,found)
    return found[0]

def inspect(obj,name):
    cmp=score.compare(obj,name,show=0)
    return dict(asdict(cmp),verdict=cmp.summary(),symbol_bytes=symbol(obj,name)['size'],
                native_bytes=4*len(score.targets()[name]))

def complete(result):
    return result['verdict']=='MATCH' and result['symbol_bytes']==result['native_bytes'] and not any(
        result[k] for k in ('differing','unresolved','unverified','errors','extra_words'))

def code_proof(work):
    obj=work/'candidate.o';score.compile_single(SOURCE,FLAGS,obj)
    result=inspect(obj,FN);assert complete(result),result
    data,secs=score._elf(obj);ti=score._text_index(secs);txt=secs[ti];fn=symbol(obj,FN)
    assert fn['value']==0 and fn['size']==260
    assert txt['size']==272 and not any(data[txt['off']+260:txt['off']+272])
    ro,=[s for s in secs if s['name']=='.rodata']
    owned=data[ro['off']:ro['off']+ro['size']]
    assert len(owned)==16 and owned==score.own_data().read(native.LITERAL,16)
    expected=struct.pack('>4f',4.16,1.0/3.4,1.0/46.3,1.0/.000055)
    assert owned==expected
    assert not any(s['size'] for s in secs if s['name'] in ('.data','.bss'))
    reloc=[]
    for s in secs:
        if s['type']==9 and s['info']==ti:
            symbols=score._symbol_table(data,secs,s['link'])
            for off in range(s['off'],s['off']+s['size'],8):
                at,info=struct.unpack_from('>II',data,off)
                reloc.append({'offset':at,'type':info&255,'symbol':symbols[info>>8]['name']})
    assert len(reloc)==8 and {r['symbol'] for r in reloc}=={'.rodata'}
    assert [r['type'] for r in reloc].count(5)==4 and [r['type'] for r in reloc].count(6)==4
    script=work/'link.ld';script.write_text('SECTIONS { .text 0x800D08E4 : SUBALIGN(4) { *(.text) }\n.rodata 0x80124138 : SUBALIGN(4) { *(.rodata) } }\n')
    elf=work/'candidate.elf'
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True)
    linked,sections=score._elf(elf);section=sections[score._text_index(sections)]
    body=linked[section['off']:section['off']+260];words=list(struct.unpack('>65I',body))
    assert words==score.targets()[FN]
    assert symbol(elf,FN)['value']==native.BASE and symbol(elf,FN)['size']==260
    linked_ro,=[s for s in sections if s['name']=='.rodata']
    assert linked[linked_ro['off']:linked_ro['off']+16]==owned
    shoff=struct.unpack_from('>I',linked,0x20)[0];shsize=struct.unpack_from('>H',linked,0x2e)[0]
    assert struct.unpack_from('>I',linked,shoff+sections.index(linked_ro)*shsize+12)[0]==native.LITERAL
    result.update(all_gnu_linked_words_equal=True,body_sha256=sha(body),relocations=reloc,
                  literal_address=hex(native.LITERAL),literal_bytes=16,literal_sha256=sha(owned),
                  excluded_zero_text_alignment_bytes=12)
    return result,words,owned


def context_proof(work):
    group=work/'context';group.mkdir()
    cfg=json.loads((CONTEXT/'group.json').read_text())
    original={name:sha((CONTEXT/name).read_bytes()) for name in cfg['files']}
    for name in cfg['files']:shutil.copyfile(CONTEXT/name,group/name)
    shutil.copyfile(SOURCE,group/'candidate.c');cfg['files'].append('candidate.c')
    cfg['members'].append(FN);cfg['keep'].append(FN)
    (group/'group.json').write_text(json.dumps(cfg));obj=group/'context.o';score.compile_group(group,obj)
    names=cfg['members']+cfg['context'];results={n:inspect(obj,n) for n in names}
    assert all(complete(v) for v in results.values()),results
    assert sha((group/'group.c').read_bytes())==original['group.c']
    return {'bodies':results,'unchanged_context_sha256':original,
            'original_recipe_sha256':sha((CONTEXT/'group.json').read_bytes()),
            'keep':cfg['keep'],'limit':'Separate existing type views; compiler context, not full-game shadow or caller execution.'}


def controls(work):
    source=SOURCE.read_text();result={}
    variants={
      'o2':(source,FLAGS.replace('-O3','-O2')),
      'donor_multiply_l2':(source.replace('tdes->l2 = tdes->k2+tdes->k2;','tdes->l2 = tdes->k2*2;'),FLAGS),
      'float_first_m2_reciprocal':(source.replace('(F32)(1.0/3.4)','(1.0f/3.4f)'),FLAGS),
    }
    for key,path in [('archived_a96','cloud/work/tiny_A96/track_preview_handler.c'),
                     ('archived_b27','cloud/work/near_miss_B27/track_preview_handler_natural.c')]:
        variants[key]=((ROOT/path).read_text(),FLAGS)
    for key,(text,flags) in variants.items():
        path=work/(key+'.c');path.write_text(text);obj=path.with_suffix('.o')
        score.compile_single(path,flags,obj);result[key]=inspect(obj,FN);result[key]['source_sha256']=sha(text.encode())
    assert complete(result['o2'])
    assert result['donor_multiply_l2']['symbol_bytes']==268 and result['donor_multiply_l2']['differing']>0
    assert result['float_first_m2_reciprocal']['differing']==0
    assert not complete(result['float_first_m2_reciprocal'])
    assert result['float_first_m2_reciprocal']['unverified']
    return result


def oracle(model,tire,scale,slot):
    out=bytearray(tire);r=native.f32
    value=lambda data,off:struct.unpack_from('>f',data,off)[0]
    load=value(model,1468);wheelbase=value(model,1480)
    stiffness=value(tire,12);friction=value(tire,16)
    z=r(r(r(load*scale)/wheelbase)*0.5)
    three=r(friction*3.0)
    k1=r(stiffness/z);k1sq=r(k1*k1)
    k2=r(k1sq/three)
    k3=r(r(k1sq*k1)/r(r(27.0*friction)*friction))
    m1=r(r(4.16)/friction);m1sq=r(m1*m1)
    output=[z,r(r(three*z)/stiffness),k1,k2,k3,r(k2+k2),r(k3*3.0),m1,
            r(m1sq*r(1.0/3.4)),r(r(m1sq*m1)*r(1.0/46.3)),r(1.0/.000055),0.0]
    assert all(math.isfinite(v) for v in output)
    for i,v in enumerate(output):struct.pack_into('>f',out,24+i*4,v)
    mout=bytearray(model)
    if slot>=0:mout[native.TIRE_OFFSET+slot*92:native.TIRE_OFFSET+(slot+1)*92]=out
    return bytes(mout),bytes(out)


def cases(count=4096):
    rng=random.Random(0xD08E4)
    edge=[-128.0,-3.5,-1.0,-0.125,0.125,1.0,3.5,128.0]
    for k in range(count):
        model=bytearray(rng.randbytes(2056));tire=bytearray(rng.randbytes(92))
        if k<512:
            load=edge[k%8];base=edge[(k//8)%8];scale=edge[(k//64)%8]
            stiffness=edge[(k*3+1)%8];friction=edge[(k*5+3)%8]
        else:
            load=rng.randint(1,60000)/16.0;base=rng.randint(1,2048)/32.0
            scale=rng.randint(1,2048)/64.0;stiffness=rng.randint(1,32000)/32.0;friction=rng.randint(1,1024)/128.0
        struct.pack_into('>f',model,1468,load);struct.pack_into('>f',model,1480,base)
        struct.pack_into('>f',tire,12,stiffness);struct.pack_into('>f',tire,16,friction)
        # Every valid original caller layout is exercised, plus disjoint objects.
        slot=k%5-1
        if slot>=0:model[1072+slot*92:1072+(slot+1)*92]=tire
        yield bytes(model),bytes(tire),scale,slot


def swap_words(data,offsets):
    out=bytearray(data)
    if sys.byteorder=='little':
        for off in offsets:out[off:off+4]=out[off:off+4][::-1]
    return bytes(out)

def host_model(data,slot):
    offsets=[1468,1480]
    if slot>=0:offsets+=list(range(1072+slot*92,1072+(slot+1)*92,4))
    return swap_words(data,offsets)


def host_library(work,mutant=None):
    path=SOURCE
    if mutant:
        before,after=MUTANTS[mutant];text=SOURCE.read_text();assert text.count(before)==1
        path=work/(mutant+'.c');path.write_text(text.replace(before,after))
    lib=work/((mutant or 'host')+'.so')
    cmd=['cc','-std=c89','-pedantic-errors','-O2','-shared','-fPIC','-Wall','-Wextra','-Werror',
         '-ffp-contract=off','-fsanitize=undefined','-fno-sanitize-recover=all',
         '-DCANDIDATE_SOURCE="%s"'%path,str(HERE/'host.c'),'-o',str(lib)]
    subprocess.run(cmd,check=True,capture_output=True)
    dll=ctypes.CDLL(str(lib));dll.run_case.argtypes=[ctypes.c_void_p,ctypes.c_void_p,ctypes.c_float,
                                                  ctypes.c_int,ctypes.c_void_p,ctypes.c_void_p]
    dll.run_case.restype=None
    return dll

def host_run(dll,model,tire,scale,slot):
    model_in=ctypes.create_string_buffer(host_model(model,slot));tire_in=ctypes.create_string_buffer(swap_words(tire,range(0,92,4)))
    model_out=ctypes.create_string_buffer(2056);tire_out=ctypes.create_string_buffer(92)
    dll.run_case(model_in,tire_in,scale,slot,model_out,tire_out)
    return host_model(model_out.raw,slot),swap_words(tire_out.raw,range(0,92,4))


def behavior(work,linked,owned,count):
    dll=host_library(work);target=score.targets()[FN];coverage=set();linked_coverage=set();digest=hashlib.sha256()
    layouts={i:0 for i in range(-1,4)}
    corpus=list(cases(count))
    for model,tire,scale,slot in corpus:
        expected=oracle(model,tire,scale,slot)
        machine=native.Machine(target,owned,model,tire,scale,slot)
        independent=native.Machine(linked,owned,model,tire,scale,slot)
        assert machine.run()==expected
        assert independent.run()==expected
        assert host_run(dll,model,tire,scale,slot)==expected
        assert machine.reads==independent.reads and machine.writes==independent.writes
        coverage|=machine.visited;linked_coverage|=independent.visited;layouts[slot]+=1
        digest.update(expected[0]+expected[1])
    assert coverage==linked_coverage==set(range(native.BASE,native.BASE+260,4))
    mutants={}
    for key in MUTANTS:
        wrong=host_library(work,key);rejected=0
        for case in corpus[:64]:
            rejected+=host_run(wrong,*case)!=oracle(*case)
        assert rejected>0,key
        mutants[key]={'rejected':True,'cases_disagreeing':rejected,'cases_checked':min(64,count)}
    return {'cases':count,'native_executions':count*2,'layouts':{str(k):v for k,v in layouts.items()},
            'native_target_instructions_executed':len(coverage),'linked_instructions_executed':len(linked_coverage),
            'host_c89_ubsan':'passed','whole_record_output_sha256':digest.hexdigest(),
            'saved_registers_stack_and_untouched_bytes':'passed','negative_controls':mutants,
            'limit':'Finite nonzero fixture denominators; no FCSR, NaN, infinity, invalid-pointer, concurrency or gameplay proof.'}


def callers():
    names=score.image_symbols();found=[]
    for name,words in score.targets().items():
        for i,w in enumerate(words):
            if w>>26==3 and (((names[name]+4*i+4)&0xf0000000)|((w&0x3ffffff)<<2))==names[FN]:
                found.append([name,hex(names[name]+4*i)])
    assert found==[['track_info_display','0x800d0e54'],['track_info_display','0x800d0e68']]
    return found


def verify(work,count=4096):
    obj,words,owned=code_proof(work)
    return {'status':'STRICT_MATCH_CANDIDATE','function':FN,'candidate_bytes':260,'accepted_byte_gain':0,
            'source_sha256':sha(SOURCE.read_bytes()),'object':obj,'context':context_proof(work),
            'controls':controls(work),'behavior':behavior(work,words,owned,count),'direct_callers':callers(),
            'target_manifest_sha256':sha((ROOT/'asm/us/blob/SHA256SUMS').read_bytes()),
            'own_data_manifest_sha256':sha((ROOT/'asm/us/blob_data/SHA256SUMS').read_bytes()),
            'scorer_sha256':sha((ROOT/'tools/cloud/score.py').read_bytes()),
            'packet_sha256':{n:sha((HERE/n).read_bytes()) for n in ['native.py','host.c','verify.py','provenance.json','claim.json']}}


def main():
    parser=argparse.ArgumentParser();parser.add_argument('--write',action='store_true');parser.add_argument('--cases',type=int,default=4096);args=parser.parse_args()
    assert args.cases>=64
    with tempfile.TemporaryDirectory(prefix='tire-constants-proof-') as directory:result=verify(Path(directory),args.cases)
    if args.write:(HERE/'verification.json').write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    elif args.cases==4096:assert result==json.loads((HERE/'verification.json').read_text()),'receipt differs; inspect before --write'
    print(json.dumps(result,indent=2,sort_keys=True))

if __name__=='__main__':main()
