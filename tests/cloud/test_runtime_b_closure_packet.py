"""Portable genuine-closure research gates; never a matching admission."""
import copy
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest
from tools.cloud import score

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/runtime_b_closure_20261006'


def verifier():
    spec = importlib.util.spec_from_file_location('runtime_b_closure_packet', PACKET / 'verify.py')
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def test_source_harness_and_original_evidence_bindings():
    verifier().bindings()


def test_complete_source_preserving_assembly():
    verifier().assembly()


def test_all_eight_current_native_targets(monkeypatch):
    monkeypatch.setattr(score, 'ASM_DIR', ROOT / 'asm/us/ovl_b')
    verifier().targets(score)


def test_research_claims_stay_bounded():
    final = json.loads((PACKET / 'baseline/final.json').read_text())
    assert final['D328']['bytes'] == 124
    assert final['D328']['unmasked_differences'] == 0
    assert final['D328']['canonical_name_based_scorer_admission'] is False
    assert final['D328']['accepted_coverage_bytes'] == 0
    assert final['behavior']['paired_cases'] == 552
    assert final['behavior']['root_invocations_per_image'] == 856
    assert final['behavior']['negative_controls'] == 10
    assert final['behavior']['private_children_hooked'] is False
    bounds = json.loads((PACKET / 'baseline/boundaries.json').read_text())
    assert len(bounds['functions']) == 8
    assert sum(x['full_unmasked_research_placement']['status'] == 'NONMATCH'
               for x in bounds['functions'].values()) == 7


def test_complete_genuine_closure_replay(tmp_path):
    if not (score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    reference = Path(os.environ.get('RUSH_REFERENCE_ROOT', str(ROOT)))
    result = subprocess.run([sys.executable, str(PACKET / 'verify.py'),
                             '--reference-root', str(reference),
                             '--output', str(tmp_path / 'closure-replay.json')],
                            text=True, capture_output=True, cwd=tmp_path)
    assert result.returncode == 0, result.stdout + result.stderr


def test_replay_comparison_keeps_proven_evidence():
    import copy
    check = verifier()
    original = check.read('build.json')
    fresh = copy.deepcopy(original)
    fresh['object']['sha256'] = 'different debug metadata'
    fresh['linked']['sha256'] = 'different ELF symbol metadata'
    fresh['object_local_path'] = '/different/invocation/closure.o'
    assert check.compiled_proof(fresh) == check.compiled_proof(original)
    fresh['object']['allocated'][0]['sha256'] = 'different allocated bytes'
    assert check.compiled_proof(fresh) != check.compiled_proof(original)
    original = check.read('boundaries.json')
    fresh = copy.deepcopy(original)
    fresh['linked_sha256'] = 'different ELF symbol metadata'
    assert check.boundary_proof(fresh) == check.boundary_proof(original)
    fresh['functions']['func_8038D328']['size'] += 4
    assert check.boundary_proof(fresh) != check.boundary_proof(original)
    original = check.read('semantics/closure-verification.json')
    fresh = copy.deepcopy(original)
    fresh['reproduction_arguments'] = ['different invocation']
    fresh['binding']['linked_elf_sha256'] = 'newly authenticated ELF'
    fresh['binding']['boundaries_sha256'] = 'newly authenticated boundary report'
    assert check.semantic_proof(fresh) == check.semantic_proof(original)
    fresh['behavior_sha256'] = 'different ordered effects'
    assert check.semantic_proof(fresh) != check.semantic_proof(original)


def test_negative_controls_only_normalize_the_zero_distance_diagnostic():
    check = verifier()
    original = check.read('semantics/controls.json')
    fresh = copy.deepcopy(original)
    fresh['controls'][6]['reason'] = 'division by zero'
    assert fresh != original  # The old whole-receipt comparison rejected this.
    for receipt in ('semantics/controls.json', 'semantic-controls-replay.json'):
        check.check_negative_controls(fresh, check.read(receipt), receipt)
    assert check.read('semantics/controls.json') == original
    assert fresh['controls'][6]['reason'] == 'division by zero'  # No mutation.


def test_negative_control_proof_retains_every_rejection_fact():
    check = verifier()
    original = check.read('semantics/controls.json')

    def reject(fresh):
        with pytest.raises(AssertionError, match='negative control drift'):
            check.check_negative_controls(fresh, original, 'adverse fixture')

    # Every top-level fact and every control field remains bound. In particular,
    # arbitrary exception text cannot pass merely because a mutant was rejected.
    for key in original:
        if key == 'controls':
            continue
        fresh = copy.deepcopy(original)
        fresh[key] = 'deliberate drift'
        reject(fresh)
    for index, control in enumerate(original['controls']):
        for key in control:
            fresh = copy.deepcopy(original)
            fresh['controls'][index][key] = 'deliberate drift'
            reject(fresh)
            fresh = copy.deepcopy(original)
            del fresh['controls'][index][key]
            reject(fresh)
        fresh = copy.deepcopy(original)
        fresh['controls'][index]['result'] = 'accepted'
        reject(fresh)
        fresh['controls'][index]['unknown_proof_field'] = True
        reject(fresh)
    for reason in ('integer division or modulo by zero', 'unsupported opcode',
                   'unmapped read: division by zero', '', None,
                   {'category': 'division by zero'}):
        fresh = copy.deepcopy(original)
        fresh['controls'][6]['reason'] = reason
        reject(fresh)
    for replacement in (original['controls'][:-1],
                        original['controls'] + [original['controls'][0]],
                        list(reversed(original['controls'])),
                        [original['controls'][0]] * len(original['controls'])):
        fresh = copy.deepcopy(original)
        fresh['controls'] = replacement
        reject(fresh)
    fresh = copy.deepcopy(original)
    fresh['unknown_proof_field'] = True
    reject(fresh)
    # A host diagnostic variant must not hide a second, substantive failure.
    fresh['controls'][6]['reason'] = 'division by zero'
    reject(fresh)


def test_negative_control_drift_reports_the_exact_field():
    check = verifier()
    original = check.read('semantics/controls.json')
    fresh = copy.deepcopy(original)
    fresh['controls'][6]['reason'] = 'unexpected arithmetic failure'
    with pytest.raises(AssertionError) as error:
        check.check_negative_controls(fresh, original, 'semantics/controls.json')
    assert error.value.args == (('negative control drift', 'semantics/controls.json',
                                ('$.controls[6].reason', 'float division by zero',
                                 'unexpected arithmetic failure')),)
