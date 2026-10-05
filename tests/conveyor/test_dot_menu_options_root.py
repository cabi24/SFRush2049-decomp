"""Regression checks for a deliberately native-gated menu reconstruction."""
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT/'cloud/work/dot_menu_options_root_20261005'


def test_receipt_is_bound_and_has_no_native_claim():
    proof = json.loads((PACKET/'verification.json').read_text())
    assert proof['claims'] == [] and proof['new_verified_function_bytes'] == 0
    for field in ['native_root_compiled','native_root_scored','native_group_match_claimed',
                  'native_literal_ownership_proved','native_elf_extent_proved',
                  'production_changes','fixture_capacity_is_native_evidence']:
        assert proof[field] is False
    assert proof['native_contracts']['storage']['capacity'] is None
    for name, expected in proof['source_sha256'].items():
        assert hashlib.sha256((PACKET/name).read_bytes()).hexdigest() == expected
    prior = ROOT/'cloud/work/ipa-groups/dot_menu_row_caller_20261005/group.c'
    assert hashlib.sha256(prior.read_bytes()).hexdigest() == proof['previous_menu_context_sha256']
    assert proof['host_root_cases'] == 410370 and proof['host_slot_cases'] == 24576


def test_native_extent_switch_callback_and_format():
    spec = importlib.util.spec_from_file_location('menu_options_verify', PACKET/'verify.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    actual = module.native_contracts()
    proof = json.loads((PACKET/'verification.json').read_text())
    assert actual == proof['native_contracts']


def test_source_is_genuine_control_flow_without_pressure_code():
    root = (PACKET/'root.c').read_text()
    assert 's32 func_8010AEAC(void *state)' in root
    assert root.count('func_8010A7A4(') == 14
    assert 'char header[MENU_OPTIONS_HEADER_CAPACITY]' in root
    assert not any(text in root for text in ['volatile','__inline','asm(','if (0)'])
    assert root.count('slot_state_setup(') == 2
    assert 'slot_state_setup(13)' in root and 'slot_state_setup(11)' in root
    assert 'D_8015698C + 1' in root and 'D_8017A4E0.labels[233]' in root


def test_root_compilation_fails_closed_without_fixture():
    cc = shutil.which('cc')
    if cc is None:
        pytest.skip('host C compiler unavailable')
    proc = subprocess.run([cc,'-std=c89','-fsyntax-only',str(PACKET/'root.c')], capture_output=True,text=True)
    assert proc.returncode != 0
    assert 'Native compilation is blocked on header capacity' in proc.stderr


@pytest.mark.parametrize('name',['host_root_test','host_slot_test'])
def test_source_behaviors(tmp_path, name):
    cc = shutil.which('cc')
    if cc is None:
        pytest.skip('host C compiler unavailable')
    binary = tmp_path/name
    subprocess.run([cc,'-std=c89','-Wall','-Wextra','-Werror','-O1',str(PACKET/(name+'.c')),
                    '-o',str(binary)],check=True)
    subprocess.run([str(binary)],check=True)


def test_toolchain_replays_bounded_semantic_verification(tmp_path):
    """Replay sanitizers and O32 checks; keep native root compilation gated."""
    from tools.cloud import score

    if not (score.IDO/'cc').is_file():
        pytest.skip('IDO compiler unavailable')
    for tool in ('cc', 'mips-linux-gnu-as'):
        if shutil.which(tool) is None:
            pytest.skip('C compiler or MIPS binutils unavailable: ' + tool)
    replay = tmp_path/'verification.json'
    subprocess.run([sys.executable, str(PACKET/'verify.py'), str(replay)],
                   cwd=ROOT, check=True, capture_output=True, text=True)
    assert replay.read_bytes() == (PACKET/'verification.json').read_bytes()
