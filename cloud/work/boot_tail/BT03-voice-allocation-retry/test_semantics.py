"""Actual C89 source behavior, native-wrap semantics and mutation sensitivity."""
import importlib.util
import os
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
FINAL = ROOT / 'cloud/matches/boot_tail/func_8001ECE0.c'
OLD = ROOT / 'cloud/work/boot_tail/BT03-high-chains/test_semantics.py'
SPEC = importlib.util.spec_from_file_location('reviewed_chain_tests', OLD)
MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)


def fixture_body():
    captured = []
    case = MODULE.Semantics()
    def capture(address, body, match=True):
        assert address == '8001ECE0' and not match
        captured.append(body)
    case.run_c = capture
    case.test_keyed_pool_insertion()
    assert len(captured) == 1
    return captured[0]


def run_source(source, body):
    with tempfile.TemporaryDirectory(prefix='voice-allocation-test-') as temp:
        work = Path(temp)
        (work / 'candidate.c').write_text(source)
        (work / 'test.c').write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n#include "candidate.c"\n' + body)
        subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-fsanitize=address,undefined', '-no-pie', str(work / 'test.c'), '-o', str(work / 'test')], check=True, capture_output=True)
        return subprocess.run([str(work / 'test')], capture_output=True, timeout=10,
            env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1'))


MUTATIONS = [
    ('key_consumed_before_empty_pool', '    key = func_8001EAEC();', '    if (D_80050C54 == 0) return 0xFFFFFFFFU;\n    key = func_8001EAEC();'),
    ('collision_regeneration', 'if (key == current->key) key = func_8001EAEC();', 'if (key == current->key) key += 0;'),
    ('equal_key_not_order_stop', 'if (key < current->key) break;', 'if (key <= current->key) break;'),
    ('remember_predecessor', '        previous = current;', '        previous = 0;'),
    ('advance_free_head', 'D_80050C54 = D_80050C54->next', 'D_80050C54 = 0'),
    ('clear_free_backlink', 'D_80050C54->previous = 0;', 'D_80050C54->previous = node;'),
    ('install_used_root', 'if (previous == 0) D_80050C50 = node;', 'if (previous == 0) D_80050C50 = previous;'),
    ('link_predecessor', 'else previous->next = node;', 'else previous->next = current;'),
    ('new_previous', 'node->previous = previous;', 'node->previous = 0;'),
    ('new_next', 'node->next = current;', 'node->next = 0;'),
    ('link_successor', 'if (current != 0) current->previous = node;', 'if (current != 0) current->previous = previous;'),
    ('store_key', 'node->key = key;', 'node->key = key + 1;'),
    ('store_voice_identifier', 'node->value = state->identifier60;', 'node->value = key;'),
    ('store_voice_node', 'state->entry18 = node;', 'state->entry18 = current;'),
    ('return_generated_key', '    return key;', '    return key + 1;'),
]

WRAP_BODY = r'''
SequenceNode *D_80050C50, *D_80050C54;
static u32 counter;
static int calls;
u32 func_8001EAEC(void) {
    u32 value;
    ++calls;
    do { value = counter++; } while (value == 0xFFFFFFFFU);
    return value;
}
int main(void) {
    SequenceNode used[2], pool[2];
    VoicePrefix state, before;
    memset(used, 0, sizeof(used));
    memset(pool, 0, sizeof(pool));
    memset(&state, 0xA5, sizeof(state));
    used[0].key = 0;
    used[0].next = &used[1];
    used[1].key = 0xFFFFFFFEU;
    used[1].previous = &used[0];
    pool[0].next = &pool[1];
    pool[1].previous = &pool[0];
    D_80050C50 = used;
    D_80050C54 = pool;
    state.identifier60 = 0x98765403U;
    state.entry18 = 0;
    before = state;
    counter = 0xFFFFFFFEU;
    calls = 0;
    assert(func_8001ECE0(&state) == 0);
    assert(calls == 2 && counter == 1);
    assert(D_80050C50 == used && used[0].next == &used[1]);
    assert(used[1].next == pool && pool[0].previous == &used[1]);
    assert(pool[0].next == 0 && pool[0].key == 0);
    assert(pool[0].value == before.identifier60 && state.entry18 == pool);
    assert(D_80050C54 == &pool[1] && pool[1].previous == 0);
    assert(memcmp(state.unknown00, before.unknown00, sizeof(state.unknown00)) == 0);
    assert(memcmp(state.unknown1C, before.unknown1C, sizeof(state.unknown1C)) == 0);
    assert(state.identifier60 == before.identifier60);
    return 0;
}
'''


class VoiceAllocationSemantics(unittest.TestCase):
    def test_final_actual_source_108_cases_and_counter_skip(self):
        result = run_source(FINAL.read_text(), fixture_body())
        self.assertEqual(result.returncode, 0, result.stderr.decode())

    def test_donor_control_preserves_actual_behavior(self):
        source = WORK / 'controls/func_8001ECE0_free_head_assignment.c'
        result = run_source(source.read_text(), fixture_body())
        self.assertEqual(result.returncode, 0, result.stderr.decode())

    def test_wrap_retains_native_no_retry_and_untouched_voice_bytes(self):
        result = run_source(FINAL.read_text(), WRAP_BODY)
        self.assertEqual(result.returncode, 0, result.stderr.decode())

    def test_fifteen_semantic_mutations_are_detected(self):
        source = FINAL.read_text()
        body = fixture_body()
        for name, old, new in MUTATIONS:
            with self.subTest(name=name):
                self.assertEqual(source.count(old), 1)
                result = run_source(source.replace(old, new), body)
                self.assertNotEqual(result.returncode, 0, name)
                self.assertIn(b'Assertion', result.stderr, name)


if __name__ == '__main__':
    unittest.main()
