"""Fast fail-closed input guards; full compiler controls live in the packet."""
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/mat3_ui_audio_init_20261017'
SPEC = importlib.util.spec_from_file_location('audio_init_proof', PACKET / 'verify.py')
PROOF = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(PROOF)


class ResearchProofGuards(unittest.TestCase):
    def rejected_copy(self, edit, expected):
        with tempfile.TemporaryDirectory(prefix='audio-init-guard-') as directory:
            copied = Path(directory) / 'packet'
            shutil.copytree(PACKET, copied)
            edit(copied)
            with self.assertRaisesRegex(RuntimeError, expected):
                PROOF.verify_inputs(copied)

    def test_modified_source_is_rejected(self):
        def edit(path):
            source = path / 'group/init.c'
            source.write_text(source.read_text() + '\n/* mutation */\n')
        self.rejected_copy(edit, 'Source fingerprint changed')

    def test_claim_is_rejected(self):
        def edit(path):
            source = path / 'group/group.json'
            value = json.loads(source.read_text())
            value['claims'] = ['drone_pathfind_main']
            source.write_text(json.dumps(value))
        self.rejected_copy(edit, 'Research-only scope changed')

    def test_target_pin_is_rejected(self):
        def edit(path):
            source = path / 'inputs.json'
            value = json.loads(source.read_text())
            value['target_words_sha256']['drone_pathfind_main'] = '0' * 64
            source.write_text(json.dumps(value))
        self.rejected_copy(edit, 'Canonical target fingerprint changed')

    def test_guards_survive_python_optimization(self):
        code = "import runpy; p=runpy.run_path(%r); p['require'](False, 'intentional-optimized-guard')" % str(PACKET / 'verify.py')
        result = subprocess.run([sys.executable, '-O', '-c', code], capture_output=True, text=True)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('intentional-optimized-guard', result.stderr)


if __name__ == '__main__':
    unittest.main()
