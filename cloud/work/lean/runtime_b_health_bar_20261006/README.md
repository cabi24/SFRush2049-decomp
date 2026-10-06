# Image B health-bar callback: lean research

Target: `B:func_80392FE4`, `[0x80392FE4,0x80393518)`, 1,332 bytes.
Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

## Observed result and recipe

Standalone IDO 5.3 O3: **14 / 333 words differ**, zero extra words,
unresolved symbols, unverified relocations or comparison errors when scored
with the authenticated image-B data. The differences are confined to
scheduling around the first color branch. The initial complete reconstruction
with external threshold declarations differed in 256/333 words plus one extra.
Recovering the real float literals removed that alias/load artifact.

Actual flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
From the repository root with IDO available:

```
python3 cloud/work/lean/runtime_b_health_bar_20261006/reproduce.py
```

The helper only compiles and scores. It reads the frozen `assets/us/data.bin`,
authenticates the asset and complete image B in memory, and supplies that data
to the unchanged canonical scorer for the four owned float references. No
raw bytes, object or data dump is written to the repository. The ordinary
`tools/cloud/score.py fn` route alone reports these references unverified
because the frozen base does not contain `asm/us/ovl_b_data`.

## Required context and assumptions

The four thresholds at B:0x80394F54, 0x80394F58, 0x80394F5C and 0x80394F60
are binary32 encodings of 0.8, 0.6, 0.4 and 0.2. Source values were decoded
from image SHA-256 `b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd`.
Native input is one BLIT pointer. Player/segment are AnimID nibbles; valid
players use the 952-byte player record's signed-halfword value at +0x386,
scaled by 800. The callback centers the bar, updates its fill extent, and
sets paired bright/dark colors through green/yellow/red thresholds. Segment
zero updates a texture-state flag. An out-of-range player disables the
callback and returns Hidden's result. Other paths update the blit and return 1.

Existing NewMultiBlit/Hidden/UpdateBlit contracts establish the BLIT fields
and external helper prototypes. Names and record views are descriptive native
reconstructions; original declarations and a whole-function arcade donor are
not established. Valid player indices are 0..3, table selector 1..4, texture
indices must be backed, and float-to-byte/halfword conversions require finite
representable values for portable C behavior. Unknown fields preserve native
record storage. No pressure locals, fake callers, volatile or inline assembly.

This is observed source/score improvement, not an accepted match or ROM gain.
Only compile/scoring was performed. Independent checker owns further tests,
acceptance and integration.
