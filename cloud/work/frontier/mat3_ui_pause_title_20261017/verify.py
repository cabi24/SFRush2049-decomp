#!/usr/bin/env python3
"""Reproduce target equality without concealing incomplete caller closure."""
import hashlib
import json
from pathlib import Path
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


def verify():
    pins = json.loads((HERE / 'inputs.json').read_text())
    for name, digest in pins['sources'].items():
        require(hashlib.sha256((HERE / name).read_bytes()).hexdigest() == digest,
                'source changed: ' + name)
    targets = score.targets()
    for name, digest in pins['targets'].items():
        actual = hashlib.sha256(b''.join(struct.pack('>I', w) for w in targets[name])).hexdigest()
        require(actual == digest, 'target changed: ' + name)
    spec = json.loads((HERE / 'group/group.json').read_text())
    require(not spec['claims'] and not spec['context'], 'research must expose all members and claim none')
    report = {'new_accepted_bytes': 0, 'members': {}, 'negative_controls': {}}
    with tempfile.TemporaryDirectory(prefix='pause-title-proof-') as td:
        temp = Path(td)
        obj = temp / 'group.o'
        score.compile_group(HERE / 'group', obj)
        for name in spec['members']:
            result = score.compare(obj, name, show=0)
            report['members'][name] = result.summary()
        result = score.compare(obj, 'voice_stop', show=0)
        require(result.accepted(False), 'title ceased to match: ' + result.summary())
        require(result.total == 138, 'title extent changed')
        require(not score.compare(obj, 'func_800B4FB0', show=0).accepted(False),
                'caller now matches: review and update closure before claiming')
        for name, before, after, expected in [
            ('font12', 'header_font(13);', 'header_font(12);', 1),
            ('wrong_text_index', 'u16,112)', 'u16,114)', 2),
            ('helper_locations', ' } static', ' }\nstatic', 4),
        ]:
            group = temp / name
            shutil.copytree(HERE / 'group', group)
            source = group / 'voice.c'
            original = source.read_text()
            require(before in original, 'missing control site: ' + name)
            source.write_text(original.replace(before, after))
            score.compile_group(group, temp / (name + '.o'))
            result = score.compare(temp / (name + '.o'), 'voice_stop', show=0)
            require(result.differing == expected and not result.accepted(False)
                    and not result.extra_words and not result.unresolved
                    and not result.unverified and not result.errors,
                    'unexpected control result: ' + name + ': ' + result.summary())
            report['negative_controls'][name] = result.summary()
    return report


if __name__ == '__main__':
    print(json.dumps(verify(), indent=2, sort_keys=True))
