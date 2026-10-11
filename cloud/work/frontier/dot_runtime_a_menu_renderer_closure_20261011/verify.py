#!/usr/bin/env python3
"""Authenticate four complete NONMATCH bodies and owned switch data."""
import argparse
import dataclasses
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import subprocess
import sys
import tempfile
import time
import zlib

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
SIZES={'func_803A6F2C':596,'func_803A7180':1184,'func_803A7620':760,'func_803A7918':576}
EXPECTED={'func_803A6F2C':[0xc,0x248],'func_803A7180':[0,0x38,0xe4,0x154,0x21c,0x494],
          'func_803A7620':[0,0x2c,0x30,0x2ec],'func_803A7918':[0,0x34,0x18c,0x1a4,0x1b0,0x1cc,0x234]}
TABLE=0x803B9834
IMAGE_HASH='0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'

def require(ok,detail):
    if not ok: raise ValueError(detail)
def sha(raw): return hashlib.sha256(raw).hexdigest()
def packed(words): return struct.pack('>%dI'%len(words),*words)
def module(name,path):
    spec=importlib.util.spec_from_file_location(name,path)
    result=importlib.util.module_from_spec(spec);sys.modules[name]=result;spec.loader.exec_module(result)
    return result

def shell(*args,**kw):
    p=subprocess.run(list(map(str,args)),capture_output=True,text=True,**kw)
    require(p.returncode==0,(args,p.stdout,p.stderr))
    return p.stdout

def gnu_link(score,elf,obj,temp):
    """Relink complete function sections and exactly five owned table entries."""
    sections,symbols,rels=elf(obj)
    ti,_,text=sections['.text'];ri,_,rodata=sections['.rodata']
    require(len(rodata)==32 and not any(rodata[20:]),'unexpected literal payload')
    require([r[0] for r in rels if r[3]==ri]==list(range(0,20,4)),'switch relocation extent')
    lines=['.set noreorder','.set noat'];externals=set();counts={}
    owners={n:v for n,v in symbols.items() if n in SIZES}
    text_functions={n:v for n,v in symbols.items() if v[2]==2 and v[3]==ti}
    require(set(text_functions)==set(SIZES),'unexpected function ownership')
    ordered=sorted(text_functions.items(),key=lambda item:item[1][0])
    require(ordered[0][1][0]==0,'unowned leading code')
    for position,(name,value) in enumerate(ordered):
        boundary=value[0]+value[1]
        if position+1<len(ordered):
            require(boundary==ordered[position+1][1][0],'overlap or unexpected function gap')
        else:
            require(0<=len(text)-boundary<16 and not any(text[boundary:]),'unowned trailing code')
    for name,size in SIZES.items():
        start,actual,kind,index=owners[name]
        require((actual,kind,index)==(size,2,ti),'complete function extent')
    for name,start,size,index,data in [(n,v[0],SIZES[n],ti,text) for n,v in owners.items()]+[('menu_switch_table',0,20,ri,rodata)]:
        words=list(struct.unpack('>%dI'%(size//4),data[start:start+size]));records=[]
        for off,typ,sym,dest in rels:
            if dest!=index or not start<=off<start+size: continue
            local=off-start;word=words[local//4]
            require(typ in (2,4,5,6),'unknown relocation')
            if sym=='.text':
                require(typ in (2,4),'unexpected text relocation')
                add=(word&0x3ffffff)*4 if typ==4 else word
                owner=[(n,v) for n,v in owners.items() if v[0]<=add<v[0]+v[1]]
                require(len(owner)==1,'ambiguous local ownership')
                sym,value=owner[0];add-=value[0]
                words[local//4]=(word&0xfc000000)|(add//4) if typ==4 else add
            elif sym=='.rodata':
                require(index==ti and typ in (5,6),'unexpected owned-data reference')
                sym='menu_switch_table'
            require(not sym.startswith('.'),'unowned section')
            records.append((local,typ,sym));externals.add(sym)
        flags='a' if index==ri else 'ax'
        lines+=['.section .review_'+name+',"'+flags+'",@progbits','.balign 4','.globl '+name,
                '.type '+name+(',@object' if index==ri else ',@function'),name+':']
        lines+=['.word 0x%08x'%w for w in words]
        lines+=['.size %s,.-%s'%(name,name)]
        lines+=['.reloc %s+%d,%s,%s'%(name,o,{2:'R_MIPS_32',4:'R_MIPS_26',5:'R_MIPS_HI16',6:'R_MIPS_LO16'}[t],s) for o,t,s in records]
        counts[name]=len(records)
    source=temp/'independent.s';source.write_text('\n'.join(lines)+'\n')
    locations={n:int(n[-8:],16) for n in SIZES};locations['menu_switch_table']=TABLE
    assignments=[]
    for n in sorted(externals-set(locations)):
        a=score.image_symbols().get(n,score.address_named(n));require(a is not None,'unresolved external '+n)
        assignments.append('%s = 0x%X;'%(n,a))
    script=temp/'independent.ld';script.write_text('SECTIONS {\n'+''.join('.review_%s 0x%X : SUBALIGN(4) { *(.review_%s) }\n'%(n,a,n) for n,a in locations.items())+' /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n'+'\n'.join(assignments))
    shell('mips-linux-gnu-as','-mips2','-EB','-o',temp/'independent.o',source)
    shell('mips-linux-gnu-ld','-EB','-T',script,'-o',temp/'independent.elf',temp/'independent.o')
    ss,sy,rr=elf(temp/'independent.elf');require(not rr,'remaining GNU relocations')
    result={}
    for n,a in locations.items():
        index,row,body=ss['.review_'+n];require(row[3]==a and len(body)==(20 if n=='menu_switch_table' else SIZES[n]),'GNU ownership')
        require(sy[n][0]==a,'GNU symbol position');result[n]=body
    return result,counts

def compiler_metadata(score,canonical,temp):
    work=temp/'trace';work.mkdir();(work/'group.c').write_text((HERE/'group.c').read_text());(work/'keep.txt').write_text('func_803A7918\n')
    ido=score.ido;c=['-mips2','-EB','-g0','-O3']
    steps=[[ido('cc'),'-j','-g0','-O3','-mips2','-G','0','-non_shared','group.c'],
      [ido('uld'),'-L/usr/lib/mips2/nonshared','-_SYSTYPE_SVR4','-mips2','-non_shared','-g0','-no_AutoGnum','-kp','keep.txt','group.u','-ko','linked'],
      [ido('usplit'),'-mips2','-o','split','-t','st','linked'],
      [ido('umerge'),'-Olimit','5000',*c,'split','-o','merged','-t','st'],
      [ido('uopt'),'-G','0','-Olimit','5000',*c,'merged','opt','-t','st','optlog'],
      [ido('ugen'),'-G','0',*c,'opt','-o','gen','-t','st','-temp','ugtmp','-e','trees','-l','listing'],
      [ido('as1'),'-elf','-G','0','-p0',*c,score.R4300_AS1,'-Olimit','5000','gen','-o','diagnostic.o','-t','st']]
    for step in steps:shell(*step,cwd=work)
    require(canonical.read_bytes()==(work/'diagnostic.o').read_bytes(),'diagnostic compiler changed object')
    names=re.findall(r'^\s*\.ent\s+(\w+)',(work/'listing').read_text(),re.M)
    blocks=(work/'trees').read_text().split('Tree dump after Build')[1:]
    require(len(names)==len(blocks)==4,'diagnostic function correspondence')
    result={}
    for name,block in zip(names,blocks):
        block=block.split('Tree dump')[0]
        result[name]={'external':int(re.search(r'\bent .*?external=(\d+)',block)[1]),
            'area_lengths':{a:int(b) for a,b in re.findall(r'def mtype=(\w+) length=(\d+)',block)}}
    return {'canonical_object_identical':True,'object_sha256':sha(canonical.read_bytes()),'functions':result}

def _verify():
    started=time.monotonic();peer=ROOT/'cloud/work/frontier/dot_runtime_a_leaf_review_20261011'
    sys.path[:0]=[str(ROOT/'tools/cloud'),str(peer)]
    score=module('renderer_closure_score',ROOT/'tools/cloud/score.py');elf=module('renderer_closure_elf',peer/'elf_reader.py').elf
    import owndata
    score.ASM_DIR=ROOT/'asm/us/ovl_a';manifest=score.target_manifest();targets=score.targets()
    asset=(ROOT/'assets/us/data.bin').read_bytes();require(sha(asset)=='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8','asset identity')
    require(int.from_bytes(asset[0x3850:0x3854],'big')==0xB5C534,'loader pointer')
    image=zlib.decompress(asset[0xB5C534-0x283D0:],-15);require(len(image)==194128 and sha(image)==IMAGE_HASH,'image identity')
    score._own_data[owndata.artifact_dir(score.ASM_DIR)]=owndata.ImageData.from_image(image,0x8038A400)
    config=json.loads((HERE/'group.json').read_text());require(config['claims']==[] and config['keep']==['func_803A7918'],'claim/root drift')
    results={}
    with tempfile.TemporaryDirectory(prefix='menu-closure-') as td:
        temp=Path(td);obj=temp/'canonical.o';score.compile_group(HERE,obj)
        sections,symbols,rels=elf(obj);_,_,raw=sections['.text'];words=list(struct.unpack('>%dI'%(len(raw)//4),raw))
        linked,counts=gnu_link(score,elf,obj,temp)
        for n,size in SIZES.items():
            result=score.compare(obj,n,show=0);require(result.differing==len(EXPECTED[n]) and result.total==size//4,'score drift')
            require(not any((result.unresolved,result.unverified,result.errors,result.extra_words)),'relocation uncertainty')
            start=symbols[n][0];resolved,masks,ur,uv,errors=score.relocate(obj,words,start,start+size,score.image_symbols())
            require(not any((ur,errors)),'complete relocation failure')
            body=resolved[start//4:(start+size)//4];native=targets[n]
            if masks:
                require(n=='func_803A7918' and set(masks)=={start+0xa0,start+0xa8} and len(uv)==2,'unexpected own-data sites')
                # Strict scorer independently proves ownership; GNU resolves the
                # two complete fields, which are then compared without masks.
                gnu_words=list(struct.unpack('>%dI'%(size//4),linked[n]))
                for off,mask in masks.items():
                    i=(off-start)//4
                    require(body[i]&mask==gnu_words[i]&mask,'GNU changed nonrelocation bits')
                    body[i]=gnu_words[i]
            else:
                require(not uv,'unverified relocation')
            require(packed(body)==linked[n],'independent GNU differs')
            diffs=[4*i for i,(a,b) in enumerate(zip(native,body)) if a!=b];require(diffs==EXPECTED[n],'residual moved')
            for off in diffs:require(native[off//4]>>16==body[off//4]>>16,'non-home operand changed')
            calls=lambda code:[hex(0x80000000|((w&0x3ffffff)<<2)) for w in code if w>>26==3]
            require(calls(native)==calls(body),'ordered call sequence')
            results[n]={'score':dataclasses.asdict(result),'native_bytes':len(native)*4,'compiled_bytes':symbols[n][1],
                'native_sha256':sha(packed(native)),'compiled_sha256':sha(packed(body)),'gnu_sha256':sha(linked[n]),
                'gnu_relocations':counts[n],'residual_offsets':[hex(x) for x in diffs],
                'native_frame':-next(struct.unpack('>h',packed([w])[2:])[0] for w in native if w>>16==0x27bd),
                'compiled_frame':-next(struct.unpack('>h',packed([w])[2:])[0] for w in body if w>>16==0x27bd),
                'same_ordered_calls':calls(native)}
        original=image[TABLE-0x8038A400:TABLE-0x8038A400+20];require(linked['menu_switch_table']==original,'owned switch values differ')
        data={'address':hex(TABLE),'bytes':20,'entry_count':5,'sha256':sha(original),'gnu_sha256':sha(linked['menu_switch_table']),
              'gnu_relocations':counts['menu_switch_table']}
        diagnostics=compiler_metadata(score,obj,temp)
        shell('gcc','-std=c89','-pedantic-errors','-fsyntax-only',HERE/'group.c')
    paths=[HERE/'group.c',HERE/'group.json',HERE/'verify.py',peer/'elf_reader.py',ROOT/'tools/cloud/score.py',ROOT/'tools/cloud/owndata.py']
    return {'image':'A','image_sha256':IMAGE_HASH,'baseline':'02e08c715d1db5462e4823b816379b30b8020b9e','claims':[],
        'matching_candidate_bytes':0,'accepted_bytes':0,'results':results,'owned_switch_table':data,'stock_ugen_metadata':diagnostics,
        'capacity_note':'Live C buffer capacities and field extents are hypotheses, not authenticated ownership or string-safety proofs.',
        'hashes':{str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in paths},
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
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--out',type=Path);args=parser.parse_args()
    result=verify()
    if args.out:args.out.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,indent=2))
