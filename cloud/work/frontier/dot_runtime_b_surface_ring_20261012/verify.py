#!/usr/bin/env python3
"""Input-bound, full-extent NONMATCH research; no uncertainty waiver."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
NAME='func_8038A408'
ADDRESS=0x8038A408
SIZE=1220
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
BASELINE=ROOT/'cloud/work/lean/runtime_b_surface_ring_20261006/candidate.c'
TEST=ROOT/'tests/cloud/test_runtime_b_surface_ring_research.py'
sys.path.insert(0,str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers


def require(ok,message):
    if not ok: raise ValueError(message)


def sha(data): return hashlib.sha256(data).hexdigest()
def packed(words): return struct.pack('>%dI'%len(words),*words)


@contextmanager
def targets(population='ovl_b'):
    prior=score.ASM_DIR
    score.ASM_DIR=ROOT/'asm/us'/population
    try: yield
    finally: score.ASM_DIR=prior


def image_bytes():
    asset=(ROOT/'assets/us/data.bin').read_bytes()
    require(sha(asset)=='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8','asset identity')
    require(struct.unpack_from('>I',asset,0x3854)[0]==0xB6FEC4,'loader pointer')
    image=zlib.decompress(asset[0xB6FEC4-0x283D0:],-15)
    require(len(image)==43888 and sha(image)=='b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd','image identity')
    return image


def inputs():
    image=image_bytes()
    recipe=json.loads((HERE/'group.json').read_text())
    require(recipe['members']==[NAME] and recipe['claims']==[] and recipe['keep']==[NAME] and recipe['context']==[], 'research-only group roles')
    require(recipe['files']==['group.c'] and recipe['flags']==FLAGS, 'stock compiler recipe')
    with targets():
        manifest=score.target_manifest()
        extents=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))['functions']
        extent=next(x for x in extents if x['name']==NAME)
        require(int(extent['address'],16)==ADDRESS and extent['size']==SIZE,'native extent')
        require(min(int(x['address'],16) for x in extents if int(x['address'],16)>ADDRESS)==ADDRESS+SIZE,'next boundary')
        native=packed(score.targets()[NAME])
        require(native==image[ADDRESS-0x8038A400:ADDRESS-0x8038A400+SIZE],'complete image body')
        symbols=score.image_symbols()
        names=['D_8014A250','D_80399A70','D_80399AD8','D_80394888','D_803948B8','camera_trigger_check','func_8008C874','func_800A78BC','func_8008C074']
        bindings={n:symbols.get(n,score.address_named(n)) for n in names}
        require(all(v is not None for v in bindings.values()),'external addresses')
    callers=[]
    for population in ('blob','ovl_b'):
        with targets(population):
            symbols=score.image_symbols()
            for name,words in score.targets().items():
                sites=[i*4 for i,w in enumerate(words) if w>>26==3 and
                       (((symbols[name]+i*4+4)&0xf0000000)|((w&0x3ffffff)<<2))==ADDRESS]
                if sites: callers.append({'population':population,'name':name,'sites':sites,'body_sha256':sha(packed(words))})
    require([(c['population'],c['name'],c['sites']) for c in callers]==[('blob','entity_physics_update',[600])],'native caller census')
    with targets('blob'):
        caller=score.targets()['entity_physics_update']
        require(caller[0x248//4]==0x00aa0019 and caller[0x254//4]==0x012e2021 and caller[0x25c//4]==0x24840008,'caller a0=player position, live a1=player index')
        helpers={n:sha(packed(score.targets()[n])) for n in ('camera_trigger_check','func_8008C874','func_800A78BC','func_8008C074')}
    return {'population':'ovl_b','native_extent':extent,'next_boundary':hex(ADDRESS+SIZE),'native_sha256':sha(native),'image_sha256':sha(image),
            'external_bindings':bindings,'callers':callers,'helpers':helpers,'flags':FLAGS,'compiler':'ido5.3',
            'baseline_sha256':sha(BASELINE.read_bytes()),'source_files':{n:sha((HERE/n).read_bytes()) for n in ('group.c','group.json','verify.py','trace.py','host_test.c')},'test_sha256':sha(TEST.read_bytes())}


def forms():
    candidate=(HERE/'group.c').read_text()
    without=re.sub(r'^    s32 frame_residual_00[^\n]+\n','',candidate,flags=re.M)
    require(without!=candidate and 'frame_residual_00' not in without,'exact-frame control')
    baseline=BASELINE.read_text()
    return [('baseline',baseline,268,1172,200),('candidate',candidate,105,1208,248),('no_frame_allowance',without,None,None,192)]


def inspect(path,linked=False):
    raw,secs=score._elf(path)
    h=struct.unpack_from('>16sHHIIIIIHHHHHH',raw)
    require(h[0][:9]==b'\x7fELF\x01\x02\x01\0\0' and h[1]==(2 if linked else 1) and h[2]==8 and h[7]==0x10000000,'ELF/MIPS2 ABI')
    syms={s['name']:s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(raw,secs,i) if s['name']}
    ti=score._text_index(secs)
    require({n for n,s in syms.items() if s['type']==2 and s['section'] not in (0,0xfff1)}=={NAME},'complete defined-function set')
    fn=syms[NAME];require(fn['section']==ti and fn['value']==(ADDRESS if linked else 0) and fn['size']>0 and fn['size']%4==0,'complete STT_FUNC extent')
    text=None
    for i,sec in enumerate(secs):
        sh=struct.unpack_from('>IIIIIIIIII',raw,h[6]+i*h[11])
        require(not linked or sec['type'] not in (4,9) or sec['size']==0,'residual relocations')
        if sh[2]&2 and sec['size']:
            if sec['name']=='.text':
                require(sh[3]==(ADDRESS if linked else 0) and sec['size']==(fn['size']+15)//16*16,'text placement/extent')
                text=raw[sec['off']:sec['off']+sec['size']]
            else: require(not linked and sec['name'] in ('.reginfo','.options'),'unexpected allocated data '+sec['name'])
    require(text is not None and not any(text[fn['size']:]),'nonzero extent suffix')
    return raw,secs,syms,text,fn['size']


def gnu(obj,directory):
    raw,secs,syms,text,size=inspect(obj)
    addresses=score.image_symbols()
    undefined={n:addresses.get(n,score.address_named(n)) for n,s in syms.items() if s['section']==0}
    relocations=[]
    for sec in secs:
        if sec['type']!=9:continue
        require(sec['info']==score._text_index(secs),'unexpected relocated section')
        names=score._symbol_table(raw,secs,sec['link'])
        for at in range(sec['off'],sec['off']+sec['size'],8):
            offset,info=struct.unpack_from('>II',raw,at);symbol=names[info>>8]
            require(offset%4==0 and 0<=offset<size and info&255 in (4,5,6),'relocation range/type')
            require(symbol['section']==0 and symbol['name'] in undefined and undefined[symbol['name']] is not None,'unsupported local/unbound relocation')
            relocations.append({'offset':offset,'kind':info&255,'symbol':symbol['name']})
    require({n for n,a in undefined.items() if a is None}=={'fabsf'},'unexpected unresolved intrinsic')
    undefined={n:a for n,a in undefined.items() if a is not None}
    script=directory/'link.ld';script.write_text('\n'.join('%s = 0x%08x;'%p for p in sorted(undefined.items()))+'\nSECTIONS { .text 0x8038A408 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n')
    linked=directory/'linked.elf'
    subprocess.run([shutil.which('mips-linux-gnu-ld'),'-EB','--hash-style=sysv','-T',str(script),'-o',str(linked),str(obj)],check=True,capture_output=True)
    _,_,outsyms,out,linked_size=inspect(linked,True)
    require(linked_size==size and not [n for n,s in outsyms.items() if s['section']==0],'unresolved linked symbol or extent drift')
    require(all(outsyms[n]['value']==a and outsyms[n]['section']==0xfff1 for n,a in undefined.items()),'GNU external bindings')
    native=packed(score.targets()[NAME])
    different=[i for i in range(0,SIZE,4) if native[i:i+4]!=(out[i:i+4] if i<size else b'')]
    frame=next(-(w&65535)+65536 for w in struct.unpack('>%dI'%(size//4),out[:size]) if w>>16==0x27bd and w&0x8000)
    return {'function_bytes':size,'native_bytes':SIZE,'differing_offsets':different,'missing_native_tail_bytes':max(0,SIZE-size),'extra_nonzero_words':sum(bool(int.from_bytes(out[i:i+4],'big')) for i in range(SIZE,size,4)),
            'text_bytes':len(text),'zero_suffix_bytes':len(text)-size,'frame':frame,'relocations':relocations,'external_bindings':undefined,'linked_body_sha256':sha(out[:size]),'owned_data_bytes':0}


def replay():
    bound=inputs();score.ido('cc');require(compilers.identify_dir(score.IDO)=='ido5.3','stock IDO5.3 required')
    require(shutil.which('mips-linux-gnu-ld') is not None,'GNU MIPS binutils missing')
    passes={n:sha(Path(score.ido(n)).read_bytes()) for n in ('cc','cfe','uld','usplit','umerge','uopt','ugen','as1')}
    results={}
    with tempfile.TemporaryDirectory(prefix='surface-ring-research-') as tmp,targets():
        for label,source,differences,extent,frame in forms():
            directory=Path(tmp)/label;directory.mkdir();(directory/'group.c').write_text(source);(directory/'group.json').write_bytes((HERE/'group.json').read_bytes())
            obj=directory/'candidate.o';score.compile_group(directory,obj)
            result=score.compare(obj,NAME,show=0)
            require(not result.unresolved and not result.unverified and not result.errors and not result.extra_words,'uncertainty or extra instructions')
            independent=gnu(obj,directory)
            require(independent['frame']==frame,'frame residual changed')
            if differences is not None:require(result.differing==differences and independent['function_bytes']==extent,'stock control drift')
            require(len(independent['differing_offsets'])==result.differing,'GNU and scorer full-word disagreement')
            results[label]={'comparison':asdict(result),'gnu':independent}
    return {'protocol_version':2,'claims':[],'new_strict_verified_bytes':0,'accepted_image_bytes':0,'inputs':bound,'compiler_passes':passes,'results':results}


def host(sanitize=False):
    cc=shutil.which('cc');require(cc is not None,'C compiler absent')
    with tempfile.TemporaryDirectory(prefix='surface-host-') as temp:
        d=Path(temp);old=d/'baseline.o';exe=d/'host'
        flags=['-O1','-fno-fast-math','-ffp-contract=off']
        if sanitize:flags+=['-fsanitize=address,undefined','-fno-omit-frame-pointer']
        subprocess.run([cc,*flags,'-Dfunc_8038A408=baseline_func','-c',str(BASELINE),'-o',str(old)],check=True,capture_output=True)
        subprocess.run([cc,*flags,str(HERE/'host_test.c'),str(old),'-lm','-o',str(exe)],check=True,capture_output=True)
        output=subprocess.check_output([str(exe)],text=True,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0'))
        require(output=='345600 finite actual-source equivalence cases\n','host matrix count')
        return output.strip()


def main():
    p=argparse.ArgumentParser();p.add_argument('--write',action='store_true');p.add_argument('--score',action='store_true');p.add_argument('--deep',action='store_true');args=p.parse_args()
    receipt=replay()
    if args.write:(HERE/'verification.json').write_text(json.dumps(receipt,indent=2,sort_keys=True)+'\n')
    else:require(receipt==json.loads((HERE/'verification.json').read_text()),'input-bound receipt differs')
    if args.score:
        prior=sys.argv[:]
        try:
            sys.argv=['score.py','group',str(HERE),'--targets',str(ROOT/'asm/us/ovl_b')]
            with targets():require(score.main()==1,'NONMATCH must fail canonical command')
        finally:sys.argv[:]=prior
    if args.deep:print(host());print(host(True)+' (ASan/UBSan)')
    print('Verified empty-claims research: baseline 268/305; stock candidate 105/305; no coverage credit.')


if __name__=='__main__':main()
