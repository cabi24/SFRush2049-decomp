"""Source-bound complete match and bounded native/host regression checks."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import re
import pytest

ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_directional_sound_20261005'
spec=importlib.util.spec_from_file_location('directional_sound_verify',HERE/'verify.py')
verify=importlib.util.module_from_spec(spec);spec.loader.exec_module(verify)


def receipt(): return json.loads((HERE/'verification.json').read_text())


def test_receipt_source_bindings():
    stored=receipt()
    assert hashlib.sha256(verify.SOURCE.read_bytes()).hexdigest()==stored['source_sha256']
    for name,digest in stored['packet_sha256'].items():
        assert hashlib.sha256((HERE/name).read_bytes()).hexdigest()==digest
    for name,digest in stored['context_sources'].items():
        assert hashlib.sha256(verify.src(name).read_bytes()).hexdigest()==digest
    assert stored['accepted_byte_gain']==0
    assert stored['range']==['0x800FEA00','0x800FEC60']
    assert stored['object']['symbol_bytes']==608
    assert stored['gnu_link']['all_152_words_equal']
    assert all(verify.complete(x) for x in stored['genuine_context'].values())


def test_source_contract_and_no_shaping_operations():
    source=re.sub(r'/\*.*?\*/','',verify.SOURCE.read_text(),flags=re.S)
    assert not re.search(r'\b(?:volatile|asm|__asm__|M2C_ERROR)\b',source)
    assert 'GameCar *car = &player_array[slot];' in source
    assert source.count('side_squared *=')==1
    assert source.count('forward_squared *=')==1
    assert 'f32 output[3];' in source and 'f32 delta[3];' in source
    assert '(f32) D_8011F020[direction]' in source
    assert '(f32) D_8011F040[direction]' in source


def test_behavior_contract_is_explicit():
    evidence=receipt()['behavior']
    assert evidence['cases']==4578
    assert evidence['host_ubsan'] and evidence['real_native_matrix_callee']
    assert evidence['native_linked_access_traces_equal']
    assert len(evidence['rejected_mutants'])==6
    assert evidence['unexecuted_instruction_offsets']==[444]
    assert len(evidence['executed_instruction_offsets'])==151
    assert evidence['dynamic_coverage_equals_conservative_static_cfg']


@pytest.mark.parametrize('field,value',[
    ('differing',1),('extra_words',1),('symbol_bytes',604),('symbol_bytes',612),
    ('unresolved',['missing']),('unverified',['own literal']),('errors',['relocation']),
])
def test_acceptance_rejects_incomplete_proof(field,value):
    candidate=receipt()['object']
    candidate[field]=value
    assert not verify.complete(candidate)


def test_complete_compiler_and_behavior_replay(tmp_path):
    ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))
    if not (ido/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        if os.environ.get('REQUIRE_TOOLCHAIN')=='1': pytest.fail('required pinned toolchain missing')
        pytest.skip('pinned IDO and GNU MIPS tools required')
    actual=verify.verify(tmp_path)
    frozen=receipt()
    # Global manifest may grow after unrelated accepted splices. The current
    # manifest is validated on every native read; function/own data remain exact.
    actual.pop('target_manifest_sha256');frozen.pop('target_manifest_sha256')
    assert actual==frozen
