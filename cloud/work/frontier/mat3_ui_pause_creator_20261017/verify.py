#!/usr/bin/env python3
"""Fail-closed pause-creator partial replay. Never claims image/ROM acceptance."""
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


def register_residual(obj):
    words = score.text_words(obj)
    start = score.symbols(obj)['func_800B4FB0'] // 4
    target = score.targets()['func_800B4FB0']
    end = (start + len(target)) * 4
    resolved, masks, _, _, _ = score.relocate(obj, words, start * 4, end, score.image_symbols())
    bad = [i for i, wanted in enumerate(target)
           if (resolved[start + i] & masks.get((start + i) * 4, 0xffffffff)) !=
              (wanted & masks.get((start + i) * 4, 0xffffffff))]
    fields = [(72, 11), (75, 16), (85, 21), (88, 11),
              (89, 16), (93, 21), (94, 16), (95, 21)]
    pure = bad == [i for i, _ in fields]
    for i, shift in fields:
        got, wanted = resolved[start+i], target[i]
        field = 31 << shift
        pure = pure and (got & ~field) == (wanted & ~field)
        pure = pure and {(got >> shift) & 31, (wanted >> shift) & 31} == {17, 18}
    return {'word_offsets': bad, 'only_s1_s2_swap': bool(pure),
            'candidate_frame_bytes': 65536 - (words[start] & 65535),
            'target_frame_bytes': 65536 - (target[0] & 65535)}


def run():
    spec = verify_inputs(HERE)
    expected = json.loads((HERE / 'verification.json').read_text())
    with tempfile.TemporaryDirectory(prefix='pause-creator-proof-') as temp:
        root = Path(temp)
        base = root / 'base'
        shutil.copytree(HERE / 'group', base)
        obj = root / 'base.o'
        rows = measure(base, obj, spec['members'])
        require(rows == expected, 'Fresh complete-group results changed')
        result = rows['func_800B4FB0']
        require(result['differing'] == 8 and result['total'] == 368,
                'Pause creator no longer has its disclosed eight-word residual')
        require(not result['accepted'] and not result['extra_words'] and
                not result['unresolved'] and not result['unverified'] and not result['errors'],
                'Partial extent or verification state changed')
        residual = register_residual(obj)
        require(residual['only_s1_s2_swap'], 'Residual is not the disclosed register swap')
        frame, target_frame = residual['candidate_frame_bytes'], residual['target_frame_bytes']
        require((frame, target_frame) == (88, 88), 'Native frame equality changed')
        results = {'members': rows, 'frame_bytes': {'candidate': frame, 'target': target_frame},
                   'residual': residual, 'negatives': {},
                   'new_exact_bytes': 0, 'claims': []}
        edits = {
            'wrong_menu_count': ('for (i=0;i<8;i++)', 'for (i=0;i<7;i++)'),
            'wrong_text_table': ('countdown_state.tab[56]', 'countdown_state.tab[57]'),
            'wrong_player_center': ('D_80117100[0][1].cx', 'D_80117100[0][1].w'),
            'wrong_title_font': ('pause_font(13);', 'pause_font(12);'),
        }
        for label, (old, new) in edits.items():
            child = root / label
            shutil.copytree(base, child)
            path = child / 'candidate.c'
            source = path.read_text()
            require(old in source, 'Stale negative control: ' + label)
            path.write_text(source.replace(old, new, 1))
            measured = measure(child, root / (label + '.o'), spec['members'])
            changed = measured['func_800B4FB0']
            changed_residual = register_residual(root / (label + '.o'))
            require(not changed['accepted'] and not changed_residual['only_s1_s2_swap'],
                    'Mutation escaped the residual-content contract: ' + label)
            results['negatives'][label] = {'comparison': changed, 'residual': changed_residual}
        print(json.dumps(results, indent=2, sort_keys=True))


if __name__ == '__main__':
    run()
