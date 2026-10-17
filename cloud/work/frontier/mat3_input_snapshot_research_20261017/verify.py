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
INPUTS=['README.md','group/process.c','init_behavior.py','init_host_behavior.py','init_negative_controls.py','group/group.c','group/group.json','verify.py','behavior.py','host_behavior.py','negative_controls.py','layout.py']
EXTENTS={TARGET:676,INIT:248,'controller_poll':928,'func_800C9590':76,'player_mode_set':60,'player_state_set':60,'process_inputs':356}
TARGET_EXTENTS=dict(EXTENTS,func_800E7038=252)
CONTEXT=['controller_poll','func_800C9590','player_mode_set','player_state_set',INIT,'process_inputs']
SNAPSHOT_NEGATIVES={'wrong_y_calibration':'host mismatch case 0','collapsed_y_output':'host mismatch case 0','wrong_direction_threshold':'host mismatch case 52','edge_history_overwrite':'host mismatch case 0'}
INIT_NEGATIVES={'misplaced_first_y_reset':'case 18, program 1','word_width_query':'query status is not byte-wide','wrong_queue_capacity':'queue creation protocol','blocked_queue_seed':'queue seed protocol','repeat_player_defaults':'case 54, program 1'}
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def verify_scope(spec):
 require(spec['claims']==[] and spec['members']==[TARGET] and spec['context']==CONTEXT,'research scope changed')
 require(spec['files']==['group.c','process.c'] and spec['keep']==['controller_poll',TARGET,'process_inputs','player_mode_set','player_state_set',INIT],'source/visibility scope changed')
 require(spec['flags']=='-g0 -O3 -mips2 -G 0 -non_shared' and spec['unprototyped']==[] and not spec.get('allow_unverified'),'compiler contract changed')
def inputs():
 targets=score.targets();addresses=score.image_symbols()
 return {
  'inputs_sha256':{n:digest(HERE/n) for n in INPUTS},
  'target_words_sha256':{n:hashlib.sha256(struct.pack('>%dI'%len(targets[n]),*targets[n])).hexdigest() for n in EXTENTS},
  'verification_inputs':{
   'protocol_version':2,
   'toolchain_sha256':{n:digest(Path(score.ido(n))) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']},
   'verifier_sha256':{n:digest(ROOT/n) for n in ['tools/cloud/score.py','tools/cloud/owndata.py','blob_matched.lock.json']},
   'context_sha256':{'cloud/matches/process_inputs.c':digest(ROOT/'cloud/matches/process_inputs.c')},
   'target_manifest_sha256':digest(score.ASM_DIR/'SHA256SUMS'),
   'target_symbols_sha256':digest(score.ASM_DIR/'symbols.json'),
   'targets':{n:{'address':hex(addresses[n]),'bytes':len(targets[n])*4} for n in EXTENTS}}}
def verify_report(r):
 require(r['status']=='VERIFIED_RESEARCH_PARTIAL' and r['claims']==[] and r['new_matching_bytes']==0 and r['canonical_or_ROM_verification'] is False,'false research credit')
 require(r['function_bytes']==676 and r['frame_bytes']==48 and r['function_extents']==EXTENTS,'function extent/frame changed')
 require(r['emitted_functions']==sorted(EXTENTS) and set(r['rows'])==set(EXTENTS),'complete emitted/scored inventory changed')
 require(r['verification_inputs']['protocol_version']==2 and set(r['verification_inputs']['targets'])==set(EXTENTS),'verification protocol changed')
 require(set(r['target_words_sha256'])==set(EXTENTS),'protected target inventory changed')
 for name in EXTENTS:
  row=r['rows'][name];different=108 if name==TARGET else 47 if name==INIT else 0
  require(row['total']*4==TARGET_EXTENTS[name] and r['verification_inputs']['targets'][name]['bytes']==TARGET_EXTENTS[name],'protected extent changed: '+name)
  require(row['differing']==different and not any(row[k] for k in ['extra_words','unresolved','unverified','errors']),'comparison changed: '+name)
  require(row['summary']==(f'{different}/{row["total"]} words differ' if different else 'MATCH'),'summary/count disagreement: '+name)
 require(r['emitted_type_layout']=={'status':'PASS','emitted_layout':[16,0,4,8,9,10],'wrong_reserved_width_rejected':True},'emitted layout changed')
 for name,fixtures,executions in [('native_and_candidate_behavior',1014,2028),('initialization_behavior',216,432)]:
  z=r[name];require(z['status']=='PASS' and z['fixtures']==fixtures and z['executions']==executions,'native behavior coverage changed: '+name)
 require(r['native_and_candidate_behavior']['matching_claim'] is False and r['initialization_behavior']['claims']==[],'semantic match claim changed')
 for name,count in [('host_behavior',1014),('initialization_host',216)]:
  require(r[name]['status']=='PASS' and r[name]['host_fixtures']==count,'host behavior coverage changed: '+name)
 require(r['host_behavior']['matching_claim'] is False,'host match claim changed')
 require(r['negative_controls']==SNAPSHOT_NEGATIVES,'snapshot semantic-negative result changed')
 require(r['initialization_negatives']=={n:{'rejected':True,'reason':reason} for n,reason in INIT_NEGATIVES.items()},'initialization semantic-negative result changed')
def verify_bindings(receipt,current):
 verify_report(receipt)
 for key,value in current.items():require(receipt[key]==value,'stale input binding: '+key)
def verify_seal(actual,receipt):
 verify_report(actual);verify_report(receipt);require(actual==receipt,'sealed verification result changed')
def verify(out,record=False):
 out=Path(out);out.mkdir(parents=True,exist_ok=True)
 spec=json.loads((HERE/'group/group.json').read_text());verify_scope(spec)
 require('__standin_' not in (HERE/'group/group.c').read_text(),'fabricated caller remains')
 current=inputs()
 if not record:
  receipt=json.loads((HERE/'verification.json').read_text());verify_bindings(receipt,current)
 require(digest(HERE/'group/process.c')==digest(ROOT/'cloud/matches/process_inputs.c'),'accepted process_inputs source changed')
 obj=out/'snapshot.o';score.compile_group(HERE/'group',obj);rows={}
 if not record:require(digest(obj)==receipt['object_sha256'],'compiled object changed')
 data,sections=score._elf(obj);text_index=score._text_index(sections)
 all_symbols=[sym for i,sec in enumerate(sections) if sec['type']==2 for sym in score._symbol_table(data,sections,i) if sym['type']==2 and sym['section']==text_index]
 require(sorted(sym['name'] for sym in all_symbols)==sorted(EXTENTS),'emitted function inventory changed')
 extents={sym['name']:sym['size'] for sym in all_symbols};require(extents==EXTENTS,'function extent changed')
 symbols=[sym for sym in all_symbols if sym['name']==TARGET];require(len(symbols)==1,'ambiguous input-state function')
 prologue=score.text_words(obj)[symbols[0]['value']//4]
 require(prologue>>26==9 and (prologue>>21)&31==29 and (prologue>>16)&31==29 and prologue&65535==65536-48,'input-state frame')
 for name in spec['members']+spec['context']:
  with contextlib.redirect_stdout(io.StringIO()):r=score.compare(obj,name,show=0)
  rows[name]=dict(asdict(r),summary=r.summary())
 result={'status':'VERIFIED_RESEARCH_PARTIAL','claims':[],'new_matching_bytes':0,'canonical_or_ROM_verification':False,
  **current,'object_sha256':digest(obj),'function_bytes':676,'frame_bytes':48,'function_extents':extents,'emitted_functions':sorted(sym['name'] for sym in all_symbols),'rows':rows,
  'emitted_type_layout':verify_layout(out),'native_and_candidate_behavior':native_verify(obj),'host_behavior':verify_host(),'negative_controls':verify_negatives(),
  'initialization_behavior':init_verify(obj),'initialization_host':init_host_verify((HERE/'group/group.c').read_text()),'initialization_negatives':init_negatives(HERE/'group')}
 verify_report(result)
 if record:(HERE/'verification.json').write_text(json.dumps(result,indent=2)+'\n')
 else:verify_seal(result,receipt)
 return result
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--record',action='store_true');p.add_argument('--out',type=Path,default=ROOT/'build/mat3_input_snapshot_research_20261017');a=p.parse_args();r=verify(a.out,a.record)
 print('PASS research: snapshot 108/169; 5 existing matches preserved; snapshot 1,014 and initialization 216 native/candidate/host fixtures; 9 semantic negatives rejected; zero new matching bytes')
