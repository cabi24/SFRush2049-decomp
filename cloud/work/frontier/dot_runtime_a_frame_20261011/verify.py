#!/usr/bin/env python3
"""Input-bound strict group replay. No ROM or unmerged checkout is needed."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import struct
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
GROUP = HERE / 'groups/availability_root'
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers


def require(value, message):
    if not value:
        raise ValueError(message)


def digest(data):
    return hashlib.sha256(data).hexdigest()


@contextmanager
def overlay_targets():
    previous = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/ovl_a'
    try:
        yield
    finally:
        score.ASM_DIR = previous


def inputs():
    with overlay_targets():
        manifest = score.target_manifest()
        native = score.targets()
        extents = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
        lookup = {item['name']: item for item in extents['functions']}
        spec = json.loads((GROUP / 'group.json').read_text())
        require(spec['members'] == ['func_80392F28', 'func_80393004', 'func_803930B8',
                'func_803931C0', 'func_803932E4', 'func_80394058'], 'unexpected member set')
        require(spec['claims'] == ['func_80394058'], 'unexpected claim set')
        require(spec['context'] == ['func_8039338C', 'func_80393B3C', 'func_803923B0'],
                'genuine context set changed')
        require(spec['keep'] == ['func_80394058'], 'private ABI keep set changed')
        names = spec['members'] + spec['context']
        targets = {}
        for name in names:
            words = native[name]
            require(len(words) * 4 == lookup[name]['size'], 'extent mismatch: ' + name)
            targets[name] = {'image': 'A', 'address': lookup[name]['address'],
                             'size': len(words) * 4,
                             'words_sha256': digest(struct.pack('>%dI' % len(words), *words))}
        paths = ['groups/availability_root/group.c', 'groups/availability_root/group.json', 'verify.py', 'trace_frame.py']
        return {'sources_sha256': {path: digest((HERE / path).read_bytes()) for path in paths},
                'target_metadata_sha256': {name: manifest[name] for name in ('extents.json', 'symbols.json')},
                'targets': targets, 'claims': spec['claims'], 'members': spec['members'],
                'context': spec['context'], 'flags': spec['flags']}


def replay():
    binding = inputs()
    score.ido('cc')  # Shared conftest recognizes an actually missing tool.
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'configured compiler is not canonical IDO 5.3')
    with overlay_targets(), tempfile.TemporaryDirectory(prefix='root-frame-replay-') as tmp:
        obj = Path(tmp) / 'root.o'
        score.compile_group(GROUP, obj)
        results = {}
        for name in binding['members']:
            value = score.compare(obj, name, show=0)
            require(value.accepted(), name + ': ' + value.summary())
            results[name] = asdict(value)
        # The newly claimed root must be exactly one complete STT_FUNC body.
        raw, sections = score._elf(obj)
        symbols = [symbol for i, section in enumerate(sections) if section['type'] == 2
                   for symbol in score._symbol_table(raw, sections, i)]
        for name in binding['members']:
            symbol = next(symbol for symbol in symbols if symbol['name'] == name)
            require(symbol['size'] == binding['targets'][name]['size'],
                    'incomplete STT_FUNC extent: ' + name)
        # Disclosed control: removing only the two unused scalar declarations
        # restores precisely the old frame gap, with no other word differing.
        control = Path(tmp) / 'control'
        control.mkdir()
        source = (GROUP / 'group.c').read_text()
        declaration = '    s32 residual_slot0; s32 residual_slot1;\n'
        require(source.count(declaration) == 1, 'disclosed scalar declaration changed')
        (control / 'group.c').write_text(source.replace(declaration, '', 1))
        (control / 'group.json').write_bytes((GROUP / 'group.json').read_bytes())
        score.compile_group(control, Path(tmp) / 'control.o')
        value = score.compare(Path(tmp) / 'control.o', 'func_80394058', show=0)
        require(value.differing == 2 and value.total == 323 and not any((value.unresolved,
                value.unverified, value.errors, value.extra_words)), 'frame-only control changed')
        words = score.text_words(Path(tmp) / 'control.o')
        start = score.symbols(Path(tmp) / 'control.o')['func_80394058']
        resolved, masks, unresolved, unverified, errors = score.relocate(
            Path(tmp) / 'control.o', words, start, start + 1292, score.image_symbols())
        require(not any((masks, unresolved, unverified, errors)), 'control relocation uncertainty')
        expected = score.targets()['func_80394058']
        indices = [i for i, word in enumerate(expected) if word != resolved[start // 4 + i]]
        require(indices == [0, 322], 'control differs outside the two frame adjustments')
        require((expected[0] & 65535) == 65408 and (resolved[start // 4] & 65535) == 65416,
                'control frame sizes are not 128 and 120')
        return {'inputs': binding, 'results': results, 'frame_control': {'differing': 2,
                'total': 323, 'byte_offsets': [0, 1288], 'native_frame': 128, 'without_slots_frame': 120},
                'new_claim_bytes': 1292, 'accepted_image_bytes': 0,
                'compiler': {'name': 'stock IDO 5.3', 'binary_sha256': {
                    name: digest(Path(score.ido(name)).read_bytes())
                    for name in ('cc', 'cfe', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1')}}}


def verify():
    receipt = json.loads((HERE / 'verification.json').read_text())
    require(inputs() == receipt['inputs'], 'receipt input binding changed')
    actual = replay()
    for key in ('inputs', 'results', 'frame_control', 'new_claim_bytes', 'accepted_image_bytes'):
        require(actual[key] == receipt[key], 'receipt replay changed: ' + key)
    return actual


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--record', action='store_true')
    parser.add_argument('--inputs-only', action='store_true')
    args = parser.parse_args()
    if args.record:
        (HERE / 'verification.json').write_text(json.dumps(replay(), indent=2) + '\n')
        print('Recorded input-bound strict MATCH receipt')
    elif args.inputs_only:
        receipt = json.loads((HERE / 'verification.json').read_text())
        require(inputs() == receipt['inputs'], 'receipt input binding changed')
        print('PASS: source, verifier, target words and metadata bindings')
    else:
        verify()
        print('PASS: six strict members; new root 1292 bytes; 120 -> 128 frame control')
