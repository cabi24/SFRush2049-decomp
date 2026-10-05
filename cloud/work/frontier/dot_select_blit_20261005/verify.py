#!/usr/bin/env python3
"""Fresh complete object/link/context/behavior proof, with no protected writes."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

spec = importlib.util.spec_from_file_location('select_blit_semantics', HERE/'verify_semantics.py')
semantics = importlib.util.module_from_spec(spec)
spec.loader.exec_module(semantics)
FN = 'stat_race_update'
SOURCE = ROOT/'cloud/matches/stat_race_update.c'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
CONTEXT_SOURCES = ['src/blob/func_800EF5B0.c','src/blob/collision_sound_play.c',
                   'src/blob/func_800B24EC.c','src/blob/groups/frontier_pad_config/group.c']
CONTEXT_NAMES = ['func_800EF5B0','collision_sound_play','func_800B24EC',
                 'Input_ApplyPadConfig','Input_InitPadHandlers']


def sha(data):
    return hashlib.sha256(data).hexdigest()


def inspect(obj, name):
    data, sections = score._elf(obj)
    ti = score._text_index(sections)
    syms = [s for i,sec in enumerate(sections) if sec['type'] == 2
            for s in score._symbol_table(data,sections,i)]
    fn, = [s for s in syms if s['name'] == name and s['type'] == 2 and s['section'] == ti]
    text = sections[ti]
    words = struct.unpack('>%dI'%(text['size']//4),data[text['off']:text['off']+text['size']])
    native = score.targets()[name]
    start = fn['value']
    got,masks,unresolved,unverified,errors = score.relocate(
        obj, words, start, start+fn['size'], score.image_symbols())
    body = got[start//4:(start+fn['size'])//4]
    comparison = score.compare(obj,name,show=0)
    return dict(asdict(comparison),canonical_verdict=comparison.summary(),
                symbol_bytes=fn['size'],native_bytes=len(native)*4,
                full_extent_equal=list(body)==native and fn['size']==len(native)*4,
                all_relocations_resolved=not(masks or unresolved or unverified or errors))


def verify(directory):
    directory.mkdir(parents=True,exist_ok=True)
    obj = directory/'candidate.o'
    score.compile_single(SOURCE,FLAGS,obj)
    result = {'base_revision':'cc4d5fdd','function':FN,'range':['0x800FE5B0','0x800FE73C'],
              'status':'STRICT_MATCH_PENDING_INDEPENDENT_REVIEW',
              'candidate_bytes':396,'accepted_byte_gain':0,'flags':FLAGS,
              'assembler_erratum_flag_added_by_scorer':score.R4300_CC,
              'source_sha256':sha(SOURCE.read_bytes()),
              'target_manifest_sha256':sha((score.ASM_DIR/'SHA256SUMS').read_bytes()),
              'object':inspect(obj,FN)}
    assert result['object']['full_extent_equal'] and result['object']['all_relocations_resolved']
    assert result['object']['canonical_verdict'] == 'MATCH'
    data,sections = score._elf(obj)
    ti = score._text_index(sections); text = sections[ti]
    assert text['size'] == 400
    assert data[text['off']+396:text['off']+400] == bytes(4)
    relocs=[]
    for sec in sections:
        if sec['type'] == 9 and sec['info'] == ti:
            syms=score._symbol_table(data,sections,sec['link'])
            for off in range(sec['off'],sec['off']+sec['size'],8):
                address,info=struct.unpack_from('>II',data,off)
                relocs.append({'offset':address,'type':info&255,'symbol':syms[info>>8]['name']})
    assert relocs == [{'offset':64,'type':4,'symbol':'func_800EF5B0'},
                      {'offset':372,'type':4,'symbol':'Input_ApplyPadConfig'}]
    result['relocations']=relocs
    result['zero_alignment_bytes_outside_symbol']=4
    assert all(s['size']==0 for s in sections if s['name'] in ('.data','.rodata','.bss'))
    # GNU ld independently applies both actual R_MIPS_26 relocations.
    ld=shutil.which('mips-linux-gnu-ld'); assert ld
    addresses=score.image_symbols()
    script=directory/'link.ld'
    script.write_text('SECTIONS { .text 0x800FE5B0 : { *(.text) } }\n'+''.join(
        '%s = 0x%08X;\n'%(n,addresses[n]) for n in ('func_800EF5B0','Input_ApplyPadConfig')))
    elf=directory/'candidate.elf'
    subprocess.run([ld,'-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True,text=True)
    linked,secs=score._elf(elf); txt=secs[score._text_index(secs)]
    linked_words=list(struct.unpack('>99I',linked[txt['off']:txt['off']+396]))
    assert linked_words==score.targets()[FN]
    linked_syms=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(linked,secs,i)]
    linked_fn,=[s for s in linked_syms if s['name']==FN]
    assert linked_fn['value']==addresses[FN] and linked_fn['size']==396
    result['gnu_link']={'full_symbol_equal':True,'address':hex(linked_fn['value']),
                        'symbol_bytes':linked_fn['size'],'no_own_data':True}
    result['controls']={}
    score.compile_single(SOURCE,FLAGS.replace('-O3','-O2'),directory/'o2.o')
    result['controls']['o2']=inspect(directory/'o2.o',FN)
    assert result['controls']['o2']['differing']==98
    archived=ROOT/'cloud/work/tiny_A71/stat_race_update.c'
    score.compile_single(archived,FLAGS,directory/'archived.o')
    result['controls']['archived_a71']=inspect(directory/'archived.o',FN)
    assert result['controls']['archived_a71']['differing']==45
    # Read-only authentic context, with the accepted exported roots unchanged.
    group=directory/'context'; group.mkdir()
    files=[SOURCE]+[ROOT/p for p in CONTEXT_SOURCES]
    for i,p in enumerate(files): shutil.copy2(p,group/('c%d.c'%i))
    names=[FN]+CONTEXT_NAMES
    (group/'group.json').write_text(json.dumps({'files':['c%d.c'%i for i in range(len(files))],
         'members':names,'keep':names,'flags':FLAGS}))
    score.compile_group(group,group/'group.o')
    result['real_context']={n:inspect(group/'group.o',n) for n in names}
    assert all(p['full_extent_equal'] and p['all_relocations_resolved'] for p in result['real_context'].values())
    result['context_source_sha256']={p:sha((ROOT/p).read_bytes()) for p in CONTEXT_SOURCES}
    result['host_native_linked']=semantics.verify(directory,linked_words)
    result['tool_sha256']={p:sha((ROOT/p).read_bytes()) for p in ['tools/cloud/score.py','tools/cloud/owndata.py']}
    result['packet_sha256']={p.name:sha(p.read_bytes()) for p in [HERE/'verify.py',HERE/'verify_semantics.py',HERE/'semantic_test.c']}
    result['compiler_sha256']={n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']}
    result['limitations']=['Private full-game image, compression and ROM gates were not run.',
                          'The six-body regression is an authentic local context check, not the full-game shadow unit.',
                          'Ownership scan cannot observe unpublished work.']
    return result


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--write',action='store_true')
    args=parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='select-blit-proof-') as tmp:
        result=verify(Path(tmp))
    if args.write: (HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'status':result['status'],'object':result['object'],
                      'gnu_link':result['gnu_link'],'host_native_linked':result['host_native_linked'],
                      'context':{k:v['canonical_verdict'] for k,v in result['real_context'].items()}},indent=2))
