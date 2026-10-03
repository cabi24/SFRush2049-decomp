#!/usr/bin/env python3
"""Reproduce repository-only HUD geometry and frozen-receipt audit.

Reads protected targets through the unchanged strict scorer. Outputs metadata,
never native instruction arrays. Optional replay builds ignored local objects.
"""
import argparse
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score

ARCHIVE = '433611408270ede7fe6acdb027f1b82179209511'
MARKER = 'cloud/work/large_hud_marker/'
TARGETS = ['func_80109A60', 'game_results_input', 'Input_ApplyPadConfig',
           'stat_race_update', 'input_new_data_wrapper']
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'


def git_blob(path):
    return subprocess.check_output(['git', 'show', ARCHIVE + ':' + path], cwd=ROOT)


def signed16(value):
    value &= 65535
    return value - 65536 if value & 32768 else value


def geometry(words, address):
    """Narrow instruction-field inspection, not a control-flow/liveness solver."""
    first = words[0]
    if first >> 26 != 9 or (first >> 21) & 31 != 29 or (first >> 16) & 31 != 29:
        raise ValueError('expected an addiu sp,sp prologue')
    frame = -signed16(first)
    calls, stack, saved = [], [], []
    for index, word in enumerate(words):
        op, base, reg = word >> 26, (word >> 21) & 31, (word >> 16) & 31
        if op == 3:
            calls.append({'offset': index * 4,
                          'callee': '0x%08X' % (((address + index * 4 + 4) & 0xf0000000) | ((word & 0x3ffffff) << 2))})
        # All integer/floating load/store forms present in these bodies.
        if op in (32, 33, 35, 36, 37, 40, 41, 43, 49, 53, 57, 61) and base == 29:
            stack.append({'offset': index * 4, 'operation': op,
                          'register': reg, 'slot': signed16(word)})
        # Only prologue saves, not later stores of ra repurposed as data.
        if index < 4 and op == 43 and base == 29 and (16 <= reg <= 23 or reg in (30, 31)):
            saved.append({'register': reg, 'slot': signed16(word)})
    return {'native_bytes': 4 * len(words),
            'target_sha256': hashlib.sha256(struct.pack('>%dI' % len(words), *words)).hexdigest(),
            'frame_bytes': frame, 'prologue_saves': saved,
            'calls': calls, 'stack_accesses': stack,
            'caller_home_accesses': [entry for entry in stack if entry['slot'] >= frame]}


def archive_audit():
    paths = subprocess.check_output(['git', 'ls-tree', '-r', '--name-only', ARCHIVE], cwd=ROOT).decode().splitlines()
    manifest = json.loads(git_blob('cloud/work/research_snapshot_20261002/manifest.json'))
    entries = {entry['path']: entry for entry in manifest['files']}
    selected = [path for path in paths if path.startswith(MARKER)]
    checked = []
    for path in selected:
        content = git_blob(path)
        digest = hashlib.sha256(content).hexdigest()
        entry = entries.get(path)
        checked.append({'path': path, 'sha256': digest, 'bytes': len(content),
                        'manifest_verified': bool(entry and entry['sha256'] == digest and entry['bytes'] == len(content))})
    missing = 'cloud/work/module_campaign_20261002/reconstruction/minimap_dots/'
    return {'commit': ARCHIVE, 'marker_files': checked,
            'minimap_packet_paths': [path for path in paths if path.startswith(missing)],
            'interrupted_receipt_status': 'unavailable in pinned published tree; not recovered'}


def replay(build):
    build.mkdir(parents=True, exist_ok=True)
    results = {}
    for name, source_path, receipt_path, grouped in [
        ('marker_O2', MARKER + 'reconstruction/baseline.c', MARKER + 'compiler/baseline_O2.json', False),
        ('marker_actual_input_O3', MARKER + 'compiler/actual_input_group/group.c', MARKER + 'compiler/actual_input_group.json', True),
    ]:
        source = git_blob(source_path)
        receipt = json.loads(git_blob(receipt_path))
        source_matches_receipt = hashlib.sha256(source).hexdigest() == receipt['source_sha256']
        directory = build / name
        directory.mkdir(exist_ok=True)
        candidate = directory / ('group.c' if grouped else 'baseline.c')
        candidate.write_bytes(source)
        obj = directory / 'candidate.o'
        if grouped:
            (directory / 'group.json').write_bytes(git_blob(MARKER + 'compiler/actual_input_group/group.json'))
            score.compile_group(directory, obj)
        else:
            score.compile_single(candidate, FLAGS, obj)
        result = score.compare(obj, 'game_results_input', show=0)
        words = score.text_words(obj)
        start = score.symbols(obj)['game_results_input'] // 4
        actual = {'source_sha256': hashlib.sha256(source).hexdigest(),
                  'archived_source_sha256': receipt['source_sha256'],
                  'source_matches_archived_receipt': source_matches_receipt,
                  'comparison': asdict(result), 'accepted': result.accepted(),
                  'frame_bytes': -signed16(words[start]),
                  'candidate_extent_bytes_including_padding': (len(words) - start) * 4,
                  'matches_archived_comparison': asdict(result) == receipt['comparison'],
                  'archived_emitted_words': receipt['emitted_words'],
                  'flags': receipt['flags'],
                  'object_sha256': hashlib.sha256(obj.read_bytes()).hexdigest(),
                  'tool_sha256': {tool: hashlib.sha256(Path(score.ido(tool)).read_bytes()).hexdigest()
                                  for tool in ('cc', 'cfe', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1')}}
        results[name] = actual
    return results


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--replay', action='store_true')
    args = parser.parse_args()
    targets = score.targets()
    symbols = score.image_symbols()
    report = {'status': 'NONMATCH research audit; no coverage', 'claims': [],
              'archive': archive_audit(),
              'native': {name: geometry(targets[name], symbols[name]) for name in TARGETS}}
    if args.replay:
        report['replay'] = replay(ROOT / 'build/d05_hud_context_audit')
    text = json.dumps(report, indent=2) + '\n'
    if args.output:
        args.output.write_text(text)
    else:
        print(text, end='')


if __name__ == '__main__':
    main()
