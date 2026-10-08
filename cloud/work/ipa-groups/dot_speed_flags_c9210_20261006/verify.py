#!/usr/bin/env python3
"""Full ELF, GNU relocation and bounded native/host proof; no native dumps written."""
import argparse
import dataclasses
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import random
import re
import struct
import subprocess
import sys
import tempfile

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=importlib.util.spec_from_file_location('speed_native',HERE/'native.py')
native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
NAME='speed_set'
NAMES=[NAME,'vsync_wait','speed_mode0_wrapper','speed_mode1_wrapper','continue_prompt']
BASE='cd22879d40b3de443cfde047b86e75e159b6cec6'
BASE_SOURCE='src/blob/groups/codex_vsync_a145/group.c'

def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def digest(data): return hashlib.sha256(data).hexdigest()
def run(args,**kwargs): return subprocess.run(args,check=True,capture_output=True,**kwargs)
def raw(words): return struct.pack('>%dI'%len(words),*words)
def git(path): return run(['git','show',BASE+':'+path],cwd=ROOT).stdout

def check_recipe(cfg):
    baseline=json.loads(git(BASE_SOURCE.replace('group.c','group.json')))
    for field in ['files','keep','flags']: assert cfg[field]==baseline[field],field
    assert cfg['members']==[NAME] and cfg['claims']==[NAME]
    assert set(cfg['context'])==set(NAMES)-{NAME}

def native_identity():
    addresses=score.image_symbols()
    assert addresses[NAME]==0x800C9210
    assert addresses['vsync_wait']==0x800FB234
    bodies=score.targets()
    assert len(bodies[NAME])==51 and len(bodies['vsync_wait'])==37
    return {n:{'start':'0x%08X'%addresses[n],
               'end_exclusive':'0x%08X'%(addresses[n]+4*len(bodies[n]))} for n in NAMES}

def caller_audit():
    census=json.loads((HERE/'callers.json').read_text())
    addresses=score.image_symbols();bodies=score.targets()
    seen=[]
    for name,words in bodies.items():
        for index,word in enumerate(words):
            if word>>26==3 and ((word&0x3FFFFFF)<<2)==(addresses[NAME]&0x0FFFFFFF):
                seen.append((name,addresses[name]+index*4))
    assert sorted(seen)==sorted((row['caller'],int(row['jal'],16)) for row in census['sites'])
    assert len(seen)==21 and len(set(n for n,pc in seen))==13
    for row in census['sites']:
        start=addresses[row['caller']];words=bodies[row['caller']]
        def at(pc):
            offset=int(pc,16)-start
            assert offset>=0 and offset%4==0 and offset//4<len(words)
            return words[offset//4]
        for field,register in [('first',17),('second',18)]:
            item=row[field];w=at(item['pc']);op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31
            if op==9:
                assert rs==0 and rt==register and (w&65535)==item['value']
            else:
                assert op==0 and (w&63) in (33,37) and rd==register and rt==0
                if rs==0: assert item['value']==0
                else:
                    assert rs==20 and item['value']==1 and row['s4_definition']
                    definition=at(row['s4_definition'])
                    assert definition>>26==9 and (definition>>21)&31==0 and (definition>>16)&31==20 and definition&65535==1
                    # Any repeated direct s4 definition preserves the same constant one.
                    first=(int(row['s4_definition'],16)-start)//4+1
                    last=(int(item['pc'],16)-start)//4
                    for previous in words[first:last]:
                        code=previous>>26
                        if code==0 and (previous&63) not in (8,9,12,13,24,25,26,27): assert (previous>>11)&31!=20
                        if code in (8,9,10,11,12,13,14,15,32,33,34,35,36,37,38) and (previous>>16)&31==20: assert previous==definition
    names=sorted(set(n for n,pc in seen))
    return {'direct_jal_sites':len(seen),'distinct_callers':len(names),
            'all_witnessed_flag_values':[0,1],
            'native_body_sha256':{n:digest(raw(bodies[n])) for n in names},
            'native_starts':{n:'0x%08X'%addresses[n] for n in names},'scope':census['scope']}

def gnu_functions(obj):
    text=run(['mips-linux-gnu-readelf','-Ws',str(obj)],text=True).stdout
    rows={}
    for line in text.splitlines():
        m=re.match(r'\s*\d+: ([0-9a-f]+)\s+(\d+)\s+FUNC\s+\S+\s+\S+\s+(\d+)\s+(\S+)\s*$',line)
        if m: rows[m[4]]=(int(m[1],16),int(m[2]))
    assert set(rows)==set(NAMES)
    return rows

def summaries(obj):
    syms=gnu_functions(obj)
    out={}
    for name in NAMES:
        result=dataclasses.asdict(score.compare(obj,name,show=0))
        result['elf_extent_bytes']=syms[name][1]
        result['native_extent_bytes']=len(score.targets()[name])*4
        result['complete_extent_differing_positions']=result['differing']+max(0,(syms[name][1]-result['native_extent_bytes'])//4)
        out[name]=result
    return out

def cases():
    floats=[0,0x80000000,0x3F800000,0x3F000000,0x3F7FFFFF,0x3F800001,
            0xBF800000,0xBF000000,0x00800000,0x80800000,0x7F7FFFFF,0xFF7FFFFF]
    flags=[0,1,127,128,255,256,257,0x7FFFFFFF,0x80000000,0xFFFFFFFF,0x12345678,0xFFFFFF80]
    out=[(a,b,x,y,mode) for a in floats for b in floats for x,y in zip(flags,flags[::-1]) for mode in range(3)]
    out += [(0x3F000000,0x40000000,i,(i*73+19)&255,mode) for i in range(256) for mode in range(3)]
    rng=random.Random(0xC9210)
    for _ in range(2048):
        a,b=rng.getrandbits(32),rng.getrandbits(32)
        if (a>>23)&255 in (0,255): a=0x3F000000
        if (b>>23)&255 in (0,255): b=0xBF800000
        out.append((a,b,rng.getrandbits(32),rng.getrandbits(32),rng.randrange(3)))
    return out

def host(work,source,corpus,label):
    group=work/(label+'_group.c');group.write_text(source)
    src=work/(label+'_host.c');src.write_text((HERE/'host.c').read_text().replace('#include "group.c"','#include "'+str(group)+'"'))
    exe=work/(label+'_host')
    run(['cc','-std=c89','-O1','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-fno-sanitize-recover=all',str(src),'-o',str(exe)])
    inputs=''.join(' '.join(map(str,c))+'\n' for c in corpus)
    output=run([str(exe)],input=inputs,text=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0')).stdout
    return [list(map(int,line.split())) for line in output.splitlines()]

def behavior(words,linked,work):
    corpus=cases();expect=[native.oracle(c) for c in corpus]
    coverage,branches=set(),set()
    for c,want in zip(corpus,expect):
        for body in [words,linked]:
            got,seen,taken=native.execute(body,c)
            assert got==want,c
            coverage |= seen;branches |= taken
    # Two dead fallthrough copies are compiler-emitted unreachable instructions.
    assert coverage==set(range(0,204,4))-{120,152},sorted(set(range(0,204,4))-coverage)
    assert {pc for pc,t in branches if t and (pc,False) in branches}=={68,104,136}
    source=(HERE/'group.c').read_text()
    assert host(work,source,corpus,'good')==expect
    variants={
        'omit_blend_upper_clamp':source.replace('if (blend > 1.0f)','if (blend > 2.0f)'),
        'omit_amount_lower_clamp':source.replace('if (amount < 0.0f)','if (amount < -1.0f)'),
        'first_flag_zero':source.replace('(s8) first','(s8) 0 * first'),
        'swap_flag_destinations':source.replace('s8 *, 0xC','s8 *, 0xE').replace('s8 *, 0xD','s8 *, 0xC').replace('s8 *, 0xE','s8 *, 0xD'),
    }
    mutants={}
    for name,text in variants.items():
        assert text!=source
        got=host(work,text,corpus,name)
        differences=sum(a!=b for a,b in zip(got,expect));assert len(got)==len(expect) and differences>0
        mutants[name]=differences
    bad={}
    controls={
        'unknown_instruction':(0,0xFFFFFFFF),
        'wrong_command_byte':(11,(words[11]&0xFFFF0000)|11),
        'redirect_record_store':(14,(words[14]&0xFFFF0000)|3),
        'wrong_saved_return_slot':(3,(words[3]&0xFFFF0000)|16),
    }
    for name,(index,value) in controls.items():
        altered=words[:];altered[index]=value
        rejected=False
        try:
            got,_,_=native.execute(altered,corpus[0]);assert got==expect[0]
        except AssertionError: rejected=True
        assert rejected,name;bad[name]='rejected'
    try: native.execute(words[:-1],corpus[0])
    except AssertionError: bad['truncated_body']='rejected'
    assert 'truncated_body' in bad
    return {'cases':len(corpus),'native_executions':len(corpus)*2,'host_c89_asan_ubsan':'passed',
            'executed_instruction_offsets':len(coverage),'unreachable_offsets':[120,152],
            'conditional_branch_outcomes':len(branches),'full_record_call_snapshots':'equal',
            'stack_canaries_and_documented_register_contract':'passed','source_mutants_rejected':mutants,
            'native_negative_controls':bad,
            'scope':'Finite normal values and signed zero; 32-bit flag carriers truncated at byte stores. No NaN, subnormal, infinity, FCSR exception or concurrency proof.'}

def build(work):
    check_recipe(json.loads((HERE/'group.json').read_text()))
    identities=native_identity()
    source=(HERE/'group.c').read_bytes();old=git(BASE_SOURCE)
    assert old.replace(b's8 first,s8 second',b's32 first,s32 second')==source
    obj=work/'group.o';score.compile_group(HERE,obj)
    rows=gnu_functions(obj);result=summaries(obj)
    assert result[NAME]['elf_extent_bytes']==204 and score.compare(obj,NAME,show=0).accepted()
    assert result['vsync_wait']['elf_extent_bytes']==148 and score.compare(obj,'vsync_wait',show=0).accepted()
    data,sections=score._elf(obj)
    own={s['name']:s['size'] for s in sections if s['name'] in ('.rodata','.data','.bss','.sdata','.sbss') and s['size']}
    assert not own
    # Link the unmodified entire object. The internal target is first at native C9210,
    # so every section-relative JAL to it also resolves independently and correctly.
    assert rows[NAME][0]==0
    addresses=score.image_symbols()
    ext=[sym['name'] for i,s in enumerate(sections) if s['type']==2 for sym in score._symbol_table(data,sections,i) if sym['section']==0 and sym['name']]
    script=work/'link.ld';script.write_text('SECTIONS { .text 0x800C9210 : { *(.text) } }\n'+''.join('%s = 0x%X;\n'%(n,addresses.get(n,score.address_named(n))) for n in ext))
    elf=work/'linked.elf';binary=work/'linked.bin'
    run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)])
    run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(elf),str(binary)])
    blob=binary.read_bytes();linked_functions=gnu_functions(elf)
    strict={}
    for name,(offset,size) in rows.items():
        assert linked_functions[name]==(0x800C9210+offset,size)
        got=blob[offset:offset+size];want=raw(score.targets()[name]);limit=max(size,len(want))
        difference=sum(got[i:i+4]!=want[i:i+4] for i in range(0,limit,4))
        strict[name]={'bytes':size,'differing_complete_positions':difference,'relocated_sha256':digest(got)}
        assert difference==result[name]['differing']+max(0,(size-len(want))//4)
    assert strict[NAME]['differing_complete_positions']==strict['vsync_wait']['differing_complete_positions']==0
    end=max(a+b for a,b in rows.values());assert len(blob)==624 and end==620 and not any(blob[end:])
    reltext=run(['mips-linux-gnu-readelf','-r',str(obj)],text=True).stdout
    relocations=[line for line in reltext.splitlines() if 'R_MIPS_' in line]
    assert len(relocations)==20
    count=sum(int(line.split()[0],16)<204 for line in relocations);assert count==8
    # One causal compiler control, exact accepted source before the two-formal repair.
    baseline=work/'baseline';baseline.mkdir();(baseline/'group.c').write_bytes(old);(baseline/'group.json').write_bytes((HERE/'group.json').read_bytes())
    baseobj=work/'baseline.o';score.compile_group(baseline,baseobj);prior=summaries(baseobj)
    assert prior[NAME]['differing']==47 and prior[NAME]['elf_extent_bytes']==228, prior
    assert score.compare(baseobj,'vsync_wait',show=0).accepted()
    words=score.targets()[NAME]
    return {'schema':1,'base':BASE,'status':'MATCH','accepted_byte_gain':0,
        'target':{'name':NAME,'start':'0x800C9210','end_exclusive':'0x800C92DC','bytes':204,'words':51,'sha256':digest(raw(words))},
        'inputs':{str(p.relative_to(ROOT)):sha(p) for p in [HERE/'group.c',HERE/'group.json',HERE/'verify.py',HERE/'host.c',HERE/'native.py',HERE/'callers.json']},
        'flags':json.loads((HERE/'group.json').read_text())['flags'],
        'caller_audit':caller_audit(),
        'native_intervals':identities,
        'native_body_sha256':{n:digest(raw(score.targets()[n])) for n in NAMES},
        'compiler_sha256':{n:sha(score.ido(n)) for n in ['cc','cfe','uld','umerge','uopt','ugen','as1']},
        'current_comparisons':result,'original_s8_control':prior,'gnu_complete_body_comparisons':strict,
        'gnu_total_relocations':len(relocations),'target_relocations':count,'owned_data_sections':own,'zero_alignment_bytes':len(blob)-end,
        'external_symbol_bindings':{n:'0x%08X'%addresses.get(n,score.address_named(n)) for n in ext},
        'behavior':behavior(words,list(struct.unpack('>51I',blob[:204])),work)}

def portable(receipt):
    result=json.loads(json.dumps(receipt))
    result.pop('accepted_source_sha256',None)
    return result

def binding():
    receipt=json.loads((HERE/'verification.json').read_text())
    check_recipe(json.loads((HERE/'group.json').read_text()))
    assert native_identity()==receipt['native_intervals']
    assert caller_audit()==receipt['caller_audit']
    for path,expected in receipt['inputs'].items(): assert sha(ROOT/path)==expected,path
    for n,expected in receipt['native_body_sha256'].items(): assert digest(raw(score.targets()[n]))==expected,n
    assert digest(raw(score.targets()[NAME]))==receipt['target']['sha256']
    addresses=score.image_symbols()
    for n,expected in receipt['external_symbol_bindings'].items(): assert '0x%08X'%addresses.get(n,score.address_named(n))==expected,n
    return receipt

def main():
    p=argparse.ArgumentParser();p.add_argument('--record',action='store_true');p.add_argument('--compiler',action='store_true');a=p.parse_args()
    if a.record or a.compiler:
        with tempfile.TemporaryDirectory(prefix='speed-flags-proof-') as temp: receipt=build(Path(temp))
        if a.record: (HERE/'verification.json').write_text(json.dumps(receipt,indent=2,sort_keys=True)+'\n')
        else: assert portable(receipt)==portable(binding()),'fresh replay differs from receipt'
    else: receipt=binding()
    print('speed_set: MATCH, 204 bytes; %d bounded cases'%receipt['behavior']['cases'])
if __name__=='__main__':main()
