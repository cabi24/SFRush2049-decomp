#!/usr/bin/env python3
"""Fail-closed exact-pair research replay, preserving all other member failures."""
import contextlib
import dataclasses
import hashlib
import io
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


def describe(result):
    scrub = lambda text: re.sub(r'retail [0-9a-f]+, got [0-9a-f]+',
                               'compared byte values omitted', text)
    row = {**dataclasses.asdict(result), 'accepted': result.accepted(),
           'summary': scrub(result.summary()), 'notes': [scrub(t) for t in result.notes]}
    row['errors'] = [scrub(text) for text in row['errors']]
    return row


def measure(group, obj, names):
    score.compile_group(group, obj)
    with contextlib.redirect_stdout(io.StringIO()):
        return {n: describe(score.compare(obj, n, show=0)) for n in names}


def replace_one(directory, file, old, new):
    p = directory / file
    text = p.read_text()
    require(text.count(old) == 1, 'Stale negative: ' + file + ': ' + old)
    p.write_text(text.replace(old, new, 1))


def validate_inputs():
    pins = json.loads((HERE / 'inputs.json').read_text())
    group = HERE / 'group'
    spec = json.loads((group / 'group.json').read_text())
    require(spec['claims'] == [] and spec['context'] == [], 'Research scope changed')
    require(set(spec['files']) | {'group.json'} == set(pins['source_sha256']),
            'Source inventory changed')
    require(set(spec['members']) == set(pins['target_words_sha256']),
            'Target inventory changed')
    for name, expected in pins['source_sha256'].items():
        require(digest(group / name) == expected, 'Source fingerprint changed: ' + name)
    targets = score.targets()
    for name, expected in pins['target_words_sha256'].items():
        words = targets[name]
        actual = hashlib.sha256(struct.pack('>%dI' % len(words), *words)).hexdigest()
        require(actual == expected, 'Target fingerprint changed: ' + name)
    return spec


def main():
    spec = validate_inputs()
    expected = json.loads((HERE / 'verification.json').read_text())
    fidelity = json.loads((HERE / 'fidelity.json').read_text())
    require(fidelity['byte_identical'] and fidelity['new_accepted_credit'] == 0,
            'Research fidelity/scope receipt changed')
    with tempfile.TemporaryDirectory(prefix='videoscr-pair-proof-') as temp:
        root = Path(temp)
        base = root / 'base'
        shutil.copytree(HERE / 'group', base)
        obj = root / 'base.o'
        rows = measure(base, obj, spec['members'])
        require(rows == expected, 'Complete member receipt changed')
        require(digest(obj) == fidelity['candidate_object_sha256'], 'Stock object changed')
        for name, count in [('particle_render', 112), ('particle_collision', 253),
                            ('sign_extend_call', 10), ('func_800B8810', 4)]:
            require(rows[name]['accepted'] and rows[name]['total'] == count,
                    'Exact local function failed: ' + name)
        require(rows['func_8008E26C']['accepted'] and rows['physics_sym']['accepted'],
                'Real allocator/root failed')
        require(not rows['func_800B59F0']['accepted'] and rows['func_800B59F0']['errors'],
                'Inherited own-data refusal disappeared')
        output = {'members': rows, 'negatives': {}, 'claims': [], 'new_accepted_credit': 0}
        edits = {
            'remove_valid_local': ('particle_render', [('caller.c',
                's8 video_validOption(s32 i) { s8 valid; valid = D_8011AD40[i]; return valid; }',
                's8 video_validOption(s32 i) { return D_8011AD40[i]; }')]),
            'wrong_buffer_extent': ('particle_render', [('rect.c', 'u8 buf[40];', 'u8 buf[56];')]),
            'wrong_font': ('particle_render', [('rect.c', 'vr_font_set(11);', 'vr_font_set(10);')]),
            'wrong_rectangle_literal': ('particle_render', [('rect.c', '"%s  %s"', '"%s %s"')]),
            'remove_dlidx_local': ('particle_collision', [('caller.c',
                '    s16 dlidx;\n    dlidx = func_8008E26C(object, mat, parent, flags);\n    return dlidx;',
                '    return func_8008E26C(object, mat, parent, flags);')]),
            'flatten_globals_boundary': ('particle_collision', [('caller.c',
                '    func_800B8810();', '    D_80117428 = 1;')]),
            'wrong_initializer_literal': ('particle_collision', [('caller.c',
                'D_8011A994[9].pos[0] = -75.1f;', 'D_8011A994[9].pos[0] = -75.2f;')]),
            'wide_allocator_contract': ('particle_collision', [
                ('caller.c', 's16 func_8008E26C(s32, void *, s16, s32);',
                 's32 func_8008E26C(s32, void *, s16, s32);'),
                ('allocator.c', 's16 func_8008E26C(s32 a,', 's32 func_8008E26C(s32 a,')]),
        }
        for label, (target, replacements) in edits.items():
            child = root / label
            shutil.copytree(base, child)
            for file, old, new in replacements:
                replace_one(child, file, old, new)
            measured = measure(child, root / (label + '.o'), spec['members'])
            require(not measured[target]['accepted'], 'Mutation accepted: ' + label)
            require(measured['physics_sym']['accepted'], 'Mutation altered real root: ' + label)
            if 'literal' in label:
                require(measured[target]['errors'] and measured[target]['unverified'],
                        'Literal mutation escaped ownership proof: ' + label)
            if label == 'wide_allocator_contract':
                require(measured['func_8008E26C']['accepted'], 'Allocator body control changed')
                require(not measured['sign_extend_call']['accepted'], 'Wrapper negative escaped')
            output['negatives'][label] = measured[target]
        layout_controls = {
            'mbox_definition_before': (
                's16 sign_extend_call(s32 object, f32 (*mat)[3], s32 parent, u32 flags);',
                's16 sign_extend_call(s32 object, f32 (*mat)[3], s32 parent, u32 flags)\n'
                '{\n    s16 dlidx;\n    dlidx = func_8008E26C(object, mat, parent, flags);\n'
                '    return dlidx;\n}'),
            'globals_definition_before': (
                'void func_800B8810(void);',
                'void func_800B8810(void) { D_80117428 = 1; }'),
        }
        output['definition_order_controls'] = {}
        for label, (declaration, definition) in layout_controls.items():
            child = root / label
            shutil.copytree(base, child)
            replace_one(child, 'caller.c', definition, '')
            replace_one(child, 'caller.c', declaration, definition)
            measured = measure(child, root / (label + '.o'), spec['members'])
            require(measured == expected,
                    'Definition-order equivalence control changed member results: ' + label)
            output['definition_order_controls'][label] = measured['particle_collision']
        # Source-evidence control, not a wrong-source negative: MAT3 records
        # ordinary unsigned char. N64 volatility has not been authenticated.
        child = root / 'ordinary_model_count'
        shutil.copytree(base, child)
        for file in ['group.c', 'caller.c']:
            replace_one(child, file, 'volatile u8 gNumModels', 'u8 gNumModels')
        measured = measure(child, root / 'ordinary_model_count.o', spec['members'])
        ordinary = json.loads((HERE / 'ordinary_model_count.json').read_text())
        require(measured == ordinary, 'Ordinary model-count boundary changed')
        require(measured['particle_render']['accepted'] and measured['physics_sym']['accepted'],
                'Ordinary model-count control lost preserved rectangle/root')
        require(not measured['particle_collision']['accepted'],
                'Volatile dependency silently disappeared; review source boundary')
        output['source_evidence_controls'] = {'ordinary_model_count': measured}
        output['initializer_volatile_source_authenticated'] = False
        print(json.dumps(output, indent=2, sort_keys=True))


if __name__ == '__main__':
    main()
