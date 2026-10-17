"""GhostEnd research-only replay and optimized integrity refusals."""
import importlib.util,json,os,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];PACKET=ROOT/'cloud/work/frontier/dot_mat3_ghostend_20261017'
REPO=Path(os.environ.get('N64_RESEARCH_REPO',str(ROOT))).resolve()
def test_ghostend_research():
 s=importlib.util.spec_from_file_location('ghostend_verify',PACKET/'verify.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m);r=m.verify(REPO)
 assert r['claims']==[] and r['new_accepted_bytes']==0 and len(r['preserved_locked_members'])==33
 assert r['results']['natural']['behavior']['instruction_coverage']==[108,110]

def test_ghostend_optimized_refusals(tmp_path):
 runner=tmp_path/'run.py';runner.write_text('''import dataclasses,importlib.util,sys
from pathlib import Path
p=Path(sys.argv[1]);repo=Path(sys.argv[2]);mode=sys.argv[3]
sys.path.insert(0,str(repo/'tools/cloud'));import score
s=importlib.util.spec_from_file_location('proof',p/'verify.py');v=importlib.util.module_from_spec(s);s.loader.exec_module(v)
if mode=='target':
 rows=dict(score.targets());rows['ghost_race_setup']=list(rows['ghost_race_setup']);rows['ghost_race_setup'][0]^=1;score.targets=lambda:rows
if mode=='unverified':
 original=score.compare
 def changed(*a,**kw):
  r=original(*a,**kw);return dataclasses.replace(r,unverified=r.unverified+['deliberate uncertainty'])
 score.compare=changed
try:v.verify(repo,compile=mode in ('unverified','trace_object'))
except RuntimeError:print('rejected '+mode)
else:raise SystemExit('accepted '+mode)
''')
 for mode in ['claims','source','script','model','target','unverified','trace_input','trace_object']:
  p=tmp_path/mode;shutil.copytree(PACKET,p)
  if mode=='claims':
   f=p/'receipt.json';r=json.loads(f.read_text());r['claims']=['ghost_race_setup'];f.write_text(json.dumps(r))
  elif mode in ('source','script','model'):
   f=p/{'source':'group/ghost.c','script':'verify.py','model':'behavior.py'}[mode];f.write_text(f.read_text()+ ('\n/* changed */\n' if mode=='source' else '\n# changed\n'))
  elif mode.startswith('trace_'):
   f=p/'trace_fidelity.json';r=json.loads(f.read_text());r['inputs_sha256' if mode=='trace_input' else 'object_sha256']='changed';f.write_text(json.dumps(r))
  run=subprocess.run([sys.executable,'-O',str(runner),str(p),str(REPO),mode],capture_output=True,text=True)
  if run.returncode or run.stdout.strip()!='rejected '+mode:raise RuntimeError(mode+': '+run.stdout+run.stderr)
