import importlib.util,json,tempfile,unittest
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[2]
spec=importlib.util.spec_from_file_location('model',HERE/'padding_model.py');model=importlib.util.module_from_spec(spec);spec.loader.exec_module(model)
class BoundaryTests(unittest.TestCase):
 def setUp(self):self.record=json.loads((HERE/'boundary_record.json').read_text())
 def test_original_extent_and_zero_tail_are_authoritative(self):
  model.validate(ROOT,self.record)
 def test_nonzero_retail_tail_refused(self):
  with tempfile.TemporaryDirectory() as d:
   repo=Path(d);data=bytearray((ROOT/'baserom.us.z64').read_bytes()[:0x8800]);data[0x87fc]=1;(repo/'baserom.us.z64').write_bytes(data)
   p=repo/self.record['original_asm'];p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/self.record['original_asm']).read_bytes())
   with self.assertRaises(ValueError):model.validate(repo,self.record)
 def test_regenerated_linker_replay_and_duplicate_input_refusal(self):
  text='SECTIONS {\n        '+self.record['input']+'\n}\n';once=model.rewrite(text,self.record);self.assertEqual(once,model.rewrite(once,self.record))
  with self.assertRaises(ValueError):model.rewrite(text+text,self.record)
 def test_no_accepted_or_new_function_body_changed(self):
  import sys;sys.path.insert(0,str(ROOT))
  from tools.conveyor.pipeline.lock import body_sha
  self.assertEqual(body_sha(HERE/'lib_8700.baseline.c','osDpSetNextBuffer'),body_sha(HERE/'lib_8700.candidate.c','osDpSetNextBuffer'))
  self.assertEqual(body_sha(ROOT/'cloud/work/static_C14/osDpWait.c','osDpWait'),body_sha(HERE/'lib_8700.candidate.c','osDpWait'))
if __name__=='__main__':unittest.main()
