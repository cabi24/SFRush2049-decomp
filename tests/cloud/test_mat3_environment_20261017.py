"""Bounded environment proof, sealed reports and clean-root replay."""
from pathlib import Path
import json,shutil,subprocess,sys
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/mat3_environment_20261017'
PRELUDE='''import copy,importlib.util,json,sys
from pathlib import Path
p=Path(sys.argv[1]);s=importlib.util.spec_from_file_location('environment_proof',p/'verify.py');v=importlib.util.module_from_spec(s);s.loader.exec_module(v)
r=json.loads((p/'verification.json').read_text())
'''
def run(script,packet=PACKET):
    result=subprocess.run([sys.executable,'-O','-c',PRELUDE+script,str(packet)],cwd=ROOT,text=True,capture_output=True)
    assert result.returncode==0,result.stdout+result.stderr
    return result.stdout

def test_environment_research_packet(tmp_path):
    assert run(f"v.verify(Path({str(tmp_path)!r}));print('full replay passes')\n")=='full replay passes\n'

def test_environment_guards_survive_optimized_python():
    script='''
count=0
def reject(call):
 global count
 try:call()
 except ValueError:count+=1;return
 raise RuntimeError('optimized interpreter accepted an altered proof')
v.verify_report(r)
for key in ['comparison','original_flag_control']:
 for field,value in [('differing',0),('total',247),('extra_words',1),('unresolved',['bad']),('unverified',['bad']),('errors',['bad'])]:
  z=copy.deepcopy(r);z[key][field]=value;reject(lambda:v.verify_report(z))
for change in [lambda x:x.update(claims=['music_control']),lambda x:x.update(new_matching_bytes=992),lambda x:x.update(canonical_or_ROM_verification=True),lambda x:x.update(protocol_version=1),lambda x:x.update(status='MATCH'),lambda x:x.update(complete_symbol_bytes=988),lambda x:x.update(candidate_frame_bytes=160),lambda x:x.update(native_frame_bytes=112),lambda x:x['inventory']['functions'].update(extra={'offset':992,'bytes':8}),lambda x:x['inventory'].update(text_bytes=1008),lambda x:x['behavior'].update(fixtures=1),lambda x:x['behavior'].update(executions=512),lambda x:x['behavior'].update(executed_words=[248,248]),lambda x:x['behavior'].update(branch_outcomes=[34,35]),lambda x:x['behavior'].update(matching_claim=True),lambda x:x['emitted_type_layout'].update(wrong_capacity_rejected=False),lambda x:x['negative_controls']['shadow_alpha'].update(first_divergent_case=0),lambda x:x['negative_controls'].pop('arrow_z')]:
 z=copy.deepcopy(r);change(z);reject(lambda:v.verify_report(z))
spec=json.loads((p/'group/group.json').read_text());v.verify_scope(spec)
for change in [lambda x:x.update(claims=['music_control']),lambda x:x.update(members=[]),lambda x:x.update(context=['extra']),lambda x:x.update(files=['fake.c']),lambda x:x.update(keep=[]),lambda x:x.update(flags='-O2'),lambda x:x.update(allow_unverified=True),lambda x:x.update(unprototyped=['callee'])]:
 z=copy.deepcopy(spec);change(z);reject(lambda:v.verify_scope(z))
for change in [lambda x:x.update(object_sha256='0'*64),lambda x:x['comparison'].update(summary='false'),lambda x:x['environment_inputs']['compiler'].update(uopt='0'*64),lambda x:x['behavior'].update(limits='unbounded')]:
 z=copy.deepcopy(r);change(z);reject(lambda:v.verify_seal(z,r))
print('optimized refusals',count)
'''
    assert run(script)=='optimized refusals 42\n'

def test_environment_relocated_binding(tmp_path):
    root=tmp_path/'new_root';packet=root/'cloud/work/frontier'/PACKET.name;packet.parent.mkdir(parents=True)
    shutil.copytree(PACKET,packet,ignore=shutil.ignore_patterns('__pycache__'))
    for path in ['tools/cloud','asm/us/blob','asm/us/blob_data','tests/cloud/test_mat3_environment_20261017.py','blob_matched.lock.json']:
        src=ROOT/path;dst=root/path
        if not src.exists():continue
        dst.parent.mkdir(parents=True,exist_ok=True)
        if src.is_dir():shutil.copytree(src,dst,ignore=shutil.ignore_patterns('__pycache__','ido'))
        else:shutil.copyfile(src,dst)
    assert run(f"v.verify(Path({str(tmp_path/'objects')!r}));print('relocated replay passes')\n",packet)=='relocated replay passes\n'
    changes=[('verification.json',lambda x:x.update(claims=['false-credit'])),('verification.json',lambda x:x.update(new_matching_bytes=992)),('verification.json',lambda x:x['comparison'].update(unverified=['bad'])),('verification.json',lambda x:x['inputs_sha256'].update({'group/group.c':'0'*64})),('verification.json',lambda x:x.update(target_words_sha256='0'*64)),('verification.json',lambda x:x['environment_inputs']['tools'].update({'score.py':'0'*64})),('verification.json',lambda x:x['environment_inputs']['compiler'].update(ujoin='0'*64)),('verification.json',lambda x:x['comparison'].update(summary='invented')),('trace_fidelity.json',lambda x:x['inputs_sha256'].update({'verify.py':'0'*64})),('trace_fidelity.json',lambda x:x['environment_inputs']['compiler'].update(uopt='0'*64)),('trace_fidelity.json',lambda x:x.update(stock_object_sha256='0'*64)),('trace_fidelity.json',lambda x:x.update(complete_objects_identical=False))]
    for name,change in changes:
        file=packet/name;original=file.read_bytes();data=json.loads(original);change(data);file.write_text(json.dumps(data))
        assert run(f"\ntry:v.verify(Path({str(tmp_path/'refused')!r}))\nexcept ValueError:print('rejected')\nelse:raise RuntimeError('accepted altered receipt')\n",packet)=='rejected\n'
        file.write_bytes(original)
    # Change actual inputs, rather than only editing their saved hashes.
    for file in [packet/'group/group.c',root/'tools/cloud/score.py',root/'asm/us/blob/symbols.json']:
        original=file.read_bytes();file.write_bytes(original+b'\n')
        assert run(f"\ntry:v.verify(Path({str(tmp_path/'changed_input')!r}))\nexcept ValueError:print('rejected')\nelse:raise RuntimeError('accepted changed input')\n",packet)=='rejected\n'
        file.write_bytes(original)

def test_environment_missing_ido_diagnostic(tmp_path):
    script=f'''v.score.IDO=Path({str(tmp_path/'missing_ido')!r})
try:v.environment_inputs()
except SystemExit as e:
 if 'run tools/cloud/setup.sh' not in str(e):raise
else:raise RuntimeError('missing compiler was accepted')
print('standard missing-IDO diagnostic passes')
'''
    assert run(script)=='standard missing-IDO diagnostic passes\n'
