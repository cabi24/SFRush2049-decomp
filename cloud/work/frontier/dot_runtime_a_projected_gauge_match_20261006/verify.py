#!/usr/bin/env python3
"""Complete strict-MATCH proof. No live integration-state or production-file pins."""
import argparse, hashlib, importlib.util, json, os, shutil, struct, subprocess, sys, tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
SOURCE=ROOT/'cloud/matches/ovl_a/func_803A3DA4.c'
def support(label,path):
 spec=importlib.util.spec_from_file_location(label,path)
 module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
 return module
elf=support('projected_gauge_match_elf',HERE/'elf_support.py').elf
behavior=support('projected_gauge_match_behavior',HERE/'behavior.py')
BASE='6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
NAME='func_803A3DA4';ENTRY=0x803A3DA4;SIZE=912
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
ANCHORS={
'D_8014A108':0x8014A108,'D_803BA028':0x803BA028,'D_803B9FD0':0x803B9FD0,
'D_803AF9A8':0x803AF9A8,'D_803B97A8':0x803B97A8,'D_803B97AC':0x803B97AC,
'D_803B97B0':0x803B97B0,'D_803B97B4':0x803B97B4,'D_803B97B8':0x803B97B8,
'D_80111310':0x80111310,'D_80111414':0x80111414,'D_80150B70':0x80150B70,
'input_new_data_wrapper':0x80094F88,'brake_light_update':0x800A6244,'Input_ApplyPadConfig':0x80094EC8}
NATIVE='e0b68bf004e8b9e88d2faf1fddabd1f9e48029b6438bb18557601a6158f5a407'

def sha(b):return hashlib.sha256(b).hexdigest()
def run(*args):
 p=subprocess.run([str(a) for a in args],capture_output=True,text=True)
 assert p.returncode==0,(args,p.stdout,p.stderr)
 return p.stdout

def load_score(root):
 sys.path.insert(0,str(root/'tools/cloud'))
 spec=importlib.util.spec_from_file_location('score',root/'tools/cloud/score.py')
 score=importlib.util.module_from_spec(spec);sys.modules['score']=score;spec.loader.exec_module(score)
 return score

def inspect(obj,linked=False):
 secs,syms,rels=elf(obj);idx,row,raw=secs['.text']
 funcs={k:v for k,v in syms.items() if v[2]==2 and v[3] not in (0,0xfff1)}
 assert funcs=={NAME:(ENTRY if linked else 0,SIZE,2,idx)},funcs
 assert len(raw)==SIZE and row[3]==(ENTRY if linked else 0)
 for name,(_,s,b) in secs.items():
  if s[2]&2 and name not in ('.text','.options','.reginfo'):assert s[5]==0,(name,s[5])
 if linked:
  assert not rels
  for n,a in ANCHORS.items():assert syms[n][0]==a and syms[n][3]==0xfff1
 else:
  assert len(rels)==31
  assert {n for n,s in syms.items() if s[3]==0}==set(ANCHORS)
  assert all(off%4==0 and off<SIZE and kind in (4,5,6) and symbol in ANCHORS and target==idx for off,kind,symbol,target in rels)
 return raw,rels

def check_match(native,compiled):
 assert len(native)==len(compiled)==228
 assert native==compiled,'complete native word mismatch'

def verify(root,build):
 score=load_score(root)
 if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
  return {'status':'SKIP','reason':'pinned IDO and MIPS GNU linker required'}
 build.mkdir(parents=True,exist_ok=True);tmp=build/'tmp';tmp.mkdir(exist_ok=True);os.environ['TMPDIR']=str(tmp);tempfile.tempdir=str(tmp)
 score.ASM_DIR=root/'asm/us/ovl_a';words=score.targets()[NAME];native=struct.pack('>228I',*words)
 assert sha(native)==NATIVE
 addresses=score.image_symbols()
 for n,v in ANCHORS.items():assert addresses.get(n,score.address_named(n))==v,n
 source=SOURCE;assert source.read_text().splitlines()[0]=='/* flags: '+FLAGS+' */'
 obj=build/'candidate.o';score.compile_single(source,FLAGS,obj)
 raw,rels=inspect(obj);result=score.compare(obj,NAME,show=0)
 assert result.differing==0 and result.total==228 and result.extra_words==0 and result.accepted()
 full=score.text_words(obj);relocated,masks,unresolved,unverified,errors=score.relocate(obj,full,0,SIZE,addresses)
 assert not any((masks,unresolved,unverified,errors))
 check_match(words,relocated)
 script=build/'whole.ld';script.write_text('SECTIONS { .text 0x803A3DA4 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'+''.join('%s = 0x%X;\n'%(n,v) for n,v in ANCHORS.items()))
 linked=build/'whole.elf';run('mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj);gnu,_=inspect(linked,True)
 assert gnu==struct.pack('>228I',*relocated)
 controls={}
 for p in sorted((HERE/'controls').glob('*.c')):
  out=build/(p.stem+'.o');score.compile_single(p,FLAGS,out);r=score.compare(out,NAME,show=0);assert not r.accepted();controls[p.name]=r.__dict__
 assert {k:v['differing'] for k,v in controls.items()}=={'named_slot_and_height.c':23,'direct_slot_only.c':4,'inline_quarter_only.c':27}
 fields={'X':14,'Y':16,'Width':20,'Height':22,'Alpha':24,'Hide':26,'Top':28,'Bot':30,'Left':32,'Right':34,'AnimFunc':40,'AnimID':44}
 layout=build/'layout.c';layout.write_text('#include "'+str(source)+'"\n'+''.join('typedef char check_%s[__builtin_offsetof(Blit,%s)==%d?1:-1];\n'%(n,n,v) for n,v in fields.items())+'typedef char slot_stride[sizeof(MenuSlot)==64?1:-1];\ntypedef char view_stride[sizeof(ProjectContext)==152?1:-1];\n')
 run('gcc','-m32','-std=c89','-fsyntax-only',layout)
 behavior_result=behavior.verify(build,source)
 mutants={}
 for label,old,new in [('wrong_height_division','blt->Y=position[1]-(blt->Height/4)/2;','blt->Y=position[1]-(blt->Height/8)/2;'),('wrong_marker_case','case 13:','case 12:'),('missing_disable','blt->AnimFunc=0;',';')]:
  assert source.read_text().count(old)==1
  mutant=build/(label+'.c');mutant.write_text(source.read_text().replace(old,new));host=behavior.host(build/label,mutant)
  failure=next((list(c) for c in behavior.cases() if host(c)!=behavior.oracle(c)),None);assert failure is not None,label
  mutants[label]={'rejected':True,'counterexample':failure}
 receipt={'status':'MATCH','base_commit':BASE,'image':'A','function':NAME,'extent':[ENTRY,ENTRY+SIZE],'function_bytes':SIZE,'alignment_bytes':0,'owned_data_bytes':0,'flags':FLAGS,'mandatory_backend_flag':score.R4300_CC,'native_sha256':NATIVE,'compiled_sha256':sha(gnu),'relocations':len(rels),'relocation_records':rels,'anchors':ANCHORS,'comparison':result.__dict__,'gnu_full_relocation_equals_native':True,'controls':controls,'behavior':behavior_result,'mutants':mutants,'layout':fields,'packet_sha256':{str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in [source,HERE/'verify.py',HERE/'host.c',HERE/'behavior.py',HERE/'elf_support.py',*sorted((HERE/'controls').glob('*.c'))]},'limits':['Strict standalone matching evidence; no lock, splice, or ROM-coverage action','No native instruction emulator or callback helper implementation executed','Host behavior uses finite representable float results and side-effecting hooks','Readonly external float constants remain symbolic; actual image data values are unproved','No original source spelling, translation-unit identity, unrestricted aliases, concurrency, renderer, gameplay, image, compression, or ROM gate']}
 return json.loads(json.dumps(receipt))

def main():
 if not __debug__:raise SystemExit('Python optimization is unsupported')
 ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,default=ROOT);ap.add_argument('--out',type=Path,default=HERE/'verification.json');ap.add_argument('--check',action='store_true');args=ap.parse_args()
 receipt=verify(args.repo.resolve(),args.repo.resolve()/'build/projected-gauge-match')
 if receipt['status']=='SKIP':print(json.dumps(receipt));return
 if args.check:assert json.loads(args.out.read_text())==receipt,'receipt mismatch'
 else:args.out.write_text(json.dumps(receipt,indent=2)+'\n')
 print(json.dumps({k:receipt[k] for k in ('status','function_bytes','relocations','comparison','behavior')},indent=2))
if __name__=='__main__':main()
