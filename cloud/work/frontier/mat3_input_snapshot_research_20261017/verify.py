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
from init_behavior import verify as init_verify
from init_host_behavior import verify_host as init_host_verify
from init_negative_controls import verify_negatives as init_negatives
TARGET='func_800E7134'
PRESERVED=['controller_poll','func_800C9590','player_mode_set','player_state_set','process_inputs']
INIT='func_800E7038'
INPUTS=['group/process.c','init_behavior.py','init_host_behavior.py','init_negative_controls.py','group/group.c','group/group.json','verify.py','behavior.py','host_behavior.py','negative_controls.py','layout.py']
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def verify(out,record=False):
 out=Path(out);out.mkdir(parents=True,exist_ok=True)
 spec=json.loads((HERE/'group/group.json').read_text());require(spec['claims']==[] and spec['members']==[TARGET],'research scope changed')
 require('__standin_' not in (HERE/'group/group.c').read_text(),'fabricated caller remains')
 inputs={n:digest(HERE/n) for n in INPUTS}
 target_hashes={name:hashlib.sha256(struct.pack('>%dI'%len(score.targets()[name]),*score.targets()[name])).hexdigest() for name in [TARGET,INIT,*PRESERVED]}
 if not record:
  receipt=json.loads((HERE/'verification.json').read_text());require(receipt['inputs_sha256']==inputs,'source changed; reverify receipt');require(receipt['target_words_sha256']==target_hashes,'target changed')
 require(digest(HERE/'group/process.c')==digest(ROOT/'cloud/matches/process_inputs.c'),'accepted process_inputs source changed')
 obj=out/'snapshot.o';score.compile_group(HERE/'group',obj);rows={}
 if not record:require(digest(obj)==receipt['object_sha256'],'compiled object changed')
 data,sections=score._elf(obj);text_index=score._text_index(sections);symbols=[sym for i,sec in enumerate(sections) if sec['type']==2 for sym in score._symbol_table(data,sections,i) if sym['name']==TARGET and sym['type']==2 and sym['section']==text_index]
 all_symbols=[sym for i,sec in enumerate(sections) if sec['type']==2 for sym in score._symbol_table(data,sections,i) if sym['type']==2 and sym['section']==text_index]
 extents={sym['name']:sym['size'] for sym in all_symbols}
 expected_extents={TARGET:676,INIT:248,'controller_poll':928,'func_800C9590':76,'player_mode_set':60,'player_state_set':60,'process_inputs':356}
 require(all(extents.get(name)==size for name,size in expected_extents.items()),'function extent changed')
 require(len(symbols)==1,'ambiguous input-state function')
 prologue=score.text_words(obj)[symbols[0]['value']//4]
 require(prologue>>26==9 and (prologue>>21)&31==29 and (prologue>>16)&31==29 and prologue&65535==65536-48,'input-state frame')
 for name in spec['members']+spec['context']:
  with contextlib.redirect_stdout(io.StringIO()):r=score.compare(obj,name,show=0)
  rows[name]=dict(asdict(r),summary=r.summary())
 for name in [TARGET,INIT,*PRESERVED]:
  require(not any(rows[name][k] for k in ['extra_words','unresolved','unverified','errors']),f'uncertain bytes in {name}')
  require(rows[name]['differing']==(108 if name==TARGET else 47 if name==INIT else 0),f'changed comparison {name}')
 result={'status':'VERIFIED_RESEARCH_PARTIAL','claims':[],'new_matching_bytes':0,'canonical_or_ROM_verification':False,
  'inputs_sha256':inputs,'target_words_sha256':target_hashes,'object_sha256':digest(obj),'function_bytes':676,'frame_bytes':48,'function_extents':expected_extents,'rows':rows,
  'emitted_type_layout':verify_layout(out),'native_and_candidate_behavior':native_verify(obj),'host_behavior':verify_host(),'negative_controls':verify_negatives(),
  'initialization_behavior':init_verify(obj),'initialization_host':init_host_verify((HERE/'group/group.c').read_text()),'initialization_negatives':init_negatives(HERE/'group')}
 if record:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
 return result
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--record',action='store_true');p.add_argument('--out',type=Path,default=ROOT/'build/mat3_input_snapshot_research_20261017');a=p.parse_args();r=verify(a.out,a.record)
 print('PASS research: snapshot 108/169; 5 existing matches preserved; snapshot 1,014 and initialization 216 native/candidate/host fixtures; 9 semantic negatives rejected; zero new matching bytes')
