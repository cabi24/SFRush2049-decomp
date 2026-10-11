#!/usr/bin/env python3
"""Bounded research: authenticate and compare complete image-A bodies twice."""
import argparse
import dataclasses
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import time
import zlib

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
IMAGE_HASH='0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
NAMES={'func_8038F648':236,'func_803A6F2C':596}

def require(condition,message):
    if not condition:
        raise ValueError(message)

def sha(raw):
    return hashlib.sha256(raw).hexdigest()

def module(name,path):
    spec=importlib.util.spec_from_file_location(name,path)
    result=importlib.util.module_from_spec(spec)
    sys.modules[name]=result
    spec.loader.exec_module(result)
    return result

def _verify():
    started=time.monotonic()
    peer=ROOT/'cloud/work/frontier/dot_runtime_a_leaf_review_20261011'
    sys.path[:0]=[str(ROOT/'tools/cloud'),str(peer)]
    score=module('menu_renderer_score',ROOT/'tools/cloud/score.py')
    review=module('menu_renderer_gnu',peer/'review.py')
    import owndata
    score.ASM_DIR=ROOT/'asm/us/ovl_a'
    manifest=score.target_manifest()
    targets=score.targets()
    meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))
    require(meta['image']=='A' and meta['image_sha256']==IMAGE_HASH,'image manifest')
    asset=(ROOT/'assets/us/data.bin').read_bytes()
    require(sha(asset)=='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8','asset drift')
    require(int.from_bytes(asset[0x3850:0x3854],'big')==0xB5C534,'loader pointer')
    image=zlib.decompress(asset[0xB5C534-0x283D0:],-15)
    require(len(image)==194128 and sha(image)==IMAGE_HASH,'image identity')
    score._own_data[owndata.artifact_dir(score.ASM_DIR)]=owndata.ImageData.from_image(image,0x8038A400)
    receipts={}
    with tempfile.TemporaryDirectory(prefix='menu-renderer-research-') as temporary:
        temp=Path(temporary)
        for name,size in NAMES.items():
            run=temp/name
            run.mkdir()
            obj=run/'candidate.o'
            if name=='func_8038F648':
                score.compile_single(HERE/'input_sequence.c','-g0 -O2 -mips2 -G 0 -non_shared',obj)
            else:
                score.compile_group(HERE,obj)
            result=score.compare(obj,name,show=0)
            require(result.differing==2 and result.total==size//4,'research score changed')
            require(not any((result.unresolved,result.unverified,result.errors,result.extra_words)),'uncertain complete body')
            sections,symbols,relocations=review.elf(obj)
            ti,_,raw=sections['.text']
            start,extent,kind,index=symbols[name]
            require((extent,kind,index)==(size,2,ti),'full ELF extent')
            words=list(struct.unpack('>%dI'%(len(raw)//4),raw))
            resolved,masks,ur,uv,errors=score.relocate(obj,words,start,start+size,score.image_symbols())
            require(not any((masks,ur,uv,errors)),'full relocation uncertainty')
            body=resolved[start//4:(start+size)//4]
            review.SIZES={name:size}
            linked,counts=review.gnu_link(score,obj,run)
            require(linked[name]==body,'independent GNU relocation differs')
            native=targets[name]
            differences=[i*4 for i,(a,b) in enumerate(zip(native,body)) if a!=b]
            require(differences==({'func_8038F648':[0x90,0x98],'func_803A6F2C':[0xc,0x248]}[name]),'residual moved')
            calls=lambda code:[hex(0x80000000|((w&0x3ffffff)<<2)) for w in code if w>>26==3]
            require(calls(native)==calls(body),'helper call sequence')
            receipts[name]={'score':dataclasses.asdict(result),'native_bytes':len(native)*4,'compiled_bytes':size,
                'native_sha256':sha(review.packed(native)),'compiled_sha256':sha(review.packed(body)),
                'gnu_sha256':sha(review.packed(linked[name])),'gnu_relocations':counts[name],
                'residual_offsets':[hex(x) for x in differences],'same_ordered_calls':calls(native)}
            if name=='func_803A6F2C':
                root='func_803A7918'
                receipts[root]={'score':dataclasses.asdict(score.compare(obj,root,show=0)),
                    'native_bytes':len(targets[root])*4,'compiled_bytes':symbols[root][1],
                    'status':'Complete reconstruction, unverified owned switch table, NONMATCH'}
        for source in ('group.c','input_sequence.c'):
            p=subprocess.run(['gcc','-std=c89','-pedantic-errors','-fsyntax-only',str(HERE/source)],capture_output=True,text=True)
            require(p.returncode==0,p.stderr)
    hashes={str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in [HERE/'group.c',HERE/'group.json',HERE/'input_sequence.c',HERE/'verify.py',peer/'review.py',peer/'elf_reader.py',ROOT/'tools/cloud/score.py',ROOT/'tools/cloud/owndata.py']}
    return {'image':'A','image_sha256':IMAGE_HASH,'baseline':'84bdadc830834bde962aa532874a114b48353a06','claims':[],
        'matching_candidate_bytes':0,'accepted_bytes':0,'results':receipts,'hashes':hashes,
        'protected_manifest':{n:sha((score.ASM_DIR/n).read_bytes()) for n in manifest},
        'toolchain_sha256':{n:sha(Path(score.ido(n)).read_bytes()) for n in ('cc','cfe','uld','usplit','umerge','uopt','ugen','as1')},
        'seconds':round(time.monotonic()-started,4)}


def verify():
    """Keep cross-packet module imports and search paths isolated."""
    previous_path=list(sys.path)
    absent=object()
    previous={name:sys.modules.get(name,absent) for name in ('native','elf_reader')}
    peer=ROOT/'cloud/work/frontier/dot_runtime_a_leaf_review_20261011'
    try:
        sys.path.insert(0,str(peer))
        module('native',peer/'native.py')
        module('elf_reader',peer/'elf_reader.py')
        return _verify()
    finally:
        sys.path[:]=previous_path
        for name,value in previous.items():
            if value is absent:
                sys.modules.pop(name,None)
            else:
                sys.modules[name]=value


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out',type=Path)
    args=parser.parse_args()
    receipt=verify()
    if args.out:
        args.out.write_text(json.dumps(receipt,indent=2)+'\n')
    print(json.dumps(receipt,indent=2))
