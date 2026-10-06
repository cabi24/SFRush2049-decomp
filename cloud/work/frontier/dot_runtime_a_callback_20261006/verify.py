#!/usr/bin/env python3
"""Fresh complete-body IDO/GNU proof and bounded callback-contract checks.

Use --repo for a separate complete checkout's protected targets and tooling.
Generated binaries stay in build/; no target bytes enter the JSON receipt.
"""
import argparse, ctypes, hashlib, importlib.util, json, os
from pathlib import Path
import struct, subprocess, sys

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
NAME='func_803A6A28'
ENTRY=0x803A6A28
SIZE=296
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
SOURCE=ROOT/'cloud/matches/ovl_a'/f'{NAME}.c'
# Independently decoded call/data addresses, compared to the protected symbol map.
ANCHORS={'render_helper':0x800B65B4,'object_create':0x800B42F0,
 'object_byte9_set':0x800B41C0,'mode_byte_set':0x800ED764,
 'func_800B669C':0x800B669C,'dispatch_handler':0x800B74A0,
 'camera_auto_follow':0x800BE078,'music_tempo_adjust':0x800BE9E8,
 'state_utility':0x800B71D4,'D_8017A4E4':0x8017A4E4,
 'D_80156978':0x80156978,'D_80156994':0x80156994,
 'D_803B87E8':0x803B87E8,'D_8002E440':0x8002E440,'D_8002E444':0x8002E444}


def portable_receipt(receipt):
    """Retain proof; omit whole-tree provenance and path-sensitive raw-object digest."""
    result = json.loads(json.dumps(receipt))
    for key in ('protected_targets', 'scorer_sha256', 'base', 'object_sha256'):
        result.pop(key, None)
    return result


def sha(b): return hashlib.sha256(b).hexdigest()
def sh(args):
 p=subprocess.run([str(x) for x in args],capture_output=True,text=True)
 assert p.returncode==0,(args,p.stdout,p.stderr)
 return p.stdout

def elf(path):
 """Independent ELF parser, not the production scorer's object reader."""
 b=path.read_bytes();assert b[:6]==b'\x7fELF\x01\x02'
 assert struct.unpack_from('>H',b,18)[0]==8
 at=struct.unpack_from('>I',b,32)[0]
 stride,n,ni=struct.unpack_from('>HHH',b,46);assert stride==40
 rows=[struct.unpack_from('>10I',b,at+i*stride) for i in range(n)]
 nr=rows[ni];names=b[nr[4]:nr[4]+nr[5]];sections={};symbols={};relocs=[]
 for i,r in enumerate(rows):
  name=names[r[0]:].split(b'\0')[0].decode();raw=b[r[4]:r[4]+r[5]]
  sections[name]=(i,r,raw)
  if r[1]==2:
   st=rows[r[6]];strings=b[st[4]:st[4]+st[5]]
   for off in range(0,len(raw),16):
    no,v,size,info,other,index=struct.unpack_from('>IIIBBH',raw,off)
    name=strings[no:].split(b'\0')[0].decode()
    if name:symbols[name]=(v,size,info&15,index)
  if r[1]==9:
   for off in range(0,len(raw),8):relocs.append(struct.unpack_from('>II',raw,off))
 return sections,symbols,relocs

def inspect(path,linked=False):
 sections,symbols,relocs=elf(path);i,r,raw=sections['.text']
 funcs={n:s for n,s in symbols.items() if s[2]==2 and s[3] not in (0,0xfff1)}
 assert set(funcs)=={NAME};v,size,typ,index=funcs[NAME]
 assert v==(ENTRY if linked else 0) and size==SIZE and index==i
 assert r[3]==(ENTRY if linked else 0) and len(raw)==304
 assert raw[SIZE:]==bytes(8)
 for name,(_,row,body) in sections.items():
  if row[2]&2 and name not in ('.text','.options','.reginfo'):assert row[5]==0,name
 if linked:
  assert not relocs
  for name,address in ANCHORS.items():assert symbols[name][0]==address and symbols[name][3]==0xfff1
 return raw,relocs

def oracle(flags,enabled,hook):
 out=[];state=[flags,enabled,0,-2147483648,23]
 def emit(*args):
  out.append(tuple(args)+(0,)*(8-len(args)))
  if len(out)==hook:
   state[:]=[state[0]^0x3c000,int(not state[1]),1,-17,2147483647]
 emit(1,0);emit(2,0);emit(3,0);emit(4,2);emit(5,1,1);emit(6,1)
 emit(7,160,165,288,110,-1,0,921+2*state[2])
 if state[0]==0x3c000:
  emit(6,14);emit(8,210,30,100,state[3],state[4])
 if state[1]:
  emit(6,14);emit(9,160,10,920+2*state[2])
 emit(5,0,3);emit(3,1);emit(4,-1);emit(1,-1)
 return out

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,default=ROOT);ap.add_argument('--out',type=Path,default=HERE/'verification.json');a=ap.parse_args()
 repo=a.repo.resolve();build=ROOT/'build/runtime_a_callback';build.mkdir(parents=True,exist_ok=True)
 tmp=build/'tmp';tmp.mkdir(exist_ok=True);os.environ['TMPDIR']=str(tmp)
 import tempfile;tempfile.tempdir=str(tmp)
 sys.path.insert(0,str(repo/'tools/cloud'))
 spec=importlib.util.spec_from_file_location('score',repo/'tools/cloud/score.py');score=importlib.util.module_from_spec(spec);sys.modules['score']=score;spec.loader.exec_module(score)
 score.ASM_DIR=repo/'asm/us/ovl_a';manifest=score.target_manifest();words=score.targets()[NAME]
 target=struct.pack('>%dI'%len(words),*words)
 assert sha(target)=='8ae1fb8d99e88873a762aa8ebd26f86cd745f8b22344cd10ac454280ef772bef'
 meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))
 row=next(x for x in meta['functions'] if x['name']==NAME)
 assert meta['image']=='A' and meta['base']=='0x8038A400'
 assert int(row['address'],16)==ENTRY and row['size']==SIZE
 addresses=score.image_symbols()
 assert addresses[NAME]==ENTRY
 for name,address in ANCHORS.items():
  assert addresses.get(name,score.address_named(name))==address,(name,'address drift')
 obj=build/'candidate.o';score.compile_single(SOURCE,FLAGS,obj)
 comparison=score.compare(obj,NAME,show=0);assert comparison.accepted(),comparison.summary()
 raw,relocs=inspect(obj);full=score.text_words(obj)
 resolved,masks,unresolved,unverified,errors=score.relocate(obj,full,0,len(full)*4,addresses)
 assert not any((masks,unresolved,unverified,errors))
 assert struct.pack('>%dI'%len(resolved),*resolved)==target+bytes(8)
 script=build/'whole.ld';script.write_text('SECTIONS { .text 0x803A6A28 : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) } }\n'+''.join('%s = 0x%X;\n'%(n,v) for n,v in ANCHORS.items()))
 linked=build/'candidate.elf';sh(['mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj]);linkedraw,_=inspect(linked,True);assert linkedraw==target+bytes(8)
 # Source declarations/record layout checked under an actual 32-bit C ABI.
 layout=build/'layout.c';layout.write_text('#include "'+str(SOURCE)+'"\n'+'typedef char p[sizeof(void*)==4?1:-1];\ntypedef char a[__builtin_offsetof(MenuText,text920)==920?1:-1];\ntypedef char b[__builtin_offsetof(MenuText,text936)==936?1:-1];\n')
 sh(['gcc','-m32','-std=c89','-fsyntax-only',layout])
 host=build/'host.so';sh(['gcc','-std=c89','-O2','-fstrict-aliasing','-shared','-fPIC','-fsanitize=undefined,bounds','-fno-sanitize-recover=all',HERE/'host.c','-o',host]);lib=ctypes.CDLL(str(host));lib.host_run.argtypes=[ctypes.c_uint,ctypes.c_int,ctypes.c_int,ctypes.POINTER(ctypes.c_int)]
 cases=0;tracehash=hashlib.sha256()
 for flags in (0,1,0x3bfff,0x3c000,0x3c001,0x8003c000,0xffffffff):
  for enabled in range(-128,128):
   for hook in (0,2,6,7,8,9,10):
    expected=oracle(flags,enabled,hook);out=(ctypes.c_int*256)();count=lib.host_run(flags,enabled,hook,out)
    actual=[tuple(out[i*8:i*8+8]) for i in range(count)]
    assert actual==expected,(flags,enabled,hook,actual,expected)
    tracehash.update(repr(actual).encode());cases+=1
 # A wrong optional-branch condition must fail the entire-body proof.
 mutant=build/'wrong_guard.c';mutant.write_text(SOURCE.read_text().replace('== 0x3C000','!= 0x3C000'))
 mutantobj=build/'wrong_guard.o';score.compile_single(mutant,FLAGS,mutantobj);wrong=score.compare(mutantobj,NAME,show=0);assert not wrong.accepted()
 receipt={'status':'MATCH','base':sh(['git','-C',ROOT,'rev-parse','HEAD']).strip(),'image':'A','address':hex(ENTRY),'end':hex(ENTRY+SIZE),'bytes':SIZE,'words':len(words),'flags':FLAGS+' -Wab,-r4300_mul','source_sha256':sha(SOURCE.read_bytes()),'verifier_sha256':sha((HERE/'verify.py').read_bytes()),'host_source_sha256':sha((HERE/'host.c').read_bytes()),'native_sha256':sha(target),'object_sha256':sha(obj.read_bytes()),'gnu_linked_body_sha256':sha(linkedraw[:SIZE]),'comparison':comparison.__dict__,'function_bytes':SIZE,'alignment_bytes':8,'owned_data_bytes':0,'relocations':len(relocs),'anchors':{n:hex(v) for n,v in ANCHORS.items()},'host_cases':cases,'host_trace_sha256':tracehash.hexdigest(),'host_scope':'C89 UBSan/bounds call traces against independent oracle, including external-call changes to later-observed globals; no actual renderer implementation execution','native_execution':'No native interpreter execution claimed; full-byte IDO/production/GNU identity proven','layout_checks':'32-bit pointer width and pointer fields at 920/936','negative_wrong_guard':wrong.__dict__,'tools':{p:sha((score.IDO/p).read_bytes()) for p in ('cc','cfe','uopt','ugen','as1')},'limits':['No broad repository suite','No image/compression/ROM/hardware gate','No cartridge coverage claim','No original typedef or TU-boundary recovery claim']}
 a.out.write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps({k:receipt[k] for k in ('status','bytes','comparison','host_cases','relocations','alignment_bytes','negative_wrong_guard')},indent=2))
if __name__=='__main__':main()
