# LEAN RESEARCH: per-player widget sizing/opacity (80108154)

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. Native target: 896 bytes / 224 words, frame 152.

This complete typed callback updates a per-player text-widget Blit: visibility, glyph-based width, coordinates, height, color pointer and alpha. It retains the native unsigned floating conversion in ordinary C rather than m2c's unimplemented FCSR macros.

## Observed compilation

IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared`; canonical automatic `-Wab,-r4300_mul`:

- Generated seed: **221/224 differing words**, 804-byte function, frame 112.
- Complete typed body standalone: 223/224, 860 bytes/frame 120.
- Same complete body with genuine font context: **167/224**, **888 bytes/frame 136**.

All three have zero excess and no unresolved/unverified sites or scorer errors. The retained group candidate is eight bytes short and its frame remains sixteen bytes short. It is research, not a match.

The baseline is frozen `cloud/work/registered-heads/seeds/func_80108154/group.c`, cut before the helper definition to remove generated stand-ins. It is explicitly **not a semantic reference**: `M2C_ERROR` replaces FCSR operations, the visibility test reads a byte where native reads a float, and other pointer/width declarations are guessed. The candidate has none of those placeholders.

## Actual source reconstruction

- Uses the real +0x2C player index, +0x28 callback-state word and +0x1A hidden byte; indices beyond player count clear/hide the Blit as native does.
- Preserves the short-circuit hide condition and float comparison from the actual per-player timer array, followed by the signed player-state byte at +0xEF of 952-byte records.
- Selects font 1 or 2 using the real returning gfx/font wrapper; measures glyphs 56 and 58, temporarily switches the byte-9 mode, and restores the returned old mode.
- Uses `(u32)((time * 192.0f) / 3.0f)` for the native unsigned conversion before the alpha-byte store, otherwise alpha 192.
- Keeps combined glyph width in a full-width integer for signed halving before the native halfword stores. The old seed truncated it prematurely.
- Uses actual coordinate pairs, RGBA records, pointer field +4, alpha byte +0x18 and the other evidenced Blit halfword offsets.

Opaque global record spans express native object layout, not stack padding. Assumptions are a nonnegative valid player index, sufficient array extents, valid glyph/font objects, and finite intended timer values whose scaled unsigned conversion is representable. Original struct/field names and behavior outside the native valid runtime domain are not claimed recovered.

`slot_context.c` is the actual frozen credits-scroll font body normalized as in PR #215. `countdown_caller.c` is the unchanged actual callback from corrected PR #220, providing genuine additional font call sites. The group keeps real roots, internalizes the font helper and retains empty claims. Context scores/hashes are included; full font/sound-update IPA context and the remaining frame layout remain unresolved.

## Minimal replay

With project IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_widget_opacity_20261006/replay.py --repo .
```

The replay materializes frozen tools, baseline and protected target/manifests in temporary storage, compiles the controls and reports metadata. No behavior/acceptance checks, invented locals, volatile pressure, live-source changes, locks, splice claims or ROM integration are added. Independent verification, acceptance and merging belong to the checker.
