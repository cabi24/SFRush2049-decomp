"""Behavior checks for the explicitly nonmatching pad-state callback packet."""
from pathlib import Path
import shutil
import subprocess

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/dot_pad_channel_reset_20261005'


@pytest.mark.parametrize('packet_name', ['dot_pad_channel_reset_20261005',
                                        'dot_state_update_context_20261005'])
def test_accepted_context_prefix_is_unchanged(packet_name):
    accepted = ROOT / 'src/blob/groups/frontier_pad_config/group.c'
    packet = ROOT / 'cloud/work' / packet_name
    assert (packet / 'group.c').read_bytes().startswith(accepted.read_bytes())


def test_callback_behavior_and_native_layouts(tmp_path):
    cc = shutil.which('cc')
    if cc is None:
        pytest.skip('Host C compiler unavailable')
    harness = tmp_path / 'harness.c'
    source = (PACKET / 'group.c').as_posix()
    harness.write_text(r'''
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "SOURCE"

Record D_80140BF0[16];
s16 D_8014A108;
Player76 D_8014A118[32768];
Car772 D_80144030[256];
static Sprite expected;
static int stage;
static unsigned rng = 2049;
static unsigned next_random(void) { rng = rng * 1664525U + 1013904223U; return rng; }

void Input_SetAnalogBounds(s32 index, s32 a, s32 b, s32 c, s32 d) {
    assert(stage++ == 0);
    assert(index == expected.index);
    assert(a == expected.unk1C && b == expected.unk1E);
    assert(c == expected.unk20 && d == expected.unk22);
}
void input_status_update(s32 index, s32 a, s32 b, unsigned short c) {
    assert(stage++ == 1);
    assert(index == expected.index && a == expected.unk1A);
    assert(b == expected.unk18 && c == expected.unk12);
}
void Input_SetPadSecondaryFlag(s32 index, s32 a) {
    assert(stage++ == 2);
    assert(index == expected.index && a == expected.unk19);
}
void Input_SetPadEnabledFlag(s32 index, s32 a) {
    assert(stage++ == 3);
    assert(index == expected.index && a == (expected.unk0 == -1));
}

static void run_case(int count, int mode, int prior) {
    Sprite actual;
    Record before[16];
    int i, all = 1, changed;
    memset(&actual, 0x5a, sizeof(actual));
    actual.index = 7;
    actual.unk0 = (mode & 1) ? -1 : 0;
    actual.unk1A = prior;
    actual.unkE = -32768;
    actual.unk10 = 32767;
    actual.unk14 = -123;
    actual.unk16 = 456;
    actual.unk1C = -17;
    actual.unk1E = 29;
    actual.unk20 = -32768;
    actual.unk22 = 32767;
    for(i = 0; i < 256; i++) {
        D_80144030[i].flag = mode == 0 ? 1 : mode == 1 ? -1 :
            mode == 2 ? 0 : (s8)(next_random() >> 24);
    }
    D_8014A108 = count;
    for(i = 0; i < count; i++) {
        D_8014A118[i].car = (u8)next_random();
        if(!D_80144030[D_8014A118[i].car].flag) all = 0;
    }
    expected = actual;
    expected.unk1A = all;
    changed = prior != all;
    memset(D_80140BF0, 0x3c, sizeof(D_80140BF0));
    memcpy(before, D_80140BF0, sizeof(before));
    stage = 0;
    assert(audio_channel_reset(&actual) == 1);
    assert(stage == (changed ? 4 : 0));
    assert(memcmp(&actual, &expected, sizeof(actual)) == 0);
    if(changed) {
        Record *r = &D_80140BF0[7];
        assert(r->word0 == actual.unk4 && r->word4 == actual.unk8);
        assert(r->half8 == actual.unkC && r->halfA == (u16)actual.unkE);
        assert(r->halfC == (u16)actual.unk10);
        assert(r->half10 == (u16)actual.unk14 && r->half12 == (u16)actual.unk16);
        before[7] = *r;
    }
    assert(memcmp(before, D_80140BF0, sizeof(before)) == 0);
    /* A repeat call with unchanged inputs never reapplies the configuration. */
    stage = 0;
    assert(audio_channel_reset(&actual) == 1 && stage == 0);
}

int main(void) {
    static int counts[] = {-32768, -1, 0, 1, 2, 4, 255, 32767};
    static int priors[] = {-128, -1, 0, 1, 2, 127};
    int i, j, mode;
    assert(sizeof(Player76) == 76 && offsetof(Player76, car) == 1);
    assert(sizeof(Car772) == 772 && offsetof(Car772, flag) == 6);
    assert(offsetof(Sprite, unk1A) == 26 && offsetof(Sprite, index) == 52);
    for(i = 0; i < 8; i++) for(j = 0; j < 6; j++) for(mode = 0; mode < 4; mode++)
        run_case(counts[i], mode, priors[j]);
    for(i = 0; i < 1000; i++)
        run_case(next_random() % 512, 3, (s8)(next_random() >> 24));
    return 0;
}
'''.replace('SOURCE', source))
    binary = tmp_path / 'harness'
    subprocess.run([cc, '-std=c89', '-O2', '-Wall', '-Wextra', '-Werror',
                    str(harness), '-o', str(binary)], check=True, capture_output=True)
    subprocess.run([str(binary)], check=True, capture_output=True)


def test_state_update_reloads_and_callback_order(tmp_path):
    cc = shutil.which('cc')
    if cc is None:
        pytest.skip('Host C compiler unavailable')
    harness = tmp_path / 'state_harness.c'
    source = ROOT / 'cloud/work/dot_state_update_context_20261005/group.c'
    harness.write_text(r'''
#include <assert.h>
#include <string.h>
#include "SOURCE"

Record D_80140BF0[16];
s32 D_80149D98, D_80117358;
static Sprite *current;
static int phase, calls, mutation;
static s8 observed[2];
static u32 words4[2], words28[2];
static u32 read28(Sprite *p) { u32 v; memcpy(&v, (u8 *)p + 0x28, 4); return v; }
void Input_SetAnalogBounds(s32 index, s32 a, s32 b, s32 c, s32 d) {
    assert(phase++ == 0);
    assert(index == 7 && a == 0 && b == 0 && c == 0 && d == 0);
    assert(calls < 2);
    observed[calls] = current->unk1A;
    words4[calls] = current->unk4;
    words28[calls] = read28(current);
}
void input_status_update(s32 index, s32 a, s32 b, unsigned short c) {
    assert(phase++ == 1);
    assert(index == 7 && a == current->unk1A && b == 0 && c == 0);
}
void Input_SetPadSecondaryFlag(s32 index, s32 a) {
    assert(phase++ == 2 && index == 7 && a == 0);
}
void Input_SetPadEnabledFlag(s32 index, s32 a) {
    assert(phase++ == 3 && index == 7 && a == 0);
    phase = 0;
    calls++;
    if(mutation != 1000) current->unk1A = mutation;
}
static void run_case(s32 global, int prior, int mutate) {
    Sprite p;
    int expected_flag, desired, first, fallback;
    u32 sentinel = 0x12345678U;
    memset(&p, 0, sizeof(p));
    p.index = 7;
    p.unk1A = prior;
    p.unk4 = 0xabcdef01U;
    memcpy((u8 *)&p + 0x28, &sentinel, 4);
    D_80149D98 = global;
    current = &p; mutation = mutate; phase = calls = 0;
    desired = global != 0;
    first = prior != desired;
    expected_flag = first ? (mutate == 1000 ? desired : mutate) : prior;
    fallback = expected_flag == 0;
    assert(state_update_global(&p) == 1);
    assert(calls == first + fallback && phase == 0);
    if(first) {
        assert(observed[0] == desired);
        assert(words4[0] == 0xabcdef01U && words28[0] == sentinel);
    }
    if(fallback) {
        assert(observed[first] == 0);
        assert(words4[first] == (u32)&D_80117358);
        assert(words28[first] == sentinel);
        assert(p.unk4 == (u32)&D_80117358 && read28(&p) == 0);
        if(mutate != 1000) expected_flag = mutate;
    } else {
        assert(p.unk4 == 0xabcdef01U && read28(&p) == sentinel);
    }
    assert(p.unk1A == expected_flag);
    current = 0;
}
int main(void) {
    static s32 globals[] = {0, 1, -1, (-2147483647 - 1)};
    static int priors[] = {-128, 0, 1, 127};
    static int mutations[] = {1000, 0, 1, -1};
    int i, j, k;
    for(i = 0; i < 4; i++) for(j = 0; j < 4; j++) for(k = 0; k < 4; k++)
        run_case(globals[i], priors[j], mutations[k]);
    return 0;
}
'''.replace('SOURCE', source.as_posix()))
    binary = tmp_path / 'state_harness'
    # The source models 32-bit N64 address fields. Only that intentional
    # host pointer-width conversion warning is disabled in this behavior test.
    subprocess.run([cc, '-std=c89', '-O2', '-Wall', '-Wextra', '-Werror',
                    '-Wno-pointer-to-int-cast', str(harness), '-o', str(binary)],
                   check=True, capture_output=True)
    subprocess.run([str(binary)], check=True, capture_output=True)
