#!/usr/bin/env python3
"""Fresh strict compilation of frozen BT03-high matches and bounded nonmatches."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
MATCHES = {'800175A8': 12, '80019AA8': 44, '8001C770': 12, '8001CCC0': 28}
NONMATCHES = {'800177EC': 56, '80017824': 56, '8001989C': 44,
              '8001C19C': 60, '8001C390': 60, '8001CC9C': 36}
BASE_FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'

def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    inventory = {r['name']: r for r in json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']}
    results = []
    with tempfile.TemporaryDirectory(prefix='bt03-high-') as temp:
        for address, size in sorted(dict(MATCHES, **NONMATCHES).items()):
            name = 'func_' + address
            row = inventory[name]
            if row['scope'] != 'in_scope' or row['size'] != size or len(score.targets()[name]) * 4 != size:
                raise ValueError('claim/extent drift: ' + name)
            matched = address in MATCHES
            relative = ('cloud/matches/boot_tail/' if matched else 'cloud/work/boot_tail/BT03-high/nonmatch/') + name + '.c'
            source = ROOT / relative
            data = source.read_bytes()
            flags = data.splitlines()[0].decode()[10:-3]
            controls = [flags] if matched else [BASE_FLAGS, BASE_FLAGS.replace('-O2', '-O1')]
            for flagset in controls:
                obj = Path(temp) / (name + '.o')
                score.compile_single(source, flagset, obj)
                result = score.compare(obj, name, show=0)
                if source.read_bytes() != data:
                    raise ValueError('source changed during replay: ' + name)
                rec = {'name': name, 'bytes': size, 'source_path': relative,
                       'source_sha256': hashlib.sha256(data).hexdigest(),
                       'flags': flagset, 'effective_flags': flagset + ' -Wab,-r4300_mul',
                       'differing_words': result.differing, 'total_words': result.total,
                       'extra_words': result.extra_words, 'unresolved': result.unresolved,
                       'unverified': result.unverified, 'errors': result.errors,
                       'strict_match': result.accepted()}
                results.append(rec)
                if matched and not result.accepted():
                    raise ValueError(name + ': ' + result.summary())
    return {'schema_version': 1, 'result': 'PASS',
            'base_commit': '76780b3a1b3e26c54b86e1f153344e92dac15b20',
            'match_functions': len(MATCHES), 'verified_body_bytes': sum(MATCHES.values()),
            'nonmatch_functions': len(NONMATCHES), 'nonmatch_bytes': sum(NONMATCHES.values()),
            'target_manifest_sha256': hashlib.sha256((score.ASM_DIR / 'SHA256SUMS').read_bytes()).hexdigest(),
            'results': results}

if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
