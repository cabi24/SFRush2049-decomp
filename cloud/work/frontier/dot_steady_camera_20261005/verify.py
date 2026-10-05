#!/usr/bin/env python3
"""Source-bound complete camera-group proof; no target payloads are published."""
import argparse, dataclasses, hashlib, json, os, struct, subprocess, sys, tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
FN='func_800E95DC'
CONTEXT=['func_800E92C8','func_800EA108','func_800EA2DC']
GROUP=ROOT/'src/blob/groups/func_800E92C8'
SUBMISSION=ROOT/'cloud/work/ipa-groups/dot_steady_camera_20261005'

def sha(b):return hashlib.sha256(b).hexdigest()
def packed(w):return struct.pack('>%dI'%len(w),*w)
def syms(obj):
 d,s=score._elf(obj)
 return [x for i,z in enumerate(s) if z['type']==2 for x in score._symbol_table(d,s,i)]
def symbol(obj,n):
 x,=[x for x in syms(obj) if x['name']==n and x['type']==2 and x['section']]
 return x

def body_bounds(source):
 a=source.index('void '+FN+'(',source.index('void func_800E92C8(Cr *car, f32 ab, f32 AB, f32 *camoff) {'))
 b=source.index('\nvoid func_800EA108(',a)
 return a,b

def make_group(directory,body=None):
 directory.mkdir(parents=True,exist_ok=True)
 source=(GROUP/'group.c').read_text();a,b=body_bounds(source)
 if body is None:body=(HERE/'candidate.c').read_text()
 (directory/'group.c').write_text(source[:a]+body+source[b:])
 (directory/'group.json').write_bytes((GROUP/'group.json').read_bytes())
 return directory

def registered_group(derived):
 """Bind the discoverable submission to unchanged accepted context plus this body."""
 assert (SUBMISSION/'group.c').read_bytes()==(derived/'group.c').read_bytes()
 expected=json.loads((GROUP/'group.json').read_text())
 submitted=json.loads((SUBMISSION/'group.json').read_text())
 assert submitted['members']==submitted['claims']==[FN]
 assert submitted['context']==CONTEXT
 for key in set(expected)|set(submitted):
  if key not in ('members','context','claims','provenance'):assert submitted.get(key)==expected.get(key),key
 return SUBMISSION

def inspect(obj,name):
 fn=symbol(obj,name);start=fn['value'];end=start+fn['size'];data,secs=score._elf(obj);ti=score._text_index(secs)
 words=score.text_words(obj);target=score.targets()[name];addresses=score.image_symbols()
 result=dataclasses.asdict(score.compare(obj,name,show=0))
 own=score.owndata.verify(obj,name,target,address=addresses[name],image=score.own_data(),start=start,addresses=lambda n:addresses.get(n,score.address_named(n)))
 assert own.ok,(name,own)
 relocs=[]
 for sec in secs:
  if sec['type']!=9 or sec['info']!=ti:continue
  table=score._symbol_table(data,secs,sec['link'])
  for off in range(sec['off'],sec['off']+sec['size'],8):
   site,info=struct.unpack_from('>II',data,off)
   if start<=site<end:relocs.append({'offset':site-start,'kind':info&255,'symbol':table[info>>8]['name']})
 next_offset=min([s['value'] for s in syms(obj) if s['type']==2 and s['section']==ti and s['value']>=end]+[len(words)*4])
 padding=words[end//4:next_offset//4];assert not any(padding)
 result.update(elf_function_bytes=fn['size'],native_bytes=4*len(target),zero_alignment_bytes=4*len(padding),relocations=relocs,
               own_placements=own.placements,body_sha256=sha(packed(target)))
 return result

def named_calls(obj,output):
 """Losslessly express section-relative JAL relocations using their real function symbols.

 GNU cannot place one monolithic input .text at four independent native addresses.
 This changes only the relocation representation, preserving section+addend value.
 """
 raw,secs=score._elf(obj);data=bytearray(raw);ti=score._text_index(secs);changes=[]
 for rel in secs:
  if rel['type']!=9 or rel['info']!=ti:continue
  table=score._symbol_table(raw,secs,rel['link'])
  for off in range(rel['off'],rel['off']+rel['size'],8):
   site,info=struct.unpack_from('>II',raw,off);symbol=table[info>>8]
   if (info&255)!=4 or symbol['type']!=3 or symbol['section']!=ti:continue
   word,=struct.unpack_from('>I',raw,secs[ti]['off']+site);target=(word&0x3ffffff)*4
   owner,=[(i,x) for i,x in enumerate(table) if x['type']==2 and x['section']==ti and x['value']<=target<x['value']+x['size']]
   index,fn=owner;addend=target-fn['value'];assert fn['value']+addend==target
   struct.pack_into('>I',data,off+4,(index<<8)|4)
   struct.pack_into('>I',data,secs[ti]['off']+site,(word&0xfc000000)|(addend//4))
   changes.append({'site':site,'owner':fn['name'],'owner_offset':fn['value'],'relative_addend':addend})
 output.write_bytes(data)
 return changes

def gnu(obj,directory,name):
 """Apply every MIPS relocation with GNU, assigning only the audited function's owned literal section."""
 directory.mkdir(exist_ok=True)
 addr=score.image_symbols();fn=symbol(obj,name);data,secs=score._elf(obj)
 own=score.owndata.verify(obj,name,score.targets()[name],address=addr[name],image=score.own_data(),start=fn['value'],addresses=lambda n:addr.get(n,score.address_named(n)))
 assert own.ok
 bases=own.bases()
 # All four original function symbols retain their native identities for calls.
 definitions=[]
 for sym in syms(obj):
  n=sym['name']
  if not n or sym['type']==3:continue
  if sym['section']==0 or (sym['type']==2 and n in addr):
   value=addr.get(n,score.address_named(n))
   if value is None:continue  # intrinsic-only undefined declaration has no relocation
   definitions.append('%s = 0x%x;'%(n,value))
 named_obj=directory/'named.o';changes=named_calls(obj,named_obj)
 script='SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } '%(addr[name]-fn['value'])
 for section,base in bases.items():script+='%s 0x%x : SUBALIGN(4) { *(%s) } '%(section,base,section)
 script+='}\n'+'\n'.join(definitions)+'\n'
 (directory/'proof.ld').write_text(script)
 subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(directory/'proof.ld'),'-o',str(directory/'proof.elf'),str(named_obj)],check=True,capture_output=True)
 subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(directory/'proof.elf'),str(directory/'proof.bin')],check=True,capture_output=True)
 raw=(directory/'proof.bin').read_bytes();body=raw[fn['value']:fn['value']+fn['size']]
 assert body==packed(score.targets()[name]),name
 return {'all_function_bytes_equal':True,'body_sha256':sha(body),'equivalent_named_call_relocations':changes},list(struct.unpack('>%dI'%(len(body)//4),body))

def verify(directory):
 candidate=(HERE/'candidate.c').read_text();g=make_group(directory/'group',candidate);obj=directory/'group.o';score.compile_group(registered_group(g),obj)
 result={'status':'MATCH','claims':[FN],'accepted_byte_gain':0,'base':'e0e734babdac3c6a79d2f87f7f895e34aa170148',
         'flags':json.loads((GROUP/'group.json').read_text())['flags'],'inputs':{str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in [GROUP/'group.c',GROUP/'group.json',HERE/'candidate.c',SUBMISSION/'group.c',SUBMISSION/'group.json']},
         'group':{},'gnu':{},'source_changes':'Only the actual E95DC function body is replaced. All declarations, three accepted bodies and compiler/keep recipe stay unchanged.'}
 for name in [FN]+CONTEXT:
  r=inspect(obj,name);assert score.compare(obj,name,show=0).accepted();assert r['elf_function_bytes']==r['native_bytes'];result['group'][name]=r
  result['gnu'][name],words=gnu(obj,directory/name,name)
  if name==FN:linked=words
 result['source_sha256']={p.name:sha(p.read_bytes()) for p in HERE.iterdir() if p.suffix in ('.c','.py')}
 result['compiler_sha256']={n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']}
 data,sections=score._elf(obj)
 result['elf']={'class':data[4],'encoding':data[5],'type':struct.unpack_from('>H',data,16)[0],'machine':struct.unpack_from('>H',data,18)[0],'flags':struct.unpack_from('>I',data,36)[0],
                'non_debug_sections':{s['name']:{'bytes':s['size'],'sha256':sha(data[s['off']:s['off']+s['size']])} for s in sections if s['name'] and s['name']!='.mdebug'}}
 result['target_manifest_sha256']=sha((score.ASM_DIR/'SHA256SUMS').read_bytes())
 targets=score.targets();addresses=score.image_symbols()
 result['native_callers']=[{'caller':n,'site':hex(addresses[n]+4*i)} for n,ws in targets.items() for i,w in enumerate(ws) if w>>26==3 and ((addresses[n]&0xf0000000)|((w&0x3ffffff)*4))==addresses[FN]]
 result['native_callees']=[hex(((addresses[FN]&0xf0000000)|((w&0x3ffffff)*4))) for w in targets[FN] if w>>26==3]
 import steady_camera_experiments as experiments, steady_camera_semantics as semantics
 from steady_camera_native import REAL
 result['native_runtime_input_sha256']={n:sha(packed(targets[n])) for n in [FN]+REAL}
 result['controls']=experiments.verify(directory/'controls',sys.modules[__name__])
 result['semantics']=semantics.verify(directory,linked)
 result['semantics']['rejected_mutants']=semantics.negative_controls(directory/'mutants')
 assert result['semantics']['native_cfg_reachable_offsets']==result['semantics']['native_executed_offsets']
 return json.loads(json.dumps(result)),linked

if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--record',action='store_true');args=p.parse_args()
 with tempfile.TemporaryDirectory(prefix='steady-camera-') as d:result,_=verify(Path(d))
 if args.record:(HERE/'verification.json').write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
 else:
  saved=json.loads((HERE/'verification.json').read_text())
  # Manifest annotation churn is provenance; all selected bytes and source bindings still compare.
  for r in [saved,result]:r.pop('target_manifest_sha256')
  assert result==saved
 print(json.dumps(result,indent=2))
