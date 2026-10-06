#!/usr/bin/env python3
"""Audit an existing AF06C object and replay independent cases. Never compiles."""
import argparse,hashlib,json,os,re,struct,subprocess,sys
from pathlib import Path
HERE=Path(__file__).resolve().parent
_boot=argparse.ArgumentParser(add_help=False);_boot.add_argument('--packet-dir',type=Path,default=HERE.parent)
PACKET=_boot.parse_known_args()[0].packet_dir.resolve();sys.path.insert(0,str(PACKET))
from native import Machine
from compiled_machine import Machine as Compiled
from fixture import Fixture
from challenge import cases
from audit_contracts import load,BASE
SOURCE='65593ef2a077ce151f6ef43a5843593c302b8d5588c3cb47bb4f2ae51c440ee6'
def sha(raw):return hashlib.sha256(raw).hexdigest()

def elf(path,kind):
 raw=path.read_bytes();h=struct.unpack_from('>16sHHIIIIIHHHHHH',raw)
 assert h[0][:7]==b'\x7fELF\x01\x02\x01' and h[1]==kind and h[2]==8
 assert h[11]==40 and h[6]+h[11]*h[12]<=len(raw)
 sections=[]
 for i in range(h[12]):
  sh=struct.unpack_from('>10I',raw,h[6]+i*h[11]);sections.append(dict(zip(('n','type','flags','address','offset','size','link','info','align','entsize'),sh)))
 names=sections[h[13]];strings=raw[names['offset']:names['offset']+names['size']]
 for s in sections:
  s['name']=strings[s['n']:].split(b'\0')[0].decode();assert s['type']==8 or s['offset']+s['size']<=len(raw)
  s['data']=bytes(s['size']) if s['type']==8 else raw[s['offset']:s['offset']+s['size']]
 syms={};tables={}
 for i,s in enumerate(sections):
  if s['type']!=2:continue
  assert s['entsize']==16 and s['size']%16==0
  table=[];names=sections[s['link']]['data']
  for j in range(0,s['size'],16):
   n,v,size,info,other,idx=struct.unpack_from('>IIIBBH',s['data'],j)
   name=names[n:].split(b'\0')[0].decode();x=dict(name=name,value=v,size=size,type=info&15,bind=info>>4,section=idx)
   table.append(x)
   if name:assert name not in syms;syms[name]=x
  tables[i]=table
 relocs=[]
 for i,s in enumerate(sections):
  if s['type']==4:assert s['size']==0,'unexpected RELA'
  if s['type']!=9:continue
  assert s['entsize']==8 and s['size']%8==0
  for off,info in struct.iter_unpack('>II',s['data']):
   symbol=tables[s['link']][info>>8]
   relocs.append(dict(section=s['name'],target=s['info'],offset=off,type=info&255,symbol=symbol['name']))
 return raw,sections,syms,relocs

def object_audit(repo,obj,linked,base_receipt):
 ob,os,osy,rel=elf(obj,1);lb,ls,lsy,lrel=elf(linked,2)
 assert not lrel and all(s['section']!=0 for s in lsy.values()),'linked unresolved state'
 fn=osy['save_write_data'];lfunc=lsy['save_write_data']
 funcs={n:x for n,x in osy.items() if x['type']==2 and x['section'] not in (0,0xFFF1)}
 assert set(funcs)=={'save_write_data'} and 'func_80090308' not in lsy
 assert fn['value']==8 and fn['size']==2096 and lfunc['value']==0x81000008 and lfunc['size']==2096
 ot=os[fn['section']];lt=ls[lfunc['section']]
 assert ot['name']==lt['name']=='.text' and ot['size']==lt['size']==2112
 assert ot['data'][:8]==lt['data'][:8] and ot['data'][:4]==struct.pack('>I',0x03E00008) and ot['data'][4:8]==bytes(4)
 assert ot['data'][-8:]==lt['data'][-8:]==bytes(8)
 assert {(s['name'],s['size']) for s in os if s['flags']&2 and s['size']}=={('.text',2112),('.reginfo',24)}
 assert [(s['name'],s['size']) for s in ls if s['flags']&2 and s['size']]==[('.text',2112)]
 symbols=json.loads(subprocess.check_output(['git','-C',str(repo),'show',BASE+':asm/us/blob/symbols.json']))['symbols']
 bindings={}
 for r in rel:
  n=r['symbol'];assert r['target']==fn['section'] and r['type'] in (4,5,6)
  assert n in lsy and lsy[n]['section']==0xFFF1 and n in osy and osy[n]['section']==0
  target=int(n[2:],16) if re.fullmatch(r'D_[0-9A-Fa-f]{8}',n) else int(symbols[n],16)
  assert lsy[n]['value']==target
  bindings[n]=target
 expected=bytearray(ot['data']);pending={}
 def word(off):return int.from_bytes(ot['data'][off:off+4],'big')
 def write(off,w):expected[off:off+4]=w.to_bytes(4,'big')
 for r in rel:
  off,typ,n=r['offset'],r['type'],r['symbol'];w=word(off);s=bindings[n]
  assert off%4==0 and 0<=off<=len(expected)-4
  if typ==4:
   value=s+((w&0x3FFFFFF)<<2);assert value>>28==(lt['address']+off+4)>>28
   write(off,(w&0xFC000000)|((value>>2)&0x3FFFFFF))
  elif typ==5:pending.setdefault(n,[]).append(off)
  else:
   lo=w&65535;lo=lo-65536 if lo&32768 else lo
   assert n in pending and pending[n],('orphan LO16',n,off)
   for high in pending.pop(n):
    h=word(high);value=s+((h&65535)<<16)+lo
    write(high,(h&0xFFFF0000)|(((value+0x8000)>>16)&65535))
   write(off,(w&0xFFFF0000)|((s+lo)&65535))
 assert not pending and bytes(expected)==lt['data'],'full linked text differs from independently relocated object'
 assert sha(ob)==base_receipt['object']['sha256'] and sha(lb)==base_receipt['linked']['sha256']
 assert base_receipt['source_sha256']==SOURCE and base_receipt['base']==BASE
 assert base_receipt['flags']=='-g0 -O3 -mips2 -G 0 -non_shared' and base_receipt['mandatory_backend_flag']=='-Wab,-r4300_mul'
 assert base_receipt['target_compiler_invocations']==1
 for n,a in bindings.items():assert base_receipt['external_bindings'][n]==hex(a)
 assert len(bindings)==len(base_receipt['external_bindings'])
 actual_relocs=[dict(section=r['section'],offset=hex(r['offset']),type=r['type'],symbol=r['symbol']) for r in rel]
 assert actual_relocs==base_receipt['object']['relocations']
 code={lt['address']+4*i:w[0] for i,w in enumerate(struct.iter_unpack('>I',lt['data']))}
 raw_fn=lt['data'][8:8+2096]
 return code,lfunc['value'],dict(object_sha256=sha(ob),elf_sha256=sha(lb),source_sha256=SOURCE,
  root_bytes=2096,text_bytes=2112,prefix_bytes=8,tail_alignment_bytes=8,emitted_functions=['save_write_data'],
  private90308='naturally absent; body inlined',relocations_verified=len(rel),external_bindings_verified=len(bindings),
  unresolved_relocations=0,owned_data_bytes=0,owned_data_references=0,object_metadata_reginfo_bytes=24,
  complete_function_sha256=sha(raw_fn))

def run(repo,obj,linked):
 source=(PACKET/'candidate.c').read_bytes();assert sha(source)==SOURCE
 baseline=json.loads((PACKET/'baseline.json').read_text())
 compiled,entry,metadata=object_audit(repo,obj,linked,baseline)
 original,image,native=load(repo)
 native_words=[original[0x800AF06C+4*i] for i in range(300)]
 words=[compiled[entry+4*i] for i in range(2096//4)]
 difference=sum(x!=y for x,y in zip(words,native_words));extra=sum(x!=0 for x in words[300:])
 assert difference==298 and len(words[300:])==224 and extra==220
 canon=baseline['comparisons']['save_write_data']['canonical'];assert canon['differing']==difference and canon['extra_words']==extra
 coverage,branches=set(),set();passed=[]
 for name,kw in cases():
  a,b=Fixture(image,**kw),Fixture(image,**kw);Machine(original,a).run();n=Compiled(compiled,b,entry);n.run()
  assert a.trace==b.trace,('trace',name,a.trace,b.trace)
  sa,sb=a.memory.snapshot(),b.memory.snapshot();assert len(sa)==len(sb)
  for (aa,ad),(ba,bd) in zip(sa,sb):
   assert aa==ba
   if ad!=bd:
    i=next(i for i in range(len(ad)) if ad[i]!=bd[i]);raise AssertionError(('state',name,hex(aa+i),ad[i:i+16].hex(),bd[i:i+16].hex()))
  assert (a.created,a.alloc_count,a.node_count)==(b.created,b.alloc_count,b.node_count)
  coverage.update(n.coverage);branches.update(n.branches);passed.append(name)
 assert sha((PACKET/'candidate.c').read_bytes())==SOURCE
 return dict(status='PASS: bounded existing-object/native semantic agreement and complete ELF integrity; NOT STRICT MATCH',base=BASE,
  target_compiler_invocations=0,host_compiler_invocations=0,object=metadata,fixtures=len(passed),compiled_instructions_covered=len(coverage),branch_outcomes=len(branches),
  strict_comparison=dict(differing_native_words=difference,native_words=300,extra_words_total=224,extra_words_nonzero=extra,match=False),
  packet={n:sha((PACKET/n).read_bytes()) for n in ['candidate.c','native.py','fixture.py','compiled_machine.py']},
  native={n:native[n] for n in ['save_write_data','func_80090308']},
  receipt_honesty='Current source and historical native identities, object/ELF hashes, complete extents, every relocation and own-data accounting agree with the receipts and actual artifacts. Canonical flags and one-build count are recorded provenance consistent with build_once.py; this no-compilation audit does not independently reconstruct the compiler process history or regenerate the source-to-object mapping.',
  limitations=['Valid aligned initialized bounded storage and finite normal-or-zero binary32 under default rounding',
  'External services remain explicit effect models; no full scene/audio/gameplay or callback lifetime proof',
  'Private90308 native stack convention is preserved in reference execution; emitted helper is naturally inlined',
  'No original-TU admission, strict match, integration, ROM, publication or CI claim'])
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--packet-dir',type=Path,default=PACKET);p.add_argument('--reference-root',type=Path,required=True);p.add_argument('--object',type=Path,required=True);p.add_argument('--elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 result=run(a.reference_root,a.object,a.elf);a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
