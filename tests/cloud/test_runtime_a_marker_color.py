"""Packet-only verification; never substitutes for image/compression/ROM gates."""
import hashlib,importlib.util,json,os,struct,subprocess,sys,unittest,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_runtime_a_marker_color_20261006'
sys.path.insert(0,str(HERE))
import semantics,verify

def load():
 s=verify.scorer(ROOT,ROOT,'ovl_a');b=verify.scorer(ROOT,ROOT,'blob')
 return s,b,s.targets()[verify.NAME],b.targets()['input_new_data_wrapper']

class MarkerColorContract(unittest.TestCase):
 def test_receipt_is_canonical_and_source_bound(self):
  path=HERE/'verification.json';r=json.loads(path.read_text())
  self.assertEqual(path.read_text(),json.dumps(r,indent=2,sort_keys=True)+'\n')
  self.assertEqual(r['source_sha256'],hashlib.sha256(verify.SOURCE.read_bytes()).hexdigest())
  for n,h in r['proof_sha256'].items():self.assertEqual(h,hashlib.sha256((HERE/n).read_bytes()).hexdigest())
  self.assertEqual((r['bytes'],r['comparison']['differing'],r['relocations']),(816,0,33))
 def test_only_provenance_fields_are_nonbinding(self):
  r=json.loads((HERE/'verification.json').read_text())
  for key in ('protected_image_manifest','protected_blob_manifest','tool_sha256','gnu_ld_version','host_gcc_version'):
   changed=dict(r);changed[key]={'unrelated_comments_or_tool_version':'changed'}
   self.assertEqual(verify.comparable(changed),verify.comparable(r))
  for key in ('source_sha256','native_sha256','gnu_linked_body_sha256','anchors','helper_native_bindings','comparison','behavior','compiler_sha256','proof_sha256','layout_checks','complete_function_bytes','text_bytes','alignment_bytes','owned_data_bytes','relocations'):
   changed=dict(r);changed[key]='changed'
   self.assertNotEqual(verify.comparable(changed),verify.comparable(r))
 def test_native_hidden_and_oracle(self):
  s,b,w,h=load();engine=semantics.Native(w,h)
  cases=list(semantics.cases())
  for i in (0,1,4,155,780,1300,2399,2700,3000,len(cases)-1):self.assertEqual(engine.run(cases[i],i),semantics.oracle(cases[i]))
 def test_unknown_instruction_and_extent_rejected(self):
  s,b,w,h=load()
  with self.assertRaises(AssertionError):semantics.Native(w[:-1],h)
  bad=list(w);bad[0]=0xffffffff
  with self.assertRaises(AssertionError):semantics.Native(bad,h).run(next(semantics.cases()))
 def test_scorer_namespaces_are_isolated(self):
  s,b,w,h=load();s._own_data['sentinel']='owned'
  self.assertNotIn('sentinel',b._own_data)
  self.assertNotEqual(s.ASM_DIR,b.ASM_DIR)
  self.assertNotEqual(s.targets(),b.targets())
 def test_submission_registration(self):
  spec=importlib.util.spec_from_file_location('packet_submission',ROOT/'tools/cloud/check_submissions.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
  jobs=list(m.commands(ROOT,['cloud/matches/ovl_a/func_803A3A6C.c']))
  self.assertEqual(len(jobs),1)
  p=subprocess.run(jobs[0],cwd=ROOT,capture_output=True,text=True)
  self.assertEqual(p.returncode,0,p.stdout+p.stderr);self.assertIn('MATCH',p.stdout)
 def test_frozen_replay_from_unrelated_directory(self):
  r=json.loads((HERE/'verification.json').read_text());r['host_gcc_version']='different host GCC provenance';r['gnu_ld_version']='different GNU linker provenance'
  with tempfile.TemporaryDirectory(prefix='marker-version-check-') as d:
   path=Path(d)/'verification.json';path.write_text(json.dumps(r))
   p=subprocess.run([sys.executable,str(HERE/'verify.py'),'--check','--out',str(path)],cwd='/tmp',capture_output=True,text=True)
   self.assertEqual(p.returncode,0,p.stdout+p.stderr)

if __name__=='__main__':unittest.main()
