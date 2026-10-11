"""Image-qualified helper evidence from genuine overlay-A caller context."""
import importlib.util
import json
import os
from pathlib import Path
import pytest

ROOT = Path(__file__).resolve().parents[4]
PACKET = ROOT / 'cloud/work/frontier/dot_runtime_a_callers_deep_20261011'
spec = importlib.util.spec_from_file_location('runtime_a_callers_deep', PACKET / 'verify.py')
verify = importlib.util.module_from_spec(spec)
spec.loader.exec_module(verify)


def test_authorized_source_shape_and_scope():
    source = (PACKET / 'group.c').read_text()
    config = json.loads((PACKET / 'group.json').read_text())
    verify.source_contract(source, config)
    assert sum(verify.SIZES[n] for n in verify.CLAIMS) == 928
    assert 'func_80393004' not in config['claims']
    assert 'func_8039E3BC' not in config['members']
    assert source.count('void func_8039338C(void)\n{') == 1
    assert source.count('void func_80394058(void)\n{') == 1


def test_no_assertion_dependent_author_guards():
    with pytest.raises(ValueError, match='sentinel guard'):
        verify.require(False, 'sentinel guard')


def test_guard_rejects_root_and_claim_drift():
    source = (PACKET / 'group.c').read_text()
    config = json.loads((PACKET / 'group.json').read_text())
    with pytest.raises(ValueError, match='root/ABI'):
        verify.source_contract(source, dict(config, keep=['func_803931C0']))
    with pytest.raises(ValueError, match='claim drift'):
        verify.source_contract(source, dict(config, claims=config['claims'] + ['func_80393004']))
    with pytest.raises(ValueError, match='changed archived body'):
        verify.source_contract(source.replace('slot < 4', 'slot < 3'), config)


def test_full_extent_relocation_and_compiled_controls():
    ido = Path(os.environ.get('IDO_DIR', ROOT/'tools/cloud/ido'))/'cc'
    if not ido.is_file():
        if os.environ.get('REQUIRE_TOOLCHAIN') == '1':
            pytest.fail('required IDO toolchain unavailable')
        pytest.skip('IDO toolchain unavailable')
    import sys
    sys.path.insert(0, str(ROOT/'tools/cloud'))
    import score
    prior_image = score.ASM_DIR
    result = verify.verify()
    assert score.ASM_DIR == prior_image
    assert result['new_candidate_bytes'] == 928
    assert result['accepted_bytes'] == 0
    for name, size in verify.SIZES.items():
        assert result['scores'][name]['differing'] == 0
        assert result['bindings'][name]['compiled_bytes'] == size
    assert len(result['negative_controls']) == 7
    assert result['negative_controls']['cleanup_pointer_loop']['score']['differing'] == 15
    assert result['negative_controls']['without_authentic_9338C']['score']['differing'] == 42
    assert len(result['generated_family_calls']) == 14
