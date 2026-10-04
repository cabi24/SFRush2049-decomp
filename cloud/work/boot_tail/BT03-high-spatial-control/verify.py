#!/usr/bin/env python3
"""Fresh strict compilation of frozen BT03-high-spatial-control matches and bounded nonmatches."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
MATCHES = {'80019C8C':580}
NONMATCHES = {'8001D1F4':508,'8001DC08':472}
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
            relative = ('cloud/matches/boot_tail/' if matched else 'cloud/work/boot_tail/BT03-high-spatial-control/nonmatch/') + name + '.c'
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
                elf, sections = score._elf(obj)
                symbols = [sym for idx, sec in enumerate(sections) if sec['type'] == 2
                           for sym in score._symbol_table(elf, sections, idx)
                           if sym['type'] == 2 and sym['section'] == score._text_index(sections)]
                if len(symbols) != 1 or symbols[0]['name'] != name or symbols[0]['value'] != 0:
                    raise ValueError('ELF function identity drift: ' + name)
                symbol = symbols[0]
                if result.accepted():
                    words = score.text_words(obj)
                    target = score.targets()[name]
                    relocated, masks, unresolved, unverified, errors = score.relocate(obj, words, 0, len(words)*4, score.image_symbols())
                    if symbol['size'] != size or relocated[:len(target)] != target or masks or unresolved or unverified or errors or any(relocated[len(target):]):
                        raise ValueError('exact extent/full relocated equality failure: ' + name)
                if source.read_bytes() != data:
                    raise ValueError('source changed during replay: ' + name)
                rec = {'name': name, 'bytes': size, 'source_path': relative,
                       'source_sha256': hashlib.sha256(data).hexdigest(),
                       'flags': flagset, 'effective_flags': flagset + ' -Wab,-r4300_mul',
                       'differing_words': result.differing, 'total_words': result.total,
                       'extra_words': result.extra_words, 'unresolved': result.unresolved,
                       'unverified': result.unverified, 'errors': result.errors,
                       'strict_match': result.accepted(), 'elf_function_size': symbol['size']}
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
