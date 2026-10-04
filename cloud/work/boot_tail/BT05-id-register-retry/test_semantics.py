"""Run the actual one-form candidate in a C89 host harness with UBSan.

Host tests establish source arithmetic/call behavior, not N64 object equality.
The helper implementation below is test-only and never used for IDO scoring.
"""
import json
from pathlib import Path
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score


def run():
    source = (WORK / 'candidates/func_80022324.c').read_text()
    harness = source + r'''
#include <assert.h>
VoiceState D_8004BEB8[256];
u8 D_8004FA18;
static unsigned int calls, seen[256];
static int change;
int func_80021700(u32 id) {
    assert(calls < 256);
    seen[calls++] = id;
    if (change == 1) { ++D_8004FA18; change = 0; }
    if (change == 2) { D_8004FA18 = 0; change = 0; }
    return 0;
}
static void one(u32 group, u32 offset, u32 tag) {
    VoiceState voice;
    MacroCommand command;
    u32 expected;
    voice.keyGroup4E = (u16)group;
    command.word0 = (tag << 16) | (offset << 8) | 0x5aU;
    command.word1 = 0xffffffffU;
    expected = ((group + offset) << 8) | (tag << 16);
    D_8004BEB8[0].id60 = expected;
    calls = 0;
    D_8004FA18 = 1;
    assert(func_80022324(&voice, &command) == 0);
    assert(calls == 1 && seen[0] == expected);
}
int main(void) {
    u32 group, offset, tag, i, base;
    VoiceState voice;
    MacroCommand command;
    /* All 16,777,216 low-composition pairs, with full-width tag variation. */
    for (group = 0; group < 65536U; ++group) {
        for (offset = 0; offset < 256U; ++offset) {
            one(group, offset, (group ^ (offset * 257U)) & 65535U);
        }
    }
    /* Every high-half value, crossed with low-sum boundaries. */
    for (tag = 0; tag < 65536U; ++tag) {
        one(0, 0, tag); one(65535U, 255U, tag);
    }
    voice.keyGroup4E = 65535U;
    command.word0 = 0xffffff5aU; command.word1 = 0;
    base = ((65535U + 255U) << 8) | 0xffff0000U;
    for (i = 0; i < 256U; ++i) D_8004BEB8[i].id60 = base | i;
    calls = 0; D_8004FA18 = 0;
    assert(func_80022324(&voice, &command) == 0 && calls == 0);
    calls = 0; D_8004FA18 = 255;
    assert(func_80022324(&voice, &command) == 0 && calls == 255);
    for (i = 0; i < 255U; ++i) assert(seen[i] == (base | i));
    calls = 0; D_8004FA18 = 4; change = 1;
    D_8004BEB8[2].id60 ^= 0x100U;
    assert(func_80022324(&voice, &command) == 0 && calls == 4);
    assert(seen[0] == base && seen[1] == (base | 1U));
    assert(seen[2] == (base | 3U) && seen[3] == (base | 4U));
    calls = 0; D_8004FA18 = 4; change = 2;
    assert(func_80022324(&voice, &command) == 0 && calls == 1);
    return 0;
}
'''
    with tempfile.TemporaryDirectory(prefix='bt05-id-semantics-') as directory:
        directory = Path(directory)
        cfile = directory / 'host.c'
        binary = directory / 'host'
        cfile.write_text(harness)
        subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-O1',
                        '-fsanitize=undefined', '-fno-sanitize-recover=all', str(cfile),
                        '-o', str(binary)], check=True, capture_output=True, text=True)
        subprocess.run([str(binary)], check=True, capture_output=True, text=True)
        layout = source.split('extern VoiceState')[0] + '''
#define offsetof(T, M) ((unsigned int)&(((T *)0)->M))
typedef char pointer_size[sizeof(void *) == 4 ? 1 : -1];
typedef char word_size[sizeof(u32) == 4 ? 1 : -1];
typedef char voice_size[sizeof(VoiceState) == 416 ? 1 : -1];
typedef char command_size[sizeof(MacroCommand) == 8 ? 1 : -1];
typedef char group_offset[offsetof(VoiceState, keyGroup4E) == 78 ? 1 : -1];
typedef char identifier_offset[offsetof(VoiceState, id60) == 96 ? 1 : -1];
'''
        layout_file = directory / 'layout.c'
        layout_file.write_text(layout)
        score.compile_single(layout_file, '-g0 -O2 -mips2 -G 0 -non_shared', directory / 'layout.o')
    return {'result': 'PASS', 'low_pairs': 16777216, 'high_boundary_cases': 131072,
            'actual_candidate_executed': True, 'c89_ubsan': True, 'ido_layout': True,
            'zero_and_max_count': True, 'live_count_growth_and_shrink': True,
            'mismatch_skipped': True,
            'limitations': ['Host pointers/endianness differ; not N64 execution or a match proof.',
                           'The count-changing helper is test-only, never scored.']}


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
