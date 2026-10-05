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

spec = importlib.util.spec_from_file_location('blit_bar_semantics', HERE/'verify_semantics.py')
semantics = importlib.util.module_from_spec(spec)
spec.loader.exec_module(semantics)
FN = 'func_800EF62C'
SOURCE = ROOT/'cloud/matches/func_800EF62C.c'
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
    if masks:
        addresses=score.image_symbols()
        named=lambda n: addresses[n] if n in addresses else score.address_named(n)
        own=score.owndata.verify(obj,name,native,address=named(name),image=score.own_data(),start=start,addresses=named)
        assert own.ok and set(masks)<=own.sites
        bases=own.bases()
        for section in sections:
            if section['type']!=9 or section['info']!=ti:continue
            syms=score._symbol_table(data,sections,section['link']);pending={}
            for off in range(section['off'],section['off']+section['size'],8):
                site,info=struct.unpack_from('>II',data,off);index,kind=info>>8,info&255
                if site not in masks:continue
                symbol=syms[index];base=bases[sections[symbol['section']]['name']]+symbol['value']
                if kind==5:pending.setdefault(index,[]).append(site)
                elif kind==6:
                    low=words[site//4]&65535;low=low-65536 if low&32768 else low
                    his=pending.pop(index,[]);assert his
                    value=base+((words[his[0]//4]&65535)<<16)+low
                    for high in his:got[high//4]=(words[high//4]&0xffff0000)|(((value+0x8000)>>16)&65535)
                    got[site//4]=(words[site//4]&0xffff0000)|(value&65535)
                else:raise AssertionError('unsupported own relocation')
            assert not pending
        masks={};unverified=[]
    body = got[start//4:(start+fn['size'])//4]
    comparison = score.compare(obj,name,show=0)
    return dict(asdict(comparison),canonical_verdict=comparison.summary(),
                symbol_bytes=fn['size'],native_bytes=len(native)*4,
                full_extent_equal=list(body)==native and fn['size']==len(native)*4,
                all_relocations_resolved=not(masks or unresolved or unverified or errors))


def verify(directory):
    directory.mkdir(parents=True,exist_ok=True)
    obj=directory/'candidate.o'
    score.compile_single(SOURCE,FLAGS,obj)
    result={'base_revision':'cc4d5fdd','function':FN,'range':['0x800EF62C','0x800EF8F4'],
            'status':'STRICT_MATCH_PENDING_INDEPENDENT_REVIEW','candidate_bytes':712,
            'accepted_byte_gain':0,'flags':FLAGS,'source_sha256':sha(SOURCE.read_bytes()),
            'assembler_erratum_flag_added_by_scorer':score.R4300_CC,
            'target_manifest_sha256':sha((score.ASM_DIR/'SHA256SUMS').read_bytes()),
            'data_manifest_sha256':sha((ROOT/'asm/us/blob_data/SHA256SUMS').read_bytes()),
            'object':inspect(obj,FN)}
    assert result['object']['full_extent_equal'] and result['object']['all_relocations_resolved']
    assert result['object']['canonical_verdict']=='MATCH'
    data,sections=score._elf(obj);ti=score._text_index(sections);txt=sections[ti]
    symbols=[s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i)]
    fn,=[s for s in symbols if s['name']==FN]
    assert fn['value']==0 and fn['size']==712 and txt['size']==720
    assert data[txt['off']+712:txt['off']+720]==bytes(8)
    own,=[s for s in sections if s['name']=='.rodata']
    assert own['size']==16
    literals=struct.pack('>ff',1.35,.0001)
    assert data[own['off']:own['off']+8]==literals==score.own_data().read(0x801245A4,8)
    assert data[own['off']+8:own['off']+16]==bytes(8)
    assert all(s['size']==0 for s in sections if s['name'] in ('.data','.bss','.sdata','.sbss'))
    relocs=[]
    for sec in sections:
        if sec['type']==9 and sec['info']==ti:
            syms=score._symbol_table(data,sections,sec['link'])
            for off in range(sec['off'],sec['off']+sec['size'],8):
                address,info=struct.unpack_from('>II',data,off)
                relocs.append({'offset':address,'type':info&255,'symbol':syms[info>>8]['name']})
    assert len(relocs)==35 and sum(r['type']==4 for r in relocs)==5
    result['relocations']=relocs
    result['alignment_bytes']={'text_zero_outside_function':8,'rodata_zero_outside_literals':8}
    result['own_literals']={'address':'0x801245A4','bytes':8,'values':[1.35,0.0001],'verified':True}
    addresses=score.image_symbols()
    externals=set(r['symbol'] for r in relocs)-{'.rodata'}
    for name in externals:
        if name not in addresses:addresses[name]=int(name[2:],16)
    script=directory/'link.ld'
    script.write_text('SECTIONS { .text 0x800EF62C : SUBALIGN(4) { *(.text) } .rodata 0x801245A4 : SUBALIGN(4) { *(.rodata) } }\n'+''.join('%s = 0x%08X;\n'%(n,addresses[n]) for n in sorted(externals)))
    elf=directory/'candidate.elf'
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True,text=True)
    linked,secs=score._elf(elf);txt=secs[score._text_index(secs)]
    linked_words=list(struct.unpack('>178I',linked[txt['off']:txt['off']+712]))
    assert linked_words==score.targets()[FN]
    linked_syms=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(linked,secs,i)]
    linked_fn,=[s for s in linked_syms if s['name']==FN]
    assert linked_fn['value']==addresses[FN] and linked_fn['size']==712
    own,=[s for s in secs if s['name']=='.rodata']
    shoff,=struct.unpack_from('>I',linked,0x20)
    shsize,=struct.unpack_from('>H',linked,0x2e)
    own_address,=struct.unpack_from('>I',linked,shoff+secs.index(own)*shsize+12)
    assert own_address==0x801245A4 and linked[own['off']:own['off']+8]==literals
    result['gnu_link']={'full_symbol_equal':True,'address':hex(linked_fn['value']),
                        'symbol_bytes':linked_fn['size'],'own_literals_equal':True}
    result['controls']={}
    for label,path,flags in [('o2',SOURCE,FLAGS.replace('-O3','-O2')),
                             ('archived_a70',ROOT/'cloud/work/tiny_A70/func_800EF62C.c',FLAGS)]:
        o=directory/(label+'.o');score.compile_single(path,flags,o)
        result['controls'][label]=inspect(o,FN)
    assert result['controls']['archived_a70']['differing']==9
    source_text=SOURCE.read_text()
    crop='        blt->Right=(s32)((blt->Width-1)*amount);\n        blt->Top=blt->Height/2;\n        blt->Bot=blt->Height-1;'
    assert source_text.count(crop)==1
    lines=crop.splitlines()
    for label,order,expected in [('right_bot_top',(0,2,1),5),('bot_right_top',(2,0,1),9),
                                  ('bot_top_right',(2,1,0),13),('top_right_bot',(1,0,2),42),
                                  ('top_bot_right',(1,2,0),42)]:
        path=directory/(label+'.c');path.write_text(source_text.replace(crop,'\n'.join(lines[i] for i in order)))
        control=directory/(label+'.o');score.compile_single(path,FLAGS,control)
        result['controls'][label]=inspect(control,FN)
        assert result['controls'][label]['differing']==expected
    # Accepted exported roots and genuine definitions, unchanged and read-only.
    group=directory/'context';group.mkdir()
    files=[SOURCE]+[ROOT/p for p in CONTEXT_SOURCES]
    for i,p in enumerate(files):shutil.copy2(p,group/('c%d.c'%i))
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
    result['limitations']=['No full-game image, compression or ROM gates run.',
        'The genuine six-body context is narrower than the complete shadow unit.',
        'Native behavioral cases use safe table domains and synthetic callee effects; no gameplay-domain claim.']
    return result

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--write',action='store_true');args=parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='blit-bar-proof-') as tmp:result=verify(Path(tmp))
    if args.write:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({'status':result['status'],'object':result['object'],'gnu_link':result['gnu_link'],
        'host_native_linked':result['host_native_linked'],
        'context':{k:v['canonical_verdict'] for k,v in result['real_context'].items()}},indent=2))
