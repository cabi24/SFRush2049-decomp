#!/usr/bin/env python3
"""Pinned, source-only audio-screen research replay; no image claim."""
import dataclasses
import hashlib
import json
from pathlib import Path
import re
import shutil
import struct
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = next(p for p in HERE.parents if (p / 'tools/cloud/score.py').is_file())
sys.path.insert(0, str(ROOT))
from tools.cloud import score


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def scrub(text):
    return re.sub(r'retail [0-9a-f]+, got [0-9a-f]+',
                  'compared byte values omitted', text)


def describe(result):
    row = dataclasses.asdict(result)
    row.update(accepted=result.accepted(), summary=scrub(result.summary()),
               notes=[scrub(text) for text in result.notes])
    row['errors'] = [scrub(text) for text in row['errors']]
    return row


def verify_inputs(directory):
    group = directory / 'group'
    pins = json.loads((directory / 'inputs.json').read_text())
    spec = json.loads((group / 'group.json').read_text())
    require(spec['claims'] == [] and spec['context'] == [],
            'Research-only scope changed')
    require(len(spec['members']) == len(set(spec['members'])) == 48,
            'Complete native member inventory changed')
    require(set(spec['files']) | {'group.json'} == set(pins['source_sha256']),
            'Source inventory differs from pinned inputs')
    for name, expected in pins['source_sha256'].items():
        require(digest(group / name) == expected, 'Source fingerprint changed: ' + name)
    targets = score.targets()
    for name, expected in pins['target_words_sha256'].items():
        words = targets[name]
        fingerprint = hashlib.sha256(struct.pack('>%dI' % len(words), *words)).hexdigest()
        require(fingerprint == expected, 'Canonical target fingerprint changed: ' + name)
    require(set(spec['members']) == set(pins['target_words_sha256']),
            'Member inventory differs from pinned targets')
    return spec


def measure(group, obj, names):
    score.compile_group(group, obj)
    return {name: describe(score.compare(obj, name, show=0)) for name in names}


def frame_bytes(obj, name):
    word = score.text_words(obj)[score.symbols(obj)[name] // 4]
    require(word >> 26 == 9 and (word >> 21) & 31 == 29 and
            (word >> 16) & 31 == 29, 'Frame allocation shape changed')
    return 65536 - (word & 65535)


def emitted_inventory(obj, members):
    symbols = score.symbols(obj)
    targets = score.targets()
    require(set(symbols) & set(targets) == set(members),
            'Known native body escaped the explicit member inventory')
    text_size = len(score.text_words(obj)) * 4
    rows = {}
    for name, start in sorted(symbols.items()):
        end = min((offset for offset in symbols.values() if offset > start),
                  default=text_size)
        rows[name] = {'extent_bytes': end - start, 'native_member': name in members}
    return rows


def rectangle_proof(obj):
    """Accept only this fixed residual, after strict owned-data verification."""
    name = 'drone_collision_avoid'
    result = score.compare(obj, name, show=0)
    require(result.total == 213 and result.differing == 30 and
            not result.unresolved and not result.unverified and
            not result.errors and not result.extra_words,
            'Rectangle strict extent/content boundary changed')
    words = score.text_words(obj)
    symbols = score.symbols(obj)
    start = symbols[name]
    end = min((offset for offset in symbols.values() if offset > start),
              default=len(words) * 4)
    require(end - start == 852, 'Rectangle emitted extent changed')
    resolved, masks, unresolved, _, errors = score.relocate(
        obj, words, start, end, score.image_symbols())
    require(not unresolved and not errors, 'Rectangle link resolution changed')
    want = score.targets()[name]
    expected = {0: 24, 0x340: -24}
    expected.update(dict.fromkeys([0x5c, 0x78, 0xbc, 0x2d0, 0x2e0], -4))
    expected.update(dict.fromkeys([0x58, 0xa4, 0xd4, 0x2c4, 0x2e8], -24))
    expected.update(dict.fromkeys([
        0x44, 0x4c, 0x6c, 0x98, 0xd0, 0x104, 0x158, 0x178, 0x180,
        0x1a4, 0x1a8, 0x21c, 0x298, 0x2c0, 0x2d4, 0x2ec, 0x308, 0x338], -20))
    residual = {}
    signed = lambda value: (value & 65535) - (65536 if value & 32768 else 0)
    for index, target in enumerate(want):
        offset = index * 4
        got = resolved[start // 4 + index]
        mask = masks.get(start + offset, 0xffffffff)
        if (got & mask) == (target & mask):
            continue
        require(offset in expected and mask == 0xffffffff,
                'Rectangle difference outside the fixed stack residual')
        require(got >> 16 == target >> 16 and (got >> 21) & 31 == 29,
                'Rectangle opcode/register/base changed')
        delta = signed(got) - signed(target)
        require(delta == expected[offset], 'Rectangle stack immediate changed')
        residual['0x%03x' % offset] = delta
    require(set(residual) == {'0x%03x' % x for x in expected},
            'Rectangle residual-site inventory changed')
    require(frame_bytes(obj, name) == 120, 'Rectangle frame changed')
    return {'candidate_frame_bytes': 120, 'native_frame_bytes': 144,
            'extent_bytes': 852, 'differing_words': 30,
            'candidate_minus_native_stack_immediates': residual,
            'owned_literals_strictly_verified': True}


def run():
    spec = verify_inputs(HERE)
    expected = json.loads((HERE / 'verification.json').read_text())
    fidelity = json.loads((HERE / 'fidelity.json').read_text())
    with tempfile.TemporaryDirectory(prefix='audio-init-proof-') as temp:
        root = Path(temp)
        base = root / 'base'
        shutil.copytree(HERE / 'group', base)
        obj = root / 'base.o'
        rows = measure(base, obj, spec['members'])
        require(rows == expected, 'Fresh complete-group results changed')
        require(digest(obj) == fidelity['stock_object_sha256'], 'Whole stock object changed')
        inventory = emitted_inventory(obj, spec['members'])
        require(inventory == fidelity['emitted_functions'], 'Emitted function inventory changed')
        require(fidelity['whole_object_identical'] and
                fidelity['stock_object_sha256'] == fidelity['traced_object_sha256'],
                'Recorded compiler fidelity changed')
        result = rows['drone_pathfind_main']
        require(result['accepted'] and result['total'] == 281 and
                frame_bytes(obj, 'drone_pathfind_main') == 168,
                'Native initializer exact result changed')
        failures = [name for name in spec['members'] if not rows[name]['accepted']]
        require(failures == ['func_800D6914', 'func_800D6160',
                            'drone_collision_avoid', 'world_velocity_integrate',
                            'object_bytes23_sum', 'object_bytes_sum_global'],
                'Complete-group failures changed')
        results = {'members': rows, 'emitted_functions': inventory,
                   'rectangle_proof': rectangle_proof(obj), 'negatives': {},
                   'type_controls': {},
                   'claims': [], 'new_accepted_image_bytes': 0,
                   'new_aggregate_candidate_bytes': 0, 'complete_group_accepted': False}
        edits = {
            'wrong_button_count': ('init.c', 'buttonIdx < 12', 'buttonIdx < 11', 'init'),
            'wrong_state_mask': ('init.c', '0x7C03FFFE', '0x7C03FFFC', 'init'),
            'wrong_random_song': ('init.c', 'sync_entry_register(6,1)', 'sync_entry_register(7,1)', 'init'),
            'wrong_button_float': ('init.c', '-75.1f', '-74.1f', 'init'),
            'wrong_button_color': ('init.c', '{255, 255, 255, 255}', '{254, 255, 255, 255}', 'init'),
            'wrong_buffer_extent': ('rect.c', 'str[40]', 'str[48]', 'rect'),
            'wrong_font': ('rect.c', 'object_create(10)', 'object_create(11)', 'rect'),
            'wrong_literal': ('rect.c', '"10"', '"11"', 'rect'),
            'wrong_song_count': ('rect.c', 'songIdx<14', 'songIdx<13', 'rect'),
            'wrong_uppercase': ('rect.c', '*charPtr^0x20', '*charPtr^0x21', 'rect'),
        }
        for label, (filename, old, new, kind) in edits.items():
            child = root / label
            shutil.copytree(base, child)
            path = child / filename
            source = path.read_text()
            require(old in source, 'Stale negative control: ' + label)
            path.write_text(source.replace(old, new))
            changed_obj = root / (label + '.o')
            measured = measure(child, changed_obj, spec['members'])
            if kind == 'init':
                require(not measured['drone_pathfind_main']['accepted'],
                        'Mutation escaped strict initializer proof: ' + label)
            else:
                try:
                    rectangle_proof(changed_obj)
                except RuntimeError:
                    pass
                else:
                    raise RuntimeError('Mutation escaped rectangle content/residual proof: ' + label)
            results['negatives'][label] = measured
        child = root / 'legacy_volatile_model_count'
        shutil.copytree(base, child)
        for filename in ['init.c', 'func_800D7634.c']:
            path = child / filename
            source = path.read_text()
            require('extern u8 gNumModels' in source, 'Stale model-count type control')
            path.write_text(source.replace('extern u8 gNumModels',
                                           'extern volatile u8 gNumModels'))
        measured = measure(child, root / 'legacy_volatile.o', spec['members'])
        for name in spec['members']:
            if name != 'func_800D6914':
                require(measured[name] == rows[name],
                        'Model-count type control changed another body: ' + name)
        refused = measured['func_800D6914']
        require(not refused['accepted'] and refused['differing'] == 0 and
                len(refused['unverified']) == 12 and len(refused['errors']) == 1,
                'Legacy model-count ownership refusal changed')
        results['type_controls']['legacy_volatile_model_count'] = measured
        print(json.dumps(results, indent=2, sort_keys=True))


if __name__ == '__main__':
    run()
