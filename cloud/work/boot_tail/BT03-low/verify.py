#!/usr/bin/env python3
"""Replay packet A with the unmodified strict scorer, storing no native words."""
from contextlib import redirect_stdout
from hashlib import sha256
from io import StringIO
from pathlib import Path
import json
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score

score.ASM_DIR = ROOT / 'asm/us/boot_tail'
ADDRESSES = ['80014624', '8001467C', '800146AC', '800149BC', '800149DC',
             '80014A04', '80014BB0', '80014C18', '80014C40', '80014D1C']
receipt = {'schema_version': 1, 'base_commit': '76780b3a1b3e26c54b86e1f153344e92dac15b20',
           'target_manifest_sha256': sha256((score.ASM_DIR / 'SHA256SUMS').read_bytes()).hexdigest(),
           'scorer_sha256': sha256((ROOT / 'tools/cloud/score.py').read_bytes()).hexdigest(),
           'results': []}
with tempfile.TemporaryDirectory(prefix='bt03-verify-') as directory:
    for address in ADDRESSES:
        name = 'func_' + address
        source = (PACKET if address == '8001467C' else ROOT / 'cloud/matches/boot_tail') / (name + '.c')
        row = {'name': name, 'bytes': len(score.targets()[name]) * 4,
               'source_path': str(source.relative_to(ROOT)),
               'source_sha256': sha256(source.read_bytes()).hexdigest(), 'trials': []}
        for level in (2, 1):
            flags = '-g0 -O%d -mips2 -G 0 -non_shared' % level
            obj = Path(directory) / (name + '-O%d.o' % level)
            score.compile_single(source, flags, obj)
            with redirect_stdout(StringIO()):
                comparison = score.compare(obj, name)
            strict = not (comparison.differing or comparison.extra_words or comparison.unresolved or comparison.unverified or comparison.errors)
            row['trials'].append({'flags': flags, 'effective_flags': flags + ' -Wab,-r4300_mul',
                                  'differing_words': comparison.differing, 'total_words': comparison.total,
                                  'extra_words': comparison.extra_words, 'unresolved': comparison.unresolved,
                                  'unverified': comparison.unverified, 'errors': comparison.errors,
                                  'strict_match': strict})
        receipt['results'].append(row)
        print(name + ': ' + ('MATCH' if row['trials'][0]['strict_match'] else 'NONMATCH'))
(PACKET / 'verification.json').write_text(json.dumps(receipt, indent=2) + '\n')
