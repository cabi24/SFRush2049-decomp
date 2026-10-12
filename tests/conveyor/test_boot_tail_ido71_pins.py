"""The committed IDO 7.1 switch: exactly the 19 Rush boot-tail ROM TUs build with
IDO 7.1 (rom_compilers.json -> src/rom/opt_overrides.mk). Evidence:
cloud/work/frontier/w17g (tu71.log, locked71.log) and w17k (full-ROM gate);
0x26f60 (lib_26f60, libgsm decoder) converted and pinned by lane w24a."""
from pathlib import Path

from tools.conveyor import compilers

REPO = Path(__file__).resolve().parents[2]


BOOT_TAIL = ("11050 113e0 11640 15930 15a10 16320 17dc0 1a660 1cf90 1ece0 1f340 "
             "1f5b0 207b0 21210 22300 257f0 25bb0 26ab0 26f60").split()


def test_committed_pins_are_exactly_the_boot_tail_tus():
    pins = compilers.load_segment_pins()
    assert pins == {f"0x{s}": "ido7.1" for s in BOOT_TAIL}
    for s in BOOT_TAIL:
        assert (REPO / f"src/rom/lib_{s}.c").is_file()


def test_committed_overrides_select_ido71_for_exactly_the_pinned_tus():
    text = (REPO / "src/rom/opt_overrides.mk").read_text()
    cc_lines = [line for line in text.splitlines() if ": CC := " in line]
    assert cc_lines == [f"$(BUILD_DIR)/src/rom/lib_{s}.o: CC := $(IDO71_CC)" for s in BOOT_TAIL]
