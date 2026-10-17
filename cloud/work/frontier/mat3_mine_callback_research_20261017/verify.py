"""Source-bound Mine callback research replay. Empty claims, zero accepted bytes."""
from pathlib import Path
import argparse,contextlib,hashlib,importlib.util,io,json,shutil,struct,sys
from dataclasses import asdict
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
INPUTS=['verify.py','behavior.py','layout.py','trace_fidelity.py','group/group.c','group/group.json']
TARGET='func_8010C974';ALLOC='func_8008E26C'
def require(ok,msg):
 if not ok:raise ValueError(msg)
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def wordhash(ws):return hashlib.sha256(struct.pack('>%dI'%len(ws),*ws)).hexdigest()
def module(name):
 s=importlib.util.spec_from_file_location('mine_proof_'+name,HERE/(name+'.py'));m=importlib.util.module_from_spec(s);s.loader.exec_module(m);return m

def details(score,obj,name):
 with contextlib.redirect_stdout(io.StringIO()):r=score.compare(obj,name,show=0)
 require(not(r.unresolved or r.unverified or r.errors),'uncertain relocation')
 data,secs=score._elf(obj);ti=score._text_index(secs);es=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']==name and s['type']==2 and s['section']==ti];require(len(es)==1,'ambiguous symbol');e=es[0];ws=score.text_words(obj)[e['value']//4:(e['value']+e['size'])//4]
 frame=next((65536-(w&65535) for w in ws[:12] if w>>26==9 and w>>21&31==29 and w>>16&31==29 and w&32768),0)
 return {'comparison':asdict(r),'symbol_bytes':e['size'],'frame_bytes':frame}

def verify(out,record=False):
 out=Path(out);out.mkdir(parents=True,exist_ok=True);behavior=module('behavior');score=behavior.score
 g=json.loads((HERE/'group/group.json').read_text());require(g['claims']==[] and g['members']==[TARGET] and g['context']==[ALLOC],'research scope changed')
 inputs={n:digest(HERE/n) for n in INPUTS};targets={n:wordhash(score.targets()[n]) for n in (TARGET,ALLOC,'math_utility')}
 if not record:
  receipt=json.loads((HERE/'verification.json').read_text());require(receipt['claims']==[] and receipt['new_matching_bytes']==0,'false credit');require(receipt['inputs_sha256']==inputs,'source binding changed');require(receipt['target_words_sha256']==targets,'target binding changed');trace=json.loads((HERE/'trace_fidelity.json').read_text());require(trace['inputs_sha256']==inputs,'trace input binding changed')
 obj=out/'candidate.o';score.compile_group(HERE/'group',obj);main=details(score,obj,TARGET);alloc=details(score,obj,ALLOC)
 require(main['comparison']['differing']==499 and main['comparison']['total']==659 and main['comparison']['extra_words']==0 and main['symbol_bytes']==2632 and main['frame_bytes']==344,'callback residual changed')
 require(alloc['comparison']['differing']==0 and alloc['comparison']['total']==75 and alloc['comparison']['extra_words']==3 and alloc['symbol_bytes']==300,'allocator extent/control changed')
 if not record:require(digest(obj)==trace['stock_object_sha256']==trace['trace_object_sha256'],'trace object binding changed')
 semantic=behavior.verify(obj);require(semantic['fixtures']==206 and semantic['mine_executed_words'][0]==654,'behavior coverage changed')
 source=(HERE/'group/group.c').read_text();mutations={'wrong_alpha':('d->alpha[i]-=8','d->alpha[i]-=7','bytes'),'wrong_oldest_tie':('oldest<D_80151528','oldest<=D_80151528','bytes'),'wrong_damage':('400.0f,1600','400.0f,1599','events')};negatives={}
 for tag,(old,new,kind) in mutations.items():
  require(source.count(old)==1,'ambiguous mutation');d=out/tag;d.mkdir(exist_ok=True);(d/'group.c').write_text(source.replace(old,new));shutil.copyfile(HERE/'group/group.json',d/'group.json');o=d/'candidate.o';score.compile_group(d,o);row=details(score,o,TARGET)
  try:behavior.verify(o)
  except ValueError as e:reason=str(e).splitlines()[0]
  else:raise ValueError('semantic negative passed')
  require(reason.startswith('behavior mismatch program 1'),'negative failed outside candidate semantics')
  require(('bytes []' not in reason and 'events True' in reason) if kind=='bytes' else ('bytes []' in reason and 'events False' in reason),'negative failed for wrong reason');negatives[tag]={'detail':row,'rejection':reason}
 result={'status':'VERIFIED_RESEARCH_PARTIAL','claims':[],'new_matching_bytes':0,'inputs_sha256':inputs,'target_words_sha256':targets,'object_sha256':digest(obj),'callback':main,'allocator_context':alloc,'behavior':semantic,'emitted_layout':module('layout').verify(score,out/'layout',require),'negative_controls':negatives,'canonical_or_ROM_verification':False}
 if record:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
 else:require(result==receipt,'sealed receipt changed')
 return result
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--record',action='store_true');p.add_argument('--out',type=Path,default=ROOT/'build/mat3_mine_callback_research_20261017');a=p.parse_args();verify(a.out,a.record);print('PASS research: Mine 499/659 differing, 206 bounded fixtures, three semantic negatives, zero accepted bytes')
