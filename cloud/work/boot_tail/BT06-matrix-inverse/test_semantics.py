"""Binary32 operation-order fixtures and bounded inverse/alias behavior.

Generated float bit fixtures are mathematical test data, not native image bytes.
Special-value observations assume ordinary nontrapping IEEE host FP behavior;
they do not promise portable C89 zero division or arbitrary native FCSR behavior.
"""
import math
import os
from pathlib import Path
import random
import struct
import subprocess
import tempfile
import unittest
ROOT = Path(__file__).resolve().parents[4]
SOURCE = ROOT / 'cloud/matches/boot_tail/func_80024D74.c'


def f32(value):
    try:
        return struct.unpack('<f', struct.pack('<f', value))[0]
    except OverflowError:
        return math.copysign(float('inf'), value)


def bits(value):
    return struct.unpack('<I', struct.pack('<f', f32(value)))[0]


def from_bits(value):
    return struct.unpack('<f', struct.pack('<I', value))[0]


def add(a, b):
    return f32(a + b)


def sub(a, b):
    return f32(a - b)


def mul(a, b):
    return f32(a * b)


def reciprocal(value):
    if value == 0.0:
        return math.copysign(float('inf'), value)
    return f32(1.0 / value)


def native_order(values, alias):
    """Algebraic operation graph, with every float operation rounded to binary32."""
    src = [f32(x) for x in values]
    dst = src if alias else [0.0] * 12
    a = sub(mul(src[4], src[8]), mul(src[5], src[7]))
    b = -sub(mul(src[3], src[8]), mul(src[5], src[6]))
    c = sub(mul(src[3], src[7]), mul(src[4], src[6]))
    determinant = add(mul(src[2], c), add(mul(src[0], a), mul(src[1], b)))
    f = reciprocal(determinant)
    dst[0] = mul(f, a)
    dst[3] = mul(f, b)
    dst[6] = mul(f, c)
    dst[1] = mul(sub(mul(src[1], src[8]), mul(src[2], src[7])), -f)
    dst[4] = mul(sub(mul(src[0], src[8]), mul(src[2], src[6])), f)
    dst[7] = mul(sub(mul(src[0], src[7]), mul(src[1], src[6])), -f)
    dst[2] = mul(sub(mul(src[1], src[5]), mul(src[2], src[4])), f)
    dst[5] = mul(sub(mul(src[0], src[5]), mul(src[2], src[3])), -f)
    dst[8] = mul(sub(mul(src[0], src[4]), mul(src[1], src[3])), f)
    for row in range(3):
        dst[9 + row] = sub(sub(mul(-src[9], dst[row * 3]),
                                mul(src[10], dst[row * 3 + 1])),
                            mul(dst[row * 3 + 2], src[11]))
    return [bits(x) for x in dst], determinant == 0.0


def determinant_double(m):
    return m[0] * (m[4] * m[8] - m[5] * m[7]) - m[1] * (m[3] * m[8] - m[5] * m[6]) + m[2] * (m[3] * m[7] - m[4] * m[6])


def fixtures():
    identity = [1., 0., 0., 0., 1., 0., 0., 0., 1., 0., 0., 0.]
    cases = [('identity', identity, True)]
    for exponent in range(-10, 11):
        for sign in (-1, 1):
            m = list(identity)
            m[0], m[4], m[8] = sign * 2.0 ** exponent, 2.0 ** -exponent, -2.0
            m[9:] = [3., -5., 7.]
            cases.append(('diagonal', m, True))
    generator = random.Random(0x24D74)
    while sum(name == 'integer' for name, _, _ in cases) < 512:
        m = [float(generator.randrange(-4, 5)) for _ in range(9)] + [float(generator.randrange(-16, 17)) for _ in range(3)]
        if abs(determinant_double(m)) >= 2:
            cases.append(('integer', m, True))
    for exponent in range(1, 25):
        e = 2.0 ** -exponent
        cases.append(('near_singular', [1., 1., 1., 1., f32(1 + e), 1., 1., 1., f32(1 + e), 3., -5., 7.], False))
    for sign in (0.0, -0.0):
        cases.append(('zero_matrix', [sign] * 12, False))
        for diagonal in range(3):
            m = list(identity)
            m[diagonal * 4] = sign
            m[9:] = [sign, -sign, sign]
            cases.append(('singular_diagonal', m, False))
    for special in (from_bits(0x7F800000), from_bits(0xFF800000), from_bits(0x7FC00000), from_bits(1), from_bits(0x80000001), from_bits(0x7F7FFFFF)):
        for index in (0, 4, 8, 9, 10, 11):
            m = list(identity)
            m[index] = special
            cases.append(('ieee_special', m, False))
    return cases


class Semantics(unittest.TestCase):
    def test_binary32_order_alias_and_special_values(self):
        cases = fixtures()
        rows = []
        for _, values, inverse in cases:
            ordinary, divide_zero = native_order(values, False)
            alias, alias_divide_zero = native_order(values, True)
            self.assertEqual(divide_zero, alias_divide_zero)
            arrays = [[bits(x) for x in values], ordinary, alias]
            rows.append('{' + ','.join('{' + ','.join('0x%08xU' % x for x in a) + '}' for a in arrays) + ',%d,%d}' % (inverse, divide_zero))
        harness = r'''
typedef unsigned int u32;
typedef struct Fixture {u32 input[12],expected[12],alias[12];int inverse,divide_zero;} Fixture;
static const Fixture tests[] = { FIXTURES };
typedef struct Guarded {u32 before;Matrix value;u32 after;} Guarded;
static void (*volatile call_body)(Matrix *, const Matrix *)=func_80024D74;
static int same(u32 a,u32 b){if((b&0x7F800000U)==0x7F800000U&&(b&0x7FFFFFU))return (a&0x7F800000U)==0x7F800000U&&(a&0x7FFFFFU);return a==b;}
int main(void){
    Guarded input,output;Matrix original;u32 actual[12];size_t i;int alias,j,row,col,k;double value;
    typedef char representation[sizeof(float)==4&&sizeof(u32)==4&&sizeof(Matrix)==48&&offsetof(Matrix,t)==36&&FLT_RADIX==2?1:-1];
    (void)sizeof(representation);
    assert(fesetenv(FE_DFL_ENV)==0&&fesetround(FE_TONEAREST)==0);
    for(i=0;i<sizeof(tests)/sizeof(tests[0]);i++)for(alias=0;alias<2;alias++){
        input.before=output.before=0xABCDEF12U;input.after=output.after=0x12345678U;
        memcpy(&input.value,tests[i].input,sizeof(Matrix));memcpy(&original,&input.value,sizeof(Matrix));memset(&output.value,0xA5,sizeof(Matrix));
        assert(feclearexcept(FE_ALL_EXCEPT)==0);
        if(alias)call_body(&input.value,&input.value);else call_body(&output.value,&input.value);
        if(tests[i].divide_zero)assert(fetestexcept(FE_DIVBYZERO)!=0);
        memcpy(actual,alias?&input.value:&output.value,sizeof(actual));
        for(j=0;j<12;j++)if(!same(actual[j],alias?tests[i].alias[j]:tests[i].expected[j])){
            fprintf(stderr,"case %lu alias %d field %d: %08x versus %08x\n",(unsigned long)i,alias,j,actual[j],alias?tests[i].alias[j]:tests[i].expected[j]);return 1;
        }
        assert(input.before==0xABCDEF12U&&output.before==0xABCDEF12U&&input.after==0x12345678U&&output.after==0x12345678U);
        if(!alias){
            assert(memcmp(&input.value,&original,sizeof(Matrix))==0);
            if(tests[i].inverse){
                for(row=0;row<3;row++)for(col=0;col<3;col++){
                    value=0;for(k=0;k<3;k++)value+=(double)output.value.m[row][k]*original.m[k][col];
                    assert(fabs(value-(row==col?1.:0.))<0.0005);
                }
                for(row=0;row<3;row++){
                    value=output.value.t[row];for(k=0;k<3;k++)value+=(double)output.value.m[row][k]*original.t[k];assert(fabs(value)<0.0005);
                }
            }
        }
    }
    return 0;
}
'''.replace('FIXTURES', ',\n'.join(rows))
        with tempfile.TemporaryDirectory(prefix='bt06-matrix-tests-') as tmp:
            p = Path(tmp)
            prefix = '#include <assert.h>\n#include <fenv.h>\n#include <float.h>\n#include <math.h>\n#include <stddef.h>\n#include <stdio.h>\n#include <string.h>\n#include "' + str(SOURCE) + '"\n'
            (p / 'test.c').write_text(prefix + harness)
            for optimization in ('-O0', '-O2'):
                subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', optimization, '-fno-fast-math', '-ffp-contract=off', '-frounding-math', '-fsanitize=address,undefined', '-no-pie', str(p / 'test.c'), '-lm', '-o', str(p / 'test')], check=True, capture_output=True)
                result = subprocess.run([str(p / 'test')], capture_output=True, text=True, env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1'))
                self.assertEqual(result.returncode, 0, optimization + ': ' + result.stdout + result.stderr)


if __name__ == '__main__':
    unittest.main()
