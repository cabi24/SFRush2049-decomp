"""Fast source-bound guards for the audio draft's supporting MBOX cleanup."""
import importlib.util,json,shutil,tempfile,unittest
from pathlib import Path
from unittest.mock import patch
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/mat3_ui_audio_init_20261017/helper_cleanup'
s=importlib.util.spec_from_file_location('mbox_cleanup_proof',PACKET/'verify.py')
v=importlib.util.module_from_spec(s);s.loader.exec_module(v)
class ProofGuards(unittest.TestCase):
    def copy(self):
        temp=tempfile.TemporaryDirectory(prefix='mbox-guards-');self.addCleanup(temp.cleanup)
        out=Path(temp.name)/'packet';shutil.copytree(PACKET,out,ignore=shutil.ignore_patterns('__pycache__'));return out
    def test_false_credit_rejected(self):
        p=self.copy();r=json.loads((p/'inputs.json').read_text());r['claims']=['model_data_load'];(p/'inputs.json').write_text(json.dumps(r))
        with self.assertRaisesRegex(RuntimeError,'False supporting credit'):v.inputs(p)
    def test_changed_source_rejected(self):
        p=self.copy();f=p/'group/helpers.c';f.write_text(f.read_text()+'\n/* modified */\n')
        with self.assertRaisesRegex(RuntimeError,'Changed supporting input'):v.inputs(p)
    def test_changed_proof_rejected(self):
        p=self.copy();f=p/'verify.py';f.write_text(f.read_text()+'\n# modified\n')
        with self.assertRaisesRegex(RuntimeError,'Changed supporting input'):v.inputs(p)
    def test_changed_target_rejected(self):
        targets=dict(v.score.targets());targets['model_data_load']=list(targets['model_data_load']);targets['model_data_load'][0]^=1
        with patch.object(v.score,'targets',return_value=targets):
            with self.assertRaisesRegex(RuntimeError,'Changed target'):v.inputs()
if __name__=='__main__':unittest.main()
