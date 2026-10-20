#!/usr/bin/env python3
"""Recovered PR163 image-A slider: complete strict source/extent/identity proof."""
from pathlib import Path
import dataclasses, hashlib, importlib.util, json, os, shutil, struct, subprocess, sys, tempfile, zlib
HERE=Path(__file__).resolve().parent
ROOT=Path(os.environ['N64_RESEARCH_REPO']).resolve() if os.environ.get('N64_RESEARCH_REPO') else next((p for p in HERE.parents if (p/'tools/cloud/score.py').is_file()),Path.cwd())
if not (ROOT/'tools/cloud/score.py').is_file():raise RuntimeError('Set N64_RESEARCH_REPO to a complete checkout')
sys.path.insert(0,str(ROOT))
from tools.cloud import score

def require(ok,message):
 if not ok:raise RuntimeError(message)
def sha(b):return hashlib.sha256(b).hexdigest()
def load(name):
 spec=importlib.util.spec_from_file_location('slider_'+name,HERE/(name+'.py'));m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m
elf=load('elf_support').elf
behavior=load('behavior')
DELIVERABLE='cloud/matches/ovl_a/func_803A3DA4.c'
NAME='func_803A3DA4';ENTRY=0x803A3DA4;SIZE=912
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
ANCHORS={'D_8014A108':0x8014A108,'D_803BA028':0x803BA028,'D_803B9FD0':0x803B9FD0,'D_803AF9A8':0x803AF9A8,'D_803B97A8':0x803B97A8,'D_803B97AC':0x803B97AC,'D_803B97B0':0x803B97B0,'D_803B97B4':0x803B97B4,'D_803B97B8':0x803B97B8,'D_80111310':0x80111310,'D_80111414':0x80111414,'D_80150B70':0x80150B70,'input_new_data_wrapper':0x80094F88,'brake_light_update':0x800A6244,'Input_ApplyPadConfig':0x80094EC8}
TOOL_FILES={'tools/cloud/score.py','tools/cloud/owndata.py'}
COMPILERS={'cc','acpp','cfe','uopt','ugen','as1','uld','umerge','usplit','ujoin'}
SOURCES={'group.json','fidelity.json','candidate.c','pr163_source.c','host.c','behavior.py','elf_support.py','verify.py','evidence.json','controls/direct_slot_only.c','controls/inline_quarter_only.c','controls/named_slot_and_height.c'}

def run(*args):
 p=subprocess.run([str(x) for x in args],capture_output=True,text=True)
 require(p.returncode==0,'Command failed: '+str(args[0])+'\n'+p.stderr[:1500]);return p.stdout

def validate_delivery(pins,packet=HERE,root=ROOT):
 require(pins['delivery_path']==DELIVERABLE,'Deliverable path changed')
 delivery=root/DELIVERABLE
 require(sha(delivery.read_bytes())==pins['delivery_sha256']==pins['sources']['candidate.c'] and delivery.read_bytes()==(packet/'candidate.c').read_bytes(),'Deliverable source mismatch')
 return delivery

def validate_inputs(packet=HERE):
 pins=json.loads((packet/'inputs.json').read_text())
 require(pins['claims']==[NAME] and pins['new_discovery_bytes']==0 and pins['new_accepted_image_bytes']==0 and pins['new_aggregate_candidate_bytes']==0,'False credit')
 require(set(pins['sources'])==SOURCES,'Source inventory changed')
 require(set(pins['compiler_sha256'])==COMPILERS,'Compiler inventory changed')
 require(set(pins['tools_sha256'])==TOOL_FILES,'Comparison tool inventory changed')
 for n,h in pins['tools_sha256'].items():require(sha((ROOT/n).read_bytes())==h,'Comparison tool implementation changed: '+n)
 require(pins['recovered_prior_exact_bytes']==912,'Recovered extent changed')
 for n,h in pins['sources'].items():require(sha((packet/n).read_bytes())==h,'Source changed: '+n)
 validate_delivery(pins,packet)
 require(pins['image']=='A' and pins['target']==NAME and pins['function_bytes']==SIZE,'Target scope changed')
 previous=score.ASM_DIR
 try:
  score.ASM_DIR=ROOT/'asm/us/ovl_a';words=score.targets()[NAME];manifest=score.target_manifest();meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest));extent=next(x for x in meta['functions'] if x['name']==NAME)
  require(extent['address']=='0x803A3DA4' and extent['size']==SIZE,'Protected extent changed')
  require(sha(struct.pack('>%dI'%len(words),*words))==pins['native_sha256'] and len(words)*4==SIZE,'Native target changed')
  addresses=score.image_symbols()
  for n,a in ANCHORS.items():require(addresses.get(n,score.address_named(n))==a,'External anchor changed: '+n)
 finally:score.ASM_DIR=previous
 return pins

def inspect(obj,linked=False):
 secs,syms,rels=elf(obj);idx,row,raw=secs['.text'];funcs={n:v for n,v in syms.items() if v[2]==2 and v[3] not in (0,0xfff1)}
 require(funcs=={NAME:(ENTRY if linked else 0,SIZE,2,idx)},'Function inventory or extent changed')
 require(len(raw)==SIZE and row[3]==(ENTRY if linked else 0),'Complete text extent/address changed')
 for n,(_,s,b) in secs.items():
  if s[2]&2 and n not in ('.text','.options','.reginfo'):require(s[5]==0,'Unexpected allocated storage: '+n)
 if linked:
  require(not rels,'Unresolved GNU relocations')
  for n,a in ANCHORS.items():require(syms[n][0]==a and syms[n][3]==0xfff1,'GNU external anchor changed')
 else:
  require(len(rels)==31,'Relocation count changed')
  require({n for n,s in syms.items() if s[3]==0}==set(ANCHORS),'Undefined symbol set changed')
  require(all(off%4==0 and off<SIZE and kind in (4,5,6) and symbol in ANCHORS and target==idx for off,kind,symbol,target in rels),'Relocation scope changed')
 return raw,rels

def check_native(native,compiled):require(len(native)==len(compiled)==228 and native==compiled,'Complete native words changed')

def authenticate_callbacks(pins):
 asset=(ROOT/'assets/us/data.bin').read_bytes();require(sha(asset)==pins['asset_sha256'],'Private asset identity changed')
 manifest=score.target_manifest();meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest));off=int(meta['rom_offset'],16)
 require(int.from_bytes(asset[0x3850:0x3854],'big')==off==0xB5C534,'Overlay loader identity changed')
 d=zlib.decompressobj(-15);image=d.decompress(asset[off-0x283D0:])+d.flush();require(d.eof and len(image)==meta['size'] and sha(image)==meta['image_sha256']==pins['image_sha256'],'Image identity changed')
 base=int(meta['base'],16);needle=struct.pack('>I',ENTRY);hits=[base+i for i in range(0,len(image)-4,4) if image[i:i+4]==needle]
 expected=[0x803B2AD4+36*i for i in range(16)];require(hits==expected,'Callback reference set changed')
 ids=[]
 for address in hits:
  at=address-base;nameptr=struct.unpack_from('>I',image,at-28)[0]-base
  require(image[nameptr:nameptr+7]==b'SLIDER\0','Callback resource identity changed')
  ids.append(struct.unpack_from('>I',image,at+4)[0])
 require(ids==[(opt<<16)|(p<<4)|kind for p in range(4) for opt in (13,14) for kind in (0,1)],'Native slider callback IDs changed')
 return {'record_count':16,'definition_stride':36,'resource':'SLIDER','anim_ids':ids,'pointer_addresses':hits,'image_sha256':meta['image_sha256']}

def validate_compilers(pins,resolver=None):
 require(set(pins['compiler_sha256'])==COMPILERS,'Compiler inventory changed')
 resolver=resolver or (lambda n:Path(score.ido(n)))
 for n,h in pins['compiler_sha256'].items():
  p=Path(resolver(n));require(p.is_file() and sha(p.read_bytes())==h,'Pinned compiler unavailable or changed: '+n)

def measure():
 pins=validate_inputs()
 validate_compilers(pins)
 linker=shutil.which('mips-linux-gnu-ld');require(linker is not None and shutil.which('gcc') is not None,'Existing GNU linker and host compiler required')
 previous=score.ASM_DIR
 try:
  score.ASM_DIR=ROOT/'asm/us/ovl_a';words=score.targets()[NAME];addresses=score.image_symbols()
  callbacks=authenticate_callbacks(pins)
  with tempfile.TemporaryDirectory(prefix='slider-proof-') as t:
   build=Path(t);source=validate_delivery(pins);obj=build/'candidate.o';score.compile_single(source,FLAGS,obj)
   raw,rels=inspect(obj);r=score.compare(obj,NAME,show=0);require(r.accepted() and r.total==228,'Candidate lost strict match')
   relocated,masks,unresolved,unverified,errors=score.relocate(obj,score.text_words(obj),0,SIZE,addresses)
   require(not any((masks,unresolved,unverified,errors)),'Relocation not fully verified');check_native(words,relocated)
   script=build/'whole.ld';script.write_text('SECTIONS { .text 0x803A3DA4 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'+''.join('%s = 0x%X;\n'%(n,a) for n,a in ANCHORS.items()))
   linked=build/'whole.elf';run(linker,'-EB','-T',script,'-o',linked,obj);gnu,_=inspect(linked,True);require(gnu==struct.pack('>228I',*words),'Independent GNU complete-body mismatch')
   group_spec=json.loads((HERE/'group.json').read_text());require(group_spec['members']==[NAME] and group_spec['context']==[] and group_spec['claims']==[NAME] and group_spec['files']==['candidate.c'] and group_spec['keep']==[NAME] and group_spec['flags']==FLAGS,'Secondary group scope changed');group_obj=build/'group.o';score.compile_group(HERE,group_obj);inspect(group_obj);gr=score.compare(group_obj,NAME,show=0);require(gr.accepted(),'Secondary one-root group lost strict match');fidelity=json.loads((HERE/'fidelity.json').read_text());require(sha(group_obj.read_bytes())==fidelity['stock_sha256']==fidelity['traced_sha256'] and fidelity['whole_object_identical'],'Secondary group fidelity changed')
   baseline=build/'prior.o';score.compile_single(HERE/'pr163_source.c',FLAGS,baseline);inspect(baseline);br=score.compare(baseline,NAME,show=0);require(br.accepted(),'Recovered historical baseline changed')
   controls={}
   for p in sorted((HERE/'controls').glob('*.c')):
    out=build/(p.stem+'.o');score.compile_single(p,FLAGS,out);cr=score.compare(out,NAME,show=0);require(not cr.accepted(),'Historical negative unexpectedly exact');controls[p.name]=dataclasses.asdict(cr)
   require({n:r['differing'] for n,r in controls.items()}=={'named_slot_and_height.c':23,'direct_slot_only.c':4,'inline_quarter_only.c':27},'Historical controls changed')
   fields={'X':14,'Y':16,'Width':20,'Height':22,'Alpha':24,'Hide':26,'Top':28,'Bot':30,'Left':32,'Right':34,'AnimFunc':40,'AnimID':44}
   layout=build/'layout.c';expected_layout=list(fields.values())+[64,64,152]
   layout.write_text('#include "'+str(source)+'"\nunsigned int slider_layout[] = {'+','.join('(unsigned int)&(((Blit *)0)->'+n+')' for n in fields)+',sizeof(Blit),sizeof(MenuSlot),sizeof(ProjectContext)};\n')
   layout_obj=build/'layout.o';score.compile_single(layout,FLAGS,layout_obj);ls,ly,lr=elf(layout_obj);symbol=ly['slider_layout'];section=next(v for v in ls.values() if v[0]==symbol[3]);values=list(struct.unpack('>15I',section[2][symbol[0]:symbol[0]+symbol[1]]));require(values==expected_layout,'Native target-compiler layout changed')
   behavior_result=behavior.verify(build/'host',source);mutants={}
   for label,old,new in [('wrong_height_division','blt->Y=screenPos[1]-(blt->Height/4)/2;','blt->Y=screenPos[1]-(blt->Height/8)/2;'),('wrong_marker_case','case 13:','case 12:'),('missing_disable','blt->AnimFunc=0;',';')]:
    text=source.read_text();require(text.count(old)==1,'Mutation binding changed');mutant=build/(label+'.c');mutant.write_text(text.replace(old,new));host=behavior.host(build/label,mutant);failure=next((list(c) for c in behavior.cases() if host(c)!=behavior.oracle(c)),None);require(failure is not None,'Behavior negative accepted: '+label);score.compile_single(mutant,FLAGS,build/(label+'.o'));mr=score.compare(build/(label+'.o'),NAME,show=0);require(not mr.accepted(),'Source negative accepted: '+label);mutants[label]={'host_rejected':True,'counterexample':failure,'strict_differences':mr.differing,'strict_rejected':True}
   return {'claims':[NAME],'new_discovery_bytes':0,'new_accepted_image_bytes':0,'new_aggregate_candidate_bytes':0,'recovered_prior_exact_bytes':SIZE,'delivery_path':DELIVERABLE,'delivery_sha256':pins['delivery_sha256'],'image':'A','target':NAME,'native_sha256':pins['native_sha256'],'comparison':dataclasses.asdict(r),'accepted':r.accepted(),'function_bytes':SIZE,'alignment_bytes':0,'allocated_own_data_bytes':0,'relocations':len(rels),'anchors':ANCHORS,'gnu_complete_body_equal':True,'historical_baseline_accepted':br.accepted(),'secondary_group_accepted':gr.accepted(),'controls':controls,'native_callback_identity':callbacks,'native_layout':dict(fields,Blit_size=64,MenuSlot_size=64,ProjectContext_size=152),'behavior':behavior_result,'source_negatives':mutants,'source_pinset_sha256':sha(json.dumps(pins['sources'],sort_keys=True).encode())}
 finally:score.ASM_DIR=previous

if __name__=='__main__':
 actual=measure();expected=json.loads((HERE/'verification.json').read_text());require(actual==expected,'Saved verification changed');print(json.dumps(actual,indent=2,sort_keys=True))
