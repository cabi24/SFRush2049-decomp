#!/usr/bin/env python3
"""Exercise the unchanged final C through C89 host harnesses and 32-bit layouts."""
from pathlib import Path
import json
import os
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
SOURCES = {
    '15720': PACKET / 'func_80015720_NONMATCH.c',
    '164D0': ROOT / 'cloud/matches/boot_tail/func_800164D0.c',
}
COMMON = r'''
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "line %d: %s\n", __LINE__, #x); exit(1); } } while (0)
'''
TEST157 = r'''
int D_8003C610;
RecordStorage D_8003C618[2050];
static RecordStorage expected[2050];
static int payloads[65536];
static int entered, left;
void func_80014594(void) { CHECK(entered == 0 && left == 0); entered++; }
void func_800145DC(void) { CHECK(entered == 1 && left == 0); left++; }
static void one(int n, unsigned int id, unsigned int refs)
{
    int i, k, result, expected_result, expected_count;
    void *payload;
    memset(D_8003C618, 0xa5, sizeof(D_8003C618));
    for (i = 0; i < n; i++) {
        D_8003C618[i].fields.payload = &payloads[i];
        D_8003C618[i].fields.id = (u16)(2 * i + 2);
        D_8003C618[i].fields.references = (u16)refs;
    }
    memcpy(expected, D_8003C618, sizeof(expected));
    D_8003C610 = n;
    expected_count = n;
    payload = &payloads[65535];
    for (k = 0; k < n && expected[k].fields.id < id; k++) { }
    expected_result = 0;
    if (k < n && expected[k].fields.id == id) {
        expected[k].fields.references = (u16)(expected[k].fields.references + 1);
    } else if (n < 2048) {
        for (i = n; i > k; i--) expected[i] = expected[i - 1];
        expected[k].fields.payload = payload;
        expected[k].fields.id = (u16)id;
        expected[k].fields.references = 1;
        expected_count++;
        expected_result = 1;
    }
    entered = left = 0;
    result = func_80015720((u16)id, payload);
    CHECK(result == expected_result && D_8003C610 == expected_count);
    CHECK(entered == 1 && left == 1);
    CHECK(memcmp(expected, D_8003C618, sizeof(expected)) == 0);
}
int main(void)
{
    int i, n;
    unsigned long state;
    state = 1234567;
    for (i = 0; i < 1000; i++) {
        state = (state * 1664525UL + 1013904223UL) & 0xffffffffUL;
        n = (int)(state % 200);
        one(n, (unsigned int)((state >> 8) % 450), (unsigned int)(state >> 16));
    }
    one(0, 0, 0); one(0, 65535, 0);
    one(1, 2, 65535); one(2047, 1, 1);
    one(2048, 1, 1); one(2048, 2048, 65535); one(2048, 65535, 1);
    puts("15720: 1007 cases passed");
    return 0;
}
'''
TEST164 = r'''
int D_80042228;
RecordStorage D_80042230[130];
static RecordStorage expected[130];
static Descriptor descriptors[65538], before[65538];
static int entered, left, guard_count;
void func_80014594(void) {
    CHECK(entered == 0 && left == 0); entered++;
    if (guard_count >= 0) D_80042228 = guard_count;
}
void func_800145DC(void) { CHECK(entered == 1 && left == 0); left++; }
static void one(int n, unsigned int id, unsigned int count, int reload)
{
    int i, k, result, added, index;
    memset(D_80042230, 0xa5, sizeof(D_80042230));
    for (i = 0; i < 130; i++) {
        D_80042230[i].fields.id = (u16)(2 * i + 2);
        D_80042230[i].fields.count = 17;
        D_80042230[i].fields.descriptors = &descriptors[0];
    }
    memcpy(expected, D_80042230, sizeof(expected));
    memset(descriptors, 0xa5, sizeof(descriptors));
    memcpy(before, descriptors, sizeof(before));
    for (k = 0; k < n && expected[k].fields.id != id; k++) { }
    added = k == n && n < 128;
    index = reload < 0 ? n : reload;
    if (added) {
        expected[index].fields.id = (u16)id;
        expected[index].fields.count = (u16)count;
        expected[index].fields.descriptors = &descriptors[1];
        for (i = 0; i < (int)count; i++) before[i + 1].state = 31;
    }
    D_80042228 = n;
    guard_count = reload;
    entered = left = 0;
    result = func_800164D0((u16)id, &descriptors[1], (u16)count);
    CHECK(result == added);
    CHECK(D_80042228 == (added ? index + 1 : n));
    CHECK(entered == added && left == added);
    CHECK(memcmp(expected, D_80042230, sizeof(expected)) == 0);
    CHECK(memcmp(before, descriptors, sizeof(before)) == 0);
}
int main(void)
{
    int i;
    for (i = 0; i < 200; i++) {
        one(i % 129, 1, (unsigned int)i, -1);
        one(i % 129, 2, (unsigned int)i, -1);
    }
    one(0, 65535, 65535, -1);
    one(127, 1, 4, -1); one(128, 1, 7, -1);
    one(0, 1, 5, 1); one(3, 1, 0, 4);
    puts("164D0: 405 cases passed");
    return 0;
}
'''
LAYOUT157 = r'''
typedef char storage_size[(sizeof(RecordStorage) == 8) ? 1 : -1];
typedef char fields_size[(sizeof(RecordFields) == 8) ? 1 : -1];
typedef char id_offset[(offsetof(RecordFields, id) == 4) ? 1 : -1];
typedef char references_offset[(offsetof(RecordFields, references) == 6) ? 1 : -1];
'''
LAYOUT164 = r'''
typedef char storage_size[(sizeof(RecordStorage) == 8) ? 1 : -1];
typedef char fields_size[(sizeof(RecordFields) == 8) ? 1 : -1];
typedef char id_offset[(offsetof(RecordFields, id) == 0) ? 1 : -1];
typedef char count_offset[(offsetof(RecordFields, count) == 2) ? 1 : -1];
typedef char payload_offset[(offsetof(RecordFields, descriptors) == 4) ? 1 : -1];
typedef char descriptor_size[(sizeof(Descriptor) == 12) ? 1 : -1];
typedef char state_offset[(offsetof(Descriptor, state) == 9) ? 1 : -1];
'''

def run(command, **kwargs):
    result = subprocess.run(command, capture_output=True, text=True, **kwargs)
    if result.returncode:
        raise SystemExit(result.stdout + result.stderr)
    return result.stdout.strip()

receipt = {'source_files_unchanged': True, 'host_pointer_width_is_not_native_abi_proof': True, 'runs': []}
with tempfile.TemporaryDirectory(prefix='bt03-low-larger-host-') as directory:
    work = Path(directory)
    for address, source in SOURCES.items():
        harness = work / (address + '.c')
        harness.write_text(COMMON + '\n#include "' + str(source) + '"\n' +
                           (TEST157 if address == '15720' else TEST164))
        for label, flags in [('c89-O2', ['-O2']), ('asan-ubsan-O1', ['-O1', '-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer'])]:
            binary = work / (address + '-' + label)
            run(['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', *flags, str(harness), '-o', str(binary)])
            env = dict(os.environ, ASAN_OPTIONS='detect_leaks=0')
            output = run([str(binary)], env=env)
            receipt['runs'].append({'function': 'func_800' + address, 'mode': label, 'result': output})
        layout = work / (address + '-layout.c')
        layout.write_text('#include <stddef.h>\n#include "' + str(source) + '"\n' +
                          (LAYOUT157 if address == '15720' else LAYOUT164))
        run(['cc', '-m32', '-std=c89', '-pedantic-errors', '-c', str(layout), '-o', str(work / (address + '-layout.o'))])
        receipt['runs'].append({'function': 'func_800' + address, 'mode': '32-bit-layout', 'result': 'PASS'})
print(json.dumps(receipt, indent=2))
