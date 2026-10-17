"""Mine research replay and optimized-Python integrity refusals."""
import importlib.util,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/mat3_mine_callback_research_20261017'
def test_mine_callback_research(tmp_path):
 s=importlib.util.spec_from_file_location('mine_research',PACKET/'verify.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m);r=m.verify(tmp_path)
 assert r['claims']==[] and r['new_matching_bytes']==0
 assert r['callback']['symbol_bytes']==2632 and r['behavior']['fixtures']==206

def test_mine_optimized_refusals(tmp_path):
 program=r'''
import dataclasses,importlib.util,json,shutil,sys
from pathlib import Path
packet,scratch=map(Path,sys.argv[1:]);sys.path.insert(0,str(packet.parents[3]))
s=importlib.util.spec_from_file_location('mine_verify',packet/'verify.py');v=importlib.util.module_from_spec(s);s.loader.exec_module(v)
from tools.cloud import score
for mode in ('claims','source','script','model','receipt','target','unverified','trace_input','trace_object'):
 d=scratch/mode;shutil.copytree(packet,d);v.HERE=d
 p=d/'group/group.json'
 if mode=='claims':
  r=json.loads(p.read_text());r['claims']=['false-credit'];p.write_text(json.dumps(r))
 elif mode in ('source','script','model'):
  p=d/{'source':'group/group.c','script':'verify.py','model':'behavior.py'}[mode];p.write_text(p.read_text()+ ('\n/* mutation */\n' if mode=='source' else '\n# mutation\n'))
 elif mode=='receipt':
  p=d/'verification.json';r=json.loads(p.read_text());r['new_matching_bytes']=2636;p.write_text(json.dumps(r))
 elif mode=='target':
  original=score.targets;rows=dict(original());rows['func_8010C974']=list(rows['func_8010C974']);rows['func_8010C974'][0]^=1;score.targets=lambda:rows
 elif mode=='unverified':
  original=score.compare
  def uncertain(*a,**kw):
   r=original(*a,**kw);return dataclasses.replace(r,unverified=r.unverified+['deliberate uncertainty'])
  score.compare=uncertain
 elif mode.startswith('trace_'):
  p=d/'trace_fidelity.json';r=json.loads(p.read_text())
  if mode=='trace_input':r['inputs_sha256']['verify.py']='changed'
  else:r['stock_object_sha256']='changed'
  p.write_text(json.dumps(r))
 try:v.verify(scratch/('out_'+mode))
 except ValueError:pass
 else:raise RuntimeError('accepted '+mode)
 if mode=='target':score.targets=original
 if mode=='unverified':score.compare=original
'''
 r=subprocess.run([sys.executable,'-O','-c',program,str(PACKET),str(tmp_path)],text=True,capture_output=True)
 assert r.returncode==0,r.stdout+r.stderr
