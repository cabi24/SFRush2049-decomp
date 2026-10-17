"""The controller reconstruction remains partial with a fully sealed receipt."""
from pathlib import Path
import subprocess,sys
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/mat3_input_snapshot_research_20261017'
IMPORT=r'''
import copy,importlib.util,json,sys
from pathlib import Path
p=Path(sys.argv[1]);sys.path.insert(0,str(p));spec=importlib.util.spec_from_file_location('input_packet',p/'verify.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
r=json.loads((p/'verification.json').read_text())
'''
def run(script,optimized=False):
 r=subprocess.run([sys.executable,*(['-O'] if optimized else []),'-c',IMPORT+script,str(HERE)],capture_output=True,text=True)
 assert r.returncode==0,(r.stdout,r.stderr)
 return r.stdout

def test_input_snapshot_scope_and_receipt():
 assert run("m.verify_scope(json.loads((p/'group/group.json').read_text()));m.verify_bindings(r,m.inputs());print('scope and input bindings pass')\n")=='scope and input bindings pass\n'

def test_input_snapshot_optimized_full_replay(tmp_path):
 script=f"actual=m.verify(Path({str(tmp_path)!r}));m.verify_seal(actual,r);print('complete optimized replay passes')\n"
 assert run(script,True)=='complete optimized replay passes\n'

def test_input_snapshot_optimized_refusals():
 script=r'''
count=0
def rejected(fn):
 global count
 try:fn()
 except ValueError:count+=1;return
 raise ValueError('verification accepted a false receipt')
for name in m.EXTENTS:
 for key,value in [('differing',r['rows'][name]['differing']+1),('total',r['rows'][name]['total']+1),('extra_words',1),('unresolved',['bad']),('unverified',['bad']),('errors',['bad']),('summary','false match')]:
  b=copy.deepcopy(r);b['rows'][name][key]=value;rejected(lambda:m.verify_report(b))
 b=copy.deepcopy(r);b['function_extents'][name]+=4;rejected(lambda:m.verify_report(b))
 b=copy.deepcopy(r);b['verification_inputs']['targets'][name]['bytes']+=4;rejected(lambda:m.verify_report(b))
for mutate in [lambda x:x.update(claims=[m.TARGET]),lambda x:x.update(new_matching_bytes=676),lambda x:x.update(canonical_or_ROM_verification=True),lambda x:x.update(status='MATCH'),lambda x:x.update(frame_bytes=56),lambda x:x.update(function_bytes=680),lambda x:x['emitted_functions'].pop(),lambda x:x['rows'].pop('process_inputs'),lambda x:x['verification_inputs'].update(protocol_version=1),lambda x:x['target_words_sha256'].pop(m.INIT),lambda x:x['emitted_type_layout'].update(wrong_reserved_width_rejected=False),lambda x:x['native_and_candidate_behavior'].update(fixtures=0),lambda x:x['native_and_candidate_behavior'].update(executions=1),lambda x:x['native_and_candidate_behavior'].update(matching_claim=True),lambda x:x['initialization_behavior'].update(claims=[m.INIT]),lambda x:x['initialization_host'].update(host_fixtures=0),lambda x:x['host_behavior'].update(matching_claim=True),lambda x:x['negative_controls'].update(wrong_y_calibration='generic error'),lambda x:x['initialization_negatives']['word_width_query'].update(reason='generic error')]:
 b=copy.deepcopy(r);mutate(b);rejected(lambda:m.verify_report(b))
current=m.inputs();m.verify_bindings(r,current)
for key in ['inputs_sha256','target_words_sha256']:
 b=copy.deepcopy(current);b[key][next(iter(b[key]))]='0'*64;rejected(lambda:m.verify_bindings(r,b))
for key in ['toolchain_sha256','verifier_sha256','context_sha256']:
 b=copy.deepcopy(current);z=b['verification_inputs'][key];z[next(iter(z))]='0'*64;rejected(lambda:m.verify_bindings(r,b))
b=copy.deepcopy(current);b['verification_inputs']['toolchain_sha256']['ujoin']='0'*64;rejected(lambda:m.verify_bindings(r,b))
for key in ['target_manifest_sha256','target_symbols_sha256']:
 b=copy.deepcopy(current);b['verification_inputs'][key]='0'*64;rejected(lambda:m.verify_bindings(r,b))
b=copy.deepcopy(current);b['verification_inputs']['targets'][m.TARGET]['address']='0x80000000';rejected(lambda:m.verify_bindings(r,b))
for mutate in [lambda x:x.update(object_sha256='0'*64),lambda x:x['native_and_candidate_behavior'].update(model='changed model description'),lambda x:x['initialization_behavior'].update(native_words_executed=0),lambda x:x['host_behavior'].update(source='different source')]:
 b=copy.deepcopy(r);mutate(b);rejected(lambda:m.verify_seal(b,r))
scope=json.loads((p/'group/group.json').read_text())
for mutate in [lambda x:x.update(claims=[m.TARGET]),lambda x:x['context'].pop(),lambda x:x['files'].append('extra.c'),lambda x:x['keep'].pop(),lambda x:x.update(flags='-O2'),lambda x:x.update(unprototyped=['osRecvMesg']),lambda x:x.update(allow_unverified=True)]:
 b=copy.deepcopy(scope);mutate(b);rejected(lambda:m.verify_scope(b))
print('optimized refusal cases:',count)
'''
 assert run(script,True)=='optimized refusal cases: 102\n'

def test_input_snapshot_missing_ido_diagnostic(tmp_path):
 script=f"""
m.score.IDO=Path({str(tmp_path/'missing_ido')!r})
try:m.inputs()
except SystemExit as e:
 if 'run tools/cloud/setup.sh' not in str(e):raise
else:raise ValueError('missing IDO did not use standard diagnostic')
print('standard missing-IDO diagnostic passes')
"""
 assert run(script,True)=='standard missing-IDO diagnostic passes\n'
