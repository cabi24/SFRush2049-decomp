"""IDO `-mips3 -32` ROM TUs: the CFLAGS override and the EF_MIPS_32BITMODE fix."""
import re
import struct
import subprocess
import sys
from pathlib import Path

import pytest

from tools.conveyor.pipeline import layout
from tools.conveyor.pipeline import o32abi

REPO = Path(__file__).resolve().parents[2]
BASE_CFLAGS = "-G 0 -mips2 -O2 -non_shared -Iinclude -Wab,-r4300_mul -Xcpluscomm"


def _overrides(tmp_path, monkeypatch, segments):
    fragment = tmp_path / "overrides.mk"
    monkeypatch.setattr(layout, "OPT_OVERRIDES_MK", fragment)
    layout.write_opt_overrides({"segments": segments})
    return fragment


def _effective(tmp_path, fragment, tu="probe"):
    makefile = tmp_path / "Makefile"
    makefile.write_text(
        f"COMPILER := ido\nBUILD_DIR := build\nCFLAGS := {BASE_CFLAGS}\n"
        f"include {fragment}\n"
        f"build/src/rom/{tu}.o:\n\t@echo $(CFLAGS)\n")
    return subprocess.run(["make", "--no-print-directory", "-f", str(makefile),
                           f"build/src/rom/{tu}.o"], check=True, capture_output=True,
                          text=True).stdout.split()


def _seg(flagset, tu="rom/probe", converted=True):
    return {"yaml_name": "0xe4f0", "converted": converted, "flagset": flagset, "rom_tu": tu}


def test_mips3_32_segment_gets_isa_and_opt_substitution(tmp_path, monkeypatch):
    frag = _overrides(tmp_path, monkeypatch, [_seg("-g0 -O1 -mips3 -32 -G 0 -non_shared")])
    assert "$(subst -mips2,-mips3 -32,$(subst -O2,-O1,$(CFLAGS)))" in frag.read_text()
    flags = _effective(tmp_path, frag)
    assert flags[:6] == ["-G", "0", "-mips3", "-32", "-O1", "-non_shared"]
    assert "-mips2" not in flags and "-O2" not in flags
    assert "-Wab,-r4300_mul" in flags and "-Xcpluscomm" in flags


def test_mips3_32_at_default_opt_is_not_skipped_as_default(tmp_path, monkeypatch):
    frag = _overrides(tmp_path, monkeypatch, [_seg("-g0 -O2 -mips3 -32 -G 0 -non_shared")])
    flags = _effective(tmp_path, frag)
    assert "-mips3" in flags and "-32" in flags and "-O2" in flags and "-mips2" not in flags


def test_mips2_segments_are_unchanged(tmp_path, monkeypatch):
    frag = _overrides(tmp_path, monkeypatch, [
        _seg("-g0 -O2 -mips2 -G 0 -non_shared", "rom/a"),
        _seg("-g0 -O1 -mips2 -G 0 -non_shared", "rom/b"),
        _seg("-g0 -O1 -mips3 -32 -G 0 -non_shared", "rom/c", converted=False)])
    text = frag.read_text()
    assert "rom/a.o" not in text and "rom/c.o" not in text
    assert "$(BUILD_DIR)/src/rom/b.o: CFLAGS := $(subst -O2,-O1,$(CFLAGS))" in text
    assert "mips3" not in text


@pytest.mark.parametrize("flagset", ["-g0 -O1 -mips3 -G 0", "-O1 -mips1", "-O1 -mips2 -mips3 -32"])
def test_other_isa_selections_refuse(tmp_path, monkeypatch, flagset):
    with pytest.raises(SystemExit, match="ISA the ROM-TU"):
        _overrides(tmp_path, monkeypatch, [_seg(flagset)])


def _elf(flags, machine=8, e_type=1, cls=1, data=2):
    head = b"\x7fELF" + bytes([cls, data, 1]) + b"\0" * 9
    head += struct.pack(">HHIIIII", e_type, machine, 1, 0, 0, 0, flags)
    return head + b"\0" * 16 + b"payload"


def test_set_32bitmode_sets_only_the_flag(tmp_path):
    obj = tmp_path / "a.o"
    obj.write_bytes(_elf(0x20000001))
    assert o32abi.main([str(obj), "--cflags", "-G 0 -mips3 -32 -O1 -non_shared"]) == 0
    out = obj.read_bytes()
    assert struct.unpack_from(">I", out, 36)[0] == 0x20000101
    assert out[:36] == _elf(0x20000001)[:36] and out[40:] == _elf(0x20000001)[40:]
    assert o32abi.main([str(obj), "--cflags", "-mips3 -32"]) == 0      # idempotent
    assert obj.read_bytes() == out


def test_untouched_without_mips3_32(tmp_path):
    obj = tmp_path / "a.o"
    obj.write_bytes(_elf(0x20000001))
    for cflags in ("-G 0 -mips2 -O2", "-mips3 -O1", "-32 -O1"):
        assert o32abi.main([str(obj), "--cflags", cflags]) == 0
        assert obj.read_bytes() == _elf(0x20000001)


@pytest.mark.parametrize("blob,why", [
    (_elf(0x10000001), "not MIPS III"),
    (_elf(0x20000001, machine=3), "not a MIPS relocatable"),
    (_elf(0x20000001, e_type=2), "not a MIPS relocatable"),
    (_elf(0x20000001, data=1), "big-endian ELF32"),
    (b"not an elf" * 8, "not an ELF"),
])
def test_refuses_unexpected_objects(tmp_path, capsys, blob, why):
    obj = tmp_path / "a.o"
    obj.write_bytes(blob)
    assert o32abi.main([str(obj), "--cflags", "-mips3 -32"]) == 1
    assert why in capsys.readouterr().err
    assert obj.read_bytes() == blob


def _rom_tu_recipe():
    text = (REPO / "Makefile").read_text()
    rule = text[text.index("$(BUILD_DIR)/$(SRC_DIR)/rom/%.o: $(SRC_DIR)/rom/%.c"):]
    return [line for line in rule.split("\n\n")[0].splitlines() if line.startswith("\t")]


def test_makefile_rom_tu_rule_marks_only_mips3_objects(tmp_path):
    recipe = _rom_tu_recipe()
    fix = [i for i, line in enumerate(recipe) if "o32abi" in line]
    owned = [i for i, line in enumerate(recipe) if "owned_data object" in line]
    assert len(fix) == 1 and owned and fix[0] < owned[0]
    for cflags, expected in ((BASE_CFLAGS.replace("-mips2", "-mips3 -32"), 0x20000101),
                             (BASE_CFLAGS, 0x20000001)):
        obj = tmp_path / "build/src/rom/probe.o"
        obj.parent.mkdir(parents=True, exist_ok=True)
        obj.write_bytes(_elf(0x20000001))
        makefile = tmp_path / "Makefile"
        makefile.write_text(f"PYTHON := {sys.executable}\nV := @\nCFLAGS := {cflags}\n"
                            f"probe:\n{recipe[fix[0]]}\n".replace("$@", str(obj)))
        subprocess.run(["make", "--no-print-directory", "-f", str(makefile), "probe"],
                       check=True, cwd=REPO, capture_output=True, text=True)
        assert struct.unpack_from(">I", obj.read_bytes(), 36)[0] == expected
