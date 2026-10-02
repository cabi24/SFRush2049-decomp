# A80 helper-only resource byte hash — frozen image MATCH

format_string_parse @ 0x800B466C..0x800B4720, 180 bytes / 45 words.
Source `group.c` SHA256:
`5352f376ad96994820b1c6e169a2a8ac272531481b0efce8d45912843352e829`.
Literal flags: `-g0 -O3 -mips2 -G 0 -non_shared`.

Fresh one-member actual group is 0/45 under canonical comparison, with exactly two
known own .rodata relocations (HI/LO at +0x20/+0x28). No other unresolved or unknown
references, extras, or errors. The explicit allow_unverified setting is confined
to this verified switch-table lead as authorized by the handoff/coordinator.

The existing canonical blob_group.group_bodies pipeline resolves and verifies the
original table and then reports 0 image word differences for the full helper.
Table: .rodata at 0x80123C20, six R_MIPS_32 entries into this function's own .text,
24 consumed bytes; object section is 32 bytes with trailing alignment padding.
The complete relocated 24 bytes independently equal the protected original image;
SHA256 `f6bcb0ccbee2571534ca6b32bdc230101a2d54ae46263f7873c8ff08698a567e`.
Sanitized fresh proofs are verification.json and image_verification.json.
The object has 48 text words, three trailing zero alignment words after the true
45-word body. No opcode/table bytes or object are included in this packet.

This is the genuine recovered arithmetic helper only: low-three-bit switch over
each actual unsigned byte, unsigned subtract/OR/AND/XOR/multiply/divide for 0..5,
add for other values. Each branch advances the consumed cursor once. No dummy
callers, mismatching A80 caller context, pressure locals, padding, seeded stub,
unused formals, or synthetic instructions. The caller remains a separate frozen
non-match with no claim. Coordinator owns final source/lock publication and ROM
SHA-1 gates; worker froze this packet without accepted-tree mutation.
