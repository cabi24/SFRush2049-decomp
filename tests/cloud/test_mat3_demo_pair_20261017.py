"""Source-bound demo helper research and optimized integrity refusal tests."""
import importlib.util,json,os,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];PACKET=ROOT/'cloud/work/frontier/dot_mat3_demo_pair_20261017';REPO=Path(os.environ.get('N64_RESEARCH_REPO',str(ROOT))).resolve()
def test_demo_pair_research(tmp_path):
 s=importlib.util.spec_from_file_location('demo_verify',PACKET/'verify.py');v=importlib.util.module_from_spec(s);s.loader.exec_module(v);r=v.verify(REPO,tmp_path/'objects')
 assert r['claims']==[] and r['new_accepted_bytes']==0 and len(r['preserved_bodies'])==29
 assert r['results']['natural']['functions']['debug_stats']['comparison']['differing']==0
 assert r['results']['natural']['functions']['debug_collision']['comparison']['differing']==3
 assert r['results']['natural']['behavior']['executions']==1536

def test_demo_pair_optimized_refusals(tmp_path):
 runner=tmp_path/'run.py';runner.write_text('''import dataclasses,importlib.util,sys
from pathlib import Path
p=Path(sys.argv[1]);repo=Path(sys.argv[2]);mode=sys.argv[3];sys.path.insert(0,str(repo/'tools/cloud'));import score
s=importlib.util.spec_from_file_location('proof',p/'verify.py');v=importlib.util.module_from_spec(s);s.loader.exec_module(v)
if mode=='target':
 rows=dict(score.targets());rows['debug_stats']=list(rows['debug_stats']);rows['debug_stats'][0]^=1;score.targets=lambda:rows
if mode=='unverified':
 original=score.compare
 def changed(*a,**kw):
  r=original(*a,**kw);return dataclasses.replace(r,unverified=r.unverified+['deliberate uncertainty'])
 score.compare=changed
try:v.verify(repo,p/'objects',compile=mode in ('unverified','trace_object'))
except RuntimeError:print('rejected '+mode)
else:raise SystemExit('accepted '+mode)
''')
 for mode in ['claims','group_claim','source','script','model','relocator','target','unverified','trace_input','trace_object']:
  p=tmp_path/mode;shutil.copytree(PACKET,p,ignore=shutil.ignore_patterns('__pycache__'))
  if mode in ('claims','group_claim'):
   f=p/('receipt.json' if mode=='claims' else 'group/group.json');r=json.loads(f.read_text());r['claims']=['debug_stats'];f.write_text(json.dumps(r))
  elif mode in ('source','script','model','relocator'):
   f=p/{'source':'group/demo.c','script':'verify.py','model':'behavior.py','relocator':'relocate.py'}[mode];f.write_text(f.read_text()+ ('\n/* changed */\n' if mode=='source' else '\n# changed\n'))
  elif mode.startswith('trace_'):
   f=p/'trace_fidelity.json';r=json.loads(f.read_text());r['inputs_sha256' if mode=='trace_input' else 'object_sha256']='changed';f.write_text(json.dumps(r))
  run=subprocess.run([sys.executable,'-O',str(runner),str(p),str(REPO),mode],capture_output=True,text=True)
  if run.returncode or run.stdout.strip()!='rejected '+mode:raise RuntimeError(mode+': '+run.stdout+run.stderr)


def test_demo_pair_shallow_relocation():
 # A direct /tmp packet has fewer than four ancestors. Explicit --repo/import
 # must be honored before any assumption about a repository-shaped path.
 import tempfile
 with tempfile.TemporaryDirectory(prefix='demo-shallow-',dir='/tmp') as d:
  p=Path(d);shutil.copytree(PACKET,p,dirs_exist_ok=True,ignore=shutil.ignore_patterns('__pycache__'))
  code="import importlib.util,sys;from pathlib import Path;p=Path(sys.argv[1]);s=importlib.util.spec_from_file_location('proof',p/'verify.py');v=importlib.util.module_from_spec(s);s.loader.exec_module(v);v.verify(Path(sys.argv[2]),compile=False);print('PASS')"
  run=subprocess.run([sys.executable,'-O','-c',code,str(p),str(REPO)],capture_output=True,text=True)
  if run.returncode or run.stdout.strip()!='PASS':raise RuntimeError(run.stdout+run.stderr)
  r=json.loads((p/'receipt.json').read_text());r['claims']=['debug_stats'];(p/'receipt.json').write_text(json.dumps(r))
  run=subprocess.run([sys.executable,'-O','-c',code,str(p),str(REPO)],capture_output=True,text=True)
  if run.returncode==0 or 'RuntimeError: false credit' not in run.stderr:raise RuntimeError('shallow refusal failed: '+run.stdout+run.stderr)
