"""Keep debug O1 assembly placeholders and real compiler flags consistent."""
import importlib.util
from pathlib import Path
import subprocess

import pytest

from tools.conveyor.pipeline import layout


spec = importlib.util.spec_from_file_location(
    "asm_processor_compiler_flags",
    Path(__file__).parents[2] / "tools/asm-processor/compiler_flags.py")
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)


@pytest.mark.parametrize("debug", ["-g1", "-g2"])
def test_debug_o1_uses_measured_four_word_placeholder_model(debug):
    args = ["-c", "-mips2", debug, "-O1", "-G", "0", "-non_shared"]
    original = list(args)
    assert module.processor_flags(args) == ["-g"]
    assert args == original  # The real IDO recipe must remain intact.


@pytest.mark.parametrize("args,expected", [
    (["-g0", "-O1", "-mips2"], ["-O1"]),
    (["-O2", "-g3", "-mips2"], ["-O2", "-g3"]),
    (["-g", "-framepointer", "-mips2"], ["-g", "-framepointer"]),
    (["-O0"], ["-O0", "-mips1"]),
    (["-g1", "-O1", "-g0", "-mips2"], ["-O1"]),
])
def test_existing_models_and_effective_debug_option_are_preserved(args, expected):
    assert module.processor_flags(args) == expected


def _generated_flags(tmp_path, monkeypatch, recipe, converted=True):
    fragment = tmp_path / "overrides.mk"
    monkeypatch.setattr(layout, "OPT_OVERRIDES_MK", fragment)
    layout.write_opt_overrides({"segments": [{
        "converted": converted, "flagset": recipe, "rom_tu": "rom/probe"}]})
    makefile = tmp_path / "Makefile"
    makefile.write_text(
        "COMPILER := ido\nBUILD_DIR := build\n"
        "CFLAGS := -mips2 -O2 -g0 -g3 -Iinclude -Wab,-r4300_mul -Xcpluscomm\n"
        f"include {fragment}\n"
        "build/src/rom/probe.o:\n\t@echo $(CFLAGS)\n")
    result = subprocess.run(
        ["make", "--no-print-directory", "-f", str(makefile), "build/src/rom/probe.o"],
        check=True, capture_output=True, text=True)
    return result.stdout.split()


@pytest.mark.parametrize("debug", ["-g1", "-g2"])
def test_generated_make_recipe_preserves_includes_and_errata(tmp_path, monkeypatch, debug):
    flags = _generated_flags(tmp_path, monkeypatch, f"{debug} -O1 -mips2 -G 0")
    assert "-O1" in flags and "-O2" not in flags
    assert [flag for flag in flags if flag.startswith("-g")] == [debug]
    assert "-Iinclude" in flags and "-Wab,-r4300_mul" in flags and "-Xcpluscomm" in flags


def test_debug_override_also_applies_with_default_optimization(tmp_path, monkeypatch):
    flags = _generated_flags(tmp_path, monkeypatch, "-g1 -O2")
    assert "-O2" in flags
    assert [flag for flag in flags if flag.startswith("-g")] == ["-g1"]


def test_unconverted_segment_does_not_change_build_recipe(tmp_path, monkeypatch):
    flags = _generated_flags(tmp_path, monkeypatch, "-g1 -O1", converted=False)
    assert "-O2" in flags and "-g0" in flags and "-g3" in flags


@pytest.mark.parametrize("debug", ["-g1", "-g2"])
@pytest.mark.parametrize("optimization,expected", [
    (["-O1", "-O3"], ["-O1"]),
    (["-O3", "-O1"], ["-g"]),
])
def test_last_optimization_option_controls_debug_model(debug, optimization, expected):
    args = [debug, *optimization, "-mips2"]
    original = list(args)
    assert module.processor_flags(args) == expected
    assert args == original
