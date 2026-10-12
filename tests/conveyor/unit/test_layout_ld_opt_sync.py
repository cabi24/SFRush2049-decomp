"""w24t: generated build inputs stay equal to their generators.

- rush2049.us.ld names each converted subsegment's TU object, never its old
  build/us/asm/us/<SEG>.o (the 0x26F60 conversion's gate failures);
  `layout ldsync` repairs exactly that and refuses other drift.
- src/rom/opt_overrides.mk equals layout.render_opt_overrides() (a hand edit
  in 7a90b14e drifted from the generator until 4b4be251 regenerated it)."""
from pathlib import Path

import pytest

from tools.conveyor.pipeline import layout as L

REPO = Path(__file__).resolve().parents[3]

SUBSEGS = [{"off": 0x26AB0, "type": "c", "name": "rom/lib_26ab0"},
           {"off": 0x26F60, "type": "c", "name": "rom/lib_26f60"},
           {"off": 0x8410, "type": "asm", "name": None}]


def _ld(*objs):
    return "".join(f"        {o}({sec});\n" for sec in (".text", ".data", ".rodata", ".bss") for o in objs)


def test_stale_asm_object_of_a_converted_segment_is_rewritten(tmp_path):
    ld = tmp_path / "rush2049.us.ld"
    ld.write_text(_ld("build/us/src/rom/lib_26ab0.o", "build/us/asm/us/26F60.o", "build/us/asm/us/8410.o"))
    assert L.sync_linker_objects(ld, check=True, subsegs=SUBSEGS) == [
        ("build/us/asm/us/26F60.o", "build/us/src/rom/lib_26f60.o")]
    assert "26F60.o" in ld.read_text()                     # --check changes nothing
    L.sync_linker_objects(ld, subsegs=SUBSEGS)
    text = ld.read_text()
    assert "26F60.o" not in text and text.count("build/us/src/rom/lib_26f60.o(") == 4
    assert L.sync_linker_objects(ld, subsegs=SUBSEGS) == []   # idempotent


def test_drift_a_substitution_cannot_fix_refuses(tmp_path):
    ld = tmp_path / "rush2049.us.ld"
    # a reverted segment: yaml says asm, the script still names the TU
    ld.write_text(_ld("build/us/src/rom/lib_26ab0.o", "build/us/src/rom/lib_26f60.o", "build/us/src/rom/lib_8410.o"))
    before = ld.read_text()
    with pytest.raises(SystemExit, match="make extract"):
        L.sync_linker_objects(ld, subsegs=SUBSEGS)
    assert ld.read_text() == before


def test_checked_in_linker_script_matches_splat_yaml():
    assert L.sync_linker_objects(REPO / "rush2049.us.ld", check=True) == []


def test_makefile_regenerates_ld_from_splat_yaml():
    text = (REPO / "Makefile").read_text()
    rule = next(line for line in text.splitlines() if line.startswith("$(LD_SCRIPT):"))
    assert "splat.us.yaml" in rule
    assert "pipeline.layout ldsync $@" in text


@pytest.mark.skipif(not (REPO / "baserom.us.z64").is_file(), reason="layout.derive reads the ROM (absent on CI)")
def test_checked_in_opt_overrides_equal_the_generator():
    assert (REPO / "src/rom/opt_overrides.mk").read_text() == L.render_opt_overrides()
