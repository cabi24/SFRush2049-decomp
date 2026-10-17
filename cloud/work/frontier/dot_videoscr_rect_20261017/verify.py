#!/usr/bin/env python3
"""Reproduce local true-zero/helper-negative research gates; no production edits."""
import contextlib
import dataclasses
import hashlib
import io
import json
import re
from pathlib import Path
import shutil
import sys
import tempfile
HERE = Path(__file__).resolve().parent
ROOT = next((p for p in HERE.parents if (p / 'tools/cloud/score.py').is_file()))
sys.path.insert(0, str(ROOT))
from tools.cloud import score

def require(condition, message):
    if not condition:
        raise RuntimeError(message)

def measure(directory):
    obj = directory / 'candidate.o'
    spec = score.compile_group(directory, obj)
    rows = {}
    for name in spec['members']:
        with contextlib.redirect_stdout(io.StringIO()):
            result = score.compare(obj, name)
        row = {**dataclasses.asdict(result), 'accepted': result.accepted(), 'summary': result.summary(), 'notes': list(result.notes)}
        scrub = lambda text: re.sub('retail [0-9a-f]+, got [0-9a-f]+', 'compared byte values omitted', text)
        row['summary'] = scrub(row['summary'])
        row['errors'] = [scrub(text) for text in row['errors']]
        rows[name] = row
    return rows

def main():
    spec = json.loads((HERE / 'group.json').read_text())
    require(spec['claims'] == [], 'Research packet must not declare claims')
    receipt = json.loads((HERE / 'verification.json').read_text())
    for name, digest in receipt['source_sha256'].items():
        require(hashlib.sha256((HERE / name).read_bytes()).hexdigest() == digest, name)
    with tempfile.TemporaryDirectory(prefix='videoscr-proof-') as temp:
        base = Path(temp) / 'base'
        base.mkdir()
        for name in spec['files'] + ['group.json']:
            shutil.copyfile(HERE / name, base / name)
        rows = measure(base)
        require(rows['particle_render']['accepted'], 'Research verification gate failed')
        require(rows['particle_render']['total'] == 112, 'Research verification gate failed')
        require(rows['particle_render']['notes'] == ['own .rodata verified at 0x8012267C..0x8012268C'], 'Research verification gate failed')
        require(rows['physics_sym']['accepted'], 'Research verification gate failed')
        require(not rows['particle_collision']['accepted'], 'Research verification gate failed')
        require(rows['particle_collision']['differing'] == 5, 'Research verification gate failed')
        require(not rows['particle_collision']['unverified'], 'Research verification gate failed')
        require(not rows['func_800B59F0']['accepted'], 'Research verification gate failed')
        require(rows['func_800B59F0']['errors'], 'Preserve inherited mixed-section refusal')
        results = {'base': rows, 'negatives': {}}
        edits = {'remove_recorded_valid_local': ('caller.c', 's8 video_validOption(s32 i) { s8 valid; valid = D_8011AD40[i]; return valid; }', 's8 video_validOption(s32 i) { return D_8011AD40[i]; }'), 'replace_authentic_buffer_extent': ('rect.c', 'char buf[40];', 'char buf[56];'), 'wrong_font': ('rect.c', 'vr_font_set(11);', 'vr_font_set(10);'), 'wrong_literal': ('rect.c', '"%s  %s"', '"%s %s"')}
        for label, (name, old, new) in edits.items():
            child = Path(temp) / label
            shutil.copytree(base, child)
            text = (child / name).read_text()
            require(text.count(old) == 1, (label, 'stale negative'))
            (child / name).write_text(text.replace(old, new))
            measured = measure(child)
            require(not measured['particle_render']['accepted'], label)
            require(measured['physics_sym']['accepted'], label)
            results['negatives'][label] = measured['particle_render']
        print(json.dumps(results, indent=2))
    return 0
if __name__ == '__main__':
    raise SystemExit(main())
