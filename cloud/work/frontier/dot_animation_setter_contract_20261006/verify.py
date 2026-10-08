#!/usr/bin/env python3
"""Reproduce bounded source, complete ELF, GNU-link and behavioral evidence."""
import argparse
import dataclasses
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
import tempfile

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=importlib.util.spec_from_file_location('animation_native',HERE/'native.py')
native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
NAME='audio_channel_setup'
CONTEXT='func_80090770'
ENTRY_ADDRESSES={NAME:0x80094888,CONTEXT:0x80090770,'func_8010E694':0x8010E694}
BASE='cd22879d40b3de443cfde047b86e75e159b6cec6'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'


def sha(data):return hashlib.sha256(data).hexdigest()
def file_sha(path):return sha(Path(path).read_bytes())
def run(command,**kw):return subprocess.run(command,check=True,capture_output=True,**kw)


def base_bytes(path):
    return run(['git','show',BASE+':'+path],cwd=ROOT).stdout


def elf(obj):
    data,sections=score._elf(obj)
    symbols=[s for i,section in enumerate(sections) if section['type']==2 for s in score._symbol_table(data,sections,i)]
    return data,sections,symbols


def function(obj,name):
    data,sections,symbols=elf(obj)
    found=[s for s in symbols if s['name']==name and s['type']==2]
    assert len(found)==1,'ambiguous function symbol'
    symbol=found[0];section=sections[symbol['section']]
    assert symbol['size']>0 and symbol['size']%4==0
    assert section['name']=='.text'
    start=symbol['value'];size=symbol['size']
    assert start+size<=section['size']
    raw=data[section['off']+start:section['off']+start+size]
    return start,size,raw


def comparison(obj,name):
    report=dataclasses.asdict(score.compare(obj,name,show=0))
    start,size,raw=function(obj,name)
    report['elf_function_bytes']=size
    report['function_unrelocated_sha256']=sha(raw)
    return report


def link_body(obj,name,addresses,work,label):
    start,size,raw=function(obj,name)
    data,sections,symbols=elf(obj)
    own={s['name']:s['size'] for s in sections if s['name'] in ['.data','.rodata','.rdata','.sdata','.bss','.sbss'] and s['size']}
    assert not own,('unexpected owned data',own)
    functions=score.symbols(obj)
    addresses_used={}
    relocation_count=0
    for sec in sections:
        if sec['type']!=9 or sections[sec['info']]['name']!='.text':continue
        table=score._symbol_table(data,sections,sec['link'])
        for offset in range(sec['off'],sec['off']+sec['size'],8):
            position,info=struct.unpack_from('>II',data,offset)
            if start<=position<start+size:
                relocation_count+=1
                sym=table[info>>8]
                assert sym['name'] in addresses,('unexpected relocation',sym['name'])
                addresses_used[sym['name']]=addresses[sym['name']]
    script=work/(label+'.ld');linked=work/(label+'.elf');binary=work/(label+'.bin')
    script.write_text('SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } }\n'%(addresses[name]-start)+''.join('%s = 0x%x;\n'%(k,v) for k,v in addresses.items() if k not in functions))
    run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(linked),str(obj)])
    readelf=run(['mips-linux-gnu-readelf','-sW',str(linked)],text=True).stdout
    rows=[line.split() for line in readelf.splitlines() if line.split() and line.split()[-1]==name]
    assert len(rows)==1 and int(rows[0][1],16)==addresses[name] and int(rows[0][2])==size
    run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(linked),str(binary)])
    linked_bytes=binary.read_bytes()[start:start+size]
    words=score.text_words(obj)
    relocated,masks,unresolved,unverified,errors=score.relocate(obj,words,start,start+size,addresses)
    assert not masks and not unresolved and not unverified and not errors
    project=struct.pack('>%dI'%(size//4),*relocated[start//4:(start+size)//4])
    assert linked_bytes==project,'independent GNU relocation disagreement'
    target=score.targets()[name]
    target_bytes=struct.pack('>%dI'%len(target),*target)
    linked_words=list(struct.unpack('>%dI'%(size//4),linked_bytes))
    differences=[i*4 for i in range(max(len(linked_words),len(target))) if i>=len(linked_words) or i>=len(target) or linked_words[i]!=target[i]]
    return linked_words,{'elf_function_bytes':size,'full_target_bytes':len(target_bytes),
        'complete_differing_offsets':differences,'linked_sha256':sha(linked_bytes),'target_sha256':sha(target_bytes),
        'gnu_and_project_equal':True,'gnu_readelf_size_verified':True,'owned_data_bytes':0,
        'relocation_count':relocation_count,'consumed_symbols':dict(sorted(addresses_used.items()))}


def corpus():
    out=[]
    # Cross every cleanup priority, timer boundary, frame equality and callback mutation.
    for mode,hold,step,timer,delta,count,flags,mutation in itertools.product(
        [0,1,-1],[0,1],[0,2],[-.0625,0,.0625,.125],[0,.0625],[0,3],[0,0x1000,0x2000,0x3000],[0,1]):
        base=10;frame=base+(step+1 if step+1<count else 0)
        out.append([mode,hold,step,native.bits(timer),native.bits(delta),count,base,frame,flags,3,2,mutation])
        if mode==1 and hold==0:out.append([mode,hold,step,native.bits(timer),native.bits(delta),count,base,99,flags,3,2,mutation])
    rng=random.Random(0x94888)
    for _ in range(600):
        step=rng.randrange(-1,6);count=rng.randrange(-1,8);base=rng.randrange(16,128)
        mode=rng.choice([0,1,32767,-32768,-1,0x12340000,0x12340001])
        out.append([mode,rng.choice([0,0,0,-1]),step,native.bits(rng.choice([-.125,-0.,0.,.015625,.0625,.125])),
                    native.bits(rng.choice([-.03125,0.,.015625,.0625,.125])),count,base,rng.randrange(256),
                    rng.choice([0,1,0xfff,0x1000,0x2000,0x3000,0xffff]),rng.randrange(8),rng.randrange(8),rng.randrange(2)])
    # Halfword-mode upper bits and extreme counters are harmless on hold/cleanup paths.
    for step in [-32768,32767]:
        for mode,hold in [(0,0),(0x12340000,0),(1,1),(-32768,1)]:
            out.append([mode,hold,step,native.bits(.0625),native.bits(.0625),3,10,0,0,7,7,1])
    return out


def behavior(target,linked,addresses,work):
    cases=corpus();expected=[];coverage=set();branches=set()
    for case in cases:
        want,memory=native.oracle(case,addresses)
        for words in [target,linked]:
            got,actual,seen,edges=native.execute(words,addresses[NAME],case,addresses)
            assert got==want,('summary',case,got,want)
            assert all(actual[k]==v for k,v in memory.items() if not native.STACK-64<=k<native.STACK+64),('memory',case)
            coverage|=seen;branches|=edges
        expected.append(want)
    assert coverage==set(range(0,332,4)),('uncovered',set(range(0,332,4))-coverage)
    conditional={i*4 for i,w in enumerate(target) if w>>26 in [4,5,20,21] and ((w>>21)&31 or (w>>16)&31)}
    conditional|={i*4 for i,w in enumerate(target) if w>>26==17 and ((w>>21)&31)==8}
    assert branches=={(pc,outcome) for pc in conditional for outcome in [False,True]} | {(40,True),(228,True)},('branches',branches,conditional)
    text=''.join(' '.join(map(str,case))+'\n' for case in cases)
    flags=['cc','-std=c89','-O1','-g','-Wall','-Wextra','-Werror','-fsanitize=undefined','-fno-sanitize-recover=all']
    binary=work/'host';run(flags+[str(HERE/'host.c'),str(HERE/'group/setter.c'),'-o',str(binary)])
    rows=run([str(binary)],input=text,text=True).stdout.splitlines()
    assert [list(map(int,row.split())) for row in rows]==expected,'unchanged host source mismatch'
    mutations={
        'wrong_interval':('0.0625f','0.125f'),
        'advance_twice':('obj->step++;','obj->step += 2;'),
        'reverse_cleanup_priority':('if (k->flags & 0x1000)','if (k->flags & 0x2000)'),
        'wrong_mapping':('D_801427C0[f]','D_801427C0[f + 1]'),
        'omit_current_frame':('m->frame = f;','m->frame = m->frame;'),
    }
    rejected={}
    source=(HERE/'group/caller.c').read_text()
    host=(HERE/'host.c').read_text()
    for label,(old,new) in mutations.items():
        assert old in source
        mutant=work/(label+'.c');mutant.write_text(source.replace(old,new))
        wrapper=work/(label+'_host.c');wrapper.write_text(host.replace('"group/caller.c"','"'+str(mutant)+'"'))
        exe=work/label;run(flags+[str(wrapper),str(HERE/'group/setter.c'),'-o',str(exe)])
        rows=run([str(exe)],input=text,text=True).stdout.splitlines()
        actual=[list(map(int,row.split())) for row in rows]
        differing=sum(g!=e for g,e in zip(actual,expected))
        assert len(actual)==len(expected) and differing>0,label
        rejected[label]=differing
    # Malformed native controls, on a path guaranteed to exercise each changed instruction.
    control_case=[1,0,2,native.bits(0),native.bits(.0625),3,10,99,0x2000,3,2,1]
    adverse={}
    for label,index,word in [('unknown_opcode',0,0xffffffff),('wrong_stack_save',3,(target[3]&0xffff0000)|16),
                             ('wrong_callback',55,0x0c000000)]:
        bad=list(target);bad[index]=word
        try:native.execute(bad,addresses[NAME],control_case,addresses)
        except (AssertionError,OverflowError):adverse[label]=True
        else:raise AssertionError(('native adverse control survived',label))
    try:native.execute(target[:-1],addresses[NAME],control_case,addresses)
    except AssertionError:adverse['truncated_body']=True
    else:raise AssertionError('truncated body survived')
    return {'cases':len(cases),'native_executions':len(cases)*2,'complete_instruction_coverage':len(coverage),
            'conditional_branches':len(conditional),'both_outcomes':True,'host_c89_ubsan':'passed',
            'full_nonstack_memory_and_callback_snapshots':'equal','callee_saves_and_stack_canaries':'passed',
            'semantic_mutants_rejected':rejected,'malformed_native_controls':adverse}


def clock_witness(targets,addresses):
    name='func_8010E694';words=targets[name]
    upper,lower=words[16],words[17]
    assert upper>>26==15 and ((upper>>16)&31)==6
    assert lower>>26==9 and ((lower>>21)&31)==6 and ((lower>>16)&31)==6
    address=(((upper&0xffff)<<16)+native.signed(lower&0xffff,16))&0xffffffff
    assert address==addresses['D_8002EB94']==0x8002eb94
    offsets=[i*4 for i,w in enumerate(words) if w>>26==49 and ((w>>21)&31)==6 and w&0xffff==0]
    assert offsets==[76,80,100]
    between=words[offsets[0]//4:offsets[-1]//4+1]
    assert all(w>>26 in [9,17,35,49] for w in between), 'intervening side effect or branch'
    assert all(not(w>>26 in [9,35] and ((w>>16)&31)==6) for w in between)
    return {'function':name,'target_sha256':sha(struct.pack('>%dI'%len(words),*words)),
            'clock_address':address,'clock_read_offsets':offsets,'intervening_calls_or_stores':0,
            'meaning':'Independent repeated observations support a volatile view, not proof of original qualifier or asynchronous writes.'}


def verify():
    targets=score.targets();addresses=score.image_symbols()
    entries={name:addresses[name] for name in ENTRY_ADDRESSES}
    assert entries==ENTRY_ADDRESSES,'selected entry address drift'
    assert len(targets[NAME])==83 and len(targets[CONTEXT])==11
    witness=clock_witness(targets,addresses)
    locks=json.loads(base_bytes('blob_matched.lock.json'))
    assert NAME not in locks
    assert file_sha(HERE/'group/setter.c')==locks[CONTEXT]['source_sha256']
    assert (HERE/'group/setter.c').read_bytes()==base_bytes(locks[CONTEXT]['source'])
    assert (HERE/'controls/archive.c').read_bytes()==run(['git','show',BASE+':cloud/work/frontier/w7b/audio_channel_setup/best.c'],cwd=ROOT).stdout
    recipe=json.loads((HERE/'group/group.json').read_text());assert recipe['claims']==[]
    assert recipe['keep']==[NAME,CONTEXT] and recipe['files']==['caller.c','setter.c'] and recipe['flags']==FLAGS
    with tempfile.TemporaryDirectory(prefix='animation-proof-') as temp:
        work=Path(temp);obj=work/'group.o';score.compile_group(HERE/'group',obj)
        compile_report={name:comparison(obj,name) for name in [NAME,CONTEXT]}
        assert compile_report[NAME]['differing']==3 and compile_report[NAME]['elf_function_bytes']==332
        assert score.compare(obj,CONTEXT,show=0).accepted() and compile_report[CONTEXT]['elf_function_bytes']==44
        linked,proof=link_body(obj,NAME,addresses,work,'caller')
        helper,helper_proof=link_body(obj,CONTEXT,addresses,work,'setter')
        assert proof['complete_differing_offsets']==[252,280,284]
        assert helper_proof['complete_differing_offsets']==[]
        # No nonzero excess instructions may hide outside either ELF body.
        _,sections,symbols=elf(obj);text=next(s for s in sections if s['name']=='.text')
        covered=set()
        for name in [NAME,CONTEXT]:
            start,size,_=function(obj,name);covered.update(range(start,start+size))
        raw=obj.read_bytes()[text['off']:text['off']+text['size']]
        assert all(byte==0 for i,byte in enumerate(raw) if i not in covered)
        controls={}
        for label,contents in [('archive',(HERE/'controls/archive.c').read_text())]:
            source=work/(label+'.c');source.write_text(contents);out=work/(label+'.o')
            score.compile_single(source,FLAGS,out);controls[label]=comparison(out,NAME)
            _,control_link=link_body(out,NAME,addresses,work,label)
            assert control_link['linked_sha256']==proof['linked_sha256']
        ordinary=work/'ordinary';ordinary.mkdir()
        for name in recipe['files']:(ordinary/name).write_text((HERE/'group'/name).read_text().replace('volatile f32 D_8002EB94','f32 D_8002EB94'))
        (ordinary/'group.json').write_text(json.dumps(recipe));out=work/'ordinary.o';score.compile_group(ordinary,out)
        controls['ordinary_clock']=comparison(out,NAME)
        assert controls['ordinary_clock']['differing']==69
        _,ordinary_proof=link_body(out,NAME,addresses,work,'ordinary')
        assert ordinary_proof['elf_function_bytes']==328 and len(ordinary_proof['complete_differing_offsets'])==70
        # Compile the exact source with O32 layout assertions in a separate TU.
        layout=work/'layout.c';layout.write_text('#define OFF(T,F) ((unsigned long)&((T *)0)->F)\n#include "'+str(HERE/'group/caller.c')+'"\n'+
            '\n'.join('typedef char check%d[(%s)==%d?1:-1];'%(i,expr,value) for i,(expr,value) in enumerate([
                ('sizeof(Obj)',24),('OFF(Obj,step)',4),('OFF(Obj,model)',12),('OFF(Obj,timer)',16),
                ('sizeof(Model)',92),('OFF(Model,id)',14),('OFF(Model,type)',16),('OFF(Model,frame)',80),
                ('OFF(Model,base)',88),('OFF(Model,count)',90),('sizeof(Kind)',48),('OFF(Kind,flags)',18)])))
        score.compile_single(layout,FLAGS,work/'layout.o')
        layout_setter=work/'layout_setter.c'
        layout_setter.write_text('#define OFF(T,F) ((unsigned long)&((T *)0)->F)\n#include "'+str(HERE/'group/setter.c')+'"\ntypedef char a[(sizeof(IndexedRecord44)==68)?1:-1];\ntypedef char b[(OFF(IndexedRecord44,value)==20)?1:-1];\n')
        score.compile_single(layout_setter,FLAGS,work/'layout_setter.o')
        result=behavior(targets[NAME],linked,addresses,work)
    inputs={str(p.relative_to(ROOT)):file_sha(p) for p in sorted(HERE.rglob('*')) if p.is_file() and p.suffix in ['.c','.py','.json'] and p.name not in ['verification.json'] and '__pycache__' not in str(p)}
    return {'status':'NONMATCH','claims':[],'accepted_byte_gain':0,'base_commit':BASE,
            'target':NAME,'entry_addresses':entries,
            'interval':['0x%08X'%entries[NAME],'0x%08X'%(entries[NAME]+len(targets[NAME])*4)],'flags':FLAGS,
            'input_sha256':inputs,
            'compile':compile_report,'complete_gnu_proof':{NAME:proof,CONTEXT:helper_proof},
            'controls':controls,'ordinary_clock_complete_gnu':ordinary_proof,'behavior':result,
            'clock_witness':witness,'o32_layout_assertions':14,'compiler_sha256':file_sha(score.IDO/'cc'),
            'compiler_stage_sha256':{name:file_sha(score.IDO/name) for name in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as0','as1']},
            'gnu_ld_sha256':file_sha(run(['which','mips-linux-gnu-ld'],text=True).stdout.strip()),
            'limits':['External callback contract hooks only; actual callback internals are not executed.',
                      'Finite binary32 samples; no FCSR, NaN, concurrency or gameplay claim.',
                      'Per-body GNU linking does not establish original contiguous translation-unit placement.',
                      'No shadow, image, compression, ROM, production admission or hosted CI gate.']}


def portable(receipt):
    # Tool identity is retained as run provenance. Portable replay compares the
    # complete GNU-linked bytes, relocations, native bindings and behavior instead.
    receipt=json.loads(json.dumps(receipt))
    for field in ['target_manifest_provenance','gnu_ld_sha256']:receipt.pop(field,None)
    return receipt


def main():
    parser=argparse.ArgumentParser();parser.add_argument('--write',action='store_true');args=parser.parse_args()
    receipt=verify();path=HERE/'verification.json'
    if args.write:path.write_text(json.dumps(receipt,indent=2,sort_keys=True)+'\n')
    else:assert portable(receipt)==portable(json.loads(path.read_text())),'saved evidence differs'
    print('NONMATCH: 3/83 complete words; accepted setter 11/11; %d native/host cases'%receipt['behavior']['cases'])

if __name__=='__main__':main()
