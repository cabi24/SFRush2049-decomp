#!/usr/bin/env python3
"""Fresh strict compilation of frozen BT03-high-medium matches and bounded nonmatches."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
MATCHES = {'8001EAA0': 76, '8001EDF4': 64}
NONMATCHES = {'8001785C': 84, '80018AEC': 80, '80019A60': 72, '8001BDB8': 92}
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
            relative = ('cloud/matches/boot_tail/' if matched else 'cloud/work/boot_tail/BT03-high-medium/nonmatch/') + name + '.c'
            source = ROOT / relative
            data = source.read_bytes()
            if data.splitlines()[0] != ('/* flags: ' + BASE_FLAGS + ' */').encode():
                raise ValueError('source flag header drift: ' + name)
            flags = BASE_FLAGS
            controls = [BASE_FLAGS, BASE_FLAGS.replace('-O2', '-O1')]
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
                if matched and flagset == BASE_FLAGS and not result.accepted():
                    raise ValueError(name + ': ' + result.summary())
    return {'schema_version': 1, 'result': 'PASS',
            'base_commit': '301d9e7552ad4fd7f54a38796db84671e1000d35',
            'match_functions': len(MATCHES), 'verified_body_bytes': sum(MATCHES.values()),
            'nonmatch_functions': len(NONMATCHES), 'nonmatch_bytes': sum(NONMATCHES.values()),
            'target_manifest_sha256': hashlib.sha256((score.ASM_DIR / 'SHA256SUMS').read_bytes()).hexdigest(),
            'results': results}

if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
