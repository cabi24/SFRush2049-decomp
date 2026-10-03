"""Portable checks for the narrowly rejected D06 hypothesis; no match claims."""
import importlib.util
from pathlib import Path
import pytest

PATH = Path(__file__).resolve().parents[2] / 'cloud/work/d06_tire_stack_diagnosis/replay.py'
SPEC = importlib.util.spec_from_file_location('d06_diagnosis', PATH)
m = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(m)


def test_predicate_results_and_short_circuit_reads():
    assert m.predicate_check() == 343


def test_source_identity_fails_closed():
    with pytest.raises(AssertionError):
        m.candidate(m.ORIGINAL.encode())
