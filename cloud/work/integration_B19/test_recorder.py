import copy,hashlib,importlib.util,json,tempfile,unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('recorder',HERE/'record_owner_locks.py');recorder=importlib.util.module_from_spec(spec);spec.loader.exec_module(recorder)
class RecorderTests(unittest.TestCase):
 def setUp(self):
  self.row=json.loads((ROOT/'cloud/work/integration_B18/storage_block.json').read_text());self.proof=json.loads((HERE/'fresh_verification.json').read_text())
 def run_proof(self,proof):
  with tempfile.TemporaryDirectory() as t:
   p=Path(t)/'proof.json';p.write_text(json.dumps(proof));return recorder.validated_entries(ROOT,self.row,p,hashlib.sha256(p.read_bytes()).hexdigest(),Path.home()/'.conveyor')
 def test_exact_reviewed_proof_records_two_honest_locks(self):
  entries=self.run_proof(self.proof);self.assertEqual(len(entries),2);self.assertEqual(sorted(e['raw_word_diff'] for e in entries.values()),[5,9]);self.assertTrue(all(e['verified']=='score0' for e in entries.values()))
 def test_nonzero_strict_refused(self):
  self.proof['results'][0]['strict_score']=1
  with self.assertRaises(ValueError):self.run_proof(self.proof)
 def test_empty_extent_refused(self):
  self.proof['results'][0]['words']=0
  with self.assertRaises(ValueError):self.run_proof(self.proof)
 def test_changed_target_refused(self):
  self.proof['results'][0]['target_sha256']='0'*64
  with self.assertRaises(ValueError):self.run_proof(self.proof)
 def test_changed_header_refused(self):
  self.proof['headers']['include/PR/os_thread.h']='0'*64
  with self.assertRaises(ValueError):self.run_proof(self.proof)
 def test_changed_body_refused(self):
  self.proof['results'][0]['body_sha256']='0'*64
  with self.assertRaises(ValueError):self.run_proof(self.proof)
 def test_unreviewed_proof_digest_refused(self):
  with self.assertRaises(ValueError):recorder.validated_entries(ROOT,self.row,HERE/'fresh_verification.json','0'*64,Path.home()/'.conveyor')
if __name__=='__main__':unittest.main()
