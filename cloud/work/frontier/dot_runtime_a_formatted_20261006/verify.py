#!/usr/bin/env python3
"""Complete image-qualified ELF/GNU proof; bounded compiled-source contracts.

Native bytes are read only from manifest-verified protected repository inputs.
Receipts contain hashes/counts, never ROM bytes or raw instruction dumps.
"""
import argparse
import ctypes
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import struct
import subprocess
import sys

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
NAME='func_803AF744'
ENTRY=0x803AF744
SIZE=564
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
SOURCE=ROOT/'cloud/matches/ovl_a'/ (NAME+'.c')
ANCHORS={'render_helper':0x800B65B4,'func_800B669C':0x800B669C,
 'object_create':0x800B42F0,'dispatch_handler':0x800B74A0,
 'func_800A3508':0x800A3508,'sprintf':0x80004990,
 'camera_auto_follow':0x800BE078,'object_bytes_sum_global':0x800B3F50,
 'state_utility':0x800B71D4,'D_8017A4E4':0x8017A4E4,
 'D_803B83A0':0x803B83A0,'D_803B839C':0x803B839C,'D_803B92B4':0x803B92B4}

HELPERS={'func_800A3508': {'address': '0x800a3508', 'bytes': 16, 'sha256': '032b72d14840ad38e3c3e11a9cad4f2d20d2d471d6380b1600dd4c4702d631d0'}, 'object_bytes_sum_global': {'address': '0x800b3f50', 'bytes': 84, 'sha256': '1f195feade67d0cbc0e3608addd9d498dce8ad0f3acd3e16431a453ecfbfe82f'}, 'state_utility': {'address': '0x800b71d4', 'bytes': 396, 'sha256': '636951ffa0efe5e074676c7f3f0456d1149fa0bb76caf89dcdf5331c916bf6e6'}, 'render_helper': {'address': '0x800b65b4', 'bytes': 232, 'sha256': '4dc690156b257d875500ce8913cdeb9906660b2fee02b08495dee63ca91a8410'}, 'func_800B669C': {'address': '0x800b669c', 'bytes': 20, 'sha256': 'b8afbed98b9ba3628a0f09bfd44fe0013653321610eb1ab113a5cc6dc1855fdb'}, 'object_create': {'address': '0x800b42f0', 'bytes': 112, 'sha256': 'faee709c3df1e073e647c2851a4c4ce19db047599daaa796882845fa345f6858'}, 'dispatch_handler': {'address': '0x800b74a0', 'bytes': 1028, 'sha256': '21a02928c539b8245d8f31d68e81a52c43a252f127da03437e864f108309bdd6'}, 'camera_auto_follow': {'address': '0x800be078', 'bytes': 1084, 'sha256': 'fdd2db8697b645e3bff91cf0e6351a20a280a69db979bb576fd0015516d9cf73'}}

def sha(b):return hashlib.sha256(b).hexdigest()
def shell(*args):
 p=subprocess.run([str(x) for x in args],capture_output=True,text=True)
 assert p.returncode==0,(args,p.stdout,p.stderr)
 return p.stdout

def elf(path):
 """Independent ELF32 big-endian section/symbol/relocation reader."""
 b=path.read_bytes();assert b[:6]==b'\x7fELF\x01\x02'
 assert struct.unpack_from('>H',b,18)[0]==8
 at=struct.unpack_from('>I',b,32)[0]
 stride,n,ni=struct.unpack_from('>HHH',b,46);assert stride==40
 rows=[struct.unpack_from('>10I',b,at+i*stride) for i in range(n)]
 nr=rows[ni];names=b[nr[4]:nr[4]+nr[5]];sections={};symbols={};tables={};relocs=[]
 for i,r in enumerate(rows):
  name=names[r[0]:].split(b'\0')[0].decode();raw=b[r[4]:r[4]+r[5]]
  sections[name]=(i,r,raw)
  if r[1]==2:
   st=rows[r[6]];strings=b[st[4]:st[4]+st[5]];table=[]
   for off in range(0,len(raw),16):
    no,v,size,info,other,index=struct.unpack_from('>IIIBBH',raw,off)
    name=strings[no:].split(b'\0')[0].decode();table.append(name)
    if name:symbols[name]=(v,size,info&15,index)
   tables[i]=table
 for r in rows:
  if r[1] in (4,9):
   assert r[1]==9
   for off in range(r[4],r[4]+r[5],8):
    loc,info=struct.unpack_from('>II',b,off)
    relocs.append((loc,info&255,tables[r[6]][info>>8],r[7]))
 return sections,symbols,relocs

def inspect(path,linked=False):
 sections,symbols,relocs=elf(path);i,r,raw=sections['.text']
 funcs={n:s for n,s in symbols.items() if s[2]==2 and s[3] not in (0,0xfff1)}
 assert set(funcs)=={NAME};v,size,typ,index=funcs[NAME]
 assert v==(ENTRY if linked else 0) and size==SIZE and index==i
 assert r[3]==(ENTRY if linked else 0) and len(raw)==576
 assert raw[SIZE:]==bytes(12)
 for name,(_,row,body) in sections.items():
  if row[2]&2 and name not in ('.text','.options','.reginfo'):assert row[5]==0,name
 if linked:
  assert not relocs
  for name,address in ANCHORS.items():assert symbols[name][0]==address and symbols[name][3]==0xfff1
 else:
  assert len(relocs)==44
  assert all(off<SIZE and off%4==0 and kind in (4,5,6) and symbol in ANCHORS and section==i for off,kind,symbol,section in relocs)
 return raw,relocs

def oracle(enabled,flag,height,hook,length):
 out=[];state=[enabled,flag,0]
 def emit(*args):
  out.append(tuple(args)+(0,)*(10-len(args)))
  if len(out)==hook:
   state[:]=[0 if state[0] else 1,0 if state[1] else -128,1]
 def token(i):return 100+state[2]*8+i
 emit(1,0);emit(2,1,1);emit(3,11);emit(4,1)
 emit(5,9000,9001,token(4 if state[0]>0 else 3),token(5),token(1),token(2),9,token(0))
 emit(6,160,90,300,220,-1,0,9000,length)
 emit(7,height);y=200-3*height
 emit(3,11 if state[1] else 10);emit(4,1 if state[1] else 22)
 emit(8,160,y-(0 if state[1] else 1),token(7))
 if state[0]>0:
  emit(3,10 if state[1] else 11);emit(4,22 if state[1] else 1)
  emit(8,160,y-(1 if state[1] else 0)+height,token(6))
 emit(2,0,3);emit(1,-1)
 return out

def host_check(path,cases):
 lib=ctypes.CDLL(str(path));lib.host_run.argtypes=[ctypes.c_int]*5+[ctypes.POINTER(ctypes.c_int)]
 digest=hashlib.sha256();count=0
 for enabled,flag,height,hook,length in cases:
  expected=oracle(enabled,flag,height,hook,length);out=(ctypes.c_int*320)()
  n=lib.host_run(enabled,flag,height,hook,length,out)
  actual=[tuple(out[i*10:i*10+10]) for i in range(n)]
  assert actual==expected,(enabled,flag,height,hook,length,actual,expected)
  digest.update(repr(actual).encode());count+=1
 return count,digest.hexdigest()

def cases():
 # Every signed flag byte, both enabled states and unsigned boundary bytes;
 # all byte-representable helper-sum boundaries, each external hook position,
 # and empty/small/full output buffers. Pure ceil helper is never mutated.
 for flag in range(-128,128):
  for enabled in (0,1,128,255):
   for height in (-128,-1,0,1,16,127,255,510,637):
    for hook in (0,1,3,4,5,6,7,8,9,10,11,12,13):
     length=(0,1,127,255)[(flag+enabled+height+hook)%4]
     yield enabled,flag,height,hook,length

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,default=ROOT);ap.add_argument('--tools-repo',type=Path,default=ROOT);ap.add_argument('--out',type=Path,default=HERE/'verification.json');ap.add_argument('--check',action='store_true');a=ap.parse_args()
 repo=a.repo.resolve();tr=a.tools_repo.resolve();build=ROOT/'build/runtime_a_formatted';build.mkdir(parents=True,exist_ok=True)
 tmp=build/'tmp';tmp.mkdir(exist_ok=True);os.environ['TMPDIR']=str(tmp)
 import tempfile;tempfile.tempdir=str(tmp)
 sys.path.insert(0,str(tr/'tools/cloud'))
 spec=importlib.util.spec_from_file_location('score',tr/'tools/cloud/score.py');score=importlib.util.module_from_spec(spec);sys.modules['score']=score;spec.loader.exec_module(score)
 score.ASM_DIR=repo/'asm/us/ovl_a';manifest=score.target_manifest();words=score.targets()[NAME]
 native=struct.pack('>%dI'%len(words),*words)
 assert len(native)==SIZE
 assert sha(native)==NATIVE_HASH
 meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))
 row=next(x for x in meta['functions'] if x['name']==NAME)
 assert meta['image']=='A' and meta['base']=='0x8038A400'
 assert row==dict(name=NAME,address='0x803AF744',size=SIZE,evidence=['data_ref','prologue'])
 addresses=score.image_symbols();assert addresses[NAME]==ENTRY
 for name,address in ANCHORS.items():assert addresses.get(name,score.address_named(name))==address,(name,'address drift')
 # Bind each actual helper body and entry used to justify the source contract.
 score.ASM_DIR=repo/'asm/us/blob';helper_manifest=score.target_manifest();helper_words=score.targets();helper_symbols=score.image_symbols()
 for name,pin in HELPERS.items():
  b=struct.pack('>%dI'%len(helper_words[name]),*helper_words[name])
  assert helper_symbols[name]==int(pin['address'],16) and len(b)==pin['bytes'] and sha(b)==pin['sha256'],name
 score.ASM_DIR=repo/'asm/us/ovl_a'
 obj=build/'candidate.o';score.compile_single(SOURCE,FLAGS,obj)
 comparison=score.compare(obj,NAME,show=0);assert comparison.accepted(),comparison.summary()
 raw,relocs=inspect(obj);full=score.text_words(obj)
 resolved,masks,unresolved,unverified,errors=score.relocate(obj,full,0,len(full)*4,addresses)
 assert not any((masks,unresolved,unverified,errors))
 assert struct.pack('>%dI'%len(resolved),*resolved)==native+bytes(12)
 script=build/'whole.ld';script.write_text('SECTIONS { .text 0x803AF744 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'+''.join('%s = 0x%X;\n'%(n,v) for n,v in ANCHORS.items()))
 linked=build/'candidate.elf';shell('mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj);linkedraw,_=inspect(linked,True);assert linkedraw==native+bytes(12)
 layout=build/'layout.c';layout.write_text('#include "'+str(SOURCE)+'"\ntypedef char ptr[sizeof(void*)==4?1:-1];\n'+''.join('typedef char f%d[__builtin_offsetof(MenuText,text%d)==%d?1:-1];\n'%(n,n,n) for n in (344,352,360,608,612,616,624,944)))
 shell('gcc','-m32','-std=c89','-fsyntax-only',layout)
 host=build/'host.so';hostflags=['gcc','-std=c89','-O2','-fstrict-aliasing','-shared','-fPIC','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-fstack-protector-all']
 shell(*hostflags,HERE/'host.c','-o',host)
 count,tracehash=host_check(host,cases())
 negatives={}
 mutants={'invert_optional':'if (D_803B83A0 > 0)|if (D_803B83A0 == 0)',
 'wrong_vertical_spacing':'height * 3|height * 2',
 'wrong_field':'D_8017A4E4->text624);|D_8017A4E4->text608);'}
 for label,change in mutants.items():
  old,new=change.split('|');assert old in SOURCE.read_text()
  mutant=build/(label+'.c');mutant.write_text(SOURCE.read_text().replace(old,new));mo=build/(label+'.o');score.compile_single(mutant,FLAGS,mo);result=score.compare(mo,NAME,show=0);assert not result.accepted()
  harness=build/(label+'_host.c');harness.write_text((HERE/'host.c').read_text().replace('"../../../matches/ovl_a/func_803AF744.c"','"'+str(mutant)+'"'));so=build/(label+'.so');shell(*hostflags,harness,'-o',so)
  rejected=False
  try:host_check(so,[(1,1,16,0,255),(0,0,16,0,0)])
  except AssertionError:rejected=True
  assert rejected,label
  negatives[label]={'strict_match':False,'comparison':result.__dict__,'host_oracle_rejected':True}
 receipt={'status':'MATCH','image':'A','address':hex(ENTRY),'end':hex(ENTRY+SIZE),'bytes':SIZE,'words':len(words),'flags':FLAGS,'source_sha256':sha(SOURCE.read_bytes()),'host_source_sha256':sha((HERE/'host.c').read_bytes()),'native_sha256':sha(native),'gnu_linked_body_sha256':sha(linkedraw[:SIZE]),'comparison':comparison.__dict__,'function_bytes':SIZE,'alignment_bytes':12,'owned_data_bytes':0,'relocations':len(relocs),'anchors':{n:hex(v) for n,v in ANCHORS.items()},'host_cases':count,'host_trace_sha256':tracehash,'host_scope':'C89 UBSan/bounds bounded helper call traces with between-call global mutation and output lengths 0/1/127/255; authentic pure ceil helper; height [-128,637]. External renderer/formatter internals are contract hooks. Native execution is separately independently reviewed.','layout_checks':'32-bit pointers and all eight native MenuText field offsets','negative_controls':negatives,'protected_targets':manifest,'helper_native_bindings':HELPERS,'helper_protected_targets':helper_manifest,'scorer_sha256':sha((tr/'tools/cloud/score.py').read_bytes()),'owndata_sha256':sha((tr/'tools/cloud/owndata.py').read_bytes()),'tools':{p:sha((score.IDO/p).read_bytes()) for p in ('cc','cfe','uopt','ugen','as1')},'limits':['Opaque format contents; no general sprintf overflow safety proof','Bounded helper-contract test, not actual renderer execution','No broad suite or image/compression/ROM/hardware gate','No original typedef/source-unit recovery','Zero newly accepted bytes or cartridge coverage']}
 # Compare JSON to JSON: scorer notes are tuples in memory and arrays on disk.
 receipt=json.loads(json.dumps(receipt))
 if a.check:assert json.loads(a.out.read_text())==receipt,'portable receipt differs'
 else:a.out.write_text(json.dumps(receipt,indent=2)+'\n')
 (build/'local_provenance.json').write_text(json.dumps({'object_sha256':sha(obj.read_bytes()),'gcc':shell('gcc','--version').splitlines()[0],'gnu_ld':shell('mips-linux-gnu-ld','--version').splitlines()[0]},indent=2)+'\n')
 print(json.dumps({k:receipt[k] for k in ('status','bytes','comparison','host_cases','relocations','alignment_bytes','negative_controls')},indent=2))

NATIVE_HASH='ed1671be9fc34041ba730588e6f535f411fcff98631a3ea959a9c65f7e9ee164'
if __name__=='__main__':main()
