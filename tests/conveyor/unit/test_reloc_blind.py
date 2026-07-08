"""Relocation-blind score (`jobs/scoring.py`), contract invariants I1-I6.

The public `reloc_blind_score` shells out to the toolkit objdump, but the
algorithm is factored into three pure helpers (parse words, parse relocs,
masked diff). These tests build objdump-format fixture text and drive the
pure composition, so no objdump is needed — mirroring the contract's
"tests need no objdump" split (contracts/scoring-reloc-blind.md).
"""
from tools.conveyor.jobs import scoring


# --- objdump-format fixture builders ----------------------------------------

def _disasm(words):
    """`objdump -d` .text output for a list of 32-bit words."""
    lines = [
        "cand.o:     file format elf32-tradbigmips",
        "",
        "Disassembly of section .text:",
        "",
        "00000000 <f>:",
    ]
    for i, w in enumerate(words):
        lines.append("%8x:\t%08x \tnop" % (i * 4, w))
    lines.append("")
    return "\n".join(lines)


def _relocs(entries):
    """`objdump -r` output for [(offset, type_name), ...] in [.text]."""
    lines = [
        "",
        "RELOCATION RECORDS FOR [.text]:",
        "OFFSET   TYPE              VALUE",
    ]
    for offset, rtype in entries:
        lines.append("%08x %-17s %s" % (offset, rtype, "__osThreadQueue"))
    lines.append("")
    return "\n".join(lines)


def _blind(target_words, cand_words, reloc_entries):
    """Full pure composition: parse both fixtures + masked diff."""
    t = scoring._parse_text_words(_disasm(target_words))
    c = scoring._parse_text_words(_disasm(cand_words))
    sites = scoring._parse_relocs(_relocs(reloc_entries))
    return scoring._masked_diff(t, c, sites)


# The osCreateMesgQueue case from research D4: 12 words, identical except
# words 0-3 (two lui / two addiu) which the linker pair-patches for
# __osThreadQueue. HI16 relocs at offsets 0,4; LO16 at 8,12. In the target
# the low 16 bits hold resolved address halves; in the candidate they are 0.
_TARGET = [
    0x3C0E8003,  # 0: lui   t6, 0x8003     (HI16)
    0x3C0F8003,  # 1: lui   t7, 0x8003     (HI16)
    0x25CE1234,  # 2: addiu t6, t6, 0x1234 (LO16)
    0x25EF5678,  # 3: addiu t7, t7, 0x5678 (LO16)
    0xAFA40000,  # 4: sw    a0, 0(sp)
    0xAFA50004,  # 5: sw    a1, 4(sp)
    0x8DCE0000,  # 6: lw    t6, 0(t6)
    0x8DEF0000,  # 7: lw    t7, 0(t7)
    0x00000000,  # 8: nop
    0x03E00008,  # 9: jr    ra
    0x00000000,  # 10: nop
    0x00001025,  # 11: move  v0, zero
]
_CAND = [
    0x3C0E0000,  # 0: lui   t6, 0x0   (low 16 zeroed pre-link)
    0x3C0F0000,  # 1: lui   t7, 0x0
    0x25CE0000,  # 2: addiu t6, t6, 0
    0x25EF0000,  # 3: addiu t7, t7, 0
    0xAFA40000, 0xAFA50004, 0x8DCE0000, 0x8DEF0000,
    0x00000000, 0x03E00008, 0x00000000, 0x00001025,
]
_HI_LO = [(0, "R_MIPS_HI16"), (4, "R_MIPS_HI16"),
          (8, "R_MIPS_LO16"), (12, "R_MIPS_LO16")]


def test_i1_empty_relocs_is_word_equality():
    # True score 0 (identical words) => reloc-blind 0 when no relocs.
    assert _blind(_TARGET, list(_TARGET), []) == 0
    # A genuine word difference with no relocs is nonzero.
    diff = list(_TARGET)
    diff[5] ^= 0x1
    assert _blind(_TARGET, diff, []) > 0


def test_i2_masked_fields_at_reloc_sites_score_zero():
    # Streams equal except in the masked (low) fields at the four reloc sites.
    assert _blind(_TARGET, _CAND, _HI_LO) == 0


def test_i3_unmasked_bits_of_reloc_word_are_significant():
    # Different destination register in a HI16 lui: bits ABOVE the masked
    # low 16 differ, so masking does not hide it.
    cand = list(_CAND)
    cand[0] = 0x3C180000  # lui t8 instead of lui t6 (rt field differs)
    assert _blind(_TARGET, cand, _HI_LO) > 0


def test_i4_non_reloc_site_difference_is_significant():
    cand = list(_CAND)
    cand[6] = 0x8DCE0004  # lw t6, 4(t6) — a non-reloc word, changed
    assert _blind(_TARGET, cand, _HI_LO) > 0


def test_i5_length_mismatch_is_nonzero():
    assert _blind(_TARGET, _CAND[:-1], _HI_LO) > 0


def test_i6_unknown_reloc_type_compares_word_unmasked():
    # Same low-16 difference as I2 at index 2, but typed as an unknown reloc:
    # no normalization creep, so the difference survives.
    entries = [(0, "R_MIPS_HI16"), (4, "R_MIPS_HI16"),
               (8, "R_MIPS_32"), (12, "R_MIPS_LO16")]
    assert _blind(_TARGET, _CAND, entries) > 0


# --- parser-level checks (objdump text -> structured) -----------------------

def test_parse_text_words_reads_hex_column_only():
    words = scoring._parse_text_words(_disasm(_TARGET))
    assert words == _TARGET


def test_parse_text_words_skips_ellipsis_and_other_sections():
    text = (
        "Disassembly of section .text:\n"
        "\n"
        "00000000 <f>:\n"
        "   0:\t3c0e8003 \tlui\tt6,0x8003\n"
        "\t...\n"
        "   8:\t03e00008 \tjr\tra\n"
        "\n"
        "Disassembly of section .data:\n"
        "   0:\tdeadbeef \t.word 0xdeadbeef\n"
    )
    assert scoring._parse_text_words(text) == [0x3C0E8003, 0x03E00008]


def test_parse_relocs_offset_to_index_and_masks():
    sites = scoring._parse_relocs(_relocs(_HI_LO))
    assert sites == [(0, 0xFFFF0000), (1, 0xFFFF0000),
                     (2, 0xFFFF0000), (3, 0xFFFF0000)]
    r26 = scoring._parse_relocs(_relocs([(0x28, "R_MIPS_26")]))
    assert r26 == [(10, 0xFC000000)]
    unknown = scoring._parse_relocs(_relocs([(0x0, "R_MIPS_GPREL16")]))
    assert unknown == [(0, 0xFFFFFFFF)]


def test_parse_relocs_ignores_non_text_sections():
    text = (
        "RELOCATION RECORDS FOR [.data]:\n"
        "OFFSET   TYPE              VALUE\n"
        "00000000 R_MIPS_32         foo\n"
    )
    assert scoring._parse_relocs(text) == []
