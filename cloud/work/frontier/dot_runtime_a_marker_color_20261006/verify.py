#!/usr/bin/env python3
"""Recompile full function, link with GNU at native address, prove bounded behavior."""
import argparse,hashlib,importlib.util,json,os,struct,subprocess,sys,tempfile
from pathlib import Path
from elf_support import elf
import semantics
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
NAME='func_803A3A6C';ENTRY=0x803A3A6C;SIZE=816
SOURCE=ROOT/'cloud/matches/ovl_a'/ (NAME+'.c')
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
ANCHORS={'D_8014A108':0x8014A108,'D_803BA028':0x803BA028,'D_803AF9A8':0x803AF9A8,'D_803B97A0':0x803B97A0,'D_803B97A4':0x803B97A4,'D_80150B70':0x80150B70,'D_803B9FD0':0x803B9FD0,'D_80111611':0x80111611,'D_80111655':0x80111655,'D_80111699':0x80111699,'D_803B9B60':0x803B9B60,'D_801226C0':0x801226C0,'input_new_data_wrapper':0x80094F88,'brake_light_update':0x800A6244,'Input_ApplyPadConfig':0x80094EC8}
HELPERS={'sound_control':[0x800B37E8,468,'56fb7406e5ae20cf6e85f574d6bc988e5e5d6dd694cfee5e7ef20c1ed73daad9'],'brake_light_update':[0x800A6244,448,'2cc57eb276b9927306fb036ea25b9a090ab5f08a0e4dab6a093f0c16a90ba8ed'],'input_new_data_wrapper':[0x80094F88,60,'51bff8e44879d93b2c44f11fea4c78e7a725283b496a06d94290f1793f4763b3'],'Input_ApplyPadConfig':[0x80094EC8,192,'3282399bd7919e39047f452faa4070df2ef7b4b91d82318591c2592e7bfda590']}

def sha(b):return hashlib.sha256(b).hexdigest()
def shell(*args):
 p=subprocess.run(list(map(str,args)),capture_output=True,text=True);assert p.returncode==0,(args,p.stdout,p.stderr);return p.stdout

def scorer(tools,repo,image):
 # Fresh namespace per protected image. No mutable scorer cache is shared.
 name='score_'+image;sys.path.insert(0,str(tools/'tools/cloud'))
 spec=importlib.util.spec_from_file_location(name,tools/'tools/cloud/score.py');s=importlib.util.module_from_spec(spec);sys.modules[name]=s;spec.loader.exec_module(s)
 s.ASM_DIR=repo/'asm/us'/image
 assert s._targets is None and not s._own_data
 return s

def inspect(path,linked=False,strict=True):
 sections,symbols,relocs=elf(path);index,row,text=sections['.text']
 funcs={n:s for n,s in symbols.items() if s[2]==2 and s[3] not in (0,0xfff1)}
 assert set(funcs)=={NAME},funcs
 v,size,typ,section=funcs[NAME];assert section==index and size>0 and size%4==0
 base=ENTRY if linked else 0;assert v==base and row[3]==base
 assert len(text)>=size and not any(text[size:]),'nonzero code beyond complete function'
 for name,(_,r,raw) in sections.items():
  if r[2]&2 and name not in ('.text','.reginfo','.options'):assert r[5]==0,('allocated data',name,r[5])
 if strict:assert size==len(text)==SIZE
 if linked:
  assert not relocs
  for n,a in ANCHORS.items():assert symbols[n][0]==a and symbols[n][3]==0xfff1,n
 else:
  if strict:assert len(relocs)==33
  assert all(off<size and off%4==0 and kind in (4,5,6) and n in ANCHORS and target==index for off,kind,n,target in relocs)
 return text,size,relocs

def comparable(receipt):
 # Manifest/tool hashes and host/linker versions are dated provenance; actual current integrity,
 # source, native bodies, addresses, ELF/GNU and behavior gates still rerun.
 result=json.loads(json.dumps(receipt))
 for key in ('protected_image_manifest','protected_blob_manifest','tool_sha256','gnu_ld_version','host_gcc_version'):
  result.pop(key,None)
 return result

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,default=ROOT);ap.add_argument('--tools-repo',type=Path,default=ROOT);ap.add_argument('--check',action='store_true');ap.add_argument('--out',type=Path,default=HERE/'verification.json');a=ap.parse_args()
 repo=a.repo.resolve();tr=a.tools_repo.resolve();build=ROOT/'build/marker_color';build.mkdir(parents=True,exist_ok=True)
 tmp=build/'tmp';tmp.mkdir(exist_ok=True);os.environ['TMPDIR']=str(tmp);tempfile.tempdir=str(tmp)
 score=scorer(tr,repo,'ovl_a');blob=scorer(tr,repo,'blob');manifest=score.target_manifest();bm=blob.target_manifest()
 nw=score.targets()[NAME];native=struct.pack('>%dI'%len(nw),*nw);assert len(native)==SIZE
 meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest));row=next(f for f in meta['functions'] if f['name']==NAME)
 assert row=={'name':NAME,'address':'0x803A3A6C','size':816,'evidence':['data_ref','prologue']}
 assert meta['image']=='A' and meta['image_sha256']=='0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
 addresses=score.image_symbols();assert addresses[NAME]==ENTRY
 for n,v in ANCHORS.items():assert addresses.get(n,score.address_named(n))==v,n
 bt=blob.targets();bs=blob.image_symbols()
 for n,(entry,size,digest) in HELPERS.items():
  b=struct.pack('>%dI'%len(bt[n]),*bt[n]);assert bs[n]==entry and len(b)==size and sha(b)==digest,n
 source_hash=sha(SOURCE.read_bytes());obj=build/'candidate.o';score.compile_single(SOURCE,FLAGS,obj)
 comparison=score.compare(obj,NAME,show=0);assert comparison.accepted(),comparison.__dict__
 raw,size,relocs=inspect(obj);words=list(struct.unpack('>%dI'%(len(raw)//4),raw))
 resolved,masks,unresolved,unverified,errors=score.relocate(obj,words,0,len(raw),addresses)
 assert not any((masks,unresolved,unverified,errors));assert struct.pack('>%dI'%len(resolved),*resolved)==native
 script=build/'whole.ld';script.write_text('SECTIONS { .text 0x803A3A6C : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'+''.join('%s = 0x%X;\n'%(n,v) for n,v in ANCHORS.items()))
 linked=build/'candidate.elf';shell('mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj);gnu,_,_=inspect(linked,True);assert gnu==native
 layout=build/'layout.c';checks={'blit_size':('sizeof(Blit)',48),'slot_size':('sizeof(MenuSlot)',64),'context_size':('sizeof(ProjectContext)',152),'color_size':('sizeof(Color)',4),'depth':('__builtin_offsetof(MenuSlot,state.depth)',8),'alpha':('__builtin_offsetof(MenuSlot,state.alpha)',60),'blit_x':('__builtin_offsetof(Blit,X)',14),'blit_y':('__builtin_offsetof(Blit,Y)',16),'blit_alpha':('__builtin_offsetof(Blit,Alpha)',24),'blit_hide':('__builtin_offsetof(Blit,Hide)',26),'blit_callback':('__builtin_offsetof(Blit,AnimFunc)',40),'blit_id':('__builtin_offsetof(Blit,AnimID)',44),'view_position':('__builtin_offsetof(ProjectContext,position)',36)}
 layout.write_text('#include "'+str(SOURCE)+'"\ntypedef char pointer32[sizeof(void*)==4?1:-1];\n'+''.join('typedef char %s[%s==%d?1:-1];\n'%(n,e,v) for n,(e,v) in checks.items()));shell('gcc','-m32','-std=c89','-fsyntax-only',layout)
 controls={}
 for opt in ('O1','O2','O3'):
  p=HERE/'controls/initial.c';o=build/('initial_'+opt+'.o');flags=FLAGS.replace('-O3','-'+opt);score.compile_single(p,flags,o)
  r=score.compare(o,NAME,show=0);text,extent,rels=inspect(o,strict=False)
  full,mask,ur,uv,err=score.relocate(o,list(struct.unpack('>%dI'%(len(text)//4),text)),0,len(text),addresses);assert not any((mask,ur,uv,err))
  controls[opt]={'source_sha256':sha(p.read_bytes()),'flags':flags,'comparison':r.__dict__,'function_bytes':extent,'text_bytes':len(text),'resolved_text_sha256':sha(struct.pack('>%dI'%len(full),*full)),'relocations':len(rels)}
 behavior,samples=semantics.verify(build,nw,resolved,list(struct.unpack('>204I',gnu)),bt['input_new_data_wrapper'])
 negatives={};text=SOURCE.read_text();changes={'wrong_width_division':('blt->Width/2','(blt->Width>>1)'),'wrong_player_nibble':('blt->AnimID&15','(blt->AnimID>>4)&15'),'wrong_hidden_flag':('if (D_803BA028[player])','if (D_803BA028[player]==1)'),'wrong_projection_slot':('&slot[19].matrix[12]','&slot[18].matrix[12]'),'wrong_second_palette':('palette=D_80111655[player]','palette=D_80111611[player]'),'wrong_alpha':('blt->Alpha=slot->state.alpha','blt->Alpha=1'),'missing_disable':('blt->AnimFunc=0;',';'),'wrong_alpha_channel':('D_803B9B60[player][channel].a=blt->Alpha','D_803B9B60[player][channel].a=255')}
 # Invalid helper pointers are contract failures and may abort the host; all
 # source mutants run in a child process, never the verifier process.
 worker=build/'mutant_host.py';worker.write_text('import sys\nfrom pathlib import Path\nsys.path.insert(0,'+repr(str(HERE))+')\nimport semantics\nfn=semantics.host(Path(sys.argv[1]),Path(sys.argv[2]),sys.argv[3])\nfor i,c in enumerate(semantics.cases()):\n if fn(c)!=semantics.oracle(c): print(i);sys.exit(3)\nsys.exit(0)\n')
 for label,(old,new) in changes.items():
  assert text.count(old)==1,(label,text.count(old));p=build/(label+'.c');p.write_text(text.replace(old,new));o=build/(label+'.o');score.compile_single(p,FLAGS,o);r=score.compare(o,NAME,show=0);assert not r.accepted()
  run=subprocess.run([sys.executable,str(worker),str(build),str(p),label],capture_output=True,text=True)
  assert run.returncode in (3,-6),('mutant not semantically rejected',label,run.returncode,run.stdout,run.stderr)
  negatives[label]={'strict_comparison':r.__dict__,'host_rejected':True,'host_exit':run.returncode,'first_mismatching_case':run.stdout.strip() or None}
 support=[HERE/'semantics.py',HERE/'host.c',HERE/'elf_support.py',Path(__file__).resolve()]
 receipt={'status':'MATCH','base':'dea99f09ab19b1d3b324ed7097162f7b378e7096','image':'A','function':NAME,'address':hex(ENTRY),'end':hex(ENTRY+SIZE),'bytes':SIZE,'words':204,'flags':FLAGS,'source_sha256':source_hash,'native_sha256':sha(native),'gnu_linked_body_sha256':sha(gnu),'complete_function_bytes':size,'text_bytes':len(raw),'alignment_bytes':0,'owned_data_bytes':0,'relocations':len(relocs),'anchors':{n:hex(v) for n,v in ANCHORS.items()},'comparison':comparison.__dict__,'layout_checks':{n:v for n,(_,v) in checks.items()},'controls':controls,'behavior':behavior,'negative_controls':negatives,'helper_native_bindings':HELPERS,'protected_image_manifest':manifest,'protected_blob_manifest':bm,'proof_sha256':{p.name:sha(p.read_bytes()) for p in support},'tool_sha256':{n:sha((tr/'tools/cloud'/n).read_bytes()) for n in ('score.py','owndata.py')},'compiler_sha256':{n:sha((score.IDO/n).read_bytes()) for n in ('cc','cfe','uopt','ugen','as1')},'gnu_ld_version':shell('mips-linux-gnu-ld','--version').splitlines()[0],'host_gcc_version':shell('gcc','--version').splitlines()[0],'accepted_bytes':0,'limits':['Projection-reaching descriptors: player 0..3, marker 7..9, selected car 0..12 and palette 0..15; synthetic valid disjoint backing.','Early rejection additionally tests player 4/15 and count signed16 endpoints; wider hidden-only marker values do not prove descriptor reachability.','Actual engine Blit extends beyond this prefix; UpdateBlit accesses +0x34.','External float pool contents and descriptor/asset data are unavailable; finite test thresholds are synthetic, not asserted retail values.','The exact protected Hidden helper executes; projection and UpdateBlit are explicit side-effecting O32 contract hooks, not complete graphics execution.','Signed integer to short narrowing follows target/compiler implementation-defined wrapping; no portable out-of-range signed narrowing claim.','No NaN/infinity/trap/FCSR, concurrency, whole-game, image, compression, ROM, hardware or accepted-coverage claim.']}
 receipt=json.loads(json.dumps(receipt));assert sha(SOURCE.read_bytes())==source_hash
 if a.check:assert comparable(json.loads(a.out.read_text()))==comparable(receipt),'portable canonical receipt differs'
 else:a.out.write_text(json.dumps(receipt,indent=2,sort_keys=True)+'\n')
 (build/'local_provenance.json').write_text(json.dumps({'object_sha256':sha(obj.read_bytes()),'linked_elf_sha256':sha(linked.read_bytes())},indent=2)+'\n')
 print(json.dumps({k:receipt[k] for k in ('status','bytes','relocations','behavior')},indent=2))
if __name__=='__main__':
 assert __debug__,'Python optimization is unsupported';main()
