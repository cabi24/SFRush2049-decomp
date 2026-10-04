#!/usr/bin/env python3
"""Unchanged-scorer evidence and independent GNU-linker whole-object proof.
Temporary scripts are research placement assumptions, not production ownership.
No native/object/table bytes are persisted by this verifier.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
P = Path(__file__).resolve().parent
ROOT = P.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
NAME = 'func_8001B9F8'
BASE, TABLE = 0x8001B9F8, 0x8002D8D0
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
SOURCE = P/'nonmatch'/f'{NAME}.c'

def digest(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def unpack(raw): return list(struct.unpack('>'+str(len(raw)//4)+'I',raw))
def sections(path):
    raw, secs = score._elf(path)
    return raw, secs, {s['name']:raw[s['off']:s['off']+s['size']] for s in secs}
def function_size(obj):
    raw, secs = score._elf(obj)
    syms=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(raw,secs,i) if s['type']==2 and s['section']==score._text_index(secs)]
    assert len(syms)==1 and syms[0]['name']==NAME
    return syms[0]['size']
def compile_link(source, flags, directory):
    obj, elf = directory/'candidate.o', directory/'candidate.elf'
    score.compile_single(source, flags, obj)
    script = directory/'research-placement.ld'
    script.write_text('SECTIONS { .text 0x8001B9F8 : SUBALIGN(4) { *(.text) } .rodata 0x8002D8D0 : SUBALIGN(4) { *(.rodata) } /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.mdebug) *(.comment) *(.gnu.attributes) } }\nfunc_8001E930 = 0x8001E930;\nD_8004F300 = 0x8004F300;\n')
    ld=Path(os.environ['MIPS_OBJDUMP']).with_name('mips-linux-gnu-ld')
    subprocess.run([str(ld),'-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True)
    raw,secs,byname=sections(elf)
    assert not any(s['type'] in (4,9) and s['size'] for s in secs), 'unresolved relocation section'
    # Check ELF's actual section addresses, independently of the requested script.
    shoff=struct.unpack_from('>I',raw,32)[0];shsize=struct.unpack_from('>H',raw,46)[0]
    addr={s['name']:struct.unpack_from('>I',raw,shoff+i*shsize+12)[0] for i,s in enumerate(secs)}
    assert addr['.text']==BASE and addr['.rodata']==TABLE
    n=function_size(obj)
    assert not any(byname['.text'][n:]), 'nonzero section tail'
    assert len(byname['.rodata'])==32 and not any(byname['.rodata'][24:]), 'unexpected local data'
    oraw,osecs,oby=sections(obj)
    rels=[]
    for sec in osecs:
        if sec['type']!=9: continue
        target=osecs[sec['info']]['name'];syms=score._symbol_table(oraw,osecs,sec['link'])
        for k in range(sec['size']//8):
            offset,info=struct.unpack_from('>II',oraw,sec['off']+8*k)
            rels.append({'section':target,'offset':offset,'type':info&255,'symbol':syms[info>>8]['name']})
    table_rels=[r for r in rels if r['section']=='.rodata']
    assert [(r['offset'],r['type'],r['symbol']) for r in table_rels]==[(i*4,2,'.text') for i in range(6)]
    assert {r['symbol'] for r in rels if r['section']=='.text'}=={'.rodata','D_8004F300','func_8001E930'}
    return obj, byname['.text'][:n], byname['.rodata'][:24], rels

def one(source,flags):
    with tempfile.TemporaryDirectory(prefix='voice-proof-') as tmp:
        obj,body,table,rels=compile_link(source,flags,Path(tmp))
        c=score.compare(obj,NAME,show=0)
        want=score.targets()[NAME];got=unpack(body)
        receipt=json.loads((P/'table_receipt.json').read_text())
        assert hashlib.sha256(struct.pack('>'+str(len(want))+'I',*want)).hexdigest()==receipt['canonical_function_sha256']
        destinations=[int(x['target'],16) for x in receipt['selector_to_target']]
        tablegot=unpack(table)
        bad=[i*4 for i in range(max(len(want),len(got))) if (want[i] if i<len(want) else None)!=(got[i] if i<len(got) else None)]
        return {'source':str(source.relative_to(ROOT)),'source_sha256':digest(source),'flags':flags,'effective_flags':flags+' -Wab,-r4300_mul',
                'elf_function_bytes':len(body),'canonical_function_bytes':len(want)*4,'canonical_function_sha256':hashlib.sha256(struct.pack('>'+str(len(want))+'I',*want)).hexdigest(),
                'unchanged_scorer':{'differing_words':c.differing,'total_words':c.total,'extra_words':c.extra_words,'unresolved':c.unresolved,'unverified':c.unverified,'errors':c.errors,'accepted':c.accepted()},
                'independent_full_relocation':{'differing_words':len(bad),'differing_offsets':bad,'all_words_equal':not bad,'relocations_resolved':len(rels),'relocations':rels,
                 'table_entry_count':6,'table_matching_entries':sum(a==b for a,b in zip(tablegot,destinations)),'table_sha256':hashlib.sha256(table).hexdigest(),'table_equals_receipt':tablegot==destinations,
                 'table_all_targets_inside_candidate':all(BASE<=a<BASE+len(body) and a%4==0 for a in tablegot),'production_storage_ownership':'unassigned; temporary research placement only'}}

def run():
    score.ASM_DIR=ROOT/'asm/us/boot_tail'
    manifest=subprocess.run(['sha256sum','-c','SHA256SUMS'],cwd=score.ASM_DIR,check=True,capture_output=True,text=True).stdout.splitlines()
    inv=json.loads((ROOT/'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
    ext=json.loads((score.ASM_DIR/'extents.json').read_text())['functions']
    assert {x['address']:x['size'] for x in inv}=={x['address']:x['size'] for x in ext}
    with tempfile.TemporaryDirectory(prefix='voice-getter-') as tmp:
        obj=Path(tmp)/'getter.o';score.compile_single(ROOT/'cloud/matches/boot_tail/func_80010A00.c',FLAGS,obj)
        assert score.compare(obj,'func_80010A00',show=0).accepted()
    final=[one(SOURCE,FLAGS),one(SOURCE,FLAGS.replace('-O2','-O1'))]
    controls=[one(s,FLAGS) for s in sorted((P/'controls').glob('*.c'))]
    return {'result':'COMPLETE-NONMATCH','base_commit':'cf4b9c619c72e83aa5da2e8b5c110765f9b03693','manifest':manifest,'extent_census_count':len(inv),'getter':'strict MATCH','source_forms_tested':len(controls),
            'scorer_sha256':digest(ROOT/'tools/cloud/score.py'),'table_receipt_sha256':digest(P/'table_receipt.json'),
            'compiler_files_sha256':{p.name:digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()},'final':final,'controls':controls}

if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path);args=parser.parse_args()
    result=json.dumps(run(),indent=2)+'\n'
    if args.output:args.output.write_text(result)
    else:print(result,end='')
