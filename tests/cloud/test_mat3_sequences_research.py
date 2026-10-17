"""Research-only sequence proof: complete closure accounting and robust guards."""
import copy
import importlib.util
import json
from pathlib import Path
import subprocess
import sys
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_mat3_sequences_research_20261017'
spec=importlib.util.spec_from_file_location('sequences_packet',HERE/'replay.py')
packet=importlib.util.module_from_spec(spec);spec.loader.exec_module(packet)
def test_sequences_receipt_and_scope():
 saved=json.loads((HERE/'proof.json').read_text());packet.verify_report(saved)
 assert packet.inputs()==saved['inputs']
 assert packet.recipe()['claims']==[]
 assert len(packet.preserved())==16 and len(packet.names())==20
 assert saved['new_candidate_bytes']==saved['accepted_image_bytes']==0
 assert saved['positive']['first']==saved['positive']['repeat']
 assert saved['residual']['accepted'] is False
 assert len(saved['semantic_negatives'])==3

def test_sequences_stock_replay():
 if not (packet.score.IDO/'cc').exists():pytest.skip('stock IDO unavailable')
 assert packet.verify()==json.loads((HERE/'proof.json').read_text())

def test_sequences_guards_survive_optimized_python():
 script=r'''
import copy,importlib.util,json,sys
from pathlib import Path
p=Path(sys.argv[1]);spec=importlib.util.spec_from_file_location('packet',p/'replay.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
r=json.loads((p/'proof.json').read_text())
def rejected(fn):
 try:fn()
 except RuntimeError:return
 raise RuntimeError('expected an explicit verification rejection')
for mutation in [
 lambda r:r.update(claims=[m.NAME]),
 lambda r:r.update(new_candidate_bytes=380),
 lambda r:r['positive']['first']['rows'][m.NAME].update(bytes=384),
 lambda r:r['positive']['first']['rows'][m.NAME].update(frame=32),
 lambda r:r['positive']['first']['rows'][m.NAME].update(strict=True),
 lambda r:r['positive']['first']['rows']['engine_sound_sync'].update(strict=False),
 lambda r:r['positive']['first']['emitted_functions'].pop(),
 lambda r:r['residual'].update(accepted=True),
 lambda r:r['semantic_negatives'].update(wrong_state='generic nonzero exit'),
 lambda r:r['controls']['real_thread']['rows']['entity_ai_pathfind'].update(errors=[]),
 lambda r:r['layout'].update(wrong_width_emitted=r['layout']['emitted']),
]:
 bad=copy.deepcopy(r);mutation(bad);rejected(lambda:m.verify_report(bad))
for key,value in [('differing',1),('extra_words',1),('unresolved',['bad']),('unverified',['bad']),('errors',['bad']),('total',1)]:
 bad=copy.deepcopy(r)
 for label in ['first','repeat']:bad['positive'][label]['rows']['engine_sound_sync'][key]=value
 rejected(lambda:m.verify_report(bad))
original=m.inputs
for key in ['toolchain_sha256','verifier_sha256']:
 bad=original();name=next(iter(bad[key]));bad[key][name]='0'*64;m.inputs=lambda:bad;rejected(lambda:m.verify())
m.inputs=original
print('optimized proof guards reject mutations and stale compiler/verifier inputs')
'''
 r=subprocess.run([sys.executable,'-O','-c',script,str(HERE)],capture_output=True,text=True)
 assert r.returncode==0,(r.stdout,r.stderr)
 assert r.stdout=='optimized proof guards reject mutations and stale compiler/verifier inputs\n'

def test_sequences_missing_compiler_diagnostic(monkeypatch, tmp_path):
 monkeypatch.setattr(packet.score,'IDO',tmp_path/'missing_ido')
 with pytest.raises(SystemExit,match='run tools/cloud/setup.sh'):
  packet.inputs()
