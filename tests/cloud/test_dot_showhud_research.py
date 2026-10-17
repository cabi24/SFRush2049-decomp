"""Standard-library guard tests for the conditional ShowHUD research packet."""
import importlib.util,json,shutil,tempfile,unittest
from pathlib import Path
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/cloud/score.py').is_file())
DEFAULT=ROOT/'cloud/work/frontier/dot_showhud_research_20261017'
LOCAL=Path(__file__).resolve().parent
PACKET=LOCAL if (LOCAL/'verify.py').is_file() else DEFAULT
spec=importlib.util.spec_from_file_location('showhud_proof',PACKET/'verify.py');proof=importlib.util.module_from_spec(spec);spec.loader.exec_module(proof)
class Guards(unittest.TestCase):
    def copy(self,tmp):
        p=Path(tmp)/'packet';shutil.copytree(PACKET,p,ignore=shutil.ignore_patterns('__pycache__'));return p
    def test_input_pins(self):self.assertEqual(len(proof.validate_inputs(PACKET)['members']),32)
    def test_missing_member_rejected(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.copy(t);f=p/'group/group.json';j=json.loads(f.read_text());j['members'].remove('sound_handles_clear');f.write_text(json.dumps(j))
            with self.assertRaises(RuntimeError):proof.validate_inputs(p)
    def test_claim_upgrade_rejected(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.copy(t);f=p/'group/group.json';j=json.loads(f.read_text());j['claims']=['skid_mark_render'];f.write_text(json.dumps(j))
            with self.assertRaises(RuntimeError):proof.validate_inputs(p)
    def test_changed_clock_source_rejected(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.copy(t);f=p/'group/candidate.c';f.write_text(f.read_text().replace('extern volatile f32 D_8002EB90;','extern f32 D_8002EB90;'))
            with self.assertRaises(RuntimeError):proof.validate_inputs(p)
    def test_changed_proof_rejected(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.copy(t);f=p/'verify.py';f.write_text(f.read_text()+'\n# changed proof\n')
            with self.assertRaises(RuntimeError):proof.validate_inputs(p)
if __name__=='__main__':unittest.main()
