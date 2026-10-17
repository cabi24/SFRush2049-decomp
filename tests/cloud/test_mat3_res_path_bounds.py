"""Strict bounds replay and optimized-interpreter proof refusal controls."""
import importlib.util,json,os,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];PACKET=ROOT/'cloud/work/frontier/dot_mat3_res_path_bounds_20261017';REPO=Path(os.environ.get('N64_RESEARCH_REPO',str(ROOT))).resolve();SOURCE=ROOT/'cloud/matches/func_800B9740.c'
def test_mat3_res_path_bounds():
 spec=importlib.util.spec_from_file_location('res_path_bounds_verify',PACKET/'verify.py');module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
 result=module.verify(REPO,source=SOURCE)
 assert result['results']['candidate']=='MATCH'

def test_bounds_optimized_refusals(tmp_path):
 runner=tmp_path/'run.py';runner.write_text('''import dataclasses,importlib.util,sys
from pathlib import Path
p=Path(sys.argv[1]);repo=Path(sys.argv[2]);mode=sys.argv[3];sys.path.insert(0,str(repo/'tools/cloud'));import score
s=importlib.util.spec_from_file_location('proof',p/'verify.py');v=importlib.util.module_from_spec(s);s.loader.exec_module(v)
if mode=='target':
 rows=dict(score.targets());rows['func_800B9740']=list(rows['func_800B9740']);rows['func_800B9740'][0]^=1;score.targets=lambda:rows
if mode=='unverified':
 original=score.compare
 def changed(*a,**kw):
  r=original(*a,**kw);return dataclasses.replace(r,unverified=r.unverified+['deliberate uncertainty'])
 score.compare=changed
try:v.verify(repo,compile=mode=='unverified',source=p/'candidate.c')
except RuntimeError:print('rejected '+mode)
else:raise SystemExit('accepted '+mode)
''')
 for mode in ['protocol','claims','bytes','script','source','target','unverified']:
  p=tmp_path/mode;shutil.copytree(PACKET,p,ignore=shutil.ignore_patterns('__pycache__'));shutil.copy2(SOURCE,p/'candidate.c')
  if mode in ('protocol','claims','bytes'):
   f=p/'receipt.json';r=json.loads(f.read_text());r[{'protocol':'protocol_version','claims':'claims','bytes':'native_bytes'}[mode]]={'protocol':1,'claims':[],'bytes':412}[mode];f.write_text(json.dumps(r))
  elif mode in ('script','source'):
   f=p/('verify.py' if mode=='script' else 'candidate.c');f.write_text(f.read_text()+ ('\n# changed\n' if mode=='script' else '\n/* changed */\n'))
  r=subprocess.run([sys.executable,'-O',str(runner),str(p),str(REPO),mode],capture_output=True,text=True)
  if r.returncode or r.stdout.strip()!='rejected '+mode:raise RuntimeError(mode+': '+r.stdout+r.stderr)
