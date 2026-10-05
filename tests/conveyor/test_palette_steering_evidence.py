"""Fresh compiler checks for bounded palette/steering evidence and refusals.

Steering's successful replay reproduces an expected NONMATCH; it does not
accept steering as a match. Only the palette interpolator has a new claim.
"""
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

from tools.cloud import score


ROOT = Path(__file__).resolve().parents[2]
PALETTE = ROOT / 'cloud/work/ipa-groups/dot_palette_generation_20261005'
STEERING = ROOT / 'cloud/work/dot_steering_medium'
HAS_IDO = (score.IDO / 'cc').is_file()


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def test_receipts_are_source_bound_and_claim_scope_is_narrow():
    palette = json.loads((PALETTE / 'verification.json').read_text())
    spec = json.loads((PALETTE / 'group.json').read_text())
    assert palette['claims'] == spec['claims'] == ['func_800B0EA0']
    for name, digest in palette['source_sha256'].items():
        assert sha(PALETTE / name) == digest
    for row in palette['rejected_controls']:
        assert sha(PALETTE / row['source']) == row['source_sha256']
    steering = json.loads((STEERING / 'verification.json').read_text())
    assert steering['source_sha256'] == sha(STEERING / 'group.c')
    assert steering['status'] == 'NONMATCH'
    assert steering['claims'] == json.loads((STEERING / 'group.json').read_text())['claims'] == []


@pytest.mark.skipif(not HAS_IDO, reason='IDO compiler unavailable')
def test_fresh_palette_complete_extents_and_accepted_context():
    result = subprocess.run([sys.executable, str(PALETTE / 'verify.py')],
                            cwd=ROOT, check=True, capture_output=True, text=True)
    fresh = json.loads(result.stdout)
    saved = json.loads((PALETTE / 'verification.json').read_text())
    # Tool/protected-file hashes are provenance, not locks on future changes.
    for key in ('claims', 'native_layout_sizes_and_offsets', 'source_sha256',
                'maximum_formatted_name_bytes_including_null', 'results',
                'rejected_controls', 'unchanged_accepted_context_sources'):
        assert fresh[key] == saved[key]
    matching = [row for row in fresh['results'] if row['function'] != 'sound_bank_unload']
    assert len(matching) == 14  # one new helper and 13 unchanged accepted bodies
    for row in matching:
        assert row['exact_full_extent_bytes']
        assert row['elf_st_size'] == row['target_bytes']
        assert row['extent_to_next_symbol_or_text_end'] == row['target_bytes']
        assert row['fully_resolved_extent_sha256'] == row['target_sha256']
        assert not row['full_extent_unresolved']
        assert not row['full_extent_unverified']
        assert not row['full_extent_errors']
    caller = next(row for row in fresh['results'] if row['function'] == 'sound_bank_unload')
    assert not caller['claimed'] and not caller['exact_full_extent_bytes']
    assert caller['comparison']['differing'] == 262
    assert caller['comparison']['extra_words'] == 192
    assert caller['elf_st_size'] == 2048


@pytest.mark.skipif(not HAS_IDO, reason='IDO compiler unavailable')
def test_fresh_steering_expected_nonmatch_and_all_controls():
    subprocess.run([sys.executable, str(STEERING / 'replay.py'), '--all'],
                   cwd=ROOT, check=True, capture_output=True, text=True)
    fresh = json.loads((ROOT / 'build/dot_steering_medium/verification.json').read_text())
    saved = json.loads((STEERING / 'verification.json').read_text())
    for key in ('status', 'claims', 'source_sha256', 'comparison', 'function_bytes',
                'steering_frame_bytes', 'context_bodies_unchanged', 'controls_reproduced'):
        assert fresh[key] == saved[key]
    assert fresh['comparison']['steering_sensitivity']['differing'] == 106
    assert fresh['function_bytes']['steering_sensitivity'] == 928
    assert fresh['controls_reproduced'] == 20


@pytest.mark.skipif(not HAS_IDO, reason='IDO compiler unavailable')
@pytest.mark.parametrize('mutation', ['claim_nonmatching_parent', 'change_helper_alpha'])
def test_palette_claim_gate_rejects_real_negative_controls(tmp_path, mutation):
    spec = json.loads((PALETTE / 'group.json').read_text())
    for name in spec['files']:
        shutil.copyfile(PALETTE / name, tmp_path / name)
    if mutation == 'claim_nonmatching_parent':
        spec['context'].remove('sound_bank_unload')
        spec['members'].append('sound_bank_unload')
        spec['claims'].append('sound_bank_unload')
    else:
        helper = tmp_path / 'interpolate.c'
        source = helper.read_text()
        assert source.count('return red | green | blue | 1;') == 1
        helper.write_text(source.replace('return red | green | blue | 1;',
                                         'return red | green | blue | 0;'))
    (tmp_path / 'group.json').write_text(json.dumps(spec))
    result = subprocess.run([sys.executable, str(ROOT / 'tools/cloud/score.py'),
                             'group', str(tmp_path), '--claims'], cwd=ROOT,
                            capture_output=True, text=True)
    assert result.returncode == 1, result.stdout + result.stderr
    expected = '262/312 words differ' if mutation == 'claim_nonmatching_parent' else '4/48 words differ'
    assert expected in result.stdout, result.stdout + result.stderr
