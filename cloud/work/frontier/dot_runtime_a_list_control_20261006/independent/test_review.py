"""Fast independent-proof guards; no compiler or MIPS binutils required."""
import hashlib,json,os,subprocess,sys
from pathlib import Path
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[4]
SCRIPT=HERE/'review.py'
def test_source_and_proof_bindings():
 r=json.loads((HERE/'verification.json').read_text())
 assert r['source_sha256']==hashlib.sha256((ROOT/'cloud/matches/ovl_a/func_803AE940.c').read_bytes()).hexdigest()
 for name,digest in r['support_sha256'].items():
  p=HERE/name
  if not p.is_file():p=HERE.parent/name
  assert hashlib.sha256(p.read_bytes()).hexdigest()==digest
 assert r['function_bytes']==900 and r['zero_alignment_bytes']==12 and r['relocations']==35
 assert r['behavior']['cases']==5840 and len(r['negative_controls'])==5

def test_provenance_drift_does_not_change_semantic_projection():
 code='''import copy,runpy
n=runpy.run_path(%r);project=n['semantic_receipt']
r={'source_sha256':'source','native_sha256':'body','behavior':{'cases':5840},'comparison':{'differing':0}}
a=copy.deepcopy(r);b=copy.deepcopy(r)
for field in n['PROVENANCE_ONLY']:
 a[field]={'old':'provenance'};b[field]={'new':'provenance'}
assert project(a)==project(b)
for field,value in [('source_sha256','changed'),('native_sha256','changed'),('behavior',{'cases':0}),('comparison',{'differing':1})]:
 c=copy.deepcopy(b);c[field]=value;assert project(a)!=project(c)
''' % str(SCRIPT)
 p=subprocess.run([sys.executable,'-c',code],cwd=HERE,capture_output=True,text=True)
 assert p.returncode==0,p.stderr

def test_python_optimization_is_explicitly_rejected():
 p=subprocess.run([sys.executable,'-O',str(SCRIPT),'--check'],capture_output=True,text=True)
 assert p.returncode!=0 and 'Python optimization is unsupported' in p.stderr

def test_missing_native_tools_fail_clearly():
 env=dict(os.environ,PATH='')
 p=subprocess.run([sys.executable,str(SCRIPT),'--check'],env=env,capture_output=True,text=True)
 assert p.returncode!=0 and 'Required verification tool is unavailable' in p.stderr
