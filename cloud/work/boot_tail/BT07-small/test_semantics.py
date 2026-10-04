#!/usr/bin/env python3
"""Host C89 checks linked against the actual submitted translation units."""
from hashlib import sha256
from pathlib import Path
import json
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
ADDRESSES = ['80024FB0', '800250F0', '80025120', '80025150',
             '80025D84', '80026328', '80026348']
HARNESS = r'''
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct StreamState {
    unsigned char unknown_0000[4573];
    signed char state;
    signed char scale;
    unsigned char unknown_11DF[33];
    volatile unsigned char busy;
    unsigned char unknown_1201[39];
} StreamState;
typedef struct MessageQueue { unsigned char storage[24]; } MessageQueue;
unsigned char D_80038290;
StreamState D_80056230[2];
MessageQueue D_800586A8;
static int recv_count, jam_count, queue_result;
int osRecvMesg(MessageQueue *queue, void **message, int flags)
{
    assert(queue == &D_800586A8 && message == 0 && flags == 1);
    recv_count++;
    return queue_result;
}
int osJamMesg(MessageQueue *queue, void *message, int flags)
{
    assert(queue == &D_800586A8 && message == 0 && flags == 0);
    jam_count++;
    return queue_result;
}
void func_80024FB0(StreamState *);
int func_800250F0(void);
void func_80025120(int);
void func_80025150(void);
void func_80025D84(void);
void func_80026328(StreamState *);
void func_80026348(StreamState *);

int main(int argc, char **argv)
{
    StreamState stream, expected;
    int value, enabled, mode;
    assert(sizeof(StreamState) == 4648);
    assert(offsetof(StreamState, state) == 4573);
    assert(offsetof(StreamState, scale) == 4574);
    assert(offsetof(StreamState, busy) == 4608);
    mode = argc > 1 ? atoi(argv[1]) : 0;
    if (mode != 0) {
        D_80056230[mode - 1].busy = 1;
        func_80025D84();
        return 2; /* A permanently busy record must not return. */
    }
    for (value = -128; value < 128; value++) {
        for (enabled = 0; enabled < 2; enabled++) {
            memset(&stream, 0xA5, sizeof(stream));
            stream.scale = (signed char)value;
            memcpy(&expected, &stream, sizeof(stream));
            D_80038290 = enabled ? 255 : 0;
            if (enabled) expected.scale = (signed char)(value * 2);
            func_80024FB0(&stream);
            assert(memcmp(&stream, &expected, sizeof(stream)) == 0);
        }
        memset(&stream, 0x5A, sizeof(stream));
        stream.state = (signed char)value;
        memcpy(&expected, &stream, sizeof(stream));
        if (value == 2) expected.state = 3;
        func_80026328(&stream);
        assert(memcmp(&stream, &expected, sizeof(stream)) == 0);
        expected.state = 4;
        func_80026348(&stream);
        assert(memcmp(&stream, &expected, sizeof(stream)) == 0);
    }
    for (value = -1; value <= 1; value++) {
        queue_result = value;
        assert(func_800250F0() == 0);
        func_80025120(value);
        func_80025150();
    }
    assert(recv_count == 6 && jam_count == 3);
    D_80056230[0].busy = 0;
    D_80056230[1].busy = 0;
    func_80025D84();
    return 0;
}
'''


def run():
    sources = [ROOT / 'cloud/matches/boot_tail' / ('func_' + address + '.c')
               for address in ADDRESSES]
    with tempfile.TemporaryDirectory(prefix='bt07-host-') as directory:
        directory = Path(directory)
        harness = directory / 'harness.c'
        executable = directory / 'test'
        harness.write_text(HARNESS)
        command = ['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra',
                   '-Werror', '-Wno-unused-parameter', '-O2',
                   str(harness), *map(str, sources), '-o', str(executable)]
        subprocess.run(command, capture_output=True, text=True, check=True)
        subprocess.run([str(executable)], timeout=5, check=True)
        for mode in ('1', '2'):
            try:
                subprocess.run([str(executable), mode], timeout=0.25, check=True)
            except subprocess.TimeoutExpired:
                pass
            else:
                raise AssertionError('permanently busy record returned')
    return {'result': 'PASS', 'standard': 'C89', 'host_flags': command[1:7],
            'source_sha256': {str(p.relative_to(ROOT)): sha256(p.read_bytes()).hexdigest() for p in sources},
            'test_script_sha256': sha256(Path(__file__).read_bytes()).hexdigest(),
            'cases': {'scale_inputs': 512, 'state_inputs': 256,
                      'queue_receive_calls': 6, 'queue_jam_calls': 3,
                      'clear_scan_returns': 1, 'busy_record_timeout_checks': 2},
            'limitations': ['Host C behavior only; no native execution or ROM proof.',
                            'Busy timeout checks establish no early return during observation; concurrent clear transitions are not tested.']}


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
