"""Sound force/peak reconstruction: source, extent, clock and behavior gates."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT/'cloud/work/frontier/dot_audio_force_20261005'
spec = importlib.util.spec_from_file_location('dot_audio_force_verify',PACKET/'verify.py')
verify = importlib.util.module_from_spec(spec)
spec.loader.exec_module(verify)


def receipt():
    return json.loads((PACKET/'verification.json').read_text())


def test_source_receipt_binding():
    for name,digest in receipt()['source_bindings'].items():
        assert hashlib.sha256(verify.context_path(name).read_bytes()).hexdigest() == digest


def test_full_match_is_not_coverage():
    r = receipt()
    assert r['status'] == 'MATCHING_RESEARCH' and r['coverage_credit'] == 0
    assert r['proof']['elf_bytes'] == r['gnu_link']['elf_bytes'] == 488
    assert r['proof']['comparison'] == dict(differing=0,total=122,unresolved=[],unverified=[],errors=[],extra_words=0)
    assert r['full_genuine_closure']['elf_bytes'] == 488
    assert r['full_genuine_closure']['comparison']['differing'] == 0
    assert r['gnu_link']['full_body_equal']
    assert r['gnu_link']['literal_bytes'] == 4
    assert r['gnu_link']['literal_address'] == '0x80124320'
    assert r['unclaimed_caller']['differing'] > 0
    assert r['unclaimed_caller']['unverified'] and r['unclaimed_caller']['errors']
    assert len(r['accepted_context']) == 5
    for result in r['accepted_context'].values():
        assert result['comparison']['differing'] == 0
        assert result['comparison']['unverified'] == []


def test_meaningful_negative_controls():
    r = receipt()
    assert r['controls']['ordinary_clock']['elf_bytes'] == 468
    assert r['controls']['player_first']['comparison']['differing'] == 1
    wrong = r['controls']['wrong_literal']['comparison']
    assert wrong['differing'] == 0 and wrong['errors'] and wrong['unverified']
    assert all(control['rejected'] for control in r['semantic_mutants'].values())
    assert len(r['semantic_mutants']) == 4


def test_behavior_scope_and_native_clock_contract():
    r = receipt()
    b = r['behavior']
    assert b['host_ubsan_cases'] == b['stable_clock_cases'] > 16000
    assert b['dynamic_clock_cases'] == 4
    assert b['native_runs'] == 2*(b['stable_clock_cases']+4)
    assert len(b['executed_instruction_offsets']) == 120
    assert b['unexecuted_instruction_offsets'] == [0xe0,0xec]
    assert r['clock_observation']['no_intervening_stores']
    assert r['clock_observation']['static_alias'] == '__osScElapsedTime'


def test_real_caller_preserves_both_calls():
    source = verify.caller()
    assert source.count('func_800DED78(') == 3  # declaration plus two actual calls
    assert 'func_800DED78(var_a3, (s32) m->player,' in source
    assert 'func_800DED78(4, (s32) m->player,' in source
    assert '__standin' not in source
    assert 'extern volatile f32 D_8002EB90;' in source
    assert 'extern f32 D_8002EB90' not in source
    assert 'extern f32 D_8002EB90' in verify.caller(False,False)
    candidate = (PACKET/'candidate.c').read_text()
    assert 'void func_800DED78(s16 index, s32 player, f32 vec[3], f32 threshold)' in candidate
    assert 'extern volatile f32 D_8002EB90;' in candidate
    assert 'pad[' not in candidate and '__asm' not in candidate


@pytest.mark.parametrize('size',[484,492,496])
def test_wrong_complete_extent_refused(monkeypatch,size):
    monkeypatch.setattr(verify,'extent',lambda obj,name:(0,size))
    monkeypatch.setattr(verify.score,'targets',lambda:{verify.NAME:[0]*122})
    class Exact:
        def accepted(self): return True
    monkeypatch.setattr(verify.score,'compare',lambda *args,**kwargs:Exact())
    with pytest.raises(AssertionError):
        verify.strict('unused.o')


def test_fresh_compiler_native_and_host_replay(tmp_path):
    if not (verify.score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        if os.environ.get('REQUIRE_TOOLCHAIN') == '1': pytest.fail('pinned toolchain missing')
        pytest.skip('pinned IDO and GNU MIPS toolchain required')
    assert verify.build_report(tmp_path) == receipt()


def test_generated_clock_declarations_are_consistent(tmp_path,monkeypatch):
    core = verify.full_core()
    assert 'extern volatile float D_8002EB90;' in core
    assert 'extern float D_8002EB90;' not in core
    assert 'extern float D_8002EB90;' in (verify.OLD/'audio_core.c').read_text()
    monkeypatch.setattr(verify.score,'compile_group',lambda *args: None)
    original = (PACKET/'candidate.c').read_text()
    for volatile in (False,True):
        source = original if volatile else original.replace('extern volatile f32 D_8002EB90;','extern f32 D_8002EB90;')
        directory = tmp_path/str(volatile)
        verify.group(directory,candidate=source)
        for name in ('candidate.c','caller.c'):
            generated = (directory/name).read_text()
            assert ('extern volatile f32 D_8002EB90;' in generated) == volatile
            assert ('extern f32 D_8002EB90' in generated) == (not volatile)
