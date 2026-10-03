#!/usr/bin/env python3
"""Check 32-bit layouts and run real C in a native-width C89 host harness."""
from pathlib import Path
import json
import os
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
MATCH = ROOT/'cloud/matches/boot_tail'
SOURCES = [MATCH/('func_'+a+'.c') for a in ['800250AC','800251A8','80025264','80025EB0','800262BC']]
TYPES = (MATCH/'func_800262BC.c').read_text().split('\nunsigned int func_800262BC')[0]
HARNESS = r'''
#include <stddef.h>
#define CHECK(x) do { if (!(x)) return __LINE__; } while (0)
#define OFFSET(t, m) ((unsigned int)&((t *)0)->m)
#ifndef WIDE_HOST
typedef char layout_size[(sizeof(StreamState) == 4648) ? 1 : -1];
typedef char queue_size[(sizeof(OSMesgQueue) == 24) ? 1 : -1];
typedef char remaining_offset[(offsetof(StreamState, remaining) == 4520) ? 1 : -1];
typedef char queue_offset[(offsetof(StreamState, queue) == 4524) ? 1 : -1];
typedef char state_offset[(offsetof(StreamState, state) == 4573) ? 1 : -1];
typedef char busy_offset[(offsetof(StreamState, busy) == 4608) ? 1 : -1];
typedef char token_offset[(offsetof(StreamState, token) == 4644) ? 1 : -1];
#endif
StreamState D_80056230[2];
unsigned int D_80058680;
OSMesgQueue D_800586A8;
OSMesg D_800586C0;
static OSMesgQueue *created;
static OSMesg *storage;
static int capacity;
static int jammed;
void osCreateMesgQueue(OSMesgQueue *queue, OSMesg *messages, int count)
{
    created = queue;
    storage = messages;
    capacity = count;
}
int osJamMesg(OSMesgQueue *queue, OSMesg message, int flags)
{
    if (queue == &D_800586A8 && message == 0 && flags == 0) jammed++;
    return 0;
}
void bzero(void *memory, int length)
{
    unsigned char *p = memory;
    while (length-- > 0) *p++ = 0;
}
static void callback(void *a, void *b, unsigned int c, OSMesgQueue *q)
{
    (void)a; (void)b; (void)c; (void)q;
}
extern void func_800250AC(void);
extern unsigned int func_800251A8(int);
extern int func_80025264(unsigned int);
extern unsigned int func_800262BC(StreamState *, unsigned int);
extern void func_80025EB0(StreamState *, void *, void *, unsigned int,
                         void (*)(void *, void *, unsigned int, OSMesgQueue *));
int main(void)
{
    StreamState *s = &D_80056230[0];
    unsigned char *bytes = (unsigned char *)s;
    unsigned int count, wanted, before, got;
    int state, remaining, i;
    #ifndef WIDE_HOST
    CHECK(sizeof(OSMesgQueue) == 24);
    CHECK(OFFSET(StreamState, remaining) == 4520);
    CHECK(OFFSET(StreamState, queue) == 4524);
    CHECK(OFFSET(StreamState, state) == 4573);
    CHECK(OFFSET(StreamState, busy) == 4608);
    CHECK(OFFSET(StreamState, token) == 4644);
    #endif
    func_800250AC();
    CHECK(created == &D_800586A8 && storage == &D_800586C0 && capacity == 1 && jammed == 1);
    for (i = 0; i < (int)sizeof(StreamState); i++) bytes[i] = 0x5a;
    func_80025EB0(s, (void *)0x1234, (void *)0x5678, 160, callback);
    CHECK(s->block_count == 40 && s->data == (void *)0x1234 && s->argument2 == (void *)0x5678);
    CHECK(s->argument3 == 160 && s->callback == callback);
    CHECK(s->state == 1 && s->scale == 2 && s->remaining == 0);
    CHECK(s->read_count == 0 && s->write_count == 0 && s->buffered == 0);
    CHECK(s->field_11C8 == 0 && s->field_11CC == 0 && s->field_11D0 == 0);
    CHECK(s->available == 0 && s->consumed == 0 && s->field_11DC == 0);
    CHECK(created == &s->queue && storage == &s->message && capacity == 1);
    for (i = 0; i < 400; i++) if (i != 358 && i != 359) CHECK(bytes[i] == 0);
    for (i = (int)((unsigned char *)&s->unknown_01A0 - bytes); i < (int)((unsigned char *)&s->read_count - bytes); i++) CHECK(bytes[i] == 0x5a);
    CHECK(s->busy == 0x5a && s->token == 0x5a5a5a5aU);
    func_80025EB0(s, 0, 0, 0, callback); CHECK(s->state == 0);
    func_80025EB0(s, (void *)1, 0, 0, 0); CHECK(s->state == 0);
    for (state = -128; state < 128; state++) {
        for (remaining = -1; remaining <= 3; remaining++) {
            for (count = 0; count <= 5; count++) {
                s->state = (signed char)state; s->remaining = remaining;
                s->consumed = 100; s->available = 103; before = s->consumed;
                wanted = state == 3 ? (count < 3 ? count : 3) : 0;
                got = func_800262BC(s, count);
                CHECK(got == wanted && s->consumed == before + wanted);
                if (state == 3 && remaining > 0) {
                    CHECK(s->remaining == (remaining > (int)count ? remaining - (int)count : 0));
                    CHECK(s->state == (remaining > (int)count ? 3 : 4));
                } else CHECK(s->state == state && s->remaining == remaining);
            }
        }
    }
    s->busy = 0; D_80056230[1].busy = 0;
    CHECK(func_80025264(99) == -1);
    s->token = 99; s->busy = 1; CHECK(func_80025264(99) == 0);
    D_80056230[1].token = 99; D_80056230[1].busy = 1;
    s->busy = 0; CHECK(func_80025264(99) == 1);
    CHECK(func_80025264(100) == -1);
    s->busy = 1; s->token = 5; D_80056230[1].token = 5; D_80058680 = 5;
    CHECK(func_800251A8(0) == 6 && s->token == 6 && D_80058680 == 7);
    D_80056230[1].busy = 0; D_80058680 = 6;
    CHECK(func_800251A8(0) == 6 && D_80058680 == 7);
    D_80058680 = 0xfffffffeU;
    CHECK(func_800251A8(0) == 0xfffffffeU && D_80058680 == 0);
    D_80056230[1].busy = 1; D_80056230[1].token = 0xfffffffeU; D_80058680 = 0xfffffffeU;
    CHECK(func_800251A8(0) == 0 && D_80058680 == 1);
    return 0;
}
'''


def run():
    with tempfile.TemporaryDirectory(prefix='bt07-medium-host-') as tmp:
        tmp = Path(tmp)
        harness = tmp/'harness.c'
        harness.write_text(TYPES + HARNESS)
        layout_object = tmp/'layout.o'
        subprocess.run(['gcc', '-m32', '-std=c89', '-pedantic', '-ffreestanding',
                        '-fno-builtin', '-c', str(harness), '-o', str(layout_object)],
                       check=True, capture_output=True, text=True)
        executable = tmp/'test'
        command = ['gcc', '-DWIDE_HOST', '-std=c89', '-pedantic', '-Wall', '-Wextra',
                   '-Wno-unused-parameter', '-O1', '-fno-builtin',
                   '-fsanitize=address,undefined', '-fno-omit-frame-pointer',
                   str(harness), *map(str, SOURCES), '-o', str(executable)]
        subprocess.run(command, check=True, capture_output=True, text=True)
        env = dict(os.environ, ASAN_OPTIONS='detect_leaks=0')
        result = subprocess.run([str(executable)], timeout=10, env=env)
        assert result.returncode == 0, 'host assertion failed (line modulo 256): %d' % result.returncode
    return {'result': 'PASS', 'source_units': 5, 'abi': 'i386 32-bit layout compile; native-width execution',
            'language': 'C89', 'tested': ['layout and preserved object bytes', 'queue arguments',
            'initializer fields and null guards', '7680 state/countdown/consume cases',
            'token search, selected-record exclusion, conflict avoidance and sentinel wrap'],
            'limitations': ['source semantics only; native targets not executed',
                            'kernel cannot execute i386; host uses native pointer width',
                            'LeakSanitizer disabled under ptrace; ASan and UBSan enabled']}


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
