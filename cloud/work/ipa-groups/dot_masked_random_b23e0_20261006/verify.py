#!/usr/bin/env python3
"""Reproduce complete ELF/GNU/source/behavior evidence for masked Random."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score,owndata
from tools.conveyor.pipeline import blob_group
spec=importlib.util.spec_from_file_location('masked_random_semantics',HERE/'verify_semantics.py')
sem=importlib.util.module_from_spec(spec);spec.loader.exec_module(sem)
FN='func_800B23E0'
NAMES=['func_8008B2B4','func_8008B2E4',FN]
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'

def sha(data):return hashlib.sha256(data).hexdigest()
def packed(words):return struct.pack('>%dI'%len(words),*words)

def inspect(obj,name):
    data,sections=score._elf(obj);ti=score._text_index(sections)
    syms=[s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i)]
    fn,=[s for s in syms if s['name']==name and s['type']==2 and s['section']==ti]
    words=score.text_words(obj);start,end=fn['value'],fn['value']+fn['size']
    assert start%4==end%4==0 and end<=len(words)*4
    resolved,masks,unresolved,unverified,errors=score.relocate(obj,words,start,end,score.image_symbols())
    body=resolved[start//4:end//4];want=score.targets()[name]
    comparison=score.compare(obj,name,show=0)
    full_bad=sum(a!=b for a,b in zip(body,want))+abs(len(body)-len(want))
    return dict(asdict(comparison),canonical_verdict=comparison.summary(),symbol_bytes=fn['size'],symbol_offset=start,
                native_bytes=len(want)*4,full_differing_positions=full_bad,full_extent_equal=body==want,
                all_references_resolved=not(masks or unresolved or unverified or errors),body_sha256=sha(packed(body)))

def image_window():
    # Always authenticate current data, even if the score module's cache is warm.
    image=owndata.ImageData.from_artifact(score.ASM_DIR.with_name('blob_data'))
    assert image is not None,'authenticated data artifact required'
    window=image.read(sem.TABLE,128)
    assert window is not None and len(window)==128
    return window

def gnu_body(directory,obj,name,report,addresses):
    # Link the unmodified full ELF with this complete body's original address.
    # Independent per-body placement is not original contiguous TU placement.
    offset=report['symbol_offset'];size=report['symbol_bytes']
    base=addresses[name]-offset
    script=directory/(name+'.ld')
    script.write_text('SECTIONS { .text 0x%X : SUBALIGN(4) { *(.text) } }\n'%base+
                      'D_8011735C = 0x8011735C;\nD_80123418 = 0x80123418;\n')
    elf=directory/(name+'.elf');binary=directory/(name+'.bin')
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True)
    readelf=subprocess.run(['mips-linux-gnu-readelf','-Ws',str(elf)],check=True,capture_output=True,text=True).stdout
    nm=subprocess.run(['mips-linux-gnu-nm','-S','--defined-only',str(elf)],check=True,capture_output=True,text=True).stdout
    assert re.search(r'%08x\s+%d\s+FUNC.*\b%s\b'%(addresses[name],size,name),readelf)
    assert re.search(r'%08x %08x T %s\b'%(addresses[name],size,name),nm)
    subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(elf),str(binary)],check=True,capture_output=True)
    body=binary.read_bytes()[offset:offset+size]
    assert len(body)==size and body==packed(score.targets()[name])
    return body

def verify(directory,behavior=True):
    directory.mkdir(parents=True,exist_ok=True)
    assert (HERE/'rand.c').read_bytes()==(ROOT/'src/blob/func_8008B2B4.c').read_bytes()
    native=score.targets();addresses=score.image_symbols();window=image_window()
    assert {k:addresses[k] for k in NAMES+['D_8011735C','D_80123418']}=={
        'func_8008B2B4':0x8008b2b4,'func_8008B2E4':0x8008b2e4,FN:0x800b23e0,'D_8011735C':sem.SEED,'D_80123418':sem.TABLE}
    locks=json.loads((ROOT/'blob_matched.lock.json').read_text())
    # Before promotion FN must be unclaimed; once spliced, only as this packet's source.
    if FN in locks:
        assert locks[FN]['source']=='src/blob/'+FN+'.c'
    else:
        accepted_addresses={addresses.get(n) for n in locks}
        assert addresses[FN] not in accepted_addresses
    obj=directory/'candidate.o';score.compile_group(HERE,obj)
    bodies={n:inspect(obj,n) for n in NAMES}
    assert [bodies[n]['symbol_bytes'] for n in NAMES]==[48,72,268]
    assert [bodies[n]['symbol_offset'] for n in NAMES]==[0,48,120]
    assert all(b['full_extent_equal'] and b['all_references_resolved'] and b['canonical_verdict']=='MATCH' for b in bodies.values())
    data,sections=score._elf(obj);ti=score._text_index(sections);text=sections[ti]
    assert text['size']==400 and data[text['off']+388:text['off']+400]==bytes(12)
    assert all(s['size']==0 for s in sections if s['name'] in ('.data','.rodata','.rdata','.bss','.sdata','.sbss','.lit4','.lit8'))
    relocs=[]
    for sec in sections:
        if sec['type']==9 and sec['info']==ti:
            syms=score._symbol_table(data,sections,sec['link'])
            for pos in range(sec['off'],sec['off']+sec['size'],8):
                where,info=struct.unpack_from('>II',data,pos)
                relocs.append({'offset':where,'type':info&255,'symbol':syms[info>>8]['name']})
    assert all(r['type'] in (5,6) and r['symbol'] in ('D_8011735C','D_80123418') for r in relocs)
    assert len(relocs)==10
    linked={n:gnu_body(directory,obj,n,bodies[n],addresses) for n in NAMES}
    extents={n:{'vaddr':addresses[n],'size':len(native[n])*4} for n in NAMES}
    slices,index=blob_group.member_slices(obj,NAMES,extents)
    relocated=blob_group.relocate(obj,slices,index,addresses,members=NAMES)
    assert all(relocated[n]==packed(native[n]) for n in NAMES)
    callword=0x0c000000|((addresses[FN]>>2)&0x3ffffff)
    callers=[n for n,w in native.items() if callword in w]
    assert callers==[]
    result={'status':'MATCH_CANDIDATE_PENDING_INDEPENDENT_REVIEW','claim':[FN],'range':['0x800B23E0','0x800B24EC'],
      'new_candidate_bytes':268,'accepted_byte_gain':0,'base_revision':json.loads((HERE/'claim.json').read_text())['base_revision'],
      'flags':FLAGS,'assembler_erratum_flag':score.R4300_CC,'bodies':bodies,'relocations':relocs,'gnu_complete_bodies_equal':list(NAMES),
      'production_reader_complete_bodies_equal':list(NAMES),'zero_section_alignment_outside_functions':12,'no_owned_literals_or_data':True,
      'native_direct_callers':callers,'referenced_symbols':{n:hex(addresses[n]) for n in NAMES+['D_8011735C','D_80123418']},
      'selected_native_body_sha256':{n:sha(packed(native[n])) for n in NAMES},
      'table_window':{'start':'0x80123418','bytes':128,'sha256':sha(window),'nonzero_rows':sum(bool(x) for x in struct.unpack('>32I',window)),
                      'scope':'observed 128-byte protected window; not a recovered complete table capacity'},
      'target_manifest_historical_sha256':sha((score.ASM_DIR/'SHA256SUMS').read_bytes()),
      'data_manifest_historical_sha256':sha((score.ASM_DIR.with_name('blob_data')/'SHA256SUMS').read_bytes())}
    source=(HERE/'selector.c').read_text();variants={
      'pre_cached_table':source.replace('    u8 choice;','    u32 mask = D_80123418[index];\n    u8 choice;').replace('D_80123418[index] &','mask &'),
      'without_range_mask':(HERE/'range.c').read_text().replace('func_8008B2B4() & 0x07FFF','func_8008B2B4()')}
    result['causal_controls']={}
    for label,content in variants.items():
        group=directory/label;group.mkdir()
        for f in ('group.json','rand.c','range.c','selector.c'):shutil.copyfile(HERE/f,group/f)
        (group/('range.c' if label=='without_range_mask' else 'selector.c')).write_text(content)
        other=group/'candidate.o';score.compile_group(group,other)
        result['causal_controls'][label]={n:inspect(other,n) for n in NAMES}
    assert result['causal_controls']['pre_cached_table'][FN]['differing']==14
    # The removed donor mask is causal only if the complete object proves it.
    result['causal_controls']['without_range_mask_effect']=result['causal_controls']['without_range_mask'][FN]['full_differing_positions']
    old=directory/'archived.c'
    old.write_bytes(subprocess.run(['git','show','cd22879d:cloud/work/frontier/dot_fresh_masked_rng/best.c'],cwd=ROOT,check=True,capture_output=True).stdout)
    score.compile_single(old,score.DEFAULT_FLAGS,directory/'archived.o')
    result['archived_source_sha256']=sha(old.read_bytes());result['archived_control']=inspect(directory/'archived.o',FN)
    assert result['archived_control']['differing']==13
    if behavior:result['behavior']=sem.verify(directory,native[FN],list(struct.unpack('>67I',linked[FN])),list(struct.unpack('>32I',window)))
    result['compiler_sha256']={n:sha(Path(score.ido(n)).read_bytes()) for n in ('cc','cfe','uld','usplit','umerge','uopt','ugen','as1')}
    result['source_sha256']={n:sha((HERE/n).read_bytes()) for n in ('rand.c','range.c','selector.c','group.json','claim.json','semantic_test.c','verify.py','verify_semantics.py')}
    result['tools_sha256']={n:sha((ROOT/n).read_bytes()) for n in ('tools/cloud/score.py','tools/cloud/owndata.py','tools/conveyor/pipeline/blob_group.py')}
    result['limitations']=['Matching candidate only; no source-image, shadow, compression, ROM or gameplay gates.',
      'Original whole translation unit and inlined caller ancestry not recovered; GNU placements are independently per body.',
      'Range context is previously reviewed candidate B2E4, not accepted production code; accepted rand is unchanged.',
      'Stable nonvolatile seed/table and single-threaded execution assumed; no concurrent mutation contract.',
      'Zero mask does not terminate; bounded native prefix is checked, not host termination.',
      'C89 host uses -fwrapv for inherited rand signed overflow; native FP conversion fallback remains byte-compared but unreachable in valid range.']
    return result

def portable(result):
    return {k:v for k,v in result.items() if k not in ('target_manifest_historical_sha256','data_manifest_historical_sha256')}

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path);parser.add_argument('--skip-behavior',action='store_true');args=parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='masked-random-proof-') as tmp:result=verify(Path(tmp),not args.skip_behavior)
    if args.output:args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ('status','bodies','behavior') if k in result},indent=2))
