"""The bounded heap-unlink improvement remains explicitly unaccepted."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import pytest
ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT/'cloud/work/frontier/dot_heap_unlink_20261005'
spec = importlib.util.spec_from_file_location('heap_unlink_proof', HERE/'verify.py')
packet = importlib.util.module_from_spec(spec); spec.loader.exec_module(packet)

def saved(): return json.loads((HERE/'verification.json').read_text())

def test_sources_and_native_are_bound():
    receipt = saved()
    assert receipt['status'] == 'NONMATCH' and receipt['claims'] == [] and receipt['accepted_byte_gain'] == 0
    for name, digest in receipt['source_sha256'].items():
        assert hashlib.sha256((HERE/name).read_bytes()).hexdigest() == digest
    for name, digest in receipt['context_source_sha256'].items():
        assert hashlib.sha256((ROOT/name).read_bytes()).hexdigest() == digest
    assert packet.sha(packet.packed(packet.score.targets()[packet.FN])) == receipt['native']['sha256']

def test_complete_residual_and_gnu_relocation_proof():
    receipt = saved(); proof = receipt['candidate']
    assert proof['elf_bytes'] == proof['native_bytes'] == 172
    assert proof['canonical']['differing'] == 4 and proof['canonical']['extra_words'] == 0
    assert proof['full_extent_differing_offsets'] == [92, 96, 116, 120]
    assert all(proof['canonical'][k] == [] for k in ['unresolved','unverified','errors'])
    assert proof['relocated_sha256'] == receipt['gnu_link']['body_sha256']
    assert len(receipt['gnu_link']['relocations']) == 11
    assert receipt['gnu_link']['excluded_zero_alignment_bytes'] == 4
    assert receipt['archive_a29']['elf_bytes'] == 164 and receipt['archive_a29']['canonical']['differing'] == 31

def test_twenty_accepted_context_bodies_stay_exact():
    context = saved()['accepted_context']; assert len(context) == 20
    for name, proof in context.items():
        assert proof['elf_bytes'] == proof['native_bytes']
        assert proof['canonical']['differing'] == proof['canonical']['extra_words'] == 0
        assert all(proof['canonical'][k] == [] for k in ['unresolved','unverified','errors'])
    assert context['car_damage_visual']['native_bytes'] == 680
    assert context['audio_reverb_update']['native_bytes'] == 244

def test_full_wrapper_instruction_coverage_and_mutation_controls():
    receipt = saved(); proof = receipt['behavior']
    assert proof['cases'] == 1153 and proof['native_candidate_gnu_executions'] == 3459
    assert proof['covered_words_each'] == [43,43,43]
    assert proof['total_words'] == 43 and proof['delay_slots_stack_canaries_and_saved_registers']
    assert receipt['host']['c89_ubsan_cases'] == 1153
    assert len(receipt['host']['mutants']) == 4
    assert 'hooks' in proof['external_callees']

def test_native_boundary_cases_and_decoder_fail_closed():
    native = packet.score.targets()[packet.FN]
    cases = packet.behavior.cases()
    for case in [cases[0], cases[15], cases[31], cases[127], cases[128], cases[-1]]:
        packet.behavior.execute(native,case)
    invalid = list(native); invalid[0] = 0xFC000000
    with pytest.raises(AssertionError,match='unsupported opcode'):
        packet.behavior.execute(invalid,cases[0])
    wrong = list(native); wrong[32] ^= 1
    with pytest.raises(AssertionError): packet.behavior.execute(wrong,cases[0])

def test_source_has_no_artificial_matching_operations():
    source = (HERE/'candidate.c').read_text()
    assert not any(x in source for x in ['volatile','__asm','__inline','pad[','standin'])
    assert 'chosen = func_800A51D8(heap);' in source
    assert source.count('osRecvMesg(') == source.count('osJamMesg(') == 1
    assert source.count('audio_reverb_update(') == 1

def test_fresh_compiler_native_host_replay(tmp_path):
    available = (packet.score.IDO/'cc').exists() and all(shutil.which(n) for n in ['cc','mips-linux-gnu-as','mips-linux-gnu-ld'])
    if not available:
        if os.environ.get('REQUIRE_TOOLCHAIN') == '1': pytest.fail('required compiler unavailable')
        pytest.skip('IDO/GNU MIPS/host toolchain unavailable')
    assert packet.verify(tmp_path) == saved()
