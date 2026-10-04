#!/usr/bin/env python3
"""Read-only strict replay of two bounded C13 controller nonmatches."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
MATCHES = {}
NONMATCHES = {'80020820': 484, '80020DA8': 420}
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    inventory = {r['name']: r for r in json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']}
    results = []
    with tempfile.TemporaryDirectory(prefix='bt05-wrappers-') as tmp:
        for address, size in sorted(dict(MATCHES, **NONMATCHES).items()):
            name = 'func_' + address
            row = inventory[name]
            if row['scope'] != 'in_scope' or row['size'] != size or len(score.targets()[name]) * 4 != size:
                raise ValueError('claim/extent drift: ' + name)
            matched = address in MATCHES
            relative = ('cloud/matches/boot_tail/' if matched else 'cloud/work/boot_tail/C13-controller-pair/nonmatch/') + name + '.c'
            source = ROOT / relative
            data = source.read_bytes()
            if data.splitlines()[0] != ('/* flags: ' + FLAGS + ' */').encode():
                raise ValueError('source flag header drift: ' + name)
            flagsets = [FLAGS, FLAGS.replace('-O2', '-O1')]
            for flags in flagsets:
                obj = Path(tmp) / (name + '.o')
                score.compile_single(source, flags, obj)
                result = score.compare(obj, name, show=0)
                elf, sections = score._elf(obj)
                symbol = next(sym for idx, sec in enumerate(sections) if sec["type"] == 2
                              for sym in score._symbol_table(elf, sections, idx)
                              if sym["name"] == name)
                if matched and flags == FLAGS and symbol["size"] != size:
                    raise ValueError("ELF function extent differs from target: " + name)
                if source.read_bytes() != data:
                    raise ValueError('source changed during replay: ' + name)
                results.append({
                    'name': name, 'bytes': size, 'source_path': relative,
                    'source_sha256': hashlib.sha256(data).hexdigest(),
                    'flags': flags, 'effective_flags': flags + ' -Wab,-r4300_mul',
                    'differing_words': result.differing, 'total_words': result.total,
                    'extra_words': result.extra_words, 'unresolved': result.unresolved,
                    'unverified': result.unverified, 'errors': result.errors,
                    'strict_match': result.accepted(), 'elf_function_size': symbol['size'],
                })
                if matched and flags == FLAGS and not result.accepted():
                    raise ValueError(name + ': ' + result.summary())
    return {
        'schema_version': 1, 'result': 'COMPLETE_NONMATCH_RESEARCH',
        'base_commit': '9af97ad3c5e1968ccf5d3b7b7ec060fbb9d2f3c6',
        'match_functions': len(MATCHES), 'verified_body_bytes': sum(MATCHES.values()),
        'nonmatch_functions': len(NONMATCHES), 'nonmatch_bytes': sum(NONMATCHES.values()),
        'target_manifest_sha256': hashlib.sha256((score.ASM_DIR / 'SHA256SUMS').read_bytes()).hexdigest(),
        'results': results,
    }


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
