"""The two w19c owned slots as committed: registry rows, companions, TU blocks.

osAiSetNextBuffer owns .data 0x8002C3C0 (16 B; the function-static
hdwrBugFlag, alias __osAiNeedsAlign); __fixunsdfdi owns, with co-owner
__fixunssfdi, .rodata 0x8002D800 (two u64 2^63 bias literals). While the
members are passthrough, the TU carries exactly one companion block.
"""
from pathlib import Path

from tools.conveyor.pipeline import owned_data as O

REPO = Path(__file__).resolve().parents[2]
SLOTS = [
    ("osAiSetNextBuffer", ["osAiSetNextBuffer"], ".data", 0x8002C3C0),
]


def test_committed_rows_match_their_companions_and_tus():
    rows = {r["owner"]: r for r in O.rom_slots(REPO, check_source=False)}
    for owner, members, section, address in SLOTS:
        r = rows[owner]
        assert O.slot_members(r) == members and r["owner_section"] == section
        assert r["container_vram"] + r["offset"] == address and r["size"] == 0x10
        assert (REPO / r["passthrough_asm"]).read_text() == O.companion_source(r)
        text = (REPO / r["tu"]).read_text()
        if all(f'/{m}.s")' in text for m in members):
            assert text.count(O.companion_block(r)) == 1


def test_linker_script_places_both_slots():
    ld = (REPO / "rush2049.us.ld").read_text()
    for owner, members, section, address in SLOTS:
        obj = Path(next(r["tu"] for r in O.rom_slots(REPO, check_source=False)
                        if r["owner"] == owner)).stem
        assert f"rom_owned_{owner}_START == {hex(address)}" in ld
        assert f"/* ROM_OWNED_SECTION_MOVED build/us/src/rom/{obj}.o({section}) */" in ld
