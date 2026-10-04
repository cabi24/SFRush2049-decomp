"""Actual-source C89 sanitizer checks, including mutation sensitivity."""
import importlib.util
import os
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
BASE = ROOT / 'cloud/work/boot_tail/BT03-high-runtime-lists/nonmatch/func_8001EF8C.c'
CONTROL = WORK / 'controls/func_8001EF8C_head_assignment.c'


def run_source(source, body):
    with tempfile.TemporaryDirectory(prefix='voice-priority-semantics-') as temp:
        temp = Path(temp)
        source_path = temp / 'source.c'
        source_path.write_text(source)
        fixture = temp / 'fixture.c'
        fixture.write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n'
                           '#include "source.c"\n' + body)
        subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror',
                        '-fsanitize=address,undefined', '-no-pie', str(fixture), '-o', str(temp/'test')],
                       check=True, capture_output=True)
        return subprocess.run([str(temp/'test')], capture_output=True,
                              env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',
                                       UBSAN_OPTIONS='halt_on_error=1'), timeout=30)


class Semantics(unittest.TestCase):
    def test_historical_2048_cases(self):
        path = ROOT / 'cloud/work/boot_tail/BT03-high-runtime-lists/test_semantics.py'
        spec = importlib.util.spec_from_file_location('frozen_semantics', path)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        captured = []
        fixture = module.Semantics('test_channel_group_insertion_live_unlink')
        fixture.run_c = lambda address, body, match: captured.append((address, body, match))
        fixture.test_channel_group_insertion_live_unlink()
        self.assertEqual(len(captured), 1)
        self.assertEqual(captured[0][0], '8001EF8C')
        for path in (BASE, CONTROL):
            with self.subTest(path=path.name):
                result = run_source(path.read_text(), captured[0][1])
                self.assertEqual(result.returncode, 0, result.stderr.decode())

    def test_all_slots_priorities_and_live_tables(self):
        body = (WORK / 'semantics.inc.c').read_text()
        for path in (BASE, CONTROL):
            with self.subTest(path=path.name):
                result = run_source(path.read_text(), body)
                self.assertEqual(result.returncode, 0, result.stderr.decode())

    def test_mutations_are_rejected(self):
        source = CONTROL.read_text()
        mutations = {
            'missing_next_store': ('(link->next = D_80050548[channel])', 'D_80050548[channel]'),
            'missing_unlink': ('        func_8001EE9C(state);', '        (void)state;'),
            'lost_early_return': ('if (channel == state->channel2E) return;', 'if (channel == state->channel2E) (void)state;'),
            'wrong_active_test': ('if (link->active == 1)', 'if (link->active != 0)'),
            'recaptured_identifier': ('        func_8001EE9C(state);', '        func_8001EE9C(state);\n        index = state->identifier60;'),
            'recaptured_link': ('        func_8001EE9C(state);', '        func_8001EE9C(state);\n        link = &D_800504C8[(u8)state->identifier60];'),
            'stale_bucket': ('    index = state->identifier60;', '    u8 stale;\n    stale = D_80050548[channel];\n    index = state->identifier60;'),
            'wrong_previous': ('link->previous = 255;', 'link->previous = 0;'),
            'missing_active': ('link->active = 1;', 'link->active = 0;'),
            'missing_backlink': ('D_800504C8[D_80050548[channel]].previous = index;', 'D_800504C8[D_80050548[channel]].previous = 255;'),
            'missing_bucket_head': ('D_80050548[channel] = index;', 'D_80050548[channel] = 255;'),
            'missing_state_priority': ('state->channel2E = channel;', 'state->channel2E = 0;'),
            'wrong_group_order': ('channel >= current', 'channel <= current'),
            'missing_next_backlink': ('if (current != 65535) D_80050648[current].previous = channel;', 'if (current != 65535) D_80050648[current].previous = 65535;'),
        }
        body = (WORK / 'semantics.inc.c').read_text()
        for name, (old, new) in mutations.items():
            with self.subTest(mutation=name):
                self.assertEqual(source.count(old), 1)
                mutated = source.replace(old, new)
                if name == 'stale_bucket':
                    mutated = mutated.replace('(link->next = D_80050548[channel])', '(link->next = stale)')
                result = run_source(mutated, body)
                self.assertNotEqual(result.returncode, 0, name + ' escaped fixture')
                self.assertTrue(b'Assertion' in result.stderr or b'assertion' in result.stderr or
                                b'Sanitizer' in result.stderr or b'runtime error' in result.stderr,
                                result.stderr.decode())


if __name__ == '__main__':
    unittest.main()
