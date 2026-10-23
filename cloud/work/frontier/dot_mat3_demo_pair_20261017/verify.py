"""Protocol-v2 demo pair research: exact helper, unmatched real caller, no credit."""
from pathlib import Path
import argparse,contextlib,dataclasses,hashlib,importlib.util,io,json,shutil,struct,sys,tempfile
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3] if len(HERE.parents)>3 else Path.cwd()
HELPER='debug_stats';CALLER='debug_collision';STUBS=['func_800FCDF0','func_800FD00C','func_800FD014','func_800FD01C']
def require(ok,msg):
 if not ok:raise RuntimeError(msg)
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words_hash(w):return hashlib.sha256(struct.pack('>%dI'%len(w),*w)).hexdigest()
def module(name):
 s=importlib.util.spec_from_file_location('demo_'+name,HERE/(name+'.py'));m=importlib.util.module_from_spec(s);s.loader.exec_module(m);return m
def inputs():return {str(p.relative_to(HERE)):digest(p) for p in sorted(HERE.rglob('*')) if p.is_file() and p.suffix in ('.c','.py','.json') and p.name not in ('receipt.json','trace_fidelity.json','independent_review.json') and '__pycache__' not in p.parts}
def detail(score,obj,name):
 with contextlib.redirect_stdout(io.StringIO()):r=score.compare(obj,name,show=0)
 raw,ss=score._elf(obj);entries=[s for i,sec in enumerate(ss) if sec['type']==2 for s in score._symbol_table(raw,ss,i) if s['name']==name];require(len(entries)==1,'missing/ambiguous function');e=entries[0];ws=score.text_words(obj)[e['value']//4:(e['value']+e['size'])//4]
 data=dataclasses.asdict(r);data['errors']=[x.split(' (+0x')[0].split(' expected ')[0] for x in data['errors']]
 frame=next((65536-(w&65535) for w in ws[:12] if w>>26==9 and w>>21&31==29 and w>>16&31==29 and w&32768),0)
 return {'comparison':data,'symbol_bytes':e['size'],'frame_bytes':frame}
def exact(row,name):
 r=row['comparison'];require(not any(r[k] for k in ('differing','unresolved','unverified','errors','extra_words')),'not strict '+name)
def verify(repo=ROOT,out=None,record=False,compile=True):
 repo=Path(repo).resolve();sys.path.insert(0,str(repo/'tools/cloud'));import score
 spec=json.loads((HERE/'group/group.json').read_text());require(spec['claims']==[],'false group claims');names=spec['members']+spec['context'];require(len(names)==31 and len(set(names))==31,'incomplete group boundary')
 preserved=spec['members']+STUBS;require(len(preserved)==29,'preserved group scope');source=inputs();targets={n:words_hash(score.targets()[n]) for n in names};baseline={n:digest(repo/n) for n in ('tools/cloud/score.py','tools/cloud/owndata.py','blob_matched.lock.json')}
 if not record:
  receipt=json.loads((HERE/'receipt.json').read_text());require(receipt['protocol_version']==2 and receipt['claims']==[] and receipt['new_accepted_bytes']==0,'false credit');require(receipt['source_sha256']==source,'source/script/model changed');require(receipt['target_words_sha256']==targets,'target changed');require(receipt['baseline_sha256']==baseline,'baseline changed');trace=json.loads((HERE/'trace_fidelity.json').read_text());require(trace['inputs_sha256']==source,'trace input changed');require(trace['observation_only'] and not trace['forced_colors'] and trace['complete_objects_identical'],'trace scope changed')
 if not compile:return {'integrity':'PASS'}
 out=Path(out) if out else Path(tempfile.mkdtemp(prefix='demo-proof-'));out.mkdir(parents=True,exist_ok=True);body=(HERE/'group/demo.c').read_text()
 variants={'natural':body,'repeat':body,
  'external_demo_flag':body.replace('static s8 D_80114154 = 0;','extern s8 D_8011415C;\nstatic s8 D_80114154 = 0;').replace('    static s8 D_8011415C = 0;\n',''),
  'uninitialized_demo_flag':body.replace('static s8 D_8011415C = 0;','static s8 D_8011415C;'),
  'wrong_requested_state':body.replace('setGameState(4);','setGameState(3);'),
  'wrong_preplay_mask':body.replace('D_801174B4 & 8','D_801174B4 & 4'),
  'wrong_teardown_flag':body.replace('        D_8011415C = 0;','        D_8011415C = 1;')}
 behavior=module('behavior');reloc=module('relocate');native={a:score.targets()[n] for n,a in reloc.NAMES.items()};results={};objects={}
 for tag,text in variants.items():
  require(tag in ('natural','repeat') or text!=body,'negative anchor absent');d=out/tag;shutil.copytree(HERE/'group',d,dirs_exist_ok=True);(d/'demo.c').write_text(text);o=d/'candidate.o';score.compile_group(d,o);objects[tag]=o;rows={n:detail(score,o,n) for n in names}
  for n in preserved:exact(rows[n],n);require(rows[n]['symbol_bytes']==len(score.targets()[n])*4,'preserved extent '+n)
  result={'object_sha256':digest(o),'functions':rows}
  if tag in ('natural','repeat'):
   exact(rows[HELPER],HELPER);require(rows[HELPER]['symbol_bytes']==532 and rows[HELPER]['frame_bytes']==24,'helper extent/frame');r=rows[CALLER];require(r['symbol_bytes']==532 and r['frame_bytes']==48,'caller extent/frame');c=r['comparison'];require(c['differing']==3 and not any(c[k] for k in ('unresolved','unverified','errors','extra_words')),'caller residual changed')
   pair=reloc.pair(score,o);bad=[4*i for i,(x,y) in enumerate(zip(pair[reloc.NAMES[CALLER]],native[reloc.NAMES[CALLER]])) if x!=y];require(bad==[204,284,412],'caller mismatch sites changed');result['differing_caller_offsets']=bad;result['behavior']=behavior.verify(native,pair)
  elif tag=='external_demo_flag':require(rows[HELPER]['comparison']['differing']==117,'static-scope causal control')
  elif tag=='uninitialized_demo_flag':require(rows[HELPER]['comparison']['unverified'] and rows[HELPER]['comparison']['errors'],'initialized storage negative passed')
  else:
   try:behavior.verify(native,reloc.pair(score,o))
   except RuntimeError as e:reason=str(e)
   else:raise RuntimeError('semantic negative passed')
   require(reason.startswith('fixture '),'negative failed outside model');result['semantic_rejection']='independent field/event model mismatch'
  results[tag]=result
 require(objects['natural'].read_bytes()==objects['repeat'].read_bytes(),'repeat object changed')
 if not record:require(digest(objects['natural'])==trace['object_sha256'],'trace object changed')
 layout=module('layout').verify(score,out/'layout',body,require)
 result={'protocol_version':2,'claims':[],'new_accepted_bytes':0,'source_sha256':source,'target_words_sha256':targets,'baseline_sha256':baseline,'preserved_bodies':preserved,'exact_research_helper':HELPER,'results':results,'layout':layout,'canonical_or_ROM_verification':False}
 if record:(HERE/'receipt.json').write_text(json.dumps(result,indent=2)+'\n')
 else:require(result==receipt,'sealed result changed')
 return result
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--repo',type=Path,default=ROOT);p.add_argument('--out',type=Path);p.add_argument('--record',action='store_true');a=p.parse_args();verify(a.repo,a.out,a.record);print('PASS: exact demo helper; caller 3/133; 29 preserved bodies; no new credit')
