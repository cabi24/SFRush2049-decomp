#!/usr/bin/env python3
"""Reproduce the rejected guarded-exit source under stock IDO, with full linking."""
from pathlib import Path
import hashlib
import importlib.util
import json
import os
import subprocess

ROOT = Path(__file__).resolve().parents[3]
HERE = Path(__file__).resolve().parent
BUILD = ROOT / 'build/texture_rect_schedule_constraints'
FLAGS = ['-g0', '-O3', '-mips2', '-G', '0', '-non_shared', '-Wab,-r4300_mul']
SPEC = importlib.util.spec_from_file_location(
    'rectangle_replay', ROOT / 'cloud/work/texture_rect_verification/replay.py')
REPLAY = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(REPLAY)
BASELINE = ROOT / 'cloud/work/frontier/w4a/func_80087110/best.c'
HISTORICAL_FALLBACK_HASH = 'ca9ec43290346368916e16d7cdec1ee967dde09b4cd8e00e2f59bcd2b0ca60c4'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def verify():
    ido = Path(os.environ.get('IDO_DIR', ROOT / 'tools/cloud/ido')).resolve()
    receipt = {
        'accepted': False,
        'research_only': True,
        'flags': FLAGS,
        'tool_sha256': {name: sha(ido / name) for name in ('cc', 'cfe', 'uopt', 'ugen', 'as1')},
        'controls': {},
    }
    for name, source, expected in [
            ('baseline', BASELINE, 4), ('guarded_exit', HERE / 'guarded_exit.c', 12)]:
        folder = BUILD / name
        folder.mkdir(parents=True, exist_ok=True)
        (folder / 'source.c').write_bytes(source.read_bytes())
        subprocess.run([str(ido / 'cc'), '-c', '-K', *FLAGS,
                        '-o', 'source.o', 'source.c'], cwd=folder, check=True,
                       capture_output=True)
        result = REPLAY.inspect(folder / 'source.o', folder)[0]
        assert result['elf_function_bytes'] == 1780
        assert result['project_scorer_differing_words'] == expected
        assert result['project_scorer_extra_words'] == 0
        assert result['gnu_linker_equals_project_relocator']
        assert not result['unresolved'] and not result['unverified']
        assert not result['relocation_errors']
        listing = (folder / 'u.out.s').read_text()
        # This is a read-only observation of stock compiler metadata, not an edit.
        after_exit = '\tb\t$49\n\t.alias\t$3,$sp\n$46:' in listing
        assert after_exit, 'Expected post-exit alias-close placement changed'
        receipt['controls'][name] = {
            'source': str(source.relative_to(ROOT)),
            'source_sha256': sha(source),
            'critical_alias_close_after_unconditional_exit': after_exit,
            'full_elf_verification': result,
        }
    guarded = receipt['controls']['guarded_exit']['full_elf_verification']
    assert guarded['linked_body_sha256'] == HISTORICAL_FALLBACK_HASH
    receipt['guarded_exit_equals_prior_rejected_fallback_dispatch'] = True
    (BUILD / 'verification.json').write_text(json.dumps(receipt, indent=2) + '\n')
    return receipt


if __name__ == '__main__':
    print(json.dumps(verify(), indent=2))
