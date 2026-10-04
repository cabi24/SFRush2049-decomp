"""Replay the reviewed live-callback fixture against all three new source forms."""
import importlib.util
import os
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
ORIGINAL = ROOT / 'cloud/work/boot_tail/BT03-high-spatial-control/test_semantics.py'
spec = importlib.util.spec_from_file_location('reviewed_spatial_tests', ORIGINAL)
reviewed = importlib.util.module_from_spec(spec)
spec.loader.exec_module(reviewed)


class CachedThresholdSemantics(unittest.TestCase):
    def check_c(self, name, body):
        sources = [WORK / 'nonmatch/func_8001DC08.c'] + sorted((WORK / 'controls').glob('*.c'))
        for source in sources:
            with self.subTest(source=source.name):
                text = ('#include <assert.h>\n#include <string.h>\n#include <math.h>\n'
                        '#include <stddef.h>\n#include "'+str(source)+'"\n'+body)
                with tempfile.TemporaryDirectory(prefix='dc08-semantics-') as temp:
                    p = Path(temp)
                    (p / 'test.c').write_text(text)
                    cmd = ['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror',
                           '-O1', '-fno-fast-math', '-ffp-contract=off',
                           '-fsanitize=address,undefined,float-cast-overflow', '-no-pie',
                           str(p / 'test.c'), '-lm', '-o', str(p / 'test')]
                    r = subprocess.run(cmd, capture_output=True, text=True)
                    self.assertEqual(r.returncode, 0, r.stdout+r.stderr)
                    r = subprocess.run([str(p / 'test')], capture_output=True, text=True,
                                       env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',
                                                UBSAN_OPTIONS='halt_on_error=1'))
                    self.assertEqual(r.returncode, 0, r.stdout+r.stderr)

    def test_cached_thresholds_counter_wrap_and_live_lists(self):
        reviewed.Semantics.test_cached_thresholds_counter_wrap_and_live_lists(self)


if __name__ == '__main__':
    unittest.main()
