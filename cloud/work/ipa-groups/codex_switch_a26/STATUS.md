# A26 switch helper — frozen image MATCH follow-up

func_800B4B00 @ 0x800B4B00..0x800B4B94, 148 bytes / 37 words.
Source group.c SHA256:
`951a1c1c8f3bc82e90cde8d366e1246e56a86e5f1989353146535e9ad0729fec`.
Literal flags: -g0 -O3 -mips2 -G 0 -non_shared.
Original frozen tiny_A26 source and its hashes remain unchanged. This group's
actual C body is byte-for-byte the old body; only its line-1 O2 flag comment changes
to O3 to document the required group recipe. No reflow or pressure controls.

Fresh helper-only genuine group is 0/37 with exactly the original two known local
.rodata HI/LO references at +0x24/+0x2c. No extras or other verification exceptions.
The explicit allowed-unverified setting only carries this known switch-table lead.
The canonical blob_group private replay fully resolves and verifies the original
11-entry table, then reports 0 image word differences for all 37 body words.
Table .rodata at 0x80123CE4, 44 bytes used, object section48 bytes. All entries are
R_MIPS_32 references into the helper's own .text. Independently relocated bytes
equal the protected original table; SHA256:
`db0b41b6a0dce190225975c030829ba6be12073d672a86384b391053f0afc2c0`.
Object text is40 words: three trailing zero alignment words after the 37-word body.

Source follows the actual owner->object+44->resource->data chain. Selectors21..24
return signed bytes80..83;25 and all out-of-range values return0;26..31 return
signed bytes84..89. No stand-ins, fake callers, accepted context, unused formals,
new data ownership, or raw opcode/table artifacts. Verification metadata frozen
in verification.json and image_verification.json; coordinator owns ROM/lock gates.
