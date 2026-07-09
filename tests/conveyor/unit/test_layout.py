"""Layout map derivation (`pipeline.layout`, 004), per contracts/layout-map.md.

Pi-local: fixture splat yaml + symbol_addrs + monkeypatched region/word
sources — no real asm, no builder. Covers clean tiling, the refusal families
(gap/data, unnamed, dynamic), determinism, and coverage math.
"""
import json
import types

import pytest

from tools.conveyor.pipeline import layout as L
from tools.conveyor.pipeline import targets as targetsmod

# Fixture ROM: two functions in one segment, padded to the next segment.
#   seg 0x8800 (vram 0x80007c00, ends 0x80007c40 next subseg):
#     f_a @ 0x80007c00 (8 instrs = 32 bytes) then 32 bytes nop pad -> 64 total
#   seg 0x8840 (vram 0x80007c40, ends 0x80007c68):
#     f_b @ 0x80007c40 (10 instrs = 40 bytes) exactly
SPLAT_FIXTURE = """\
segments:
  - name: main
    type: code
    start: 0x8800
    vram: 0x80007c00
    subsegments:
      - [0x8800, asm]
      - [0x8840, asm]
      - [0x8868, asm]
  - type: bin
    start: 0x8900
"""

SYMS_FIXTURE = """\
f_a = 0x80007C00; // type:func
f_b = 0x80007C40; // type:func
func_80007C68 = 0x80007C68; // type:func
some_data = 0x80007C90; // type:data
"""


def _region(nwords):
    return types.SimpleNamespace(words=["00000000"] * nwords)


@pytest.fixture
def fixture_repo(tmp_path, monkeypatch):
    splat = tmp_path / "splat.yaml"
    splat.write_text(SPLAT_FIXTURE)
    syms = tmp_path / "symbol_addrs.txt"
    syms.write_text(SYMS_FIXTURE)
    lock = tmp_path / "matched.lock.json"
    lock.write_text(json.dumps({
        "src/x.c:f_a": {"target_id": "f_a", "flagset": "-O2"},
        "src/x.c:f_b": {"target_id": "f_b", "flagset": "-O2"},
    }))
    monkeypatch.setattr(L, "SPLAT_YAML", splat)
    monkeypatch.setattr(L, "SYMBOL_ADDRS", syms)
    monkeypatch.setattr(L, "LOCKFILE", lock)
    monkeypatch.setattr(L, "NONMATCHINGS", tmp_path / "nonmatchings")  # absent

    # Regions: f_a=8 instrs, f_b=10, func@c68=... (segment 0x8868 -> 0x8900 =
    # 0x98 bytes = 38 words; give it 38 so it tiles exactly).
    regions = {0x80007C00: _region(8), 0x80007C40: _region(10),
               0x80007C68: _region(38)}
    monkeypatch.setattr(targetsmod, "index_asm_regions",
                        lambda asm_dir=None: dict(regions))

    # All gap bytes read as zero (nop padding) by default.
    monkeypatch.setattr(targetsmod, "function_words",
                        lambda vaddr, nbytes: ["00000000"] * (nbytes // 4))
    return tmp_path


def test_clean_tile_with_padding_absorbed(fixture_repo):
    m = L.derive()
    seg = L._segment_by_name(m, "0x8800")
    assert seg["refusal"] is None
    assert seg["flagset"] == "-O2"
    # f_a: 8 instrs = 32 bytes of code, tiled to the next function at 0x8840 =
    # 64 bytes (32 nop padding absorbed).
    assert seg["functions"] == [
        {"name": "f_a", "vaddr": "0x80007c00", "size": 64, "state": "passthrough"}]
    seg_b = L._segment_by_name(m, "0x8840")
    assert seg_b["functions"][0]["size"] == 40  # exact, no padding


def test_dynamic_population_entry_refuses(fixture_repo):
    m = L.derive()
    dyn = L._segment_by_name(m, "dynamic")
    assert dyn["refusal"] == "dynamic-population (blob rebuild feature)"
    assert dyn["rom_tu"] is None


def test_unnamed_function_refuses(fixture_repo):
    # segment 0x8868's function has an auto-name (func_80007C68) -> refusal.
    m = L.derive()
    seg = L._segment_by_name(m, "0x8868")
    assert seg["refusal"] == "unnamed@0x80007c68"
    assert seg["functions"] == []


def test_nonzero_data_gap_refuses(fixture_repo, monkeypatch):
    # f_a is only 8 instrs but the slot is 64 bytes; make the 32 padding bytes
    # non-zero (a jumptable/rodata) -> data refusal (D4).
    def words(vaddr, nbytes):
        # non-zero at the very first padding word after f_a's 32 code bytes
        return ["DEADBEEF"] + ["00000000"] * (nbytes // 4 - 1)
    monkeypatch.setattr(targetsmod, "function_words", words)
    m = L.derive()
    seg = L._segment_by_name(m, "0x8800")
    assert seg["refusal"] == "data@0x80007c20: non-zero bytes not in any region"


def test_flag_conflict_yields_null_flagset(fixture_repo, monkeypatch):
    lock = fixture_repo / "matched.lock.json"
    lock.write_text(json.dumps({
        "src/x.c:f_a": {"target_id": "f_a", "flagset": "-O2"},
        "src/x.c:f_b": {"target_id": "f_b", "flagset": "-O1"},  # different
    }))
    # Put both functions in one segment by making 0x8840 a data start so they
    # tile together: simplest is to check the segment-level join helper directly.
    assert L._segment_flagset(["f_a", "f_b"], {"f_a": "-O2", "f_b": "-O1"}) is None
    assert L._segment_flagset(["f_a", "f_b"], {"f_a": "-O2", "f_b": "-O2"}) == "-O2"
    assert L._segment_flagset(["f_a"], {}) is None


def test_determinism(fixture_repo):
    a = L.map_hash(L.derive())
    b = L.map_hash(L.derive())
    assert a == b
    # The map JSON itself is byte-identical across runs.
    j1 = json.dumps(L.derive(), sort_keys=True)
    j2 = json.dumps(L.derive(), sort_keys=True)
    assert j1 == j2


def test_map_hash_stable_across_promotion_state(fixture_repo):
    # The structural (TU-header) hash must not move when a slot flips to
    # promoted — only the body changes, not the structure.
    m = L.derive()
    h1 = L.map_hash(m)
    seg = L._segment_by_name(m, "0x8800")
    seg["functions"][0]["state"] = "promoted"
    seg["functions"][0]["body"] = "int f_a(void){return 0;}"
    assert L.map_hash(m) == h1


def test_coverage_math(fixture_repo):
    m = L.derive()
    cov = L.coverage(m)
    # Two clean single-function segments (0x8800=64b, 0x8840=40b); 0x8868 and
    # dynamic refuse -> excluded. Nothing promoted yet.
    assert cov["static_functions"] == 2
    assert cov["static_bytes"] == 104
    assert cov["promoted_functions"] == 0 and cov["promoted_bytes"] == 0
    # Promote f_a -> coverage reflects exactly its size.
    L._segment_by_name(m, "0x8800")["functions"][0]["state"] = "promoted"
    cov2 = L.coverage(m)
    assert cov2["promoted_functions"] == 1 and cov2["promoted_bytes"] == 64


def test_generated_tu_is_passthrough_and_deterministic(fixture_repo):
    m = L.derive()
    seg = L._segment_by_name(m, "0x8800")
    mh = L.map_hash(m)
    tu1 = L.generate_tu(seg, mh)
    tu2 = L.generate_tu(seg, mh)
    assert tu1 == tu2
    assert 'GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8800/f_a.s")' in tu1
    assert mh in tu1 and "GENERATED ROM-aligned TU" in tu1
