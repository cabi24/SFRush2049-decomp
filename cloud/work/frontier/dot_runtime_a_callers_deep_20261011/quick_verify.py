#!/usr/bin/env python3
"""Small input-bound strict group gate; exhaustive research is opt-in.

This compiles an overlay IPA group with IDO 5.3, not a static ROM TU.
ROM-TU experiments must instead use tools.conveyor.compilers.tu_cc.
"""
import hashlib
import importlib.util
import json
from pathlib import Path
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]


def require(condition, message):
    if not condition:
        raise ValueError(message)


def digest(data):
    return hashlib.sha256(data).hexdigest()


def verify(inputs_only=False):
    receipt = json.loads((HERE / 'packet.json').read_text())
    require(receipt['protocol_version'] == 2, 'protocol drift')
    require(receipt['image'] == 'A', 'receipt image drift')
    require(receipt['compiler'] == 'ido5.3', 'compiler pin drift')
    require(set(receipt['input_sha256']) == {'group.c', 'group.json', 'verify.py', 'quick_verify.py'},
            'incomplete input bindings')
    require(set(receipt['target_manifest_rows']) == {'extents.json', 'symbols.json'},
            'incomplete target metadata bindings')
    require(set(receipt['target_words_sha256']) == set(receipt['members']), 'incomplete target word bindings')
    require(set(receipt['member_bytes']) == set(receipt['members']), 'incomplete member extent bindings')
    require(len(receipt['members']) == len(set(receipt['members'])), 'duplicate member')
    require(receipt['new_candidate_bytes'] == sum(receipt['member_bytes'][n] for n in receipt['claims']),
            'new-byte credit drift')
    for path, expected in receipt['input_sha256'].items():
        require(digest((HERE / path).read_bytes()) == expected, 'input drift: ' + path)
    config = json.loads((HERE / 'group.json').read_text())
    for key in ('members', 'claims', 'context', 'keep', 'flags'):
        require(config[key] == receipt[key], key + ' drift')
    require(config['claims'] and set(config['claims']) <= set(config['members']), 'empty/invalid claim set')
    require(not set(config['members']) & set(config['context']), 'context overlaps gated members')
    require(not config.get('allow_unverified'), 'unverified members forbidden')
    sys.path[:0] = [str(ROOT), str(ROOT / 'tools/cloud')]
    spec = importlib.util.spec_from_file_location(HERE.name + '_packet_score', ROOT / 'tools/cloud/score.py')
    score = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = score
    spec.loader.exec_module(score)
    score.ASM_DIR = ROOT / 'asm/us/ovl_a'
    manifest = score.target_manifest()
    for name, expected in receipt['target_manifest_rows'].items():
        require(manifest[name] == expected, 'target metadata drift: ' + name)
        score.verified_bytes(score.ASM_DIR / name, manifest)
    extents = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
    require((extents['image'], extents['base'], extents['rom_offset']) ==
            ('A', '0x8038A400', '0xB5C534'), 'wrong runtime image')
    targets = score.targets()
    for name, expected in receipt['target_words_sha256'].items():
        words = b''.join(word.to_bytes(4, 'big') for word in targets[name])
        require(digest(words) == expected, 'target words drift: ' + name)
        extent = next(row for row in extents['functions'] if row['name'] == name)
        require(extent['size'] == len(words) == receipt['member_bytes'][name], 'extent drift: ' + name)
    if inputs_only:
        return {'status': 'INPUTS PASS', 'compiled': False}
    # Preserve the shared conftest missing-tool skip/fail policy.
    ido_dir = Path(score.ido('cc')).parent
    from tools.conveyor.compilers import identify_dir
    require(identify_dir(ido_dir) == receipt['compiler'], 'wrong/unknown compiler')
    with tempfile.TemporaryDirectory(prefix='overlay-packet-') as temporary:
        obj = Path(temporary) / 'group.o'
        score.compile_group(HERE, obj)
        data, sections = score._elf(obj)
        text_index = score._text_index(sections)
        symtab = next(i for i, section in enumerate(sections) if section['type'] == 2)
        symbols = score._symbol_table(data, sections, symtab)
        results = {}
        for name in config['members']:
            symbol = next(row for row in symbols if row['name'] == name and row['section'] == text_index)
            require(symbol['size'] == receipt['member_bytes'][name], 'compiled full extent drift: ' + name)
            result = score.compare(obj, name, show=0)
            require(result.accepted(), name + ': ' + result.summary())
            results[name] = result.summary()
    return {'status': 'PASS', 'compiled': True, 'members': results,
            'new_candidate_bytes': receipt['new_candidate_bytes'],
            'accepted_image_bytes': 0, 'cartridge_bytes': 0}


if __name__ == '__main__':
    print(json.dumps(verify(inputs_only='--inputs-only' in sys.argv), indent=2))
