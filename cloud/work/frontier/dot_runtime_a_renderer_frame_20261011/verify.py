#!/usr/bin/env python3
"""Input-bound two-member strict replay; owned-data context stays unclaimed."""
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
GROUP = HERE / 'groups/menu_renderers'
MEMBERS = ['func_803A6F2C', 'func_803A7620']
CONTEXT = ['func_803A7180', 'func_803A7918']
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


def without_slots(source):
    first = 'void func_803A6F2C(void)\n{\n    s32 residual_slot0, residual_slot1;\n'
    second = ('    s32 residual_slot0;\n    char status[20];\n    char title[40];\n'
              '    s32 residual_slot1, residual_slot2, residual_slot3;\n')
    require(source.count(first) == source.count(second) == 1, 'disclosed declaration forms changed')
    return source.replace(first, 'void func_803A6F2C(void)\n{\n', 1).replace(
        second, '    char status[20];\n    char title[40];\n', 1)


def inputs():
    with overlay_targets():
        manifest = score.target_manifest()
        native = score.targets()
        extents = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
        lookup = {item['name']: item for item in extents['functions']}
        spec = json.loads((GROUP / 'group.json').read_text())
        require(spec['members'] == spec['claims'] == MEMBERS, 'unexpected members/claims')
        require(spec['context'] == CONTEXT, 'genuine context set changed')
        require(spec['keep'] == ['func_803A7918'], 'private ABI keep set changed')
        targets = {}
        for name in MEMBERS + CONTEXT:
            words = native[name]
            require(len(words) * 4 == lookup[name]['size'], 'extent mismatch: ' + name)
            targets[name] = {'image': 'A', 'address': lookup[name]['address'],
                             'size': len(words) * 4,
                             'words_sha256': digest(struct.pack('>%dI' % len(words), *words))}
        paths = ['groups/menu_renderers/group.c', 'groups/menu_renderers/group.json',
                 'verify.py', 'trace_frame.py']
        return {'sources_sha256': {path: digest((HERE / path).read_bytes()) for path in paths},
                'target_metadata_sha256': {name: manifest[name] for name in ('extents.json', 'symbols.json')},
                'targets': targets, 'claims': MEMBERS, 'context': CONTEXT, 'flags': spec['flags']}


def replay():
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'configured compiler is not canonical IDO 5.3')
    with overlay_targets(), tempfile.TemporaryDirectory(prefix='renderer-frame-replay-') as tmp:
        obj = Path(tmp) / 'renderers.o'
        score.compile_group(GROUP, obj)
        results = {}
        for name in MEMBERS:
            value = score.compare(obj, name, show=0)
            require(value.accepted(), name + ': ' + value.summary())
            results[name] = asdict(value)
        raw, sections = score._elf(obj)
        symbols = [symbol for i, section in enumerate(sections) if section['type'] == 2
                   for symbol in score._symbol_table(raw, sections, i)]
        for name in MEMBERS + CONTEXT:
            symbol = next(symbol for symbol in symbols if symbol['name'] == name)
            require(symbol['size'] == binding['targets'][name]['size'], 'incomplete extent: ' + name)
        for name, expected in zip(CONTEXT, [2, 0]):
            value = score.compare(obj, name, show=0)
            require(value.differing == expected and not any((value.errors, value.unresolved,
                    value.extra_words)), 'context code residual changed: ' + name)
            # Never promote context because its code happens to compare equal.
            # The default scorer may lack 7918's switch-table data artifact;
            # a future authenticated artifact can verify it without rebinding.
            if name == 'func_803A7180':
                require(not value.unverified, '7180 unexpectedly owns data')
            else:
                require(len(value.unverified) in (0, 2), 'unexpected 7918 data references')
        control = Path(tmp) / 'control'
        control.mkdir()
        (control / 'group.c').write_text(without_slots((GROUP / 'group.c').read_text()))
        (control / 'group.json').write_bytes((GROUP / 'group.json').read_bytes())
        score.compile_group(control, Path(tmp) / 'control.o')
        words = score.text_words(Path(tmp) / 'control.o')
        starts = score.symbols(Path(tmp) / 'control.o')
        controls = {}
        for name, offsets, frame in [('func_803A6F2C', [12, 584], [320, 312]),
                                      ('func_803A7620', [0, 44, 48, 748], [144, 128])]:
            size = binding['targets'][name]['size']
            start = starts[name]
            got, masks, unresolved, unverified, errors = score.relocate(
                Path(tmp) / 'control.o', words, start, start + size, score.image_symbols())
            require(not any((masks, unresolved, unverified, errors)), 'control relocation uncertainty')
            want = score.targets()[name]
            differences = [4 * i for i, word in enumerate(want) if word != got[start // 4 + i]]
            require(differences == offsets, 'control residual moved: ' + name)
            for offset in offsets:
                require(want[offset // 4] >> 16 == got[(start + offset) // 4] >> 16,
                        'control changes more than a frame/home immediate')
            adjustments = [w for w in got[start // 4:(start + size) // 4] if w >> 16 == 0x27bd]
            require(adjustments[0] & 65535 == 65536 - frame[1], 'control frame changed')
            controls[name] = {'byte_offsets': offsets, 'native_frame': frame[0],
                               'without_slots_frame': frame[1]}
        return {'inputs': binding, 'results': results, 'frame_controls': controls,
                'new_claim_bytes': 1356, 'accepted_image_bytes': 0,
                'compiler': {'name': 'stock IDO 5.3', 'binary_sha256': {
                    name: digest(Path(score.ido(name)).read_bytes())
                    for name in ('cc', 'cfe', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1')}}}


def verify():
    receipt = json.loads((HERE / 'verification.json').read_text())
    require(inputs() == receipt['inputs'], 'receipt input binding changed')
    actual = replay()
    for key in ('inputs', 'results', 'frame_controls', 'new_claim_bytes', 'accepted_image_bytes'):
        require(actual[key] == receipt[key], 'receipt replay changed: ' + key)
    return actual


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--record', action='store_true')
    parser.add_argument('--inputs-only', action='store_true')
    args = parser.parse_args()
    if args.record:
        (HERE / 'verification.json').write_text(json.dumps(replay(), indent=2) + '\n')
        print('Recorded two strict renderer claims and frame controls')
    elif args.inputs_only:
        receipt = json.loads((HERE / 'verification.json').read_text())
        require(inputs() == receipt['inputs'], 'receipt input binding changed')
        print('PASS: source, verifier, target words and metadata bindings')
    else:
        verify()
        print('PASS: two strict members, 1356 new bytes; complete context retained and unclaimed')
