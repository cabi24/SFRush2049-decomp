#!/usr/bin/env python3
"""Fail-closed audio-cleanup research replay. No complete-group or image claim."""
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
               notes=list(result.notes))
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


def frame_bytes(obj, name):
    words = score.text_words(obj)
    start = score.symbols(obj)[name] // 4
    return 65536 - (words[start] & 65535)


def run():
    spec = verify_inputs(HERE)
    expected = json.loads((HERE / 'verification.json').read_text())
    with tempfile.TemporaryDirectory(prefix='audio-cleanup-proof-') as temp:
        root = Path(temp)
        base = root / 'base'
        shutil.copytree(HERE / 'group', base)
        obj = root / 'base.o'
        rows = measure(base, obj, spec['members'])
        require(rows == expected, 'Fresh complete-group results changed')
        result = rows['credits_screen']
        require(result['accepted'] and result['total'] == 74,
                'Native cleanup no longer matches its full 296-byte extent')
        frame = frame_bytes(obj, 'credits_screen')
        require(frame == 24, 'Native cleanup frame changed')
        require([n for n in spec['members'] if not rows[n]['accepted']] ==
                ['func_800D6914', 'func_800D6160'],
                'Complete-group boundary or inherited failures changed')
        require(not rows['func_800D6914']['differing'] and
                len(rows['func_800D6914']['unverified']) == 12 and
                len(rows['func_800D6914']['errors']) == 1,
                'Mixed-data ownership refusal changed')
        require(rows['func_800D6160']['differing'] == 65 and
                rows['func_800D6160']['total'] == 76,
                'Inherited mode-initializer failure changed')
        results = {'members': rows, 'frame_bytes': frame, 'negatives': {},
                   'exact_function_candidate_bytes': 296,
                   'new_accepted_image_bytes': 0, 'complete_group_accepted': False,
                   'claims': []}
        edits = {
            'wrong_button_count': ('buttonIdx < 12', 'buttonIdx < 11'),
            'wrong_audio_command': ('task->f2 = 1;', 'task->f2 = 2;'),
            'wrong_state_mask': ('state_word_a & 0x7C03FFFE', 'state_word_a & 0x7C03FFFC'),
            'wrong_button_field': ('D_801102B4[buttonIdx].handle', 'D_801102B4[buttonIdx].type'),
        }
        for label, (old, new) in edits.items():
            child = root / label
            shutil.copytree(base, child)
            path = child / 'cleanup.c'
            source = path.read_text()
            require(old in source, 'Stale negative control: ' + label)
            path.write_text(source.replace(old, new))
            measured = measure(child, root / (label + '.o'), spec['members'])
            require(not measured['credits_screen']['accepted'],
                    'Mutation escaped strict native cleanup proof: ' + label)
            results['negatives'][label] = measured
        print(json.dumps(results, indent=2, sort_keys=True))


if __name__ == '__main__':
    run()
