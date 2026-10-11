#!/usr/bin/env python3
"""Reproduce the nonmatching root checkpoint without weakening acceptance."""
import argparse
import dataclasses
import hashlib
import importlib.util
import json
from pathlib import Path
import random
import struct
import subprocess
import sys
import tempfile
import time
import zlib

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
BASE=ROOT/'cloud/work/frontier/dot_runtime_a_callers_deep_20261011'
REVIEW=ROOT/'cloud/work/frontier/dot_runtime_a_leaf_review_20261011'
SIZES={'func_80392F28':220,'func_80393004':180,'func_803930B8':264,
       'func_803931C0':276,'func_803932E4':168,'func_80394058':1292}


def require(ok,detail):
    if not ok:raise ValueError(detail)


def sha(data):return hashlib.sha256(data).hexdigest()

def safe_score(result):
    value=dataclasses.asdict(result)
    value['errors']=['unpaired relocation' if 'unpaired' in x else 'owned-data mismatch' if 'own ' in x else 'relocation validation failure' for x in value['errors']]
    return value


def load(name,path):
    spec=importlib.util.spec_from_file_location(name,path)
    module=importlib.util.module_from_spec(spec);sys.modules[name]=module;spec.loader.exec_module(module)
    return module


def source_contract(source,config):
    require(config['members']==list(SIZES)[:5]+['func_8039338C','func_80394058','func_80393B3C','func_803923B0'],'member drift')
    require(config['context']==list(SIZES)[:5],'context drift')
    require(config['claims']==[],'research has no claims')
    require(config['keep']==['func_80394058'],'only genuine root retained')
    require(config['flags']=='-g0 -O3 -mips2 -G 0 -non_shared','canonical flags')
    require(config['files']==['group.c'],'source set')
    require(not any(x in source for x in ('volatile','__asm','asm(')),'unsupported source construct')
    old=load('roots_old_verify',BASE/'verify.py')
    prior=(BASE/'group.c').read_text()
    for name in (*list(SIZES)[:5],'func_8039338C'):
        require(old.function(source,name)==old.function(prior,name),'archived body drift '+name)
    root=old.function(source,'func_80394058')
    expected=old.function(prior,'func_80394058').replace('s8 item_step;','s8 item_step;\n    s32 mode;').replace('switch (D_803BA804)', 'mode = D_803BA804;\n        switch (mode)')
    require(root==expected,'root change is only witnessed mode capture')
    return old


def cases(native):
    rng=random.Random(0x94058)
    for i in range(1024):
        memory={a:0 for a in range(0x803B6500,0x803B6600)}
        memory.update({a:0 for a in range(0x803BA700,0x803BA900)})
        memory.update({a:0 for a in range(native.SP-256,native.SP+32)})
        memory.update({a:0 for a in range(0x80700000,0x80700200)})
        for a,n in ((0x80157244,1),(0x80156944,4),(0x8015694C,4),(0x80149784,4)):
            memory.update({a+j:0 for j in range(n)})
        native.write(memory,0x803B6528,1,0 if i%11==0 else 1)
        native.write(memory,0x80157244,1,1 if i%7==0 else 0)
        native.write(memory,0x803BA804,4,(0,1,1,1,2,-1)[i%6])
        native.write(memory,0x803B65A4,1,i%4)
        for j in range(4):native.write(memory,0x803BA830+j*4,4,(-4,-1,0,1,8)[(i+j)%5])
        native.write(memory,0x803BA800,4,0x80700000 if i%2 else 0)
        native.write(memory,0x80700000,4,0x80700040)
        native.write(memory,0x80700048,4,0x80700080)
        native.write(memory,0x80700100,4,0x80700140)
        native.write(memory,0x80700148,4,0x80700180)
        native.write(memory,0x803BA858,1,(0,11,12,19,20,23,24,25,28,29)[i%10])
        native.write(memory,0x803BA880,4,i%6)
        native.write(memory,0x803BA810,4,i%2)
        for j in range(6):
            native.write(memory,0x803BA818+j*4,4,(i+j)%13)
            available={(i+3*j)%13,(i+j+5)%13}
            for k in range(13):native.write(memory,0x803B6554+j*13+k,1,int(k in available))
        for k in range(30):native.write(memory,0x803B6534+k,1,int(k%5==i%5 or k in (0,28,29)))
        for j in range(2):native.write(memory,0x803BA7FC+j,1,int(j==i%2 or i%3==0))
        inputs=(0,0x400,0x800,0x1000,0x2000,0x3000,0x1400,0x2800)
        native.write(memory,0x80149784,4,inputs[i%len(inputs)])
        native.write(memory,0x80156944,4,inputs[(i//8)%len(inputs)])
        native.write(memory,0x8015694C,4,(0,1,2,4,7)[i%5])
        yield memory,rng.getrandbits(32)


def verify():
    incoming_path=list(sys.path)
    try:
        return _verify()
    finally:
        sys.path[:]=incoming_path


def _verify():
    started=time.monotonic()
    source=(HERE/'group.c').read_text();config=json.loads((HERE/'group.json').read_text())
    old=source_contract(source,config)
    score=old.load_score(ROOT)
    manifest=score.target_manifest();meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))
    require((meta['image'],meta['base'],meta['rom_offset'])==('A','0x8038A400','0xB5C534'),'image extent identity')
    asset=(ROOT/'assets/us/data.bin').read_bytes()
    require(sha(asset)=='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8','asset identity')
    require(int.from_bytes(asset[0x3850:0x3854],'big')==0xB5C534,'loader identity')
    image=zlib.decompress(asset[0xB5C534-0x283D0:],-15)
    require(len(image)==194128 and sha(image)==old.IMAGE_HASH,'image content identity')
    targets=score.targets()
    # Reuse the independently authored complete-ELF/GNU linker; isolate its
    # configurable selected-size map to this freshly loaded module instance.
    saved_path=list(sys.path)
    saved_modules={name:sys.modules.get(name) for name in ('native','elf_reader')}
    try:
        sys.path.insert(0,str(REVIEW))
        for name in saved_modules:load(name,REVIEW/(name+'.py'))
        independent=load('root_independent_link',REVIEW/'review.py')
    finally:
        sys.path[:]=saved_path
        for name,module in saved_modules.items():
            if module is None:sys.modules.pop(name,None)
            else:sys.modules[name]=module
    independent.SIZES=dict(SIZES)
    native=load('root_boundary_native',HERE/'native.py')
    with tempfile.TemporaryDirectory(prefix='runtime-a-roots-') as td:
        temp=Path(td);obj=temp/'group.o';score.compile_group(HERE,obj)
        data,sections=score._elf(obj);ti=score._text_index(sections)
        funcs={s['name']:s for i,x in enumerate(sections) if x['type']==2 for s in score._symbol_table(data,sections,i) if s['type']==2 and s['section']==ti}
        words=score.text_words(obj)
        linked,counts=independent.gnu_link(score,obj,temp)
        bindings={};scores={};calls={}
        for name in config['members']:
            symbol=funcs[name];start=symbol['value'];end=start+symbol['size']
            result=score.compare(obj,name,show=0);scores[name]=safe_score(result)
            resolved,masks,unresolved,unverified,errors=score.relocate(obj,words,start,end,score.image_symbols())
            body=resolved[start//4:end//4]
            sequence=lambda ww:[0x80000000|((w&0x3FFFFFF)<<2) for w in ww if w>>26==3]
            calls[name]={'native_count':len(sequence(targets[name])),'compiled_count':len(sequence(body)),
                         'same_static_sequence':sequence(targets[name])==sequence(body)}
            require(calls[name]['same_static_sequence'],'authentic call sequence '+name)
            bindings[name]={'native_bytes':len(targets[name])*4,'compiled_bytes':symbol['size'],
                            'native_sha256':sha(struct.pack('>%dI'%len(targets[name]),*targets[name]))}
            if name in SIZES:
                require(symbol['size']==SIZES[name],'complete extent '+name)
                require(not any((masks,unresolved,unverified,errors)),'uncertain full body '+name)
                require(linked[name]==body,'independent GNU relocation '+name)
                bindings[name]['compiled_sha256']=sha(struct.pack('>%dI'%len(body),*body))
                if name!='func_80394058':require(result.accepted() and body==targets[name],'inherited exact body '+name)
                else:
                    residual=[4*i for i,(a,b) in enumerate(zip(body,targets[name])) if a!=b]
                    require(residual==[0,0x508] and result.differing==2 and result.extra_words==0,'root residual changed')
                    require((native.signed(body[0],16),native.signed(body[0x508//4],16))==(-120,120),'candidate frame')
                    require((native.signed(targets[name][0],16),native.signed(targets[name][0x508//4],16))==(-128,128),'native frame')
        rodata=next(x for x in sections if x['name']=='.rodata')
        literal=data[rodata['off']:rodata['off']+rodata['size']]
        require(len(literal)==48 and literal==image[0x2F040:0x2F070],'complete literal ownership')
        # Uniqueness and non-code ownership are checked for the entire section.
        require(image.count(literal)==1,'literal section is not uniquely located')
        for f in meta['functions']:
            address=int(f['address'],16);size=f['size']
            require(not(address<0x803B9470 and address+size>0x803B9440),'literal overlaps code')
        coverage=set();branches=set();observed_calls=set();case_count=0
        for initial,seed in cases(native):
            a=native.run(targets['func_80394058'],initial,seed)
            b=native.run(linked['func_80394058'],initial,seed)
            visible=lambda m:{k:v for k,v in m.items() if not native.SP-256<=k<native.SP+32}
            require(visible(a['memory'])==visible(b['memory']) and a['calls']==b['calls'],'root boundary behavior')
            require(a['callee_saved']==b['callee_saved'] and a['fp_saved']==b['fp_saved'],'root preserved registers')
            coverage|=a['coverage'];branches|=a['branches'];observed_calls|={x[0] for x in a['calls']};case_count+=1
        controls={}
        variants={
            'missing_renderer':source.replace(old.function(source,'func_803923B0'),''),
            'missing_initializer':source.replace(old.function(source,'func_80393B3C'),''),
            'uncaptured_mode':source.replace(old.function(source,'func_80394058'),old.function((BASE/'group.c').read_text(),'func_80394058')),
        }
        for name,text in variants.items():
            directory=temp/name;directory.mkdir();(directory/'group.c').write_text(text);(directory/'group.json').write_text(json.dumps(config))
            obj2=directory/'out.o';score.compile_group(directory,obj2);result=score.compare(obj2,'func_80394058',show=0)
            require(result.differing==2 if name=='missing_initializer' else result.differing>2, 'context ablation '+name)
            controls[name]=safe_score(result)
        subprocess.run(['gcc','-std=c89','-fsyntax-only','-fno-builtin',str(HERE/'group.c')],check=True,capture_output=True)
    return {'status':'PASS_RESEARCH_NONMATCH','new_candidate_bytes':0,'accepted_bytes':0,
            'source_sha256':sha(source.encode()),'group_sha256':sha((HERE/'group.json').read_bytes()),
            'scorer_sha256':sha((ROOT/'tools/cloud/score.py').read_bytes()),
            'compiler_sha256':{name:sha(Path(score.ido(name)).read_bytes()) for name in ('cc','cfe','uld','usplit','umerge','uopt','ugen','as1')},
            'baseline_commit':'84bdadc830834bde962aa532874a114b48353a06',
            'master_preflight':'0119270d89538f38195e8a36ad66269c04ebb4de',
            'open_prs':[163,305,306,307,308,309,310],
            'scores':scores,'bindings':bindings,'call_audit':calls,'gnu_relocations':counts,
            'literal_section':{'address':'0x803B9440','bytes':48,'sha256':sha(literal),'unique_noncode_full_section':True},
            'root_boundary_cases':case_count,'root_instructions_covered':len(coverage),
            'root_branch_outcomes':len(branches),'root_called_targets':len(observed_calls),
            'negative_controls':controls,'seconds':round(time.monotonic()-started,4),
            'limits':['No new match or coverage claim; root frame remains 120 rather than 128 bytes.',
                      'Boundary fixtures are not complete child execution or whole-program semantic proof.',
                      'The two reconstructed FP dependencies remain nonmatching; canonical positional literal checks still fail.',
                      'The complete compiler literal section is verified independently without overriding scorer acceptance.',
                      'No original-TU layout, image splice, ROM hash, hardware or gameplay gate.']}


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path);args=parser.parse_args()
    result=verify();text=json.dumps(result,indent=2)+'\n'
    if args.output:args.output.write_text(text)
    print(text)
