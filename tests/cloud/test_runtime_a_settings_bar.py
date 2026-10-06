"""Frozen research result and actual replay, with no matching submission."""
import hashlib,json,os,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/runtime_a_settings_bar_20261006'
def test_frozen_nonmatch_and_source_binding():
 r=json.loads((PACKET/'verification.json').read_text())
 assert r['status']=='NONMATCH' and r['claim'] is False and r['accepted_bytes']==0
 assert r['bytes']==772 and r['words']==193 and r['function_bytes']==772
 assert r['alignment_bytes']==12 and r['owned_data_bytes']==0
 assert r['comparison']==dict(differing=4,total=193,unresolved=[],unverified=[],errors=[],extra_words=0)
 assert r['residual_offsets']==['0x8c','0x9c','0xd0','0xe4']
 assert r['source_sha256']==hashlib.sha256((PACKET/'candidate.c').read_bytes()).hexdigest()
 for name,digest in r['support_sha256'].items():assert digest==hashlib.sha256((PACKET/name).read_bytes()).hexdigest()
 assert not (ROOT/'cloud/matches/ovl_a/func_803AE63C.c').exists()
def test_complete_bounded_execution_and_negative_controls():
 r=json.loads((PACKET/'verification.json').read_text());b=r['behavior']
 assert b['cases']==1864 and b['executions']==3728
 assert b['target_instructions_executed']==193 and b['hidden_instructions_executed']==15
 assert b['unexecuted_offsets']==[]
 assert set(r['mutants'])=={'wrong_alpha_cap','wrong_first_divisor','wrong_marker_bits','wrong_return'}
 assert all(v['witness'] and v['reason'] for v in r['mutants'].values())
def test_replay_current_protected_inputs():
 repo=Path(os.environ.get('RUSH_TRUSTED_REPO',ROOT)).resolve()
 tools=Path(os.environ.get('RUSH_TOOLS_REPO',repo)).resolve()
 p=subprocess.run([sys.executable,str(PACKET/'verify.py'),'--repo',str(repo),'--tools-repo',str(tools),'--check'],capture_output=True,text=True)
 assert p.returncode==0,p.stdout+p.stderr
