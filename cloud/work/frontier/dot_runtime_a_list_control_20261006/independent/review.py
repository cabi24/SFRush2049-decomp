#!/usr/bin/env python3
"""Independent full-body replay, native execution and source-bound controls.

No target words, object files or extracted data are embedded in this packet.
"""
import argparse,hashlib,importlib.util,json,shutil,struct,subprocess,sys
from pathlib import Path
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[4]
sys.path.insert(0,str(HERE.parent))
import semantics
from elf_support import elf
import native
NAME='func_803AE940';ENTRY=0x803AE940;SIZE=900
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
BASE='7e62ed7b3c3e6f1e788bf013419b89e132b4c65d'
def sha(b):return hashlib.sha256(b).hexdigest()
def run(*cmd):
 p=subprocess.run(list(map(str,cmd)),capture_output=True,text=True)
 assert p.returncode==0,(cmd,p.stdout,p.stderr)
 return p.stdout

PROVENANCE_ONLY={'base_commit','protected_targets','protected_helpers','compiler_sha256','scorer_sha256'}
def semantic_receipt(receipt):
 return {key:value for key,value in receipt.items() if key not in PROVENANCE_ONLY}

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,default=ROOT);ap.add_argument('--tools-repo',type=Path,default=ROOT);ap.add_argument('--check',action='store_true');ap.add_argument('--out',type=Path,default=HERE/'verification.json');args=ap.parse_args()
 for tool in ('gcc','mips-linux-gnu-ld','mips-linux-gnu-readelf'):
  if not shutil.which(tool):raise SystemExit('Required verification tool is unavailable: '+tool)
 repo=args.repo.resolve();tr=args.tools_repo.resolve();build=ROOT/'build/list_control_independent';build.mkdir(parents=True,exist_ok=True)
 source=ROOT/'cloud/matches/ovl_a'/ (NAME+'.c')
 sys.path.insert(0,str(tr/'tools/cloud'))
 spec=importlib.util.spec_from_file_location('score',tr/'tools/cloud/score.py');score=importlib.util.module_from_spec(spec);sys.modules['score']=score;spec.loader.exec_module(score)
 score.ASM_DIR=repo/'asm/us/ovl_a';manifest=score.target_manifest();words=score.targets()[NAME];target=struct.pack('>%dI'%len(words),*words);assert len(target)==SIZE
 metadata=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest));extent=next(e for e in metadata['functions'] if e['name']==NAME)
 assert extent==dict(name=NAME,address='0x803AE940',size=900,evidence=['data_ref','prologue'])
 assert metadata['image']=='A' and metadata['base']=='0x8038A400'
 obj=build/'fresh.o';score.compile_single(source,FLAGS,obj);comparison=score.compare(obj,NAME,show=0)
 assert comparison.differing==comparison.extra_words==0 and not any((comparison.unresolved,comparison.unverified,comparison.errors,comparison.notes))
 sec,syms,rel=elf(obj);text_index,info,text=sec['.text']
 assert syms[NAME]==(0,SIZE,2,text_index) and info[3]==0
 assert [n for n,s in syms.items() if s[2]==2 and s[3] not in (0,0xfff1)]==[NAME]
 assert len(text)==912 and text[SIZE:]==bytes(12)
 assert len(rel)==35 and all(a%4==0 and 0<=a<SIZE and k in (4,5,6) and index==text_index for a,k,n,index in rel)
 assert all(row[5]==0 for name,(_,row,b) in sec.items() if row[2]&2 and name not in ('.text','.reginfo','.options'))
 addresses=score.image_symbols();anchors={n:addresses.get(n,score.address_named(n)) for a,k,n,i in rel};assert None not in anchors.values()
 script=build/'whole.ld';script.write_text('SECTIONS { .text 0x803AE940 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'+''.join('%s = 0x%X;\n'%(n,a) for n,a in sorted(anchors.items())))
 linked=build/'fresh.elf';run('mips-linux-gnu-ld','--hash-style=sysv','-EB','-T',script,'-o',linked,obj)
 se,ss,rr=elf(linked);assert ss[NAME]==(ENTRY,SIZE,2,se['.text'][0]);assert se['.text'][1][3]==ENTRY
 assert se['.text'][2]==target+bytes(12) and not rr
 assert all(ss[n][0]==a and ss[n][3]==0xfff1 for n,a in anchors.items())
 assert all(row[5]==0 for name,(_,row,b) in se.items() if row[2]&2 and name!='.text')
 full=score.text_words(obj);resolved,masks,unres,unver,errors=score.relocate(obj,full,0,len(full)*4,addresses)
 assert not any((masks,unres,unver,errors));assert struct.pack('>%dI'%len(resolved),*resolved)==se['.text'][2]
 (build/'readelf.txt').write_text(run('mips-linux-gnu-readelf','-W','-S','-s','-r',obj,linked))
 score.ASM_DIR=repo/'asm/us/blob';helpers=score.targets();hs=score.image_symbols();hm=score.target_manifest()
 bindings={n:{'address':hex(hs[n]),'bytes':4*len(helpers[n]),'sha256':sha(struct.pack('>%dI'%len(helpers[n]),*helpers[n]))} for n in ('input_new_data_wrapper','Input_ApplyPadConfig','func_800EF5B0','func_800BEA30','object_create','object_manager_update','sound_control','UpdateActiveObjects')}
 hidden=helpers['input_new_data_wrapper'];assert hs['input_new_data_wrapper']==native.HIDDEN and len(hidden)==15
 # Callback APIs are checked against native fields/call slots, without replacing
 # them by an invented public/private calling convention.
 dispatcher={}
 for name,call_offset in (('UpdateActiveObjects',0x48),('sound_control',0x160)):
  ww=helpers[name];call=ww[call_offset//4]
  assert call>>26==0 and call&63==9 and (call>>11)&31==31
  assert ww[(call_offset+8)//4]>>26==5 and (ww[(call_offset+8)//4]>>21)&31==2
  dispatcher[name]={'call_offset':hex(call_offset),'argument':'a0 points to Blit','result':'v0 nonzero retains Blit; zero enters removal path','execution':'callsite inspected; complete dispatcher not executed'}
 engines=[native.Native(w,hidden) for w in (words,list(struct.unpack('>225I',se['.text'][2][:SIZE])))];host=semantics.host(build,source,'independent_host')
 samples=list(semantics.cases());original=len(samples)
 for selected in (0,1,15,31):
  for fade in (8421505.,8421506.,10000000.,16000000.):
   c=(1,0,0,15,0,-32768,32767,-128,selected,-32768,32767,semantics.fbits(fade),0,-128)
   assert 0<=semantics.f32(semantics.fvalue(c[11])*255.)<2**32;samples.append(c)
 digest=hashlib.sha256()
 for i,c in enumerate(samples):
  want=semantics.oracle(c)
  for label,e in zip(('native','GNU'),engines):assert e.run(c,i)==want,(label,i,c)
  assert host(c)==want,('host',i,c)
  digest.update(repr(want).encode())
 missing=sorted(set(range(ENTRY,ENTRY+SIZE,4))-engines[0].coverage)
 assert [a-ENTRY for a in missing]==[0x1e4,0x1e8,0x1ec]
 behavior={'cases':len(samples),'author_cases_replayed':original,'additional_unsigned_conversion_selection_cases':len(samples)-original,'native_executions':len(samples)*2,'target_instructions_executed':225-len(missing),'unexecuted_offsets':[hex(a-ENTRY) for a in missing],'hidden_instructions_executed':len(set(range(native.HIDDEN,native.HIDDEN+60,4))&engines[0].coverage),'branch_outcomes':[[hex(a),b] for a,b in sorted(engines[0].branches)],'trace_sha256':digest.hexdigest(),'abi':'Integer caller-save and f0-f19 registers poisoned at each hook. Preserved s0-s7, gp, sp, s8, ra and f20-f31 are checked; memory writes are confined. Exact native Hidden body executes.','routes':['protected native','GNU-linked fresh IDO','unchanged C89 with UBSan/bounds/float-cast-overflow','state oracle']}
 score.ASM_DIR=repo/'asm/us/ovl_a';source_text=source.read_text();negative={}
 for label,old,new in [('row_spacing','* 13','* 12'),('texture_flag','flags |= 8','flags |= 4'),('signed_half','blt->Height / 2','(blt->Height >> 1)'),('pulse_selector','blt->AnimID & 15','blt->AnimID & 14'),('return_value','    return 1;\n}','    return 0;\n}')]:
  assert source_text.count(old)==1
  p=build/('mutant_'+label+'.c');p.write_text(source_text.replace(old,new));o=p.with_suffix('.o');score.compile_single(p,FLAGS,o)
  comp=score.compare(o,NAME,show=0);assert not comp.accepted()
  ww=score.text_words(o);rw,mm,uu,vv,ee=score.relocate(o,ww,0,len(ww)*4,addresses);assert not any((mm,uu,vv,ee));size=elf(o)[1][NAME][1];engine=native.Native(rw[:size//4],hidden)
  witness=next((list(c) for c in samples if engine.run(c)!=semantics.oracle(c)),None);assert witness is not None,label
  negative[label]={'comparison':comp.__dict__,'native_counterexample':witness,'rejected_by':'native semantic trace differs','source_change':{'old':old,'new':new}}
 receipt={'status':'MATCH','base_commit':BASE,'source_sha256':sha(source.read_bytes()),'flags':FLAGS,'extent':extent,'population':{k:metadata[k] for k in ('image','base','size','image_sha256')},'comparison':comparison.__dict__,'text_bytes':912,'function_bytes':SIZE,'zero_alignment_bytes':12,'owned_data_bytes':0,'relocations':len(rel),'anchors':{n:hex(a) for n,a in anchors.items()},'native_sha256':sha(target),'gnu_body_sha256':sha(se['.text'][2][:SIZE]),'helper_bindings':bindings,'dispatcher_api':dispatcher,'behavior':behavior,'negative_controls':negative,'protected_targets':manifest,'protected_helpers':hm,'compiler_sha256':{n:sha((score.IDO/n).read_bytes()) for n in ('cc','cfe','uopt','ugen','as1')},'scorer_sha256':sha((tr/'tools/cloud/score.py').read_bytes()),'support_sha256':{p.name:sha(p.read_bytes()) for p in (HERE/'native.py',HERE/'review.py',HERE.parent/'host.c',HERE.parent/'semantics.py',HERE.parent/'elf_support.py')},'accepted_bytes':0,'limits':['Finite nonnegative float products below 2**32; trap-disabled round-to-nearest multiply and toward-zero conversion only. No NaN/Inf/general FCSR/trap proof.','Offsets +0x1e4/+0x1e8 are excluded invalid-conversion fallback; +0x1ec is structurally bypassed.','Synthetic disjoint tables: item 0..3, group/texture 0..15, selected 0..31. Actual retail data, bounds and reachability are unproved.','Signed-short narrowing follows target and tested GCC behavior, not portable out-of-range signed conversion.','External renderer/font/fade calls use effectful O32 hooks. Only Hidden executes its complete native helper body.','No exact arcade donor/original source-unit claim. No full image, compression, ROM, hardware or accepted coverage claim.']}
 receipt=json.loads(json.dumps(receipt))
 if args.check:assert semantic_receipt(json.loads(args.out.read_text()))==semantic_receipt(receipt),'independent semantic receipt differs'
 else:args.out.write_text(json.dumps(receipt,indent=2)+'\n')
 (build/'local_provenance.json').write_text(json.dumps({'gnu_ld':run('mips-linux-gnu-ld','--version').splitlines()[0],'gcc':run('gcc','--version').splitlines()[0],'object_sha256':sha(obj.read_bytes())},indent=2)+'\n')
 print(json.dumps({k:receipt[k] for k in ('status','source_sha256','comparison','function_bytes','zero_alignment_bytes','relocations')}));print(json.dumps({k:behavior[k] for k in ('cases','native_executions','target_instructions_executed','trace_sha256')}))
if __name__=='__main__':
 if not __debug__:raise SystemExit('Python optimization is unsupported: verification assertions must execute')
 main()
