#!/usr/bin/env python3
"""Behavior checks of final sources; host tests do not replace target matching."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
SHARED = '''
AudioNode *D_80043EB0;
AudioState state_storage;
AudioState *D_8004BE80 = &state_storage;
AudioNode nodes[8];
static void reset_state(void) {
    memset(nodes, 0, sizeof(nodes));
    memset(&state_storage, 0, sizeof(state_storage));
    D_80043EB0 = 0;
}
'''
CASES = {
'800148F8': ('''
AudioSlot slots[3];
AudioSlot *D_80038294 = slots;
int main(void) {
    unsigned short data[4];
    unsigned int i;
    memset(slots, 0xA5, sizeof(slots));
    for (i = 0; i < 65536; i += 257) {
        data[0] = (unsigned short)i;
        data[1] = (unsigned short)(65535 - i);
        data[2] = (unsigned short)i;
        data[3] = (unsigned short)(i ^ 0xA55A);
        func_800148F8(1, data);
        assert(slots[1].value20 == data[0]);
        assert(slots[1].value22 == data[1]);
        assert(slots[1].value24 == (float)i / 4096.0f);
        assert(slots[1].release_count == data[3]);
        assert(slots[1].active == 0xA5 && slots[1].unknown2A[0] == 0xA5);
        assert(slots[0].active == 0xA5 && slots[2].active == 0xA5);
    }
    return 0;
}
''', '256 input vectors including zero/full-range values; only selected slot fields change'),
'80014D30': ('''
int D_8004F800;
int main(void) {
    unsigned int rates[6] = {0U, 1U, 4096U, 65535U, 0x80000000U, 0xFFFFFFFFU};
    int divisors[4] = {22050, 44100, 48000, 1000000};
    unsigned int i, j, expected;
    float scaled;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 4; j++) {
            D_8004F800 = divisors[j];
            scaled = (float)rates[i];
            scaled = scaled * 4096.0f;
            scaled = scaled / (float)divisors[j];
            expected = (unsigned int)scaled;
            assert(func_80014D30(rates[i]) == expected);
        }
    }
    return 0;
}
''', '24 in-range float-to-unsigned vectors including high-bit/full-width unsigned inputs'),
'800171C0': ('''
AudioNode D_800426B0[256];
AudioNode *D_80043EB0;
int main(void) {
    int i;
    memset(D_800426B0, 0x5A, sizeof(D_800426B0));
    func_800171C0();
    assert(D_80043EB0 == &D_800426B0[0]);
    for (i = 0; i < 256; i++) {
        assert(D_800426B0[i].previous == (i ? &D_800426B0[i - 1] : 0));
        assert(D_800426B0[i].next == (i < 255 ? &D_800426B0[i + 1] : 0));
        assert(D_800426B0[i].unknown08[0] == 0x5A);
    }
    func_800171C0();
    assert(D_800426B0[255].next == 0 && D_800426B0[0].previous == 0);
    return 0;
}
''', 'all 256 forward/back links, sentinel ends, payload preservation and repeated initialization'),
'8001729C': (SHARED + '''
int main(void) {
    int a, p, f, i, count;
    AudioNode *cursor, *expected[5];
    for (a = 0; a < 2; a++) for (p = 0; p < 2; p++) for (f = 0; f < 2; f++) {
        reset_state();
        if (a) { state_storage.active = &nodes[0]; nodes[0].next = &nodes[1]; nodes[1].previous = &nodes[0]; }
        if (p) { state_storage.pending = &nodes[2]; nodes[2].next = &nodes[3]; nodes[3].previous = &nodes[2]; }
        if (f) D_80043EB0 = &nodes[4];
        func_8001729C(&state_storage);
        assert(state_storage.active == 0 && state_storage.pending == 0);
        count = 0;
        if (p) { expected[count++] = &nodes[2]; expected[count++] = &nodes[3]; }
        if (a) { expected[count++] = &nodes[0]; expected[count++] = &nodes[1]; }
        if (f) expected[count++] = &nodes[4];
        cursor = D_80043EB0;
        for (i = 0; i < count; i++) {
            assert(cursor == expected[i]);
            assert(cursor->previous == (i ? expected[i - 1] : 0));
            cursor = cursor->next;
        }
        assert(cursor == 0);
        func_8001729C(&state_storage);
        assert(D_80043EB0 == (count ? expected[0] : 0));
    }
    return 0;
}
''', 'eight empty/nonempty active/pending/free combinations; order/backlinks and idempotent second release'),
'800173B4': (SHARED + '''
int main(void) {
    int free_count, active;
    AudioNode *result;
    for (free_count = 0; free_count < 3; free_count++) for (active = 0; active < 2; active++) {
        reset_state();
        if (free_count) D_80043EB0 = &nodes[0];
        if (free_count == 2) { nodes[0].next = &nodes[1]; nodes[1].previous = &nodes[0]; }
        if (active) state_storage.active = &nodes[2];
        result = func_800173B4();
        if (!free_count) { assert(result == 0); assert(state_storage.active == (active ? &nodes[2] : 0)); }
        else {
            assert(result == &nodes[0] && state_storage.active == result && result->previous == 0);
            assert(result->next == (active ? &nodes[2] : 0));
            if (active) assert(nodes[2].previous == result);
            assert(D_80043EB0 == (free_count == 2 ? &nodes[1] : 0));
            if (D_80043EB0) assert(D_80043EB0->previous == 0);
        }
    }
    return 0;
}
''', 'six free-list exhaustion/single/multiple and active-empty/nonempty combinations')}
for address, field in [('80017410', 'active'), ('80017470', 'pending')]:
    CASES[address] = (SHARED + '''
int main(void) {
    int left, right, free_list;
    for (left = 0; left < 2; left++) for (right = 0; right < 2; right++) for (free_list = 0; free_list < 2; free_list++) {
        reset_state();
        state_storage.FIELD = left ? &nodes[0] : &nodes[1];
        nodes[1].previous = left ? &nodes[0] : 0;
        nodes[1].next = right ? &nodes[2] : 0;
        if (left) nodes[0].next = &nodes[1];
        if (right) nodes[2].previous = &nodes[1];
        if (free_list) D_80043EB0 = &nodes[3];
        FUNC(&nodes[1]);
        assert(D_80043EB0 == &nodes[1] && nodes[1].previous == 0);
        assert(nodes[1].next == (free_list ? &nodes[3] : 0));
        if (free_list) assert(nodes[3].previous == &nodes[1]);
        assert(state_storage.FIELD == (left ? &nodes[0] : (right ? &nodes[2] : 0)));
        if (left) assert(nodes[0].next == (right ? &nodes[2] : 0));
        if (right) assert(nodes[2].previous == (left ? &nodes[0] : 0));
    }
    return 0;
}
'''.replace('FIELD', field).replace('FUNC', 'func_' + address),
        'eight head/middle/tail/only-node and free-list empty/nonempty combinations')
CASES['800174D0'] = (SHARED + '''
int main(void) {
    int left, right, pending;
    for (left = 0; left < 2; left++) for (right = 0; right < 2; right++) for (pending = 0; pending < 2; pending++) {
        reset_state();
        state_storage.active = left ? &nodes[0] : &nodes[1];
        nodes[1].previous = left ? &nodes[0] : 0;
        nodes[1].next = right ? &nodes[2] : 0;
        if (left) nodes[0].next = &nodes[1];
        if (right) nodes[2].previous = &nodes[1];
        if (pending) state_storage.pending = &nodes[3];
        func_800174D0(&nodes[1]);
        assert(state_storage.pending == &nodes[1] && nodes[1].previous == 0);
        assert(nodes[1].next == (pending ? &nodes[3] : 0));
        if (pending) assert(nodes[3].previous == &nodes[1]);
        assert(state_storage.active == (left ? &nodes[0] : (right ? &nodes[2] : 0)));
        if (left) assert(nodes[0].next == (right ? &nodes[2] : 0));
        if (right) assert(nodes[2].previous == (left ? &nodes[0] : 0));
        assert(D_80043EB0 == 0);
    }
    return 0;
}
''', 'eight active head/middle/tail/only-node and pending-empty/nonempty combinations')


def run():
    rows = []
    with tempfile.TemporaryDirectory(prefix='bt03-low-eight-host-') as directory:
        tmp = Path(directory)
        for address, (harness, coverage) in sorted(CASES.items()):
            source = ROOT / 'cloud/matches/boot_tail' / ('func_' + address + '.c')
            test = tmp / ('host_' + address + '.c')
            test.write_text('#include <assert.h>\n#include <string.h>\n#define __unaligned\n' +
                            source.read_text() + harness)
            executable = tmp / ('host_' + address)
            command = ['cc', '-std=c89', '-pedantic', '-O2', '-Wall', '-Wextra', '-Werror',
                       '-fsanitize=undefined', '-fno-sanitize-recover=all', str(test), '-o', str(executable)]
            subprocess.run(command, check=True, capture_output=True, text=True)
            subprocess.run([str(executable)], check=True, capture_output=True, text=True)
            rows.append({'function': 'func_' + address, 'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
                         'result': 'PASS', 'coverage': coverage})
    return {'schema_version': 1, 'result': 'PASS', 'source_language': 'C89',
            'compiler': 'host cc -std=c89 -pedantic -O2 -Wall -Wextra -Werror -fsanitize=undefined -fno-sanitize-recover=all',
            'limits': 'Host pointer width/endian differ from MIPS. __unaligned is erased only for aligned host input arrays; these tests do not establish odd-address/big-endian loads, native struct layout, or invalid/out-of-range float conversion behavior. Strict target replay and independent IDO layout assertions cover native ABI/layout.',
            'results': rows}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    text = json.dumps(run(), indent=2) + '\n'
    if args.output:
        args.output.write_text(text)
    else:
        print(text, end='')
