"""asm_matched.lock.json: static functions counted as built from assembly source.

Owner policy (b), 2026-10-21: a static function that was assembly in the
original build counts as static coverage "from assembly", reported apart from
C. The lock must stay honest: only functions with evidence of no C form, built
from the repo's own .s, with words that still match what was pinned.
"""
import copy

import pytest

from tools.conveyor.pipeline import asm_lock as A, layout
from tools.conveyor.pipeline import lock as clock

C_REACHABLE = ("func_80012D18", "__osAiDeviceBusy")    # docs/static-asm-ceiling.md §3

# layout.derive reads function words from the ROM.
pytestmark = pytest.mark.skipif(not A.BASEROM.is_file(), reason="needs the ROM")


@pytest.fixture(scope="module")
def mapping():
    return layout.derive()


@pytest.fixture(scope="module")
def entries():
    return A.load_lock()


def test_the_live_lock_holds(entries, mapping):
    assert len(entries) == 33
    assert A.check(entries, mapping) == []


def test_every_entry_has_evidence_of_no_c_form(entries):
    for name, e in entries.items():
        kind, _, detail = e["evidence"].partition(":")
        assert kind in ("ultralib", "handwritten") and detail.strip(), name
        if kind == "ultralib":
            A.check_evidence(e["evidence"], int(e["vaddr"], 16), e["words"])
    assert not set(C_REACHABLE) & set(entries)


def test_no_entry_shadows_c(entries, mapping):
    c_targets = {e.get("target_id") for e in clock.load_lock().values()}
    assert not set(entries) & c_targets
    states = {f["name"]: f.get("state") for s in mapping["segments"] for f in s.get("functions", [])}
    assert all(states[name] != "promoted" for name in entries)


def test_drift_and_promotion_are_reported(entries, mapping):
    drifted = copy.deepcopy(entries)
    drifted["osGetCount"]["words_sha256"] = "0" * 64
    assert ("osGetCount", "assembly words drifted from the locked form") in A.check(drifted, mapping)
    promoted = copy.deepcopy(mapping)
    for seg in promoted["segments"]:
        for f in seg.get("functions", []):
            if f["name"] == "bcopy":
                f["state"] = "promoted"
    assert ("bcopy", "promoted to C: remove the asm entry") in A.check(entries, promoted)


def test_add_refuses_c_and_bad_evidence(mapping):
    with pytest.raises(A.AsmLockError, match="C lock"):
        A.make_entry("bcopy", "handwritten:no", mapping,
                     c_lock={"src/libc/bcopy.c:bcopy": {"target_id": "bcopy"}})
    with pytest.raises(A.AsmLockError, match="already promoted"):
        A.make_entry("__osAiDeviceBusy", "handwritten:no", mapping)    # C since 2026-10-21
    with pytest.raises(A.AsmLockError, match="evidence must be"):
        A.check_evidence("vibes:it looks like asm", 0x8000C970, 3)
    with pytest.raises(A.AsmLockError, match="no IDENTICAL row"):
        A.check_evidence("ultralib:src/os/getsr.s", 0x8000C970, 3)


def test_coverage_counts_only_unpromoted_locked_functions(entries, mapping):
    by_name = A.coverage(mapping, entries)
    assert set(by_name) == set(entries)
    assert sum(by_name.values()) == 6252
