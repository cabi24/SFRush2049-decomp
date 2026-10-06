#!/usr/bin/env python3
"""Verify complete claimed ELF, GNU link, literals, genuine context and behavior."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
import re
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]

# Context groups superseded after this packet was frozen are kept byte-identical
# under cloud/work/frontier/superseded/; receipts keep the original paths.
SUPERSEDED={'src/blob/groups/frontier_list_alloc_sound':'cloud/work/frontier/superseded/frontier_list_alloc_sound'}
def src(p):
    p=str(p)
    for old,new in SUPERSEDED.items():
        if p.startswith(old) and not (ROOT/old).exists():return ROOT/(new+p[len(old):])
    return ROOT/p
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=importlib.util.spec_from_file_location('object_semantics',HERE/'semantics.py')
semantics=importlib.util.module_from_spec(spec);spec.loader.exec_module(semantics)
FN='func_8010DBB8';SOURCE=HERE/'candidate.c';FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
CONTEXT=['src/blob/func_80090284.c','src/blob/func_80090E9C.c','src/blob/stat_lap_split.c',
         'src/blob/func_800A61B0.c','src/blob/groups/frontier_list_alloc_sound/group.c']
NAMES=['func_80090284','func_80090E9C','stat_lap_split','func_800A61B0','entity_flags_apply',
       'high_scores_display','func_8009211C','func_80091FBC','func_80092278']

def sha(data):return hashlib.sha256(data).hexdigest()
def symbols(data,sections):return [s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i)]
def inspect(obj,name):
    data,sections=score._elf(obj);ti=score._text_index(sections)
    fn,=[s for s in symbols(data,sections) if s['name']==name and s['type']==2 and s['section']==ti]
    cmp=score.compare(obj,name,show=0)
    result=dict(asdict(cmp),verdict=cmp.summary(),elf_bytes=fn['size'],native_bytes=4*len(score.targets()[name]),notes=list(cmp.notes))
    # Error text can include compared literal payloads. Keep only proof metadata.
    result['errors']=[re.sub(r'retail [0-9a-f]+, got [0-9a-f]+','retail and candidate byte values omitted',v) for v in result['errors']]
    result['verdict']=re.sub(r'retail [0-9a-f]+, got [0-9a-f]+','retail and candidate byte values omitted',result['verdict'])
    return result
def complete(result):
    return result['verdict']=='MATCH' and result['elf_bytes']==result['native_bytes'] and not any(result[k] for k in ('differing','unresolved','unverified','errors','extra_words'))

def compile_variant(work,label,text):
    folder=work/label;folder.mkdir();(folder/'candidate.c').write_text(text)
    shutil.copyfile(HERE/'group.json',folder/'group.json');obj=folder/'candidate.o'
    score.compile_group(folder,obj);return obj

def verify(work,behavior=True):
    obj=work/'candidate.o';score.compile_group(HERE,obj)
    result={'base_revision':'31b2799ebb821a7ec0983a34d2611bba2cedaab9','function':FN,
            'range':['0x8010DBB8','0x8010DCFC'],'candidate_bytes':324,'accepted_byte_gain':0,
            'flags':FLAGS,'erratum_flag':score.R4300_CC,'source_sha256':sha(SOURCE.read_bytes()),
            'recipe_sha256':sha((HERE/'group.json').read_bytes()),'object':inspect(obj,FN)}
    assert complete(result['object'])
    data,sections=score._elf(obj);ti=score._text_index(sections);syms=symbols(data,sections)
    fn,=[s for s in syms if s['name']==FN and s['type']==2]
    donor,=[s for s in syms if s['name']=='dotprod' and s['type']==2]
    assert donor['value']==0 and donor['size']==8 and fn['value']==8 and fn['size']==324
    text=sections[ti];raw=data[text['off']:text['off']+text['size']]
    assert not any(raw[332:])
    assert len(raw)==336
    result['unclaimed_object_text']={'deleted_donor_helper_bytes_before_candidate':8,'zero_alignment_bytes_after_candidate':4,
        'donor_stub_is_not_a_native_name_or_coverage_claim':True}
    ro,=[s for s in sections if s['name']=='.rodata']
    own=data[ro['off']:ro['off']+ro['size']]
    literal=struct.pack('>ff',.0333333,3.1415927)
    assert own[:8]==literal==score.own_data().read(0x801249c4,8) and not any(own[8:])
    assert not any(s['size'] for s in sections if s['name'] in ('.data','.bss'))
    result['owned_data']={'address':'0x801249C4','verified_bytes':8,'values':list(struct.unpack('>ff',literal)),
                          'zero_alignment_bytes':len(own)-8}
    relocs=[]
    for sec in sections:
        if sec['type']==9 and sec['info']==ti:
            table=score._symbol_table(data,sections,sec['link'])
            for off in range(sec['off'],sec['off']+sec['size'],8):
                at,info=struct.unpack_from('>II',data,off)
                if fn['value']<=at<fn['value']+fn['size']:
                    relocs.append({'offset':at-fn['value'],'type':info&255,'symbol':table[info>>8]['name']})
    assert len(relocs)==15 and sum(r['type']==4 for r in relocs)==3
    result['relocations']=relocs
    addresses=score.image_symbols();undefined=[s['name'] for s in syms if s['name'] and s['section']==0]
    script=work/'link.ld';script.write_text('SECTIONS { .text 0x8010DBB0 : SUBALIGN(4) { *(.text) }\n'
                '.rodata 0x801249C4 : SUBALIGN(4) { *(.rodata) } }\n'+''.join('%s = 0x%08X;\n'%(n,addresses[n]) for n in sorted(set(undefined))))
    elf=work/'candidate.elf';subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True)
    linked,lsecs=score._elf(elf);lfn,=[s for s in symbols(linked,lsecs) if s['name']==FN and s['type']==2]
    lt=lsecs[score._text_index(lsecs)];lr,=[s for s in lsecs if s['name']=='.rodata']
    assert lfn['value']==0x8010dbb8 and lfn['size']==324
    words=list(struct.unpack('>81I',linked[lt['off']+8:lt['off']+332]));assert words==score.targets()[FN]
    shoff=struct.unpack_from('>I',linked,0x20)[0];shentsize=struct.unpack_from('>H',linked,0x2e)[0]
    assert struct.unpack_from('>I',linked,shoff+lsecs.index(lr)*shentsize+12)[0]==0x801249c4
    assert linked[lr['off']:lr['off']+8]==literal
    result['gnu_link']={'complete_81_words_equal':True,'elf_symbol_bytes':324,'owned_literal_bytes_equal':True,
                        'claimed_body_sha256':sha(struct.pack('>81I',*words))}
    body=SOURCE.read_text();controls={}
    collapsed=body.replace('    Node *allocated;\n','').replace('    allocated=func_80090284();\n    node=allocated;\n    if (allocated!=0) {','    node=func_80090284();\n    if (node!=0) {')
    direct=body.replace('dotprod(car->velocity,actor->matrix[2])','(car->velocity[0]*actor->matrix[2][0] + car->velocity[1]*actor->matrix[2][1] + car->velocity[2]*actor->matrix[2][2])')
    for name,source in [('collapsed_node_carriers',collapsed),('expanded_dot_expression',direct),('wrong_literal',body.replace('0.0333333f','0.0333334f'))]:
        assert source!=body
        out=compile_variant(work,name,source);controls[name]=inspect(out,FN);assert not complete(controls[name])
    assert controls['collapsed_node_carriers']['differing']==10 and controls['collapsed_node_carriers']['elf_bytes']==324
    assert controls['wrong_literal']['differing']==0 and not controls['wrong_literal']['verdict']=='MATCH'
    archived=ROOT/'cloud/work/heads_B10/func_8010DBB8_best.c';out=work/'archived.o'
    score.compile_single(archived,FLAGS,out);controls['archived_b10']=inspect(out,FN)
    assert controls['archived_b10']['differing']==14
    result['source_controls']=controls
    group=work/'context';group.mkdir();files=CONTEXT+['cloud/work/ipa-groups/dot_object_sound_20261005/candidate.c']
    for i,p in enumerate(files):shutil.copyfile(src(p),group/('c%d.c'%i))
    old=json.loads(src('src/blob/groups/frontier_list_alloc_sound/group.json').read_text())
    keep=[FN,'func_80090284','func_80090E9C','stat_lap_split','func_800A61B0']+old['keep']
    (group/'group.json').write_text(json.dumps({'files':['c%d.c'%i for i in range(len(files))],
        'members':[FN],'context':NAMES,'keep':keep,'flags':FLAGS}))
    out=group/'group.o';score.compile_group(group,out)
    result['genuine_context']={n:inspect(out,n) for n in [FN]+NAMES}
    assert all(complete(r) for r in result['genuine_context'].values())
    result['context_sources']={p:sha(src(p).read_bytes()) for p in CONTEXT}
    abi=work/'abi.c';abi.write_text('#include "'+str(SOURCE)+'"\n#define OFF(T,M) ((unsigned int)&((T*)0)->M)\n'+
        '\n'.join('typedef char check%d[(%s)?1:-1];'%(i,c) for i,c in enumerate([
        'sizeof(void*)==4','sizeof(Node)==24','OFF(Node,fieldC)==12','OFF(Node,field10)==16','OFF(Node,field14)==20',
        'sizeof(Actor)==96','OFF(Actor,index)==16','OFF(Actor,matrix)==20','OFF(Actor,position)==56','OFF(Actor,state)==90','OFF(Actor,player)==92',
        'sizeof(Player)==952','OFF(Player,velocity)==20','sizeof(Definition)==48','OFF(Definition,stat)==12','OFF(Definition,sound)==28'])))
    score.compile_single(abi,FLAGS,work/'abi.o');result['o32_layout_assertions']=16
    if behavior:result['behavior']=semantics.verify(work,words,SOURCE)
    result['target_manifest_sha256']=sha((score.ASM_DIR/'SHA256SUMS').read_bytes())
    result['compiler_sha256']={n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']}
    result['packet_sha256']={n:sha((HERE/n).read_bytes()) for n in ['candidate.c','group.json','verify.py','semantics.py','native.py','host.c']}
    result['limitations']=['No complete shadow unit, splice, image, compression or ROM gate.',
       'Dot helper ancestry is authentic; a whole-function arcade donor and original N64 helper name are not established.',
       'Allocator-result and working-node carriers preserve archived source roles; native code cannot prove original local names.',
       'This context regression does not establish a unified shared-type model or complete game caller closure.']
    return result

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path);parser.add_argument('--no-behavior',action='store_true')
    args=parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='object-sound-') as folder:result=verify(Path(folder),not args.no_behavior)
    rendered=json.dumps(result,indent=2,sort_keys=True)+'\n'
    if args.output:args.output.write_text(rendered)
    print(json.dumps({'function':FN,'strict':result['object']['verdict'],'bytes':324,'context_bodies':len(result['genuine_context']),
                      'behavior_cases':result.get('behavior',{}).get('cases',0)}))
if __name__=='__main__':main()
