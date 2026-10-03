#!/usr/bin/env python3
"""Host contract tests of actual standalone sources; not native execution."""
from pathlib import Path
from hashlib import sha256
import json
import os
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
COMMON = '#include <assert.h>\n#include <stddef.h>\n#include <string.h>\n'
QUEUE = '''
struct MessageQueue { unsigned int marker; };
MessageQueue D_80038368;
static int calls;
'''
HARNESS = {
'80014550': QUEUE + '''
void *D_80038380;
void osCreateMesgQueue(MessageQueue *queue, void **messages, int count)
{
    assert(calls++ == 0);
    assert(queue == &D_80038368 && messages == &D_80038380 && count == 1);
    queue->marker = 0x10203040U;
}
int osJamMesg(MessageQueue *queue, void *message, int flag)
{
    assert(calls++ == 1);
    assert(queue == &D_80038368 && queue->marker == 0x10203040U);
    assert(message == 0 && flag == 0);
    return -1;
}
int main(void)
{
    func_80014550();
    assert(calls == 2);
    return 0;
}
''',
'80014594': QUEUE + '''
int D_8002C5DC;
static int replacement;
int osRecvMesg(MessageQueue *queue, void **destination, int flag)
{
    calls++;
    assert(queue == &D_80038368 && destination == 0 && flag == 1);
    assert(D_8002C5DC == 0);
    D_8002C5DC = replacement;
    return -1;
}
int main(void)
{
    D_8002C5DC = 0; replacement = 0;
    func_80014594(); assert(D_8002C5DC == 1 && calls == 1);
    func_80014594(); assert(D_8002C5DC == 2 && calls == 1);
    D_8002C5DC = -1;
    func_80014594(); assert(D_8002C5DC == 0 && calls == 1);
    replacement = 4;
    func_80014594(); assert(D_8002C5DC == 5 && calls == 2);
    return 0;
}
''',
'800145DC': QUEUE + '''
int D_8002C5DC;
int osJamMesg(MessageQueue *queue, void *message, int flag)
{
    calls++;
    assert(queue == &D_80038368 && message == 0 && flag == 0);
    assert(D_8002C5DC == 0);
    return -1;
}
int main(void)
{
    D_8002C5DC = -10;
    func_800145DC(); assert(D_8002C5DC == -10 && calls == 0);
    D_8002C5DC = 0;
    func_800145DC(); assert(D_8002C5DC == 0 && calls == 0);
    D_8002C5DC = 2;
    func_800145DC(); assert(D_8002C5DC == 1 && calls == 0);
    func_800145DC(); assert(D_8002C5DC == 0 && calls == 1);
    func_800145DC(); assert(D_8002C5DC == 0 && calls == 1);
    return 0;
}
''',
'8001489C': '''
AudioSlot *D_80038294;
static AudioSlot slots[3], expected[3];
static int calls, selected;
void func_80011A3C(AudioSlot *slot)
{
    calls++;
    assert(slot == &slots[selected] && slot->release_count == 20);
}
int main(void)
{
    int flag;
    assert(sizeof(AudioSlot) == 104);
    assert(offsetof(AudioSlot, position) == 20);
    assert(offsetof(AudioSlot, release_count) == 40);
    assert(offsetof(AudioSlot, release_pending) == 97);
    D_80038294 = slots;
    for (selected = 0; selected < 3; selected++) {
        for (flag = 0; flag < 256; flag++) {
            memset(slots, 0xA5, sizeof(slots));
            memcpy(expected, slots, sizeof(slots));
            expected[selected].release_count = 20;
            calls = 0;
            func_8001489C(selected, (unsigned char)flag);
            assert(calls == (flag != 0));
            assert(memcmp(slots, expected, sizeof(slots)) == 0);
        }
    }
    return 0;
}
''',
'80014AF0': '''
AudioSlot *D_80038294;
static AudioSlot slots[3], expected[3];
int main(void)
{
    int index, active;
    assert(sizeof(AudioSlot) == 104);
    assert(offsetof(AudioSlot, active) == 0);
    assert(offsetof(AudioSlot, release_pending) == 97);
    D_80038294 = slots;
    for (index = 0; index < 3; index++) {
        for (active = 0; active < 256; active++) {
            memset(slots, 0xA5, sizeof(slots));
            slots[index].active = (unsigned char)active;
            memcpy(expected, slots, sizeof(slots));
            if (active) {
                expected[index].active = 0;
                expected[index].release_pending = 1;
            }
            func_80014AF0(index);
            assert(memcmp(slots, expected, sizeof(slots)) == 0);
        }
    }
    return 0;
}
''',
'80014CAC': '''
static int calls;
static unsigned long seen;
static void *convert(void *address)
{
    calls++;
    seen = (unsigned long)address;
    return (void *)(seen ^ 0x1020UL);
}
void *(*D_80038014)(void *) = convert;
int main(void)
{
    unsigned long regions[] = {0UL, 0x7FFFFFFFUL, 0x80000000UL, 0x80FFFFFFUL,
        0x81000000UL, 0x8FFFFFFFUL, 0xA0000000UL, 0xAFFFFFFFUL,
        0xB0000000UL, 0xB0FFFFFFUL, 0xB1000000UL, 0xFFFFFFFFUL};
    unsigned int i;
    unsigned long region, expected;
    for (i = 0; i < sizeof(regions) / sizeof(regions[0]); i++) {
        calls = 0;
        region = regions[i] & 0xFF000000UL;
        expected = regions[i];
        if (region != 0x80000000UL && region != 0xB0000000UL) expected ^= 0x1020UL;
        assert((unsigned long)func_80014CAC((void *)regions[i]) == expected);
        assert(calls == (region != 0x80000000UL && region != 0xB0000000UL));
        if (calls) assert(seen == regions[i]);
    }
    return 0;
}
'''
}
rows = []
with tempfile.TemporaryDirectory(prefix='bt03-low-medium-host-') as directory:
    tmp = Path(directory)
    layout = tmp / 'layout.c'
    slot_source = (ROOT / 'cloud/matches/boot_tail/func_80014AF0.c').read_text().split('extern ')[0]
    layout.write_text(slot_source + '''
#include <stddef.h>
#define CHECK(n,e) typedef char n[(e)?1:-1]
CHECK(slot_size, sizeof(AudioSlot)==104);
CHECK(active, offsetof(AudioSlot,active)==0);
CHECK(pending, offsetof(AudioSlot,pending)==1);
CHECK(position, offsetof(AudioSlot,position)==20);
CHECK(release_count, offsetof(AudioSlot,release_count)==40);
CHECK(release_pending, offsetof(AudioSlot,release_pending)==97);
''')
    subprocess.run(['cc', '-m32', '-std=c89', '-pedantic-errors', '-Werror', '-fsyntax-only', str(layout)], check=True)
    for address, harness in HARNESS.items():
        source = (PACKET if address == '8001489C' else ROOT / 'cloud/matches/boot_tail') / ('func_' + address + '.c')
        unit = tmp / (address + '.c')
        unit.write_text(COMMON + '#include "' + str(source) + '"\n' + harness)
        modes = []
        for mode, flags in [('c89-O2', ['-O2']),
                            ('c89-asan-ubsan-O1', ['-O1', '-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer'])]:
            executable = tmp / (address + '-' + mode)
            subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror',
                            '-Wno-pointer-to-int-cast'] + flags + [str(unit), '-o', str(executable)], check=True)
            environment = dict(os.environ, ASAN_OPTIONS='detect_leaks=0')
            subprocess.run([str(executable)], check=True, env=environment)
            modes.append(mode)
        rows.append({'name': source.stem, 'source_path': str(source.relative_to(ROOT)),
                     'source_sha256': sha256(source.read_bytes()).hexdigest(), 'result': 'PASS', 'modes': modes, 'asan_options': 'detect_leaks=0 (ptrace environment does not support LeakSanitizer)'})
print(json.dumps({'result': 'PASS', 'host_compiler': subprocess.check_output(['cc', '--version'], text=True).splitlines()[0],
                  'results': rows, '32bit_c89_slot_layout': 'PASS',
                  'scope': 'Actual standalone sources with test-only mocks. Scalar counters exclude signed-overflow states. Pointer addresses are non-dereferenced 32-bit values represented on the host; this does not prove native execution or cartridge behavior.'}, indent=2))
