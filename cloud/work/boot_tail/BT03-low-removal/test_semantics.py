#!/usr/bin/env python3
"""Execute final C against a logical removal model, including guarded mutations."""
import argparse
import ctypes
import json
from pathlib import Path
import random
import subprocess
import tempfile

PACKET = Path(__file__).resolve().parent
CASES = [('800158D8', 'D_8003C610', 'D_8003C618', False),
         ('80015F28', 'D_80038608', 'D_80038610', False),
         ('8001661C', 'D_80042228', 'D_80042230', True)]
CAPACITY = 32


def run_case(address, count_name, table_name, uncounted, tmp):
    name = 'func_' + address
    source = PACKET / (name + '_NONMATCH.c')
    harness = tmp / (name + '_host.c')
    harness.write_text('#include "' + str(source) + '"\n' +
        'int ' + count_name + ';\nRecordStorage ' + table_name + '[32];\n' +
        'int entered, exited, enter_count = -1, exit_count;\n' +
        'void func_80014594(void) { entered++; if (enter_count >= 0) ' + count_name + ' = enter_count; }\n' +
        'void func_800145DC(void) { exited++; exit_count = ' + count_name + '; }\n' +
        'typedef char size_assert[(sizeof(RecordStorage) == 8) ? 1 : -1];\n')
    so = tmp / (name + '.so')
    subprocess.run(['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror',
                    '-Wno-error=pragmas', '-O2', '-fPIC', '-shared', str(harness),
                    '-o', str(so)], check=True)
    lib = ctypes.CDLL(str(so))
    if uncounted:
        fields = [('id', ctypes.c_uint16), ('metadata', ctypes.c_uint16),
                  ('payload', ctypes.c_uint32)]
    else:
        fields = [('payload', ctypes.c_uint32), ('id', ctypes.c_uint16),
                  ('references', ctypes.c_uint16)]
    class Fields(ctypes.Structure):
        _pack_ = 1
        _fields_ = fields
    class Record(ctypes.Union):
        _fields_ = [('fields', Fields), ('words', ctypes.c_uint32 * 2)]
    assert ctypes.sizeof(Fields) == ctypes.sizeof(Record) == 8
    assert Fields.id.offset == (0 if uncounted else 4)
    table = (Record * CAPACITY).in_dll(lib, table_name)
    count = ctypes.c_int.in_dll(lib, count_name)
    entered = ctypes.c_int.in_dll(lib, 'entered')
    exited = ctypes.c_int.in_dll(lib, 'exited')
    enter_count = ctypes.c_int.in_dll(lib, 'enter_count')
    exit_count = ctypes.c_int.in_dll(lib, 'exit_count')
    fn = getattr(lib, name)
    fn.argtypes = [ctypes.c_uint16]
    fn.restype = ctypes.c_int
    rng = random.Random(int(address, 16))
    cases = 0
    # Includes empty/missing, first/middle/last/duplicate ID, wrap and retained refcounts.
    for iteration in range(1500):
        n = iteration % 17
        ids = [rng.randrange(8) for _ in range(CAPACITY)]
        references = [rng.choice([0, 1, 2, 3, 65535]) for _ in range(CAPACITY)]
        for index, record in enumerate(table):
            record.fields.id = ids[index]
            record.fields.payload = rng.getrandbits(32)
            setattr(record.fields, 'metadata' if uncounted else 'references', references[index])
        target = rng.choice([0, 1, 7, 8, 65535] + ids[:n])
        count.value = n
        entered.value = exited.value = 0
        enter_count.value = -1
        # The third function searches before the guard and rereads count afterward.
        # Only extend, preserving the found index's validity and capacity.
        if uncounted and iteration % 5 == 0:
            enter_count.value = n + 1
        before = [bytes(record) for record in table]
        expected = list(before)
        index = next((i for i in range(n) if ids[i] == target), n)
        expected_count = n
        expected_enter = 1 if not uncounted or index != n else 0
        removed = False
        if index != n:
            if uncounted:
                if enter_count.value >= 0:
                    expected_count = enter_count.value
                removed = True
            else:
                changed = Record.from_buffer_copy(expected[index])
                changed.fields.references = (references[index] - 1) & 65535
                expected[index] = bytes(changed)
                removed = changed.fields.references == 0
            if removed:
                for i in range(index + 1, expected_count):
                    expected[i - 1] = expected[i]
                expected_count -= 1
        result = fn(target)
        assert result == int(removed), (name, iteration, 'return')
        assert count.value == expected_count, (name, iteration, 'count')
        assert entered.value == exited.value == expected_enter, (name, iteration, 'guards')
        if expected_enter:
            assert exit_count.value == expected_count, (name, iteration, 'release order')
        assert [bytes(record) for record in table] == expected, (name, iteration, 'storage')
        cases += 1
    return {'function': name, 'cases': cases, 'result': 'PASS', 'record_bytes': 8,
            'id_offset': Fields.id.offset, 'source': source.name}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='bt03-low-removal-host-') as temporary:
        rows = [run_case(*case, Path(temporary)) for case in CASES]
    report = {'result': 'PASS', 'cases': sum(row['cases'] for row in rows),
              'method': 'Final C compiled as C89 on host; deterministic whole-storage comparison against logical model.',
              'limitations': 'Valid bounded storage only; packed fields tested by logical value, not host byte order as a target substitute. No ROM or cartridge claim.',
              'results': rows}
    text = json.dumps(report, indent=2) + '\n'
    if args.output:
        args.output.write_text(text)
    else:
        print(text, end='')

if __name__ == '__main__':
    main()
