"""Research-only LoadCars scope, complete context, and optimized guard checks."""
import importlib.util,json,subprocess,sys,textwrap
from pathlib import Path
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_mat3_loadcars_research_20261017'
spec=importlib.util.spec_from_file_location('mat3_loadcars_research',HERE/'replay.py')
packet=importlib.util.module_from_spec(spec);spec.loader.exec_module(packet)

def test_source_bound_research_receipt():
 saved=json.loads((HERE/'proof.json').read_text())
 packet.verify_scope(packet.recipe());packet.verify_context();packet.verify_report(saved)
 assert packet.inputs()==saved['inputs']
 assert saved['claims']==[] and saved['new_candidate_bytes']==0
 assert saved['positive']['first']['rows'][packet.NAME]['bytes']==1372
 assert saved['positive']['first']['rows'][packet.UNMATCHED]['bytes']==432
 assert len(saved['semantic_negatives'])==6

def test_stock_full_replay():
 if not (packet.score.IDO/'cc').exists():pytest.skip('stock IDO unavailable')
 assert packet.verify()==json.loads((HERE/'proof.json').read_text())

def test_optimized_guards_reject_false_or_stale_proof():
 code=r'''
import copy,importlib.util,json,shutil,tempfile
from pathlib import Path
path=Path(__PACKET__)
spec=importlib.util.spec_from_file_location('guard_packet',path/'replay.py')
p=importlib.util.module_from_spec(spec);spec.loader.exec_module(p)
saved=json.loads((path/'proof.json').read_text())
def rejects(fn):
 try:fn()
 except RuntimeError:return
 raise RuntimeError('optimized guard accepted a negative')
bad=copy.deepcopy(saved);bad['claims']=[p.NAME];rejects(lambda:p.verify_report(bad))
bad=copy.deepcopy(saved);bad['new_candidate_bytes']=1372;rejects(lambda:p.verify_report(bad))
bad=copy.deepcopy(saved);bad['positive']['first']['rows'][p.NAME]['unverified']=['injected'];rejects(lambda:p.verify_report(bad))
bad=copy.deepcopy(saved);bad['positive']['first']['rows'][p.PRESERVED[0]]['bytes']=12;rejects(lambda:p.verify_report(bad))
bad=copy.deepcopy(saved);bad['positive']['first']['rows'][p.UNMATCHED]['strict']=True;rejects(lambda:p.verify_report(bad))
with tempfile.TemporaryDirectory() as tmp:
 d=Path(tmp)/'packet';shutil.copytree(path,d);p.HERE=d
 for name in ['group/root.c','replay.py']:
  original=(d/name).read_text();(d/name).write_text(original+'\n/* changed receipt input */\n');rejects(lambda:p.verify());(d/name).write_text(original)
 bad=copy.deepcopy(saved);bad['inputs']['targets'][p.NAME]['sha256']='0'*64;(d/'proof.json').write_text(json.dumps(bad));rejects(lambda:p.verify());(d/'proof.json').write_text(json.dumps(saved))
 for key,item in [('toolchain_sha256','uopt'),('verifier_sha256','tools/cloud/score.py')]:
  bad=copy.deepcopy(saved);bad['inputs'][key][item]='0'*64;(d/'proof.json').write_text(json.dumps(bad));rejects(lambda:p.verify());(d/'proof.json').write_text(json.dumps(saved))
 recipe=json.loads((d/'group/group.json').read_text());recipe['claims']=[p.NAME];(d/'group/group.json').write_text(json.dumps(recipe));rejects(lambda:p.verify())
print('optimized guards rejected false claims, credit, relocation, extent, loader status, source, verifier, target/compiler/scorer receipts and manifest')
'''.replace('__PACKET__',repr(str(HERE)))
 run=subprocess.run([sys.executable,'-O','-c',textwrap.dedent(code)],capture_output=True,text=True)
 assert run.returncode==0,run.stderr
 assert run.stdout.startswith('optimized guards rejected')
