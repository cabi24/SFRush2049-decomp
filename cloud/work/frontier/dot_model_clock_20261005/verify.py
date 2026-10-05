#!/usr/bin/env python3
"""Reproducible metadata-only model-clock research verification."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
from tools.conveyor.pipeline import frontier
spec = importlib.util.spec_from_file_location('clock_semantics',HERE/'semantics.py')
semantics = importlib.util.module_from_spec(spec);spec.loader.exec_module(semantics)
FN = 'func_800E762C'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
CONTEXT = ['func_800E5D64','battle_mode_setup','func_800EC914']
BASELINE = 'cloud/work/tiny_A30/func_800E762C.c'

def sha(data):return hashlib.sha256(data).hexdigest()
def packed(words):return struct.pack('>%dI'%len(words),*words)

def inspect(obj,name):
    data,secs=score._elf(obj); ti=score._text_index(secs);text=secs[ti]
    symbols=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i)]
    fn,=[s for s in symbols if s['name']==name and s['type']==2]
    start,end=fn['value'],fn['value']+fn['size']
    words=score.text_words(obj)
    got,masks,unresolved,unverified,errors=score.relocate(obj,words,start,end,score.image_symbols())
    if masks:
        addresses=score.image_symbols()
        named=lambda n: addresses[n] if n in addresses else score.address_named(n)
        own=score.owndata.verify(obj,name,score.targets()[name],address=named(name),
            image=score.own_data(),start=start,addresses=named)
        assert own.ok and set(masks)<=own.sites
        bases=own.bases()
        for section in secs:
            if section['type']!=9 or section['info']!=ti:continue
            table=score._symbol_table(data,secs,section['link']);pending={}
            for off in range(section['off'],section['off']+section['size'],8):
                site,info=struct.unpack_from('>II',data,off);index,kind=info>>8,info&255
                if site not in masks:continue
                symbol=table[index];base=bases[secs[symbol['section']]['name']]+symbol['value']
                if kind==5:pending.setdefault(index,[]).append(site)
                elif kind==6:
                    low=words[site//4]&65535;low=low-65536 if low&32768 else low
                    his=pending.pop(index,[]);assert his
                    value=base+((words[his[0]//4]&65535)<<16)+low
                    for high in his:got[high//4]=(words[high//4]&0xffff0000)|(((value+0x8000)>>16)&65535)
                    got[site//4]=(words[site//4]&0xffff0000)|(value&65535)
                else:raise AssertionError('unsupported own relocation')
            assert not pending
        masks={};unverified=[]
    assert not(masks or unresolved or unverified or errors)
    target=score.targets()[name];body=got[start//4:end//4]
    next_start=min([s['value'] for s in symbols if s['type']==2 and s['section']==ti and s['value']>=end]+[len(words)*4])
    padding=words[end//4:next_start//4]
    assert not any(padding)
    relocs=[]
    for sec in secs:
        if sec['type']!=9 or sec['info']!=ti:continue
        table=score._symbol_table(data,secs,sec['link'])
        for off in range(sec['off'],sec['off']+sec['size'],8):
            site,info=struct.unpack_from('>II',data,off)
            if start<=site<end:relocs.append({'offset':site-start,'type':info&255,'symbol':table[info>>8]['name']})
    differing=[i*4 for i in range(max(len(body),len(target))) if i>=len(body) or i>=len(target) or body[i]!=target[i]]
    return {'canonical':asdict(score.compare(obj,name,show=0)),'elf_bytes':fn['size'],
            'native_bytes':len(target)*4,'full_extent_equal':body==target,
            'full_extent_differing_offsets':differing,'zero_alignment_bytes':len(padding)*4,
            'nonzero_excess_words':sum(w!=0 for w in body[len(target):]),
            'body_sha256':sha(packed(body)),'relocations':relocs,
            'own_sections':[{ 'name':s['name'],'size':s['size']} for s in secs if s['name'] in ('.data','.rodata','.bss') and s['size']]},body

def native():
    target=score.targets()[FN]; addresses=score.image_symbols();start=addresses[FN]
    calls=[]
    for name,words in score.targets().items():
        for i,w in enumerate(words):
            if w>>26==3 and ((addresses[name]&0xf0000000)|((w&0x3ffffff)<<2))==start:
                calls.append({'caller':name,'site':hex(addresses[name]+4*i)})
    return {'start':hex(start),'end_exclusive':hex(start+4*len(target)),'bytes':4*len(target),
            'sha256':sha(packed(target)),'direct_callers':calls,
            'direct_calls':sum(w>>26==3 for w in target),
            'unsaved_callee_writes':frontier.unsaved_callee_writes(target)}

def gnu_link(obj,directory):
    symbols=score.image_symbols();data,secs=score._elf(obj)
    table=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i)]
    undefined=[s['name'] for s in table if s['section']==0 and s['name']]
    definitions=[]
    for name in undefined:
        addr=symbols.get(name,score.address_named(name));assert addr is not None,name
        definitions.append('%s = 0x%x;'%(name,addr))
    script=directory/'proof.ld';script.write_text('SECTIONS { .text 0x800E762C : SUBALIGN(4) { *(.text) } }\n'+'\n'.join(definitions))
    elf=directory/'proof.elf';binary=directory/'proof.bin'
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True)
    subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(elf),str(binary)],check=True,capture_output=True)
    data,secs=score._elf(elf);symbols=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i)]
    fn,=[s for s in symbols if s['name']==FN]
    assert fn['value']==0x800E762C
    raw=binary.read_bytes();assert not any(raw[fn['size']:])
    return list(struct.unpack('>%dI'%(fn['size']//4),raw[:fn['size']]))

def verify(directory):
    result={'status':'NONMATCH','claims':[],'accepted_byte_gain':0,'base':'cc4d5fdd',
            'flags':FLAGS,'native':native(),
            'source_sha256':{n:sha((HERE/n).read_bytes()) for n in ['candidate.c','host.c','semantics.py','verify.py']},
            'baseline_source':BASELINE,'baseline_source_sha256':sha((ROOT/BASELINE).read_bytes()),
            'target_manifest_sha256':sha((score.ASM_DIR/'SHA256SUMS').read_bytes()),
            'tool_sha256':{n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','umerge','uopt','ugen','as1']}}
    result['controls']={}
    final_obj=directory/'final.o';score.compile_single(HERE/'candidate.c',FLAGS,final_obj)
    result['candidate'],body=inspect(final_obj,FN)
    linked=gnu_link(final_obj,directory);assert linked==body
    result['gnu_link']={'agrees_with_all_project_relocations':True,'body_sha256':sha(packed(linked))}
    result['semantics']=semantics.verify(directory,linked)
    old=directory/'baseline.o';score.compile_single(ROOT/BASELINE,FLAGS,old)
    result['controls']['baseline_O3']=inspect(old,FN)[0]
    old=directory/'candidate_O2.o';score.compile_single(HERE/'candidate.c',FLAGS.replace('-O3','-O2'),old)
    result['controls']['candidate_O2']=inspect(old,FN)[0]
    source=(HERE/'candidate.c').read_text()
    controls={
        'local_clock_snapshot':source.replace('time = D_80143FF4 * step;', 'time = negative_ticks * step;'),
        'unconditional_rate_cache':source.replace('f32 ratio;', 'f32 ratio;\n    f32 old_step;').replace('if (mode != 2', 'old_step = model->step;\n        if (mode != 2').replace('model->step == 0.0f','old_step == 0.0f').replace('time / model->step','time / old_step'),
        'hypothetical_storage_owner':source.replace('extern s32 D_80143FF4;', 's32 D_80143FF4;').replace('extern ModelClockRecord D_8014A250[6];', 'ModelClockRecord D_8014A250[6];').replace('    mode = D_8014A110;\n', '').replace('if (mode != 2', 'if (D_8014A110 != 2'),
        'joined_time_store':source.replace('            model->last_time = time;\n','').replace('        }\n    }','        }\n        model->last_time = time;\n    }'),
    }
    for label,text in controls.items():
        p=directory/(label+'.c');p.write_text(text);o=directory/(label+'.o');score.compile_single(p,FLAGS,o)
        result['controls'][label]=inspect(o,FN)[0]
    group=directory/'context';group.mkdir();shutil.copyfile(HERE/'candidate.c',group/'candidate.c')
    files=['candidate.c'];result['context_source_sha256']={}
    for name in CONTEXT:
        path=ROOT/'src/blob'/ (name+'.c');result['context_source_sha256'][name]=sha(path.read_bytes())
        shutil.copyfile(path,group/path.name);files.append(path.name)
    (group/'group.json').write_text(json.dumps({'files':files,'flags':FLAGS,'members':[FN],
        'context':CONTEXT,'keep':[FN]+CONTEXT,'claims':[]}))
    obj=directory/'context.o';score.compile_group(group,obj)
    result['context']={n:inspect(obj,n)[0] for n in [FN]+CONTEXT}
    assert all(result['context'][n]['full_extent_equal'] for n in CONTEXT)
    assert not result['candidate']['full_extent_equal']
    return result

def main():
    parser=argparse.ArgumentParser();parser.add_argument('--record',action='store_true');parser.add_argument('--output',type=Path)
    args=parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='model-clock-proof-') as tmp:result=verify(Path(tmp))
    if args.record:(HERE/'verification.json').write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    else:
        saved=json.loads((HERE/'verification.json').read_text())
        # Manifest annotations can change after unrelated accepted work. All selected
        # native/source/body/relocation fields must still reproduce exactly.
        saved.pop('target_manifest_sha256');current=dict(result);current.pop('target_manifest_sha256')
        assert current==saved,'fresh verification differs from saved receipt'
    if args.output:args.output.write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    print(json.dumps({'status':result['status'],'bytes':result['candidate']['elf_bytes'],
                      'differing':result['candidate']['canonical']['differing'],
                      'behavior_cases':result['semantics']['cases'],
                      'native_coverage':result['semantics']['native_covered_words']},sort_keys=True))
if __name__=='__main__':main()
