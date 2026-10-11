#!/usr/bin/env python3
"""Strict image-A full-body, independent GNU relocation, and semantic verification."""
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

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
BASELINE = ROOT/'cloud/work/frontier/dot_runtime_a_available_list_20261011'
BOUNDARY = ROOT/'cloud/work/frontier/dot_runtime_a_availability_boundary_20261006'
sys.path[:0] = [str(ROOT/'tools/cloud'), str(BOUNDARY), str(HERE)]
spec=importlib.util.spec_from_file_location('record_context_score',ROOT/'tools/cloud/score.py')
score=importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
import owndata
from elf_support import elf
semantics_spec=importlib.util.spec_from_file_location('record_context_semantics',HERE/'semantics.py')
semantics=importlib.util.module_from_spec(semantics_spec);semantics_spec.loader.exec_module(semantics)
NAMES={'func_8039E200':444,'func_8039E3BC':136,'func_8039D494':528,'func_8039ED20':480}
CLAIM='func_8039E200'
IMAGE_HASH='0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
def require(condition,message):
    if not condition:raise ValueError(message)
def sha(data):return hashlib.sha256(data).hexdigest()
def packed(words):return struct.pack('>%dI'%len(words),*words)
def shell(*args,**kwargs):
    result=subprocess.run(list(map(str,args)),capture_output=True,text=True,**kwargs)
    require(result.returncode==0,(args,result.stdout,result.stderr))
    require('runtime error:' not in result.stderr,result.stderr)
    return result.stdout

def gnu_resolve(obj, temp):
    """Re-section complete ELF function bodies; GNU independently applies relocations.

    Local .text addends are translated to their ELF STT_FUNC owner and offset.
    This is not a claim that this extracted three-function ELF is a full image.
    """
    secs, syms, rels = elf(obj)
    ti, _, raw = secs['.text']; lines = ['.set noreorder', '.set noat']; externs = set(); counts = {}
    for name, size in NAMES.items():
        start, actual, kind, section = syms[name]
        assert (actual, kind, section) == (size, 2, ti)
        following = min(v[0] for v in syms.values() if v[2] == 2 and v[3] == ti and v[0] > start)
        assert following == start + size, (name, actual, following)
        words = list(struct.unpack('>%dI' % (size // 4), raw[start:start+size])); entries = []
        for loc, typ, symbol, dest in rels:
            if dest != ti or not start <= loc < start + size: continue
            assert typ in (4, 5, 6) and loc % 4 == 0
            off = loc - start
            if symbol == '.text':
                assert typ == 4
                add = (words[off//4] & 0x3ffffff) * 4
                owners = [(n,v) for n,v in syms.items() if v[2] == 2 and v[3] == ti and v[0] <= add < v[0]+v[1]]
                assert len(owners) == 1
                symbol, owner = owners[0]
                words[off//4] = (words[off//4] & 0xfc000000) | ((add-owner[0])//4)
            assert not symbol.startswith('.')
            entries.append((off, typ, symbol)); externs.add(symbol)
        lines += ['.section .review_' + name + ',"ax",@progbits', '.balign 4', '.globl ' + name, '.type '+name+',@function', name+':']
        lines += ['.word 0x%08x' % w for w in words]
        lines += ['.size %s, .-%s' % (name, name)]
        lines += ['.reloc %s+%d,%s,%s' % (name, off, {4:'R_MIPS_26',5:'R_MIPS_HI16',6:'R_MIPS_LO16'}[typ], sym) for off,typ,sym in entries]
        counts[name] = len(entries)
    assembly = temp/'review.s'; assembly.write_text('\n'.join(lines)+'\n')
    reloc = temp/'review.o'; linked = temp/'review.elf'; script = temp/'review.ld'
    script.write_text('SECTIONS {\n'+''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n' % (n,n[-8:],n) for n in NAMES)+' /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n'+''.join('%s = 0x%X;\n' % (n, score.image_symbols().get(n,score.address_named(n))) for n in sorted(externs-set(NAMES))))
    shell('mips-linux-gnu-as','-EB','-mips2','-o',reloc,assembly)
    shell('mips-linux-gnu-ld','-EB','-T',script,'-o',linked,reloc)
    ss, sy, rr = elf(linked); assert not rr
    result = {}
    for n,size in NAMES.items():
        index, row, body = ss['.review_'+n]
        assert row[3] == int(n[-8:],16) and len(body) == size
        assert sy[n] == (int(n[-8:],16),size,2,index)
        result[n] = list(struct.unpack('>%dI' % (size//4), body))
    return result, counts

def expected_source(source):
    source=source.replace('extern Record **D_80152028,**D_803BA010,**D_80152698[4];',
        'typedef struct RecordList {u8 indirect,doubly,unknown02[2];s32 count;Record **first, **last;} RecordList;\nextern RecordList D_80152020;\nextern Record **D_803BA010,**D_80152698[4];')
    source=source.replace('extern Record **D_8015202C;\n','').replace('D_80152028','D_80152020.first').replace('D_8015202C','D_80152020.last')
    source=source.replace('(!record->file || menu_item_value_get(node))','(!(*node)->file || menu_item_value_get(node))')
    start=source.index('void func_8039D05C(void)\n{');end=source.index('\ntypedef struct Color4',start)
    body=source[start:end].replace('s32 player,i;', 's32 player,i,count;')
    body=body.replace('for(player=0;player<D_8014A108;player++) {','for(player=0;player<(count=D_8014A108);player++) {')
    body=body.replace('i<44','i!=44').replace('i<11','i!=11').replace('part->handle=part->auxiliary=-1;','part->handle=-1;part->auxiliary=-1;')
    body=body.replace('for(i=0,part=D_803B2618[player];i!=11;i++,part++) {','for(i=0;i!=11;i++) {\n                part=&D_803B2618[player][i];')
    body=body.replace('if(player<=0) {','if(player>0)continue;\n        {')
    return source[:start]+body+source[end:]

def verify_layout(source,temp):
    declarations=source[:source.index('void func_8039D300(s32 player)')]
    panel=source[source.index('typedef struct PanelPart'):source.index('extern PanelPart')]
    values=[('RecordList size','sizeof(RecordList)',16),('RecordList first','OFF(RecordList,first)',8),
        ('RecordList last','OFF(RecordList,last)',12),('Record size','sizeof(Record)',44),
        ('Record next','OFF(Record,next)',0),('Record file','OFF(Record,file)',4),
        ('Record track','OFF(Record,track)',8),('Record mode','OFF(Record,mode)',9),
        ('Record active flag','OFF(Record,unknown24)',36),('Record associated car','OFF(Record,active)',40),
        ('Player size','sizeof(Player)',76),('Slot size','sizeof(Slot)',64),('PanelPart size','sizeof(PanelPart)',52)]
    layout=temp/'layout.c';layout.write_text(declarations+panel+'\n#define OFF(T,F) ((u32)&((T*)0)->F)\n'+
        'u32 record_context_layout[]={'+','.join(expr for _,expr,_ in values)+'};\n')
    obj=temp/'layout.o';score.compile_single(layout,'-g0 -O2 -mips2 -G 0 -non_shared',obj)
    sections,symbols,relocs=elf(obj);offset,size,kind,index=symbols['record_context_layout']
    _,_,data=next(value for value in sections.values() if value[0]==index)
    require(size==len(values)*4 and not any(dest==index for _,_,_,dest in relocs),'layout array extent or relocation')
    actual=list(struct.unpack('>%dI'%len(values),data[offset:offset+size]))
    require(actual==[expected for _,_,expected in values],'MIPS source layout differs')
    return {label:value for (label,_,_),value in zip(values,actual)}

def verify():
    require(__debug__,'Run without Python -O: GNU helper uses assertions')
    start=time.monotonic();score.ASM_DIR=ROOT/'asm/us/ovl_a'
    manifest=score.target_manifest();targets=score.targets()
    extents=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))
    require((extents['image'],extents['base'],extents['rom_offset'])==('A','0x8038A400','0xB5C534'),'wrong image')
    source=(HERE/'group.c').read_text();prior=(BASELINE/'group.c').read_text()
    require(source==expected_source(prior),'unexpected source delta')
    config=json.loads((HERE/'group.json').read_text());oldconfig=json.loads((BASELINE/'group.json').read_text())
    expected_config=dict(oldconfig,claims=[CLAIM],
        members=['func_8039D494','func_8039E3BC',CLAIM,'func_8039ED20'],
        context=[n for n in oldconfig['context'] if n != CLAIM])
    require(config==expected_config,'unexpected group recipe')
    asset=(ROOT/'assets/us/data.bin').read_bytes()
    require(sha(asset)=='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8','asset identity')
    require(int.from_bytes(asset[0x3850:0x3854],'big')==0xB5C534,'loader pointer')
    image=zlib.decompress(asset[0xB5C534-0x283D0:],-15)
    require(len(image)==194128 and sha(image)==IMAGE_HASH,'image identity')
    score._own_data[owndata.artifact_dir(score.ASM_DIR)]=owndata.ImageData.from_image(image,0x8038A400)
    with tempfile.TemporaryDirectory(prefix='record-context-') as td:
        temp=Path(td);obj=temp/'candidate.o';score.compile_group(HERE,obj)
        scores={name:dataclasses.asdict(score.compare(obj,name,show=0)) for name in config['members'] + config.get('context', [])}
        for result in scores.values():result['errors']=['owned-data comparison mismatch' if 'retail' in e else e for e in result['errors']]
        linked,relocs=gnu_resolve(obj,temp);bindings={}
        for name,size in NAMES.items():
            extent=next(f for f in extents['functions'] if f['name']==name)
            require(int(extent['address'],16)==int(name[-8:],16) and extent['size']==size,'extent drift')
            require(score.compare(obj,name,show=0).accepted(),name+' not exact')
            words=score.text_words(obj);offset=score.symbols(obj)[name]
            resolved,masks,unresolved,unverified,errors=score.relocate(obj,words,offset,offset+size,score.image_symbols())
            require(not any((masks,unresolved,unverified,errors)),name+' relocation uncertainty')
            require(resolved[offset//4:(offset+size)//4]==linked[name]==targets[name],name+' resolved body differs')
            bindings[name]={'address':extent['address'],'bytes':size,'sha256':sha(packed(targets[name])),'gnu_relocations':relocs[name]}
        controls={}
        for label,variant in {
            'prior_source':prior,
            'cached_file_access':source.replace('(!(*node)->file || menu_item_value_get(node))','(!record->file || menu_item_value_get(node))'),
            'wrong_direction':source.replace('if(direction<0)next=D_803BA010+1;', 'if(direction<=0)next=D_803BA010+1;'),
            'wrong_tail':source.replace('else next=D_80152020.last;', 'else next=D_80152020.first;'),
        }.items():
            directory=temp/label;directory.mkdir();(directory/'group.c').write_text(variant);(directory/'group.json').write_text(json.dumps(config))
            other=directory/'candidate.o';score.compile_group(directory,other);result=score.compare(other,CLAIM,show=0)
            require(not result.accepted(),label+' must fail');controls[label]=dataclasses.asdict(result)
        layouts=verify_layout(source,temp)
        callers=semantics.audit_calls(obj,score,elf,targets)
        behavior=semantics.verify(source,targets[CLAIM],linked[CLAIM],temp)
    return {'status':'PASS','image':'A','baseline_commit':'4296906ea06f551540a29a2547ffdffe92e92104',
        'claim':CLAIM,'new_candidate_bytes':444,'accepted_image_bytes':0,'cartridge_bytes':0,
        'flags':config['flags']+' -Wab,-r4300_mul','source_sha256':sha(source.encode()),
        'group_sha256':sha((HERE/'group.json').read_bytes()),'image_sha256':IMAGE_HASH,
        'bindings':bindings,'scores':scores,'negative_controls':controls,'semantics':behavior,'callsites':callers,'MIPS_layouts':layouts,
        'elapsed_seconds':round(time.monotonic()-start,3)}

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path);args=parser.parse_args()
    result=verify();output=json.dumps(result,indent=2)+'\n'
    if args.output:args.output.write_text(output)
    print(output)
