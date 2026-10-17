"""Replay the controller snapshot research with empty matching claims."""
from pathlib import Path
import argparse,contextlib,hashlib,io,json,struct,sys
from dataclasses import asdict
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT/'tools/cloud'));import score
from behavior import require,verify as native_verify
from host_behavior import verify_host
from negative_controls import verify_negatives
from layout import verify_layout
TARGET='func_800E7134'
PRESERVED=['controller_poll','func_800C9590','player_mode_set','player_state_set']
INPUTS=['group/group.c','group/group.json','verify.py','behavior.py','host_behavior.py','negative_controls.py','layout.py']
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def verify(out,record=False):
 out=Path(out);out.mkdir(parents=True,exist_ok=True)
 spec=json.loads((HERE/'group/group.json').read_text());require(spec['claims']==[] and spec['members']==[TARGET],'research scope changed')
 require('__standin_' not in (HERE/'group/group.c').read_text(),'fabricated caller remains')
 inputs={n:digest(HERE/n) for n in INPUTS}
 target_hashes={name:hashlib.sha256(struct.pack('>%dI'%len(score.targets()[name]),*score.targets()[name])).hexdigest() for name in [TARGET,*PRESERVED]}
 if not record:
  receipt=json.loads((HERE/'verification.json').read_text());require(receipt['inputs_sha256']==inputs,'source changed; reverify receipt');require(receipt['target_words_sha256']==target_hashes,'target changed')
 obj=out/'snapshot.o';score.compile_group(HERE/'group',obj);rows={}
 data,sections=score._elf(obj);text_index=score._text_index(sections);symbols=[sym for i,sec in enumerate(sections) if sec['type']==2 for sym in score._symbol_table(data,sections,i) if sym['name']==TARGET and sym['type']==2 and sym['section']==text_index]
 require(len(symbols)==1 and symbols[0]['size']==676,'input-state function extent')
 prologue=score.text_words(obj)[symbols[0]['value']//4]
 require(prologue>>26==9 and (prologue>>21)&31==29 and (prologue>>16)&31==29 and prologue&65535==65536-48,'input-state frame')
 for name in spec['members']+spec['context']:
  with contextlib.redirect_stdout(io.StringIO()):r=score.compare(obj,name,show=0)
  rows[name]=dict(asdict(r),summary=r.summary())
 for name in [TARGET,*PRESERVED]:
  require(not any(rows[name][k] for k in ['extra_words','unresolved','unverified','errors']),f'uncertain bytes in {name}')
  require(rows[name]['differing']==(108 if name==TARGET else 0),f'changed comparison {name}')
 result={'status':'VERIFIED_RESEARCH_PARTIAL','claims':[],'new_matching_bytes':0,'canonical_or_ROM_verification':False,
  'inputs_sha256':inputs,'target_words_sha256':target_hashes,'object_sha256':digest(obj),'function_bytes':676,'frame_bytes':48,'rows':rows,
  'emitted_type_layout':verify_layout(out),'native_and_candidate_behavior':native_verify(obj),'host_behavior':verify_host(),'negative_controls':verify_negatives()}
 if record:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
 return result
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--record',action='store_true');p.add_argument('--out',type=Path,default=ROOT/'build/mat3_input_snapshot_research_20261017');a=p.parse_args();r=verify(a.out,a.record)
 print('PASS research: snapshot 108/169; 4 existing matches preserved; 1,014 native/candidate/host fixtures; 4 semantic negatives rejected; zero new matching bytes')
