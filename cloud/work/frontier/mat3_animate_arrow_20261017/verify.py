"""Strict source-bound AnimateArrow proof and complete seven-function locked closure."""
from pathlib import Path
import argparse,contextlib,hashlib,importlib.util,io,json,shlex,shutil,struct,sys
from dataclasses import asdict
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3];sys.path.insert(0,str(ROOT))
from tools.cloud import score
TARGET='entity_render_setup';ENTRY=0x8008C884
LOCKED=['func_8008B3C8','func_8008C074','func_8008C544','func_8008C5E0','func_8008C680','func_8008C720','func_8008C768']
EXPECTED_GRAPH={TARGET:['func_8008B3C8','func_8008C074','func_8008C544','func_8008C768'],'func_8008B3C8':[],'func_8008C074':[],'func_8008C544':[],'func_8008C5E0':[],'func_8008C680':['func_8008C5E0'],'func_8008C720':['func_8008C680'],'func_8008C768':['func_8008C680','func_8008C720']}
def require(ok,msg):
 if not ok:raise ValueError(msg)
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def wordhash(ws):return hashlib.sha256(struct.pack('>%dI'%len(ws),*ws)).hexdigest()
def module(name):
 s=importlib.util.spec_from_file_location('arrow_proof_'+name,HERE/(name+'.py'));m=importlib.util.module_from_spec(s);s.loader.exec_module(m);return m
def inputs():
 files=[p for p in HERE.rglob('*') if p.is_file() and p.suffix in ('.c','.py','.json') and p.name not in ('verification.json','trace_fidelity.json','independent_review.json') and '__pycache__' not in p.parts]
 result={str(p.relative_to(HERE)):digest(p) for p in sorted(files)}
 # w34i: blob_matched.lock.json and SHA256SUMS change with every splice (handoff §5): not bound.
 for name in ('tools/cloud/score.py','tools/cloud/owndata.py'):result['baseline:'+name]=digest(ROOT/name)
 return result
def symbol(score,obj,name):
 data,secs=score._elf(obj);es=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']==name];require(len(es)==1,'ambiguous symbol');e=es[0];sec=secs[e['section']];return e,data[sec['off']+e['value']:sec['off']+e['value']+e['size']]
def details(obj,name):
 with contextlib.redirect_stdout(io.StringIO()):r=score.compare(obj,name,show=0)
 e,body=symbol(score,obj,name);ws=struct.unpack('>%dI'%(len(body)//4),body)
 frame=next((65536-(w&65535) for w in ws[:12] if w>>26==9 and w>>21&31==29 and w>>16&31==29 and w&32768),0)
 comparison=asdict(r)
 # Retain refusal reasons without publishing the scorer's raw byte excerpts.
 comparison['errors']=[reason.split(' (+0x',1)[0] for reason in comparison['errors']]
 return {'comparison':comparison,'symbol_bytes':e['size'],'frame_bytes':frame}
def compile_leaf(source,flags,out):
 # IDO stores the argument spelling in .mdebug even with -g0. Keep complete
 # object hashes portable without weakening the instruction/extent proof.
 flags=shlex.split(flags)
 if score.R4300_CC not in flags:flags.append(score.R4300_CC)
 out=out.resolve()
 proc=score._run([score.ido('cc'),'-c',*flags,'-o',str(out),source.name],cwd=source.parent)
 require(proc.returncode==0 and out.exists(),'leaf compilation failed')
def exact(row,name):
 r=row['comparison'];require(not(r['differing'] or r['unresolved'] or r['unverified'] or r['errors'] or r['extra_words']),'non-strict result '+name);require(row['symbol_bytes']==len(score.targets()[name])*4,'incomplete extent '+name)
def graph():
 targets=score.targets();addresses=score.image_symbols();a={name:addresses.get(name) or score.address_named(name) for name in [TARGET]+LOCKED};byaddr={v:k for k,v in a.items()};result={}
 for name,addr in a.items():
  calls=set()
  for i,w in enumerate(targets[name]):
   if w>>26 in (2,3):
    dest=((addr+4*i+4)&0xf0000000)|((w&0x3ffffff)<<2)
    if not addr<=dest<addr+len(targets[name])*4:require(dest in byaddr,'unlisted closure destination');calls.add(byaddr[dest])
  result[name]=sorted(calls)
 require(result==EXPECTED_GRAPH,'closure graph changed');return result

def verify(out,record=False):
 out=Path(out);out.mkdir(parents=True,exist_ok=True);g=json.loads((HERE/'group/group.json').read_text());require(g['claims']==[TARGET] and g['members']==[TARGET] and g['context']==[],'main claim scope changed')
 sources=inputs();targets={n:wordhash(score.targets()[n]) for n in [TARGET]+LOCKED}
 if not record:
  receipt=json.loads((HERE/'verification.json').read_text());require(receipt['claims']==[TARGET] and receipt['new_matching_bytes']==2048,'false credit');require(receipt['inputs_sha256']==sources,'source binding changed');require(receipt['target_words_sha256']==targets,'target binding changed');trace=json.loads((HERE/'trace_fidelity.json').read_text());require(trace['inputs_sha256']==sources,'trace input binding changed')
 obj=out/'candidate.o';score.compile_group(HERE/'group',obj);main=details(obj,TARGET);exact(main,TARGET);require(main['frame_bytes']==184,'main frame changed')
 if not record:require(digest(obj)==trace['stock_object_sha256']==trace['trace_object_sha256'],'trace object binding changed')
 # Include all zero coordinates and all three trailing zero bytes, not just
 # the nonzero own-data windows used by the relocation proof.
 qe,_=symbol(score,obj,'D_8011AD90');qd,qs=score._elf(obj);qsec=qs[qe['section']];require(qe['value']+48<=qsec['size'],'quad section extent');quad=qd[qsec['off']+qe['value']:qsec['off']+qe['value']+48];expected=struct.pack('>12f',1.,1.,0.,-1.,1.,0.,-1.,-1.,0.,1.,-1.,0.);require(len(quad)==48 and quad==expected==score.own_data().read(0x8011AD90,48),'full quad table mismatch')
 palette=bytes(x for row in ((0,0,224,255),(224,0,0,255),(224,224,0,255),(0,224,0,255),(224,224,224,255)) for x in row);alpha=bytes((32,48,80,128,160,192,239));require(score.own_data().read(0x8011B558,20)==palette and score.own_data().read(0x8011B56C,7)==alpha and score.own_data().read(0x8011B578,4)==bytes([255]*4),'native palette/alpha/white changed')
 closure={};claims=[];spec=json.loads((HERE/'closure.json').read_text());lock=json.loads((ROOT/'blob_matched.lock.json').read_text())
 for tag,row in spec.items():
  require(row['members']==row['claims'],'incomplete closure claims');claims+=row['claims'];d=HERE/'closure'/tag;o=out/(tag+'.o')
  for name in row['claims']:require(name in lock,'closure member is not locked')
  origin=ROOT/row['original_source'];require(digest(origin)==row['original_source_sha256'],'original closure provenance changed')
  if row['kind']=='group':
   gs=json.loads((d/'group.json').read_text());require(gs['claims']==row['claims']==gs['members']+gs['context'],'hidden group context');score.compile_group(d,o)
  else:compile_leaf(d/'source.c',row['flags'],o)
  closure[tag]={'object_sha256':digest(o),'members':{}}
  for name in row['members']:r=details(o,name);exact(r,name);closure[tag]['members'][name]=r
 require(sorted(claims)==LOCKED,'complete locked claim set changed')
 source=(HERE/'group/group.c').read_text();controls={
 'direct_hide_store':('ZOID_HidePoly(&D_8015B268[effect->mesh]);','D_8015B268[effect->mesh].flags |= 0x8000;'),
 'reconstructed_local_order':('f32 x, y, depth, len, slope, threshold;','f32 depth, x, len, slope, threshold, y;'),
 'wrong_default_palette':('D_8011B558[4]','D_8011B558[3]'),
 'wrong_alpha_cap':('color.c.a = 112;','color.c.a = 113;'),
 'wrong_world_flags':('(1 << type) | 0x3600','(1 << type) | 0x3700'),
 'wrong_zero_coordinate':('{ 1.0f, 1.0f, 0.0f }','{ 1.0f, 1.0f, 0.25f }')};negative={}
 behavior=module('behavior');semantic=behavior.verify(out/'host')
 for tag,(old,new) in controls.items():
  require(old in source,'mutation anchor absent');changed=source.replace(old,new);d=out/tag;d.mkdir(exist_ok=True);(d/'group.c').write_text(changed);shutil.copyfile(HERE/'group/group.json',d/'group.json');o=d/'candidate.o';score.compile_group(d,o);r=details(o,TARGET);require(r['comparison']['differing'] or r['comparison']['errors'] or r['comparison']['unverified'],'negative strict match');negative[tag]={'detail':r}
  if tag in ('wrong_default_palette','wrong_alpha_cap','wrong_world_flags','wrong_zero_coordinate'):
   try:behavior.verify(d/'host',changed)
   except ValueError as e:reason=str(e)
   else:raise ValueError('semantic negative passed')
   require(reason.startswith('behavior '),'semantic negative failed outside behavior');negative[tag]['semantic_rejection']=reason
 require(negative['reconstructed_local_order']['detail']['comparison']['differing']==7,'local-order causal control changed')
 result={'status':'STRICT_MATCH_WITH_COMPLETE_LOCKED_CLOSURE','claims':[TARGET],'preserved_locked_claims':LOCKED,'new_matching_bytes':2048,'inputs_sha256':sources,'target_words_sha256':targets,'object_sha256':digest(obj),'main':main,'closure':closure,'call_graph':graph(),'full_quad_sha256':hashlib.sha256(quad).hexdigest(),'palette_sha256':hashlib.sha256(palette).hexdigest(),'behavior':semantic,'emitted_layout':module('layout').verify(score,out/'layout',require),'negative_controls':negative,'canonical_or_ROM_verification':False}
 if record:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
 else:require(result==receipt,'sealed receipt changed')
 return result
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--record',action='store_true');p.add_argument('--out',type=Path,default=ROOT/'build/mat3_animate_arrow_20261017');a=p.parse_args();verify(a.out,a.record);print('PASS: AnimateArrow 2048 bytes; seven exact locked callees; causal and semantic negatives')
