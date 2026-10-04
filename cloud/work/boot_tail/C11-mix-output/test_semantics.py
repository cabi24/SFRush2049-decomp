"""Synthetic-data semantics: native instructions, high-level model and host C89."""
import hashlib
import itertools
import json
import os
from pathlib import Path
import random
import shutil
import subprocess
import tempfile
import unittest
from native_model import Mixer, reference, f32, score
WORK = Path(__file__).resolve().parent
ROOT = WORK.parents[3]
SOURCE = WORK / 'func_8001E0E0_NONMATCH.c'


def profiles():
    return [
        ([f32((i + 1) / 256.) for i in range(129)], [0., .25, .75, 1.], [16384., .25, .75, -.125, .5]),
        ([f32((i - 64) / 128.) for i in range(129)], [1., .5, -.125, 0.], [-2048., -.25, .5, -.75, .125]),
        ([f32(((i * 37) % 193 - 96) / 64.) for i in range(129)], [-.25, 1.25, .5, -.5], [65536., .125, -.5, .25, .75])]


def cases():
    levels = [0, 1, 0xFFFF, 0x10000, 0x10001, 0x3FFFFF, 0x7EFFFF, 0x7F0000, 0x7F0001, 0x800000, 0xFFFFFFFF]
    positions = [0, 1, 0x3FFFFF, 0x400000, 0x400001, 0x7EFFFF, 0x7F0000, 0x7FFFFF, 0x800000]
    result = [(v, p, s, a, (0, 1, 2, 3)) for v, p, s, a in itertools.product(levels, positions, positions, levels)]
    for aliases in itertools.product(range(4), repeat=4):
        for values in [(0, 0, 0, 0), (0xFFFFFFFF, 0x800000, 0x800000, 0xFFFFFFFF),
                       (0x345678, 0x400000, 0x400001, 0x123456), (0x10001, 1, 0x7FFFFF, 0x7F0001)]:
            result.append((*values, aliases))
    rng = random.Random(0x1E0E0)
    for _ in range(512):
        result.append((rng.randrange(0x100000000), rng.randrange(0x800001), rng.randrange(0x800001),
                       rng.randrange(0x100000000), tuple(rng.randrange(4) for _ in range(4))))
    assert len(result) == 11337
    return result


def literal(value):
    text = format(value, '.9g')
    if '.' not in text and 'e' not in text:
        text += '.0'
    return text + 'f'


class Semantics(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.profiles, cls.cases = profiles(), cases()
        cls.expected = []
        cls.native = Mixer()
        for table, balance, scalars in cls.profiles:
            outcomes = []
            for volume, pan, span, aux, aliases in cls.cases:
                native, writes, reads = cls.native.run(volume, pan, span, aux, table, balance, scalars, aliases)
                model, model_writes = reference(volume, pan, span, aux, table, balance, scalars, aliases)
                if (native, writes) != (model, model_writes):
                    raise AssertionError(('native/model disagreement', volume, pan, span, aux, aliases, native, model))
                allowed = set(range(0x8002CA40, 0x8002CC44, 4)) | set(range(0x8002CC44, 0x8002CC54, 4)) | set(range(0x8002D910, 0x8002D924, 4))
                if not reads <= allowed:
                    raise AssertionError('external data read outside documented objects')
                outcomes.append(native)
            cls.expected.append(outcomes)

    def test_native_model_all_bounded_fixtures(self):
        self.assertEqual(sum(map(len, self.expected)), 34011)

    def test_c89_sanitizers_against_native_outputs(self):
        for (table, balance, scalars), expected in zip(self.profiles, self.expected):
            declarations = 'const float D_8002CA40[129]={' + ','.join(map(literal, table)) + '};\n'
            declarations += 'const float D_8002CC44[4]={' + ','.join(map(literal, balance)) + '};\n'
            for address, value in zip(('D910', 'D914', 'D918', 'D91C', 'D920'), scalars):
                declarations += 'const float D_8002' + address + '=' + literal(value) + ';\n'
            rows = []
            for (v, p, s, a, aliases), output in zip(self.cases, expected):
                rows.append('{%s,{%s},{%s}}' % (','.join(str(n) + 'U' for n in (v, p, s, a)), ','.join(map(str, aliases)), ','.join(map(str, output))))
            harness = '#include <assert.h>\n#include <float.h>\n#include "' + str(SOURCE) + '"\n' + declarations
            harness += 'typedef struct Case {u32 volume,pan,span,aux;unsigned char alias[4];u16 expected[4];} Case;\nstatic const Case cases[]={' + ',\n'.join(rows) + '};\n'
            harness += r'''
int main(void){unsigned int i,j;u16 output[4];const Case *c;typedef char widths[sizeof(float)==4&&sizeof(u32)==4&&sizeof(u16)==2?1:-1];(void)sizeof(widths);assert(FLT_RADIX==2&&FLT_MANT_DIG==24);
for(i=0;i<sizeof(cases)/sizeof(cases[0]);i++){c=&cases[i];for(j=0;j<4;j++)output[j]=0xACDC;func_8001E0E0(&output[c->alias[0]],&output[c->alias[1]],c->volume,c->pan,c->span,&output[c->alias[2]],c->aux,&output[c->alias[3]]);for(j=0;j<4;j++)assert(output[j]==c->expected[j]);}return 0;}
'''
            with tempfile.TemporaryDirectory(prefix='mixer-host-') as t:
                p = Path(t);(p / 'test.c').write_text(harness)
                cc = shutil.which('cc');self.assertIsNotNone(cc)
                command = [cc, '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-ffp-contract=off', '-fsanitize=address,undefined,float-cast-overflow', '-no-pie', str(p / 'test.c'), '-o', str(p / 'test')]
                proc = subprocess.run(command, text=True, capture_output=True)
                self.assertEqual(proc.returncode, 0, proc.stdout + proc.stderr)
                env = dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1')
                proc = subprocess.run([str(p / 'test')], text=True, capture_output=True, env=env)
                self.assertEqual(proc.returncode, 0, proc.stdout + proc.stderr)

    def test_relocated_ido_objects_against_native(self):
        paths = [SOURCE] + sorted((WORK / 'controls').glob('*.c'))
        # Every retained/control O2 object, all alias partitions, selected edge
        # grid and random cases. Whole objects and native use the same decoder;
        # the independent high-level model and host C check its interpretation.
        selected = sorted(set(range(0, 9801, 101)) | set(range(9801, 11337)))
        self.assertEqual(len(selected), 1634)
        for source in paths:
            with tempfile.TemporaryDirectory(prefix='mixer-native-') as t:
                obj = Path(t) / 'candidate.o';score.compile_single(source, score.DEFAULT_FLAGS, obj)
                words = score.text_words(obj)
                relocated, masks, unresolved, unverified, errors = score.relocate(obj, words, 0, len(words) * 4, score.image_symbols())
                self.assertFalse(masks or unresolved or unverified or errors)
                candidate = Mixer(relocated)
                for profile, expected in zip(self.profiles, self.expected):
                    for index in selected:
                        v, p, s, a, aliases = self.cases[index]
                        result, _, _ = candidate.run(v, p, s, a, *profile, aliases)
                        self.assertEqual(result, expected[index], (source.name, index))

    def test_boundaries_and_fail_closed_model(self):
        table, balance, scalars = self.profiles[0]
        # This is explicitly outside the four-float external-object domain.
        with self.assertRaises(KeyError):
            self.native.run(0, 0, 0xC00000, 0, table, balance, scalars)
        broken = list(self.native.words);broken[0] = 0xFC000000
        with self.assertRaises(AssertionError):
            Mixer(broken).run(0, 0, 0, 0, table, balance, scalars)

    def test_native_widths_and_receipt(self):
        with tempfile.TemporaryDirectory(prefix='mixer-widths-') as t:
            p=Path(t);(p/'probe.c').write_text('typedef char abi[sizeof(unsigned int)==4&&sizeof(unsigned short)==2&&sizeof(float)==4?1:-1];\nvoid width_probe(void) {}\n');score.compile_single(p/'probe.c',score.DEFAULT_FLAGS,p/'probe.o')
        receipt=json.loads((WORK/'verification.json').read_text());self.assertEqual(len(receipt['results']),8)
        for row in receipt['results']:
            self.assertEqual(hashlib.sha256((ROOT/row['source_path']).read_bytes()).hexdigest(),row['source_sha256'])
            self.assertFalse(row['strict_match'])
        retained=[r for r in receipt['results'] if r['purpose']=='retained' and '-O2 ' in r['flags']][0]
        self.assertEqual((retained['differing_words'],retained['total_words'],retained['extra_words']),(207,216,3))
        self.assertFalse(retained['masked_relocations'] or retained['unresolved'] or retained['unverified'] or retained['errors'])

if __name__ == '__main__':
    unittest.main()
