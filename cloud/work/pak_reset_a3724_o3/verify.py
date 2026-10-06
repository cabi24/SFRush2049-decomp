"""Portable single-recipe replay for the complete genuine-context A3724 claim.
Mutable production context comes exclusively from BASE history, never live hashes.
"""
import argparse,dataclasses,hashlib,importlib.util,json,os,pathlib,shutil,struct,subprocess,sys,tempfile
HERE=pathlib.Path(__file__).resolve().parent
ROOT=HERE.parents[2]
GROUP=ROOT/'cloud/matches/pak_reset_a3724_group'
BASE='6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
REPO=pathlib.Path(os.environ.get('RUSH_RECOVERY_ROOT',ROOT)).resolve()
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
CLAIM='func_800A3724'
def sha(data):return hashlib.sha256(data).hexdigest()
def normalize(x):
 if isinstance(x,set):return sorted(normalize(v) for v in x)
 if isinstance(x,dict):return {k:normalize(v) for k,v in x.items()}
 if isinstance(x,(list,tuple)):return [normalize(v) for v in x]
 return x
def read(path):return subprocess.check_output(['git','-C',str(REPO),'show',BASE+':'+path])
def source_contract():
 manifest=json.loads((HERE/'source_manifest.json').read_text());spec=json.loads((GROUP/'group.json').read_text())
 assert manifest['base']==BASE
 assert sha(pathlib.Path(__file__).read_bytes())==manifest['verifier_sha256']
 assert sha((GROUP/'group.json').read_bytes())==manifest['group_json_sha256']
 assert sha((GROUP/'group.c').read_bytes())==manifest['group_source_sha256']
 assert (GROUP/'group.c').read_text().splitlines()[0]=='/* flags: '+FLAGS+' */'
 assert {k:spec[k] for k in ['flags','files','keep']}==manifest['build_recipe']
 assert spec['claims']==spec['members']==[CLAIM]
 assert set(spec['context'])==set(manifest['target_sha256'])-{CLAIM}
 assert CLAIM not in spec['keep'] and 'no_catchup' not in spec['keep']
 assert manifest['semantic_source_review']['decision']=='PASS_INDEPENDENT_BOUNDED_SEMANTIC_REVIEW'
 assert manifest['semantic_source_review']['source_files']==manifest['producer_source_files']
 return manifest,spec

def snapshot(work):
 paths=['tools/cloud/score.py','tools/cloud/owndata.py']
 for directory in ['asm/us/blob','asm/us/blob_data']:
  paths+=subprocess.check_output(['git','-C',str(REPO),'ls-tree','-r','--name-only',BASE,'--',directory],text=True).splitlines()
 for path in paths:
  dest=work/path;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(read(path))
 # Isolated module names keep this verifier independent of an imported live scorer.
 prefix='pak_a3724_base'
 package=importlib.util.spec_from_file_location(prefix,work/'tools/cloud/score.py',submodule_search_locations=[str(work/'tools/cloud')])
 score=importlib.util.module_from_spec(package);sys.modules[prefix]=score;package.loader.exec_module(score)
 if 'IDO_DIR' not in os.environ:score.IDO=REPO/'tools/cloud/ido'
 return score

def function_table(score,obj):
 data,secs=score._elf(obj);ti=score._text_index(secs);functions={}
 for i,sec in enumerate(secs):
  if sec['type']!=2:continue
  for index,s in enumerate(score._symbol_table(data,secs,i)):
   if s['section']==ti and s['type']==2:
    assert s['name'] not in functions
    functions[s['name']]={**s,'binding':data[sec['off']+index*16+12]>>4}
 return data,secs,ti,functions

def relocated_body(score,obj,name,functions,addresses):
 f=functions[name];words=score.text_words(obj);start=f['value'];end=start+f['size']
 assert f['size']>0 and end<=4*len(words)
 r,m,u,v,e=score.relocate(obj,words,start,end,addresses)
 return r[start//4:end//4],m,u,v,e

def body_calls(body,address):
 return [(i*4,((address+i*4+4)&0xf0000000)|((w&0x3ffffff)<<2)) for i,w in enumerate(body) if w>>26==3]

def native_call_contract(score):
 targets=score.targets();addresses=score.image_symbols();rows=[]
 for caller,pc,source_register in [('track_render_process',0x800A391C,30),('track_render_process',0x800A3990,30),('car_lod_select',0x800A45C0,19)]:
  body=targets[caller];i=(pc-addresses[caller])//4;w=body[i];delay=body[i+1]
  assert w>>26==3 and (((pc+4)&0xf0000000)|((w&0x3ffffff)<<2))==addresses[CLAIM]
  assert (delay>>26,delay>>21&31,delay>>16&31,delay&65535)==(12,source_register,7,255)
  rows.append({'caller':caller,'native_pc':f'0x{pc:08X}','port_register':'a3','source_register_number':source_register})
 return rows

def reset_contract(body,address,memset_address):
 """Execute only the complete reset body's witnessed integer instruction subset.
 All valid ports and all signed-byte bit patterns, with aggressive O32 call clobbers.
 """
 PFS=0x80144030;STACK=0x300080;STOP=0x400000;MASK=0xffffffff
 def signed(v):return v-0x100000000 if v&0x80000000 else v
 def sext(v):return v-65536 if v&32768 else v
 assert len(body)==22
 assert (body[0]>>26,body[0]>>21&31,body[0]&65535)==(12,7,255)
 assert (body[12]>>26,body[12]>>21&31,body[12]>>16&31,body[12]&65535)==(32,16,17,5)
 results=0
 for port in range(4):
  for blocked in range(256):
   for high in (0,0xA5B60000):
    mem={PFS-8+i:(i*37+11)&255 for i in range(772*4+16)}
    mem.update({STACK-64+i:0x5a for i in range(192)})
    mem[PFS+772*port+5]=blocked
    expected={a:v for a,v in mem.items() if not STACK-64<=a<STACK+128}
    expected.update({PFS+772*port+i:0 for i in range(772)});expected[PFS+772*port+5]=blocked
    r=[(0x12340000+i*137)&MASK for i in range(32)];r[0]=0;r[7]=high|port;r[29]=STACK;r[31]=STOP;initial=list(r)
    pc=address;pending=None;steps=0;calls=0
    def get(a,n):
     assert a%n==0 and all(a+i in mem for i in range(n)),('unmapped read',hex(a),n)
     return int.from_bytes(bytes(mem[a+i] for i in range(n)),'big')
    def put(a,n,v):
     assert a%n==0 and all(a+i in mem for i in range(n)),('unmapped write',hex(a),n)
     for i,x in enumerate((v&((1<<(n*8))-1)).to_bytes(n,'big')):mem[a+i]=x
    while pc!=STOP:
     steps+=1;assert steps<100
     if pc==memset_address:
      assert r[4:7]==[PFS+port*772,0,772];ret=r[31];calls+=1
      for i in range(772):put(r[4]+i,1,0)
      for reg in [1,2,3,*range(4,16),24,25]:r[reg]=(0xDEC00000+reg*191)&MASK
      pc=ret;continue
     assert address<=pc<address+len(body)*4 and pc%4==0,('unmapped pc',hex(pc))
     w=body[(pc-address)//4];op=w>>26;s=w>>21&31;t=w>>16&31;d=w>>11&31;imm=w&65535
     before=pending;pending=None
     if op==0:
      fn=w&63
      if fn==0:r[d]=(r[t]<<(w>>6&31))&MASK
      elif fn in (33,35,37):r[d]=((r[s]+r[t]) if fn==33 else (r[s]-r[t]) if fn==35 else (r[s]|r[t]))&MASK
      elif fn==8:pending=r[s]
      else:raise AssertionError(('unsupported special',fn))
     elif op==9:r[t]=(r[s]+sext(imm))&MASK
     elif op==12:r[t]=r[s]&imm
     elif op==15:r[t]=imm<<16
     elif op==35:r[t]=get((r[s]+sext(imm))&MASK,4)
     elif op==43:put((r[s]+sext(imm))&MASK,4,r[t])
     elif op==32:
      v=get((r[s]+sext(imm))&MASK,1);r[t]=(v-256 if v&128 else v)&MASK
     elif op==40:put((r[s]+sext(imm))&MASK,1,r[t])
     elif op==3:r[31]=pc+8;pending=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
     else:raise AssertionError(('unsupported opcode',op))
     r[0]=0;pc=before if before is not None else pc+4
    assert calls==1 and r[29]==initial[29] and r[31]==initial[31] and r[28]==initial[28]
    actual={a:v for a,v in mem.items() if not STACK-64<=a<STACK+128};assert actual==expected
    results+=1
 return {'executions':results,'ports':4,'blocked_byte_patterns':256,'a3_high_bit_patterns':2,'all_modeled_nonstack_bytes_per_execution':772*4+16,'contract':'Exactly one memset clears only the selected 772-byte Pak; signed +5 survives and is restored; adjacent Pak/guard bytes unchanged; sp/ra restored. Caller-clobbered registers are poisoned at memset.','limitations':'Private native boundary permits s0/s1 clobbers. Valid ports 0..3 only. This is a focused fail-closed integer interpreter with an explicit memset contract, not whole-system execution.'}

def proof(score,obj,manifest):
 data,secs,ti,fns=function_table(score,obj);addresses=score.image_symbols();targets=score.targets()
 assert set(fns)==set(manifest['target_sha256'])
 result={'functions':{},'own_sections':[],'relocations':[]}
 for n,expected in manifest['target_sha256'].items():
  assert sha(b''.join(w.to_bytes(4,'big') for w in targets[n]))==expected,n
 for sec in secs:
  if sec['type']==9:
   syms=score._symbol_table(data,secs,sec['link'])
   for p in range(sec['off'],sec['off']+sec['size'],8):
    offset,info=struct.unpack_from('>II',data,p);symbol=syms[info>>8]
    result['relocations'].append({'section':secs[sec['info']]['name'],'offset':offset,'type':info&255,'symbol':symbol['name'],'symbol_type':symbol['type'],'symbol_value':symbol['value']})
  if sec['name'] in ['.data','.sdata','.rodata','.rdata','.lit4','.lit8','.bss','.sbss']:
   row={k:sec[k] for k in ['name','size','type']}
   if sec['type']==1:row['sha256']=sha(data[sec['off']:sec['off']+sec['size']])
   result['own_sections'].append(row)
 for n,f in fns.items():
  body,m,u,v,e=relocated_body(score,obj,n,fns,addresses)
  own=score.owndata.verify(obj,n,targets[n],address=addresses[n],image=score.own_data(),start=f['value'],addresses=lambda name:addresses.get(name,score.address_named(name)))
  comparison=score.compare(obj,n,show=0)
  result['functions'][n]={'offset':f['value'],'extent':f['size'],'elf_type':'STT_FUNC','binding':f['binding'],'native_extent':len(targets[n])*4,'relocation_applied_body_sha256':sha(b''.join(w.to_bytes(4,'big') for w in body)),'full_extent_relocation':{'masks':sorted(m),'unresolved':u,'unverified':v,'errors':e},'own_data':normalize(dataclasses.asdict(own)),'canonical':{**dataclasses.asdict(comparison),'notes':list(comparison.notes)},'strict_match':f['size']==len(targets[n])*4 and comparison.accepted()}
  if n==CLAIM:
   assert len(body)*4==f['size']==88 and body==targets[n]
   assert not(m or u or v or e) and own.references==0 and comparison.accepted()
   result['reset_behavior']=reset_contract(body,addresses[n],addresses['memset'])
 result['native_callers']=native_call_contract(score);result['compiled_callers']=[]
 for caller in ['track_render_process','car_lod_select']:
  body,m,u,v,e=relocated_body(score,obj,caller,fns,addresses);assert not(m or u or v or e)
  for off,target in body_calls(body,addresses[caller]):
   if target==addresses[CLAIM]:
    delay=body[off//4+1];assert (delay>>26,delay>>16&31,delay&65535)==(12,7,255)
    result['compiled_callers'].append({'caller':caller,'function_offset':off,'port_register':'a3','source_register_number':delay>>21&31})
 assert len(result['compiled_callers'])==3
 result['accepted_context_regressions_at_base']=sorted(n for n in manifest['accepted_context_at_base'] if not result['functions'][n]['strict_match'])
 assert result['accepted_context_regressions_at_base']==['audio_reverb_update','func_80095EC0','func_80095EF4','func_800A43FC','no_catchup']
 result['claims']=[CLAIM];return normalize(result)

def verify(existing_object=None,record=False):
 manifest,spec=source_contract()
 with tempfile.TemporaryDirectory(prefix='pak-reset-a3724-') as td:
  work=pathlib.Path(td);score=snapshot(work/'base')
  if existing_object is None:
   if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
    return {'status':'SKIP','reason':'pinned IDO and MIPS GNU linker required','target_compilations':0}
   obj=work/'candidate.o';score.compile_group(GROUP,obj);compiles=1
  else:
   obj=pathlib.Path(existing_object);compiles=0
   assert sha(obj.read_bytes())==manifest['original_baseline_object_sha256'],'existing-object route accepts only recorded baseline'
  result=proof(score,obj,manifest)
  if record:(HERE/'receipt.json').write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
  else:assert result==json.loads((HERE/'receipt.json').read_text()),'fresh proof differs from recorded words/extents/relocations/own data/behavior'
  return {'status':'PASS','claims':result['claims'],'claim_bytes':88,'target_compilations':compiles,'reset_executions':result['reset_behavior']['executions'],'accepted_context_regressions_at_base':result['accepted_context_regressions_at_base']}

def main():
 p=argparse.ArgumentParser();p.add_argument('--existing-object');p.add_argument('--record',action='store_true');a=p.parse_args()
 print(json.dumps(verify(a.existing_object,a.record),indent=2))
if __name__=='__main__':main()
