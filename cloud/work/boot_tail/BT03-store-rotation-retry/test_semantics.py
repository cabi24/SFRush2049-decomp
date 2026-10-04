"""Run the reviewed full-source initializer fixture against the exact new source."""
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[4]
OLD = ROOT / 'cloud/work/boot_tail/BT03-high-runtime-lists/test_semantics.py'
SPEC = importlib.util.spec_from_file_location('reviewed_initializer_tests', OLD)
MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)


class InitializerSemantics(unittest.TestCase):
    def test_full_actual_source_callback_order_and_untouched_bytes(self):
        case = MODULE.Semantics()
        original = case.run_c
        def canonical_source(address, body, match=True):
            self.assertEqual(address, '8001C1D8')
            return original(address, body, True)
        case.run_c = canonical_source
        case.test_initializer_exact_untouched_bytes()

    def test_only_natural_loop_body_delta(self):
        import importlib.util
        spec = importlib.util.spec_from_file_location('initializer_verify', Path(__file__).with_name('verify.py'))
        verifier = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(verifier)
        old = (ROOT / verifier.BASE).read_text()
        new = (ROOT / verifier.FINAL).read_text()
        self.assertEqual(new, old.replace(verifier.OLD_LOOP, verifier.NEW_LOOP))


if __name__ == '__main__':
    unittest.main()
