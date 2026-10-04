"""Actual C89 source checks with explicit synthetic status-helper contracts."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

WORK = Path(__file__).resolve().parent
ROOT = WORK.parents[3]
CALLEES = {'800150C8': '80016998', '80015120': '80015F28',
           '80015178': '800158D8', '800151D0': '80015C0C', '800152A8': '800163A8'}
HARNESS = r'''
static u16 actual[76], expected[76], expected_log[72];
static int original_length, mode_now, calls, expected_calls;
static int reverse_walk = REVERSE;
static unsigned int cases;

static void mutate(u16 *data, int count) {
    int slot;
    if (count != 0 || original_length == 0) return;
    slot = reverse_walk ? original_length - 2 : 1;
    if (mode_now == 1 && original_length > 1) data[4 + slot] = 0x4321;
    if (mode_now == 2 && original_length > 1) data[4 + slot] = 0xFFFF;
    if (mode_now == 3 && original_length < 64) {
        data[4 + original_length] = 0x1234;
        data[5 + original_length] = 0xFFFF;
    }
}

int CALLEE(u16 id) {
    assert(calls < expected_calls && id == expected_log[calls]);
    mutate(actual, calls);
    ++calls;
    return calls & 1 ? 1 : 0;
}

static void check_case(int length, unsigned int seed, int mode, int one_value) {
    int i, position;
    original_length = length;
    mode_now = mode;
    for (i = 0; i < 76; i++) actual[i] = (u16)(0xA000 + i);
    for (i = 0; i < length; i++) {
        actual[4 + i] = one_value ? (u16)seed : (u16)((seed * 997U + i * 313U) % 65535U);
    }
    actual[4 + length] = 0xFFFF;
    memcpy(expected, actual, sizeof(actual));
    expected_calls = 0;
    if (reverse_walk) {
        /* Scan length is fixed before any helper callback. */
        for (position = length - 1; position >= 0; position--) {
            expected_log[expected_calls] = expected[4 + position];
            mutate(expected, expected_calls);
            ++expected_calls;
        }
    } else {
        position = 0;
        while (expected[4 + position] != 0xFFFF) {
            assert(expected_calls < 72);
            expected_log[expected_calls] = expected[4 + position];
            mutate(expected, expected_calls);
            ++expected_calls;
            ++position;
        }
    }
    calls = 0;
    FUNCTION(actual + 4);
    assert(calls == expected_calls);
    assert(memcmp(actual, expected, sizeof(actual)) == 0);
    ++cases;
}

int main(void) {
    unsigned int value;
    int length, seed, mode;
    typedef char u16_width[sizeof(u16) == 2 ? 1 : -1];
    (void)sizeof(u16_width);
    for (length = 0; length <= 64; length++) {
        for (seed = 0; seed < 7; seed++) {
            for (mode = 0; mode < 4; mode++) check_case(length, seed, mode, 0);
        }
    }
    for (value = 0; value < 65535; value++) check_case(1, value, 0, 1);
    assert(cases == 67355U);
    return 0;
}
'''


class SemanticsTests(unittest.TestCase):
    def check_function(self, address):
        source = ROOT / 'cloud/matches/boot_tail' / ('func_' + address + '.c')
        harness = HARNESS.replace('REVERSE', '1' if address == '800152A8' else '0')
        harness = harness.replace('CALLEE', 'func_' + CALLEES[address]).replace('FUNCTION', 'func_' + address)
        cc = shutil.which('cc')
        self.assertIsNotNone(cc)
        with tempfile.TemporaryDirectory(prefix='low-dispatch-host-') as tmp:
            p = Path(tmp)
            (p / 'test.c').write_text('#include <assert.h>\n#include <string.h>\n#include "' + str(source) + '"\n' + harness)
            subprocess.run([cc, '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror',
                '-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-no-pie',
                str(p / 'test.c'), '-o', str(p / 'test')], check=True, capture_output=True, text=True)
            # Ptrace prevents LeakSanitizer here; no candidate/harness heap allocation.
            # Address and undefined-behavior checks still halt on their first error.
            env = dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',
                       UBSAN_OPTIONS='halt_on_error=1:print_stacktrace=1')
            result = subprocess.run([str(p / 'test')], capture_output=True, text=True, env=env)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_forward_16998(self): self.check_function('800150C8')
    def test_forward_15F28(self): self.check_function('80015120')
    def test_forward_158D8(self): self.check_function('80015178')
    def test_forward_15C0C(self): self.check_function('800151D0')
    def test_reverse_163A8(self): self.check_function('800152A8')

    def test_exact_source_receipt_hashes(self):
        receipt = json.loads((WORK / 'verification.json').read_text())
        rows = [r for r in receipt['results'] if '-O2 ' in r['flags']]
        self.assertEqual(len(rows), 5)
        self.assertEqual(sum(r['native_bytes'] for r in rows), 464)
        for row in rows:
            self.assertTrue(row['strict_match'] and row['relocated_full_word_equality'])
            self.assertFalse(row['unresolved'] or row['unverified'] or row['errors'] or row['masked_relocations'])
            self.assertEqual((row['differing_words'], row['extra_words']), (0, 0))
            self.assertEqual(hashlib.sha256((ROOT / row['source_path']).read_bytes()).hexdigest(), row['source_sha256'])


if __name__ == '__main__':
    unittest.main()
