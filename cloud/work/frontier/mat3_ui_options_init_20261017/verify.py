#!/usr/bin/env python3
"""Fail-closed options-init partial replay. Never claims image/ROM acceptance."""
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
    require(spec['claims'] == [], 'Research-only claims must stay empty')
    require(spec['context'] == [], 'Known emitted bodies must stay explicit members')
    require(set(spec['files']) | {'group.json'} == set(pins['source_sha256']),
            'Source inventory differs from pinned inputs')
    for name, expected in pins['source_sha256'].items():
        require(hashlib.sha256((group / name).read_bytes()).hexdigest() == expected,
                'Source fingerprint changed: ' + name)
    targets = score.targets()
    for name, expected in pins['target_words_sha256'].items():
        digest = hashlib.sha256(b''.join(struct.pack('>I', w) for w in targets[name])).hexdigest()
        require(digest == expected, 'Canonical target fingerprint changed: ' + name)
    require(set(spec['members']) == set(pins['target_words_sha256']),
            'Member inventory differs from pinned targets')
    return spec


def measure(group, obj, names):
    score.compile_group(group, obj)
    return {name: describe(score.compare(obj, name, show=0)) for name in names}


def run():
    spec = verify_inputs(HERE)
    expected = json.loads((HERE / 'verification.json').read_text())
    with tempfile.TemporaryDirectory(prefix='options-init-proof-') as temp:
        root = Path(temp)
        base = root / 'base'
        shutil.copytree(HERE / 'group', base)
        rows = measure(base, root / 'base.o', spec['members'])
        require(rows == expected, 'Fresh complete-group results changed')
        result = rows['replay_save_prompt']
        require(result['differing'] == 2 and result['total'] == 385,
                'Options init must retain the disclosed two-word partial')
        require(not result['accepted'] and not result['extra_words'] and
                not result['unresolved'] and not result['unverified'] and not result['errors'],
                'Partial extent or verification state changed')
        require(rows['func_800DB1E0']['accepted'], 'Genuine root lost equality')
        obj = root / 'base.o'
        words = score.text_words(obj)
        start = score.symbols(obj)['replay_save_prompt'] // 4
        target = score.targets()['replay_save_prompt']
        frame = 65536 - (words[start] & 65535)
        target_frame = 65536 - (target[0] & 65535)
        require((frame, target_frame) == (248, 256), 'Frame residual changed')
        end = (start + 385) * 4
        resolved, masks, _, _, _ = score.relocate(obj, words, start * 4, end, score.image_symbols())
        bad = [i for i, wanted in enumerate(target)
               if (resolved[start + i] & masks.get((start + i) * 4, 0xffffffff)) !=
                  (wanted & masks.get((start + i) * 4, 0xffffffff))]
        require(bad == [0, 384], 'Residual is no longer only allocation/release')
        results = {'members': rows, 'frame_bytes': {'candidate': frame, 'target': target_frame},
                   'negatives': {}, 'new_exact_bytes': 0, 'claims': []}
        edits = {
            'wrong_font': ('font_set(13);', 'font_set(12);'),
            'remove_recorded_dlidx': (
                '    s16 dlidx;\n    dlidx = func_8008E26C(obj, mat, parent, flags);\n    return dlidx;',
                '    return func_8008E26C(obj, mat, parent, flags);'),
            'wrong_owned_literal': ('-75.1f;', '-75.2f;'),
            'flatten_globals_boundary': ('    func_800B8810();', '    D_80117428 = 1;'),
        }
        for label, (old, new) in edits.items():
            child = root / label
            shutil.copytree(base, child)
            path = child / 'replay_caller.c'
            source = path.read_text()
            require(old in source, 'Stale negative control: ' + label)
            path.write_text(source.replace(old, new, 1))
            measured = measure(child, root / (label + '.o'), spec['members'])
            bad = measured['replay_save_prompt']
            require(not bad['accepted'], 'Mutation unexpectedly accepted: ' + label)
            require(bad != result, 'Mutation escaped current-source verification: ' + label)
            require(measured['func_800DB1E0']['accepted'], 'Negative altered genuine root: ' + label)
            if label == 'wrong_owned_literal':
                require(bool(bad['errors']) and bool(bad['unverified']),
                        'Owned literal mutation did not fail content proof')
            else:
                require(bad['differing'] > 2, 'Negative did not worsen partial: ' + label)
            results['negatives'][label] = bad
        # Original MAT3 type evidence, not a wrong-source negative. The native
        # version's qualifier remains unestablished; change both aliases.
        child = root / 'ordinary_model_count'
        shutil.copytree(base, child)
        for filename, old, new in [
                ('buttons.c', 'volatile u8 gNumModels', 'u8 gNumModels'),
                ('replay_caller.c', 'volatile u8 D_80140BDC', 'u8 D_80140BDC')]:
            path = child / filename
            source = path.read_text()
            require(source.count(old) == 1, 'Stale model-count type control')
            path.write_text(source.replace(old, new))
        measured = measure(child, root / 'ordinary_model_count.o', spec['members'])
        ordinary = json.loads((HERE / 'ordinary_model_count.json').read_text())
        require(measured == ordinary, 'Original-type complete receipt changed')
        for name in spec['members']:
            if name not in ['func_800DA2C0', 'replay_save_prompt']:
                require(measured[name] == rows[name], 'Original-type preserved member changed')
        require(measured['replay_save_prompt']['differing'] == 237 and
                not measured['replay_save_prompt']['accepted'],
                'Unresolved qualifier dependency changed; review source boundary')
        results['source_evidence_controls'] = {'ordinary_model_count': measured}
        results['initializer_volatile_source_authenticated'] = False
        print(json.dumps(results, indent=2, sort_keys=True))


if __name__ == '__main__':
    run()
