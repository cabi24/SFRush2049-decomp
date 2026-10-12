#!/usr/bin/env python3
"""Full-extent NONMATCH research replay; no ROM/native dumps are persisted."""
import argparse
from dataclasses import asdict
import hashlib
import json
import re
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import zlib
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers
NAME='func_803A5358'
ENTRY,SIZE=0x803A5358,768
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared'
SOURCE=HERE/'candidate.c'

def require(ok,message):
    if not ok:raise ValueError(message)
def sha(data):return hashlib.sha256(data).hexdigest()
def packed(words):return struct.pack('>%dI'%len(words),*words)

def elf(path):
    """Independent ELF32 big-endian reader, including complete symbol sizes."""
    data = path.read_bytes()
    require(data[:6] == b'\x7fELF\x01\x02', 'not ELF32 big-endian')
    require(struct.unpack_from('>H', data, 18)[0] == 8, 'not MIPS')
    at = struct.unpack_from('>I', data, 32)[0]
    stride, count, ni = struct.unpack_from('>HHH', data, 46)
    require(stride == 40, 'section size')
    rows = [struct.unpack_from('>10I', data, at+i*stride) for i in range(count)]
    nr = rows[ni]; names = data[nr[4]:nr[4]+nr[5]]
    sections, symbols, tables, relocs = {}, {}, {}, []
    for index, row in enumerate(rows):
        name = names[row[0]:].split(b'\0')[0].decode()
        raw = data[row[4]:row[4]+row[5]]
        sections[name] = (index, row, raw)
        if row[1] == 2:
            strings_row = rows[row[6]]
            strings = data[strings_row[4]:strings_row[4]+strings_row[5]]
            table = []
            for offset in range(0, len(raw), 16):
                no, value, size, info, other, section = struct.unpack_from('>IIIBBH', raw, offset)
                name = strings[no:].split(b'\0')[0].decode(); table.append(name)
                if name:
                    symbols[name] = (value, size, info & 15, section)
            tables[index] = table
    for row in rows:
        if row[1] in (4, 9):
            require(row[1] == 9, 'unsupported RELA')
            for offset in range(row[4], row[4]+row[5], 8):
                loc, info = struct.unpack_from('>II', data, offset)
                relocs.append((loc, info & 255, tables[row[6]][info >> 8], row[7]))
    return sections, symbols, relocs



def inspect(path,linked=False):
    sections,symbols,relocs=elf(path);ti,row,raw=sections['.text']
    funcs={n:s for n,s in symbols.items() if s[2]==2 and s[3]==ti}
    require(funcs=={NAME:(ENTRY if linked else 0,SIZE,2,ti)},'complete function extent')
    require(len(raw)==SIZE,'unexpected text suffix')
    for name,(_index,sec,_data) in sections.items():
        if sec[2]&2 and name not in ('.text','.options','.reginfo'):
            require(sec[5]==0,'unexpected owned data '+name)
    require(not relocs if linked else len(relocs)==29,'relocation count')
    require(all(off<SIZE and off%4==0 and typ in (4,5,6) and dest==ti for off,typ,_n,dest in relocs),'unsupported relocation')
    return raw,relocs


def inputs():
    manifest=score.target_manifest()
    meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))
    row=next(e for e in meta['functions'] if e['name']==NAME)
    require(row['size']==SIZE and int(row['address'],16)==ENTRY,'target extent')
    asset=(ROOT/'assets/us/data.bin').read_bytes()
    require(sha(asset)=='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8','asset identity')
    require(struct.unpack_from('>I',asset,0x3850)[0]==0xB5C534,'image pointer')
    image=zlib.decompress(asset[0xB5C534-0x283D0:],-15)
    require(len(image)==194128 and sha(image)=='0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667','image identity')
    require(image[ENTRY-0x8038A400:ENTRY-0x8038A400+SIZE]==packed(score.targets()[NAME]),'target/image identity')
    require(image[0x803B8668-0x8038A400:0x803B8668-0x8038A400+3]==b'%d\0','numeric format changed')
    bound=max(len(str(n))+1 for n in range(-128,128))
    require(bound==5 and bound<=24,'signed-byte text bound')
    source=SOURCE.read_text()
    symbols=score.image_symbols()
    addresses={name:symbols.get(name,score.address_named(name)) for name in set(re.findall(r'\b[A-Za-z_]\w*\b',source))}
    anchors={name:hex(address) for name,address in sorted(addresses.items()) if address is not None}
    require(source.count('fcvt_wrapper(value,D_803B8668,D_801613AB);')==1 and 'char text[24];' in source,'write set changed')
    return dict(protocol_version=2,claims=[],status='NONMATCH',
        sources={n:sha((HERE/n).read_bytes()) for n in ['candidate.c','verify.py']},
        test_sha256=sha((ROOT/'tests/cloud/test_runtime_a_settings_scroll.py').read_bytes()),
        referenced_symbols=anchors,
        target=dict(population='A',address=hex(ENTRY),size=SIZE,words_sha256=sha(packed(score.targets()[NAME]))),
        flags=FLAGS,compiler='stock IDO 5.3 overlay single; no static ROM TU rebuild',
        format=dict(address='0x803B8668',sha256=sha(b'%d\0'),signed_input_range=[-128,127],maximum_write_with_nul=bound,buffer_bytes=24),
        asset_sha256=sha(asset),image_sha256=sha(image))


def replay(deep=False):
    old=score.ASM_DIR;score.ASM_DIR=ROOT/'asm/us/ovl_a'
    try:
        binding=inputs();score.ido('cc');require(compilers.identify_dir(score.IDO)=='ido5.3','stock IDO 5.3 required')
        with tempfile.TemporaryDirectory(prefix='settings-scroll-') as tmp:
            work=Path(tmp);obj=work/'candidate.o';score.compile_single(SOURCE,FLAGS,obj)
            result=score.compare(obj,NAME,show=0)
            require(result.differing==58 and result.total==192 and not result.accepted(),'NONMATCH classification changed')
            require(not result.extra_words and not result.unresolved and not result.unverified and not result.errors,'uncertain result')
            body,relocs=inspect(obj)
            require(all(n in binding['referenced_symbols'] for _o,_t,n,_d in relocs),'unbound relocation anchor')
            require(struct.unpack_from('>I',body)[0]&65535==65536-128,'candidate frame')
            control=work/'signed.c';control.write_text(SOURCE.read_text().replace('(u32)D_8017A4E0.setting_labels','D_8017A4E0.setting_labels'))
            control_obj=work/'signed.o';score.compile_single(control,FLAGS,control_obj)
            base=score.compare(control_obj,NAME,show=0)
            baseline_sections,baseline_symbols,_=elf(control_obj)
            require(baseline_symbols[NAME][1]==760,'baseline true extent')
            require(base.differing==128 and not base.extra_words and not base.unresolved and not base.unverified and not base.errors,'unsigned-index causal control')
            details=dict(comparison=asdict(result),baseline_comparison=asdict(base),baseline_function_bytes=760,complete_function_bytes=SIZE,
                emitted_text_bytes=len(body),relocations=len(relocs),owned_bytes=0,candidate_frame=128,native_frame=136,
                new_exact_bytes=0,new_rom_verified_bytes=0,
                compiler_binary_sha256={n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uopt','ugen','as1']})
            if deep:
                addresses=score.image_symbols();names={n for _o,_t,n,_d in relocs}
                anchors={n:addresses.get(n,score.address_named(n)) for n in names}
                require(all(a is not None for a in anchors.values()),'unknown GNU anchor')
                ld=work/'candidate.ld';ld.write_text('SECTIONS { .text 0x803A5358 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'+''.join('%s = 0x%X;\n'%(n,a) for n,a in sorted(anchors.items())))
                linked=work/'candidate.elf';subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(ld),'-o',str(linked),str(obj)],check=True,capture_output=True)
                raw,_=inspect(linked,True);words=struct.unpack('>192I',raw);native=score.targets()[NAME]
                require(sum(a!=b for a,b in zip(words,native))==58,'GNU residual differs')
                details['gnu']=dict(full_extent_bytes=SIZE,differing_words=58,remaining_relocations=0,linked_words_sha256=sha(raw))
            return dict(inputs=binding,verification=details)
    finally:score.ASM_DIR=old


def verify(deep=False):
    expected=json.loads((HERE/'verification.json').read_text());actual=replay(deep)
    require(actual['inputs']==expected['inputs'],'input binding changed')
    wanted=dict(expected['verification'])
    if not deep:wanted.pop('gnu',None)
    require(actual['verification']==wanted,'verification receipt changed')
    return actual

if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--deep',action='store_true');parser.add_argument('--record',action='store_true');args=parser.parse_args()
    if args.record:(HERE/'verification.json').write_text(json.dumps(replay(args.deep),indent=2,sort_keys=True)+'\n')
    else:
        verify(args.deep);print('PASS: complete NONMATCH research, 58/192 differing, zero exact/ROM bytes')
