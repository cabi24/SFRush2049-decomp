import importlib.util,json,shutil,tempfile,unittest
from pathlib import Path
HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('highscore_verify',HERE/'verify.py');verify=importlib.util.module_from_spec(spec);spec.loader.exec_module(verify)

class ReplayGuards(unittest.TestCase):
 def setUp(self):
  self.temp=tempfile.TemporaryDirectory();self.root=Path(self.temp.name);shutil.copytree(HERE/'group',self.root/'group');shutil.copyfile(HERE/'inputs.json',self.root/'inputs.json');self.original=verify.HERE;verify.HERE=self.root
 def tearDown(self):verify.HERE=self.original;self.temp.cleanup()
 def test_research_claims_fail_closed(self):
  p=self.root/'group/group.json';s=json.loads(p.read_text());s['claims']=['DrawName'];p.write_text(json.dumps(s))
  with self.assertRaisesRegex(RuntimeError,'scope'):verify.validate_inputs()
 def test_source_pin_fails_closed(self):
  p=self.root/'group/highscore.c';p.write_text(p.read_text()+'\n/* changed */\n')
  with self.assertRaisesRegex(RuntimeError,'Source fingerprint'):verify.validate_inputs()
 def test_target_pin_fails_closed(self):
  p=self.root/'inputs.json';s=json.loads(p.read_text());s['target_words_sha256']['highscore_animText']='0'*64;p.write_text(json.dumps(s))
  with self.assertRaisesRegex(RuntimeError,'Target fingerprint'):verify.validate_inputs()
if __name__=='__main__':unittest.main()
