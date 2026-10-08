"""Portable source adaptation, native scope and complete TU invariants."""
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/boot_tail_promotion/voice_unblock_contract'
SPEC = importlib.util.spec_from_file_location('test_voice_unblock_contract_verifier', PACKET / 'verify.py')
VERIFY = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(VERIFY)


def receipt():
    return json.loads((PACKET / 'evidence.json').read_text())


def test_packet_binds_only_own_source_verifier_and_claims():
    hashes = receipt()['packet_sha256']
    assert hashes == VERIFY.packet_hashes()
    assert set(hashes) == {VERIFY.SOURCE, VERIFY.GROUP + '/group.json', 'verify.py', 'host_test.c'}
    assert receipt()['base'] == VERIFY.BASE
    assert receipt()['accepted_byte_gain'] == receipt()['new_matching_bytes'] == 0
    assert receipt()['previous_match_preserved_bytes'] == 124


def test_behavior_proof_source_tamper_is_rejected(tmp_path, monkeypatch):
    for filename in ('verify.py', 'host_test.c'):
        (tmp_path / filename).write_bytes((PACKET / filename).read_bytes())
    harness = tmp_path / 'host_test.c'
    harness.write_text(harness.read_text().replace('stage=0;func_8001F954(selected);',
                                                  'stage=0; /* skipped candidate call */'))
    assert harness.read_bytes() != (PACKET / 'host_test.c').read_bytes()
    monkeypatch.setattr(VERIFY, 'PACKET', tmp_path)
    with pytest.raises(AssertionError, match='proof-source binding drift'):
        VERIFY.check_packet_binding(receipt())


def test_source_reuses_exact_base_types_and_only_renames_known_fields():
    VERIFY.source_contract((ROOT / VERIFY.SOURCE).read_text(), VERIFY.git(VERIFY.TU).decode(),
                           VERIFY.git(VERIFY.LEGACY).decode())


@pytest.mark.parametrize('before,after', [
    ('u32 identifier60;', 'u32 identifier60[2];'),
    ('.identifier60 = index;', '.identifier60 = index + 1;'),
    ('.activeBD = 0;', '.unknownBE[0] = 0;'),
])
def test_wrong_type_or_body_contract_rejected(before, after):
    source = (ROOT / VERIFY.SOURCE).read_text()
    assert source.count(before) == 1
    with pytest.raises(AssertionError):
        VERIFY.source_contract(source.replace(before, after), VERIFY.git(VERIFY.TU).decode(),
                               VERIFY.git(VERIFY.LEGACY).decode())


def test_native_and_production_evidence_is_complete():
    proof = receipt()['proof']
    for row in list(proof['standalone'].values()) + [proof['O3_group']]:
        assert row['comparison'] == dict(differing=0, total=31, unresolved=[], unverified=[], errors=[], extra_words=0)
        assert row['elf_bytes'] == 124
        assert row['gnu_link']['linked_text_bytes'] == 128
        assert row['gnu_link']['alignment_zero_bytes'] == 4
        assert row['compiled_body_sha256'] == row['native_sha256'] == row['gnu_link']['body_sha256']
    commands = {command[0]: command[1:] for command in proof['O3_group']['backend_invocations']}
    assert set(commands) == {'cc', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1'}
    assert commands['cc'][:7] == ['-j', '-g0', '-O3', '-mips2', '-G', '0', '-non_shared']
    assert '-r4300_mul' in commands['as1']
    for tool in ('umerge', 'uopt', 'as1'):
        assert commands[tool][commands[tool].index('-Olimit') + 1] == '5000'
    whole = proof['whole_TU']
    assert len(whole['rows']) == 18
    assert len(whole['base_C_bodies']) == 14
    assert len(whole['unchanged_passthroughs']) == 3
    assert whole['membership_unchanged']
    assert all(row['comparison']['differing'] == 0 and row['owned_data_bytes'] == 0
               for row in whole['rows'].values())
    assert proof['native_layout']['VoiceState_bytes'] == 416
    assert proof['native_layout']['offsets']['identifier60'] == 96
    assert proof['native_layout']['offsets']['activeBD'] == 189
    assert set(proof['genuine_caller_helper']) == {'func_8001C7F4', 'func_80014AF0'}
    assert proof['native_behavior']['cases'] == 2112
    assert proof['native_behavior']['all_instruction_offsets_covered'] == list(range(0, 124, 4))
    assert proof['host_behavior'] == '2112 actual-source cases passed'
    assert len(proof['host_mutants_rejected']) == len(proof['expected_refusals']) + 1 == 3


def test_optimized_python_refuses():
    result = subprocess.run([sys.executable, '-O', str(PACKET / 'verify.py')], capture_output=True, text=True)
    assert result.returncode != 0 and 'assertions' in result.stderr


@pytest.mark.parametrize('missing', ['compiler', 'linker'])
def test_cli_missing_toolchain_skips_cleanly(tmp_path, missing):
    env = dict(os.environ)
    if missing == 'compiler':
        env['IDO_DIR'] = str(tmp_path / 'missing-ido')
    else:
        env['PATH'] = str(tmp_path / 'missing-bin')
    result = subprocess.run([sys.executable, str(PACKET / 'verify.py'), '--check'],
                            cwd=tmp_path, env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    assert json.loads(result.stdout) == {'status': 'SKIP', 'reason': 'pinned IDO and MIPS GNU linker required'}


def test_complete_replay_from_unrelated_directory(tmp_path):
    if not (VERIFY.score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    result = subprocess.run([sys.executable, str(PACKET / 'verify.py'), '--check'],
                            cwd=tmp_path, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    assert json.loads(result.stdout)['status'] == 'PASS'
