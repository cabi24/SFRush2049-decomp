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

if not __debug__:
 raise RuntimeError("verification requires assertions; Python -O is unsupported")

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
NAME='func_803A4340'
ENTRY=0x803A4340
SIZE=428
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
SOURCE=HERE/'candidate.c'
ANCHORS={'render_helper': 2148230580, 'object_create': 2148221680, 'dispatch_handler': 2148234400, 'func_800B24EC': 2148213996, 'object_bytes_sum_global': 2148220752, 'state_utility': 2148233684, 'D_8014A108': 2148835592, 'D_8014A110': 2148835600, 'D_803BA028': 2151391272, 'D_80140BDC': 2148797404, 'D_803B85E8': 2151384552, 'D_8017A4E0': 2149033184}
HELPERS={'object_bytes_sum_global': {'address': '0x800b3f50', 'bytes': 84, 'sha256': '1f195feade67d0cbc0e3608addd9d498dce8ad0f3acd3e16431a453ecfbfe82f'}, 'state_utility': {'address': '0x800b71d4', 'bytes': 396, 'sha256': '636951ffa0efe5e074676c7f3f0456d1149fa0bb76caf89dcdf5331c916bf6e6'}, 'render_helper': {'address': '0x800b65b4', 'bytes': 232, 'sha256': '4dc690156b257d875500ce8913cdeb9906660b2fee02b08495dee63ca91a8410'}, 'object_create': {'address': '0x800b42f0', 'bytes': 112, 'sha256': 'faee709c3df1e073e647c2851a4c4ce19db047599daaa796882845fa345f6858'}, 'dispatch_handler': {'address': '0x800b74a0', 'bytes': 1028, 'sha256': '21a02928c539b8245d8f31d68e81a52c43a252f127da03437e864f108309bdd6'}, 'func_800B24EC': {'address': '0x800b24ec', 'bytes': 364, 'sha256': '22ccee42826816c61c8ad198ddb3767a6b5523f51d1c3b676284968d0661a517'}, 'sound_update_channel': {'address': '0x800b3d18', 'bytes': 488, 'sha256': '843f08a4e541babf1af03e56b8e6b066d220408edf658e142f404844369e515c'}}


def portable_receipt(receipt):
    """Compare packet proof; base-context and whole-tree digests are provenance."""
    result = json.loads(json.dumps(receipt))
    for key in ('protected_targets', 'scorer_sha256', 'helper_protected_targets', 'owndata_sha256'):
        result.pop(key, None)
    return result


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
 assert r[3]==(ENTRY if linked else 0) and len(raw)==432
 assert raw[SIZE:]==bytes(4)
 for name,(_,row,body) in sections.items():
  if row[2]&2 and name not in ('.text','.options','.reginfo'):assert row[5]==0,name
 if linked:
  assert not relocs
  for name,address in ANCHORS.items():assert symbols[name][0]==address and symbols[name][3]==0xfff1
 else:
  assert len(relocs)==21
  assert all(off<SIZE and off%4==0 and kind in (4,5,6) and symbol in ANCHORS and section==i for off,kind,symbol,section in relocs)
 return raw,relocs

def signed(value,bits):
 value&=(1<<bits)-1
 return value-(1<<bits) if value&(1<<(bits-1)) else value

def oracle(count,flag,mode,height,font,tables,hook,replacement):
 out=[];state=[count,mode,tables,0,flag,2]
 def emit(kind,*args):
  out.append((kind,)+args+(0,)*(5-len(args))+tuple(state))
  if len(out)==hook and kind!=5:
   state[:]=[replacement,0 if state[1]==2 else 2,255-state[2],1,0 if state[4]==1 else 1,3]
 emit(1,0);emit(2,12);emit(3,1)
 i=0
 while i<state[0]:
  assert i<4
  if state[4]!=1:
   for j in range(4):
    if i<state[0] and state[0]==1 and state[1]!=2:
     emit(4,9000,0,signed(state[2]-1,8),1,1234)
     y=48+2*j*((height^(65535 if state[3] else 0))//8)
     emit(5,font)
     emit(6,10,signed(y-font,16),100+16*state[3]+state[5]+j)
  i+=1
 emit(1,-1)
 return out

def host_check(path,cases):
 lib=ctypes.CDLL(str(path));lib.host_run.argtypes=[ctypes.c_int]*8+[ctypes.POINTER(ctypes.c_int)]
 digest=hashlib.sha256();count=0
 for case in cases:
  expected=oracle(*case);out=(ctypes.c_int*384)()
  n=lib.host_run(*case,out)
  actual=[tuple(out[i*12:i*12+12]) for i in range(n)]
  assert actual==expected,(case,actual,expected)
  digest.update(repr(actual).encode());count+=1
 return count,digest.hexdigest()

def cases():
 heights=(0,1,7,8,255,32767,43688,43689,65534,65535)
 fonts=(-128,-1,0,1,127,510,637)
 tables=(0,1,127,128,129,255)
 counts=(-32768,-1,0,1,2,4)
 for flag in range(-128,128):
  for count in counts:
   for mode in (-1,0,2):
    for hook in range(17):
     n=flag+count+mode+hook
     yield count,flag,mode,heights[n%len(heights)],fonts[n%len(fonts)],tables[n%len(tables)],hook,counts[n%len(counts)]
 # Cross every height/font/table boundary with every hook, ensuring draw paths.
 for height in heights:
  for font in fonts:
   for table in tables:
    for hook in range(17):
     yield 1,0,0,height,font,table,hook,(-1,0,1,2,4)[hook%5]

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,default=ROOT);ap.add_argument('--tools-repo',type=Path,default=ROOT);ap.add_argument('--out',type=Path,default=HERE/'verification.json');ap.add_argument('--check',action='store_true');a=ap.parse_args()
 repo=a.repo.resolve();tr=a.tools_repo.resolve();build=ROOT/'build/runtime_a_menu_loop';build.mkdir(parents=True,exist_ok=True)
 tmp=build/'tmp';tmp.mkdir(exist_ok=True);os.environ['TMPDIR']=str(tmp)
 import tempfile;tempfile.tempdir=str(tmp)
 sys.path.insert(0,str(tr/'tools/cloud'))
 spec=importlib.util.spec_from_file_location('score',tr/'tools/cloud/score.py');score=importlib.util.module_from_spec(spec);sys.modules['score']=score;spec.loader.exec_module(score)
 score.ASM_DIR=repo/'asm/us/ovl_a';manifest=score.target_manifest();words=score.targets()[NAME]
 native=struct.pack('>%dI'%len(words),*words)
 assert len(native)==SIZE and sha(native)==NATIVE_HASH
 meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))
 row=next(x for x in meta['functions'] if x['name']==NAME)
 assert meta['image']=='A' and meta['base']=='0x8038A400'
 assert row==dict(name=NAME,address='0x803A4340',size=SIZE,evidence=['data_ref','prologue'])
 addresses=score.image_symbols();assert addresses[NAME]==ENTRY
 for name,address in ANCHORS.items():assert addresses.get(name,score.address_named(name))==address,(name,'address drift')
 score.ASM_DIR=repo/'asm/us/blob';helper_manifest=score.target_manifest();helper_words=score.targets();helper_symbols=score.image_symbols()
 for name,pin in HELPERS.items():
  b=struct.pack('>%dI'%len(helper_words[name]),*helper_words[name])
  assert helper_symbols[name]==int(pin['address'],16) and len(b)==pin['bytes'] and sha(b)==pin['sha256'],name
 score.ASM_DIR=repo/'asm/us/ovl_a'
 obj=build/'candidate.o';score.compile_single(SOURCE,FLAGS,obj)
 comparison=score.compare(obj,NAME,show=0);assert not comparison.accepted()
 raw,relocs=inspect(obj);full=score.text_words(obj)
 resolved,masks,unresolved,unverified,errors=score.relocate(obj,full,0,len(full)*4,addresses)
 assert not any((masks,unresolved,unverified,errors))
 resolved=struct.pack('>%dI'%len(resolved),*resolved)
 differences=[i for i in range(0,SIZE,4) if resolved[i:i+4]!=native[i:i+4]]
 assert differences==[0x70,0x7c,0x150,0x164]
 for offset in differences:
  a_word=struct.unpack_from('>I',native,offset)[0];b_word=struct.unpack_from('>I',resolved,offset)[0]
  assert a_word>>16==b_word>>16 and a_word&65535==76 and b_word&65535==80
 assert resolved[SIZE:]==bytes(4)
 script=build/'whole.ld';script.write_text('SECTIONS { .text 0x803A4340 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'+''.join('%s = 0x%X;\n'%(n,v) for n,v in ANCHORS.items()))
 linked=build/'candidate.elf';shell('mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj);linkedraw,_=inspect(linked,True);assert linkedraw==resolved
 layout=build/'layout.c';layout.write_text('#include "'+str(SOURCE)+'"\ntypedef char ptr[sizeof(void*)==4?1:-1];\ntypedef char texsize[sizeof(Texture)==36?1:-1];\n'+''.join('typedef char f%d[__builtin_offsetof(%s,%s)==%d?1:-1];\n'%(n,t,f,n) for t,f,n in [('Texture','height',18),('MenuIndices','first',26),('MenuData','indices',12),('MenuData','text',16)]))
 shell('gcc','-m32','-std=c89','-fsyntax-only',layout)
 host=build/'host.so';hostflags=['gcc','-std=c89','-O2','-fstrict-aliasing','-shared','-fPIC','-fsanitize=undefined,bounds','-fno-sanitize-recover=all','-fstack-protector-all']
 shell(*hostflags,HERE/'host.c','-o',host)
 count,tracehash=host_check(host,cases())
 negatives={}
 mutants={'invert_skip':'D_803BA028[i] == 1|D_803BA028[i] != 1',
 'wrong_mode':'D_8014A110 != 2|D_8014A110 == 2',
 'wrong_row_spacing':'texture->height / 8|texture->height / 4',
 'signed_texture_height':'u16 height|s16 height',
 'wrong_text_index':'indices->first + j|indices->first + j + 1'}
 for label,change in mutants.items():
  old,new=change.split('|');assert old in SOURCE.read_text()
  mutant=build/(label+'.c');mutant.write_text(SOURCE.read_text().replace(old,new));mo=build/(label+'.o');score.compile_single(mutant,FLAGS,mo);result=score.compare(mo,NAME,show=0);assert not result.accepted()
  harness=build/(label+'_host.c');harness.write_text((HERE/'host.c').read_text().replace('"candidate.c"','"'+str(mutant)+'"'));so=build/(label+'.so');shell(*hostflags,harness,'-o',so)
  rejected=False
  try:host_check(so,[(1,0,0,65535,-128,255,0,1),(1,1,2,32768,637,0,0,1)])
  except AssertionError:rejected=True
  assert rejected,label
  negatives[label]={'strict_match':False,'comparison':result.__dict__,'host_oracle_rejected':True}
 receipt={'status':'NONMATCH','image':'A','address':hex(ENTRY),'end':hex(ENTRY+SIZE),'bytes':SIZE,'words':len(words),'flags':FLAGS,'source_sha256':sha(SOURCE.read_bytes()),'verifier_sha256':sha((HERE/'verify.py').read_bytes()),'host_source_sha256':sha((HERE/'host.c').read_bytes()),'native_sha256':sha(native),'gnu_linked_body_sha256':sha(linkedraw[:SIZE]),'comparison':comparison.__dict__,'difference_offsets':[hex(x) for x in differences],'residual':'Only the cached flag-array pointer stack home: native +76, candidate +80; every other complete body word agrees.','function_bytes':SIZE,'alignment_bytes':4,'owned_data_bytes':0,'relocations':len(relocs),'anchors':{n:hex(v) for n,v in ANCHORS.items()},'host_cases':count,'host_trace_sha256':tracehash,'host_scope':'Unchanged C89 source with UBSan/bounds; valid four-flag storage, successful 36-byte texture lookup, u16 texture height, font return [-128,637], safe text arrays. External side effects bounded by fixtures; font helper may write disjoint renderer caches; menu state stays stable during that call. Native execution independently reviewed.','layout_checks':'32-bit pointers; 36-byte texture; native fields +18, +26, +12 and +16','negative_controls':negatives,'helper_native_bindings':HELPERS,'tools':{p:sha((score.IDO/p).read_bytes()) for p in ('cc','cfe','uopt','ugen','as1')},'limits':['Signed narrowings follow observed IDO and tested GCC low-bit behavior, not portable ISO C for out-of-range conversions','Lookup must succeed; full caller bounds and real renderer implementations unproved','Bounded tests, not unrestricted pointers, concurrency, gameplay, image/compression/ROM or hardware proof','Font helper writes renderer caches; no menu aliasing with these writes is proved beyond the stated disjoint-storage precondition','No original typedef/source-unit recovery','Zero matching, accepted-byte or cartridge coverage gain']}
 receipt=json.loads(json.dumps(receipt))
 if a.check:assert portable_receipt(json.loads(a.out.read_text()))==portable_receipt(receipt),'portable receipt differs'
 else:a.out.write_text(json.dumps(receipt,indent=2)+'\n')
 (build/'local_provenance.json').write_text(json.dumps({'object_sha256':sha(obj.read_bytes()),'gcc':shell('gcc','--version').splitlines()[0],'gnu_ld':shell('mips-linux-gnu-ld','--version').splitlines()[0]},indent=2)+'\n')
 print(json.dumps({k:receipt[k] for k in ('status','bytes','difference_offsets','host_cases','relocations','alignment_bytes')},indent=2))

NATIVE_HASH='1e306c7d79a18c4d7a008d658cf6abe17a5188817857ea3d4348ed8fe078ace7'
if __name__=='__main__':main()
