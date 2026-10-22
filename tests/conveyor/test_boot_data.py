"""Every label in pipeline.boot_data, re-proven against the ROM.

The boot segment past the static code (RAM 0x800277D0..0x8002E8E0) is still
inside the ROM `data` bin. boot_data labels it in place: hand-written CPU
routines, RSP microcode text and data, and C .data/.rodata. These checks are
the evidence for those labels and for counting the CPU routines as static
functions.
"""
import struct

import pytest

from tools.conveyor.pipeline import blob_layout, blob_rom, boot_data as B, layout

JR_RA = 0x03E00008
VECTOR_OPS = (0x12, 0x32, 0x3A)        # COP2, LWC2, SWC2: the RSP vector unit

pytestmark = pytest.mark.skipif(not blob_rom.BASEROM.is_file(), reason="needs the ROM")


def _words(lo, hi):
    rom = blob_rom.BASEROM.read_bytes()
    return struct.unpack(f">{(hi - lo) // 4}I", rom[B.rom_offset(lo):B.rom_offset(hi)])


def _static():
    return [(r.vaddr, [int(w, 16) for w in r.words], r.name)
            for r in layout.collect_regions().values()]


def _game():
    """(vaddr, words, name) per game function, or [] without the image."""
    if not (blob_layout.IMAGE.is_file() and blob_layout.LAYOUT_JSON.exists()):
        return []
    image = blob_layout.IMAGE.read_bytes()
    out = []
    for region in blob_layout.load()["regions"]:
        for e in region["entries"]:
            if e["kind"] == "function":
                off = e["vaddr"] - blob_layout.BASE
                out.append((e["vaddr"], struct.unpack(f">{e['size'] // 4}I",
                                                      image[off:off + e["size"]]), e["target_id"]))
    return out


def _jump_targets(code):
    for vaddr, words, name in code:
        for i, w in enumerate(words):
            if w >> 26 in (2, 3):
                pc = vaddr + 4 * i
                yield ((pc + 4) & 0xF0000000) | ((w & 0x3FFFFFF) << 2), w >> 26 == 3, name


def _addresses_formed(words):
    """lui/addiu addresses a function takes."""
    hi, out = {}, set()
    for w in words:
        op, rs, rt, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF
        if op == 0x0F:
            hi[rt] = imm << 16
        elif op == 0x09 and rs in hi:
            out.add((hi[rs] + (imm - 0x10000 if imm & 0x8000 else imm)) & 0xFFFFFFFF)
    return out


def test_regions_tile_the_segment_tail_up_to_the_bss_clear():
    assert B.REGIONS[0][0] == B.VRAM_START and B.REGIONS[-1][1] == B.VRAM_END
    for (_, end, _, _), (start, _, _, _) in zip(B.REGIONS, B.REGIONS[1:]):
        assert end == start
    # entrypoint: lui t0,0x8003; lui t1,5; addiu t0,-0x1720; ori t1,0x8170 -> BSS 0x8002E8E0..
    rom = blob_rom.BASEROM.read_bytes()
    lui, _, addiu, _ = struct.unpack(">4I", rom[0x1000:0x1010])
    bss = ((lui & 0xFFFF) << 16) + ((addiu & 0xFFFF) - 0x10000)
    assert bss == B.VRAM_END
    # The boot segment's last code is where splat's data bin starts.
    assert B.rom_offset(B.VRAM_START) == B.ROM_START == 0x283D0


def test_cpu_routines_tile_the_cpu_regions_and_are_reached():
    cpu = [(s, e) for s, e, kind, _ in B.REGIONS if kind == "cpu"]
    routines = sorted((a, n) for a, n, _, _ in B.CPU_ROUTINES)
    covered = []
    for start, end in cpu:
        inside = [(a, n) for a, n in routines if start <= a < end]
        assert inside[0][0] == start and inside[-1][0] + inside[-1][1] == end
        for (a, n), (b, _) in zip(inside, inside[1:]):
            assert a + n == b
        covered += inside
    assert covered == routines
    reached = {a: set() for a, _ in routines}
    for target, is_jal, name in _jump_targets(_static()):
        if target in reached and is_jal:
            reached[target].add(name)
    kernels = _words(0x800277D0, 0x80027DD0)
    for i, w in enumerate(kernels):
        pc = 0x800277D0 + 4 * i
        off = (w & 0xFFFF) - (0x10000 if w & 0x8000 else 0)
        if w >> 16 == 0x0411:                                    # bal
            reached.get(pc + 4 + 4 * off, set()).add("bal")
        if w >> 16 == 0x0410 and kernels[i + 2] >> 16 == 0x27EB:   # bltzal $0; addiu t3,ra,imm
            imm = kernels[i + 2] & 0xFFFF
            reached.get(pc + 8 + imm, set()).add("PC-relative callback")
    assert all(reached.values()), {hex(a): r for a, r in reached.items() if not r}
    assert {"func_80026558", "func_80026360"} <= set().union(*reached.values())


def test_cpu_routines_are_cpu_code():
    for a, n, name, _ in B.CPU_ROUTINES:
        words = _words(a, a + n)
        assert not [w for w in words if w >> 26 in VECTOR_OPS], name
    # Every routine returns with jr ra or branches into one that does.
    returns = {a for a, n, _, _ in B.CPU_ROUTINES if JR_RA in _words(a, a + n)}
    assert returns == {a for a, _, _, _ in B.CPU_ROUTINES} - {0x80027D3C, 0x80027D54, 0x80027D64}
    for a in (0x80027D3C, 0x80027D54, 0x80027D64):
        n = dict((x, y) for x, y, _, _ in B.CPU_ROUTINES)[a]
        branch = _words(a, a + n)[-2]
        assert branch >> 16 == 0x1000 and a + n - 4 + 4 * (branch & 0xFFFF) == 0x80027D7C


def test_rsp_text_is_microcode_and_is_named_by_the_tasks():
    for start, end, kind, label in B.REGIONS:
        words = _words(start, end)
        vector = sum(1 for w in words if w >> 26 in VECTOR_OPS) / len(words)
        if kind == "rsp_text" and start != 0x80027DD0:           # rspboot is a DMA stub
            assert vector > 0.30, label
        if kind in ("cpu", "data"):
            assert vector < 0.02, label
    audio = [w for v, w, n in _static() if n == "func_80011910"][0]
    taken = _addresses_formed(audio)
    assert {0x80027DD0, 0x80027EA0, 0x8002A050, 0x8002E210} <= taken
    assert 0x01E3C023 in audio                                    # subu t8,t7,v1: boot size
    game = _game()
    if game:
        gfx = _addresses_formed([w for v, w, n in game if n == "world_collision_response"][0])
        assert {0x80027DD0, 0x80027EA0, 0x80028CC0, 0x8002DDF0} <= gfx


def test_nothing_jumps_into_microcode_or_data():
    rsp_or_data = [(s, e) for s, e, kind, _ in B.REGIONS if kind != "cpu"]
    for target, _, name in _jump_targets(_static() + _game()):
        assert not any(s <= target < e for s, e in rsp_or_data), (name, hex(target))
    for start, end, kind, label in B.REGIONS:
        if kind in ("data", "rsp_data"):
            assert JR_RA not in _words(start, end), label


def test_data_landmarks():
    rom = blob_rom.BASEROM.read_bytes()
    gfx_data = rom[B.rom_offset(0x8002DDF0):B.rom_offset(0x8002E210)]
    assert b"RSP Gfx ucode F3DEX.NoN   fifo 2.08" in gfx_data
    # aspMain data ends with its 512 B resample table; the build-info record
    # after it is five pointers into the stamp strings that follow.
    asp = rom[B.rom_offset(0x8002E210):B.rom_offset(0x8002E440)]
    before = rom[B.rom_offset(0x8002DBF0):B.rom_offset(0x8002DDF0)]
    assert asp[-0x8 - 512:-0x8] == before
    pointers = struct.unpack(">5I", rom[B.rom_offset(0x8002E440):B.rom_offset(0x8002E454)])
    assert all(0x8002E460 <= p < 0x8002E4A0 for p in pointers)
    assert "__osScCurAudioTask = 0x8002AFA0;" in (layout.REPO / "symbol_addrs.us.txt").read_text()


# --- built from assembly (asm/us/boot_data, composed by the Makefile) ----------

def test_the_makefile_composes_each_source_into_its_slot():
    assert B.makefile_slots() == B.expected_slots()
    assert set(B.SOURCES) == {p.name for p in B.ASM_DIR.glob("*.s")}
    cpu = sorted((s, e) for s, e, kind, _ in B.REGIONS if kind == "cpu")
    assert sorted(B.SOURCES.values()) == cpu


def test_the_sources_hold_the_rom_words_with_tables_as_data():
    for name, (lo, hi) in B.SOURCES.items():
        words = B.source_words(B.ASM_DIR / name)
        assert sorted(words) == list(range(lo, hi, 4)), name
        assert [words[v] for v in range(lo, hi, 4)] == list(_words(lo, hi)), name
    text = (B.ASM_DIR / "kernels.s").read_text()
    for vaddr, size, _ in B.INLINE_TABLES:
        assert f"dlabel D_{vaddr:08X}" in text and f"glabel func_{vaddr:08X}" not in text
    for _, _, name, _ in B.CPU_ROUTINES:
        assert f"glabel {name}" in (B.ASM_DIR / B.source_of(int(name[5:], 16))).read_text()


def test_slot_bytes_refuse_a_source_that_outgrows_its_slot():
    assert B.trim_to_slot(bytes(28) + bytes(4), "func_8002C5E0.s") == bytes(28)
    with pytest.raises(ValueError, match="slot holds exactly 28"):
        B.trim_to_slot(bytes(28) + b"\x00\x00\x00\x01", "func_8002C5E0.s")
    with pytest.raises(ValueError, match="slot holds exactly 1536"):
        B.trim_to_slot(bytes(1532), "kernels.s")
