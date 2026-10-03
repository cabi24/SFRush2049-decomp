#!/usr/bin/env python3
"""Replay three bounded donor-derived nonmatch controls without changing targets."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
FUNCTIONS = [('800223D0', 108, 'candidates/func_800223D0.c'),
    ('80022514', 108, 'candidates/func_80022514.c'),
    ('80023190', 140, 'candidates/func_80023190.c'),
    ('80023190', 140, 'controls/func_80023190_signed_byte_scale.c'),
    ('80023190', 140, 'controls/func_80023190_separate_product_shift.c')]
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'

def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    results = []
    with tempfile.TemporaryDirectory(prefix='bt05-source-led-') as tmp:
        for address, size, relative in FUNCTIONS:
            name = 'func_' + address
            source = ROOT / 'cloud/work/boot_tail/BT05-source-led' / relative
            data = source.read_bytes()
            if len(score.targets()[name]) * 4 != size:
                raise ValueError('canonical extent drift: ' + name)
            for flags in (FLAGS, FLAGS.replace('-O2', '-O1')):
                obj = Path(tmp) / (name + '.o')
                score.compile_single(source, flags, obj)
                r = score.compare(obj, name, show=0)
                if source.read_bytes() != data:
                    raise ValueError('source changed during replay: ' + name)
                results.append({'name': name, 'bytes': size,
                    'source_path': str(source.relative_to(ROOT)),
                    'source_sha256': hashlib.sha256(data).hexdigest(),
                    'flags': flags, 'effective_flags': flags + ' -Wab,-r4300_mul',
                    'differing_words': r.differing, 'total_words': r.total,
                    'extra_words': r.extra_words, 'unresolved': r.unresolved,
                    'unverified': r.unverified, 'errors': r.errors,
                    'strict_match': r.accepted()})
                if r.unresolved or r.unverified or r.errors:
                    raise ValueError('unverified candidate: ' + name)
    return {'schema_version': 1, 'result': 'COMPLETE_NONMATCH_RESEARCH',
        'base_commit': '301d9e7552ad4fd7f54a38796db84671e1000d35',
        'new_unique_targets': 0, 'new_matches': 0, 'new_verified_bytes': 0,
        'target_manifest_sha256': hashlib.sha256((score.ASM_DIR / 'SHA256SUMS').read_bytes()).hexdigest(),
        'results': results}

if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
