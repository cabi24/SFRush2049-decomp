#!/usr/bin/env python3
"""Rebuild the bounded setter research; record counts and hashes, never native bytes."""
from dataclasses import asdict
import hashlib
import json
import os
from pathlib import Path
import struct
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor.pipeline import blob_group

NAMES = ['audio_bus_mix','voice_stop_2','format_string_parse','func_800B4720','func_800B4728','func_800B4730']

def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()
def packed(words): return struct.pack('>%dI' % len(words), *words)
def signed16(i): return i-65536 if i&32768 else i

def definition(source, name):
    start=source.index('u32 '+name+'(u8 *data,u32 size)'); brace=source.index('{',start); level=1; i=brace+1
    while level:
        level += (source[i]=='{')-(source[i]=='}'); i+=1
    return source[start:i]

def native_facts():
    targets=score.targets(); addresses=score.image_symbols(); result={}
    for name in NAMES:
        words=targets[name]
        callers=[]
        for caller, body in targets.items():
            for i,w in enumerate(body):
                if w>>26==3 and (0x80000000|((w&0x3ffffff)<<2))==addresses[name]:
                    callers.append({'function':caller,'offset':i*4})
        loads=[]; stores=[]
        for i,w in enumerate(words):
            op=w>>26;rs=(w>>21)&31
            if rs==29 and op in (32,33,35,36,37):loads.append({'offset':i*4,'stack_offset':signed16(w&65535)})
            if rs==29 and op in (40,41,43):stores.append({'offset':i*4,'stack_offset':signed16(w&65535)})
        result[name]={'address':'0x%08X'%addresses[name],'bytes':len(words)*4,
                      'target_sha256':hashlib.sha256(packed(words)).hexdigest(),
                      'native_callsites':callers,'stack_loads':loads,'stack_stores':stores}
    return result

def verify():
    targets=score.targets(); addresses=score.image_symbols(); spec=json.loads((HERE/'group.json').read_text())
    assert spec['claims']==[] and spec['keep']==['audio_bus_mix']
    accepted=ROOT/'src/blob/groups/codex_hash_a80/group.c'
    assert definition((HERE/'group.c').read_text(),'format_string_parse')==definition(accepted.read_text(),'format_string_parse')
    image=owndata.ImageData.from_artifact(ROOT/'asm/us/blob_data'); assert image is not None
    result={'status':'NONMATCH_ABI_UNPROVED','claims':[], 'coverage_delta_functions':0,'coverage_delta_bytes':0,
            'base_commit':'55ddfc6b53d96d4c5dcaaa8891dedafb037b3994',
            'flags':spec['flags'],'source_sha256':sha(HERE/'group.c'),'manifest_sha256':sha(HERE/'group.json'),
            'accepted_context_sha256':sha(accepted),'native':native_facts(), 'comparisons':{},
            'protected_inputs':{str(p.relative_to(ROOT)):sha(p) for p in [ROOT/'asm/us/blob/SHA256SUMS',ROOT/'asm/us/blob/symbols.json',ROOT/'asm/us/blob_data/SHA256SUMS',ROOT/'blob_matched.lock.json']},
            'compiler_binaries_sha256':{n:sha(score.ido(n)) for n in ('cc','uld','usplit','umerge','uopt','ugen','as1')}}
    with tempfile.TemporaryDirectory(prefix='audio-setter-proof-') as tmp:
        obj=Path(tmp)/'group.o';score.compile_group(HERE,obj)
        data, sections=score._elf(obj); text=score._text_index(sections)
        symbols={s['name']:s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i) if s['type']==2 and s['section']==text}
        # Materialize only authenticated repository targets and own-data runs for
        # the existing protected relocation routine. Unknown bytes remain unused.
        base=min(addresses[n] for n in targets)
        end=max([addresses[n]+len(w)*4 for n,w in targets.items()]+[a+len(b) for a,b in image.runs])
        native_image=bytearray(end-base)
        for n,w in targets.items():
            off=addresses[n]-base; native_image[off:off+len(w)*4]=packed(w)
        for a,b in image.runs: native_image[a-base:a-base+len(b)]=b
        slices={n:(symbols[n]['value'],addresses[n],len(targets[n])*4) for n in NAMES}
        proof_names=[n for n in NAMES if n!='audio_bus_mix']
        # Exact ELF extent is independently required: no neighboring stub or
        # zero-filled section alignment can make a short body pass.
        for n in proof_names: assert symbols[n]['size']==len(targets[n])*4
        relocated=blob_group.relocate(obj,slices,str(text),addresses,members=proof_names,image=(bytes(native_image),base))
        for n in NAMES:
            comp=score.compare(obj,n,show=0); row=asdict(comp)
            row['elf_symbol_bytes']=symbols[n]['size']; row['target_bytes']=len(targets[n])*4
            row['exact_extent']=row['elf_symbol_bytes']==row['target_bytes']
            row['canonical_summary']=comp.summary()
            if n in proof_names:
                own=owndata.verify(obj,n,targets[n],address=addresses[n],image=image,addresses=addresses.get)
                assert not own.failures and not own.unverified
                row['own_data']={'verified_reference_sites':len(own.sites),'placements':own.placements,'failures':[],'unverified':[]}
                row['full_relocated_equal']=relocated[n]==packed(targets[n])
                row['relocated_sha256']=hashlib.sha256(relocated[n]).hexdigest()
                assert row['full_relocated_equal']
                row['status']='INDIVIDUAL_BODY_PROOF_ONLY_NO_CLAIM'
            else:
                row['full_relocated_equal']=False
                row['own_data']={'status':'NOT_PROVEN_FOR_NONMATCHING_CALLER'}
                row['status']='NONMATCH_MISSING_TWO_OUTGOING_PAGE_STORES'
            result['comparisons'][n]=row
        # Corrupt a temporary object's first helper-table relocation addend.
        # This negative control must fail content proof; protected inputs stay read-only.
        helper_own=result['comparisons']['voice_stop_2']['own_data']
        table_start=helper_own['placements']['.rodata'][0][0]
        rodata=next(sec for sec in sections if sec['name']=='.rodata')
        corrupted=bytearray(data); at=rodata['off']+table_start
        original,=struct.unpack_from('>I',corrupted,at)
        struct.pack_into('>I',corrupted,at,original ^ 4)
        negative=Path(tmp)/'wrong-helper-table.o';negative.write_bytes(corrupted)
        refused=owndata.verify(negative,'voice_stop_2',targets['voice_stop_2'],
                               address=addresses['voice_stop_2'],image=image,addresses=addresses.get)
        assert refused.failures
        result['negative_controls']={
            'mutated_helper_table_rejected':True,
            'short_caller_not_rescued_by_zero_alignment':not result['comparisons']['audio_bus_mix']['exact_extent'],
            'protected_inputs_remained_unchanged':all(sha(ROOT/p)==v for p,v in result['protected_inputs'].items())}
        assert all(result['negative_controls'].values())
    result['limits']='Individual helper proof occurs in an explicitly nonmatching caller group and gives no claim. The setter caller has an unproved page ABI; no dummy fourth formal is added. No splice, whole-unit, compressed-image, or cartridge gate was run.'
    return result

if __name__=='__main__':
    result=verify(); print(json.dumps(result,indent=2))
    if len(sys.argv)>1:Path(sys.argv[1]).write_text(json.dumps(result,indent=2)+'\n')
