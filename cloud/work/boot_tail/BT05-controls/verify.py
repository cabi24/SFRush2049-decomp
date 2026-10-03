#!/usr/bin/env python3
"""Read-only strict replay of nine BT05 macro control-record wrappers."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

ADDRESSES = ('80023818', '80023844', '80023870', '8002389C', '800238C8',
             '800238F4', '80023920', '8002394C', '80023978')
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())
    extents = json.loads((score.ASM_DIR / 'extents.json').read_text())
    rows = {r['name']: r for r in inventory['functions']}
    expected = {r['address']: r['size'] for r in inventory['functions']}
    actual = {r['address']: r['size'] for r in extents['functions']}
    if actual != expected or len(actual) != 439 or sum(actual.values()) != 99120:
        raise ValueError('canonical census drift')
    targets = score.targets()
    results = []
    with tempfile.TemporaryDirectory(prefix='boot-tail-bt05-controls-') as tmp:
        for address in ADDRESSES:
            name = 'func_' + address
            row = rows[name]
            if (row['scope'] != 'in_scope' or row['size'] != 44
                    or row['callees_tail'] != ['0x80023754'] or row['callees_static']):
                raise ValueError('claim metadata drift: ' + name)
            if len(targets[name]) != 11:
                raise ValueError('target extent drift: ' + name)
            source = ROOT / 'cloud/matches/boot_tail' / (name + '.c')
            data = source.read_bytes()
            if data.splitlines()[0] != ('/* flags: ' + FLAGS + ' */').encode():
                raise ValueError('flag header drift: ' + name)
            obj = Path(tmp) / (name + '.o')
            score.compile_single(source, FLAGS, obj)
            result = score.compare(obj, name, show=0)
            if source.read_bytes() != data:
                raise ValueError('source changed during replay: ' + name)
            results.append({
                'name': name, 'bytes': 44,
                'source_sha256': hashlib.sha256(data).hexdigest(),
                'flags': FLAGS, 'effective_flags': FLAGS + ' -Wab,-r4300_mul',
                'differing_words': result.differing, 'total_words': result.total,
                'extra_words': result.extra_words, 'unresolved': result.unresolved,
                'unverified': result.unverified, 'errors': result.errors,
                'strict_match': result.accepted(),
            })
            if not result.accepted():
                raise ValueError(name + ': ' + result.summary())
    return {
        'schema_version': 1, 'result': 'PASS',
        'base_commit': '301d9e7552ad4fd7f54a38796db84671e1000d35',
        'functions': len(results), 'verified_body_bytes': len(results) * 44,
        'census': {'functions': 439, 'bytes': 99120, 'equal': True},
        'target_manifest_sha256': hashlib.sha256((score.ASM_DIR / 'SHA256SUMS').read_bytes()).hexdigest(),
        'results': results,
    }


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
