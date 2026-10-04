#!/usr/bin/env python3
"""Compile the actual C89 candidate and test quantization, order and snapshots."""
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
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "SOURCE"
static unsigned int count, phase, expected_id, expected[4];
static Emitter *selected;
static Emitter after_callbacks;
static unsigned int rng = 0x41524941U;
static unsigned int random_word(void) { rng = rng * 1664525U + 1013904223U; return rng; }
static void disturb(void) {
    selected->identifier34 ^= 0xFFFFFFFFU;
    selected->flags08 ^= 0x100000U;
    selected->fade40 = 0.375f;
    after_callbacks = *selected;
}
u8 func_8001CC9C(u8 value) {
    assert(phase == 0 || phase == 2 || phase == 4);
    assert(value == (expected[phase / 2] & 255U));
    ++phase;
    disturb();
    return value > 127 ? 127 : value;
}
u16 func_8001CCC0(u32 value) {
    assert(phase == 6 && value == expected[3]);
    ++phase;
    disturb();
    return value > 16383 ? 16383 : (u16)value;
}
static int send(unsigned int id, unsigned int value, unsigned int slot) {
    unsigned int wanted;
    assert(phase == slot * 2 + 1 && id == expected_id);
    wanted = slot == 3 ? expected[slot] : expected[slot] & 255U;
    if (wanted > (slot == 3 ? 16383U : 127U)) wanted = slot == 3 ? 16383U : 127U;
    assert(value == wanted);
    ++phase;
    disturb();
    return (int)(slot % 2) - 1;
}
int func_8001B7C0(u32 id, u8 value) { return send(id, value, 0); }
int func_8001B29C(u32 id, u8 value) { return send(id, value, 1); }
int func_8001B3A0(u32 id, u8 value) { return send(id, value, 2); }
int func_8001B4A4(u32 id, u16 value) { return send(id, value, 3); }
static unsigned int reference_quantize(float value) {
    double wide;
    wide = value;
    assert(wide >= 0.0 && wide < 4294967296.0);
    return (unsigned int)wide;
}
static void trial(unsigned int flags, float fade, float vol, float xp, float yp, float zp, float doppler) {
    Emitter state;
    float gain, pan, surround, pitch;
    memset(&state, 0xA5, sizeof(state));
    state.flags08 = flags;
    state.identifier34 = random_word();
    state.fade40 = fade;
    selected = &state;
    expected_id = state.identifier34;
    gain = flags & 0x100000 ? fade * vol : vol;
    gain = gain * 127.0f;
    pan = 1.0f + xp;
    pan = pan * 64.0f;
    surround = 1.0f - zp;
    surround = surround * 64.0f;
    pitch = doppler * 8192.0f;
    expected[0] = reference_quantize(gain);
    expected[1] = reference_quantize(pan);
    expected[2] = reference_quantize(surround);
    expected[3] = reference_quantize(pitch);
    phase = 0;
    func_8001CCDC(&state, vol, xp, yp, zp, doppler);
    assert(phase == 8 && memcmp(&state, &after_callbacks, sizeof(state)) == 0);
    ++count;
}
int main(void) {
    unsigned int i, j, bits;
    float code, unused;
    static const unsigned int edges[] = {0, 1, 126, 127, 128, 254, 255, 256, 257, 511, 512, 16382, 16383, 16384, 65535, 65536, 0x7FFFFF80U, 0x80000000U, 0xFFFFFF00U};
    typedef char layout_check[sizeof(Emitter) == 68 && offsetof(Emitter, flags08) == 8 && offsetof(Emitter, identifier34) == 52 && offsetof(Emitter, fade40) == 64 ? 1 : -1];
    assert(sizeof(layout_check) == 1 && sizeof(unsigned int) == 4 && sizeof(float) == 4);
    bits = 0x7FC00001U;
    memcpy(&unused, &bits, sizeof(unused));
    for (i = 0; i < sizeof(edges) / sizeof(edges[0]); ++i) {
        code = (float)edges[i];
        for (j = 0; j < 2; ++j) {
            trial(j ? 0xFFFFFFFFU : 0xFFEFFFFFU, 0.5f, 0.75f, code / 64.0f - 1.0f, unused, 1.0f - code / 64.0f, code / 8192.0f);
        }
    }
    for (i = 0; i < 6000; ++i) {
        trial(random_word(), (float)(random_word() & 511U) / 256.0f,
              (float)(random_word() & 1023U) / 256.0f,
              (float)(random_word() & 65535U) / 64.0f - 1.0f,
              i & 1 ? unused : -123456.0f,
              1.0f - (float)(random_word() & 65535U) / 64.0f,
              (float)(random_word() & 65535U) / 8192.0f);
    }
    trial(0, 0.0f, -0.0f, -1.0f, unused, 1.0f, -0.0f);
    trial(0x100000U, 0.0f, 2.0f, 0.0f, unused, 0.0f, 2.0f);
    printf("%u\n", count);
    return 0;
}
'''


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--sanitize', action='store_true')
    ap.add_argument('--output', type=Path)
    args = ap.parse_args()
    source = HERE / 'func_8001CCDC_NONMATCH.c'
    flags = ['-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror',
             '-Wno-unused-parameter', '-fno-fast-math', '-ffp-contract=off', '-fexcess-precision=standard']
    if args.sanitize:
        flags += ['-fsanitize=address,undefined,float-cast-overflow', '-fno-omit-frame-pointer', '-no-pie']
    with tempfile.TemporaryDirectory(prefix='bt03-chain-host-') as folder:
        folder = Path(folder)
        harness = folder / 'test.c'
        harness.write_text(HARNESS.replace('SOURCE', str(source)))
        subprocess.run(['cc'] + flags + [str(harness), '-o', str(folder / 'test')], check=True, capture_output=True, text=True)
        result = subprocess.run([str(folder / 'test')], check=True, capture_output=True, text=True,
            env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1'))
    assert int(result.stdout) == 6040
    report = dict(result='PASS', calls=6040, callback_events=6040 * 8, sanitized=args.sanitize,
                  source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(), compiler_flags=flags,
                  domain='All evaluated float-to-u32 operands finite, nonnegative and below 2^32; unused yPan also NaN.',
                  coverage=['fade flag both paths and unrelated bits', 'zero/signed zero', '127/255/256 and 16383/16384 boundaries',
                            'large u32 conversion and low-byte wrap', 'ordered calls despite callee failure',
                            'captured identifier despite callback mutation', 'source read-only except modeled helper mutations',
                            'unused yPan independence'],
                  limitation='No C claim for negative integer-valued, nonfinite or >=2^32 conversion operands; native interpreter separately checks these.')
    rendered = json.dumps(report, indent=2) + '\n'
    if args.output:
        args.output.write_text(rendered)
    else:
        print(rendered, end='')


if __name__ == '__main__':
    main()
