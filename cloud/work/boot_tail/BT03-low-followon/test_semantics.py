"""C89 host checks supplement strict native matching; helpers are test contracts."""
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
LAYOUT = '''
typedef char slot_size[sizeof(AudioSlot) == 104 ? 1 : -1];
typedef char slot_active[offsetof(AudioSlot, active) == 0 ? 1 : -1];
typedef char slot_pending[offsetof(AudioSlot, pending) == 1 ? 1 : -1];
typedef char slot_rate[offsetof(AudioSlot, rate) == 4 ? 1 : -1];
typedef char slot_position[offsetof(AudioSlot, position) == 20 ? 1 : -1];
typedef char slot_release[offsetof(AudioSlot, release_count) == 40 ? 1 : -1];
typedef char slot_outputs[offsetof(AudioSlot, value40) == 64 &&
    offsetof(AudioSlot, value42) == 66 && offsetof(AudioSlot, value44) == 68 &&
    offsetof(AudioSlot, value46) == 70 ? 1 : -1];
AudioSlot slots[7], expected[7];
AudioSlot *D_80038294 = slots + 3;
'''


class SemanticsTests(unittest.TestCase):
    def compile_run(self, name, body, matching=False):
        source = ROOT / 'cloud/matches/boot_tail' / (name + '.c') if matching else WORK / (name + '_NONMATCH.c')
        cc = shutil.which('cc')
        self.assertIsNotNone(cc)
        with tempfile.TemporaryDirectory(prefix='low-followon-host-') as t:
            p = Path(t)
            (p / 'test.c').write_text('#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n#include "' + str(source) + '"\n' + body)
            subprocess.run([cc, '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror',
                            '-fsanitize=undefined,address', '-fno-omit-frame-pointer', '-no-pie',
                            str(p / 'test.c'), '-o', str(p / 'test')], check=True, capture_output=True, text=True)
            # LeakSanitizer cannot run under the executor's ptrace; none of these
            # harnesses allocates heap memory. Address/UB checking remains active.
            env = dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',
                       UBSAN_OPTIONS='halt_on_error=1:print_stacktrace=1')
            result = subprocess.run([str(p / 'test')], capture_output=True, text=True, env=env)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_rate_clamp_full_u16_domain(self):
        self.compile_run('func_80014A0C', LAYOUT + '''
int main(void) {
    unsigned int rate;
    int index;
    for (index = -3; index <= 3; index++) {
        for (rate = 0; rate <= 65535; rate++) {
            memset(slots, 0xA5, sizeof(slots));
            memcpy(expected, slots, sizeof(slots));
            expected[index + 3].rate = rate <= 8192 ? rate : 8192;
            func_80014A0C(index, (u16)rate);
            assert(memcmp(slots, expected, sizeof(slots)) == 0);
        }
    }
    return 0;
}
''')

    def test_parameter_forwarding_and_output_aliases(self):
        self.compile_run('func_80014A74', LAYOUT + '''
static int index_now, calls;
static u32 values[4];
void func_8001E0E0(u16 *right, u16 *left, u32 volume, u32 pan, u32 span,
                   u16 *surround, u32 aux, u16 *aux_out) {
    AudioSlot *s;
    s = &slots[index_now + 3];
    assert(right == &s->value42 && left == &s->value40);
    assert(surround == &s->value44 && aux_out == &s->value46);
    assert(volume == values[0] && pan == values[1]);
    assert(span == values[2] && aux == values[3]);
    *right = 0x901; *left = 0x902; *surround = 0x903; *aux_out = 0x904;
    ++calls;
}
int main(void) {
    int j;
    for (index_now = -3; index_now <= 3; ++index_now) {
        for (j = 0; j < 6; ++j) {
            values[0] = 0xFFFFFFFFU - j;
            values[1] = j * 65537U;
            values[2] = 0x80000000U + j;
            values[3] = j == 0 ? 0 : 0x12345678U;
            memset(slots, 0xA5, sizeof(slots));
            memcpy(expected, slots, sizeof(slots));
            expected[index_now + 3].value42 = 0x901;
            expected[index_now + 3].value40 = 0x902;
            expected[index_now + 3].value44 = 0x903;
            expected[index_now + 3].value46 = 0x904;
            calls = 0;
            func_80014A74(index_now, values[0], values[1], values[2], values[3]);
            assert(calls == 1);
            assert(memcmp(slots, expected, sizeof(slots)) == 0);
        }
    }
    return 0;
}
''', matching=True)

    def test_release_dispatch_all_flag_classes(self):
        self.compile_run('func_80014B3C', LAYOUT + '''
static int calls;
static struct AudioState *argument;
void func_80011A3C(struct AudioState *slot) { ++calls; argument = slot; }
int main(void) {
    int index, a, b;
    unsigned char flags[3];
    flags[0] = 0; flags[1] = 1; flags[2] = 255;
    for (index = -3; index <= 3; index++) {
        for (a = 0; a < 3; a++) {
            for (b = 0; b < 3; b++) {
                memset(slots, 0xA5, sizeof(slots));
                slots[index + 3].active = flags[a];
                slots[index + 3].pending = flags[b];
                memcpy(expected, slots, sizeof(slots));
                if (a != 0) {
                    expected[index + 3].release_count = 20;
                    if (b != 0) expected[index + 3].pending = 0;
                }
                calls = 0; argument = 0;
                func_80014B3C(index);
                assert(calls == (a != 0 && b == 0));
                if (calls) assert(argument == (struct AudioState *)&slots[index + 3]);
                assert(memcmp(slots, expected, sizeof(slots)) == 0);
            }
        }
    }
    return 0;
}
''', matching=True)

    def test_four_sample_tail_copy_and_flush_order(self):
        self.compile_run('func_80014C60', '''
static short actual[64], expected[64];
static unsigned int samples_now;
static int calls;
void osWritebackDCache(void *address, int length) {
    assert(address == actual + samples_now && length == 8);
    assert(memcmp(actual, expected, sizeof(actual)) == 0);
    ++calls;
}
int main(void) {
    int i;
    for (samples_now = 0; samples_now <= 60; samples_now++) {
        for (i = 0; i < 64; i++) actual[i] = expected[i] = (short)(i * 997 - 30000);
        for (i = 0; i < 4; i++) expected[samples_now + i] = expected[i];
        calls = 0;
        func_80014C60(actual, samples_now);
        assert(calls == 1 && memcmp(actual, expected, sizeof(actual)) == 0);
    }
    return 0;
}
''')

    def test_relative_resource_walk_and_sentinel(self):
        self.compile_run('func_80014E1C', '''
typedef char id_offset[offsetof(RelativeEntry, id) == 4 ? 1 : -1];
typedef char offset_width[sizeof(((RelativeEntry *)0)->next_offset) == 4 ? 1 : -1];
int main(void) {
    union { unsigned int alignment; unsigned char bytes[128]; } resource;
    RelativeEntry *entry[5];
    unsigned int offsets[5], ids[5], query;
    int i, found;
    offsets[0] = 0; offsets[1] = 8; offsets[2] = 20; offsets[3] = 36; offsets[4] = 56;
    ids[0] = 65535; ids[1] = 7; ids[2] = 7; ids[3] = 1000; ids[4] = 55;
    memset(&resource, 0xA5, sizeof(resource));
    for (i = 0; i < 5; i++) {
        entry[i] = (RelativeEntry *)(resource.bytes + offsets[i]);
        entry[i]->id = (unsigned short)ids[i];
        entry[i]->next_offset = i < 4 ? offsets[i + 1] - offsets[i] : 0xFFFFFFFFU;
    }
    for (query = 0; query <= 65535; query++) {
        found = -1;
        for (i = 0; i < 4; i++) if (ids[i] == query) { found = i; break; }
        assert(func_80014E1C((unsigned short)query, entry[0]) == (found < 0 ? 0 : entry[found]));
    }
    assert(func_80014E1C(55, entry[4]) == 0);
    return 0;
}
''')

    def test_retained_receipt_hashes_and_scope(self):
        receipt = json.loads((WORK / 'verification.json').read_text())
        rows = [r for r in receipt['results'] if r['purpose'] == 'retained' and '-O2 ' in r['flags']]
        self.assertEqual(len(rows), 5)
        self.assertEqual(sum(r['strict_match'] for r in rows), 2)
        self.assertEqual(sum(r['native_bytes'] for r in rows if r['strict_match']), 240)
        for row in rows:
            self.assertEqual(hashlib.sha256((ROOT / row['source_path']).read_bytes()).hexdigest(), row['source_sha256'])
            self.assertFalse(row['unresolved'] or row['unverified'] or row['errors'] or row['masked_relocations'])
            if row['strict_match']:
                self.assertTrue(row['relocated_full_word_equality'])
                self.assertEqual((row['differing_words'], row['extra_words']), (0, 0))


if __name__ == '__main__':
    unittest.main()
