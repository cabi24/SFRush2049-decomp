"""Actual-source sanitizer coverage plus bounded canonical MIPS-II replay."""
import importlib.util
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(WORK))
from tools.cloud import score
from native_replay import execute, COMMANDS, VALUES
FINAL = ROOT / 'cloud/matches/boot_tail/func_80020820.c'


class Semantics(unittest.TestCase):
    def test_actual_source_full_clear_call_order_and_untouched_rows(self):
        old = ROOT / 'cloud/work/boot_tail/C13-controller-pair/test_semantics.py'
        spec = importlib.util.spec_from_file_location('reviewed_reset_fixture', old)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        case = module.Semantics()
        def run_c(address, body):
            self.assertEqual(address, '80020820')
            with tempfile.TemporaryDirectory(prefix='byte-clear-host-') as tmp:
                p = Path(tmp)
                (p / 'test.c').write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n#include "' + str(FINAL) + '"\n' + body)
                subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-fsanitize=address,undefined', '-no-pie', str(p / 'test.c'), '-o', str(p / 'test')], check=True, capture_output=True)
                subprocess.run([str(p / 'test')], check=True, capture_output=True, env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1'))
        case.run_c = run_c
        case.test_clear_whole_row_then_ordered_controller_defaults()

    def test_native_and_final_relocated_words_full_registered_domain(self):
        score.ASM_DIR = ROOT / 'asm/us/boot_tail'
        words = score.targets()['func_80020820']
        with tempfile.TemporaryDirectory(prefix='byte-clear-native-') as tmp:
            obj = Path(tmp) / 'final.o'
            score.compile_single(FINAL, score.DEFAULT_FLAGS, obj)
            relocated, masks, unresolved, unverified, errors = score.relocate(obj, score.text_words(obj), 0, 484, score.image_symbols())
            self.assertFalse(masks or unresolved or unverified or errors)
            self.assertEqual(relocated[:121], words)
            count = 0
            for group in range(9):
                set_number = 255 if group == 8 else group
                for channel in range(32 if group == 8 else 16):
                    for pattern in (0xA5, 0xFF):
                        expected_regular = bytearray([pattern] * (8 * 16 * 134))
                        expected_external = bytearray([pattern] * (32 * 134))
                        data = expected_external if group == 8 else expected_regular
                        offset = channel * 134 if group == 8 else (group * 16 + channel) * 134
                        data[offset:offset + 134] = bytes(134)
                        data[offset] = 0xC5
                        for controller, value in zip(COMMANDS, VALUES):
                            data[offset + controller] = value
                        for upper in (0, 0xA5B60000):
                            native = execute(words, channel, set_number, pattern, upper)
                            compiled = execute(relocated[:121], channel, set_number, pattern, upper)
                            self.assertEqual(native, compiled)
                            self.assertEqual(native[:2], (bytes(expected_regular), bytes(expected_external)))
                            count += 1
            self.assertEqual(count, 640)

    def test_native_replay_rejects_unsupported_opcode(self):
        with self.assertRaisesRegex(ValueError, 'unsupported opcode'):
            execute([0xFC000000], 0, 0, 0xA5)


if __name__ == '__main__':
    unittest.main()
