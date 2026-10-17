"""The Pak cleanup packet remains research despite zero target-word differences."""
import importlib.util,json,subprocess,sys
from pathlib import Path
import pytest
ROOT=Path(__file__).resolve().parents[2];HERE=ROOT/'cloud/work/frontier/dot_mat3_pak_cleanup_research_20261017'
spec=importlib.util.spec_from_file_location('pak_packet',HERE/'replay.py');packet=importlib.util.module_from_spec(spec);spec.loader.exec_module(packet)
def test_pak_cleanup_scope_and_receipt():
 r=json.loads((HERE/'proof.json').read_text());packet.verify_report(r)
 assert packet.inputs()==r['inputs']
 assert r['claims']==[] and r['new_candidate_bytes']==r['accepted_image_bytes']==0
 assert r['positive']['first']['rows'][packet.NAME]['strict'] is False
 assert len(packet.strict_names())==28

def test_pak_cleanup_stock_replay():
 if not (packet.score.IDO/'cc').exists():pytest.skip('stock IDO unavailable')
 assert packet.verify()==json.loads((HERE/'proof.json').read_text())

def test_pak_cleanup_guards_survive_optimized_python():
 script=r'''
import copy,importlib.util,json,sys
from pathlib import Path
p=Path(sys.argv[1]);spec=importlib.util.spec_from_file_location('p',p/'replay.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);r=json.loads((p/'proof.json').read_text())
def rejected(fn):
 try:fn()
 except RuntimeError:return
 raise RuntimeError('verification accepted a false receipt')
for key,value in [('differing',1),('extra_words',1),('unresolved',['bad']),('unverified',['bad']),('errors',['bad']),('total',1),('bytes',1),('strict',False)]:
 b=copy.deepcopy(r)
 for label in ['first','repeat']:b['positive'][label]['rows']['audio_reverb_update'][key]=value
 rejected(lambda:m.verify_report(b))
for key,value in [('extra_words',0),('bytes',208),('frame',32),('strict',True)]:
 b=copy.deepcopy(r)
 for label in ['first','repeat']:b['positive'][label]['rows'][m.NAME][key]=value
 rejected(lambda:m.verify_report(b))
for mutation in [lambda x:x.update(claims=[m.NAME]),lambda x:x.update(new_candidate_bytes=204),lambda x:x.update(accepted_image_bytes=204),lambda x:x['positive']['first']['emitted_functions'].pop(),lambda x:x['positive']['first']['rows'].pop('func_800A43FC'),lambda x:x['semantic_negatives'].update(skip_callback='generic error'),lambda x:x['layout'].update(wrong_stride_emitted=x['layout']['emitted'])]:
 b=copy.deepcopy(r);mutation(b);rejected(lambda:m.verify_report(b))
original=m.inputs
for key in ['packet_sha256','context_sha256','toolchain_sha256','verifier_sha256']:
 b=original();n=next(iter(b[key]));b[key][n]='0'*64;m.inputs=lambda:b;rejected(lambda:m.verify())
m.inputs=original;b=original();b['targets'][m.NAME]['sha256']='0'*64;m.inputs=lambda:b;rejected(lambda:m.verify())
print('optimized guards reject false credit, diagnostics, extents, inventory, semantics and stale inputs')
'''
 r=subprocess.run([sys.executable,'-O','-c',script,str(HERE)],capture_output=True,text=True)
 assert r.returncode==0,(r.stdout,r.stderr)
 assert r.stdout=='optimized guards reject false credit, diagnostics, extents, inventory, semantics and stale inputs\n'
