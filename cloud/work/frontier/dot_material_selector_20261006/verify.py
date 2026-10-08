#!/usr/bin/env python3
"""Source-bound whole-ELF, independent GNU, and bounded native/host verification."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import subprocess
import sys
import tempfile

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
from tools.conveyor.pipeline import blob_group
spec=importlib.util.spec_from_file_location('material_native',HERE/'native.py')
native=importlib.util.module_from_spec(spec)
spec.loader.exec_module(native)
FN='func_8008B000'
CONSUMER='car_gear_shift'
SOURCE=HERE/'candidate.c'
OLD=ROOT/'cloud/work/near-miss/func_8008B000/base.c'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
HISTORICAL_FLAGS='-g0 -O2 -mips2 -G 0 -non_shared'
BASE='cd22879d40b3de443cfde047b86e75e159b6cec6'


def digest(data): return hashlib.sha256(data).hexdigest()
def sha(path): return digest(Path(path).read_bytes())
def packed(words): return struct.pack('>%dI'%len(words),*words)
def run(args,**kw): return subprocess.run(args,check=True,capture_output=True,**kw)

def base_bytes(path):
    return run(['git','show',BASE+':'+path],cwd=ROOT).stdout

def portable(receipt):
    result=json.loads(json.dumps(receipt))
    for field in ('target_manifest_sha256','accepted_consumer_source_sha256'):
        result.pop(field,None)
    for path in ('tools/cloud/score.py','tools/cloud/owndata.py','tools/conveyor/pipeline/blob_group.py'):
        result.get('tools_sha256', {}).pop(path,None)
    if not result.get('tools_sha256'):
        result.pop('tools_sha256', None)
    return result

def elf(path):
    data,sections=score._elf(path)
    symbols=[s for i,sec in enumerate(sections) if sec['type']==2
             for s in score._symbol_table(data,sections,i)]
    return data,sections,symbols


def inspect(obj,name,work,exact=True):
    data,sections,symbols=elf(obj)
    fn,=[s for s in symbols if s['name']==name and s['type']==2]
    assert fn['size']%4==0 and fn['size']>0
    target=score.targets()[name]
    addresses=score.image_symbols()
    readelf=run(['mips-linux-gnu-readelf','-Ws',str(obj)],text=True).stdout
    assert re.search(r'\b'+str(fn['size'])+r'\s+FUNC\s+GLOBAL\s+DEFAULT\s+\d+\s+'+name+r'\b',readelf)
    relocs=[]
    for sec in sections:
        if sec['type']!=9 or sec['info']!=fn['section']: continue
        table=score._symbol_table(data,sections,sec['link'])
        for off in range(sec['off'],sec['off']+sec['size'],8):
            site,info=struct.unpack_from('>II',data,off)
            if fn['value']<=site<fn['value']+fn['size']:
                relocs.append(dict(offset=site-fn['value'],type=info&255,symbol=table[info>>8]['name']))
    assert all(not s['size'] for s in sections if s['name'] in ('.data','.bss','.rodata','.rdata','.sdata','.sbss','.lit4','.lit8'))
    definitions=[]
    for sym in symbols:
        if sym['section']==0 and sym['name']:
            address=addresses.get(sym['name'],score.address_named(sym['name']))
            assert address is not None,sym['name']
            definitions.append('%s = 0x%x;'%(sym['name'],address))
    script=work/(name+'.ld')
    script.write_text('SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) }\n /DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n'%(addresses[name]-fn['value'])+'\n'.join(definitions))
    linked=work/(name+'.elf')
    run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(linked),str(obj)])
    lddata,ldsections,ldsymbols=elf(linked)
    ldfn,=[s for s in ldsymbols if s['name']==name and s['type']==2]
    assert ldfn['value']==addresses[name] and ldfn['size']==fn['size']
    shoff,=struct.unpack_from('>I',lddata,32)
    shsize,=struct.unpack_from('>H',lddata,46)
    section_address,=struct.unpack_from('>I',lddata,shoff+shsize*ldfn['section']+12)
    start=ldsections[ldfn['section']]['off']+ldfn['value']-section_address
    body=lddata[start:start+ldfn['size']]
    words=list(struct.unpack('>%dI'%(len(body)//4),body))
    differences=[i*4 for i in range(max(len(target),len(words)))
                 if i>=len(target) or i>=len(words) or target[i]!=words[i]]
    comparison=score.compare(obj,name,show=0)
    text=sections[fn['section']]
    raw=list(struct.unpack('>%dI'%(text['size']//4),data[text['off']:text['off']+text['size']]))
    resolved,masks,unresolved,unverified,errors=score.relocate(obj,raw,fn['value'],fn['value']+fn['size'],addresses)
    full=resolved[fn['value']//4:(fn['value']+fn['size'])//4]
    assert not(masks or unresolved or unverified or errors)
    assert full==words
    if exact: assert not differences and comparison.accepted()
    return dict(canonical=asdict(comparison),canonical_verdict=comparison.summary(),
                elf_bytes=fn['size'],native_bytes=len(target)*4,
                complete_differing_offsets=differences,gnu_sha256=digest(body),
                relocations=relocs,no_owned_data=True),words


def host_run(work,corpus,source=None):
    exe=work/'host'
    cmd=['cc','-std=c89','-pedantic-errors','-O2','-Wall','-Wextra',
         '-fsanitize=undefined','-fno-sanitize-recover=all']
    if source: cmd+=['-DCANDIDATE="'+str(source)+'"']
    run(cmd+[str(HERE/'host.c'),'-o',str(exe)])
    return run([str(exe)],input=corpus).stdout


def semantics(work,linked):
    target=score.targets()[FN]
    inputs,outputs=[],[]
    coverage,branches=set(),set()
    cases=0
    for bank in (0,1,31,63):
      for index in (0,1,511,1023):
       for count in (-32768,-1,0,1,2,3,4):
        for selector in (-1,0,1,2,3,4,5,32767):
         for value in (0,1,32767,32768,65535):
            cases+=1
            initial,args,address=native.fixture(bank*1024+index,selector,value,count,cases)
            inputs.append(packed(args)+bytes(initial[address+i] for i in range(88)))
            a,b,expected=initial.copy(),initial.copy(),initial.copy()
            for repeat in range(2):
                want=native.oracle(expected,args)
                na=native.execute(target,a,args,cases)
                nb=native.execute(linked,b,args,cases)
                assert a==b==expected and na==nb and na[0]==want
                assert na[3][:3]==[(native.STACK+4*i,4,args[i]) for i in (1,2,0)]
                # Compare every memory byte including all bank, record and stack guards.
                coverage.update(na[1]);branches.update(na[4])
                outputs.append(packed([want])+bytes(expected[address+i] for i in range(88)))
                args=args[:];args[2]^=65535
    corpus=b''.join(inputs);expected_output=b''.join(outputs)
    host=host_run(work,corpus)
    assert host==expected_output
    # +0x78 is the unreachable duplicate load between return and branch destination.
    assert coverage==set(range(0,216,4))-{0x78}
    assert branches=={(off,take) for off in (0x50,0x68,0x98,0xc4) for take in (False,True)}|{(0x84,True)}, branches
    source=SOURCE.read_text()
    mutations={
      'fallthrough_after_single':source.replace('} else {','}\n    {'),
      'moving_flag_in_bulk':source.replace('model->spans[selector + 1].flags |= 0x8000;\n        }','model->spans[i + 1].flags |= 0x8000;\n        }'),
      'wrong_handle_mask':source.replace('handle & 1023','handle & 511'),
      'wrong_flag_bit':source.replace('0x8000','0x4000'),
      'zero_selects_bulk':source.replace('selector >= 0','selector > 0')}
    rejected=[]
    for name,text in mutations.items():
        assert text!=source
        mutant=work/(name+'.c');mutant.write_text(text)
        try: got=host_run(work,corpus,mutant)
        except subprocess.CalledProcessError: rejected.append(name);continue
        assert got!=expected_output,name
        rejected.append(name)
    # Interpreter controls are fatal, not silently treated as nops/zero memory.
    initial,args,address=native.fixture(0,0,7,1,42)
    malformed=[('unknown_opcode',[0xffffffff]+target[1:]),('truncated',target[:-1])]
    for label,words in malformed:
        try: native.execute(words,initial.copy(),args)
        except (AssertionError,KeyError): continue
        raise AssertionError(label+' was accepted')
    absent=initial.copy();del absent[native.TABLE]
    try: native.execute(target,absent,args)
    except (AssertionError,KeyError): pass
    else: raise AssertionError('unmapped table was accepted')
    return dict(cases=cases,native_and_gnu_executions=cases*4,host_invocations=cases*2,
      reachable_instruction_offsets=sorted(coverage),unreachable_duplicate_load_offset=120,
      branch_outcomes=sorted([list(x) for x in branches]),
      full_memory_and_trace_equal=True,argument_homes_and_saved_registers=True,
      host_c89_ubsan=True,compiled_semantic_mutants_rejected=rejected,
      malformed_controls_rejected=['unknown_opcode','truncated','unmapped_table'],
      corpus_sha256=digest(corpus),output_sha256=digest(host))


def audit_native():
    targets=score.targets();symbols=score.image_symbols()
    word=0x0c000000|((symbols[FN]>>2)&0x03ffffff)
    sites=[(name,4*i) for name,words in targets.items() for i,w in enumerate(words) if w==word]
    assert sites==[('physics_velocity_integrate_a',0x290)]
    caller=targets[sites[0][0]]
    delay=caller[0x294//4]
    assert delay>>26==0 and delay&63==37 and (delay>>11)&31==5 and (delay>>16)&31==0 and (delay>>21)&31==0 # a1 = zero
    # Independent native table/stride consumer and fixed four-slot traversal.
    consumer=targets[CONSUMER]
    renderer=targets['entity_render_mode']
    def immediates(words):
        return [(w>>26,(w>>21)&31,(w>>16)&31,native.signed(w,16)) for w in words]
    assert (9,0,21,88) in immediates(consumer)
    assert (9,16,16,68) in immediates(consumer)
    assert (9,0,19,64) in immediates(renderer)
    assert (9,16,16,16) in immediates(renderer)
    assert (9,17,17,16) in immediates(renderer)
    assert (9,20,20,88) in immediates(renderer)
    names=targets['string_copy_format']
    call=names[0xac//4]
    assert call>>26==3 and (call&0x03ffffff)<<2==(symbols['func_80092DCC']&0x0fffffff)
    assert immediates([names[0xb0//4]])==[(9,0,6,16)]
    assert immediates([names[0x130//4]])==[(9,0,7,88)]
    comparison=targets['validate_and_call']
    assert immediates([comparison[0x20//4]])==[(9,0,6,15)]
    return sites


def verify(work):
    work.mkdir(parents=True,exist_ok=True)
    targets=score.targets();addresses=score.image_symbols()
    result=dict(base_revision=BASE,status='STRICT_MATCH_RESEARCH_ALREADY_ACCEPTED_TARGET',claims=[],
        native_range=['0x8008B000','0x8008B0D8'],candidate_bytes=216,accepted_byte_gain=0,
        source_sha256=sha(SOURCE),flags=FLAGS,assembler_erratum_flag=score.R4300_CC)
    for opt in ('O2','O3'):
        directory=work/opt;directory.mkdir()
        obj=directory/'candidate.o';score.compile_single(SOURCE,HISTORICAL_FLAGS.replace('O2',opt),obj)
        result[opt],body=inspect(obj,FN,directory)
        assert result[opt]['relocations']==[dict(offset=0x18,type=5,symbol='D_801161F4'),dict(offset=0x24,type=6,symbol='D_801161F4')]
        data,sections,syms=elf(obj);text=sections[score._text_index(sections)]
        assert text['size']==224 and data[text['off']+216:text['off']+224]==bytes(8)
    result['zero_alignment_bytes_outside_function']=8
    # Only fixed, authentic shared-data consumer context. No invented caller.
    context=work/'context';context.mkdir()
    (context/'candidate.c').write_bytes(SOURCE.read_bytes())
    accepted=base_bytes('src/blob/car_gear_shift.c')
    (context/'consumer.c').write_bytes(accepted)
    (context/'group.json').write_text(json.dumps(dict(files=['candidate.c','consumer.c'],
       members=[FN],context=[CONSUMER],keep=[FN,CONSUMER],flags=FLAGS.replace('O2','O3'),claims=[FN])))
    obj=context/'group.o';score.compile_group(context,obj)
    result['shared_data_context'],ctx=inspect(obj,FN,context)
    result['accepted_consumer'],_=inspect(obj,CONSUMER,context)
    assert ctx==body
    extents={n:dict(vaddr=addresses[n],size=len(targets[n])*4) for n in (FN,CONSUMER)}
    slices,index=blob_group.member_slices(obj,[FN,CONSUMER],extents)
    relocated=blob_group.relocate(obj,slices,index,addresses,members=[FN,CONSUMER])
    assert all(relocated[n]==packed(targets[n]) for n in (FN,CONSUMER))
    result['production_reader_equal']=[FN,CONSUMER]
    assertions=work/'layout.c'
    checks={'span16':'sizeof(RecordSpan16)==16','record88':'sizeof(ModelRecord)==88',
      'bank8':'sizeof(ModelBank)==8','header_flags10':'O(ModelRecord,spans[0].flags)==10',
      'count22':'O(ModelRecord,spans[0].tail)==22','slot0_value24':'O(ModelRecord,spans[1].value)==24',
      'slot0_flags26':'O(ModelRecord,spans[1].flags)==26','slot3_value72':'O(ModelRecord,spans[4].value)==72'}
    assertions.write_text('#include "'+str(SOURCE)+'"\n#define O(T,m) ((unsigned int)&(((T*)0)->m))\n'+
       '\n'.join('typedef char check_%s[(%s)?1:-1];'%(key,value) for key,value in checks.items()))
    score.compile_single(assertions,FLAGS,work/'layout.o')
    result['o32_layout_assertions']=checks
    baseline=work/'baseline';baseline.mkdir()
    archived=baseline/'old.c';archived.write_bytes(base_bytes(str(OLD.relative_to(ROOT))))
    score.compile_single(archived,HISTORICAL_FLAGS,baseline/'old.o')
    result['archived_seed'],_=inspect(baseline/'old.o',FN,baseline,False)
    result['archived_source_sha256']=digest(archived.read_bytes())
    result['archived_control_flags']=HISTORICAL_FLAGS
    result['behavior']=semantics(work,body)
    sites=audit_native()
    result['native_direct_call_sites']=[dict(function=n,offset=hex(off),selector=0) for n,off in sites]
    result['input_native_sha256']={n:digest(packed(targets[n])) for n in (FN,CONSUMER,'entity_render_mode','physics_velocity_integrate_a','string_copy_format','validate_and_call')}
    used={FN,CONSUMER,'func_80092DCC'}|{r['symbol'] for key in ('O2','accepted_consumer') for r in result[key]['relocations']}
    result['consumed_symbols']={n:hex(addresses.get(n,score.address_named(n))) for n in sorted(used)}
    result['compiler_sha256']={n:sha(score.ido(n)) for n in ('cc','cfe','uld','usplit','umerge','uopt','ugen','as1')}
    result['packet_sha256']={n:sha(HERE/n) for n in ('verify.py','native.py','host.c','claim.json')}
    result['limitations']=['No original typedef, donor ancestry, or original TU membership claim.',
      'Direct caller is read-only: its full private-ABI closure is not recompiled or executed.',
      'Shared-data consumer context is not proof of a recovered contiguous translation unit.',
      'Negative selector -1 is instruction-proven but its gameplay use is not established.',
      'Mapped records, signed count <=4, selectors >=-1; invalid pointers, larger capacities, and lower selectors excluded.',
      'Finite deterministic proof; no concurrency, gameplay, full-game shadow/image/compression/ROM gates.']
    return result

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path)
    args=parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='material-selector-proof-') as tmp: result=verify(Path(tmp))
    if args.output: args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ('status','O2','behavior')},indent=2))
