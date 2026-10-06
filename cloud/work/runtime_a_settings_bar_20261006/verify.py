#!/usr/bin/env python3
"""Reproduce complete NONMATCH and bounded native/linked callback behavior."""
import argparse,hashlib,importlib.util,json,os,struct,subprocess,sys
from pathlib import Path
from dataclasses import asdict
from elf_support import elf
import semantics
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
NAME='func_803AE63C';ENTRY=0x803AE63C;SIZE=772
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
ANCHORS={'D_803B6B14':0x803B6B14,'D_803B99D4':0x803B99D4,'D_803B99D8':0x803B99D8,'D_803B99DC':0x803B99DC,'D_80150B70':0x80150B70,'D_8013F1D8':0x8013F1D8,'D_80142726':0x80142726,'D_80152030':0x80152030,'D_80150EFC':0x80150EFC,'input_new_data_wrapper':0x80094F88,'brake_light_update':0x800A6244,'Input_ApplyPadConfig':0x80094EC8}
HELPERS={'input_new_data_wrapper':(0x80094F88,60),'brake_light_update':(0x800A6244,448),'Input_ApplyPadConfig':(0x80094EC8,192),'sound_control':(0x800B37E8,468),'UpdateActiveObjects':(0x800F733C,192)}
def sha(b):return hashlib.sha256(b).hexdigest()
def run(*args):
 p=subprocess.run([str(a) for a in args],capture_output=True,text=True);assert p.returncode==0,(args,p.stdout,p.stderr);return p.stdout
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,default=ROOT);ap.add_argument('--tools-repo',type=Path,default=ROOT);ap.add_argument('--check',action='store_true');a=ap.parse_args()
 repo=a.repo.resolve();tr=a.tools_repo.resolve();build=ROOT/'build/settings_bar';build.mkdir(parents=True,exist_ok=True)
 sys.path.insert(0,str(tr/'tools/cloud'))
 spec=importlib.util.spec_from_file_location('score',tr/'tools/cloud/score.py');score=importlib.util.module_from_spec(spec);sys.modules['score']=score;spec.loader.exec_module(score)
 score.ASM_DIR=repo/'asm/us/ovl_a';manifest=score.target_manifest();words=score.targets()[NAME];native=struct.pack('>%dI'%len(words),*words)
 assert len(native)==SIZE
 meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest));row=next(e for e in meta['functions'] if e['name']==NAME)
 assert row=={'name':NAME,'address':hex(ENTRY).upper().replace('0X','0x'),'size':SIZE,'evidence':['data_ref','prologue']}
 addresses=score.image_symbols()
 for n,v in ANCHORS.items():assert addresses.get(n,score.address_named(n))==v,n
 source=HERE/'candidate.c';obj=build/'candidate.o';score.compile_single(source,FLAGS,obj)
 result=score.compare(obj,NAME,show=0);assert result.differing==4 and not any((result.extra_words,result.unresolved,result.unverified,result.errors))
 sections,symbols,relocs=elf(obj);index,row,raw=sections['.text'];assert symbols[NAME]==(0,SIZE,2,index);assert len(raw)==784 and raw[SIZE:]==bytes(12)
 assert len(relocs)==21 and set(r[2] for r in relocs)==set(ANCHORS)
 assert all(r[0]<SIZE and r[0]%4==0 and r[1] in (4,5,6) and r[3]==index for r in relocs)
 for n,(_,s,b) in sections.items():
  if s[2]&2 and n not in ('.text','.reginfo','.options'):assert s[5]==0,n
 full=score.text_words(obj);resolved,masks,unresolved,unverified,errors=score.relocate(obj,full,0,len(full)*4,addresses)
 assert not any((masks,unresolved,unverified,errors));rb=struct.pack('>%dI'%len(resolved),*resolved)
 offsets=[i*4 for i,(x,y) in enumerate(zip(words,resolved)) if x!=y];assert offsets==[0x8c,0x9c,0xd0,0xe4]
 for i in offsets:assert words[i//4]^resolved[i//4]==4 and words[i//4]&0xffff==44 and resolved[i//4]&0xffff==40
 script=build/'whole.ld';script.write_text('SECTIONS { .text 0x803AE63C : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'+''.join('%s = 0x%X;\n'%(n,v) for n,v in ANCHORS.items()))
 linked=build/'candidate.elf';run('mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj)
 sec,sym,rel=elf(linked);linkedraw=sec['.text'][2];assert linkedraw==rb and not rel and sym[NAME]==(ENTRY,SIZE,2,sec['.text'][0])
 for n,v in ANCHORS.items():assert sym[n][0]==v and sym[n][3]==0xfff1
 score.ASM_DIR=repo/'asm/us/blob';helper_manifest=score.target_manifest();hw=score.targets();hs=score.image_symbols();helper_bindings={}
 for n,(entry,size) in HELPERS.items():
  body=struct.pack('>%dI'%len(hw[n]),*hw[n]);assert hs[n]==entry and len(body)==size
  helper_bindings[n]={'address':hex(entry),'bytes':size,'sha256':sha(body)}
 behavior,samples=semantics.verify(words,list(struct.unpack('>193I',linkedraw[:SIZE])),hw['input_new_data_wrapper'])
 score.ASM_DIR=repo/'asm/us/ovl_a';text=source.read_text();controls={};mutants={}
 changes={'wrong_alpha_cap':('blt->Alpha=254','blt->Alpha=253'),'wrong_first_divisor':('*D_8013F1D8/3','*D_8013F1D8/5'),'wrong_marker_bits':('blt->AnimID>>16','blt->AnimID>>17'),'wrong_return':('return 1;','return 2;')}
 for label,(old,new) in changes.items():
  assert old in text;p=build/(label+'.c');p.write_text(text.replace(old,new));o=build/(label+'.o');score.compile_single(p,FLAGS,o)
  se,sy,re=elf(o);assert sy[NAME][1]==SIZE,(label,sy[NAME])
  ww=score.text_words(o);rr,mm,uu,vv,ee=score.relocate(o,ww,0,len(ww)*4,addresses);assert not any((mm,uu,vv,ee))
  engine=semantics.Native(rr[:SIZE//4],hw['input_new_data_wrapper']);witness=None;reason=None
  for c in samples:
   try:got=engine.run(c)
   except AssertionError as exc:witness=list(c);reason='native contract rejected '+str(exc);break
   if got!=semantics.oracle(c):witness=list(c);reason='oracle output differs';break
  assert witness is not None,label
  mutants[label]={'source_sha256':sha(p.read_bytes()),'witness':witness,'reason':reason,'comparison':asdict(score.compare(o,NAME,show=0))}
 named=text.replace('    s16 position[2];','    s16 position[2];\n    s32 height;').replace('    blt->Y=position[1]-(blt->Height/4)/2;','    height=blt->Height/4;\n    blt->Y=position[1]-height/2;').replace('blt->Bot=blt->Height/4-1','blt->Bot=height-1')
 grouped=text.replace('    s16 position[2];\n    MenuSlot *slot=&D_803B6B14[marker];','    MenuSlot *slot=&D_803B6B14[marker];\n    s16 position[2];')
 for label,s,flags,expected in [('named_height_O2',named,FLAGS.replace('-O3','-O2'),192),('named_height_O3',named,FLAGS,21),('slot_before_output',grouped,FLAGS,7),('narrow_height',named.replace('s32 height;','s16 height;'),FLAGS,136)]:
  p=build/(label+'.c');p.write_text(s);o=build/(label+'.o');score.compile_single(p,flags,o);r=score.compare(o,NAME,show=0);assert r.differing==expected
  se,sy,re=elf(o);controls[label]={'source_sha256':sha(s.encode()),'flags':flags,'comparison':asdict(r),'function_bytes':sy[NAME][1],'text_bytes':len(se['.text'][2])}
 layout=build/'layout.c';layout.write_text('#include "'+str(source)+'"\n'+''.join('typedef char a%d[%s ? 1 : -1];\n'%(i,expr) for i,expr in enumerate(['sizeof(void*)==4','sizeof(MenuSlot)==64','sizeof(ProjectContext)==152','__builtin_offsetof(Blit,AnimID)==44','__builtin_offsetof(Blit,Top)==28','__builtin_offsetof(Blit,Right)==34','__builtin_offsetof(MenuSlot,state.alpha)==60'])))
 run('gcc','-m32','-std=c89','-fsyntax-only',layout)
 receipt={'base_commit':'dea99f09ab19b1d3b324ed7097162f7b378e7096','status':'NONMATCH','claim':False,'accepted_bytes':0,'image':'A','address':hex(ENTRY),'end':hex(ENTRY+SIZE),'bytes':SIZE,'words':len(words),'flags':FLAGS,'source_sha256':sha(source.read_bytes()),'native_sha256':sha(native),'linked_sha256':sha(linkedraw),'function_bytes':SIZE,'alignment_bytes':12,'owned_data_bytes':0,'comparison':asdict(result),'residual_offsets':[hex(i) for i in offsets],'relocations':len(relocs),'anchors':{n:hex(v) for n,v in ANCHORS.items()},'behavior':behavior,'controls':controls,'mutants':mutants,'helpers':helper_bindings,'protected_targets':manifest,'helper_protected_targets':helper_manifest,'tools':{n:sha((score.IDO/n).read_bytes()) for n in ('cc','cfe','uopt','ugen','as1')},'scorer_sha256':sha((tr/'tools/cloud/score.py').read_bytes()),'support_sha256':{n:sha((HERE/n).read_bytes()) for n in ('semantics.py','elf_support.py','verify.py')},'limits':['Synthetic backed marker indices 0..256, including >8-bit control; actual descriptor reachability/table bounds unknown','Finite float comparisons only; actual threshold values unavailable and not fabricated','Projection is an external pointer/output contract; numerical internals, float overflow and FCSR/traps unproved','Real native Hidden body executes, but UpdateBlit renderer internals are effectful contract hooks','Signed halfword narrowing is target behavior, not a portability claim','No host-C runtime, full image, compression, ROM, hardware, gameplay or coverage-acceptance claim']}
 out=HERE/'verification.json'
 # The whole blob manifest changes with every splice; helper bodies stay bound in 'helpers'.
 movable=lambda r:{k:v for k,v in r.items() if k!='helper_protected_targets'}
 if a.check:assert movable(json.loads(out.read_text()))==movable(receipt),'receipt changed'
 else:out.write_text(json.dumps(receipt,indent=2)+'\n')
 print(json.dumps({k:receipt[k] for k in ('status','bytes','comparison','behavior')},indent=2))
if __name__=='__main__':
 assert __debug__,'Python optimization unsupported'
 main()
