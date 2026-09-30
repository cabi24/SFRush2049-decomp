#!/usr/bin/env python3
"""Diagnostic pass replay, NEVER matching-C evidence or a splice submission.

Run on watchman2 with IDO_DIR set to the IDO 5.3 toolchain directory.
Only compiler scratch is changed; no retail target or committed object is edited.
"""
import argparse
import json
import os
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[3]


def reserved_record(data):
    """Find the expected integer Uregs record; ambiguity is a hard failure."""
    signature = struct.pack('>4I', 0x68000000, 1, 6, 2)
    offsets = [i for i in range(0, len(data) - 15, 4)
               if data[i:i + 16] == signature]
    if len(offsets) != 1:
        raise ValueError('expected exactly one integer Uregs(offset=2,length=6), found %d'
                         % len(offsets))
    return offsets[0]


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=Path(__file__).with_name('func_800CCE5C.c'))
    ap.add_argument('--json-out', type=Path)
    args = ap.parse_args()
    if os.sysconf('SC_PAGE_SIZE') != 4096:
        ap.error('run IDO on the documented 4 KB-page builder')
    sys.path.insert(0, str(ROOT / 'tools/cloud'))
    import score

    rows = []
    with tempfile.TemporaryDirectory(prefix='rush-ring-') as tmp:
        work = Path(tmp)
        source = work / 'probe.c'
        source.write_text(args.source.read_text())
        subprocess.run([score.ido('cc'), '-g0', '-O2', '-mips2', '-G', '0',
                        '-non_shared', score.R4300_CC, '-K', '-c', str(source)],
                       cwd=work, check=True, capture_output=True, text=True)
        original = (work / 'probe.O').read_bytes()
        record = reserved_record(original)
        for length in range(6, 13):
            data = bytearray(original)
            struct.pack_into('>I', data, record + 8, length)
            optimized = work / 'replay.O'
            optimized.write_bytes(data)
            obj = work / 'replay.o'
            common = ['-G', '0', '-mips2', '-EB', '-g0', '-O2']
            subprocess.run([score.ido('ugen'), *common, str(optimized), '-o', 'replay.G',
                            '-l', 'replay.s', '-t', 'probe.T', '-temp', 'replay.tmp'],
                           cwd=work, check=True, capture_output=True)
            subprocess.run([score.ido('as1'), '-elf', '-G', '0', '-p0', '-mips2',
                            score.R4300_AS1, '-EB', '-g0', '-O2', 'replay.G',
                            '-o', str(obj), '-t', 'probe.T'],
                           cwd=work, check=True, capture_output=True)
            result = score.compare(obj, 'func_800CCE5C')
            row = {'reserved_range_length': length, 'comparison': result.summary(),
                   'diagnostic_only': True}
            rows.append(row)
            print('DIAGNOSTIC ONLY:', length, row['comparison'])
    if args.json_out:
        args.json_out.write_text(json.dumps(rows, indent=2) + '\n')


if __name__ == '__main__':
    main()
