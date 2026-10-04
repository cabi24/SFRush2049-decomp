"""C13 receipt invariants and C89 wrapper behavior; no ROM or image inputs."""
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
NAMES = ['func_%08X' % (0x80021428 + 32 * i) for i in range(9)]


class PacketTests(unittest.TestCase):
    def test_all_nine_native_extents(self):
        rows = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
        selected = [row for row in rows if row['name'] in NAMES]
        self.assertEqual([row['name'] for row in selected], NAMES)
        self.assertEqual(sum(row['size'] for row in selected), 288)
        self.assertTrue(all(row['scope'] == 'in_scope' and row['size'] == 32 for row in selected))

    def test_receipt_hashes_and_strict_invariants(self):
        receipt = json.loads((HERE / 'scores.json').read_text())
        for path, expected in receipt['source_hashes'].items():
            self.assertEqual(hashlib.sha256((ROOT / path).read_bytes()).hexdigest(), expected)
        self.assertEqual(len(receipt['results']), 18)
        for index, name in enumerate(NAMES):
            o2, o1 = receipt['results'][2*index:2*index+2]
            self.assertEqual(o2['name'], name)
            self.assertEqual(o1['name'], name)
            for row in (o2, o1):
                source = ROOT / row['source_path']
                self.assertEqual(hashlib.sha256(source.read_bytes()).hexdigest(), row['source_sha256'])
                self.assertEqual(row['unresolved'] + row['unverified'] + row['errors'], [])
                self.assertEqual(row['masked_relocation_count'], 0)
                self.assertEqual(row['control_offset'], 196 + 18*index)
            self.assertTrue(o2['strict_match'] and o2['relocated_full_word_equality'])
            self.assertEqual((o2['target_bytes'], o2['object_text_bytes']), (32, 32))
            self.assertEqual((o2['differing_words'], o2['extra_words'], o2['zero_padding_words']), (0, 0, 0))
            self.assertEqual(o2['relocations'], [{'offset': 8, 'type': 4, 'symbol': 'func_80021150'}])
            self.assertFalse(o1['strict_match'])
            self.assertEqual((o1['differing_words'], o1['extra_words']), (7, 2))

    def test_c89_host_forwarding(self):
        cc = shutil.which('cc')
        if not cc:
            self.fail('host C compiler required for this packet test')
        declarations = '\n'.join('extern unsigned short %s(void *);' % n for n in NAMES)
        initializers = ',\n'.join(NAMES)
        harness = '''#include <assert.h>
%s
static void *expected_voice;
static void *expected_control;
static unsigned short return_value;
static unsigned int calls;
unsigned short func_80021150(void *voice, void *control)
{
    assert(voice == expected_voice);
    assert(control == expected_control);
    ++calls;
    return return_value;
}
int main(void)
{
    unsigned char storage[400];
    unsigned short values[4] = {0, 1, 32767, 65535};
    unsigned short (*functions[9])(void *) = {%s};
    unsigned int i, j, k;
    for (i = 0; i < 9; ++i) {
        for (j = 0; j < 4; ++j) {
            for (k = 0; k < 4; ++k) {
                expected_voice = storage + j;
                expected_control = storage + j + 196 + 18*i;
                return_value = values[k];
                calls = 0;
                assert(functions[i](expected_voice) == return_value);
                assert(calls == 1);
            }
        }
    }
    return 0;
}
''' % (declarations, initializers)
        with tempfile.TemporaryDirectory(prefix='c13-host-') as tmp:
            cpath = Path(tmp) / 'harness.c'
            exe = Path(tmp) / 'host-test'
            cpath.write_text(harness)
            sources = [str(ROOT / ('cloud/matches/boot_tail/' + n + '.c')) for n in NAMES]
            subprocess.run([cc, '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror',
                            str(cpath), *sources, '-o', str(exe)], check=True, capture_output=True)
            subprocess.run([str(exe)], check=True, capture_output=True)


if __name__ == '__main__':
    unittest.main()
