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
    '80011F60': [('pointer_width', 'sizeof(void *) == 4'),
                 ('word_width', 'sizeof(unsigned int) == 4'),
                 ('node_size', 'sizeof(AudioBufferNode) == 20'),
                 ('node_next', '(unsigned int)&((AudioBufferNode *)0)->next == 0'),
                 ('node_previous', '(unsigned int)&((AudioBufferNode *)0)->previous == 4'),
                 ('node_buffer', '(unsigned int)&((AudioBufferNode *)0)->buffer == 8')],
    '800123A8': [('pointer_width', 'sizeof(void *) == 4'),
                 ('word_width', 'sizeof(unsigned int) == 4'),
                 ('halfword_width', 'sizeof(unsigned short) == 2'),
                 ('node_size', 'sizeof(AudioCacheNode) == 24'),
                 ('node_next', '(unsigned int)&((AudioCacheNode *)0)->next == 0'),
                 ('node_previous', '(unsigned int)&((AudioCacheNode *)0)->previous == 4'),
                 ('node_buffer', '(unsigned int)&((AudioCacheNode *)0)->buffer == 20')]
}


def run():
    rows = []
    with tempfile.TemporaryDirectory(prefix='bt02-audio-pair-layout-') as directory:
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
