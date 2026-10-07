#!/usr/bin/env python3
"""Standalone full-body, complete relocation, GNU placement and host proof."""
if not __debug__:
 raise SystemExit("Python optimization is unsupported")
import argparse,hashlib,importlib.util,json,os,struct,subprocess,sys
from pathlib import Path
from elf_support import elf
import semantics
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
NAME='func_803AE940';ENTRY=0x803AE940;SIZE=900
SOURCE=ROOT/'cloud/matches/ovl_a'/ (NAME+'.c')
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
ANCHORS={'D_803B7714':0x803B7714,'D_803BA878':0x803BA878,'D_803BA898':0x803BA898,'D_803BA908':0x803BA908,'D_803B8294':0x803B8294,'D_803B9224':0x803B9224,'D_803BA85A':0x803BA85A,'D_803BA8B0':0x803BA8B0,'D_8017A4E0':0x8017A4E0,'D_80140BF0':0x80140BF0,'input_new_data_wrapper':0x80094F88,'func_800EF5B0':0x800EF5B0,'func_800BEA30':0x800BEA30,'func_800B42F0':0x800B42F0,'func_800B3FA4':0x800B3FA4,'Input_ApplyPadConfig':0x80094EC8}
def sha(b):return hashlib.sha256(b).hexdigest()
def shell(*args):
 p=subprocess.run(list(map(str,args)),capture_output=True,text=True);assert p.returncode==0,(args,p.stdout,p.stderr);return p.stdout

def inspect(path,linked=False):
 sections,symbols,relocs=elf(path);index,row,raw=sections['.text']
 funcs={n:s for n,s in symbols.items() if s[2]==2 and s[3] not in (0,0xfff1)}
 assert set(funcs)=={NAME};assert funcs[NAME]==(ENTRY if linked else 0,SIZE,2,index)
 assert row[3]==(ENTRY if linked else 0) and len(raw)==912 and raw[SIZE:]==bytes(12)
 for name,(_,r,b) in sections.items():
  if r[2]&2 and name not in ('.text','.reginfo','.options'):assert r[5]==0,name
 if linked:
  assert not relocs
  for n,a in ANCHORS.items():assert symbols[n][0]==a and symbols[n][3]==0xfff1
 else:
  assert len(relocs)==35
  assert all(off<SIZE and off%4==0 and kind in (4,5,6) and symbol in ANCHORS and target==index for off,kind,symbol,target in relocs)
 return raw,relocs

PROVENANCE_KEYS={"protected_targets","scorer_sha256","tools"}
def stable_receipt(receipt):
 """Keep all semantic bindings; tolerate independently recorded tool metadata."""
 return {k:v for k,v in receipt.items() if k not in PROVENANCE_KEYS}

def check_receipt(saved,current):
 assert stable_receipt(saved)==stable_receipt(current),"portable semantic receipt differs"

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,default=ROOT);ap.add_argument('--tools-repo',type=Path,default=ROOT);ap.add_argument('--out',type=Path,default=HERE/'verification.json');ap.add_argument('--check',action='store_true');a=ap.parse_args()
 repo=a.repo.resolve();tr=a.tools_repo.resolve();build=ROOT/'build/list_control';build.mkdir(parents=True,exist_ok=True)
 tmp=build/'tmp';tmp.mkdir(exist_ok=True);os.environ['TMPDIR']=str(tmp)
 import tempfile;tempfile.tempdir=str(tmp)
 sys.path.insert(0,str(tr/'tools/cloud'));spec=importlib.util.spec_from_file_location('score',tr/'tools/cloud/score.py');score=importlib.util.module_from_spec(spec);sys.modules['score']=score;spec.loader.exec_module(score)
 score.ASM_DIR=repo/'asm/us/ovl_a';manifest=score.target_manifest();words=score.targets()[NAME]
 native=struct.pack('>%dI'%len(words),*words);assert len(native)==SIZE and sha(native)=='06cf3b3f92db2312ea382822691b45d872794700b16d06c31cfd6da3f7d7aef1'
 meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest));row=next(x for x in meta['functions'] if x['name']==NAME)
 assert meta['image']=='A' and meta['base']=='0x8038A400'
 assert row==dict(name=NAME,address='0x803AE940',size=SIZE,evidence=['data_ref','prologue'])
 addresses=score.image_symbols();assert addresses[NAME]==ENTRY
 for n,v in ANCHORS.items():assert addresses.get(n,score.address_named(n))==v,n
 obj=build/'candidate.o';score.compile_single(SOURCE,FLAGS,obj);comparison=score.compare(obj,NAME,show=0);assert comparison.accepted(),comparison.summary()
 raw,relocs=inspect(obj);full=score.text_words(obj);resolved,masks,unresolved,unverified,errors=score.relocate(obj,full,0,len(full)*4,addresses)
 assert not any((masks,unresolved,unverified,errors));assert struct.pack('>%dI'%len(resolved),*resolved)==native+bytes(12)
 script=build/'whole.ld';script.write_text('SECTIONS { .text 0x803AE940 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'+''.join('%s = 0x%X;\n'%(n,v) for n,v in ANCHORS.items()))
 linked=build/'candidate.elf';shell('mips-linux-gnu-ld','-EB','--hash-style=sysv','-T',script,'-o',linked,obj);linkedraw,_=inspect(linked,True);assert linkedraw==native+bytes(12)
 layout=build/'layout.c';fields={'X':14,'Y':16,'Width':20,'Height':22,'Alpha':24,'Flip':25,'Hide':26,'AnimFunc':40,'AnimID':44,'Texture':52}
 layout.write_text('#include "'+str(SOURCE)+'"\ntypedef char ptr[sizeof(void*)==4?1:-1];\n'+''.join('typedef char f_%s[__builtin_offsetof(Blit,%s)==%d?1:-1];\n'%(n,n,v) for n,v in fields.items())+'typedef char resources[__builtin_offsetof(ResourceTables,indices)==12 && __builtin_offsetof(ResourceTables,strings)==16?1:-1];\ntypedef char index[__builtin_offsetof(ResourceIndices,first)==2?1:-1];\ntypedef char tex[sizeof(Texture)==32 && __builtin_offsetof(Texture,flags)==21?1:-1];\ntypedef char point[sizeof(Point)==4?1:-1];\n');shell('gcc','-m32','-std=c89','-fsyntax-only',layout)
 behavior,samples=semantics.verify(build,SOURCE)
 changes={'wrong_mode_gate':('D_803B7714 == 1','D_803B7714 != 0'),'wrong_scroll_boundary':('D_803BA898 - D_803BA878 < 5','D_803BA898 - D_803BA878 < 4'),'wrong_alpha_scale':('255.0f','254.0f'),'wrong_signed_half':('blt->Height / 2','(blt->Height >> 1)'),'wrong_row_spacing':(') * 13',') * 12'),'wrong_texture_flag':('flags |= 8','flags |= 4'),'missing_final_update':('    Input_ApplyPadConfig(blt);','    ;'),'wrong_return':('    return 1;\n}','    return 0;\n}')}
 negatives={};text=SOURCE.read_text()
 for label,(old,new) in changes.items():
  assert text.count(old)==1,(label,text.count(old));p=build/(label+'.c');p.write_text(text.replace(old,new));changed=semantics.host(build,p,label)
  failed=next((list(c) for c in samples if changed(c)!=semantics.oracle(c)),None);assert failed is not None,label
  mo=build/(label+'.o');score.compile_single(p,FLAGS,mo);result=score.compare(mo,NAME,show=0);assert not result.accepted()
  negatives[label]={'case':failed,'host_rejected':True,'strict_match':False,'comparison':result.__dict__}
 receipt={'status':'MATCH','kind':'standalone','base':'7e62ed7b3c3e6f1e788bf013419b89e132b4c65d','image':'A','address':hex(ENTRY),'end':hex(ENTRY+SIZE),'bytes':SIZE,'words':len(words),'flags':FLAGS,'source_sha256':sha(SOURCE.read_bytes()),'native_sha256':sha(native),'gnu_linked_body_sha256':sha(linkedraw[:SIZE]),'comparison':comparison.__dict__,'function_bytes':SIZE,'alignment_bytes':12,'owned_data_bytes':0,'relocations':len(relocs),'anchors':{n:hex(v) for n,v in ANCHORS.items()},'behavior':behavior,'layout_checks':fields,'negative_controls':negatives,'protected_targets':manifest,'support_sha256':{p.name:sha(p.read_bytes()) for p in (HERE/'host.c',HERE/'semantics.py',HERE/'elf_support.py')},'scorer_sha256':sha((tr/'tools/cloud/score.py').read_bytes()),'tools':{p:sha((score.IDO/p).read_bytes()) for p in ('cc','cfe','uopt','ugen','as1')},'limits':['Synthetic fixture backing; actual resource values/ranges and reachability not proved','Finite nonnegative representable float-to-u32 test domain; FCSR invalid conversion not modeled by host proof','Signed-halfword narrowing checked for target/GNU behavior, not portable C beyond signed16 range','Helpers are side-effecting O32 contract hooks in host proof, not full renderer execution','No image/compression/ROM/hardware gates or coverage acceptance']}
 receipt=json.loads(json.dumps(receipt))
 if a.check:check_receipt(json.loads(a.out.read_text()),receipt)
 else:a.out.write_text(json.dumps(receipt,indent=2)+'\n')
 (build/'local_provenance.json').write_text(json.dumps({'object_sha256':sha(obj.read_bytes()),'current_provenance':{k:receipt[k] for k in PROVENANCE_KEYS},'gcc':shell('gcc','--version').splitlines()[0],'gnu_ld':shell('mips-linux-gnu-ld','--version').splitlines()[0]},indent=2)+'\n')
 print(json.dumps({k:receipt[k] for k in ('status','bytes','relocations','comparison','behavior')},indent=2))
if __name__=='__main__':
 main()
