"""Complete25-member PlayGame replay and optimized input/claim refusals."""
import importlib.util,json,os,shutil,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];PACKET=ROOT/'cloud/work/frontier/dot_mat3_race_tail_20261017';REPO=Path(os.environ.get('N64_RESEARCH_REPO',str(ROOT))).resolve()
def test_mat3_race_tail():
 spec=importlib.util.spec_from_file_location('mat3_race_tail_verify',PACKET/'verify.py');module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module);r=module.verify(REPO)
 assert len(r['results']['candidate'])==25 and set(r['results']['candidate'].values())=={'MATCH'}

def test_race_tail_optimized_refusals(tmp_path):
 receipt=json.loads((PACKET/'receipt.json').read_text());modes=['protocol','claims','bytes','new_claims','script','unverified']+['source:'+x for x in receipt['source_sha256']]+['target:'+x for x in receipt['target_words_sha256']]
 runner=tmp_path/'run.py';runner.write_text('''import dataclasses,importlib.util,sys
from pathlib import Path
p=Path(sys.argv[1]);repo=Path(sys.argv[2]);mode=sys.argv[3];sys.path.insert(0,str(repo/'tools/cloud'));import score
s=importlib.util.spec_from_file_location('proof',p/'verify.py');v=importlib.util.module_from_spec(s);s.loader.exec_module(v)
if mode.startswith('target:'):
 name=mode.split(':',1)[1];rows=dict(score.targets());rows[name]=list(rows[name]);rows[name][0]^=1;score.targets=lambda:rows
if mode=='unverified':
 original=score.compare
 def changed(*a,**kw):
  r=original(*a,**kw);return dataclasses.replace(r,unverified=r.unverified+['deliberate uncertainty'])
 score.compare=changed
try:v.verify(repo,compile=mode=='unverified')
except RuntimeError:print('rejected '+mode)
else:raise SystemExit('accepted '+mode)
''')
 for i,mode in enumerate(modes):
  p=tmp_path/str(i);shutil.copytree(PACKET,p,ignore=shutil.ignore_patterns('__pycache__'))
  if mode in ('protocol','claims','bytes','new_claims'):
   f=p/'receipt.json';r=json.loads(f.read_text());r[{'protocol':'protocol_version','claims':'claims','bytes':'native_bytes','new_claims':'new_claims'}[mode]]={'protocol':1,'claims':[],'bytes':928,'new_claims':['func_800F92C8']}[mode];f.write_text(json.dumps(r))
  elif mode=='script' or mode.startswith('source:'):
   f=p/('verify.py' if mode=='script' else mode.split(':',1)[1]);f.write_text(f.read_text()+ ('\n# changed\n' if f.suffix=='.py' else '\n/* changed */\n'))
  r=subprocess.run([sys.executable,'-O',str(runner),str(p),str(REPO),mode],capture_output=True,text=True)
  if r.returncode or r.stdout.strip()!='rejected '+mode:raise RuntimeError(mode+': '+r.stdout+r.stderr)
