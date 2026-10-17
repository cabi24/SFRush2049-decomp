"""Fast guard tests; compiling/mutation proof is in the packet's verify.py."""
import importlib.util
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/mat3_ui_audio_cleanup_20261017'
SPEC = importlib.util.spec_from_file_location('audio_cleanup_proof', PACKET / 'verify.py')
PROOF = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(PROOF)


class ResearchProofGuards(unittest.TestCase):
    def test_require_is_fail_closed(self):
        with self.assertRaisesRegex(RuntimeError, 'intentional'):
            PROOF.require(False, 'intentional')

    def test_modified_source_is_rejected(self):
        with tempfile.TemporaryDirectory(prefix='audio-cleanup-guard-') as directory:
            copied = Path(directory) / 'packet'
            shutil.copytree(PACKET, copied)
            source = copied / 'group/cleanup.c'
            source.write_text(source.read_text() + '\n/* mutation */\n')
            with self.assertRaisesRegex(RuntimeError, 'Source fingerprint changed'):
                PROOF.verify_inputs(copied)

    def test_guards_survive_python_optimization(self):
        code = "import runpy; p=runpy.run_path(%r); p['require'](False, 'intentional-optimized-guard')" % str(PACKET / 'verify.py')
        result = subprocess.run([sys.executable, '-O', '-c', code], capture_output=True, text=True)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('intentional-optimized-guard', result.stderr)


if __name__ == '__main__':
    unittest.main()
