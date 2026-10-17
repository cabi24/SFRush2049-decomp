"""Protocol-v2 research replay. Every matched dependency is preserved; no new claim."""
from pathlib import Path
import argparse,contextlib,dataclasses,hashlib,importlib.util,io,json,shutil,struct,sys,tempfile
HERE=Path(__file__).resolve().parent
TARGET='ghost_race_setup'
def require(ok,message):
 if not ok:raise RuntimeError(message)
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def whash(w):return hashlib.sha256(struct.pack('>%dI'%len(w),*w)).hexdigest()
def module(name):
 s=importlib.util.spec_from_file_location('ghostend_'+name,HERE/(name+'.py'));m=importlib.util.module_from_spec(s);s.loader.exec_module(m);return m
def inputs():
 return {str(p.relative_to(HERE)):digest(p) for p in sorted(HERE.rglob('*')) if p.is_file() and p.suffix in ('.c','.py','.json') and p.name not in ('receipt.json','trace_fidelity.json','independent_review.json') and '__pycache__' not in p.parts}
def body(score,obj,name):
 data,secs=score._elf(obj);es=[e for i,s in enumerate(secs) if s['type']==2 for e in score._symbol_table(data,secs,i) if e['name']==name and e['type']==2];require(len(es)==1,'symbol extent');e=es[0];a,b=e['value'],e['value']+e['size'];ws=score.text_words(obj);r,m,u,v,errors=score.relocate(obj,ws,a,b,score.image_symbols());return r[a//4:b//4],e['size'],u,v,errors
def verify(repo=None,compile=True,record=False,out=None):
 repo=Path(repo).resolve() if repo else HERE.parents[3];sys.path.insert(0,str(repo/'tools/cloud'));import score
 recipe=json.loads((HERE/'group/group.json').read_text());require(recipe['claims']==[],'claims must stay empty');names=recipe['members']+recipe['context'];sources=inputs();targets={n:whash(score.targets()[n]) for n in names};baseline={n:digest(repo/n) for n in ('tools/cloud/score.py','tools/cloud/owndata.py','blob_matched.lock.json')}
 if not record:
  receipt=json.loads((HERE/'receipt.json').read_text());require(receipt['protocol_version']==2 and receipt['claims']==[] and receipt['new_accepted_bytes']==0,'false credit');require(receipt['source_sha256']==sources,'source binding');require(receipt['target_words_sha256']==targets,'target binding');require(receipt['baseline_sha256']==baseline,'baseline binding')
 if not record:
  trace=json.loads((HERE/'trace_fidelity.json').read_text());require(trace['inputs_sha256']==sources,'trace input binding');require(trace['observation_only'] and not trace['forced_colors'] and trace['complete_objects_identical'],'trace scope')
 if not compile:return {'input_integrity':True,'claims':[],'compiler_run':False}
 natural=(HERE/'group/ghost.c').read_text();historical=(HERE/'historical_typed.c').read_text()
 variants={'natural':natural,'repeat':natural,'historical_typed':historical,'expanded_free':natural.replace('func_800960CC((u32)g->start);','osRecvMesg(&D_80152770, 0, 1);\n                    audio_reverb_update((u32)g->start, 0);\n                    osSendMesg(&D_80152770, 0, 0);'),'wrong_modelrun_byte':natural.replace('s16 reset; u8 tail[234];','s8 reset; u8 tail[235];'),'wrong_recorded_size':natural.replace('g->size = g->offset;','g->size = 0;'),'wrong_free_pointer':natural.replace('func_800960CC((u32)g->start);','func_800960CC((u32)g->start + 4);')}
 if out is None:
  with tempfile.TemporaryDirectory(prefix='ghostend-research-') as temp:return _run(score,repo,Path(temp),recipe,names,sources,targets,baseline,variants,record)
 return _run(score,repo,Path(out),recipe,names,sources,targets,baseline,variants,record)
def _run(score,repo,out,recipe,names,sources,targets,baseline,variants,record):
 out.mkdir(parents=True,exist_ok=True);behavior=module('behavior');lock=json.loads((repo/'blob_matched.lock.json').read_text());preserved=[n for n in names if n not in ('CreatePlayer','menu_back',TARGET)];require(len(preserved)==33 and all(n in lock for n in preserved),'preserved closure inventory');results={}
 for tag,source in variants.items():
  require(tag in ('natural','repeat') or source!=variants['natural'],'variant anchor');p=out/tag;shutil.copytree(HERE/'group',p,dirs_exist_ok=True);(p/'ghost.c').write_text(source);obj=p/'result.o';score.compile_group(p,obj);rows={}
  for n in names:
   with contextlib.redirect_stdout(io.StringIO()):c=score.compare(obj,n,show=0)
   if n in preserved:require(c.accepted(),'preserved nonmatch '+n)
   else:
    require(not c.accepted() and not c.unresolved and not c.unverified and not c.errors,'unexpected nonmatch proof '+n)
    if n=='CreatePlayer':require(c.differing==246 and c.extra_words==0,'CreatePlayer changed')
    if n=='menu_back':require(c.differing==3 and c.extra_words==0,'menu_back changed')
   rows[n]=dataclasses.asdict(c)
  words,extent,u,v,e=body(score,obj,TARGET);require(not(u or v or e),'root relocation incomplete');native=score.targets()[TARGET];frame=next((65536-(w&65535) for w in words[:12] if w>>16==0x27bd and w&32768),0);require(frame==64,'root frame changed');require([w&0x3ffffff for w in words if w>>26==3]==[w&0x3ffffff for w in native if w>>26==3],'root call sequence changed')
  if tag in ('natural','repeat','historical_typed','expanded_free'):
   expected={'natural':(54,2,448),'repeat':(54,2,448),'historical_typed':(16,0,440),'expanded_free':(54,1,444)}[tag];require((rows[TARGET]['differing'],rows[TARGET]['extra_words'],extent)==expected,'root plateau changed');sem=behavior.verify(native,words)
  else:
   try:behavior.verify(native,words)
   except RuntimeError as failure:reason=str(failure)
   else:raise RuntimeError('semantic negative passed')
   require('model mismatch' in reason,'semantic negative failed outside model');sem={'rejected':True,'reason':reason}
  results[tag]={'object_sha256':digest(obj),'scores':rows,'root_bytes':extent,'root_frame':frame,'behavior':sem}
 require(results['natural']==results['repeat'],'repeat changed');layout=module('layout').verify(score,variants['natural'],out/'layout',require)
 result={'protocol_version':2,'claims':[],'new_accepted_bytes':0,'source_sha256':sources,'target_words_sha256':targets,'baseline_sha256':baseline,'preserved_locked_members':preserved,'results':results,'layout':layout,'canonical_or_ROM_verification':False}
 if record:(HERE/'receipt.json').write_text(json.dumps(result,indent=2)+'\n')
 else:
  require(results['natural']['object_sha256']==json.loads((HERE/'trace_fidelity.json').read_text())['object_sha256'],'trace object binding')
  require(result==json.loads((HERE/'receipt.json').read_text()),'sealed result changed')
 return result
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--repo',type=Path);p.add_argument('--out',type=Path);p.add_argument('--record',action='store_true');a=p.parse_args();r=verify(a.repo,record=a.record,out=a.out);print('PASS: GhostEnd research, 33 preserved functions, zero new claims')
