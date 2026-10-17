"""Standard-library guards for the elapsed-timer partial research packet."""
import importlib.util,json,shutil,tempfile,unittest
from pathlib import Path
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/cloud/score.py').is_file())
LOCAL=Path(__file__).resolve().parent
PACKET=LOCAL if (LOCAL/'verify.py').is_file() else ROOT/'cloud/work/frontier/dot_elapsed_timer_research_20261017'
spec=importlib.util.spec_from_file_location('elapsed_proof',PACKET/'verify.py');proof=importlib.util.module_from_spec(spec);spec.loader.exec_module(proof)
HAS_COMPILER=all((proof.score.IDO/f).is_file() for f in json.loads((PACKET/'inputs.json').read_text())['compiler_sha256'])
class Guards(unittest.TestCase):
    def copy(self,tmp):
        p=Path(tmp)/'packet';shutil.copytree(PACKET,p,ignore=shutil.ignore_patterns('__pycache__','replay*.json'));return p
    def alter(self,path,edit):
        j=json.loads(path.read_text());edit(j);path.write_text(json.dumps(j))
    @unittest.skipUnless(HAS_COMPILER, "Existing pinned IDO toolchain is required")
    def test_inputs(self):self.assertEqual(len(proof.validate_inputs(PACKET)['members']),26)
    def test_member_omission(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.copy(t);self.alter(p/'group/group.json',lambda j:j['members'].remove('animElapsedTimerText'))
            with self.assertRaises(RuntimeError):proof.validate_inputs(p)
    def test_claim_upgrade(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.copy(t);self.alter(p/'group/group.json',lambda j:j['claims'].append('checkpoint_hit'))
            with self.assertRaises(RuntimeError):proof.validate_inputs(p)
    def test_source_change(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.copy(t);f=p/'group/candidate.c';f.write_text(f.read_text().replace('blt->state = 100;','blt->state = 99;'))
            with self.assertRaises(RuntimeError):proof.validate_inputs(p)
    def test_saved_claim_change(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.copy(t);self.alter(p/'verification.json',lambda j:j['checkpoint_hit'].update(accepted=True))
            with self.assertRaises(RuntimeError):proof.validate_inputs(p)
    @unittest.skipUnless(HAS_COMPILER, "Existing pinned IDO toolchain is required")
    def test_compiler_fingerprint(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.copy(t);self.alter(p/'inputs.json',lambda j:j['compiler_sha256'].update(cc='0'*64))
            with self.assertRaises(RuntimeError):proof.validate_inputs(p)
    def test_proof_change(self):
        with tempfile.TemporaryDirectory() as t:
            p=self.copy(t);f=p/'verify.py';f.write_text(f.read_text()+'\n# changed proof\n')
            with self.assertRaises(RuntimeError):proof.validate_inputs(p)
if __name__=='__main__':unittest.main()
