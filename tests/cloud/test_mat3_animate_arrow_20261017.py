"""AnimateArrow strict proof and optimized-Python integrity refusals."""
import importlib.util,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/mat3_animate_arrow_20261017'
def test_animate_arrow_exact(tmp_path):
 s=importlib.util.spec_from_file_location('arrow_proof',PACKET/'verify.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m);r=m.verify(tmp_path)
 assert r['claims']==['entity_render_setup'] and r['new_matching_bytes']==2048
 assert len(r['preserved_locked_claims'])==7 and r['behavior']['fixtures']==7210

def test_arrow_optimized_refusals(tmp_path):
 program=r'''
import dataclasses,importlib.util,json,shutil,sys
from pathlib import Path
packet,scratch=map(Path,sys.argv[1:]);sys.path.insert(0,str(packet.parents[3]))
s=importlib.util.spec_from_file_location('arrow_verify',packet/'verify.py');v=importlib.util.module_from_spec(s);s.loader.exec_module(v)
from tools.cloud import score
expected={'claims':'main claim scope','source':'source binding','script':'source binding','model':'source binding','receipt':'false credit','target':'target binding','unverified':'non-strict result','trace_input':'trace input binding','trace_object':'trace object binding','closure_claim':'source binding','extent':'incomplete extent'}
for mode in expected:
 d=scratch/mode;shutil.copytree(packet,d);v.HERE=d
 p=d/'group/group.json'
 if mode=='claims':
  r=json.loads(p.read_text());r['claims']=[];p.write_text(json.dumps(r))
 elif mode in ('source','script','model'):
  p=d/{'source':'group/group.c','script':'verify.py','model':'behavior.py'}[mode];p.write_text(p.read_text()+ ('\n/* mutation */\n' if mode=='source' else '\n# mutation\n'))
 elif mode=='receipt':
  p=d/'verification.json';r=json.loads(p.read_text());r['new_matching_bytes']=4096;p.write_text(json.dumps(r))
 elif mode=='target':
  original=score.targets;rows=dict(original());rows['entity_render_setup']=list(rows['entity_render_setup']);rows['entity_render_setup'][0]^=1;score.targets=lambda:rows
 elif mode=='unverified':
  original=score.compare
  def uncertain(*a,**kw):
   r=original(*a,**kw);return dataclasses.replace(r,unverified=r.unverified+['deliberate uncertainty'])
  score.compare=uncertain
 elif mode=='extent':
  original=v.details
  def truncated(*a,**kw):
   r=original(*a,**kw);r['symbol_bytes']-=4;return r
  v.details=truncated
 elif mode=='closure_claim':
  p=d/'closure/atan_reduction/group.json';r=json.loads(p.read_text());r['claims']=['func_8008C680'];p.write_text(json.dumps(r))
 elif mode.startswith('trace_'):
  p=d/'trace_fidelity.json';r=json.loads(p.read_text())
  if mode=='trace_input':r['inputs_sha256']['verify.py']='changed'
  else:r['stock_object_sha256']='changed'
  p.write_text(json.dumps(r))
 try:v.verify(scratch/('out_'+mode))
 except ValueError as e:
  if not str(e).startswith(expected[mode]):raise RuntimeError('wrong refusal '+mode+': '+str(e))
 else:raise RuntimeError('accepted '+mode)
 if mode=='target':score.targets=original
 if mode=='unverified':score.compare=original
 if mode=='extent':v.details=original
'''
 r=subprocess.run([sys.executable,'-O','-c',program,str(PACKET),str(tmp_path)],text=True,capture_output=True)
 assert r.returncode==0,r.stdout+r.stderr


def test_arrow_relocated_packet(tmp_path):
 import shutil
 relocated=tmp_path/'different-checkout-name'/'packet'
 shutil.copytree(PACKET,relocated,ignore=shutil.ignore_patterns('__pycache__'))
 s=importlib.util.spec_from_file_location('arrow_relocated',PACKET/'verify.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m);m.HERE=relocated
 r=m.verify(tmp_path/'objects')
 assert r['new_matching_bytes']==2048 and r['main']['comparison']['differing']==0
