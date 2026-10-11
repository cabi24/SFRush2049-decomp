#!/usr/bin/env python3
"""Verify complete nonmatching cleanup evidence without changing accepted inputs."""
import dataclasses
import hashlib
import importlib.util
import json
from pathlib import Path
import sys
import tempfile
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
SOURCE=ROOT/'cloud/work/frontier/dot_runtime_a_record_context_20261011'

def module(name,path):
    spec=importlib.util.spec_from_file_location(name,path);m=importlib.util.module_from_spec(spec);sys.modules[name]=m;spec.loader.exec_module(m);return m
base=module('cleanup_record_verify',SOURCE/'verify.py')
semantics=module('cleanup_lifetime_semantics',HERE/'semantics.py')
require=base.require
NAME='func_8039D05C'

def audit_callers(obj):
    targets=base.score.targets();sections,symbols,relocs=base.elf(obj);allwords=base.score.text_words(obj)
    index=sections['.text'][0];callee=symbols[NAME][0];generated=[];native=[]
    def saves(body):
        for reg in (*range(16,24),30):
            stores=[w for w in body[:64] if w>>26==43 and (w>>21&31)==29 and (w>>16&31)==reg]
            require(len(stores)==1,('caller prologue save',reg))
            require(((stores[0]&0x03ffffff)|(35<<26)) in body[-64:],('caller epilogue restore',reg))
    for name,body in targets.items():
        sites=[i*4 for i,w in enumerate(body) if w>>26==3 and (w&0x3ffffff)==((semantics.ENTRY>>2)&0x3ffffff)]
        if sites:
            require(name=='func_803A1EAC','unexpected native caller');saves(body);native.extend(sites)
    for offset,kind,symbol,section in relocs:
        if section!=index or kind!=4 or symbol!='.text' or (allwords[offset//4]&0x3ffffff)*4!=callee:continue
        owners=[(name,v) for name,v in symbols.items() if v[2]==2 and v[3]==index and v[0]<=offset<v[0]+v[1]]
        require(len(owners)==1 and owners[0][0]=='func_803A1EAC','unexpected generated caller')
        name,v=owners[0];saves(allwords[v[0]//4:(v[0]+v[1])//4]);generated.append(offset-v[0])
    require(len(native)==len(generated)==4,'cleanup caller population changed')
    return {'caller':'func_803A1EAC','native_offsets':list(map(hex,native)),'compiled_offsets':list(map(hex,generated)),
            'caller_prologue_epilogue_saves':'s0 through s8','no_arguments':True,
            'limit':'caller remains nonmatching and is not executed end-to-end'}

def verify():
    require(__debug__,'Run without Python -O: inherited ELF checks use assertions')
    source=(SOURCE/'group.c').read_bytes();base.score.ASM_DIR=ROOT/'asm/us/ovl_a';native=base.score.targets()[NAME]
    require(len(native)==169,'native full extent changed')
    with tempfile.TemporaryDirectory(prefix='cleanup-lifetime-') as td:
        temp=Path(td);obj=temp/'candidate.o';base.score.compile_group(SOURCE,obj)
        result=base.score.compare(obj,NAME,show=0)
        require(result.differing==18 and result.total==169 and not(result.unresolved or result.unverified or result.errors or result.extra_words),'expected stock nonmatch changed')
        base.NAMES={NAME:676};linked,relocations=base.gnu_resolve(obj,temp)
        words=base.score.text_words(obj);start=base.score.symbols(obj)[NAME]
        resolved,masks,unresolved,unverified,errors=base.score.relocate(obj,words,start,start+676,base.score.image_symbols())
        require(not(masks or unresolved or unverified or errors),'strict source relocation failure')
        candidate=resolved[start//4:start//4+169]
        require(candidate==linked[NAME],'independent GNU linkage mismatch')
        behavior=semantics.verify(native,candidate)
        callers=audit_callers(obj)
        owned=base.owndata.verify(obj,NAME,native,address=semantics.ENTRY,image=base.score.own_data(),start=start,addresses=lambda n:base.score.image_symbols().get(n,base.score.address_named(n)))
        require(owned.ok and not owned.unverified and not owned.failures,'owned data failure')
    return {'status':'VERIFIED NONMATCH','source_packet':str(SOURCE.relative_to(ROOT)),'source_sha256':hashlib.sha256(source).hexdigest(),
            'new_matching_bytes':0,'accepted_image_bytes':0,'cartridge_bytes':0,'score':dataclasses.asdict(result),
            'compiler_sha256':{n:base.sha((base.score.IDO/n).read_bytes()) for n in ('cc','uld','usplit','umerge','uopt','ugen','as1')},
            'verification_code_sha256':{n:base.sha((HERE/n).read_bytes()) for n in ('verify.py','semantics.py')},
            'extent':{'start':hex(semantics.ENTRY),'end':hex(semantics.ENTRY+676),'native_bytes':676,'ELF_STT_FUNC_bytes':676,'next_boundary_checked':True},
            'GNU_relocations':relocations[NAME],'native_sha256':base.sha(base.packed(native)),'linked_sha256':base.sha(base.packed(candidate)),
            'semantics':behavior,'caller_ABI':callers,'owned_data':'no owned literal or storage relocations',
            'private_ABI':'no parameters; intentionally clobbers s0 through s8; not a standalone O32 replacement'}

if __name__=='__main__':
    import argparse
    p=argparse.ArgumentParser();p.add_argument('--output',type=Path);a=p.parse_args();r=json.dumps(verify(),indent=2)+'\n'
    if a.output:a.output.write_text(r)
    print(r)
