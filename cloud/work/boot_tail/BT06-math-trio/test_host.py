#!/usr/bin/env python3
"""C89 actual-source semantic controls, including sequential input/output aliasing."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
HARNESS = r'''
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
typedef struct Vector { float x, y, z; } Vector;
typedef struct Matrix { float m[3][3]; float t[3]; } Matrix;
extern void func_80024BF0(const Matrix *, const Vector *, Vector *);
extern float func_80024C9C(Vector *);
extern void func_80024D04(Vector *, const Vector *, const Vector *);
extern float sqrtf(float);
static unsigned int rng = 0x93612049U;
static unsigned int calls;
static float sample(void)
{
    rng = rng * 1664525U + 1013904223U;
    return (float)((int)((rng >> 8) % 20001U) - 10000) / 64.0f;
}
static int equal(float a, float b)
{
    unsigned int ua, ub;
    if (a != a && b != b) return 1;
    memcpy(&ua, &a, 4);
    memcpy(&ub, &b, 4);
    return ua == ub;
}
static void check(const Vector *a, const Vector *b)
{
    assert(equal(a->x, b->x));
    assert(equal(a->y, b->y));
    assert(equal(a->z, b->z));
}
static Vector vector(void)
{
    Vector v;
    v.x = sample(); v.y = sample(); v.z = sample();
    return v;
}
static void oracle_apply(const Matrix *m, const Vector *v, Vector *out)
{
    float a, b, c;
    int i;
    for (i = 0; i < 3; ++i) {
        a = m->m[i][0] * v->x;
        b = m->m[i][1] * v->y;
        c = m->m[i][2] * v->z;
        a = a + b;
        a = a + c;
        a = a + m->t[i];
        if (i == 0) out->x = a;
        else if (i == 1) out->y = a;
        else out->z = a;
    }
}
static float component(const Vector *v, int i)
{
    return i == 0 ? v->x : (i == 1 ? v->y : v->z);
}
static void oracle_cross(Vector *out, const Vector *a, const Vector *b)
{
    int i, j, k;
    float x, y;
    for (i = 0; i < 3; ++i) {
        j = (i + 1) % 3; k = (i + 2) % 3;
        x = component(a, j) * component(b, k);
        y = component(a, k) * component(b, j);
        if (i == 0) out->x = x - y;
        else if (i == 1) out->y = x - y;
        else out->z = x - y;
    }
}
static void normalization(Vector original)
{
    Vector actual, expect;
    float sum, length, returned;
    actual = original; expect = original;
    sum = original.x * original.x + original.y * original.y;
    sum = sum + original.z * original.z;
    length = sqrtf(sum);
    expect.x /= length; expect.y /= length; expect.z /= length;
    returned = func_80024C9C(&actual);
    assert(equal(length, returned)); check(&actual, &expect); ++calls;
}
int main(void)
{
    Matrix m;
    Vector a, b, actual, expect;
    unsigned int bits;
    int n, i, j;
    assert(sizeof(float) == 4 && sizeof(unsigned int) == 4);
    for (n = 0; n < 1000; ++n) {
        for (i = 0; i < 3; ++i) {
            m.t[i] = sample();
            for (j = 0; j < 3; ++j) m.m[i][j] = sample();
        }
        a = vector(); b = vector();
        oracle_apply(&m, &a, &expect);
        func_80024BF0(&m, &a, &actual); check(&actual, &expect); ++calls;
        expect = a; actual = a;
        oracle_apply(&m, &expect, &expect);
        func_80024BF0(&m, &actual, &actual); check(&actual, &expect); ++calls;
        oracle_cross(&expect, &a, &b);
        func_80024D04(&actual, &a, &b); check(&actual, &expect); ++calls;
        expect = a; actual = a;
        oracle_cross(&expect, &expect, &b);
        func_80024D04(&actual, &actual, &b); check(&actual, &expect); ++calls;
        expect = b; actual = b;
        oracle_cross(&expect, &a, &expect);
        func_80024D04(&actual, &a, &actual); check(&actual, &expect); ++calls;
        normalization(a);
    }
    /* Identity plus translation is checked without the generic oracle. */
    memset(&m, 0, sizeof(m));
    m.m[0][0] = m.m[1][1] = m.m[2][2] = 1.0f;
    m.t[0] = 2.0f; m.t[1] = -3.0f; m.t[2] = 5.0f;
    a.x = 4.0f; a.y = 6.0f; a.z = -2.0f;
    expect.x = 6.0f; expect.y = 3.0f; expect.z = 3.0f;
    func_80024BF0(&m, &a, &actual); check(&actual, &expect); ++calls;
    a.x = 1.0f; a.y = a.z = 0.0f;
    b.y = 1.0f; b.x = b.z = 0.0f;
    expect.x = expect.y = 0.0f; expect.z = 1.0f;
    func_80024D04(&actual, &a, &b); check(&actual, &expect); ++calls;
    /* No invented zero/infinity/NaN guard: preserve the actual arithmetic. */
    a.x = a.y = a.z = 0.0f; normalization(a);
    bits = 0x80000000U; memcpy(&a.x, &bits, 4); normalization(a);
    a.x = 3.0f; a.y = 4.0f; a.z = 0.0f; normalization(a);
    bits = 0x7F800000U; memcpy(&a.x, &bits, 4); normalization(a);
    bits = 0x7FC00000U; memcpy(&a.x, &bits, 4); normalization(a);
    printf("PASS %u actual-source calls\n", calls);
    return 0;
}
'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', type=Path, default=HERE.parents[3])
    parser.add_argument('--sanitize', action='store_true')
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    root = args.repo.resolve()
    sources = [root / 'cloud/matches/boot_tail' / ('func_' + a + '.c')
               for a in ['80024BF0', '80024C9C', '80024D04']]
    flags = ['-std=c89', '-pedantic-errors', '-O2', '-fno-fast-math', '-ffp-contract=off', '-fno-builtin']
    if args.sanitize:
        flags += ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']
    with tempfile.TemporaryDirectory(prefix='bt06-math-host-') as folder:
        folder = Path(folder)
        harness = folder / 'harness.c'
        harness.write_text(HARNESS)
        binary = folder / 'test'
        subprocess.run([os.environ.get('CC', 'cc')] + flags + [str(harness)] + [str(s) for s in sources]
                       + ['-lm', '-o', str(binary)], check=True)
        env = os.environ.copy()
        if args.sanitize:
            # LeakSanitizer cannot run under this container's tracing sandbox.
            # Address/undefined checks remain enabled; this harness does not allocate.
            env['ASAN_OPTIONS'] = env.get('ASAN_OPTIONS', '') + ':detect_leaks=0'
        output = subprocess.check_output([str(binary)], text=True, env=env).strip()
        assert output == 'PASS 6007 actual-source calls', output
    report = dict(schema_version=1, result='PASS', calls=6007, flags=flags,
                  leak_detection='not run: sandbox ptrace limitation' if args.sanitize else 'not requested',
                  source_hashes={str(s.relative_to(root)): hashlib.sha256(s.read_bytes()).hexdigest() for s in sources},
                  controls=['1000 disjoint matrix transforms', '1000 in-place sequential matrix transforms',
                            '1000 disjoint cross products', '1000 cross outputs alias first input',
                            '1000 cross outputs alias second input', '1000 finite vector normalizations',
                            'identity plus translation', 'oriented unit cross product',
                            'zero, negative zero, 3-4-0, infinity and NaN normalization'],
                  limitation='Host semantics only; MIPS layout and exact native floating operation order are verified separately. NaN payload equality is not asserted.')
    rendered = json.dumps(report, indent=2) + '\n'
    if args.output:
        args.output.write_text(rendered)
    else:
        print(rendered, end='')


if __name__ == '__main__':
    main()
