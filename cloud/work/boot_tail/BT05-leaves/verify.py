#!/usr/bin/env python3
"""Strict read-only replay of the seven claimed BT05 macro-handler leaves."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

FUNCTIONS = {
    'func_80021F68': 28,
    'func_8002243C': 32,
    'func_800225AC': 48,
    'func_800225DC': 32,
    'func_80022A78': 32,
    'func_80023710': 36,
    'func_80023734': 32,
}
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())
    rows = {r['name']: r for r in inventory['functions']}
    targets = score.targets()
    results = []
    with tempfile.TemporaryDirectory(prefix='boot-tail-bt05-') as tmp:
        for name, size in FUNCTIONS.items():
            row = rows[name]
            if row['scope'] != 'in_scope' or row['size'] != size or row['callees_tail'] or row['callees_static']:
                raise ValueError('claim metadata drift: ' + name)
            if len(targets[name]) * 4 != size:
                raise ValueError('canonical extent drift: ' + name)
            source = ROOT / 'cloud/matches/boot_tail' / (name + '.c')
            data = source.read_bytes()
            if data.splitlines()[0] != ('/* flags: ' + FLAGS + ' */').encode():
                raise ValueError('source flag header drift: ' + name)
            obj = Path(tmp) / (name + '.o')
            score.compile_single(source, FLAGS, obj)
            result = score.compare(obj, name, show=0)
            if source.read_bytes() != data:
                raise ValueError('source changed during replay: ' + name)
            record = {
                'name': name, 'bytes': size,
                'source_sha256': hashlib.sha256(data).hexdigest(),
                'flags': FLAGS, 'effective_flags': FLAGS + ' -Wab,-r4300_mul',
                'differing_words': result.differing, 'total_words': result.total,
                'extra_words': result.extra_words, 'unresolved': result.unresolved,
                'unverified': result.unverified, 'errors': result.errors,
                'strict_match': result.accepted(),
            }
            results.append(record)
            if not result.accepted():
                raise ValueError(name + ': ' + result.summary())
    return {
        'schema_version': 1, 'result': 'PASS',
        'base_commit': '76780b3a1b3e26c54b86e1f153344e92dac15b20',
        'functions': len(results), 'verified_body_bytes': sum(FUNCTIONS.values()),
        'target_manifest_sha256': hashlib.sha256((score.ASM_DIR / 'SHA256SUMS').read_bytes()).hexdigest(),
        'results': results,
    }


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
