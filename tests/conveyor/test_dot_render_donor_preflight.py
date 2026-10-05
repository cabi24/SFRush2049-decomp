"""Fail-closed metadata tests for the unclaimed render-source probes."""
import importlib.util
import json
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_render_donor_preflight_20261005'
spec = importlib.util.spec_from_file_location('render_donor_preflight', HERE/'verify.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)
RECEIPT = json.loads((HERE/'verification.json').read_text())


def test_source_native_binding():
    probe.check_inputs(RECEIPT)
    assert probe.native_summary() == RECEIPT['native']
    assert RECEIPT['claims'] == []


def test_source_mutation_refuses():
    changed = dict(RECEIPT, source_inputs=dict(RECEIPT['source_inputs']))
    changed['source_inputs'][probe.INPUTS[0]] = '0'*64
    with pytest.raises(ValueError, match='source snapshot changed'):
        probe.check_inputs(changed)


def test_generated_sources_remain_bound():
    controls = probe.controls()
    for label, (source, names) in controls.items():
        result = RECEIPT['compiler'][label]
        assert result['source_sha256'] == probe.sha(source.encode())
        assert set(result['functions']) == set(names)
        assert result['claims'] == []
        assert result['flags'] == probe.FLAGS
        if label != 'remove_archived':
            assert 'volatile' not in source


def test_negative_hypotheses_and_complete_extents():
    expected = {
        'remove_archived': ('sound_stop', 160, 12),
        'remove_recursive': ('sound_stop', 160, 32),
        'remove_recursive_helper': ('sound_stop', 160, 32),
        'remove_loop_helper': ('sound_stop', 160, 14),
        'scan_archived': ('audio_channel_reset', 164, 15),
        'scan_hidden': ('audio_channel_reset', 164, 15),
        'color_archived': ('func_800A7480', 144, 36),
        'color_unsigned': ('func_800A7480', 136, 33),
    }
    for label, (name, size, different) in expected.items():
        item = RECEIPT['compiler'][label]['functions'][name]
        assert (item['elf_bytes'], item['whole_elf_differing_words']) == (size, different)
        assert not item['strict_match']
        assert item['complete_gnu_relocation_equality']
        assert item['own_data_bytes'] == 0


def test_real_context_stays_exact():
    for label, result in RECEIPT['compiler'].items():
        for name, item in result['functions'].items():
            if name in ('Input_ApplyPadConfig', 'Input_InitPadHandlers', 'func_800A79DC'):
                assert item['strict_match']
                assert item['elf_bytes'] == item['native_bytes']
                assert item['whole_elf_differing_words'] == 0
    base, edited = probe.controls()['scan_archived'][0], probe.controls()['scan_hidden'][0]
    accepted = base[:base.index('/* Complete nonmatch')]
    assert edited.startswith(accepted)


def test_research_is_not_semantic_or_native_match():
    colors = RECEIPT['compiler']['color_archived']['functions']['func_800A7480']
    assert colors['canonical']['errors']
    assert colors['elf_bytes'] > colors['native_bytes']
    assert colors['whole_elf_differing_words'] > colors['canonical']['total']
