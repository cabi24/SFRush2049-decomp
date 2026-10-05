#!/usr/bin/env python3
"""Exact ELF extent, strict score, independent GNU link, real context and behavior."""
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
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=importlib.util.spec_from_file_location('multiblit_semantics',HERE/'verify_semantics.py')
sem=importlib.util.module_from_spec(spec);spec.loader.exec_module(sem)
FN='sound_control'
SOURCE=ROOT/'cloud/matches/sound_control.c'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
CONTEXT=['src/blob/ambient_sound_set.c','src/blob/func_800EF5B0.c','src/blob/collision_sound_play.c','src/blob/func_800B24EC.c','src/blob/groups/frontier_pad_config/group.c']
CONTEXT_NAMES=['ambient_sound_set','func_800EF5B0','collision_sound_play','func_800B24EC','Input_ApplyPadConfig','Input_InitPadHandlers']
def sha(data):return hashlib.sha256(data).hexdigest()
def inspect(obj,name):
 data,sections=score._elf(obj);ti=score._text_index(sections);txt=sections[ti]
 syms=[s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i)]
 fn,=[s for s in syms if s['name']==name and s['type']==2 and s['section']==ti]
 words=list(struct.unpack('>%dI'%(txt['size']//4),data[txt['off']:txt['off']+txt['size']]))
 start=fn['value'];end=start+fn['size'];native=score.targets()[name]
 got,masks,unresolved,unverified,errors=score.relocate(obj,words,start,end,score.image_symbols())
 comparison=score.compare(obj,name,show=0)
 return dict(asdict(comparison),canonical_verdict=comparison.summary(),symbol_bytes=fn['size'],native_bytes=len(native)*4,
  full_extent_equal=got[start//4:end//4]==native and fn['size']==len(native)*4,
  all_relocations_resolved=not(masks or unresolved or unverified or errors))
def verify(directory):
 directory.mkdir(parents=True,exist_ok=True);obj=directory/'candidate.o'
 score.compile_single(SOURCE,FLAGS,obj)
 result={'base_revision':'cc4d5fdd0bbc42dbf6be49f00d9454af8cb9c4f5','function':FN,
  'range':['0x800B37E8','0x800B39BC'],'status':'STRICT_MATCH_PENDING_INDEPENDENT_REVIEW',
  'accepted_byte_gain':0,'flags':FLAGS,'source_sha256':sha(SOURCE.read_bytes()),
  'target_manifest_sha256':sha((score.ASM_DIR/'SHA256SUMS').read_bytes()),'object':inspect(obj,FN)}
 assert result['object']['canonical_verdict']=='MATCH'
 assert result['object']['symbol_bytes']==468 and result['object']['full_extent_equal'] and result['object']['all_relocations_resolved']
 data,sections=score._elf(obj);ti=score._text_index(sections);txt=sections[ti]
 syms=[s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i)]
 defined=[s for s in syms if s['type']==2 and s['section']==ti]
 assert len(defined)==1 and defined[0]['name']==FN and defined[0]['value']==0
 assert txt['size']==480 and data[txt['off']+468:txt['off']+480]==bytes(12)
 assert all(s['size']==0 for s in sections if s['name'] in ('.rodata','.data','.bss','.sdata','.sbss','.lit4','.lit8'))
 relocs=[]
 for sec in sections:
  if sec['type']==9:
   assert sec['info']==ti
   symbols=score._symbol_table(data,sections,sec['link'])
   for off in range(sec['off'],sec['off']+sec['size'],8):
    address,info=struct.unpack_from('>II',data,off)
    relocs.append({'offset':address,'type':info&255,'symbol':symbols[info>>8]['name']})
 assert len(relocs)==4 and all(r['type']==4 for r in relocs)
 assert {r['symbol'] for r in relocs}=={'func_800B3704','Input_ApplyPadConfig','sound_stop'}
 result['relocations']=relocs;result['text_alignment_bytes_outside_symbol']=12;result['own_data_bytes']=0
 addresses=score.image_symbols();externals={r['symbol'] for r in relocs}
 script=directory/'link.ld';script.write_text('SECTIONS { .text 0x800B37E8 : SUBALIGN(4) { *(.text) } }\n'+''.join('%s = 0x%08X;\n'%(n,addresses[n]) for n in sorted(externals)))
 elf=directory/'candidate.elf'
 subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True,text=True)
 linked,secs=score._elf(elf);txt=secs[score._text_index(secs)]
 lsyms=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(linked,secs,i)]
 fn,=[s for s in lsyms if s['name']==FN and s['type']==2]
 assert fn['value']==addresses[FN] and fn['size']==468
 body=linked[txt['off']:txt['off']+468];words=list(struct.unpack('>117I',body))
 assert words==score.targets()[FN]
 assert linked[txt['off']+468:txt['off']+480]==bytes(12)
 result['gnu_link']={'full_symbol_equal':True,'address':hex(fn['value']),'symbol_bytes':fn['size'],'body_sha256':sha(body)}
 result['controls']={}
 for label,path,flags in [('o2',SOURCE,FLAGS.replace('-O3','-O2')),('archived_a78',ROOT/'cloud/work/tiny_A78/sound_control.callback.c',FLAGS)]:
  out=directory/(label+'.o');score.compile_single(path,flags,out);result['controls'][label]=inspect(out,FN)
 assert result['controls']['o2']['differing']==115 and result['controls']['archived_a78']['differing']==9
 # Two bounded reversions distinguish descriptor traversal and callback expression.
 text=SOURCE.read_text()
 controls={
  'local_callback':text.replace('    curblit->AnimFunc=mblit[i].animfunc;\n    if(!curblit->AnimFunc(curblit)) {','    s32 (*action)(Blit *)=mblit[i].animfunc;\n    curblit->AnimFunc=action;\n    if(!action(curblit)) {'),
  'cursor':text.replace(' int i;',' int i;\n const MultiBlit *record;').replace(' for(i=0;i<nblits;i++) {',' record=mblit;\n for(i=0;i<nblits;i++) {').replace('mblit[i].','record->').replace('  lastblit=curblit;','  lastblit=curblit;\n  record++;')}
 for label,body in controls.items():
  assert body!=text
  src=directory/(label+'.c');src.write_text(body);out=directory/(label+'.o')
  score.compile_single(src,FLAGS,out);result['controls'][label]=inspect(out,FN)
 assert result['controls']['local_callback']['differing']==4 and result['controls']['cursor']['differing']==7
 # Ancillary constructor lead remains explicitly a nonmatch.
 scout=HERE/'new_blit_nonmatch.c';out=directory/'new_blit.o'
 score.compile_single(scout,FLAGS,out);result['scout']=inspect(out,'func_800B3704')
 result['scout']['source_sha256']=sha(scout.read_bytes())
 assert result['scout']['symbol_bytes']==228 and result['scout']['differing']==14
 # Offset 344 is behind an unconditional branch+delay and has no direct entry.
 target=score.targets()[FN];branch=target[84]
 assert branch>>26==4 and ((branch>>16)&1023)==0
 destinations=[]
 for i,word in enumerate(target):
  op=word>>26
  if op in (1,4,5,6,7,20,21,22,23):
   imm=word&65535;imm=imm-65536 if imm&32768 else imm
   destinations.append(i*4+4+4*imm)
 assert 344 not in destinations
 result['unreachable_instruction_proof']={'offset':344,'preceding_unconditional_branch_offset':336,'no_direct_branch_destination':True,'only_indirect_transfers':'external animation call and function return'}
 # Genuine unchanged accepted siblings and helpers; no substituted callee body.
 group=directory/'context';group.mkdir();files=[SOURCE]+[ROOT/p for p in CONTEXT]
 for i,p in enumerate(files):shutil.copy2(p,group/('c%d.c'%i))
 names=[FN]+CONTEXT_NAMES
 (group/'group.json').write_text(json.dumps({'files':['c%d.c'%i for i in range(len(files))],'members':names,'keep':names,'flags':FLAGS}))
 score.compile_group(group,group/'group.o');result['real_context']={n:inspect(group/'group.o',n) for n in names}
 assert all(x['full_extent_equal'] and x['all_relocations_resolved'] and x['canonical_verdict']=='MATCH' for x in result['real_context'].values())
 result['context_source_sha256']={p:sha((ROOT/p).read_bytes()) for p in CONTEXT}
 result['host_native_linked']=sem.verify(directory,words)
 result['tool_sha256']={p:sha((ROOT/p).read_bytes()) for p in ['tools/cloud/score.py','tools/cloud/owndata.py']}
 result['packet_sha256']={p.name:sha(p.read_bytes()) for p in [HERE/'verify.py',HERE/'verify_semantics.py',HERE/'semantic_test.c',HERE/'new_blit_nonmatch.c',HERE/'claim.json']}
 result['compiler_sha256']={n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']}
 result['limitations']=['No full-game shadow unit, image, compression or ROM gates run.','The seven-body context has external NewBlit and RemoveBlit calls; those unaccepted callees are not claimed.','Behavior uses synthetic constructor/update/animation/removal effects and valid nonaliasing lists; it does not claim gameplay coverage.']
 return result
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--write',action='store_true');a=p.parse_args()
 with tempfile.TemporaryDirectory(prefix='multiblit-proof-') as d:r=verify(Path(d))
 if a.write:(HERE/'verification.json').write_text(json.dumps(r,indent=2)+'\n')
 print(json.dumps({k:r[k] for k in ['status','object','gnu_link','host_native_linked','real_context']},indent=2))
