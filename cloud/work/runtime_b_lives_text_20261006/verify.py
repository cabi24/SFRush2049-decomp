#!/usr/bin/env python3
"""Rebuild and prove the complete image-B lives-text body; emit metadata only."""
import argparse
import contextlib
from dataclasses import asdict
import hashlib
import io
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib
ROOT = Path(__file__).resolve().parents[3]
PACKET = Path(__file__).resolve().parent
SOURCE = ROOT / 'cloud/matches/ovl_b/func_80393518.c'
BASE = 'cd22879d40b3de443cfde047b86e75e159b6cec6'
NAME, ADDRESS, SIZE = 'func_80393518', 0x80393518, 400
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
TARGET = '3d42d71c005a624710193d99402c216ce58b37a1c2a2fae0847a5ab832c0ed6a'
IMAGE = 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
BINDINGS = {'render_helper':0x800B65B4, 'object_create':0x800B42F0,
 'func_800B669C':0x800B669C, 'fcvt_wrapper':0x800B4360,
 'dispatch_handler':0x800B74A0, 'state_utility':0x800B71D4,
 'D_80151AD0':0x80151AD0, 'player_array':0x80152818,
 'D_803941D0':0x803941D0, 'D_80394AB0':0x80394AB0}

def digest(data): return hashlib.sha256(data).hexdigest()
def sha(path): return digest(Path(path).read_bytes())
def run(args, **kwargs):
    p = subprocess.run([str(x) for x in args], capture_output=True, text=True, **kwargs)
    assert p.returncode == 0, (args, p.returncode, p.stdout, p.stderr)
    return p

def inspect_elf(score, path, linked=False):
    data, sections = score._elf(path)
    header = struct.unpack_from('>16sHHIIIIIHHHHHH', data)
    assert header[0][:6] == b'\x7fELF\x01\x02' and header[2] == 8
    assert header[1] == (2 if linked else 1)
    allocated = []
    for i, section in enumerate(sections):
        raw = struct.unpack_from('>IIIIIIIIII', data, header[6]+i*header[11])
        if raw[2] & 2 and raw[5]:
            assert section['name'] == '.text' or (not linked and section['name'] == '.reginfo' and raw[5] == 24), section
            allocated.append(section['name'])
        if linked:
            assert section['type'] not in (4,9) or section['size'] == 0, 'linked relocations remain'
    assert '.text' in allocated
    symbols = {}
    for section in sections:
        if section['type'] != 2: continue
        names = sections[section['link']]
        for at in range(section['off'],section['off']+section['size'],16):
            no,value,size,info,other,ndx = struct.unpack_from('>IIIBBH',data,at)
            name = data[names['off']+no:].split(b'\0',1)[0].decode()
            if name: symbols[name] = (value,size,info&15,ndx)
    assert symbols[NAME] == ((ADDRESS if linked else 0),SIZE,2,score._text_index(sections))
    undefined = {n for n,v in symbols.items() if v[3] == 0}
    assert undefined == (set() if linked else set(BINDINGS))
    defined_functions = {n for n,v in symbols.items() if v[2] == 2 and v[3] not in (0,0xfff1)}
    assert defined_functions == {NAME}
    if linked:
        for name,address in BINDINGS.items():
            assert symbols[name][0] == address and symbols[name][3] == 0xfff1
    return data, sections

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
            record['sha256']=digest(data[offset:offset+size]) if typ!=8 else None
        records.append(record)
    return {'ident_sha256':digest(data[:16]),'header':list(header[:5])+list(header[6:]),'sections':records}


def comparable(receipt):
    """Exclude enumerated historical integration provenance, never proof inputs."""
    result = json.loads(json.dumps(receipt))
    assert 'portable_object_elf' in result, 'missing complete portable ELF proof'
    for key in ('input_sha256', 'target_sha256', 'object_sha256'):
        result.pop(key, None)
    # Host compiler/linker identities are recorded provenance. Their complete
    # output bytes and behavior are proved below; the pinned IDO stays bound.
    for tool in ('gcc', 'mips-linux-gnu-ld'):
        result.get('tool_sha256', {}).pop(tool, None)
    return result


def prove(reference):
    sys.path.insert(0, str(ROOT))
    from tools.cloud import score
    score.ASM_DIR = reference / 'asm/us/ovl_b'
    target_dir = score.ASM_DIR
    native = score.targets()[NAME]
    native_bytes = struct.pack('>100I', *native)
    assert len(native_bytes) == SIZE and digest(native_bytes) == TARGET
    extents = json.loads(score.verified_bytes(target_dir/'extents.json', score.target_manifest()))
    extent = next(x for x in extents['functions'] if x['name']==NAME)
    assert extent['address']=='0x80393518' and extent['size']==SIZE and extent['evidence']==['data_ref','prologue']
    addresses = score.image_symbols()
    for symbol, address in BINDINGS.items():
        assert addresses.get(symbol, score.address_named(symbol)) == address
    # Native input and save-frame evidence, without outputting target words.
    assert native[0] >> 26 == 9 and native[0] & 65535 == 0xffa0
    assert (native[0x34//4] >> 26, (native[0x34//4] >> 16)&31, native[0x34//4]&65535) == (43,4,96)
    assert native[-1]&65535 == 1
    # Read the pinned source asset in memory only; never write its bytes.
    asset = subprocess.check_output(['git','-C',str(reference),'show',BASE+':assets/us/data.bin'])
    assert digest(asset)=='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
    inflater=zlib.decompressobj(-15)
    compressed=asset[0xB6FEC4-0x283D0:]
    image=inflater.decompress(compressed)
    assert inflater.eof and len(compressed)-len(inflater.unused_data)==23639
    assert digest(image)==IMAGE and len(image)==43888
    def word(address): return struct.unpack_from('>I',image,address-0x8038A400)[0]
    assert image[ADDRESS-0x8038A400:ADDRESS-0x8038A400+SIZE]==native_bytes
    assert word(0x80393F08)==0xffffffff and word(0x80393F24)==ADDRESS
    assert 0x80393DE8+8*36==0x80393F08
    assert image[0x80394AB0-0x8038A400:0x80394AB0-0x8038A400+3]==b'%d\0'
    setup=score.targets()['func_803925D0']
    assert (setup[0x58//4]&0x3ffffff)<<2|0x80000000 == 0x800B37E8
    assert setup[0x54//4]&65535 == 0x3de8 and setup[0x5c//4]&65535 == 10
    with tempfile.TemporaryDirectory(prefix='runtime-b-lives-') as td:
        tmp=Path(td); obj=tmp/'candidate.o'
        score.compile_single(SOURCE,FLAGS,obj)
        with contextlib.redirect_stdout(io.StringIO()): comparison=score.compare(obj,NAME)
        assert comparison.accepted()
        data,sections=inspect_elf(score,obj); text=sections[score._text_index(sections)]
        fingerprint = portable_elf(data)
        moved_dir = tmp / 'different_source_path'; moved_dir.mkdir()
        moved = moved_dir / SOURCE.name; moved.write_bytes(SOURCE.read_bytes())
        moved_object = moved_dir / 'candidate.o'; score.compile_single(moved, FLAGS, moved_object)
        assert portable_elf(moved_object.read_bytes()) == fingerprint, 'source path changed semantic ELF'
        assert digest(moved_object.read_bytes()) != digest(data), 'debug path control ineffective'
        semantic_controls = {}
        for section in sections:
            if section['name'] not in ('.text', '.rel.text', '.reginfo', '.options', '.symtab', '.strtab') or not section['size']:
                continue
            changed = bytearray(data); changed[section['off']] ^= 1
            assert portable_elf(changed) != fingerprint, section['name']
            semantic_controls[section['name']] = 'rejected'
        changed = bytearray(data); changed[39] ^= 1
        assert portable_elf(changed) != fingerprint, 'ELF ABI mutation accepted'
        semantic_controls['ELF_ABI_flags'] = 'rejected'

        assert text['size']==SIZE and score.symbols(obj)=={NAME:0}
        for section in sections:
            if section['name'] in ['.data','.rodata','.rdata','.lit4','.lit8','.sdata','.bss','.sbss']:
                assert section['size']==0, section
        resolved,masks,unresolved,unverified,errors=score.relocate(obj,score.text_words(obj),0,SIZE,addresses)
        assert not any((masks,unresolved,unverified,errors)) and resolved==native
        relocs=[]
        for sec in sections:
            if sec['type']!=9: continue
            assert sec['info']==score._text_index(sections)
            syms=score._symbol_table(data,sections,sec['link'])
            for at in range(sec['off'],sec['off']+sec['size'],8):
                offset,info=struct.unpack_from('>II',data,at)
                assert 0<=offset<SIZE and offset%4==0 and info&255 in [4,5,6]
                relocs.append({'offset':offset,'type':info&255,'symbol':syms[info>>8]['name']})
        assert len(relocs)==20 and set(r['symbol'] for r in relocs)==set(BINDINGS)
        script=tmp/'native.ld'
        script.write_text('\n'.join('%s = 0x%08x;'%(k,v) for k,v in BINDINGS.items())+
            '\nSECTIONS { .text 0x80393518 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n')
        linked=tmp/'linked.elf'; run(['mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj])
        ld,ls=inspect_elf(score,linked,True); lt=ls[score._text_index(ls)]
        assert lt['size']==SIZE
        linked_raw=ld[lt['off']:lt['off']+lt['size']]
        assert linked_raw==native_bytes
        # Verify actual placement through the ELF section header.
        h=struct.unpack_from('>16sHHIIIIIHHHHHH',ld)
        section_index=score._text_index(ls)
        sh=struct.unpack_from('>IIIIIIIIII',ld,h[6]+section_index*h[11])
        assert sh[3]==ADDRESS
        flags=['gcc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-Wno-unused-parameter','-O2',
            '-fsanitize=address,undefined,bounds','-fno-sanitize-recover=all']
        env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0')
        def host(source,output):
            run(flags+['-DCANDIDATE_PATH="'+str(source)+'"',PACKET/'host_test.c','-o',output])
            return subprocess.run([str(output)],capture_output=True,text=True,env=env)
        p=host(SOURCE,tmp/'host'); assert p.returncode==0 and p.stdout=='1796 host fixtures passed\n', (p.stdout,p.stderr)
        mutants={'wrong_kind_gate':('kind != 8','kind == 8'),
            'exclude_zero_lives':('lives >= 0','lives > 0'),
            'wrong_shadow_position':('x + 1, y + 1','x + 2, y + 1'),
            'wrong_callback_return':('return 1;','return 0;')}
        rejected=[]
        for name,(old,new) in mutants.items():
            assert SOURCE.read_text().count(old)==1
            src=tmp/(name+'.c'); src.write_text(SOURCE.read_text().replace(old,new))
            p=host(src,tmp/name); assert p.returncode!=0, name
            rejected.append(name)
        original=tmp/'buffer16.c'; original.write_text(SOURCE.read_text().replace('char text[12]','char text[16]'))
        score.compile_single(original,FLAGS,tmp/'buffer16.o')
        with contextlib.redirect_stdout(io.StringIO()): control=score.compare(tmp/'buffer16.o',NAME)
        assert control.differing==4 and not control.accepted() and not control.extra_words
        return {'status':'MATCH','base':BASE,'image':'B','image_sha256':IMAGE,'address':hex(ADDRESS),
            'end_exclusive':hex(ADDRESS+SIZE),'native_bytes':SIZE,'native_sha256':TARGET,
            'source_sha256':sha(SOURCE),'flags':FLAGS+' -Wab,-r4300_mul','accepted_or_coverage_bytes':0,
            'comparison':asdict(comparison),'object_sha256':sha(obj),'portable_object_elf':fingerprint,
            'portability_controls':{'different_source_path_same_complete_elf':True,'semantic_mutations':semantic_controls},'text_bytes':text['size'],
            'padding_bytes':0,'owned_data_bytes':0,'relocations':relocs,
            'bindings':{k:hex(v) for k,v in sorted(BINDINGS.items())},'whole_object_gnu_agrees':True,
            'linked_text_sha256':digest(linked_raw),'gnu_placement':hex(ADDRESS),
            'callback_evidence':{'descriptor': '0x80393f08','callback_pointer':'0x80393f24',
                'table':'0x80393de8','record_index':8,'record_stride':36,'record_count':10,
                'registrar':'0x803925d0','mode':6,'sentinel_texture_name':-1,
                'active_frame_reachability_proved':False},
            'data':{'format_sha256':digest(b'%d\0'),'position_table_sha256':digest(image[0x803941D0-0x8038A400:0x80394210-0x8038A400])},
            'behavior':{'host_c89_asan_ubsan_bounds_fixtures':1796,'native_execution_by_this_script':False,
                'compiled_mutants_rejected':rejected,'domain':'count <= 0 skips; positive count 1..4 with valid player/table storage; signed-byte lives; stable image B residency; no concurrency'},
            'controls':{'buffer16':asdict(control)},
            'tool_sha256':{k:sha(v) for k,v in [(n,score.ido(n)) for n in ['cc','cfe','uopt','ugen','as1']]+[(n,shutil.which(n)) for n in ['mips-linux-gnu-ld','gcc']]},


            'packet_sha256':{n:sha(PACKET/n) for n in ['verify.py','host_test.c','README.md']}}

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--reference-root',type=Path,default=ROOT)
    p.add_argument('--check',action='store_true')
    a=p.parse_args(); result=prove(a.reference_root.resolve()); output=PACKET/'verification.json'
    if a.check:
        assert comparable(result)==comparable(json.loads(output.read_text())), 'frozen verification differs'
        print('400-byte strict MATCH, GNU placement/equality and 1796 host fixtures verified')
    else:
        output.write_text(json.dumps(result,indent=2)+'\n')
        print('Wrote source-bound 400-byte strict MATCH verification')
