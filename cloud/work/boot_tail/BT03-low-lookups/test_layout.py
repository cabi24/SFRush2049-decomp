#!/usr/bin/env python3
"""Compile C89 layout assertions with the pinned IDO compiler, without target edits."""
import argparse
import hashlib
import json
from pathlib import Path
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score

CHECKS = {
    '80016C20': [('entry_size', 'sizeof(LookupEntry) == 8'),
                 ('entry_value', '(unsigned int)&((LookupEntry *)0)->value == 0'),
                 ('entry_key', '(unsigned int)&((LookupEntry *)0)->key == 4'),
                 ('group_size', 'sizeof(LookupGroup) == 4'),
                 ('group_first', '(unsigned int)&((LookupGroup *)0)->first == 2')],
    '80016F80': [('entry_prefix_size', 'sizeof(LookupEntry) == 8'),
                 ('entry_auxiliary', '(unsigned int)&((LookupEntry *)0)->auxiliary == 6')],
    '80017040': [('bank_size', 'sizeof(LookupBank) == 8'),
                 ('bank_count', '(unsigned int)&((LookupBank *)0)->count == 2'),
                 ('bank_entries', '(unsigned int)&((LookupBank *)0)->entries == 4')]
}


def run():
    rows = []
    with tempfile.TemporaryDirectory(prefix='bt03-low-lookups-layout-') as directory:
        tmp = Path(directory)
        for address, checks in CHECKS.items():
            source = PACKET / 'nonmatch' / ('func_' + address + '.c')
            text = source.read_text()
            text += '\n' + '\n'.join('typedef char check_%s[(%s) ? 1 : -1];' % item for item in checks) + '\n'
            translation = tmp / ('layout_' + address + '.c')
            translation.write_text(text)
            score.compile_single(translation, score.DEFAULT_FLAGS, tmp / ('layout_' + address + '.o'))
            rows.append({'function': 'func_' + address,
                         'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
                         'checks': [expr for label, expr in checks], 'result': 'PASS'})
    return {'schema_version': 1, 'result': 'PASS', 'compiler': 'pinned IDO 5.3',
            'flags': score.DEFAULT_FLAGS + ' -Wab,-r4300_mul', 'results': rows}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    text = json.dumps(run(), indent=2) + '\n'
    if args.output:
        args.output.write_text(text)
    else:
        print(text, end='')
