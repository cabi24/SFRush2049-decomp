# init_state_begin: independent native/compiler proof

The final commented source compiles to all 454 retail instructions exactly
(1,816 function bytes), including all external references and the complete
seven-entry generated switch table. The definitive proof is
`final_independent.json`. Cartridge coverage requires the root image/ROM gate.

## Source and ABI

Native entry is ordinary `void(void)`. The only external callee has ordinary
`void(Player *, u8 setting, s8 value)` ABI: its native argument homes and byte
conversions verify the two narrow types. The historical `audio_bus_mix` name
is misleading; its native implementation updates profile configuration,
recomputes checksums, and persists the changed region. The updater first
propagates 21 settings to each available player, then copies shared signed
configuration bytes, selects mode-specific settings, and applies the final
state mask. Seven physical native switch destinations correspond to cases
0 through 6 in ascending source order. Arcade settings searches found related
options logic, without a direct equivalent for this N64 function.

## Match progression

The new whole-function native reconstruction immediately reproduces the
correct 48-byte frame, all 454 instruction positions, and the complete native
switch layout. Both -O2 and -O3 initially have four real differences: the mode-6
store clearing D_8015F72C is scheduled after a following load, whereas native
places it before the D_80140B08 write. Reordering these two actual independent
stores, as native already does, closes all four words. No new runtime action,
local padding, volatile access, dummy argument, or custom instruction emission
is used.

## Independent final verification

Publication source SHA256:
`ce9074048228c2a88537233df8663281c921d47e514ddd45167bc2448e49a55c`.
Flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
Pinned toolkit:
`796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5`.

A fresh isolated Rocky compile runs the actual one-member whole-unit sequence:
cc -j, uld with the real keep list, usplit, umerge, uopt, ugen, and as1. The
protected existing blob_group relocation implementation resolves every body
reference and proves the generated table against the native image. An
additional independent calculation relocates all seven R_MIPS_32 entries and
compares their full 28 bytes against the table at 0x80123F98. Table SHA256:
`7de657b186dd07381fcf256bf32e205f639a0f3c9c29642dab2b160d7bd1029f`.
The four table alignment bytes and eight text alignment bytes are zero and
receive no coverage credit.

Canonical single-object scoring correctly reports its two section-relative
switch references as unverified. They are fully verified by the group table
proof; neither a masked score nor --allow-unverified authorizes this result.
Protected target object, raw listings, objects, and instruction arrays remain
only under ignored build/large_init_state/. No scorer or target was changed.
