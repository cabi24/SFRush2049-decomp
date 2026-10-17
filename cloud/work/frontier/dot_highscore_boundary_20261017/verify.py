#!/usr/bin/env python3
"""Fail-closed research replay. Partial classifications never upgrade strict acceptance."""
import contextlib,dataclasses,hashlib,io,json,re,shutil,struct,sys,tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
ROOT=next(p for p in HERE.parents if (p/'tools/cloud/score.py').is_file())
sys.path.insert(0,str(ROOT))
from tools.cloud import score,owndata
from residual_proof import proof
from window_proof import check,require

def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def emitted_sections(path):
 obj=owndata._Object(path)
 return {s['name']:hashlib.sha256(obj.data[s['off']:s['off']+s['size']]).hexdigest() for s in obj.sections if s['name'] and s['name']!='.mdebug'}
def describe(r):
 scrub=lambda text:re.sub(r'retail [0-9a-f]+, got [0-9a-f]+','compared byte values omitted',text)
 row={**dataclasses.asdict(r),'accepted':r.accepted(),'summary':scrub(r.summary()),'notes':[scrub(t) for t in r.notes]}
 row['errors']=[scrub(t) for t in row['errors']]
 return row
def measure(group,obj,names):
 score.compile_group(group,obj)
 with contextlib.redirect_stdout(io.StringIO()):return {n:describe(score.compare(obj,n,show=0)) for n in names}
def residuals(obj):
 with contextlib.redirect_stdout(io.StringIO()):return {n:proof(obj,n) for n in ['highscore_animText','DrawGhost']}
def validate_inputs():
 group=HERE/'group';spec=json.loads((group/'group.json').read_text());pins=json.loads((HERE/'inputs.json').read_text())
 require(spec['claims']==[] and spec['context']==[],'Research-only scope changed')
 require(len(spec['members'])==len(set(spec['members']))==31,'Member inventory changed')
 require(set(spec['files'])|{'group.json'}==set(pins['source_sha256']),'Source inventory changed')
 require(set(spec['members'])==set(pins['target_words_sha256']),'Target inventory changed')
 for n,h in pins['source_sha256'].items():require(digest(group/n)==h,'Source fingerprint changed: '+n)
 for n,h in pins['target_words_sha256'].items():
  w=score.targets()[n];require(hashlib.sha256(struct.pack('>%dI'%len(w),*w)).hexdigest()==h,'Target fingerprint changed: '+n)
 return spec

def main():
 spec=validate_inputs();expected=json.loads((HERE/'verification.json').read_text());shape=json.loads((HERE/'residual_expected.json').read_text());fidelity=json.loads((HERE/'fidelity.json').read_text())
 require(fidelity['byte_identical'] and fidelity['claims']==[] and fidelity['new_accepted_credit']==0,'Fidelity/research scope changed')
 with tempfile.TemporaryDirectory(prefix='highscore-research-') as tmp:
  root=Path(tmp);base=root/'base';shutil.copytree(HERE/'group',base);obj=root/'base.o';rows=measure(base,obj,spec['members'])
  require(rows==expected,'Complete native member receipt changed');require(digest(obj)==fidelity['candidate_object_sha256'],'Whole stock object changed');require(residuals(obj)==shape,'Partial residual/content proof changed')
  require(rows['DrawUnlocked']['accepted'] and rows['DrawName']['accepted'] and rows['object_create']['accepted'] and rows['world_trigger_check']['accepted'] and rows['func_800B6748']['accepted'],'Genuine graph helper result changed')
  for n in ['highscore_animText','DrawGhost','object_bytes23_sum','object_bytes_sum_global','world_velocity_integrate']:require(not rows[n]['accepted'],'Partial body silently upgraded: '+n)
  output={'members':rows,'partial_proof':shape,'negatives':{},'equivalence_controls':{},'claims':[],'new_accepted_credit':0}
  source=(base/'highscore.c').read_text();a=source.index('void DrawUnlocked(void)\n{');b=source.index('\nvoid DrawCircuitCode',a);unlocked=source[a:b]
  early=unlocked.replace('        if (D_80142694[p]) {','        if (D_80142694[p] == 0) continue;').replace('\n        }\n    }\n}','\n    }\n}')
  transmission=unlocked.replace('i < 8; i++','i < 4; i++').replace('D_80150EB8[p][i]','D_80150EB8[p][i * 2]').replace('D_80142780[p][i]','D_80142780[p][i * 2]')
  tests={
   'unlocked_short_x':('highscore.c',unlocked,unlocked.replace('    s32 x;','    s16 x;'),1,'DrawUnlocked','strict'),
   'unlocked_early_continue':('highscore.c',unlocked,early,1,'DrawUnlocked','strict'),
   'unlocked_gc_transmission':('highscore.c',unlocked,transmission,1,'DrawUnlocked','strict'),
   'wrong_unlocked_format':('highscore.c','fcvt_wrapper(str, "%s %s",','fcvt_wrapper(str, "%s:%s",',5,'DrawUnlocked','strict'),
   'wrong_name_static':('highscore.c','static unsigned char del[4] = "DEL";','static unsigned char del[4] = "BAD";',1,'DrawName','strict'),
   'wrong_score_font':('highscore.c','    object_create(13);\n    dispatch_handler(1);\n    state_utility(sound_pitch_diff_halved(countdown_state.strings[','    object_create(12);\n    dispatch_handler(1);\n    state_utility(sound_pitch_diff_halved(countdown_state.strings[',1,'highscore_animText',False),
   'wrong_ellipse_extent':('text_helpers.c','u8 label[128];','u8 label[256];',1,'highscore_animText',False),
   'wrong_score_literal':('highscore.c','sprintf((char *)str, "%d", i + 1);','sprintf((char *)str, "%u", i + 1);',1,'highscore_animText',True),
   'wrong_time_literal':('highscore.c','fcvt_wrapper(time, "%01d:%02d.%03d",','fcvt_wrapper(time, "%01d:%02d.%04d",',2,'DrawGhost',True),
   'wrong_justification_data':('ctx_gr3_b.c','Just D_80118E20 = {0, 3};','Just D_80118E20 = {0, 0};',1,'DrawGhost',True),
   'old_unused_local_placement':('highscore.c','    unsigned char *str;\n    unsigned char str1[128];\n    unsigned char time[16];\n    s16 gFreePages;','    unsigned char *str;\n    s16 gFreePages;\n    unsigned char str1[128];\n    unsigned char time[16];',1,'DrawGhost',False),
  }
  for label,(file,old,new,count,target,literal) in tests.items():
   child=root/label;shutil.copytree(base,child);p=child/file;s=p.read_text();require(s.count(old)==count,'Stale source mutation: '+label);p.write_text(s.replace(old,new,1));out=root/(label+'.o');changed=measure(child,out,spec['members']);rejected=False
   try:
    with contextlib.redirect_stdout(io.StringIO()):
     if literal=='strict':require(changed[target]['accepted'],'Strict source/content equality rejected: '+target)
     elif literal:check(out,target)
     else:proof(out,target)
   except RuntimeError as exc:rejected=True;reason=str(exc)
   require(rejected,'Mutation escaped narrow proof: '+label)
   output['negatives'][label]={'rejected':True,'reason':reason,'target':target,'receipt':changed[target]}
  child=root/'remove_unused_local';shutil.copytree(base,child);p=child/'highscore.c';s=p.read_text();require(s.count('    s16 gFreePages;')==1,'Stale unused-local control');p.write_text(s.replace('    s16 gFreePages;\n',''));out=root/'remove_unused_local.o';changed=measure(child,out,spec['members']);require(changed==expected and residuals(out)==shape,'Unused-local equivalence changed');require(emitted_sections(out)==emitted_sections(obj),'Unused-local control changed emitted sections')
  output['equivalence_controls']['remove_unused_ghost_local']={'all_member_receipts_equal':True,'emitted_code_data_relocations_symbols_equal':True,'whole_object_equal':digest(out)==digest(obj),'debug_metadata_size_changes':True}
  print(json.dumps(output,indent=2,sort_keys=True))
if __name__=='__main__':main()
