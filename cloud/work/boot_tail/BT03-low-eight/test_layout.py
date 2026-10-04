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
    '800148F8': [('slot_size', 'sizeof(AudioSlot) == 104'),
                 ('slot_value20', '(unsigned int)&((AudioSlot *)0)->value20 == 32'),
                 ('slot_value22', '(unsigned int)&((AudioSlot *)0)->value22 == 34'),
                 ('slot_value24', '(unsigned int)&((AudioSlot *)0)->value24 == 36'),
                 ('slot_release', '(unsigned int)&((AudioSlot *)0)->release_count == 40')],
    '800171C0': [('node_size', 'sizeof(AudioNode) == 24'),
                 ('node_next', '(unsigned int)&((AudioNode *)0)->next == 0'),
                 ('node_previous', '(unsigned int)&((AudioNode *)0)->previous == 4')],
    '8001729C': [('state_active', '(unsigned int)&((AudioState *)0)->active == 3960'),
                 ('state_pending', '(unsigned int)&((AudioState *)0)->pending == 3964')]
}


def run():
    rows = []
    with tempfile.TemporaryDirectory(prefix='bt03-low-eight-layout-') as directory:
        tmp = Path(directory)
        for address, checks in CHECKS.items():
            source = ROOT / 'cloud/matches/boot_tail' / ('func_' + address + '.c')
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
