# LEAN RESEARCH: eight-row menu callback (8010B7FC)

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. Native extent: 460 bytes / 115 words, frame 56.

The complete typed callback unhides its Blit, chooses the header/list fonts, draws eight centered rows with the selected color, advances the full-width y coordinate by the actual font height, and restores rendering mode.

## Observed result

IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared`; automatic `-Wab,-r4300_mul`:

- Generated seed body: **113/115 differing words**, 440-byte function, frame 64, two unverified owned-data sites, zero excess.
- Complete typed body standalone: **100/115**, 440-byte function, **native-sized frame 56**, no excess or unresolved/unverified sites/errors.
- Genuine font-context group: **100/115**, same extent/frame. It does not add a further score improvement.

The function is still 20 bytes short and plainly nonmatching. Earlier heads-A2 notes also reported 113/115 after fixing the unsigned width shift and float reset, but lacked the native font-return copies. This packet uses the real returning font wrapper and typed data views; it does not repeat the old volatile-local controls or claim those residuals are closed.

## Source and context

Baseline: `cloud/work/registered-heads/seeds/func_8010B7FC/group.c` at the frozen base, cut before its helper body so no generated stand-in is built. The candidate repairs the signed-halfword running-y truncation, signed width shift and integer-bit-pattern-as-float reset. Its width helper uses current accepted `s32 object_manager_update(u8 *, s16)`; the actual result is explicitly converted to unsigned before shifting, as native does. Rendering resets with `-1.0f`.

Blit hidden-byte access is +0x1A. The adjacent signed-halfword coordinates at D_80117280/+2 are a two-field view. The text root at D_8017A4E0 has a bank pointer at +12 and string-array pointer at +16; the bank's unsigned first-string index is at +0x70. These are evidenced object offsets, not inferred original struct names or stack padding. The row address preserves the actual `first+row+1` lookup. Valid row/string ranges and service objects remain assumptions.

`slot_context.c` is the actual frozen credits-scroll font body, normalized as in PR #215. `countdown_caller.c` is the unchanged complete actual caller from corrected PR #220. The group uses only real call sites, keeps both callbacks and internalizes the helper. All claims remain empty; context scores and hashes are included. The complete font/sound-update IPA contract is still unresolved.

## Minimal replay

With the project's IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_eight_row_callback_20261006/replay.py --repo .
```

Replay materializes frozen tools/baseline and protected targets/manifests in temporary storage, then compiles and prints metadata. No behavior/acceptance checks, live-source edits, locks, splice claims or ROM integration are added. Independent validation, acceptance and merging belong to the checker.
