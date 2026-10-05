"""Model-clock research stays source-bound, complete and explicitly unaccepted."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import pytest
ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_model_clock_20261005'
spec=importlib.util.spec_from_file_location('model_clock_proof',HERE/'verify.py')
packet=importlib.util.module_from_spec(spec);spec.loader.exec_module(packet)

def saved():return json.loads((HERE/'verification.json').read_text())

def test_receipt_sources_and_target_are_bound():
    receipt=saved()
    assert receipt['status']=='NONMATCH' and receipt['claims']==[] and receipt['accepted_byte_gain']==0
    for name,digest in receipt['source_sha256'].items():
        assert hashlib.sha256((HERE/name).read_bytes()).hexdigest()==digest
    assert packet.native()==receipt['native']
    assert receipt['native']['direct_calls']==0 and receipt['native']['unsaved_callee_writes']==[]
    assert len(receipt['native']['direct_callers'])==2

def test_complete_extent_cannot_borrow_alignment():
    proof=saved()['candidate']
    assert proof['elf_bytes']==220 and proof['native_bytes']==228 and proof['zero_alignment_bytes']==4
    assert not proof['full_extent_equal'] and proof['own_sections']==[]
    assert proof['canonical']['differing']==47 and len(proof['full_extent_differing_offsets'])==47
    assert proof['full_extent_differing_offsets'][-2:]==[220,224]
    assert len(proof['relocations'])==10
    for key in ['unresolved','unverified','errors']:assert proof['canonical'][key]==[]
    assert saved()['controls']['baseline_O3']['elf_bytes']==232

def test_genuine_context_remains_whole_body_exact():
    receipt=saved()
    assert receipt['context'][packet.FN]['elf_bytes']==220
    for name in packet.CONTEXT:
        assert receipt['context'][name]['full_extent_equal']
        assert receipt['context'][name]['canonical']['differing']==0
        assert hashlib.sha256((ROOT/'src/blob'/(name+'.c')).read_bytes()).hexdigest()==receipt['context_source_sha256'][name]

def test_bounded_behavior_has_native_host_and_wrong_contract_controls():
    proof=saved()['semantics']
    assert proof['cases']==3494 and proof['native_covered_words']==55 and proof['native_total_words']==57
    assert proof['native_cfg_reachable_words']==55
    assert set(range(0,228,4))-set(proof['native_executed_instruction_offsets'])=={144,184}
    assert proof['host_ubsan']==proof['native_and_linked_access_order']==proof['o32_preservation']=='passed'
    assert len(proof['rejected_mutants'])==5
    assert 'defined' in proof['domain']

def test_source_has_no_artificial_matching_storage():
    source=(HERE/'candidate.c').read_text()
    assert all(term not in source for term in ['volatile','__asm','__standin','pad[','__inline'])
    assert 'u8 before_clock[1808]' in source and 'u8 after_clock[236]' in source
    assert source.count('extern ModelClockRecord')==1
    assert 'hypothetical_storage_owner' in saved()['controls']

def test_oracle_boundary_and_mutant_witnesses():
    receipt=saved()
    native=packet.semantics.Native(packet.score.targets()[packet.FN])
    witnesses=list(receipt['semantics']['rejected_mutants'].values())
    witnesses += [[0x80000000,0,packet.semantics.bits(.02)]+[0]*6]
    for case in witnesses:assert native.run(case)==packet.semantics.oracle(case)

def test_fresh_complete_compiler_and_semantic_replay(tmp_path):
    available=(packet.score.IDO / 'cc').exists() and all(shutil.which(n) for n in ['cc','mips-linux-gnu-ld','mips-linux-gnu-objcopy'])
    if not available:
        if os.environ.get('REQUIRE_TOOLCHAIN')=='1':pytest.fail('required compiler or GNU toolchain unavailable')
        pytest.skip('IDO/GNU MIPS/host toolchain unavailable')
    fresh=packet.verify(tmp_path);receipt=saved()
    for value in (fresh,receipt):value.pop('target_manifest_sha256')
    assert fresh==receipt
