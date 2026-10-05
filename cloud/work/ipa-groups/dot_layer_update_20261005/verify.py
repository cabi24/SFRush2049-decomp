#!/usr/bin/env python3
"""Rebuild and verify the complete layer-update matching submission."""
import argparse,hashlib,importlib.util,json,struct,subprocess,sys,tempfile
from dataclasses import asdict
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
def load_module(label,filename):
    spec=importlib.util.spec_from_file_location(label,HERE/filename)
    module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module);return module
proof=load_module("layer_update_proof","proof.py")
semantics=load_module("layer_update_semantics","semantics.py")
FN='mode_select_input'
CONTEXT=['entity_transform_calc','client_sync','entity_hierarchy_update','func_80091BA8','func_80091B00','scheduler_recv']
OLD='cloud/work/ipa-groups/codex_mode_outer_a169'
ALLOC='src/blob/groups/frontier_slot18_alloc/func_80091B00.c'
BASE='e24b47d89a0c8ffade1e4c75ad76b9d390a1c232'

def inspect(obj,name):
    f=proof.Elf(obj).function(name);result=asdict(score.compare(obj,name,show=0))
    return {'elf_bytes':f['size'],'comparison':result}
def strict(obj,name):
    result=inspect(obj,name);c=result['comparison']
    assert result['elf_bytes']==len(score.targets()[name])*4 and c['differing']==0
    assert not any(c[k] for k in ('unresolved','unverified','errors','extra_words')),(name,result)
    return result

def compile_source(work,source,recipe=None):
    work.mkdir();(work/'candidate.c').write_text(source);(work/'allocator.c').write_bytes((HERE/'allocator.c').read_bytes())
    (work/'group.json').write_text(json.dumps(recipe or json.loads((HERE/'group.json').read_text())))
    obj=work/'candidate.o';score.compile_group(work,obj);return obj

def bindings():
    paths=[HERE/n for n in ('candidate.c','allocator.c','group.json','proof.py','native.py','semantics.py','host.c','verify.py')]
    paths += [ROOT/'tests/cloud/test_layer_update_packet.py',ROOT/OLD/'group.c',ROOT/OLD/'group.json',ROOT/ALLOC,ROOT/'src/blob/groups/codex_transform_b109/group.c',ROOT/'src/blob/groups/codex_transform_b109/group.json']
    return {str(p.relative_to(ROOT)):proof.sha(p.read_bytes()) for p in paths}

def verify(work):
    bodies,addresses=proof.native(ROOT)
    source=(HERE/'candidate.c').read_text();archived=(ROOT/OLD/'group.c').read_text()
    begin=archived.index('Msg *func_80091B00(void) {');end=archived.index('void entity_transform_calc',begin)
    assert source==archived[:begin]+'Msg *func_80091B00(void);\n'+archived[end:]
    assert (HERE/'allocator.c').read_bytes()==(ROOT/ALLOC).read_bytes()
    spec=json.loads((HERE/'group.json').read_text());old_spec=json.loads((ROOT/OLD/'group.json').read_text())
    assert spec['keep']==old_spec['keep'] and spec['flags']==old_spec['flags']
    assert spec['members']==spec['claims']==[FN]
    assert spec['files']==['candidate.c','allocator.c']
    obj=compile_source(work/'main',source);elf=proof.Elf(obj)
    # Whole object owns only executable text, ELF metadata and debug records.
    assert not [s for s in elf.sections if s['name'] in ('.data','.bss','.rodata','.rdata','.sdata','.sbss') and s['size']]
    matching={n:strict(obj,n) for n in [FN]+CONTEXT}
    linked,gnu={},{}
    for n in [FN]+CONTEXT:linked[n],gnu[n]=proof.link_function(obj,n,addresses,bodies[n],work/('gnu_'+n))
    assert gnu[FN]['bytes']==152 and len(gnu[FN]['relocations'])==6
    assert sum(r['type']==4 for r in gnu[FN]['relocations'])==4
    fn=elf.function(FN);assert fn['value']+fn['size']==elf.function('func_800DFBA0')['value']
    callers={n:inspect(obj,n) for n in ('func_800DFBA0','func_800E05F0')}
    assert callers['func_800DFBA0']['comparison']['differing']==183
    assert callers['func_800E05F0']['comparison']['differing']==315
    calls=lambda n,target:[4*i for i,w in enumerate(bodies[n]) if w>>26==3 and (0x80000000|((w&0x3ffffff)<<2))==addresses[target]]
    witnesses={'DFBA0_calls_helper':calls('func_800DFBA0',FN),'E05F0_calls_DFBA0':calls('func_800E05F0','func_800DFBA0'),
      'helper_calls_client':calls(FN,'client_sync'),'helper_calls_transform':calls(FN,'entity_transform_calc'),
      'helper_has_no_hierarchy_call':not calls(FN,'entity_hierarchy_update')}
    assert len(witnesses['DFBA0_calls_helper'])==2 and len(witnesses['E05F0_calls_DFBA0'])==1
    assert len(witnesses['helper_calls_client'])==len(witnesses['helper_calls_transform'])==1 and witnesses['helper_has_no_hierarchy_call']
    behavior=semantics.verify(work,source,HERE,bodies,linked,addresses)
    controls={}
    old_obj=work/'archived.o';score.compile_group(ROOT/OLD,old_obj)
    controls['archived_A169']={'target':strict(old_obj,FN),'allocator':inspect(old_obj,'func_80091B00')}
    assert controls['archived_A169']['allocator']['comparison']['differing']==11
    control=source.replace('__inline void entity_hierarchy_update','void entity_hierarchy_update')
    assert control!=source
    out=compile_source(work/'not_inline',control);controls['ordinary_wrapper']=inspect(out,FN)
    assert controls['ordinary_wrapper']['comparison']['differing']>0
    # These are deliberately wrong contracts, not matching attempts.
    edits={
      'missing_level_store':('        state->level=level;','        state->level=state->level;'),
      'wrong_style_predicate':('    if (style!=state->style) {','    if (style==state->style) {'),
      'style_sent_to_wrong_channel':('        entity_hierarchy_update(state->handle,style);','        client_sync(state->handle,style);'),
      'handle_read_after_lock':('        entity_hierarchy_update(state->handle,style);','        osRecvMesg(&D_80142728,0,1);\n        entity_transform_calc(state->handle,-2.0f,-2.0f,-2.0f,style);\n        osJamMesg(&D_80142728,0,0);')}
    negatives={};cases=list(semantics.corpus())
    for label,(before,after) in edits.items():
        assert source.count(before)==1,(label,source.count(before));mutant=source.replace(before,after)
        out=compile_source(work/label,mutant);comparison=inspect(out,FN)
        assert comparison['comparison']['differing']>0 or comparison['elf_bytes']!=152
        call=semantics.host(work/(label+'_host'),mutant,HERE)
        rejected=[i for i,c in enumerate(cases) if call(c)!=semantics.oracle(c)]
        assert rejected,(label,'not detected')
        negatives[label]={'rejected_cases':len(rejected),'first_counterexample':rejected[0],'compiler':comparison}
    # Genuine layout fields, not arbitrary stack reservations.
    abi=work/'layout.c';abi.write_text(source+'\n#define OFF(T,F) ((unsigned int)&((T*)0)->F)\n'+
      '\n'.join('typedef char check%d[(%s)?1:-1];'%(i,v) for i,v in enumerate([
      'sizeof(Entity)==68','OFF(Entity,id)==12','OFF(Entity,msgCount)==26','OFF(Entity,x)==36','OFF(Entity,w)==48',
      'sizeof(Msg)==24','OFF(Msg,used)==3','OFF(Msg,payload)==4','OFF(Msg,ent)==20',
      'sizeof(LayerState)==20','OFF(LayerState,level)==4','OFF(LayerState,style)==8',
      'OFF(ModelView,contact)==1564','OFF(ModelView,steering)==1880','OFF(ModelView,index)==1990','OFF(ModelView,power)==2040'])))
    score.compile_single(abi,spec['flags'],work/'layout.o')
    return {'base_revision':BASE,'status':'MATCHING_CANDIDATE','function':FN,'range':['0x800DFB08','0x800DFBA0'],
      'bytes':152,'accepted_byte_gain':0,'flags':spec['flags'],'erratum_flag':score.R4300_AS1,
      'source_bindings':bindings(),'strict_bodies':matching,'gnu_bodies':gnu,'unclaimed_callers':callers,
      'source_origin':'Exact archived A169 helper and genuine callers; old allocator replaced with unchanged accepted source.',
      'inherited_context_shaping':'Accepted allocator uses its existing volatile table declaration; no new volatile introduced.',
      'data_sections_owned':0,'target_following_alignment_bytes':0,'o32_layout_assertions':16,'native_call_witnesses':witnesses,
      'controls':controls,'semantic_mutants':negatives,'behavior':behavior,
      'compiler_sha256':{n:proof.sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']},
      'target_manifest_sha256':proof.sha((ROOT/'asm/us/blob/SHA256SUMS').read_bytes()),
      'limitations':['No accepted-byte, full shadow-unit, splice, source-image, compression, ROM or gameplay claim.',
        'The helper and six runtime context bodies match; the two complete actual caller reconstructions remain nonmatching and unclaimed.',
        'Inline syntax is a supported expansion hypothesis, not recovery of original source spelling; no whole-function arcade donor is established.',
        'Only OS queue boundaries are modeled in native replay. Host allocator uses the same scan contract with native pointer layout.',
        'Fixtures have valid accessible records, at least two free messages, single-threaded callbacks and binary32 comparison semantics. FCSR flags, signaling NaNs, exhausted allocation, invalid pointers and arbitrary concurrent writes are excluded.']}

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path);args=p.parse_args()
    with tempfile.TemporaryDirectory(prefix='layer-update-proof-') as directory:r=verify(Path(directory))
    if args.output:args.output.write_text(json.dumps(r,indent=2,sort_keys=True)+'\n')
    print(json.dumps({'function':FN,'matching_bytes':152,'context_matches':len(CONTEXT),'cases':r['behavior']['cases'],'native_executions':r['behavior']['native_executions']}))
if __name__=='__main__':main()
