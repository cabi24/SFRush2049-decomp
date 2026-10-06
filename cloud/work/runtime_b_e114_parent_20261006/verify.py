#!/usr/bin/env python3
"""Reproduce first-milestone metadata; never emit native bytes or disassembly."""
import argparse
import contextlib
from dataclasses import asdict
import hashlib
import importlib.util
import io
import json
import os
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
BASE = 'dea99f09ab19b1d3b324ed7097162f7b378e7096'
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
IMAGE_HASH = 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
CHILDREN = ['func_8038D200','func_8038D328','func_8038E088']
BINDINGS = {'math_utility':0x8008D6B0,'func_80090F44':0x80090F44,
 'func_8008E3C0':0x8008E3C0,'func_8008E398':0x8008E398,
 'sound_call_minimal':0x80090254,'func_800AFA84':0x800AFA84,
 'D_80394F70':0x80394F70,'D_80399B18':0x80399B18,'D_8011418C':0x8011418C}

def sha(data): return hashlib.sha256(data).hexdigest()
def run(args, **kwargs):
    p=subprocess.run([str(x) for x in args],capture_output=True,text=True,**kwargs)
    assert p.returncode == 0,(args,p.returncode,p.stdout,p.stderr)
    return p.stdout

def load_image(reference):
    asset=subprocess.check_output(['git','-C',str(reference),'show',BASE+':assets/us/data.bin'])
    assert sha(asset)=='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
    inflater=zlib.decompressobj(-15)
    image=inflater.decompress(asset[0xB6FEC4-0x283D0:])
    assert inflater.eof and len(image)==43888 and sha(image)==IMAGE_HASH
    return image

def portable_elf(data):
    """Bind every section and ABI attribute except nonallocated ECOFF debug data.

    Absolute source paths change .mdebug size and physical file offsets. Neither
    executable/data bytes, relocations, symbols nor MIPS ABI metadata are masked.
    """
    assert data[:6]==b"\x7fELF\x01\x02"
    header=struct.unpack_from('>HHIIIIIHHHHHH',data,16)
    assert header[0]==1 and header[1]==8 and header[9]==0
    shoff,shsize,shnum,names_index=header[5],header[10],header[11],header[12]
    assert shsize==40 and shoff+shnum*shsize<=len(data)
    sections=[struct.unpack_from('>10I',data,shoff+shsize*i) for i in range(shnum)]
    names=sections[names_index];records=[]
    for row in sections:
        index,typ,flags,addr,offset,size,link,info,align,entry_size=row
        assert typ==8 or offset+size<=len(data)
        assert index<names[5]
        start=names[4]+index
        name=data[start:data.index(b'\0',start,names[4]+names[5])].decode()
        record=dict(name=name,type=typ,flags=flags,address=addr,link=link,
                    info=info,alignment=align,entry_size=entry_size)
        if name=='.mdebug':
            assert typ==0x70000005 and flags==0 and addr==0,'unexpected allocated/debug section'
            record['scope']='nonallocated ECOFF debug metadata; fingerprinted separately'
        else:
            record['size']=size
            record['sha256']=sha(data[offset:offset+size]) if typ!=8 else None
        records.append(record)
    return {'ident_sha256':sha(data[:16]),'header':list(header[:5])+list(header[6:]),'sections':records}


def portable_linked_elf(data):
    """Bind the linked image and symbol semantics, not GNU packing/order.

    Physical file offsets, string-table packing, symbol order and load-segment
    grouping may change between GNU versions. This packet executes allocated
    sections directly; every byte, virtual extent, section attribute, symbol
    binding/visibility and ELF ABI attribute below remains equality-bound.
    Unknown sections, dangling strings/symbols and retained relocations fail.
    """
    assert data[:6] == b'\x7fELF\x01\x02'
    header = struct.unpack_from('>HHIIIIIHHHHHH', data, 16)
    assert header[:3] == (2, 8, 1)
    phoff, shoff = header[4:6]
    assert header[7] == 52 and header[10] == 40
    assert shoff + header[10] * header[11] <= len(data)
    rows = [struct.unpack_from('>10I', data, shoff + 40 * i)
            for i in range(header[11])]
    assert header[12] < len(rows)
    names = rows[header[12]]

    def string(table, offset):
        assert table[1] == 3 and table[4] + table[5] <= len(data)
        assert offset < table[5]
        start = table[4] + offset
        return data[start:data.index(b'\0', start, table[4] + table[5])].decode()

    labels = [string(names, row[0]) for row in rows]
    assert len(set(labels)) == len(labels)
    assert set(labels) == {'', '.text', '.rodata', '.symtab', '.strtab', '.shstrtab'}
    assert rows[0] == (0,) * 10 and labels[0] == ''
    sections = []
    symbols = []
    assert header[8] == 32 and phoff + header[8] * header[9] <= len(data)
    segments = [struct.unpack_from('>8I', data, phoff + 32 * i)
                for i in range(header[9])]
    for row, name in zip(rows, labels):
        _, typ, flags, address, offset, size, link, info, alignment, entry = row
        assert offset + size <= len(data)
        if name in ('.text', '.rodata'):
            assert typ == 1 and flags & 2 and link == info == entry == 0
            # File/segment repacking cannot change the actual mapped bytes.
            assert any(kind == 1 and file_size <= memory_size
                       and file_offset <= offset
                       and offset + size <= file_offset + file_size
                       and virtual + offset - file_offset == address
                       for kind, file_offset, virtual, physical, file_size,
                           memory_size, permissions, align in segments)
            sections.append(dict(name=name, type=typ, flags=flags,
                address=address, size=size, alignment=alignment,
                sha256=sha(data[offset:offset + size])))
        elif name == '.symtab':
            assert typ == 2 and flags == address == 0 and entry == 16
            assert size % entry == 0 and link < len(rows) and labels[link] == '.strtab'
            assert 0 <= info <= size // entry
            for at in range(offset, offset + size, entry):
                no, value, extent, attrs, other, section = struct.unpack_from('>IIIBBH', data, at)
                label = string(rows[link], no)
                assert section in (0, 0xfff1) or section < len(rows)
                target = labels[section] if 0 < section < len(rows) else section
                symbols.append(dict(name=label, value=value, size=extent,
                    binding=attrs >> 4, type=attrs & 15, other=other, section=target))
        elif name:
            assert typ == 3 and flags == address == link == info == entry == 0
    return {'ident_sha256': sha(data[:16]),
            'abi': {'type': header[0], 'machine': header[1], 'version': header[2],
                    'entry': header[3], 'flags': header[6]},
            'allocated_sections': sorted(sections, key=lambda value: value['name']),
            'symbols': sorted(symbols, key=lambda value: json.dumps(value, sort_keys=True))}


def comparable(receipt):
    """Remove only integration and fingerprint-proven debug-path provenance."""
    result = json.loads(json.dumps(receipt))
    for key in ('target_manifest_sha256', 'tools'):
        result.pop(key, None)
    assert 'portable_elf' in result['object'], 'missing complete portable ELF proof'
    result['object'].pop('sha256', None)
    assert 'portable_elf' in result['independent_link'], 'missing linked ELF semantics'
    result['independent_link'].pop('sha256', None)
    return result


def inspect_elf(score,path,linked):
    data,sections=score._elf(path)
    header=struct.unpack_from('>16sHHIIIIIHHHHHH',data)
    assert header[1]==(2 if linked else 1) and header[2]==8
    allocated=[]; code={}; rodata=[]
    for i,section in enumerate(sections):
        raw=struct.unpack_from('>IIIIIIIIII',data,header[6]+i*header[11])
        if raw[2]&2 and raw[5]:
            assert section['name'] in ('.text','.rodata') or (not linked and section['name']=='.reginfo' and raw[5]==24)
            allocated.append({'name':section['name'],'address':hex(raw[3]),'size':raw[5],
                              'sha256':sha(data[raw[4]:raw[4]+raw[5]])})
            payload=data[raw[4]:raw[4]+raw[5]]
            if linked and section['name']=='.text':
                code={raw[3]+4*i:w for i,(w,) in enumerate(struct.iter_unpack('>I',payload))}
            elif linked and section['name']=='.rodata': rodata.append((raw[3],payload))
        if linked: assert section['type'] not in (4,9) or section['size']==0
    symbols={s['name']:s for i,s in enumerate(sections) if s['type']==2
             for s in score._symbol_table(data,sections,i) if s['name']}
    functions={n:s for n,s in symbols.items() if s['type']==2 and s['section'] not in (0,0xFFF1)}
    assert set(functions)==set(CHILDREN)
    undefined={n for n,s in symbols.items() if s['section']==0}
    assert undefined==(set() if linked else set(BINDINGS))
    if linked:
        for name,address in BINDINGS.items():
            assert symbols[name]['value']==address and symbols[name]['section']==0xFFF1
    return {'sha256':sha(data),'portable_elf':portable_linked_elf(data) if linked else portable_elf(data),'allocated_sections':allocated,
            'functions':{n:{'address':hex(s['value']),'size':s['size']} for n,s in functions.items()},
            'undefined_symbols':sorted(undefined)},code,{n:s['value'] for n,s in functions.items()},rodata

def main():
    parser=argparse.ArgumentParser(); parser.add_argument('--reference-root',type=Path,default=ROOT)
    parser.add_argument('--output',type=Path,default=HERE/'verification.json')
    parser.add_argument('--check',action='store_true'); args=parser.parse_args()
    reference=args.reference_root.resolve(); sys.path.insert(0,str(ROOT))
    spec=importlib.util.spec_from_file_location('tools.cloud._e114_private_score',ROOT/'tools/cloud/score.py')
    score=importlib.util.module_from_spec(spec); sys.modules[spec.name]=score; spec.loader.exec_module(score)
    from native import verify
    score.ASM_DIR=reference/'asm/us/ovl_b'
    manifest=score.target_manifest(); targets=score.targets()
    extent_data=score.verified_bytes(score.ASM_DIR/'extents.json',manifest)
    extents=json.loads(extent_data); by_name={x['name']:x for x in extents['functions']}
    image=load_image(reference)
    protected={n:sha((score.ASM_DIR/n).read_bytes()) for n in manifest}
    intervals={}
    selected=CHILDREN+['func_8038E114','func_8038FCE0','func_8038DA78','func_8038D498','func_8038DDDC','func_8038CB20']
    for name in selected:
        row=by_name[name]; address=int(row['address'],16); words=targets[name]
        raw=struct.pack('>'+str(len(words))+'I',*words)
        assert len(raw)==row['size'] and raw==image[address-0x8038A400:address-0x8038A400+len(raw)]
        calls=[{'site':hex(address+i*4),'destination':hex(((address+i*4+4)&0xF0000000)|((w&0x3FFFFFF)<<2))}
               for i,w in enumerate(words) if w>>26==3]
        intervals[name]={**row,'end':hex(address+len(raw)),'sha256':sha(raw),'direct_calls':calls}
    native_data=[(0x80394DB8,image[0x4DB8+0x80390000-0x8038A400:0x4DB8+0x80390000-0x8038A400+12]),
                 (0x80394DF4,image[0x4DF4+0x80390000-0x8038A400:0x4DF4+0x80390000-0x8038A400+36])]
    with tempfile.TemporaryDirectory(prefix='runtime-parent-') as temp:
        temp=Path(temp); obj=temp/'children.o'
        score.compile_single(HERE/'children.c',FLAGS,obj)
        object_meta,_,_,_=inspect_elf(score,obj,False)
        moved_dir = temp / 'different_source_path'; moved_dir.mkdir()
        moved = moved_dir / 'children.c'; moved.write_bytes((HERE / 'children.c').read_bytes())
        moved_object = moved_dir / 'children.o'; score.compile_single(moved, FLAGS, moved_object)
        original_data = obj.read_bytes(); fingerprint = object_meta['portable_elf']
        assert portable_elf(moved_object.read_bytes()) == fingerprint, 'source path changed semantic ELF'
        assert sha(moved_object.read_bytes()) != sha(original_data), 'debug path control ineffective'
        _, sections = score._elf(obj); semantic_controls = {}
        for section in sections:
            if section['name'] not in ('.text', '.rodata', '.rel.text', '.rel.rodata', '.reginfo', '.options', '.symtab', '.strtab') or not section['size']:
                continue
            changed = bytearray(original_data); changed[section['off']] ^= 1
            assert portable_elf(changed) != fingerprint, section['name']
            semantic_controls[section['name']] = 'rejected'
        changed = bytearray(original_data); changed[39] ^= 1
        assert portable_elf(changed) != fingerprint, 'ELF ABI mutation accepted'
        semantic_controls['ELF_ABI_flags'] = 'rejected'
        object_meta['portability_controls'] = {'different_source_path_same_complete_elf': True,
                                               'semantic_mutations': semantic_controls}
        comparisons={}
        for name in CHILDREN:
            with contextlib.redirect_stdout(io.StringIO()): comparison=score.compare(obj,name,show=0)
            comparisons[name]={**asdict(comparison),'status':comparison.summary(),'notes':list(comparison.notes)}
            assert comparison.differing > 0
        # Bind complete ELF sections, rather than borrowing neighboring bytes.
        script=temp/'candidate.ld'
        script.write_text('\n'.join(f'{n} = 0x{a:08X};' for n,a in BINDINGS.items())+
          '\nSECTIONS { .text 0x81000000 : { *(.text) } .rodata 0x81010000 : { *(.rodata) } '
          '/DISCARD/ : { *(.reginfo) *(.options) *(.mdebug) *(.comment) } }\n')
        linked=temp/'children.elf'
        run(['mips-linux-gnu-ld','-T',script,'-o',linked,obj])
        linked_meta,code,starts,rodata=inspect_elf(score,linked,True)
        native=verify(targets,native_data,code,starts,rodata)
        for name, count in native['candidate_instruction_coverage'].items():
            assert count*4==linked_meta['functions'][name]['size'], (name,'candidate coverage')
        host=temp/'host-test'
        run(['cc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-Wno-implicit-fallthrough',
             '-fsanitize=address,undefined,bounds','-O1','-g',HERE/'host_test.c','-o',host])
        host_result=run([host],env={**os.environ,'ASAN_OPTIONS':'detect_leaks=0'}).strip()
        mutants={'copy mode':('if (mode == 1)','if (mode == 2)'),
                 'scale':('scale = 0.2f;','scale = 0.3f;'),
                 'transform selection':('if (omit_transform)','if (!omit_transform)'),
                 'secondary cleanup':('case 3:','case 10:')}
        rejected=[]
        for label,(before,after) in mutants.items():
            mutated=HERE.joinpath('children.c').read_text()
            assert mutated.count(before)==1
            temp.joinpath('children.c').write_text(mutated.replace(before,after))
            temp.joinpath('host_test.c').write_text(HERE.joinpath('host_test.c').read_text())
            binary=temp/'mutant'
            run(['cc','-std=c89','-O1',temp/'host_test.c','-o',binary])
            p=subprocess.run([str(binary)],capture_output=True,text=True)
            assert p.returncode != 0, ('semantic mutant survived',label)
            rejected.append(label)
        # Compile-time generated 32-bit layout evidence, separate from candidate.
        layout=temp/'layout.c'; layout_obj=temp/'layout.o'
        checks=[('sizeof(void *)',4),('sizeof(BRecord)',104),('sizeof(BObject)',60),
          ('(u32)&((BRecord *)0)->kind',6),('(u32)&((BRecord *)0)->flags',7),
          ('(u32)&((BRecord *)0)->position',0x20),('(u32)&((BRecord *)0)->uv',0x38),
          ('(u32)&((BRecord *)0)->attached_effect',0x5C),('(u32)&((BRecord *)0)->primary',0x60),
          ('(u32)&((BRecord *)0)->secondary',0x64),('(u32)&((BObject *)0)->uv',8),
          ('(u32)&((BObject *)0)->position',0x2C)]
        layout.write_text('#include "'+str(HERE/'children.c')+'"\nu32 layout_evidence[] = {'+
                          ','.join(x[0] for x in checks)+'};\n')
        score.compile_single(layout,FLAGS,layout_obj)
        ld,secs=score._elf(layout_obj); section=next(x for x in secs if x['name']=='.data')
        values=struct.unpack_from('>'+str(len(checks))+'I',ld,section['off'])
        assert values==tuple(x[1] for x in checks)
    assert protected=={n:sha((score.ASM_DIR/n).read_bytes()) for n in manifest}
    result={'status':'COMPLETE-NONMATCH children; PARTIAL-SOURCE parent; NOT READY-MATCH',
      'base':BASE,'image_sha256':IMAGE_HASH,
      'source_sha256':sha((HERE/'children.c').read_bytes()),'flags':FLAGS+' '+score.R4300_CC,

      'compiler':{n:sha((score.IDO/n).read_bytes()) for n in ('cc','cfe','uopt','ugen','as1')},
      'artifact_hashes':{n:sha((HERE/n).read_bytes()) for n in
         ('children.c','native.py','host_test.c','verify.py','parent_regions.md','contract_audit.json','README.md')},
      'intervals':intervals,'ordinary_abi_smoke_comparisons':comparisons,
      'object':object_meta,'independent_link':linked_meta,'native_behavior':native,
      'host_result':host_result,'sanitizers':'ASan, UBSan, bounds; leak detection disabled because sandbox ptrace is incompatible',
      'rejected_semantic_mutants':rejected,
      'native_layout':dict((expr,value) for (expr,_),value in zip(checks,values)),
      'protected_inputs_unchanged':True,'new_matching_claims':[],
      'not_run':['complete E114/FCE0 reconstruction','O3 private closure compile','image/stream/full-ROM gates']}
    if args.check:
        expected=json.loads(args.output.read_text())
        assert comparable(expected)==comparable(result), 'source-bound replay changed'
    else:
        args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'status':result['status'],'native_behavior':native,'host_result':host_result},indent=2))

if __name__=='__main__': main()
