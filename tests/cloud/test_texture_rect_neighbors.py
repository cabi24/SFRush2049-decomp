"""Portable controls for bounded SDK sibling-scope research; IDO runs separately."""
import importlib.util
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT/'cloud/work/texture_rect_neighbors'
spec = importlib.util.spec_from_file_location('rect_neighbor_verify', HERE/'verify.py')
verify = importlib.util.module_from_spec(spec)
spec.loader.exec_module(verify)


def test_controls_are_exact_declared_transformation():
    verify.validate_sources()


def test_sibling_emitter_counts():
    for name, decomposed in [('sdk_sibling_both', 1), ('sdk_sibling_all', 8)]:
        body = (HERE/(name+'.c')).read_text().split('void func_80087110(', 1)[1]
        assert body.count('gSPTextureRectangle(') == 8-decomposed
        assert body.count('gDPLoadTileGeneric(') == decomposed
        assert body.count('gImmp1(') == decomposed*2
        assert 'while' not in body and 'goto' not in body


def test_listing_normalization_preserves_alias_metadata_and_instructions():
    # Synthetic text only. The normalizer is never fed to the assembler.
    a = '# source\n.loc 1 20\n.file 1 "a.c"\n\t.alias\t$3,$sp\nL:\n\taddu\t$2,$2,$4\n'
    b = '# other source\n.loc 2 30\n.file 2 "b.c"\n\t.alias\t$3,$sp\nL:\n\taddu\t$2,$2,$4\n'
    assert verify.normalized_listing(a) == verify.normalized_listing(b)
    assert verify.normalized_listing(a) != verify.normalized_listing(a.replace('.alias', '.noalias'))
    assert verify.normalized_listing(a) != verify.normalized_listing(a.replace('$4', '$5'))


def test_saved_receipt_is_research_and_fully_linked():
    receipt = json.loads((HERE/'verification.json').read_text())
    assert receipt['accepted'] is False
    for name, row in receipt['rectangle_controls'].items():
        proof = row['proof']
        assert proof['elf_function_bytes'] == 1780
        assert proof['project_scorer_differing_words'] == 4
        assert proof['project_scorer_target_words'] == 445
        assert proof['gnu_linker_equals_project_relocator'] is True
        assert not proof['accepted_exact_match']
        assert not proof['unresolved'] and not proof['unverified'] and not proof['relocation_errors']
        assert row['semantics']['cases'] + row['additional_semantic_cases'] == 6926
        assert row['source_sha256'] == verify.sha(ROOT/row['source'])
        if name != 'baseline':
            assert row['linked_body_equals_baseline']
            assert row['ugen_nonlocation_equals_baseline']
